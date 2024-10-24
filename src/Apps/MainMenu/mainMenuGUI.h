//Includes for using the common GUI library
#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "GUI/GUI.h"

#endif
#include "Apps/Terminal/terminalGUI.h"
extern GUI_Class GUI;

//Here everything about the Main menu app frontend will be declared

//Should start building the main menu soon!!!
//Main menu is an app since we're already defining apps as objects that make use of the GUI class to do specific shit
//all apps share common elements & live on the "app space" the rectangle bellow the topBar

//I've moved the task handle here so that i rememeber to move the GUI task creation to this file

class mainMenu_Class : public APP_Class {
    public:
    void mainLoop();
};

//Must create the mainloop for the GUI thread here!
///@brief The method that the mainMenuApp gui thread runs
///@param parameter The standar parameter for a funtion w/ this prototype, in this case we will pass a ref to the mainMenuAPP object we want to run
void GUIloop(void* parameter);