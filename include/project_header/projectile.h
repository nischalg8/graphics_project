#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "projectile.h"
#include <graphics.h>
#include <cmath>
#include <cstdio>
#include "userInput.h"
#include <chrono>

#define PROJECTILE_RADIUS 5  // Define radius of the projectile
#define G 9.8  // Gravitational acceleration
#define RHO 1.225  // Air density at sea level (kg/m^3)
#define DRAG_COEFFICIENT 0.47  // Drag coefficient for a sphere
#define COEFFICEINT_OF_FRICTION 0.45  // Coefficient of friction
#define COEFFICEINT_OF_RESTITUTION 0.5
#define MAX_BOUNCES 10

#define THRESHOLD_VELOCITY_Y 0.0163333  // Coefficient of restitution
class projectile {
public:
    projectile();
    ~projectile();

    // actual simulation function
    void simulateProjectile(float initialv, float angle, int maxx, int maxy, bool withDrag, bool);

private:
    double x, y, vx, vy;  // postion and velocity

    // calculateScale function to calculate the scale factor for the simulation
    double calculateScale(double, int, int , int, int , bool);

    double calculateCommonScale(double, int, int, int, int);
    //applies drag force if user selects with drag
    void applyDragForce(double,  double);

    // draws projectile on the screen (x,y )
    void drawProjectile();

    // display resulst in screen
    void displayResults(double , bool , double , double , double,int, double);

    // draw scene with axes 
    void drawScene(int , int , float , float , float , float , int , int , int , int);

    // ticks on axes 
    void drawTicks(int ,  float , float , int , int , int , int);
};

#endif // PROJECTILE_H
