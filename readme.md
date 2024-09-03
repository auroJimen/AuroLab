<a id="readme-top"></a>
<h1 align="center">AuroLab</h1>
<p align="center">AuroLab is a personal project developed to turn the Cardputer into an usefull device for my daily work (with a nice UI that caters 100% to me).</p>

<p align="center">The firmware is currently being developed in C++, through PlatformIO for VS Code.</p>

<!-- ROADMAP -->
## Current feature roadmap

- Main menu
    - [x] Top Bar
        - [x] Battery indicator
            - [x] Icon
                - [x] Asset created
                - [x] Code implementation
            - [x] Percentage display
            - [ ] BUG: Disapearence when connected via USB
        - [ ] SD card indicator
            - [x] Icon asset created
            - [ ] Code implementation
        - [ ] WiFi indicator
            - [ ] Icon asset created
            - [ ] Code implementation
    - [ ] Terminal App
        - [x] Icon asset created
        - [ ] Terminal core app
        - [ ] SSH support
        - [ ] Telnet support
        - [ ] UART support through grove?
    - [ ] I2C logger app
    - [ ] Bi (Vi clone)
    - [ ] Simple IR remote app
    - [ ] RFID app?
    - [ ] Car data logger, fuel efficiency?
    - [ ] Configuration App
        - [ ] Icon asset created
        - [x] Wifi selection menu
            - [x] Scanning
                - [x] Scanning animation asset created
                - [x] Code implementation
            - [x] Display available networks via list
                - [x] Display wifi info when OPTN is presed
            - [ ] Connect
- General GUI/IO
    - [x] Interactive list class
        - [x] Create & display scrollable class
        - [x] Scroll event
            - [x] Event implemented in code
            - [x] Trigger event from keyboard input
        - [x] Select event
            - [x] Event implemented in code
                - [x] Event handler function assigned
            - [x] Trigger event from keyboard
    - [x] Input via keyboard & G0 button
        - [ ] G0 button support
        - [x] Asincronous keyboard buffer
            - [x] Customizable size
            - [x] Comunication with other execution threads
            - [x] Text entry mode with support for control characters
                - [x] Cursor movement support
                    - [x] Insert text (standard) mode
                    - [x] Overwrite text when OPT is pressed
            - [x] Navigtion mode for menus
    - [ ] Interactive text box class
        - [x] Custom size
        - [x] Async update (from the Keyboard thread)
        - [ ] GUI editing with cursor movements
            - Solving bugs with proper cursor placement and text rendering
- Future objectives:
    - [ ] Port to non M5 alternative libraries
