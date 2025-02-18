#ifndef USERINPUT_H
#define USERINPUT_H
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <cstdlib>
#include "projectile.h"

class userInput{
    
   
    public:
   userInput();
    
    ~userInput();
    float angle;
    float speed;
    
    void getUserInput(bool, int, int);
    
   
    double getValue(char *);
    bool checkButtonClick(int, int , int ,int , int , int );
};
#endif