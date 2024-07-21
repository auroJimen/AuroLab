#include <Arduino.h>
#include <M5Cardputer.h>
#include <SD.h>
#include <M5GFX.h>
#include "GUI.h"

//Global variables
GraphicalUI GUI;
//In the future put here the rest of the object instances to control all periferals


// Function declarations
int myFunction(int, int);

void setup() {
  //Serial for monitoring
  Serial.begin(115200);
  Serial.println("Hola hola");
  // Basic setup to initialize the Cardputer object defined in M5's libraries
  auto cfg = M5.config();              //Creates a cfg struct as defined in M5Unified
  M5Cardputer.begin(cfg, true);       //Initialises M5Cardputer object with all the correct pointers stored
  GUI.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  GUI.drawMainMenu();
  
}

// Function definitions
int myFunction(int x, int y) {
  return x + y;
}