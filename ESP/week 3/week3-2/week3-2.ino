#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"
#include "secret.h"
#define GPIO_LED 12
#define GPIO_BUTTONL 25
#define GPIO_BUTTONR 26

const char* ntpServer = "nl.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// Vlag om aan te geven of de tijd correct is gesynchroniseerd
volatile bool timeSynced = false;

//const variable for timestamp
const int glob_buf_size = (64 * sizeof(char));
char *glob_time_buf;
char currentTime[64];

//nogwat
volatile int wifi_delay_counter = 0;
volatile int ntp_delay_counter = 0;
volatile int press_task_counter = 0;
volatile int debounce_left_task_counter = 0;
volatile int debounce_right_task_counter = 0;
static int MAX_COUNTER_VALUE = 5;
static QueueHandle_t queue;
static const int reset_press = -998;
static QueueHandle_t audit_queue;

//wifi
void WiFi_connect(){
  Serial.println("WiFi_connect: Before WiFi.begin");
  WiFi.begin(ssid, password);
  Serial.println("WiFi_connect: After WiFi.begin");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi_connect: Before delay");
    delay(500);
    Serial.println("WiFi_connect: After delay");
    printf(".");
  }
  printf(" CONNECTED\r\n");
  vTaskDelete()
}


// connect with NTP server
void SNTP_connect(void *argp){
  const time_t old_past = 1577836800; // 2020-01-01T00:00:00Z printf("\r\nConnect to SNTP server\n");
  printf("\r\nConnect to SNTP server\n");

  // init time protocol sync
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();
  setenv("TZ", "CET-1CEST-2, M3.5.0/02:00:00, M10.5.0/03:00:00", 1);
  //https://knowledgebase.progress.com/articles/Article/P129473 
  tzset();

  bool connected = false;
  while (not connected ) {
  Serial.println("SNTP_connect: Before delay");
  delay(500);
  Serial.println("SNTP_connect: After delay");
  time_t now;
  if (time(&now) < old_past ) { // if there is no connection with NTP server than local time returns Unix epoch (1970 date)
    printf(".");
  }
  else {
    printf(" CONNECTED\r\n");
    connected = true;
    // Tijd is gesynchroniseerd met NTP-server
    timeSynced = true;
  }
 }
}

// get current time
char* getLocalTime() {
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/apireference/system/systime_t now;
  char *time_string = (char*)malloc(64 * sizeof(char));
  struct tm timeinfo;
  time_t now;
  time(&now);
  localtime_r(&now, &timeinfo);
  strftime(time_string, (64 * sizeof(char)), "%c", &timeinfo);
  return time_string;
}

//
// Button Debouncing task:
//
static void debounce_task(void *argp) {

  unsigned button_gpio = *(unsigned *)argp;
  uint32_t level, state = 0;
  uint32_t mask = 0x7FFFFFFF;
  int event, last = -999;

  for (;;) {
    if ( button_gpio== GPIO_BUTTONR ) { 
      debounce_right_task_counter++;
      if ( debounce_right_task_counter < MAX_COUNTER_VALUE ) {
        printf("\t\tin debounce_task(): RIGHT start %d\n", debounce_right_task_counter ); 
      } 
    } 
    else if ( button_gpio== GPIO_BUTTONL ) { 
    debounce_left_task_counter++;
    if ( debounce_left_task_counter < MAX_COUNTER_VALUE ) { 
      printf("\t\tin debounce_task(): LEFT start %d\n", debounce_left_task_counter );
      }
    }

    level = !digitalRead(button_gpio);
    state = (state << 1) | level;
    if ((state & mask) == mask)
      event = button_gpio;  // Press
    else
      event = -button_gpio;  // Release

    if (event != last) {
      if (xQueueSendToBack(queue, &event, 0) == pdPASS) {
        last = event;
      } else if (event < 0) {
        // Queue full, and we need to send a
        // button release event. Send a reset_press
        // event.
        do {
          xQueueReset(queue);  // Empty queue
        } while (xQueueSendToBack(queue, &reset_press, 0) != pdPASS);
        last = event;
      }
    }
    if ( button_gpio== GPIO_BUTTONR ) { 
      debounce_right_task_counter++;
      if ( debounce_right_task_counter < MAX_COUNTER_VALUE ) {
        printf("\t\tin debounce_task(): RIGHT start %d\n", debounce_right_task_counter ); 
      } 
    } 
    else if ( button_gpio== GPIO_BUTTONL ) { 
      debounce_left_task_counter++;
      if ( debounce_left_task_counter < MAX_COUNTER_VALUE ) { 
        printf("\t\tin debounce_task(): LEFT start %d\n", debounce_left_task_counter );
      }
    }

    taskYIELD();
  }
}


