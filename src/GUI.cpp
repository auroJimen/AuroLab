//GUI file, contains all functions needed for our GUI to work properly
#include "GUI.h"
#include "sprites.h"
//In the future put here the rest of the object instances to control all periferals

//GUI functions
void GraphicalUI::splashScreen(){
    //Clears the display and pushes the splashscreen to it for 1 s
    this->Display.clearDisplay(BLACK);
    this->Display.pushImage(0,0,240,135, splashscreen);
    delay(1000);
    return;
}

void GraphicalUI::drawMainMenu(){
    //Draws the main menu
}

void GraphicalUI::mainLoop(){
    //MainLoop for the GUI, handles drawing the UI elements, reacting to inputs etc.
}

void GraphicalUI::loadConfFile(){
    //Loads all saved documentation from the SD card (if present) b4 starting up the GUI
}

