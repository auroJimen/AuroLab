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
//wifiMenu fuctions

wifiMenu_Class::wifiMenu_Class() :  list_Class(String("WiFi"), 0, nullptr, coord(50,30), coord(140,96)){
    //Instantiates mother class (list_CLass) with the correct parameters & no given options
    //Then initialises the elements of this class
    //These fields are set to nullptr since they are only allocated by scan()
    this->elementName = nullptr;
    this->encript = nullptr;
    this->RSSI = nullptr;
    this->BSSID = nullptr;
    this->channel = nullptr;

}

void wifiMenu_Class::scan(){
    //Deletes previous scan data
    this->del();
    //Draws the basic outline of an empty list
    this->draw();
    //Starts async scan & displays animation while it runs
    WiFi.scanNetworks(true, true);
    M5Canvas loadingAnim(&this->Display);
    loadingAnim.createSprite(50,50);
    loadingAnim.pushImage(0,0,50,50,compassIcon);
    float i = 0.0;
    this->Display.setCursor(70,110);
    this->Display.setTextSize(1.5);
    this->Display.print("Scanning...");
    //Check if scan has finished
    while(WiFi.scanComplete() < 0) {
        loadingAnim.pushRotateZoomWithAA(240/2, 135/2 + 15, i, 1.0, 1.0, RED);
        i+= 5.0;
        delay(20);
    }
    loadingAnim.deleteSprite();

    //Get number of available networks
    int availableNetworks = WiFi.scanComplete();
    String* SSID = new String[availableNetworks];
    uint8_t* encript = new uint8_t[availableNetworks];
    int32_t* RSSI = new int32_t[availableNetworks];
    String* BSSID = new String[availableNetworks];
    int32_t* channel = new int32_t[availableNetworks];
    for (int i= 0; i < availableNetworks; i++){
        SSID[i] = (WiFi.SSID(i) == "") ? "Hidden_network" : WiFi.SSID(i);
        encript[i] = WiFi.encryptionType(i);
        RSSI[i] = WiFi.RSSI(i);
        BSSID[i] = WiFi.BSSIDstr(i);
        channel[i] = WiFi.channel(i);
    }

    this->elementNum = availableNetworks;
    this->elementName = SSID;
    this->encript = encript;
    this->RSSI = RSSI;
    this->BSSID = BSSID;
    this->channel = channel;
}

void wifiMenu_Class::appLoop(){
    //Perform primary scan
    if(this->elementNum== 0)this->scan();
    //Draw the menu
    this->draw();
    bool flag = false;
    //Check keyboard mode & that it is active
    while(!flag){
        //Endless application loop
        //Nav signal listener
        vTaskDelay(10 / portTICK_PERIOD_MS); //Allows other things to update, housekeeping etc
        if (Buffer.signal != navSignal::NP) {
            log_i("Nav signal detected");
            Buffer.keyboardEnable = false; //Deactivate keyboard during execution
            switch (Buffer.signal){
                case navSignal::ESC:    //Close the menu, UNIMPLEMENTED
                                        log_i("Close event");
                                        break;
                case navSignal::UP:
                case navSignal::LEFT:   //Scroll up
                                        log_i("Scroll up event");
                                        this->scrollUp();
                                        break;
                case navSignal::DOWN:   
                case navSignal::RIGHT:  //Scroll down
                                        log_i("Scroll down event");
                                        this->scrollDown();
                                        break;
                case navSignal::ENTER:  //Element selected event
                                        log_i("Element selected event");
                                        if (this->enterEvent()){
                                            //Prepare to exit the loop and return (wifi was connected)
                                            flag = true;
                                            this->del();
                                            break;
                                        } else {
                                            //If esc is triggered it returns to the appLoop
                                            this->draw();
                                            break;
                                        }
                case navSignal::OPT:    //Option event
                                        log_i("Option event");
                                        this->optnEvent();
                                        //After the opt event it returns to the appLoop
                                        this->draw();
                                        break;
                case navSignal::CTRL:   //Rescan event
                                        log_i("Rescan event");
                                        this->scan();
                                        this->draw();
            }
            Buffer.signal = navSignal::NP; //Clear signal flag
            Buffer.keyboardEnable = true; //Reactivate after
            log_i("Keyboard input reactivated");
        }
    }
}

