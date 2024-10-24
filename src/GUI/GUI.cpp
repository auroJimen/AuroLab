//GUI file, contains all functions needed for our GUI to work properly & exports classes
//ease of use by the APPS
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

//iconWiFi functions

iconWiFi_Class::iconWiFi_Class(){};

iconWiFi_Class::iconWiFi_Class(M5GFX disp){
    this->screen = disp;

    //Initialice the icon with the no conexion state
    this->screen.pushImage(pos.x, pos.y, size.x, size.y, wifiIcon[0]);
    
}

void iconWiFi_Class::update(){
    int signal = WiFi.RSSI();
    WiFi_level newLevel = nil;

    if (signal == 0) newLevel = WiFi_level::nil;
    else if (signal >  -30) newLevel = WiFi_level::great;
    else if (signal > -67) newLevel = WiFi_level::good;
    else if (signal > -70) newLevel = WiFi_level::med;
    else if (signal > -80) newLevel = WiFi_level::fair;
    else if (signal > -90) newLevel = WiFi_level::bad;

    log_i("\nRSSI = %i\nNew Level = %i\nCurrent level = %i", signal, newLevel, this->level);
    vTaskDelay(20); //Allow other tasks to do their shit to avoid screen corruption

    if (newLevel != this->level) {
        this->level = newLevel;
        this->draw();
    }

}

void iconWiFi_Class::draw(){
    switch(this->level){
        case WiFi_level::great: this->screen.pushImage(pos.x, pos.y, size.x, size.y, wifiIcon[5]);
                                break;
        case WiFi_level::good: this->screen.pushImage(pos.x, pos.y, size.x, size.y, wifiIcon[4]);
                                break;
        case WiFi_level::med: this->screen.pushImage(pos.x, pos.y, size.x, size.y, wifiIcon[3]);
                                break;
        case WiFi_level::fair: this->screen.pushImage(pos.x, pos.y, size.x, size.y, wifiIcon[2]);
                                break;
        case WiFi_level::bad: this->screen.pushImage(pos.x, pos.y, size.x, size.y, wifiIcon[1]);
                                break;
        case WiFi_level::nil: this->screen.pushImage(pos.x, pos.y, size.x, size.y, wifiIcon[0]);
                                break;

    }
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
    this->Wifi = iconWiFi_Class(disp);
    this->BLE = icon_Class(disp, iconType::BLE, false);
    this->SDpresent = icon_Class(disp, iconType::SDpresent, false);

}

void topBar_Class::updateIcons(){
    //Updates the status of all topbar icons

    this->Bat.updateState();
    this->Wifi.update();

}

//textBox_class functions
textBox_Class::textBox_Class(coord pos, coord size, String defText, M5Canvas* parent,
    coord border, float textSize, int textColour, int backColour, int cursorColour){
        this->pos = pos;
        this->size = size;
        this->border = border;
        this->parent = parent;
        this->defText = defText;
        this->textSize = textSize;
        this->textColour = textColour;
        this->backColour = backColour;
        this->cursorColour = cursorColour;
        this->dispLen = rowWidth(this->size.x -2*this->border.x, this->textSize);
        this->cursorSize = coord(6*this->textSize +1, 8*this->textSize+1);
}

void textBox_Class::draw(){
    
    //Create sprite for background
    log_i("Drawing textBox");
    M5Canvas background = (this->parent == nullptr) ? M5Canvas(&Display): M5Canvas(parent);
    background.createSprite(this->size.x, this->size.y);
    background.setBaseColor(this->backColour);
    background.clear();
    background.setColor(this->textColour);
    background.drawRoundRect(0,0,this->size.x, this->size.y, 5);

    //Push & delete
    background.pushSprite(this->pos.x, this->pos.y);
    background.deleteSprite();

    //Create sprite for text
    M5Canvas text = (this->parent == nullptr) ? M5Canvas(&Display): M5Canvas(parent);
    text.createSprite(this->size.x -2*this->border.x, this->size.y -2*this->border.y);
    text.setTextScroll(true);
    text.setTextSize(this->textSize);
    text.setTextColor(this->textColour);
    text.print(this->defText);

    //Push & delete
    text.pushSprite(this->pos.x + this->border.x, this->pos.y + this->border.y);
    text.deleteSprite();

};

