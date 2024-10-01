//Contains fuctions related to the backend of the terminal app
//Should support ssh, possibly telnet, serial over usb and serial over uart trough port a

//Main screen should let user select the kind of terminal they wanna use
//Must work out the split between cores for the app 
//(gui should be processed @ 0 while the backend stuff should be @ one)

#include "terminal.h"

//Base terminal class functions

terminal::terminal(){
    ///Constructor for the base terminal class
    this->history = *new String(); //Dynamically reserve the String class
};

//SSh terminal class functions

sshTerminal::sshTerminal(String host, String user, String pass) : terminal::terminal(){
    this->hostname = host;
    this->user = user;
    this->passphrase = pass;
}
