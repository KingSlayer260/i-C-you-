// press2.ino 
// MIT License (see file LICENSE)

#include "time.h"

// LED is active high
#define GPIO_LED_SEM 12 
#define GPIO_BUTTONL  25
#define GPIO_BUTTONR  26
#define DEBOUNCE_DELAY  30 // the debounce time in millisecond, increase this time if it still chatters

volatile bool button1State = false;
volatile bool button2State = false;
volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
volatile unsigned long lastInterruptTime1 = 0;
volatile unsigned long lastInterruptTime2 = 0;

//Declare SemaphoreHandle
SemaphoreHandle_t semHandle;

// button interrupt handlers
// button interrupt handlers
void IRAM_ATTR handleInterruptButton1() {
  unsigned long interruptTime = millis();
  // If interrupts come faster than DEBOUNCE_DELAY, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime1 > DEBOUNCE_DELAY) {
    button1State = digitalRead(GPIO_BUTTONL);
    if (button1State == LOW) {
      // Set a flag here instead of calling blink_sem_led
      button1Pressed = true;
    }
  }
  lastInterruptTime1 = interruptTime;
}

// button interrupt handlers
void IRAM_ATTR handleInterruptButton2() {
  unsigned long interruptTime = millis();
  // If interrupts come faster than DEBOUNCE_DELAY, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime2 > DEBOUNCE_DELAY) {
    button2State = digitalRead(GPIO_BUTTONL);
    if (button2State == LOW) {
      // Set a flag here instead of calling blink_sem_led
      button2Pressed = true;
    }
  }
  lastInterruptTime2 = interruptTime;
}




void blink_sem_led(int rate) {
    //set Semaphore and when done relase the Semaphore
    if(xSemaphoreTake(semHandle, portMAX_DELAY)) {
        printf("BLINK_SEM_LED, rate: %d\r\n", rate);

        for(int count = 0; count < 10; count++) {
            digitalWrite(GPIO_LED_SEM, HIGH);
            delay(rate);
            digitalWrite(GPIO_LED_SEM, LOW);
            delay(rate);
        }

        //free Semaphore
        xSemaphoreGive(semHandle);
    }
}


//
// Initialization:
//
void setup() {
  int app_cpu = xPortGetCoreID();
  static int left = GPIO_BUTTONL;
  static int right = GPIO_BUTTONR;
  
  delay(2000);          // Allow USB to connect

  // create here binary Semaphore and release the Semaphore

  semHandle = xSemaphoreCreateBinary();
  xSemaphoreGive(semHandle); // Release the semaphore

 
  pinMode(GPIO_LED_SEM,OUTPUT);
  pinMode(GPIO_BUTTONL,INPUT_PULLUP);
  pinMode(GPIO_BUTTONR,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(GPIO_BUTTONL), handleInterruptButton1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(GPIO_BUTTONR), handleInterruptButton2, CHANGE);

}

// Not used
void loop() {
// Check if button1 was pressed
  if (button1Pressed) {
    blink_sem_led(200);
    button1Pressed = false;
  }
    if (button2Pressed) {
    blink_sem_led(400);
    button2Pressed = false;
  }
}
