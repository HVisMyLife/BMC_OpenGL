#ifndef MY_CLASS_H // include guard
#define MY_CLASS_H

#include <iostream>
#include <tuple>

class Player {
    private:
    int x;
    int y;
    uint16_t spd;
    uint16_t acc;
        
    public:
    Player(int, int, int, int);
    void setPosition(int,int);
    std::tuple<int, int> getPosition();
};

#endif