<a id="readme-top"></a>
<h3 align="center">AuroLab</h3>
AuroLab is a personal project developed to turn the Cardputer into an usefull device for my daily work (with a nice UI that caters 100% to me).

The firmware is currently being developed in C++, through PlatformIO for VS Code.

Current feature Roadmap:

<!-- ROADMAP -->
## Roadmap

- [ ] Main menu
    - [x] Top Bar
        - [x] Battery indicator
            - [x] Icon
                -[x] Asset created
                -[x] Code implementation
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
    - [ ] Simple IR remote app
    - [ ] RFID app?
    - [ ] Car data logger, fuel efficiency?
    - [ ] Configuration App
        - [ ] Icon asset created
        - [ ] Wifi selection menu
            - [ ] Scanning
                - [ ] Scanning animation asset created
                - [x] Code implementation
            - [x] Display available networks via list
            - [ ] Connect
- [] General GUI/IO
    - [ ] Interactive list class
        - [x] Create & display scrollable class
        - [x] Scroll event
            - [x] Event implemented in code
            - [ ] Trigger event from keyboard input
        - [ ] Select event
            - [ ] Event implemented in code
                - [x] Event handler function assigned
            - [ ] Trigger event from keyboard
    - [ ] Input via keyboard & G0 button
        - Work in progress
- Future objectives:
    - [ ] Port to non M5 alternative libraries