String encriptType(int i){
    switch(i){
        case wifi_auth_mode_t::WIFI_AUTH_OPEN: return String("Open");
        case wifi_auth_mode_t::WIFI_AUTH_WEP: return String("WEP");
        case wifi_auth_mode_t::WIFI_AUTH_WPA_PSK: return String("WPA-PSK");
        case wifi_auth_mode_t::WIFI_AUTH_WPA2_PSK: return String("WPA2-PSK");
        case wifi_auth_mode_t::WIFI_AUTH_WPA_WPA2_PSK: return String("WPA1/2-PSK");
        case wifi_auth_mode_t::WIFI_AUTH_ENTERPRISE:
        case wifi_auth_mode_t::WIFI_AUTH_WPA3_PSK:
        case wifi_auth_mode_t::WIFI_AUTH_WPA2_WPA3_PSK:
        case wifi_auth_mode_t::WIFI_AUTH_WAPI_PSK:
        case wifi_auth_mode_t::WIFI_AUTH_WPA3_ENT_192:
        case wifi_auth_mode_t::WIFI_AUTH_MAX: 
        default: return String("Weird");
    }
}

void wifiMenu_Class::optnEvent(){
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
    titleSprt.println(this->elementName[this->pos]);

    //Push & delete the sprite
    titleSprt.pushSprite(origin.x +this->border.x, origin.y +this->border.y);
    titleSprt.deleteSprite();

    //Create text sprite in RAM & Set text options
    M5Canvas text(&Display);
    text.createSprite(this->size.x -2*this->border.x, this->size.y -3*this->border.y -rowSize(this->titleSize));
    text.setTextColor(this->textColour);
    text.setTextSize(1.5);
    text.setCursor(0,1);
    text.printf("RSSI: %i\n",this->RSSI[this->pos]);
    text.printf("Channel: %i\n", this->channel[this->pos]);
    text.setTextSize(1.0);
    text.printf("Encryption: %s\n", encriptType(this->encript[this->pos]));
    text.printf("BSSID: %i\n", this->BSSID[this->pos]);
    text.println("Press ESC to return.");

    //Push & delete the sprite
    text.pushSprite(origin.x +this->border.x, origin.y +2*this->border.y + rowSize(this->titleSize));
    text.deleteSprite();
    bool flag = false;
    while(!flag){
        //Wait for user input
        //Nav signal listener
        vTaskDelay(10 / portTICK_PERIOD_MS); //Alows other things to update, housekeeping etc
        if (Buffer.signal != navSignal::NP) {
            log_i("Nav signal detected");
            Buffer.keyboardEnable = false; //Deactivate keyboard during execution
            switch (Buffer.signal){
                case navSignal::ESC:    //Close the menu, UNIMPLEMENTED
                                        log_i("Close event");
                                        flag = true;
                                        break;
            }
            Buffer.signal = navSignal::NP; //Clear signal flag
            Buffer.keyboardEnable = true; //Reactivate after
            log_i("Keyboard input reactivated");
        }
    };


};

bool wifiMenu_Class::enterEvent(){
    //Handles input of the password to the network

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
    titleSprt.println(this->elementName[this->pos]);

    //Push & delete the sprite
    titleSprt.pushSprite(origin.x +this->border.x, origin.y +this->border.y);
    titleSprt.deleteSprite();

    //Create text sprite
    M5Canvas text(&Display);
    text.createSprite(this->size.x -2*this->border.x, this->size.y -3*this->border.y -rowSize(this->titleSize));
    text.setTextColor(this->textColour);
    text.setTextSize(1.3);
    text.setCursor(0,1);
    text.println("\nEnter the network password:");

    //Create textBox
    textBox_Class psswrd(coord(0,40), coord(this->size.x -2*this->border.x, rowSize(this->titleSize)), "Prueba", &text);
    //Push sprite
    psswrd.draw();
    text.pushSprite(origin.x +this->border.x, origin.y +2*this->border.y + rowSize(this->titleSize));

    bool flag = false;
    psswrd.enableTextInput();
    log_i("Entered input loop");
    String password;
    while(true) {
        //Wait for user input
        //Nav signal listener
        if (Buffer.signal == navSignal::ESC) {
            Buffer.signal = navSignal::NP; //Clear signal
            break; //Exiting
        }
        else if (Buffer.signal == navSignal::ENTER) {
            Buffer.signal = navSignal::NP; //Clear signal
            flag = true; //Attempting to connect to the WiFi
            break; //Exiting
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); //Alows other things to update, housekeeping etc
        password = psswrd.update(); //Updates the displayed string to be the current contents of the buffer
        text.pushSprite(origin.x +this->border.x, origin.y +2*this->border.y + rowSize(this->titleSize));
    }

    text.deleteSprite();
    if (flag) {
        flag = this->connectToNetWork(this->elementName[this->pos], password);
        psswrd.disableTextInput(); //Return keyboard to nav mode for the menu
        return flag;
    }
    else psswrd.disableTextInput(); //Return keyboard to nav mode for the menu
    return flag;

}

