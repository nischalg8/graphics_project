#include <graphics.h>
#include <iostream>
#include <cmath>
#include <conio.h> //kbhit
#include <stdlib.h> //atof
#include <mainMenu.h>
#include <windows.h>

int main() {
    int gd = DETECT, gm;
    int screenwidth = GetSystemMetrics(SM_CXSCREEN);
    int screenheight = GetSystemMetrics(SM_CYSCREEN);

    initwindow(screenwidth, screenheight, "This is window");
        
   // initgraph(&gd, &gm, NULL);
    mainMenu menu;
    
    menu.displayMenu();
    closegraph();
    return 0;
    
}

