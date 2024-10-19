//Includes for using the common GUI library
#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "GUI/GUI.h"

#endif

extern GUI_Class GUI;

//Here everything about the Settings app front-end will be declared

//The settings app will control basic things such as WifiConnection, it should be able to generate & load
//the config file from the SD card for the setings to be persistent!!!!

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
    /// @return Bool, true if the task was completed, false if not
    bool enterEvent() override;
    ///@brief Attempts to connect to a networl with the given details, showing an animation on the GUI
    ///@param ssid SSID of the network we are connecting to
    ///@param password Password of the network
    ///@return Bool, wether the conection was succesfull
    bool connectToNetWork(String ssid, String password);
    /// @brief Frees up the dinamically allocated memory
    void del();
};
