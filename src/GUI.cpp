//GUI file, contains all functions needed for our GUI to work properly
#include "GUI.h"

//icon functions

icon::icon(M5GFX& parent, iconType type, int state){
    //Constructor
    this->sprite = M5GFX(parent);
    this->type = type;
    this-> state = state;

    //Itiliaze the sprite properly depending on the type
    switch (type){
        case Bat:
        this->x = 209;
        this->y = 18;
        this->sprite.pushImage(this->x, this->y, 26, 2, batIconTest);
        //this->sprite.createFromBmp(batIconTest)
        break;
        case Wifi:
        case BLE:
        case SDpresent:
        break;
    }

}

//GUI functions

void GraphicalUI::begin(){

    this->Display.setBaseColor(BLACK);
    this->Display.clearDisplay(BLACK);
    this->Display.setTextSize(2.5);
    this->Display.setTextDatum(textdatum_t::top_left);
    this->Display.setTextColor(GREEN);
    this->Display.pushImage(0,0,240,135, splashscreen);
    this->Display.setCursor(15,10);
    this->Display.print("Loading");
    delay(200);
    this->Display.print(".");
    delay(200);
    this->Display.print(".");
    delay(200);
    this->Display.print(".");
    delay(200);

}

void GraphicalUI::drawMainMenu(){
    //Draws the main menu
    M5GFX disp = this->Display;
    disp.pushImage(0,0,240,135, menuBackground);

    icon test = icon(disp, Bat, 1);
    while(1) {
        delay(10);
    };
    //this->bar = topBar(this->Display);
}

void GraphicalUI::mainLoop(){
    //MainLoop for the GUI, handles drawing the UI elements, reacts to inputs etc.
}

void GraphicalUI::loadConfFile(){
    //Loads all saved documentation from the SD card (if present) b4 starting up the GUI
}

