#include "settingsGUI.h"

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