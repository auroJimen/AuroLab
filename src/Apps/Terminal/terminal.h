//Contains fuctions related to the backend of the terminal app
//Should support ssh, possibly telnet, serial over usb and serial over uart trough port a

//Main screen should let user select the kind of terminal they wanna use
//Must work out the split between cores for the app 
//(gui should be processed @ 0 while the backend stuff should be @ one)


//Includes
#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "GUI/GUI.h" //Must be able to call the GUI library to write to screen & keyboard

#endif
#include "libssh_esp32.h" //SSH libraries used
#include <libssh/libssh.h> //SSH libraries used

enum terminalType {ssh, telnet, USBserial, UARTserial, i2c, btle, localShell};

/// @brief The parent terminal that will contains funcions shared by all terminal types
class terminal {
    protected:
    //Common elements of all terminal classes are defined here
    String history; ///> Contains everything that's been sent & recieved in a given terminal session

    public:
    /// @brief Constructor for the parent class
    terminal();
    //Prototype functions that must be virtual so that daughter classes define actual behaviour
    /// @brief Starts up the terminal session & launches the terminal GUI
    /// @returns Int wether the conection was succesfull or an error ocurred (via a enum defined for every terminal type)
    virtual int connect();

};

enum sshState {SUCCESS, SESSION_ERROR, CONNECTION_ERROR, AUTH_ERROR, CHANNEL_ERROR, PYT_ERROR, SHELL_ERROR};

/// @brief Terminal mode for ssh
class sshTerminal: public terminal {
    protected: 
    String hostname;
    String user;   
    String passphrase;

    ssh_session session;
    ssh_channel channel;

    public:
    /// @brief Constructor for the ssh terminal class
    /// @param host The hostname or IP of the target device
    /// @param user The username of the target device
    /// @param pass The user password of the target device
    sshTerminal(String host, String user = String(""), String pass = String(""));
    /// @brief Starts up the terminal session, attempting to connect to host if launches terminal GUI
    /// @return Bool, true if the conection was succesfull
    int connect() override;

};