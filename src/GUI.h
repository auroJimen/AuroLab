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
    /// @param perc Battery percent (def = 0)
    iconBat_Class(M5GFX& screen, int perc = 0);

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

/// @brief Text input box class to construct menus
class textBox_Class{
    private:
    bool inputEnabled; ///< Wether inputing is currently enabled
    String defText; ///< Text displayed on the text box when its first created
    String* text; ///< Pointer to the string where the text will be shaved when a enter event is triggered
    coord pos; ///< Where the text box should be drawn
    coord size; ///< Size of the text box
    float textSize; ///< Text size multiplier for the body
    int textColour; ///< Text colour
    int backColour; ///< Background colour
    int cursorColour; ///< Cursor colour
    public:
    /// @brief Constructor for the textBox class
    /// @param pos Coord position where the textBox will be drawn
    /// @param size Coord size of the textBox
    /// @param text Pointer to a string where the text will be saved when an enter event is triggered
    /// @param inputEnabled Sets wether inputing to the box is currently enabled, def to false
    /// @param defText Text displayed on the box when it's created, def to ""
    /// @param TextSize Float text size multiplayer, def to 1.5
    /// @param textColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    /// @param backColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to black
    /// @param cursorColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    textBox_Class(coord pos, coord size, String* text, bool inputEnabled=false, String defText="", 
     float TextSize=1.5, int textColour = m5gfx::ili9341_colors::GREEN,
     int backColour = m5gfx::ili9341_colors::BLACK, int cursorColour = m5gfx::ili9341_colors::GREEN);
    /// @brief Enables the textInput and launches a keyboard listener if it isn't already running
    void enableTextInput();
    /// @brief Disables textInput and kills the keyboard listener process
    void disableTextInput();
    /// @brief Returns the value of enableTextInput
    /// @return Bool, enableTextInput
    bool getEnableTextInput();
};

///@brief Scrollable, selectable list class to construct menus
class list_Class{
    private:
    String title; ///< Title of the list
    int elementNum; ///< Number of elements on the list
    String* elementName; ///< Names of the list elements
    int pos; ///< Current highlighted position on the list
    void (*handler)(int, void*); ///< Handler function to trigger events from selecting an element
    M5GFX Display = M5Cardputer.Display; ///> Display object
    coord origin; ///< Coordinates where the list shall be drawn
    coord size; ///< Absolute size of the list rectangle
    float titleSize; ///< Text size multiplier for the title
    float textSize; ///< Text size multiplier for the body
    int textColour; ///< Text colour (title & body)
    int backColour; ///< Background color
    int highlightColour; ///< Highlight color for the currently selected option
    coord border; ///< Margin from the rectangle to the text (in both directions)
    int rows; ///< Number of rows that can be displayed @ a time

    public:
    ///@brief Constructor for the list class
    ///@param title String, list title
    ///@param elementNum Int, number of elements on the list
    ///@param elementName Pointer to the string array that holds the names of the list elements
    ///@param handler Function that triggers the proper event when an element is selected,
    /// its parameters are pos & elementNum
    ///@param origin Coordinates where the list rectangle will be drawn
    ///@param size Size of the list rectangle
    ///@param titleSize Float, text size of the title, defaults to 2.5
    ///@param textSize Float, text size of the list body, defaults to 1.5
    ///@param textColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    ///@param backColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to black
    ///@param highlightColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    ///@param border Coord, X & Y margins defaults to 6 & 5
    list_Class(String title, int elementNum, String* elementName, void (*handler)(int, void*), 
     coord origin, coord size, float titleSize = 2.5, float textSize = 1.5, 
     int textColour = m5gfx::ili9341_colors::GREEN, int backColour = m5gfx::ili9341_colors::BLACK, 
     int highlightColour = m5gfx::ili9341_colors::GREEN, coord border = coord(6, 5));
    ///@brief Draws or redraws the list on the screen
    void draw();
    ///@brief Deletes the list instance 
    void del();
    ///@brief Scrolls the list to the given position
    ///@param newPos Position we want to scroll to
    void scroll(int newPos);

    private:
    /// @brief Draws the options section with the current selection highlighted (called by draw & scroll)
    void drawOptions();

    /// @brief Calculates the number of rows that fit 
    /// on the screen given the height of the text sprite
    /// and the text size
    /// @param titleSize Float, the text size multiplier used in M5GFX for the title
    /// @param textSize Float, the text size multiplier used in M5GFX for the body
    /// @param height The height in pixels of the text sprite
    /// @return The number of rows that fit on the text sprite as an integer
    int displayableRows(float titleSize, float textSize, int height);
    /// @brief Calculates the number of rows that fit 
    /// on the screen given the height of the text sprite
    /// @param textSize Float, the text size multiplier used in M5GFX for the body
    /// @param height The height in pixels of the text sprite
    /// @return The number of rows that fit on the text sprite as an integer
    int displayableRows(float textSize, int height);

    /// @brief Calculates height in px of a line of text of given size 
    /// @param textSize Float, the text size multiplier used in M5GFX
    /// @return Height in px of the line
    inline int rowSize(float textSize) {return floor(8*textSize);}
    
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

    /// @brief Handler for the option selected event, in the wifi menu case it must draw a
    /// screen with all the selected network info & allow to choose wether to connect or go back to the previous menu
    /// @param pos The selected position at the time of the event, int
    /// @param  b A void pointer to whatever object we may want to pass as a reference to the handler
    void wifiMenuHandler(int pos, void* b);

    /// @brief Mainloop for the GUI, must be run on its own thread to prevent interference w/ backend. Handles drawing UI elements, inputs, etc. through an endless loop.
    void mainLoop();

    /// @brief Loads configuration file from SD UNIMPLEMENTED
    void loadConfFile();

};

//Global GUI instance 
extern GUI_Class GUI;

