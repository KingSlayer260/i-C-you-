#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"

// data below is set as a define in the file secret.h
#include "secret.h"
//const char* ssid = "HANZE-XX";
//const char* password = "AXAXAXA";

//const voor sntp client
const char* ntpServer = "nl.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

//const variable for timestamp
const int glob_buf_size = (64 * sizeof(char));
char *glob_time_buf;

void WiFi_connect(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    printf(".");
  }
  printf(" CONNECTED\r\n");
}

// connect with NTP server
void SNTP_connect(){
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
  delay(500);
  time_t now;
  if (time(&now) < old_past ) { // if there is no connection with NTP server than local time returns Unix epoch (1970 date)
    printf(".");
  }
  else {
    printf(" CONNECTED\r\n");
    connected = true;
  }
 }
}

// get current time
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html
void getLocalTime(char * time_buf, int time_buf_size) {
    time_t now;
    struct tm timeinfo; 
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(time_buf, time_buf_size, "%c", &timeinfo);
    return ;
}

void setup(){
  WiFi_connect();
  printf("deze shit werkt");

  SNTP_connect();
  // allocate heap memory to store 1 timestap
  glob_time_buf = (char*)malloc(glob_buf_size);
  // check if memory is allocated
  assert( glob_time_buf != NULL );
}

void loop(){
  printf("deze shit werkt");
  getLocalTime(glob_time_buf, 64);
  printf("Tijd = %s \n", glob_time_buf);
}