#include <graphics.h>
#include <iostream>
#include <cmath>
#include <conio.h> //kbhit
#include <stdlib.h> //atof
#include <mainMenu.h>
#include <windows.h>

int main() {
    int gd = DETECT, gm;
    int screenWidth =  round(GetSystemMetrics(SM_CXSCREEN) ) ;
    int screenHeight = round(GetSystemMetrics(SM_CYSCREEN) );
    

    
    initwindow(screenWidth, screenHeight, "Projectile Simulator");
  //  std::cout<<(float)screenWidth<<(float)screenHeight<<std::endl;
   // initgraph(&gd, &gm, NULL);
    mainMenu menu;
    
    menu.displayMenu();
    closegraph();
    return 0;
    
}

