#include "mainMenuGUI.h"

void mainMenu_Class::mainLoop(){
    for(;;){
        log_i("Hey there, I'm acutally running");
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void GUIloop(void* parameter){
    //This code runs on a separate core

    //Maybe fix latter so that we're not forever one func call deep for no string reason?
    //There must be a better way to do this??
    ((mainMenu_Class*)parameter)->mainLoop();

}