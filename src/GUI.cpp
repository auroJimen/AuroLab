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

int list_Class::displayableRows(float titleSize, float textSize, int height){
    int titleRowSize = rowSize(titleSize);
    int rowSize = list_Class::rowSize(textSize);
    int rows = (height- titleRowSize)/rowSize;
    ///log_i("Text size is %f, height is %i, rowSize is %i, %i rows fit", textSize, height, rowSize, rows);
    return rows;
}

int list_Class::displayableRows(float textSize, int height){
    int rowSize = list_Class::rowSize(textSize);
    int rows = height/rowSize;
    ///log_i("Text size is %f, height is %i, rowSize is %i, %i rows fit", textSize, height, rowSize, rows);
    return rows;
}

list_Class::list_Class(String title, int elementNum, String* elementName, void (*handler)(int, void*), 
coord origin, coord size, float titleSize, float textSize, 
int textColour, int backColour, int highlightColour, coord border){

    this->title = title;
    this->elementNum = elementNum;
    this->elementName = elementName;
    this->handler = handler;

    this->origin = origin;
    this->size = size;
    this->titleSize = titleSize;
    this->textSize = textSize;
    this->textColour = textColour;
    this->backColour = backColour;
    this->highlightColour = highlightColour;

    this->pos = 0;
    this->border = border;

    this->rows = displayableRows(textSize, size.y-3*this->border.y-rowSize(this->titleSize));

}

void list_Class::draw(){

    //Create the background sprite in RAM
    M5Canvas background(&Display);
    background.createSprite(this->size.x, this->size.y);

    //Draw background rectangle
    background.setBaseColor(this->backColour);
    background.clear();
    background.setColor(this->textColour);
    background.drawRoundRect(0, 0, this->size.x , this->size.y, 3);

    //Push & delete the sprite
    background.pushSprite(this->origin.x, this->origin.y);
    background.deleteSprite();
    
    //Create title sprite in RAM & Set text options
    M5Canvas titleSprt(&Display);
    titleSprt.createSprite(this->size.x -2*this->border.x, rowSize(this->titleSize));
    titleSprt.setTextColor(this->textColour);
    titleSprt.setTextSize(this->titleSize);
    titleSprt.println(title);

    //Push & delete the sprite
    titleSprt.pushSprite(origin.x +this->border.x, origin.y +this->border.y);
    titleSprt.deleteSprite();

    //Draw options
    this->drawOptions();
    
}

void list_Class::drawOptions(){
    //Create text sprite in RAM & Set text options
    M5Canvas text(&Display);
    text.createSprite(this->size.x -2*this->border.x, this->size.y -3*this->border.y -rowSize(this->titleSize));
    text.setTextColor(this->textColour);
    text.setTextSize(this->textSize);
    text.setCursor(0,1);
    //For loop to draw the options (taking current pos into account)
    int index = pos -2;
    if (index < 0) index = 0; //If the selected pos is on 0,1,2 drawing starts @ index 0
    else if (this->elementNum - index -1 < this->rows) index = this->elementNum-this->rows; //If the selected position is near the end, print enough to fill the rows
    int count = 0;
    for(int i = index; (i <this->elementNum && count < this->rows); i++){
        if (i == pos) {
            text.setColor(this->highlightColour);
            text.fillRoundRect(text.getCursorX(), text.getCursorY()-1, size.x -2*this->border.x, rowSize(this->textSize)+1, 5);
            text.setTextColor(this->backColour);
            text.println(" "+this->elementName[i]);
            text.setTextColor(this->textColour);
        } else text.println(this->elementName[i]);
        count++;
    }

    text.pushSprite(origin.x +this->border.x, origin.y +2*this->border.y + rowSize(this->titleSize));
    text.deleteSprite();

}

void list_Class::scroll(int newPos) {
    if (newPos > 0 && newPos < this->elementNum) {
        this->pos = newPos;
        //Maybe change this latter if a redraw will be already triggered by the mainloop each x ms
        this->drawOptions();
    }
}