//
// Hydraulic Press Task (LED)
//
static void press_task(void *argp) {
  static const uint32_t enable = (1 << GPIO_BUTTONL)
                                 | (1 << GPIO_BUTTONR);
  BaseType_t s;
  BaseType_t t;
  int event;
  uint32_t state = 0;
  char* currentTime;
  char* timestamp;
  currentTime = (char *)malloc(64);
  char waiting;
  char capacity;
  // Make sure press is OFF
  digitalWrite(GPIO_LED, LOW);

  for (;;) {
    s = xQueueReceive(
      queue,
      &event,
      portMAX_DELAY);
    assert(s == pdPASS);

    if (event == reset_press) {
      digitalWrite(GPIO_LED, LOW);
      state = 0;
      printf("RESET!!\n");
      continue;
    }

    if (event >= 0) {
      // Button press
      state |= 1 << event;
    } else {
      // Button release
      state &= ~(1 << -event);
    }

    // Wacht op de juiste tijd van de NTP-server voordat de LED wordt geactiveerd
    while (!timeSynced) {
      delay(1000); // Wacht 1 seconde
    }

    if (state == enable) {
      // Activate press when both
      // Left and Right buttons are
      // pressed.
      digitalWrite(GPIO_LED, HIGH);
      int available_spaces = uxQueueSpacesAvailable(audit_queue);
      printf("   available spaces in queue: %d", available_spaces);
      if (uxQueueSpacesAvailable(audit_queue) == 1){
        printf("\n");
        printf("Deleted time: ");
        t = xQueueReceive(audit_queue, &timestamp, 0);
        printf(timestamp);
      }
      currentTime = getLocalTime();
      printf("\n");
      printf("Added time: ");
      printf(currentTime);
      xQueueSendToBack(audit_queue,&currentTime,0);
    } else {
      // Deactivate press
      digitalWrite(GPIO_LED, LOW);
    }
  }
  if ( press_task_counter < MAX_COUNTER_VALUE ) {
  printf("\t\tin press_task(): end %d\n", press_task_counter );
  } 
}


void setup() {
  // allocate heap memory to store 1 timestap
  glob_time_buf = (char*)malloc(glob_buf_size);
  // check if memory is allocated
  assert( glob_time_buf != NULL );
 
  int app_cpu = xPortGetCoreID();
  static int left = GPIO_BUTTONL;
  static int right = GPIO_BUTTONR;
  TaskHandle_t h;
  BaseType_t rc;

  delay(2000);  // Allow USB to connect
  queue = xQueueCreate(2, sizeof(int));
  assert(queue);

  audit_queue = xQueueCreate(200, (64 * sizeof(char)));
  assert(audit_queue);

  pinMode(GPIO_LED, OUTPUT);
  pinMode(GPIO_BUTTONL, INPUT_PULLUP);
  pinMode(GPIO_BUTTONR, INPUT_PULLUP);

  printf("Starting Task SNTP_connect\n");
  rc = xTaskCreatePinnedToCore(
    SNTP_connect,
    "SNTP_connect",
    2024,     // Stack size
    nullptr,  // Not used
    3,        // Priority
    &h,       // Task handle
    app_cpu   // CPU
  );
  assert(rc == pdPASS);
  assert(h);
  printf("Stopping Task SNTP_connect\n");

  printf("Starting Task WiFi_connect\n");
  rc = xTaskCreatePinnedToCore(
      WiFi_connect,
     "WiFi_connect",
      2024,
      nullptr,
      2,
      &h,
      app_cpu
  );
  assert(rc == pdPASS);
  assert(h);
  printf("Stopping Task WiFi_connect\n");

  printf("Starting Task debounceL\n");
  rc = xTaskCreatePinnedToCore(
    debounce_task,
    "debounceL",
    2048,    // Stack size
    &left,   // Left button gpio
    1,       // Priority
    &h,      // Task handle
    app_cpu  // CPU
  );
  assert(rc == pdPASS);
  assert(h);
  printf("Stopping Task debounceL\n");

  printf("Starting Task debounceR\n");
  rc = xTaskCreatePinnedToCore(
    debounce_task,
    "debounceR",
    2048,    // Stack size
    &right,  // Right button gpio
    1,       // Priority
    &h,      // Task handle
    app_cpu  // CPU
  );
  assert(rc == pdPASS);
  assert(h);
  printf("Stopping Task debounceR\n");

  printf("Starting Task led\n");
  rc = xTaskCreatePinnedToCore(
    press_task,
    "led",
    2048,     // Stack size
    nullptr,  // Not used
    1,        // Priority
    &h,       // Task handle
    app_cpu   // CPU
  );
  assert(rc == pdPASS);
  assert(h);
  printf("Stopping Task led'\n");

}

void loop() {


}