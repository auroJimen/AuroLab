#include <Arduino.h>
#include <M5Cardputer.h>
#include <SD.h>
#include <M5GFX.h>
#include "GUI/GUI.h"

#define LOG_ENABLE false

//Global variables
GUI_Class GUI;
buffer_Class Buffer;
//In the future put here the rest of the object instances to control all periferal


void setup() {
  //Serial for monitoring
  Serial.begin(115200);
  // Basic setup to initialize the Cardputer object defined in M5's libraries
  auto cfg = M5.config();              //Creates a cfg struct as defined in M5Unified
  M5Cardputer.begin(cfg, true);       //Initialises M5Cardputer object with all the correct pointers stored
  GUI.begin();                        //Initialises GUI in its separate execution thread
}

void loop() {
  if (CORE_DEBUG_LEVEL >= 5 && LOG_ENABLE){
    // Debuggin what's running in which core
    TaskHandle_t  core0 = xTaskGetCurrentTaskHandleForCPU(0);
    int pri0 = uxTaskPriorityGet(core0);
    String core0Task = pcTaskGetName(core0);
    TaskHandle_t  core1 = xTaskGetCurrentTaskHandleForCPU(1);
    String core1Task = pcTaskGetName(core1);
    int pri1 = uxTaskPriorityGet(core1);
    eTaskState keyboardState = eTaskGetState(Buffer.getTaskHandle());
    int priKey = uxTaskPriorityGet(Buffer.getTaskHandle());
    String data = "\nRunning:\nCore0->" + core0Task + " p="+pri0+"\nCore1-> " + core1Task + " p="+pri1+"\nKeboard task state: "+keyboardState+"\nKeyboard task pri: "+priKey+"\nKeyboard input: "+Buffer.keyboardEnable+"\n";
    log_i("%s", data.c_str());
  }
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  
}

//I'm moving the GUIloop to here so thtat the gui class can better serve as a 
//GUI functions library intended to be called from different apps ans such
void GUIloop() {

}