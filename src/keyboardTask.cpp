#include "keyboardTask.h"

//buffer_Class functions
buffer_Class::buffer_Class(){
    this->size = DEF_BUFFER_SIZE_NAV_MODE;
    this->Mode = mode::nav; //Navigation mode
    this->data = (char*)calloc(this->size,1);
    this->cursor = 0;
    this->keyboardEnable = false; //Keyboard starts up disabled
    this->timer =  timerBegin(2, 80, true); //Create timer on core 1 (timers are 0,1,2,3 2 per core)
    this->signal = navSignal::NP;
}

buffer_Class::buffer_Class(mode Mode, int size){
    this->size = size;
    this->Mode = Mode;
    this->data = (char*)calloc(this->size,1);
    this->cursor = 0;
    this->keyboardEnable = false;
    this->timer =  timerBegin(2, 80, true); //Create timer on core 1 (timers are 0,1,2,3 2 per core)
    this->signal = navSignal::NP;
}

void buffer_Class::begin(bool keyboardEnable){
    BaseType_t key = xTaskCreatePinnedToCore(keyBoardLoop, "Keyboard thread", 10000, NULL, 0, &this->keyboardTask, 0); //Creates thread for the keyboard on core 0
    if (key != pdPASS) log_i("ERR");
    else log_i("Keyboard task created");
    this->keyboardEnable = keyboardEnable;
}

TaskHandle_t* buffer_Class::getTaskHandle(){
    return &this->keyboardTask;
}

hw_timer_t* buffer_Class::getHardwareTimer(){
    return this->timer;
}

char* buffer_Class::getData(){
    return this->data;
}

String buffer_Class::getDataStr(){
    String dataStr = "";
    for (int i = 0; i <this->size; i++){
        dataStr += this->data[i];
    }

    return dataStr;
}

void buffer_Class::del(){
    this->data[this->cursor] = ' ';
}

int buffer_Class::del(int cursor){
    if (cursor < 0 || cursor > Buffer.size) return -1;
    else {
        this->cursor = cursor;
        this->del();
        return 0;
    }
}

int buffer_Class::getBufferSize(){
    return this->size;
}

void buffer_Class::clearBuffer(){
    for (int i= 0; i < this->size; i++){
        this->data[i] = ' ';
    }
    Buffer.cursor = 0;
}

void buffer_Class::killTask(){
    log_i("Keyboard task & Buffer deleted");
    free(this->data);
    vTaskDelete(this->keyboardTask);
    timerDetachInterrupt(this->timer);
}


void keyBoardLoop(void* parameters) {
  log_i("Keyboard task created");
  //Create the interrupt we'll use to time how many times per second we check the keyboard input
  timerAttachInterrupt(Buffer.getHardwareTimer(), &keyBoardISR, false); //Atach interrupt to the timer
  timerAlarmWrite(Buffer.getHardwareTimer(), 50000, true); //Set the alarm that triggers the interrupt
  timerAlarmEnable(Buffer.getHardwareTimer()); //Enable the alarm
  //Suspend the current task (it'll resume from here once the interrupt is triggered)
  vTaskSuspend(NULL);
  //Infinite loop, each time xTaskResumeFromISR is called from within the ISR, a round of this loop will
  //be executed, succesive calls to resuming won't affect an already resumed task, & the loop suspends the task
  //once the round is finished
  for(;;){
    Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
    //Handle keyboard presses
    if (Buffer.Mode == mode::text) {
        //We are in text entry mode
        
        if (status.word.size() == 0) {
            //Special cases
            if (status.del) {
                if (status.fn) {
                    //supr / del
                    log_i("Supr pressed");
                    for(int i = Buffer.cursor+1; i < Buffer.getBufferSize() -1; i++){
                            Buffer.getData()[i] = Buffer.getData()[i+1];
                    }
                } else {
                    //Backspace
                    log_i("Backspace pressed");
                    if (Buffer.cursor != 0) {
                        Buffer.cursor --;
                        Buffer.getData()[Buffer.cursor] = ' ';
                    }
                    
                }
            } else if (status.tab){
                //Tab single press, insert \t
                log_i("Tab pressed");
                Buffer.getData()[Buffer.cursor] = '\t';
                Buffer.cursor++;
            } else if (status.enter){
                if (status.fn) Buffer.signal = navSignal::ENTER;
                else {
                    //Enter single press, insert \n
                    log_i("Enter pressed");
                    Buffer.getData()[Buffer.cursor] = '\n';
                    Buffer.cursor++;
                }
            } else if (status.alt || status.ctrl || status.fn || status.opt || status.shift || status.space) Buffer.signal = navSignal::NP;
            else log_i("Keyboard release");

            //Maybe implement opt, alt & ctrl? for text mode? 
            //Opt should bring out the conf menu for the programme,
            //Alt & ctrl should be used in some way for keyboard shortcuts, but they should be programme specific,
            //I'm not sure how i wanna implement that 
            //(maybe add a possible navSwicht style func given when creating the buffer?)
            //Other possible idea is fn+shift = BLOCK MAYUS, 
            //only if i find an elegant way to show it is active to the user
        } else {
            log_i("Keyboard pressed, text entry mode, word is %i char", status.word.size());
            for(int i= 0; i< status.word.size(); i++){
                if (Buffer.cursor == Buffer.getBufferSize()) Buffer.clearBuffer(); //Check if the buffer is full
                if (status.fn) {
                    //Nav controls for text editing
                    char aux = status.word[i];
                    Buffer.signal = navSwitch(aux);
                } else {
                    //Normal keys 
                    Buffer.getData()[Buffer.cursor] = status.word[i];
                    Buffer.cursor++;
                }
            }
        }
        log_i("Buffer: \"%s\"\n", Buffer.getDataStr().c_str());
        log_i("Nav signal: %i\n", Buffer.signal);
    } else {
        //we are in navigation mode
        if (status.word.size() ==1) {
            log_i("Keyboard pressed, menu navigation mode");
            char aux = status.word[0];
            Buffer.signal = navSwitch(aux);
        } else if (status.enter) {
            Buffer.signal = navSignal::ENTER;
            log_i("ENTER signal");
        } else if(status.ctrl){
            log_i("CTRL signal");
            Buffer.signal = navSignal::CTRL;
        } else if (status.opt){
            Buffer.signal = navSignal::OPT;
            log_i("OPT signal");
        } else if (status.alt||status.del||status.fn||
                  status.shift||status.space) Buffer.signal = navSignal::NP;
        else log_i("Keboard release");
        log_i("Nav signal: %i\n", Buffer.signal);
    }
    //Suspend itself until it's called again
    vTaskSuspend(NULL);
  }
}

navSignal navSwitch(char aux){
    switch(aux){
        case '`':
        case '~': log_i("Esc signal");
                  return navSignal::ESC;
        case ';':
        case ':': log_i("UP signal");
                  return navSignal::UP;
        case '.':
        case '>': log_i("DOWN signal");
                  return navSignal::DOWN;
        case ',':
        case '<': log_i("LEFT signal");
                  return navSignal::LEFT;
        case '/':
        case '?': log_i("RIGHT signal");
                  return navSignal::RIGHT;
        default: return navSignal::NP;
    }
}

void IRAM_ATTR keyBoardISR(){
  //log_i("Interrupt triggered");
  if (Buffer.keyboardEnable) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange()) xTaskResumeFromISR(*Buffer.getTaskHandle());
  }
}