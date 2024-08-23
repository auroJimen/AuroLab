#include <Arduino.h>
#include <M5Cardputer.h>
#include <SD.h>
#include <M5GFX.h>
#include "GUI.h"

//Global variables
GUI_Class GUI;
bool keyBoardEnable;
xTaskHandle keyBoardTask;
//In the future put here the rest of the object instances to control all periferal


// Function declarations
void GUIloop(void* parameter);
void keyBoardLoop(void *parameter);
void keyBoardISR();

void setup() {
  //Serial for monitoring
  Serial.begin(115200);
  // Basic setup to initialize the Cardputer object defined in M5's libraries
  auto cfg = M5.config();              //Creates a cfg struct as defined in M5Unified
  M5Cardputer.begin(cfg, true);       //Initialises M5Cardputer object with all the correct pointers stored
  BaseType_t gui = xTaskCreatePinnedToCore(GUIloop, "GUI thread", 10000, NULL, 0, &GUI.task, 0); //Creates thread for the GUI code on core 0
  if (gui != pdPASS) log_i("ERR");
  keyBoardEnable = true;
  BaseType_t key = xTaskCreatePinnedToCore(keyBoardLoop, "Keyboard thread", 10000, NULL, 0, &keyBoardTask, 1); //Creates thread for the keyboard on core 1
  if (key != pdPASS) log_i("ERR");
}

void GUIloop(void* parameter){
  //This code runs on a separate core
  log_i("GUI task created");
  GUI.begin();
  GUI.drawMainMenu();
  GUI.mainLoop();

}

void keyBoardLoop(void* parameters) {
  log_i("Keyboard task created");
  esp_intr_alloc(digitalPinToInterrupt(1), ESP_INTR_FLAG_SHARED, keyBoardISR(), )
  //Create the interrupt we'll use to time how many times per second we check the keyboard input
  hw_timer_t *timer =  timerBegin(2, 80, true); //Create timer on core 1 (timers are 0,1,2,3 2 per core)
  timerAttachInterrupt(timer, &keyBoardISR, true); //Atach interrupt to the timer
  timerAlarmWrite(timer, 1000000, true); //Set the alarm that triggers the interrupt
  timerAlarmEnable(timer); //Enable the alarm
  //Suspend the current task (it'll resume from here once the interrupt is triggered)
  vTaskSuspend(NULL);
  //Infinite loop, each time xTaskResumeFromISR is called from within the ISR, a round of this loop will
  //be executed, succesive calls to resuming won't affect an already resumed task, & the loop suspends the task
  //once the round is finished
  for(;;){
    //Handle keyboard presses
    //...
    log_i("Keyboard pressed");
    //Suspend itself until it's called again
    vTaskSuspend(NULL);
  }
}

void IRAM_ATTR keyBoardISR(){
  //log_i("Interrupt triggered");
  M5Cardputer.update();
  if (keyBoardEnable && M5Cardputer.Keyboard.isChange()) xTaskResumeFromISR(keyBoardTask);
}


void loop() {
  // Debuggin what's running in which core
  TaskHandle_t  core0 = xTaskGetCurrentTaskHandleForCPU(0);
  String core0Task = pcTaskGetName(core0);
  TaskHandle_t  core1 = xTaskGetCurrentTaskHandleForCPU(1);
  String core1Task = pcTaskGetName(core1);
  String data = "\nCore 0 is running: " + core0Task + "\nCore 1 is running: " + core1Task+"\n\n";
  char buffer[400];
  char* ptr = buffer;
  data.toCharArray(buffer, 400);
  log_printf(buffer);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  
}