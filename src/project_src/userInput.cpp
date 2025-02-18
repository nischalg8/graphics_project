#include <userInput.h>

userInput::userInput()
{}
userInput::~userInput()
{}
bool userInput::checkButtonClick(int x, int y, int x1, int y1, int x2, int y2) {
    return (x > x1 && x < x2 && y > y1 && y < y2);
}

double userInput::getValue(char *str)
{ 
    
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    char valueStr[10]="";
    int index = 0;
    bool showCursor = true;
    while (true) {
        cleardevice();
        outtextxy(100, 100, str);
        rectangle(100, 127, 300, 160);
        outtextxy(110, 130, valueStr);
       
        if(showCursor)
        {
            outtextxy(110 + textwidth(valueStr), 130, const_cast<char*>("_"));
        }
        delay(200);
        showCursor = !showCursor;
        if(kbhit()){
        char ch = getch();
        if (ch == 13) break;
        if (ch == 8 && index > 0) {
            valueStr[--index] = '\0';
        } else if ((ch >= '0' && ch <= '9') || ch == '.') {
            valueStr[index++] = ch;
            valueStr[index] = '\0';
        }
       }
    }
    double value = atof(valueStr);
    return value;

}
void userInput::getUserInput(bool withDrag,int maxx, int maxy)
{
    cleardevice();
    
    int x, y;
    projectile p;
    // Input for Speed
    this->speed= this->getValue(const_cast<char*>("Enter Initial Speed (m/s):"));
    this->angle= this->getValue(const_cast<char*>("Enter the Launch Angle:"));

    setcolor(BLACK);
    rectangle(300, 400, 450, 450);
    outtextxy(310, 410, const_cast<char *>("Common Scale"));
        
    rectangle(600, 400, 820, 450);
    outtextxy(610, 410, const_cast<char *>("Adjust Scale(X and Y)"));

    while (!ismouseclick(WM_LBUTTONDOWN))
        {
            delay(100);
        
         getmouseclick(WM_LBUTTONDOWN, x, y);
         outtextxy(250, 470, const_cast<char*>("Adjust Scale(X and Y) adjusts the projectile path to screen size but may distort the launch angle"));
        bool commonScale= false;
        if (this->checkButtonClick(x, y, 300, 400, 500, 450)) {
           commonScale= true;
           p.simulateProjectile(this->speed,this->angle, maxx, maxy, withDrag, commonScale);
           return;
        } else if (this->checkButtonClick(x, y, 600, 400, 800, 450)) {
           commonScale= false;
           p.simulateProjectile(this->speed,this->angle, maxx, maxy, withDrag, commonScale);
           return;
        }
        else{
            continue;
        }
    }

    
}

