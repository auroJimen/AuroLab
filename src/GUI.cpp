//GUI file, contains all functions needed for our GUI to work properly
#include "GUI.h"
//In the future put here the rest of the object instances to control all periferals

//GUI functions
void GraphicalUI::splashScreen(){
    this->Display.clearDisplay(BLACK);
    this->Display.println("Prueba");
    int32_t width = this->Display.width();
    int32_t height = this->Display.height();
    this->Display.pushImage(0,0,240,135, splashscreen);
    return;
}

void drawMainMenu(){
    
}

