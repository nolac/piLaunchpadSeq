#include "main_screen.hpp"
#include "RtMidi.h"
MainScreen::MainScreen(RtMidiOut *out){
    port=out;
    for(int row=0;row<8;row++){
        for(int column=0;column<8;column++){
            pattern[row][column]=0;
        }
    }
}

MainScreen::~MainScreen(){
    delete port;
}

std::vector<unsigned char> MainScreen::display(){
    std::vector<unsigned char> returnArray;
    for(int row=0;row<8;row++){
        for(int col=0;col<8;col++){
            unsigned char note=16*row+col;
            returnArray.push_back(0x90);
            returnArray.push_back(note);
            returnArray.push_back(pattern[row][col]==1?GREEN_HIGH:GREEN_LOW);
        }
    }
    return returnArray;
}

void MainScreen::displayScreen(){
    for(int row=0;row<8;row++){
        for(int col=0;col<8;col++){
            std::vector<unsigned char> returnArray;
            unsigned char note=16*row+col;
            returnArray.push_back(0x90);
            returnArray.push_back(note);
            returnArray.push_back(pattern[row][col]==1?GREEN_LOW:RED_LOW);
            port->sendMessage(&returnArray);
            returnArray={};
        }
    }
}