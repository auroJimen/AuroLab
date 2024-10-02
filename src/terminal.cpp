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

int sshTerminal::connect(){

    //Attempting to start the ssh session
    this->session = ssh_new();
    if (this->session == NULL) {
        log_i("Session error");
        return sshState::SESSION_ERROR;
    }
    //Attempting to connect to user@host
    ssh_options_set(this->session, SSH_OPTIONS_HOST, this->hostname.c_str());
    ssh_options_set(this->session, SSH_OPTIONS_USER, this->user.c_str());
    if (ssh_connect(this->session) != SSH_OK) {
        log_i("Connection error");
        ssh_free(this->session);
        return sshState::CONNECTION_ERROR;
    }
    //Attempting log in if password was provided
    if (this->passphrase != String("")){
        if (ssh_userauth_password(this->session, NULL, this->passphrase.c_str()) !=
        SSH_AUTH_SUCCESS) {
            log_i("Log in error");
            ssh_disconnect(this->session);
            ssh_free(this->session);
            return sshState::AUTH_ERROR;
        }
    }
    //Attempting to open a channel
    this->channel = ssh_channel_new(this->session);
    if (this->channel == NULL || ssh_channel_open_session(this->channel) != SSH_OK) {
        log_i("SSH Channel open error.");
        ssh_disconnect(this->session);
        ssh_free(this->session);
        return sshState::CHANNEL_ERROR;
    }
    //Requesting PTY from channel
    if (ssh_channel_request_pty(channel) != SSH_OK) {
        log_i("SSH PTY request error.");
        ssh_channel_close(this->channel);
        ssh_channel_free(this->channel);
        ssh_disconnect(this->session);
        ssh_free(this->session);
        return sshState::PYT_ERROR;
    }
    //Requesting shell
    if (ssh_channel_request_shell(this->channel) != SSH_OK) {
        Serial.println("SSH Shell request error.");
        ssh_channel_close(this->channel);
        ssh_channel_free(this->channel);
        ssh_disconnect(this->session);
        ssh_free(this->session);
        return sshState::SHELL_ERROR;
    }
    log_i("SSH conection finalized");
    return sshState::SUCCESS;
}
