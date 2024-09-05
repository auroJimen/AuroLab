#include <Arduino.h>
#include <M5Cardputer.h>
#include <M5GFX.h>
#include <WiFi.h>
#include "sprites.h"
#include "compassIcon.h"
#include "menuBackground.h"
#include "keyboardTask.h"

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
    String defText; ///< Text displayed on the text box when its first created
    String* text; ///< Pointer to the string where the text will be saved when a enter event is triggered
    coord pos; ///< Where the text box should be drawn
    coord size; ///< Size of the text box
    coord border; ///< Margin from the rectangle to the text (in both directions)
    M5GFX Display = M5Cardputer.Display; ///> Display object
    M5Canvas* parent; ///< Optional parent sprite object
    float textSize; ///< Text size multiplier for the body
    int textColour; ///< Text colour
    int backColour; ///< Background colour
    int cursorColour; ///< Cursor colour
    int dispLen; ///< Length of the maximum # of char that can be displayed @ a time
    coord cursorSize; ///< Size in px of the cursor rectangle
    public:
    /// @brief Constructor for the textBox class
    /// @param pos Coord position where the textBox will be drawn
    /// @param size Coord size of the textBox
    /// @param text Pointer to a string where the text will be saved when an enter event is triggered
    /// @param defText Text displayed on the box when it's created, def to ""
    /// @param parent Parent M5Canvas object to print to instead of directly to the screen (def to NULL)
    /// @param border Coord, the border margin from the rectanle to text in both directions
    /// @param textSize Float text size multiplayer, def to 1.5
    /// @param textColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    /// @param backColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to black
    /// @param cursorColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    textBox_Class(coord pos, coord size, String* text, String defText="", M5Canvas* parent = nullptr, coord border=coord(4, 6), 
     float textSize=1.1, int textColour = m5gfx::ili9341_colors::GREEN,
     int backColour = m5gfx::ili9341_colors::BLACK, int cursorColour = m5gfx::ili9341_colors::GREEN);
    /// @brief Draws or redraws the textBox on the screen
    void draw();
    /// @brief Enables the textInput and launches a keyboard listener if it isn't already running
    void enableTextInput();
    /// @brief Disables textInput and kills the keyboard listener process
    void disableTextInput();
    /// @brief Updates the contents of the displayed textBox to the current buffer contents
    void update();
    /// @brief Returns the value of enableTextInput
    /// @return Bool, enableTextInput
    bool getEnableTextInput();
};

///@brief Scrollable, selectable list class to construct menus
class list_Class{
    protected: //So that daughter classes can access them
    String title; ///< Title of the list
    int elementNum; ///< Number of elements on the list
    String* elementName; ///< Names of the list elements
    int pos; ///< Current highlighted position on the list
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
    ///@param origin Coordinates where the list rectangle will be drawn
    ///@param size Size of the list rectangle
    ///@param titleSize Float, text size of the title, defaults to 2.5
    ///@param textSize Float, text size of the list body, defaults to 1.0
    ///@param textColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    ///@param backColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to black
    ///@param highlightColour Int, colour as defined in the m5gfx::ili9341_colors space defaults to green
    ///@param border Coord, the border margin from the rectanle to text in both directions
    list_Class(String title, int elementNum, String* elementName,
     coord origin, coord size, float titleSize = 2.5, float textSize = 1.5, 
     int textColour = m5gfx::ili9341_colors::GREEN, int backColour = m5gfx::ili9341_colors::BLACK, 
     int highlightColour = m5gfx::ili9341_colors::GREEN, coord border = coord(6, 5));
    ///@brief Draws or redraws the list on the screen
    void draw();
    ///@brief Deletes the list instance UNIMPLEMENTED
    void del();
    ///@brief Scrolls the list to the given position if it's a valid one
    ///@param newPos Position we want to scroll to
    void scroll(int newPos);
    /// @brief Scrolls down one position
    void scrollDown();
    /// @brief Scrolls up one positon
    void scrollUp();
    /// @brief Handles the element selected event triggered by ENTER navSignal
    virtual void enterEvent();

    protected:
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
    /// @brief How many rows of text will the given string takes up
    /// @param textLen length of a string
    /// @param textSize Float, the text size multiplier used in M5GFX
    /// @param spriteWidth Width of the sprite in px
    /// @return Int,l number of rows the string takes up
    int rowsOccupied(int textLen, float textSize, int spriteWidth);
    
};

/// Auxiliary shared GUI related functions

/// @brief Calculates height in px of a line of text of given size 
/// @param textSize Float, the text size multiplier used in M5GFX
/// @return Height in px of the line
inline int rowSize(float textSize) {return floor(8*textSize);}
/// @brief Calculates length in px of a line of text of given size & character length
/// @param textSize Float, the text size multiplier used in M5GFX
/// @param length Length of the string in characters
/// @return Length of the string in pixel
inline int rowWidth(float textSize, int length) {return floor(length*6*textSize);}
/// @brief Calculates the maximum length of a String to be displayed in the given px w/ the given size
/// @param pxSize Int, width in px of the space were the string will be displayed
/// @param textSize Float, the text size multiplier used in M5GFX
/// @return Length of the maximum possible displayable stirng in characterts
inline int rowWidth(int pxSize, float textSize){return ceil(pxSize/(floor(6*textSize)));}

/// @brief Class for displaying the wifi connection menu, inherits from the list_Class & adds specific functionality
class wifiMenu_Class : public list_Class{
    private:
    uint8_t* encript;
    int32_t* RSSI;
    String* BSSID;
    int32_t* channel;
    public:
    /// @brief Constructor for the wifi menu class, creates an empty list with the correct name, size & handlers
    /// @param optHandler Handler for the option event
    /// @param enterHandler Handler for the option selected event;
    wifiMenu_Class();
    /// @brief Scans for available networks & initialaizes them as options
    void scan();
    /// @brief The app loop for the scan menu, draws to the screen, listens to input...
    void appLoop();
    /// @brief  Handles the element options event trigerred by OPTN navSignal
    void optnEvent();
    /// @brief Handles the element selected event (is defined on parent, overridden here)
    void enterEvent() override;
    /// @brief Frees up the dinamically allocated memory
    void del();
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
    TaskHandle_t topBarTask;
    /// @brief Starts up the GUI, launching it's execution thread and executing mainLoop()
    void begin();

    /// @brief Draws the splashscreen
    void splashScreen();

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

    /// @brief Updates the state of the icons on the topBar
    void updateTopBar();

};

/// @brief Task handler function, isolated since it mustn't be a class function
/// @param parameter Standar parameter for handler functions, a nullptr is passed if it is not needed
void GUIloop(void* parameter);

/// @brief Task andler function for the update topBar task (must be executed async with the rest of the GUI)
/// @param parameter Standar parameter for handler functions, a nullptr is passed if it is not needed
void topBarLoop(void* parameter);

/// @brief ISR for the timer interrupt, one a second, wakes up the update topBar task & 
/// changes the timeFlag state to perform timed events
/// @return 
void IRAM_ATTR timerISR();

//Global GUI instance 
extern GUI_Class GUI;
//Global variables related to keyboard
extern buffer_Class Buffer;
