#include "projectile.h"

projectile::projectile() : x(0), y(0), vx(0), vy(0) {}

projectile::~projectile() {}

void projectile::simulateProjectile(float initialv, float angle, int maxx, int maxy, bool withDrag, bool commonScale)
{

    cleardevice();
    double physicalRadius = 0.1; 
    double radianAngle = angle * M_PI / 180.0;
    double timeStep = withDrag ? 0.01 : 0.05;
    double maxHeight = (initialv * initialv * sin(radianAngle) * sin(radianAngle)) / (2 * G);
    double range = (initialv * initialv * sin(2 * radianAngle)) / G;
    double totalTime = (2 * initialv * sin(radianAngle)) / G;
   
    int bounceCount = 0;
    double axisX1 = 50, axisX2 = maxx - 50, axisY1 = maxy - 50, axisY2 = 50;
    double xScale, yScale;
    if(commonScale)
    {
        xScale = this->calculateCommonScale(range* 2, axisX1, axisX2, axisY1, axisY2);
        yScale = xScale;
    }
    else{
     xScale = this->calculateScale(range , axisX1, axisX2, axisY1, axisY2, false);
     yScale = this->calculateScale(maxHeight, axisX1, axisX2, axisY1, axisY2, true);
    }

    std::cout << "Max Height (no drag): " << maxHeight << " m" << std::endl;
    std::cout << "Range (no drag): " << range << " m" << std::endl;

    x = 90;
    y = axisY1 - PROJECTILE_RADIUS;
    vx = initialv * cos(radianAngle);
    vy = initialv * sin(radianAngle);

    this->drawScene(maxx, maxy, maxHeight, range, xScale, yScale, axisX1, axisX2, axisY1, axisY2);

    double t = 0, actualMaxHeight = y, actualRange = 0;
    fillellipse(x, y, PROJECTILE_RADIUS, PROJECTILE_RADIUS);

    bool shouldContinue = true;


  //  while ((y < (axisY1 - PROJECTILE_RADIUS) || abs(vy) > THRESHOLD_VELOCITY) && (bounceCount < MAX_BOUNCES))
    
  while(shouldContinue)
  {

       // x += vx * timeStep * xScale;
      //  y -= vy * timeStep * yScale;

        if (withDrag)
        {
            this->applyDragForce(physicalRadius, timeStep);
        }
        else
        {
            vy -= G * timeStep;
        }
        x += vx * timeStep * xScale;
        y -= vy * timeStep * yScale;

        if (y >= (axisY1 - PROJECTILE_RADIUS))
        {
            y = axisY1 - PROJECTILE_RADIUS;
            vy = -vy * COEFFICEINT_OF_RESTITUTION;
          // vx = vx * COEFFICEINT_OF_FRICTION;
            bounceCount++;

            if (abs(vy) < THRESHOLD_VELOCITY)
            {
                vy = 0;
            }
        }

        if (y <= actualMaxHeight)
        {
            actualMaxHeight = y;
        }

        actualRange = (x - 90) / xScale;

        this->drawProjectile();
        delay((totalTime / (range / (vx * timeStep))) * 1000);
        t += timeStep;
        if(bounceCount >= MAX_BOUNCES)
        {
            shouldContinue = false;
        }
        
    }

    this->displayResults(t, withDrag, actualRange, actualMaxHeight, maxHeight, axisY1, yScale);
}

double projectile::calculateScale(double value, int axisX1, int axisX2, int axisY1, int axisY2, bool isHeight)
{

   
    if (isHeight)
    {
        if (value < 50)
        {
            return (axisY1 - axisY2) / 60;
        }
        else if (value > 50 && value < 300)
        {
            return (axisY1 - axisY2) / (value * 1.2);
        }
       
        else
        {
            return (axisY1 - axisY2) / (value* 1.2);
        }
    }
    else
    {
        if (value < 50)
        {
            return (axisX2 - axisX1) / 100;
        }
        else if (value > 50 && value < 300)
        {
            return (axisX2 - axisX1) / (value * 2);
        }
       
        else
        {
            return (axisX2 - axisX1) / (value * 1.5);
        }
    }
}

 double projectile::calculateCommonScale(double value, int axisX1, int axisX2, int axisY1, int axisY2)
{
    
    if (value < 50)
    {
        return (axisX2 - axisX1) / 100;
    }
    else if (value > 50 && value < 300)
    {
        return (axisX2 - axisX1) / (value * 2);
    }
   
    else
    {
        return (axisX2 - axisX1) / (value * 1.5);
    }
}


void projectile::applyDragForce(double physicalRadius, double timeStep)
{

    double v = sqrt(vx * vx + vy * vy);
    double dragForce = 0.5 * DRAG_COEFFICIENT * RHO * M_PI * pow(physicalRadius, 2) * v * v;
    double ax = -dragForce * (vx / v);
    double ay = -G - dragForce * (vy / v);

    vx += ax * timeStep;
    vy += ay * timeStep;
}

void projectile::drawProjectile()
{

    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y, PROJECTILE_RADIUS, PROJECTILE_RADIUS);
}

void projectile::displayResults(double t, bool withDrag, double actualRange, double actualMaxHeight, double maxHeight, int axisY1, double yScale)
{

    char result[100];
    sprintf(result, "Time: %.2f s  Range: %.2f m  Max Height: %.2f m (with Drag: %s)",
            t, actualRange,
            withDrag ? (axisY1 - actualMaxHeight) / yScale : maxHeight,
            withDrag ? "Yes" : "No");

    outtextxy(100, 50, result);
}

void projectile::drawScene(int maxx, int maxy, float maxHeight, float range, float xScale, float yScale, int axisX1, int axisX2, int axisY1, int axisY2)
{

    // Draw ground and launcher stand

    line(axisX1, axisY1, axisX2, axisY1);
    line(axisX1, axisY1, axisX1, axisY2);
    rectangle(60, axisY1 - 10, 78, axisY1);
    rectangle(103, axisY1 - 10, 120, axisY1);
    // Draw X and Y axis ticks
    this->drawTicks(maxx, xScale, yScale, axisX1, axisX2, axisY1, axisY2);
}

void projectile::drawTicks(int maxx, float xScale, float yScale, int axisX1, int axisX2, int axisY1, int axisY2)
{
    // X-axis ticks

    int numXTicks = 8;
    int tickIntervalX = (axisX2 - 90) / numXTicks;
    for (int i = 0; i <= numXTicks; i++)
    {
        int tickX = 90 + i * tickIntervalX;
        line(tickX, axisY1, tickX, axisY1 - 10);

        char label[10];
        sprintf(label, "%.2f ", (i * static_cast<float>(tickIntervalX) / xScale));
        outtextxy(tickX - 10, axisY1 + 5, label);
    }

    // Y-axis ticks
    int numYTicks = 6;
    int tickIntervalY = (axisY1 - axisY2) / numYTicks;
    for (int i = 1; i <= numYTicks; i++)
    {
        int tickY = axisY1 - i * tickIntervalY;
        line(47, tickY, 52, tickY);

        char label[10];
        sprintf(label, "%.2f ", (i * static_cast<float>(tickIntervalY) / yScale));
        outtextxy(0, tickY - 5, label);
    }
}
