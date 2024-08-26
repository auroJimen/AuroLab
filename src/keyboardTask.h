#include <Arduino.h>
#include <M5Cardputer.h>
//Functions that handle the independent keyboard ex thread & its communication with other threads
enum mode {text, nav};
enum navSignal {ESC, DEL, UP, DOWN, LEFT, RIGHT,ENTER, OPT, NP};
/// @brief A struct with all needed elements & functions to create & control a 
///50 character buffer htat serves as the connection between the leyboard thread
///& the rest of the system
class buffer_Class {
  private:
  TaskHandle_t keyboardTask; ///< Task handle for the keyboard execution thread
  hw_timer_t* timer;
  int size;
  char* data; ///< Pointer to the char buffer
  public:
  int cursor; ///< Current position of the cursor on the array
  bool keyboardEnable; ///< Wether the keyboard data entry is currently enabled
  mode Mode; ///< Wether we are in text entry or navigation mode
  navSignal signal; ///< Last processed nav signal

  public:
  /// @brief Def constructor, 50 char buffer
  buffer_Class();
  /// @brief Constructor with especified buffer size
  buffer_Class(int size);
  /// @brief Getter function for the keyboardTask TaskHandle_t
  /// @return A pointer to the keyboardTask
  TaskHandle_t* getTaskHandle();
  /// @brief Getter function to the hardware timer used by the interrupt
  /// @return  A pointer to the correct hardware timer
  hw_timer_t* getHardwareTimer();
  /// @brief Creates the keyboard class, passes itself to it
  /// @param keyboardEnable Wether the keyboard is enabled, defs to true
  /// @param Mode Enum, marks wether the keyboard is in text entry or navigation mode, defs to navigation
  void begin(bool keyboardEnable = true, mode Mode = mode::nav);
  /// @brief Getter function for the data char array
  /// @return A pointer to the 50 characters char array data
  char* getData();
  /// @brief Getter function for the data char array as a String
  /// @return A string containing the whole buffer
  String getDataStr();
    /// @brief Getter function for the size of the buffer
  int getBufferSize();
  /// @brief Deletes the character @ current cursor pos
  void del();
  /// @brief Moves the cursor to the given os & deletes that character
  /// @param cursor A valid cursor pos (0-49)
  /// @return 0 if successfull -1 otherwise
  int del(int cursor);
  /// @brief Clears the buffer & moves the cursor to the first pos
  void clearBuffer();
  /// @brief Kills the current Keyboard task safely
  void killTask();
};


/// @brief Task handler for the keyboard task. 
/// Runs the first section when called, creating the timer interrupt & pausing itself, 
/// after that, each timer interrupt will make it wake up, execute one loop pass & pause again, thus 
/// updating the buffer
/// @param parameters Standar parameters passed as a void pointer, in this case it mus be cast to buffer*
void keyBoardLoop(void* parameters);

/// @brief ISR for the timer interrupt, checks wether the keyboard is enabled, if so, updates it and
/// wakes up the keyboard thread if there's new data to add to the buffer
/// @return 
void IRAM_ATTR keyBoardISR();

/// @brief A simple switch statement that trnslates keystrokes to the proper nav signals
/// @param aux A char character
/// @return A nav signal (enum)
navSignal navSwitch(char aux);

extern buffer_Class Buffer;