//GUI file, contains all functions needed for our GUI to work properly
#include "GUI.h"

//icon functions

iconBat::iconBat(M5GFX& parent, int perc){
    //Constructor
    this->screen = M5GFX(parent);
    this->percent = perc;

    //Initialize the bat drawing the base sprite
    this->screen.pushImage(basePos.x, basePos.y, baseSize.x, baseSize.y, batIconBase);
    this->screen.setCursor(textPos.x,textPos.y);
    this->screen.setTextSize(textSize);
    this->updatePercent(perc);

}

int chargeLevel(int perc){
    int result = (int)ceilf(12.0*(float)perc/100.0);
    if (result==12) result =11;
    //Serial.println(result);
    return result;

}

void iconBat::testAnim(int mil){

    for (int i = 0; i <= 100; i++){
        
        this->screen.pushImage(contPos.x, contPos.y, contSize.x, contSize.y, batIcons[chargeLevel(i)]);
        this->screen.setCursor(textPos.x,textPos.y);
        this->screen.setTextSize(textSize);
        this->updatePercent(i);
        delay(mil);
    }

}

void iconBat::updatePercent(int perc){
    //Delete current text
    this->screen.setTextColor(BLACK);
    this->screen.setCursor(textPos.x,textPos.y);
    this->screen.printf("%3d%%", percent);
    //Update saved text
    percent = perc;
    //Print saved text
    this->screen.setTextColor(GREEN);
    this->screen.setCursor(textPos.x,textPos.y);
    this->screen.printf("%3d%%", percent);

}

int iconBat::changeState(int percent){

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

    iconBat test = iconBat(disp, 1);
    while(1) {  
        test.testAnim(200);     
        delay(200);
    };
    //this->bar = topBar(this->Display);
}

void GraphicalUI::mainLoop(){
    //MainLoop for the GUI, handles drawing the UI elements, reacts to inputs etc.
}

void GraphicalUI::loadConfFile(){
    //Loads all saved documentation from the SD card (if present) b4 starting up the GUI
}

