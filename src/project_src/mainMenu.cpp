// mainMenu.cpp
#include "mainMenu.h"

mainMenu::mainMenu(){}
    
mainMenu::~mainMenu(){}

void mainMenu::displayMenu()
{
    maxx= getmaxx(), maxy = getmaxy();
    userInput userin;
    projectile p;

    while(true){
        cleardevice();
        setbkcolor(LIGHTBLUE);
        cleardevice();
        setcolor(BLACK);

        rectangle(50, 50, this->maxx - 50, this->maxy - 50);

        settextstyle(BOLD_FONT, HORIZ_DIR, 4);
        outtextxy(this->maxx / 2 - 200, 80, const_cast<char *>("Projectile Simulator"));

        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        outtextxy(this->maxx / 2 - 140, 140, const_cast<char *>("Choose an option to start:"));

        setcolor(BLACK);
        rectangle(this->maxx / 2 - 100, 220, this->maxx / 2 + 100, 270);
        outtextxy(this->maxx / 2 - 80, 240, const_cast<char *>("Start (No Drag)"));
        
        rectangle(this->maxx / 2 - 100, 300, this->maxx / 2 + 100, 350);
        outtextxy(this->maxx / 2 - 80, 320, const_cast<char *>("Start (With Drag)"));

        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(this->maxx / 2 - 150, this->maxy - 100, const_cast<char *>("Click on a button to begin simulation."));

        while (!ismouseclick(WM_LBUTTONDOWN))
        {
            delay(100);
        }
        int x,y;

        getmouseclick(WM_LBUTTONDOWN, x, y);

        bool withDrag= false;
        if (userin.checkButtonClick(x, y,this->maxx / 2 - 100, 220, this->maxx / 2 + 100, 270)) {
            withDrag= false;
        } else if (userin.checkButtonClick(x, y, this->maxx / 2 - 100, 300, this->maxx / 2 + 100, 350)) {
            withDrag=true;
        }
        else{
            continue;
        }
        userin.getUserInput(withDrag, maxx, maxy);
        
        
        outtextxy(100, 80, const_cast<char*>("Press any key to return to menu..."));
        getch();
    }
    
}

// mainMenu.cpp
