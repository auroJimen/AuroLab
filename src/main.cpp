#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>
#include "GUI.h"

//Global variables
GraphicalUI GUI;
//In the future put here the rest of the object instances to control all periferals


// Function declarations
int myFunction(int, int);

void setup() {
  // Basic setup to initialize the Cardputer object defined in M5's libraries
  auto cfg = M5.config();              //Creates a cfg struct as defined in M5Unified
  M5Cardputer.begin(cfg, true);       //Initialises M5Cardputer object with all the correct pointers stored
  int result = myFunction(2, 3);
  GUI.splashScreen();

}

void loop() {
  // put your main code here, to run repeatedly:
}

// Function definitions
int myFunction(int x, int y) {
  return x + y;
}