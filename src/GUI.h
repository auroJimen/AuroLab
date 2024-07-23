#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>
#include "sprites.h"

//In the future put here the rest of the object instances to control all periferals


//Declaration of all relevant classes

struct coord {
    public:
    int x;
    int y;
};

class iconBat {

    private:
    int percent;    //Battery percent
    coord basePos = {209,3};     //Coordinates to draw sprite base
    coord baseSize = {26,15}; //Sprite base size
    coord contPos = {212,5};     //Coordinates to draw sprite content
    coord contSize = {15,11}; //Sprite content size
    coord textPos = {173,5};     //Coordinates to write Bat percent
    float textSize = 1.6;     //Text size
    M5GFX screen;             //Screen object
    public:
    iconBat(M5GFX& screen, int percent);
    int changeState(int perc);
    int changeState(void);
    void updatePercent(int perc);
    void testAnim(int mil);

};

enum iconType {SDpresent, Wifi, BLE};

//Icon class for the contents of the topBar
class icon {
    private:
    M5GFX sprite;
    iconType type;
    int state;
    coord pos; 

    
    
    public:
    icon(M5GFX& parent, iconType type, int state);
    bool changeState(bool newState);


};

class topBar{
    //Controls what is displayed on the top bar of the GUI on the main menu
    private:
    //Time element to display current time if theres wifi
    icon Wifi; //Wifi element to display a small sprite and the SSID
    icon BLE;  //Bluetooth low energy icon
    icon SDpresent;   //SD card icon
    iconBat Bat;  //Battery indicator
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

