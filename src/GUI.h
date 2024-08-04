#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>
#include "sprites.h"
#include <WiFiScan.h>

//In the future put here the rest of the object instances to control all periferals


//Declaration of all relevant classes


/// @brief Simple coordinates struct to handle positions on the LCD
struct coord {
    public:
    int x;
    int y;
};
/// @brief Battery Icon class, handles the battery icon shown on the top bar
class iconBat_Class {

    private:
    int percent;                  ///< Battery percent
    coord basePos = {209,3};     ///Coordinates to draw sprite base
    coord baseSize = {26,15}; //Sprite base size
    coord contPos = {212,5};     //Coordinates to draw sprite content
    coord contSize = {15,11}; //Sprite content size
    coord textPos = {173,5};     //Coordinates to write Bat percent
    float textSize = 1.6;     //Text size
    M5GFX screen;             //Screen object
    public:
    /// @brief Empty constructor for the Battery Class
    iconBat_Class();

    /// @brief Constructor of the Battery Icon class
    /// @param display The M5GFX display object
    /// @param perc Battery percent
    iconBat_Class(M5GFX& screen);
    
    /// @brief Constructor of the Battery Icon class
    /// @param display  The M5GFX display object
    iconBat_Class(M5GFX& screen, int perc);

    /// @brief Updates battery level and percentage fetching value from the internal power chip (only updates if the percentage has changed)
    void updateState(void);

    /// @brief Updates battery level and percentage from given percentage (only updates if the percentage has changed)
    /// @param perc Battery percentage, int 1-100
    void updateState(int perc);

    /// @brief Updates shown charge level in the icon from the given percent value
    /// @param perc Battery percentage int 1-100;
    void updateChargeLevel(int perc);

    /// @brief Updates the battery percentage shown on the topBar, deleting the previous one 
    /// @param perc New battery percentage, int
    void updatePercent(int perc);

    /// @brief Animation to show the different levels of the battery icon
    /// @param mil Delay in ms between state changes
    void testAnim(int mil);
    private:
    /// @brief Fetches battery level from power chip via the method included in the M5Cardputer library PRIVATE METHOD
    /// @return Battery percentage, int 1-100
    int fetchBatLevel();

    /// @brief Returns the charge level 0-11 from a percentage for displaying the correct icon
    /// @param perc Battery percentage, int 1-100 if greater than 100% returns 11
    /// @return Battery level 0-11 for the battery icon
    int chargeLevel(int perc);

};

/// @brief Enum of the diffent types of icons
enum iconType {SDpresent, Wifi, BLE};

/// @brief Icon class for the contents of the topBar
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
/// @brief Controls what is displayed on the top bar of the GUI on the main menu
class topBar_Class{
    public:
    //Time element to display current time if theres wifi
    icon_Class Wifi; //Wifi element to display a small sprite and the SSID
    icon_Class BLE;  //Bluetooth low energy icon
    icon_Class SDpresent;   //SD card icon
    iconBat_Class Bat;  //Battery indicator
    M5GFX Display = M5Cardputer.Display;
    public:
    /// @brief Constructor for the Top bar class
    topBar_Class();
    /// @brief  Updates the state of all icons in the Top Bar
    void updateIcons();
    //There should be methods to update everything in here

};
/// @brief A global instance of this class controls all the GUI elements and provides the
/// method for the backend and front end to communicate.
/// The GUI class depends on the M5Cardputer object defined in M5Cardputer.h
class GUI_Class {
    private:
    //Doesnt need a constructor
    M5GFX &Display = M5Cardputer.Display;
    Keyboard_Class Keyboard = M5Cardputer.Keyboard;
    topBar_Class topBar;
    public:
    TaskHandle_t task;
    /// @brief Starts up the GUI, showing the splashscreen
    void begin();

    /// @brief Draws the main menu
    void drawMainMenu();

    /// @brief Draws the wifi menu to scan & connect to networks UNIMPLEMENTED
    void drawWifiMenu();

    /// @brief Mainloop for the GUI, must be run on its own thread to prevent interference w/ backend. Handles drawing UI elements, inputs, etc. through an endless loop.
    void mainLoop();

    /// @brief Loads configuration file from SD UNIMPLEMENTED
    void loadConfFile();

};

//Global GUI instance 
extern GUI_Class GUI;

