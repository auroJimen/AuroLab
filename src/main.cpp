#include <Arduino.h>
#include <M5Cardputer.h>
#include <SD.h>
#include <M5GFX.h>
#include "GUI.h"

//Global variables
GUI_Class GUI;
//In the future put here the rest of the object instances to control all periferals


// Function declarations
void GUIloop(void* parameter);

void setup() {
  //Serial for monitoring
  Serial.begin(115200);
  Serial.println("Hola hola");
  // Basic setup to initialize the Cardputer object defined in M5's libraries
  auto cfg = M5.config();              //Creates a cfg struct as defined in M5Unified
  M5Cardputer.begin(cfg, true);       //Initialises M5Cardputer object with all the correct pointers stored
  xTaskCreatePinnedToCore(GUIloop, "GUI thread", 10000, NULL, 0, &GUI.task, 0); //Creates thread for the GUI code on core 0

}

void GUIloop(void* parameter){
  //This code runs on a separate core
  GUI.begin();
  GUI.drawMainMenu();
  GUI.mainLoop();

}

void loop() {
  // put your main code here, to run repeatedly:
  
  
}