void list_Class::scrollDown(){
    this->scroll(this->pos+1);
}

void list_Class::scrollUp(){
    this->scroll(this->pos-1);
}

//GUI functions

void GUI_Class::begin(){

    //Launch gui execution thread
    BaseType_t gui = xTaskCreatePinnedToCore(GUIloop, "GUI thread", 10000, NULL, 0, &this->task, 0); //Creates thread for the GUI code on core 0
    if (gui != pdPASS) log_i("ERR");
    Buffer.begin();                      //Initialises Keyboard buffer in tis separate execution thread

}

void GUIloop(void* parameter){
    //This code runs on a separate core
    GUI.mainLoop();

}

void GUI_Class::splashScreen(){

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

}

void testHandler(int a, void* b) {return;};

void GUI_Class::drawMainMenu(){
    //Draws the main menu
    M5GFX disp = this->Display;
    disp.pushImage(0,0,240,135, menuBackgroundTest);
    //disp.pushImage(90,41, 58, 74, terminalIcon);
    //disp.setColor(BLACK);
    //disp.fillRoundRect(50, 30, 140, 90, 3);
    //disp.setColor(GREEN);
    //disp.drawRoundRect(50, 30, 140, 90, 3);
}

void GUI_Class::mainLoop(){
    //Draw splashscreen
    this->splashScreen();
    //Create topBar
    this->topBar = topBar_Class();
    //Draw main manu
    this->drawMainMenu();
    //MainLoop for the GUI, handles drawing the UI elements, reacts to inputs etc.
    for(;;){

        this->topBar.updateIcons();
        /*String elements[] = {"Pos0", "Pos1", "Pos2", "Pos3", "Pos4", "Pos5", "Pos6", "Pos7", "Pos8", "Pos9"};
        String *ref = elements;
        list_Class test = list_Class(String("Titulo"), 10, ref,  &testHandler, coord(50,30), coord(140,96));
        test.draw();

        for(;;){
        for(int i = 0; i < 10; i++){
            delay(500);
            test.scroll(i);
        }
        delay(2000);
        }*/
        this->drawWifiMenu();
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
        //Print something to show scan is in progress, small windget?
        //I want to draw a scanning animation if possible, should be left for future
        //this->Display.drawRect(105, 53, 30, 29, GREEN);
        delay(20);
    }

    //Get number of available networks
    int availableNetworks = WiFi.scanComplete();

    String SSID[availableNetworks];
    String* ptr = SSID;
    uint8_t encript[availableNetworks];
    int32_t RSSI[availableNetworks];
    String BSSID[availableNetworks];
    int32_t channel[availableNetworks];
    for (int i= 0; i < availableNetworks; i++){
        SSID[i] = WiFi.SSID(i);
        encript[i] = WiFi.encryptionType(i);
        RSSI[i] = WiFi.RSSI(i);
        BSSID[i] = WiFi.BSSIDstr(i);
        channel[i] = WiFi.channel(i);
    }
    
    list_Class wifiMenu(String("WiFi"), availableNetworks, ptr, &testHandler, coord(50,30), coord(140,96));
    wifiMenu.draw();

    //Wait for user input loop:
    Buffer.keyboardEnable = true;
    for(;;){
        //Check if any relevant key has been pressed
        /*if (buffer.length() == 1) {
            char key = buffer[0];
            switch(key){
                case '.':
                case '/': wifiMenu.scrollDown();
                          break;
                case ';':
                case ',': wifiMenu.scrollUp();
                          break;
                case '`': keyBoardEnable = false;
                          //wifiMenu.del();
                          break;
                
            }
            buffer.clear(); //Reset buffer
        }*/



    }

    //When selected, must ask for password, connect etc

    //B4 finishing the function a redraw must be called to get rid of the wifi UI, and delete the 

}
