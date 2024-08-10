#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>
#include "sprites.h"
#include "menuBackground.h"
#include <WiFi.h>

//In the future put here the rest of the object instances to control all periferals


//Declaration of all relevant classes


/// @brief Simple coordinates struct to handle positions on the LCD
struct coord {
    public:
    int x;
    int y;

    coord();
    coord(int x, int y);
    coord add(coord a);
};
/// @brief Battery Icon class, handles the battery icon shown on the top bar
class iconBat_Class {

    private:
    int percent;                  ///< Battery percent
    coord basePos = {209,3};     /// Coordinates to draw sprite base
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

///@brief Scrollable, selectable list class to construct menus
class list_Class{
    private:
    String title;
    int elementNum; ///< Number of elements on the list
    String* elementName; ///< Names of the list elements
    int pos; ///< Current highlighted position on the list
    void (*handler)(int, int); ///< Handler function to trigger events from selecting an element
    M5GFX Display = M5Cardputer.Display; ///> Display object
    coord origin;
    coord size;
    coord textPos;
    float titleSize;
    float textSize;
    int textColour;
    int backColour;
    int highlightColour;
    int border;


    public:
    ///@brief Constructor for the list class
    ///@param title String, list title
    ///@param elementNum Int, number of elements on the list
    ///@param elementName Pointer to the string array that holds the names of the list elements
    ///@param handler Function that triggers the proper event when an element is selected,
    /// its parameters are pos & elementNum
    ///@param origin Coordinates where the list rectangle will be drawn
    ///@param size Size of the list rectangle
    ///@param textPos Coordinates where the text begins to be drawn (relative to the origin of the rectangle)
    ///@param titleSize Float, text size of the title, defaults to 2.5
    ///@param textSize Float, text size of the list body, defaults to 1.0
    ///@param textColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    ///@param backColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to black
    ///@param highlightColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    list_Class(String title, int elementNum, String* elementName, void (*handler)(int, int), 
     coord origin, coord size, coord textPos, float titleSize = 2.5, float textSize = 1.0, 
     int textColour = m5gfx::ili9341_colors::GREEN, int backColour = m5gfx::ili9341_colors::BLACK, 
     int highlightColour = m5gfx::ili9341_colors::GREEN, int border =5);

    ///@brief Draws or redraws the list on the screen
    void draw();

    ///@brief Deletes the list instance 
    void del();

    ///@brief Scrolls the list to the given position
    ///@param newPos Position we want to scroll to
    void scroll(int newPos);

    
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

    /// @brief Draws the wifi menu to scan & connect to networks
    void drawWifiMenu();

    /// @brief Mainloop for the GUI, must be run on its own thread to prevent interference w/ backend. Handles drawing UI elements, inputs, etc. through an endless loop.
    void mainLoop();

    /// @brief Loads configuration file from SD UNIMPLEMENTED
    void loadConfFile();

};

//Global GUI instance 
extern GUI_Class GUI;

