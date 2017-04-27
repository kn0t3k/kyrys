/*
 * This file obtains some MAN or help pages for both client and server
 */


#include <string>
#include <iostream>

void printClientWelcomePage() {
    std::cout << "Welcome in Kyrys client"
            ""
            "==================================="
            "" << std::endl;
}

void printClientCommandList() {
    std::cout << "Command list:"
            ""
            "register - Registration of new client account"
            "login    - Login of existing account" << std::endl;
}

void printServerWelcomePage() {
    //todo
}

void printServerCommandList() {
    //todo
}