bool wifiMenu_Class::connectToNetWork(String ssid, String password){
    //Handles the actual connection

    //Create the background sprite in RAM
    M5Canvas base(&Display);
    base.createSprite(this->size.x, this->size.y);

    //Draw background rectangle
    base.setBaseColor(this->backColour);
    base.clear();
    base.setColor(this->textColour);
    base.drawRoundRect(0, 0, this->size.x , this->size.y, 3);
    base.pushImage(20,10, 45,74, telefonoBase);
    
    M5Canvas text(&base);
    text.createSprite(60,60);
    text.setTextColor(GREEN);
    text.print("Connecting\nto:\n");
    text.print(ssid);
    text.pushSprite(55,20);
    text.deleteSprite();

    base.pushSprite(this->origin.x, this->origin.y, RED);

    M5Canvas anim(&Display);
    anim.createSprite(13,13);
    anim.pushImage(0,0,13,13,ruedaTelefono);

    M5Canvas banner(&Display);
    banner.createSprite(60,20);

    WiFi.begin(ssid, password);
    float i = 0.0;
    float delta = 5.0;
    int ctr = 0;
    while(WiFi.status() != WL_CONNECTED && ctr < 10) {
        anim.pushRotateZoomWithAA(92, 77, i, 1.0, 1.0, RED);
        i+= delta;
        vTaskDelay(50);
        if (i > 90 || i < 0) {
            log_i("Attempting to connect, %i", ctr);
            delta = - delta;
            ctr ++;
        }

    }

    if (WiFi.isConnected()){
        log_i("Connected to: %s", ssid.c_str());
        banner.setTextColor(GREEN);
        banner.print("SUCCESS!!");
        banner.pushSprite(this->origin.x + 55, this->origin.y + 60);
        banner.deleteSprite();
        vTaskDelay(1000/portTICK_PERIOD_MS);
        return true;
    } else {
        log_i("Connection failed");
        WiFi.disconnect(); //cancel connection
        log_printf("WiFi status is %i", WiFi.status());
        banner.setTextColor(RED);
        banner.print("FAILED!!");
        banner.pushSprite(this->origin.x + 55, this->origin.y + 60);
        banner.deleteSprite();
        vTaskDelay(1000/portTICK_PERIOD_MS);
        return false;
    }

}



void wifiMenu_Class::del() {
    
    log_i("Deleting saved scan");
    //Frees dinamically allocated memory
    delete[] this->elementName;
    delete[] this->encript;
    delete[] this->RSSI;
    delete[] this->BSSID;
    delete[] this->channel;

    //resets everything to behave as an empty list
    this->elementName = nullptr;
    this->encript = nullptr;
    this->RSSI = nullptr;
    this->BSSID = nullptr;
    this->channel = nullptr;
    this->elementNum = 0;

    //Deletes the scan date from the WiFi class
    WiFi.scanDelete();
 
}

//GUI functions
void GUI_Class::begin(){

    //Keyboard stuff
    Buffer = buffer_Class();  //Reinitilizes the Buffer class to make sure its on the correct mode
    Buffer.begin();  //Starts up the separate thread that processes keyboard input, also enables input by def
    //Launch gui execution thread
    BaseType_t gui = xTaskCreatePinnedToCore(GUIloop, "GUI thread", 10000, NULL, 0, &this->task, 0); //Creates thread for the GUI code on core 0
    if (gui != pdPASS) log_i("ERR");
    //Launch topBar updater
    BaseType_t topBarUpdater = xTaskCreatePinnedToCore(topBarLoop, "topBar updater", 10000, NULL, 0, &this->topBarTask, 0); //Creates thread for the topBar updater code on core 0
    if (topBarUpdater != pdPASS) log_i("ERR");
}

void GUIloop(void* parameter){
    //This code runs on a separate core
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
    //Create topBar
    this->topBar = topBar_Class();
    //Draw main manu
    this->drawMainMenu();
    //MainLoop for the GUI, handles drawing the UI elements, reacts to inputs etc.
    for(;;){

        //this->topBar.updateIcons();
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

void GUI_Class::updateTopBar(){
    this->topBar.updateIcons();
}

void GUI_Class::drawWifiMenu(){
    //Prints available wifi networks to allow selection & connection
    wifiMenu_Class wifiMenu;
    wifiMenu.appLoop();
}
