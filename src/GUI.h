#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>
#include "sprites.h"
#include "splashscreen.h"
#include "menuBackground.h"
#include "batIconTest.h"

//In the future put here the rest of the object instances to control all periferals


//Declaration of all relevant classes

enum iconType {SDpresent, Bat, Wifi, BLE};

//Icon class for the contents of the topBar
class icon {
    private:
    M5GFX sprite;
    iconType type;
    int state;
    int x;
    int y; 

    
    
    public:
    icon(M5GFX& parent, iconType type, int state);
    bool changeState(bool newState);
    int changeState(int percent); //For the battery variant


};

class topBar{
    //Controls what is displayed on the top bar of the GUI on the main menu
    private:
    //Time element to display current time if theres wifi
    icon Wifi; //Wifi element to display a small sprite and the SSID
    icon BLE;  //Bluetooth low energy icon
    icon SDpresent;   //SD card icon
    icon Bat;  //Battery indicator
    public:
    topBar(LGFX_Sprite parent);
    //There should be methods to update everything in here

};

class GraphicalUI {
    //A global instance of this class controls all the GUI elements and provides the
    //method for the backend and front end to communicate
    //The GUI class depends on the M5Cardputer object defined in M5Cardputer.h
    private:
    //Doesnt need a constructor
    M5GFX &Display = M5Cardputer.Display;
    Keyboard_Class Keyboard = M5Cardputer.Keyboard;
    //topBar bar;
    public:
    
    void begin();
    void splashScreen();
    void drawMainMenu();
    void mainLoop();
    void loadConfFile();

};

//Global GUI instance 
extern GraphicalUI GUI;

