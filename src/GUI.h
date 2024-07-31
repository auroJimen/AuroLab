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

class iconBat_Class {

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
    iconBat_Class();
    iconBat_Class(M5GFX& screen);
    iconBat_Class(M5GFX& screen, int perc);
    void updateState(void);
    void updateState(int perc);
    void updateChargeLevel(int perc);
    void updatePercent(int perc);
    void testAnim(int mil);
    private:
    int fetchBatLevel();
    int chargeLevel(int perc);

};

enum iconType {SDpresent, Wifi, BLE};

//Icon class for the contents of the topBar
class icon_Class {
    private:
    M5GFX sprite;
    iconType type;
    int state;
    coord pos; 
    M5GFX screen;             //Screen object

    public:
    icon_Class();
    icon_Class(M5GFX screen, iconType type, int state);
    bool changeState(bool newState);


};

class topBar_Class{
    //Controls what is displayed on the top bar of the GUI on the main menu
    public:
    //Time element to display current time if theres wifi
    icon_Class Wifi; //Wifi element to display a small sprite and the SSID
    icon_Class BLE;  //Bluetooth low energy icon
    icon_Class SDpresent;   //SD card icon
    iconBat_Class Bat;  //Battery indicator
    M5GFX Display = M5Cardputer.Display;
    public:
    topBar_Class();
    void updateIcons();
    //There should be methods to update everything in here

};

class GUI_Class {
    //A global instance of this class controls all the GUI elements and provides the
    //method for the backend and front end to communicate
    //The GUI class depends on the M5Cardputer object defined in M5Cardputer.h
    private:
    //Doesnt need a constructor
    M5GFX &Display = M5Cardputer.Display;
    Keyboard_Class Keyboard = M5Cardputer.Keyboard;
    topBar_Class topBar;
    public:
    TaskHandle_t task;
    void begin();
    void splashScreen();
    void drawMainMenu();
    void mainLoop();
    void loadConfFile();

};

//Global GUI instance 
extern GUI_Class GUI;

