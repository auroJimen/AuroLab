#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>

//Declaration of all relevant classes

class topBar{
    //Controls what is displayed on the top bar of the GUI on the main menu
    private:
    //Time element to display current time if thres wifi
    //Wifi element to display a small sprite and the SSID
    //Bluetooth low energy sprite
    //Battery indicator
    public:
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
    public:
    void splashScreen();
    void drawMainMenu();
    void mainLoop();
    void loadConfFile();

};

//Global GUI instance 
extern GraphicalUI GUI;

