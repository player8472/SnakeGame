#pragma once
#ifndef SDL
#include "SDL.h"
#endif // !SDL

#ifndef cstdlib
#include <cstdlib>
#endif // !cstdlib

#ifndef uint
typedef unsigned int uint;
#endif // !uint
enum EatableType{APPLE};

// An abstract class
class Eatable {
public:
    inline Eatable(uint x, uint y) {
        this->x = x;
        this->y = y;
    }
    inline Eatable(uint width,uint height,uint usablePercent) {
        this->x = (rand() % (width * usablePercent/100)) + (width * (100 - usablePercent) / 200);
        this->y = (rand() % (height * usablePercent/100)) + (height * (100 - usablePercent) / 200);
    }
    virtual EatableType getEatableType() = 0;
    virtual Uint32 tickToDie() = 0;
    virtual uint getScore() = 0;
    virtual int getX() {
        return x;
    }
    virtual int getY() {
        return y;
    }
    virtual int getNutrition() = 0;
protected:
    uint x;
    uint y;
};

class Apple:public Eatable {
public:
    inline Apple(uint x, uint y):Eatable(x,y) {}
    inline Apple(uint width, uint height, uint usablePercent) : Eatable(width, height, usablePercent){}
    inline int getX() {
        return Eatable::getX();
    }
    inline int getY() {
        return Eatable::getY();
    }
    inline EatableType getEatableType() {
        return APPLE;
    }
    inline uint getScore() { return 1; }
    Uint32 tickToDie() {
        return UINT32_MAX;
    }
    inline int getNutrition() {
        return 1;
    }
};