void textBox_Class::enableTextInput(){

    //First we change mode to text input, and increase the size of the buffer if it's not already set that way
    if(Buffer.Mode != mode::text || Buffer.getBufferSize() != DEF_BUFFER_SIZE_TEXT_MODE
        || eTaskGetState(Buffer.getTaskHandle()) == eTaskState::eDeleted
        || eTaskGetState(Buffer.getTaskHandle()) == eTaskState::eInvalid) {
        //Checks wether the Buffer is set to an incorrect mode, or the keyboard task has been killed
        //if either of these conditions are met, it reinstantiates the buffer in the correct mode
        Buffer.killTask(); //Kill current buffer class, free up memory
        Buffer = buffer_Class(mode::text); //Reinitilize Buffer in text mode w/DEF_BUFFER_SIZE_TEXT_MODE buffer
        Buffer.begin(); //Create keyboard task and begin running it
    }

    //Then we enable input
    Buffer.keyboardEnable = true;
}

void textBox_Class:: disableTextInput(){
    //First we change mode to navigation, and decrease the buffer size

    if(Buffer.Mode != mode::nav || Buffer.getBufferSize() != DEF_BUFFER_SIZE_NAV_MODE
        || eTaskGetState(Buffer.getTaskHandle()) == eTaskState::eDeleted
        || eTaskGetState(Buffer.getTaskHandle()) == eTaskState::eInvalid) {
        //Checks wether the Buffer is set to an incorrect mode, or the keyboard task has been killed
        //if either of these conditions are met, it reinstantiates the buffer in the correct mode
        Buffer.killTask(); //Kill current buffer class, free up memory
        Buffer = buffer_Class(); //Reinitilize Buffer in nav mode w/DEF_BUFFER_SIZE_TEXT_MODE buffer
        Buffer.begin(); //Create keyboard task and begin running it
    }

    //Then we disable the input
    Buffer.keyboardEnable = false;
    log_i("Keyboard back to nav mode, input disabled");
    
}

String textBox_Class::update(){
    //Updates the text part of the sprite with the curent contents of the buffer
    M5Canvas text = (this->parent == nullptr) ? M5Canvas(&Display): M5Canvas(parent);
    text.createSprite(this->size.x -2*this->border.x, this->size.y -2*this->border.y);
    text.setTextWrap(true);
    text.setTextScroll(true);
    text.setTextSize(this->textSize);
    text.setTextColor(this->textColour);

    String aux = Buffer.getDataStr();
    if (Buffer.cursor < this->dispLen) { //Text from 0 to cursor fits
        //Print everything before the cursor
        text.print(aux.substring(0, Buffer.cursor)); 

        //Print cursor
        text.setColor(this->cursorColour);
        text.setTextColor(this->backColour);
        text.fillRect(text.getCursorX()-1, text.getCursorY()-1, this->cursorSize.x, this->cursorSize.y);
        text.print(aux[Buffer.cursor]);
        text.setTextColor(this->textColour);

        //Print everything after the cursor that still fits
        int end = (aux.length() > this->dispLen) ? this->dispLen : aux.length();
        text.print(aux.substring(Buffer.cursor+1, end));
    }
    else { //Text from 0 to cursor doesn't fit
        //Shift the start to the right as much as the cursor is greater than the display length
        text.print(aux.substring(Buffer.cursor - (this->dispLen -1), Buffer.cursor));
    
        //Print cursor
        text.setColor(this->cursorColour);
        text.setTextColor(this->backColour);
        text.fillRect(text.getCursorX()-1, text.getCursorY()-1, this->cursorSize.x, this->cursorSize.y);
        text.print(aux[Buffer.cursor]);
        text.setTextColor(this->textColour);

        //There's no space to print anything after

    }
    //Push & delete
    text.pushSprite(this->pos.x + this->border.x, this->pos.y + this->border.y);
    text.deleteSprite();

    return aux;

}


//list_Class functions
int list_Class::displayableRows(float titleSize, float textSize, int height){
    int titleSizePx = rowSize(titleSize);
    int size = rowSize(textSize);
    int rows = (height- titleSizePx)/size;
    ///log_i("Text size is %f, height is %i, rowSize is %i, %i rows fit", textSize, height, rowSize, rows);
    return rows;
}

int list_Class::rowsOccupied(int textLen, float textSize, int spriteWidth){
    int pxWidth = rowWidth(textSize, textLen);
    return ceil((float)pxWidth/(float)spriteWidth);
}

int list_Class::displayableRows(float textSize, int height){
    int size = rowSize(textSize);
    int rows = height/size;
    ///log_i("Text size is %f, height is %i, rowSize is %i, %i rows fit", textSize, height, rowSize, rows);
    return rows;
}

