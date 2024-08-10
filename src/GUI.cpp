//GUI file, contains all functions needed for our GUI to work properly
#include "GUI.h"

//cord functions
coord::coord(){};
coord::coord(int a, int b) {
    x = a;
    y = b;
}
coord coord::add(coord a){
    return coord(x+a.x, y+a.y);
}

//icon functions

icon_Class::icon_Class(){}

icon_Class::icon_Class(M5GFX screen, iconType type, int state){

}


//iconBat functions
iconBat_Class::iconBat_Class(){};

iconBat_Class::iconBat_Class(M5GFX& display, int perc){
    
    this->screen = M5GFX(display);
    this->percent = perc;

    //Initialize the bat drawing the base sprite
    this->screen.pushImage(basePos.x, basePos.y, baseSize.x, baseSize.y, batIconBase);
    updateState(perc);

}

iconBat_Class::iconBat_Class(M5GFX& display){
    //Constructor
    this->screen = M5GFX(display);

    //Initialize the bat drawing the base sprite
    this->screen.pushImage(basePos.x, basePos.y, baseSize.x, baseSize.y, batIconBase);
    updateState();

}

int iconBat_Class::chargeLevel(int perc){
    int result = (int)ceilf(12.0*(float)perc/100.0);
    if (result>=12) result =11;
    //Serial.println(result);
    return result;

}

void iconBat_Class::testAnim(int mil){

    for (int i = 0; i <= 100; i++){
        
        this->screen.pushImage(contPos.x, contPos.y, contSize.x, contSize.y, batIcons[chargeLevel(i)]);
        this->updatePercent(i);
        delay(mil);
    }

}

void iconBat_Class::updatePercent(int perc){
    if (perc > 100) perc = 100;
    //Delete current text
    this->screen.setTextColor(BLACK);
    this->screen.setCursor(textPos.x,textPos.y);
    this->screen.setTextSize(textSize);
    this->screen.printf("%3d%%", percent);
    //Update saved text
    percent = perc;
    //Print saved text
    this->screen.setTextColor(GREEN);
    this->screen.setCursor(textPos.x,textPos.y);
    this->screen.setTextSize(textSize);
    this->screen.printf("%3d%%", percent);

}

void iconBat_Class::updateState(int perc){
    if (perc > 100) perc = 100;
    if (perc != this->percent) {
    //Updates shown battery percent
    updatePercent(perc);
    //Updates charge level
    updateChargeLevel(perc);

    }

}

void iconBat_Class::updateState(){

    int perc = fetchBatLevel();
    updateState(perc);

}

void iconBat_Class::updateChargeLevel(int perc){
    if (perc >= 100) perc = 100;
    int level = chargeLevel(perc); //0-11 level
    this->screen.pushImage(basePos.x, basePos.y, baseSize.x, baseSize.y, batIconBase);
    this->screen.pushImage(contPos.x, contPos.y, contSize.x, contSize.y, batIcons[level]);
}

int iconBat_Class::fetchBatLevel(){
    //Fetches current battery level from the Power class
    return (int)M5Cardputer.Power.getBatteryLevel();

}

//topBar_Class functions

topBar_Class::topBar_Class(){
    //Constructor
    M5GFX disp = this->Display;
    this->Bat = iconBat_Class(disp);
    this->Wifi = icon_Class(disp, iconType::Wifi, false);
    this->BLE = icon_Class(disp, iconType::BLE, false);
    this->SDpresent = icon_Class(disp, iconType::SDpresent, false);

}

void topBar_Class::updateIcons(){
    //Updates the status of all topbar icons

    this->Bat.updateState();

}

//list_Class functions
list_Class::list_Class(String title, int elementNum, String* elementName, void (*handler)(int, int), 
coord origin, coord size, coord textPos, M5Canvas *sprite, float titleSize, float textSize, 
int textColour, int backColour, 
int highlightColour){

    this->title = title;
    this->elementNum = elementNum;
    this->elementName = elementName;
    this->handler = handler;

    this->origin = origin;
    this->size = size;
    this->textPos = textPos;
    this->titleSize = titleSize;
    this->textSize = textSize;
    this->textColour = textColour;
    this->backColour = backColour;
    this->highlightColour = highlightColour;

    this->pos = 0;

    this->sprite = sprite;
}

void list_Class::draw(){

    //Create the sprite in RAM
    (*this->sprite).createSprite(this->size.x, this->size.y);

    //Draw backgroun rectangle
    (*this->sprite).setBaseColor(this->backColour);
    (*this->sprite).clear();
    (*this->sprite).setColor(this->textColour);
    (*this->sprite).drawRoundRect(0, 0, this->size.x, this->size.y, 3);
    
    //Set text options
    //coord absTextPos = origin.add(textPos);
    (*this->sprite).setCursor(textPos.x, textPos.y);
    //this->Display.setTextPadding(absTextPos.x); //This feels kinda bad
    (*this->sprite).setTextColor(this->textColour);
    (*this->sprite).setTextSize(this->titleSize);
    (*this->sprite).println(title);

    //Need aux function to dinamically determine how many rows fit on the rectangle from text size

    //For loop to draw the options (taking current pos into account)
    (*this->sprite).setTextSize(this->textSize);
    for(int i = pos; i <this->elementNum; i++){
        (*this->sprite).println(this->elementName[i]);
    }

    (*this->sprite).pushSprite(this->origin.x, this->origin.y);
    
}

//GUI functions

void GUI_Class::begin(){

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
    this->Display.clear();

    this->topBar = topBar_Class();


}

void testHandler(int a, int b) {return;};

void GUI_Class::drawMainMenu(){
    //Draws the main menu
    M5GFX disp = this->Display;
    disp.pushImage(0,0,240,135, menuBackgroundTest);
    //disp.pushImage(90,41, 58, 74, terminalIcon);
    //disp.setColor(BLACK);
    //disp.fillRoundRect(50, 30, 140, 90, 3);
    //disp.setColor(GREEN);
    //disp.drawRoundRect(50, 30, 140, 90, 3);
    String elements[] = {"Hola", "Holaa", "Holaaa", "Holaaaa", "Holaaaaa", "Holaaaaaa", "Holaaaaaaa"};
    String *ref = elements;
    M5Canvas sprite;
    list_Class test = list_Class(String("Test"), 7, ref,  &testHandler, coord(50,30), coord(140,90), coord(5, 5), &sprite);
    test.draw();
}

void GUI_Class::mainLoop(){
    //MainLoop for the GUI, handles drawing the UI elements, reacts to inputs etc.
    for(;;){

        this->topBar.updateIcons();
        delay(2000);

    }
}

void GUI_Class::loadConfFile(){
    //Loads all saved documentation from the SD card (if present) b4 starting up the GUI
}

void GUI_Class::drawWifiMenu(){
    //Prints available wifi networks to allow selection & connection

    //Starts async scan
    WiFi.scanNetworks(true, true);
    //Check if scan has finished
    while(WiFi.scanComplete() < 0) {
        //Print something to show scan is in progress
        delay(20);
    }

    //Get number of available networks
    int availableNetworks = WiFi.scanComplete();

    for(int i = 0; i < availableNetworks; i++){
        //Print the ssids here in a scrollable format? 
    }

    //Wait for input loop (to scroll up and down, exit via esc etc.) 
    //Maybe attach interrupt to keyboard events?? In general

    //When selected, must ask for password, connect etc

    //B4 finishing the function a redraw must be called to get rid of the wifi UI

}
