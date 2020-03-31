#ifndef MAIN_SCREEN_HPP
#define MAIN_SCREEN_HPP
#include "screen.hpp"
#include "RtMidi.h"
#include <vector>
class MainScreen {
private:
    bool pattern[8][8];
    RtMidiOut *port=0;
public:
    MainScreen(RtMidiOut *out);
    ~MainScreen();    
    std::vector<unsigned char> display();
    //   ???   ///
    void displayScreen();
};
#endif