list_Class::list_Class(String title, int elementNum, String* elementName, 
coord origin, coord size, float titleSize, float textSize, 
int textColour, int backColour, int highlightColour, coord border){

    this->title = title;
    this->elementNum = elementNum;
    this->elementName = elementName;

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
    if(this->elementNum>0) this->drawOptions();
    
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
            int rows = list_Class::rowsOccupied(this->elementName[i].length() +1, this->textSize, this->size.x -2*this->border.x);
            text.fillRoundRect(text.getCursorX()-1, text.getCursorY()-2, size.x -2*this->border.x, rows*rowSize(this->textSize)+1, 5);
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
    if (newPos >= 0 && newPos < this->elementNum) {
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
bool list_Class::enterEvent(){return true;};

//APP_Class functions

APP_Class::APP_Class(void f(void*)){
    //This launches the GUI thread
    BaseType_t gui = xTaskCreatePinnedToCore(GUIloop, "GUI thread", 10000, NULL, 0, &this->GUI_task, 0); //Creates thread for the GUI code on core 0
    if (gui != pdPASS) log_i("ERR");
    //Send to sleep the mother thread if it's not null
    if (this->MOTHER_task != nullptr) vTaskSuspend(this->MOTHER_task);

}

APP_Class::~APP_Class(){
    //Must be called from the GUI thread!!
    if (this->BCKEND_task != nullptr) vTaskDelete(this->BCKEND_task); //Kill backend
    if (this->MOTHER_task != nullptr) vTaskResume(this->MOTHER_task); //Wake up mother
    if (this->GUI_task != nullptr) vTaskDelete(this->GUI_task);       //Kill self
}

//GUI_Class functions
void GUI_Class::begin(){

    //Keyboard stuff
    Buffer = buffer_Class();  //Reinitilizes the Buffer class to make sure its on the correct mode
    Buffer.begin();  //Starts up the separate thread that processes keyboard input, also enables input by def
    //Create topBar
    this->topBar = topBar_Class();
    //Launch topBar updater
    BaseType_t topBarUpdater = xTaskCreatePinnedToCore(topBarLoop, "topBar updater", 10000, NULL, 0, &this->topBarTask, 0); //Creates thread for the topBar updater code on core 0
    if (topBarUpdater != pdPASS) log_i("ERR");
    //Launch gui execution thread
    BaseType_t gui = xTaskCreatePinnedToCore(GUIloop, "GUI thread", 10000, NULL, 0, &this->task, 0); //Creates thread for the GUI code on core 0
    if (gui != pdPASS) log_i("ERR");
}

void GUIloop(void* parameter){
    //This code runs on a separate core

    //Maybe fix latter so that we're not forever on func call deep for no string reason?
    //There must be a better way to do this??
    GUI.mainLoop();

}

void topBarLoop(void* parameter){
    log_i("topBar updater task created");
    //Create the interrupt we'll use to time when we update timed things on screen
    hw_timer_t* timer = timerBegin(3, 80, true);
    timerAttachInterrupt(timer, &timerISR, false); //Atach interrupt to the timer
    timerAlarmWrite(timer, 2000000, true); //Set the alarm that triggers the interrupt
    timerAlarmEnable(timer); //Enable the alarm
    //Suspend the current task (it'll resume from here once the interrupt is triggered)
    vTaskSuspend(NULL);
    //Infinite loop, each time xTaskResumeFromISR is called from within the ISR, a round of this loop will
    //be executed, succesive calls to resuming won't affect an already resumed task, & the loop suspends the task
    //once the round is finished
    for(;;){
        //Update the topBar to reflect it
        vTaskDelay(100);
        GUI.updateTopBar();
        vTaskSuspend(NULL);
    }
}

void IRAM_ATTR timerISR(){
    //Change state of timeFlag
    //timeFlag = (timeFlag) ? false : true;
    //Wake up topBarLoop task
    xTaskResumeFromISR(GUI.topBarTask);
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

void GUI_Class::drawMainMenu(){
    //Draws the main menu
    M5GFX disp = this->Display;
    disp.pushImage(0,0,240,135, menuBackground);
    disp.pushImage(90,41, 58, 74, terminalIcon);
    disp.setColor(BLACK);
    disp.fillRoundRect(50, 30, 140, 90, 3);
    disp.setColor(GREEN);
    disp.drawRoundRect(50, 30, 140, 90, 3);
}

void GUI_Class::mainLoop(){
    //Draw splashscreen
    this->splashScreen();
    //Draw main menu
    this->drawMainMenu();
}

void GUI_Class::loadConfFile(){
    //Loads all saved documentation from the SD card (if present) b4 starting up the GUI
}

void GUI_Class::updateTopBar(){
    this->topBar.updateIcons();
}

void GUI_Class::drawWifiMenu(){
    //Prints available wifi networks to allow selection & connection
    //wifiMenu_Class wifiMenu;
    //wifiMenu.appLoop();
}
