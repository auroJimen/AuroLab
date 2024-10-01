//Contains fuctions related to the backend of the terminal app
//Should support ssh, possibly telnet, serial over usb and serial over uart trough port a

//Main screen should let user select the kind of terminal they wanna use
//Must work out the split between cores for the app 
//(gui should be processed @ 0 while the backend stuff should be @ one)


//Includes
#include "GUI.h" //Must be able to call the GUI library to write to screen
#include "libssh_esp32.h" //SSH libraries used
#include <libssh/libssh.h> //SSH libraries used

