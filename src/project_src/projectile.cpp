#include "projectile.h"

projectile::projectile() : x(0), y(0), vx(0), vy(0) {}

projectile::~projectile() {}

void projectile::simulateProjectile(float initialv, float angle, int maxx, int maxy, bool withDrag, bool commonScale)
{

    cleardevice();
   
    
    double physicalRadius = 0.1;
    double radianAngle = angle * M_PI / 180.0;
    double timeStep = 0.000285;
    double maxHeight = (initialv * initialv * sin(radianAngle) * sin(radianAngle)) / (2 * G);
    double range = (initialv * initialv * sin(2 * radianAngle)) / G;
    double totalRange = range / (1 - COEFFICEINT_OF_RESTITUTION);
    double totalTime = totalRange / (initialv * cos(radianAngle));
    double totalEnergy = 0.5 * pow(initialv, 2);
    double totalProjectileTime = (2 * initialv * sin(radianAngle)) / G;
    int bounceCount = 0;

    std::chrono::steady_clock::time_point beginTime = std::chrono::steady_clock::now();

    vx = initialv * cos(radianAngle);
    vy = initialv * sin(radianAngle);

    int axisX1 = 50, axisX2 = maxx - 50, axisY2 = maxy / 3, axisY1 = maxy - 50;
    int xAxisRange = (maxx - 120) / 3;
    // std::cout<<xDivisionValue<<std::endl;
    int graph1X1 = 100, graph1X2 = graph1X1 + xAxisRange, graph2X1 = graph1X2 + 150, graph2X2 = graph2X1 + xAxisRange, graphY2 = 50, graphY1 = axisY2 +10;

    double xScale = commonScale ? calculateCommonScale(range * 2, axisX1, axisX2, axisY1, axisY2)
                                : calculateScale(range * 2, axisX1, axisX2, axisY1, axisY2, false);
    double yScale = commonScale ? xScale : calculateScale(maxHeight, axisX1, axisX2, axisY1, axisY2, true);
    

    double energyScale = graphScaling(totalEnergy, graphY1, graphY2, false);
    double velocityScale = graphScaling(vy * 2, graphY1, graphY2, false);
    double timeScale = graphScaling(totalTime + 0.5, graph1X1, graph1X2, true);

   
    x = 90;
    y = axisY1 - PROJECTILE_RADIUS;

    //   double graphScale = calculateCommonScale( graphX1, graphX2, graphY1, graphY2);

    drawScene( maxHeight, range, xScale, yScale, axisX1, axisX2, axisY1, axisY2);
    drawGraphAxes(graph1X1, graph1X2, graph2X1, graph2X2, graphY1, graphY2, energyScale, velocityScale, timeScale);

    double t = 0, actualMaxHeight = y, actualRange = 0;

    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x, y, PROJECTILE_RADIUS, PROJECTILE_RADIUS);

    double kineticEnergy = 0.5 * (vx * vx + vy * vy);
    double potentialEnergy = ((axisY1 - y - PROJECTILE_RADIUS) / yScale) * G;
    
    plotRealTimeGraph(t, vy, kineticEnergy, potentialEnergy, graph1X1, graph1X2, graph2X1, graph2X2, graphY1, graphY2, energyScale, velocityScale, timeScale, true);
    bool shouldContinue = true;

    while (shouldContinue)
    {
        

        
        if (withDrag)
            this->applyDragForce(physicalRadius, timeStep);
        else
            vy -= G * timeStep;

        x += vx * timeStep * xScale;
        y -= vy * timeStep * yScale;

        if (y >= (axisY1 - PROJECTILE_RADIUS))
        {
            y = axisY1 - PROJECTILE_RADIUS;
            vy = -vy * COEFFICEINT_OF_RESTITUTION;

            bounceCount++;

            if (fabs(vy) <= THRESHOLD_VELOCITY_Y)
            {
                shouldContinue = false;
                vy = 0;
            }
        }

        if (y <= actualMaxHeight)
            actualMaxHeight = y;

        actualRange = (x - 90) / xScale;
         kineticEnergy = 0.5 * (vx * vx + vy * vy);
         potentialEnergy = ((axisY1 - y - PROJECTILE_RADIUS) / yScale) * G;
         totalEnergy = kineticEnergy + potentialEnergy;

        drawProjectile();

        plotRealTimeGraph(t, vy, kineticEnergy, potentialEnergy,  graph1X1, graph1X2, graph2X1, graph2X2, graphY1, graphY2, energyScale, velocityScale, timeScale, false);
        // delay(timeStep * 1000);
        t += timeStep;
    }
    double elapsedTime = getFrameTime(beginTime, std::chrono::steady_clock::now());
    std::cout << "Elapsed Time: " << elapsedTime << std::endl;
    displayResults(t, withDrag, actualRange, actualMaxHeight, maxHeight, axisY1, yScale);
    
    
}

double projectile::calculateScale(double value, int axisX1, int axisX2, int axisY1, int axisY2, bool isHeight)
{

    double limit = isHeight ? (axisY1 - axisY2) : (axisX2 - axisX1);

    if (value < 15)
        return (limit / 25);
    else if (value < 50)
        return (limit / 50);
    double scale = isHeight ? (limit / (value * 1.2)) : (limit / (value * 1.5));
    return scale;
}

double projectile::calculateCommonScale(double value, int axisX1, int axisX2, int axisY1, int axisY2)
{

    double range = axisX2 - axisX1;
    if (value < 50)
        return range / 100;
    else if (value < 300)
        return range / (value * 2);
    return range / (value * 1.5);
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

    outtextxy(850, 450, result);
}

void projectile::drawScene( float maxHeight, float range, float xScale, float yScale, int axisX1, int axisX2, int axisY1, int axisY2)
{

    // Draw ground and launcher stand

    line(axisX1, axisY1, axisX2, axisY1);
    line(axisX1, axisY1, axisX1, axisY2);
    rectangle(60, axisY1 - 10, 78, axisY1);
    rectangle(103, axisY1 - 10, 120, axisY1);
    // Draw X and Y axis ticks
    this->drawTicks( xScale, yScale, axisX1, axisX2, axisY1, axisY2);
 
}
void projectile::drawGraphAxes(int graphEnergyX1, int graphEnergyX2, int graphVyX1, int graphVyX2, int graphY1, int graphY2, double energyScale, double velocityScale, double timeScale)
{
    // Draw X and Y axes

    line(graphEnergyX1, graphY1, graphEnergyX2, graphY1);
    line(graphVyX1, (int)((graphY1 + graphY2) / 2), graphVyX2, (int)((graphY1 + graphY2) / 2));
    line(graphEnergyX1, graphY1, graphEnergyX1, graphY2);
    line(graphVyX1, graphY1, graphVyX1, graphY2);
    
    
    drawTicks( timeScale , energyScale, graphEnergyX1, graphEnergyX2, graphY1, graphY2);
    drawVelocityGraphTicks(  graphVyX1, graphVyX2, graphY1, graphY2, velocityScale, timeScale);

    outtextxy(graphVyX2 + 10  , (graphY1 + graphY2) / 2 - 15 , const_cast<char*>("T(s)")); // X-axis label for time
    outtextxy(graphVyX1 + 10, graphY2 -30, const_cast<char*>("Vy(m/s)")); // Y-axis label for velocity

    outtextxy(graphEnergyX2 + 5 , graphY1 - 20 ,const_cast<char*>("T(s)") ); // X-axis label for time
    outtextxy(graphEnergyX1+ 10, graphY2 - 30, const_cast<char*>("Red = K.E., Blue = P.E.(J)")); // Y-axis label for velocity
    // drawGraphTicks();
}
void projectile::drawVelocityGraphTicks(int graphX1, int graphX2, int graphY1, int graphY2, double velocityScale, double timeScale)
{
    // X-axis (Time) ticks
    int numXTicks = 8;  // Number of ticks on the time axis
    int tickIntervalX = (graphX2 - graphX1) / numXTicks;

    for (int i = 0; i <= numXTicks; i++)
    {
        int tickX = graphX1 + i * tickIntervalX;
        line(tickX, (graphY1 + graphY2) / 2 - 5, tickX, (graphY1 + graphY2) / 2 + 5);

        char label[10];
        sprintf(label, "%.2f", (i * static_cast<float>(tickIntervalX) / timeScale));
        outtextxy(tickX - 10, (graphY1 + graphY2) / 2 + 10, label);
    }

    // Y-axis (Velocity) ticks
    int numYTicks = 3;  // Number of ticks on the velocity axis (half on each side)
    int tickIntervalY = (graphY1 - graphY2) / (2 * numYTicks);

    for (int i = -numYTicks; i <= numYTicks; i++)
    {
        int tickY = (graphY1 + graphY2) / 2 - i * tickIntervalY;
        line(graphX1 - 10, tickY, graphX1 + 5, tickY);

        char label[10];
        sprintf(label, "%.2f", (i * static_cast<float>(tickIntervalY) )/ velocityScale);

        // Position labels on the left of the Y-axis
        outtextxy(graphX1 - 50, tickY - 5, label);
    }
} 

void projectile::drawTicks( float xScale, float yScale, int axisX1, int axisX2, int axisY1, int axisY2)
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

double projectile::getFrameTime(std::chrono::steady_clock::time_point beginTime, std::chrono::steady_clock::time_point currentTime)
{
    auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - beginTime).count();

    return elapsedTime / 1000000.0;
}

double projectile::graphScaling(double value, int axis1, int axis2, bool isX)
{

    double scaleFactor = isX ? ((axis2 - axis1) / value) : ((axis1 - axis2) / value);
    return scaleFactor;
}

void projectile::plotRealTimeGraph(double t, double vy, double kineticEnergy, double potentialEnergy, int graph1X1, int graph1X2, int graph2X1,
                                    int graph2X2, int graphY1, int graphY2, double energyScale, double velocityScale, double timeScale, bool reset)
{
    static int prevTime1X = graph1X1, prevKineticY = graphY1, prevPotentialY = graphY1;
    static int prevTime2X = graph2X1, prevVelocityY = static_cast<int>((graphY1 + graphY2) / 2);
    

    if (reset)
    {
        prevTime1X = graph1X1;
        prevKineticY = graphY1;
        prevPotentialY = graphY1;
        prevTime2X = graph2X1;
        prevVelocityY = (graphY1 + graphY2) / 2;
    }
    // Save the current color
    int savedColor = getcolor();

    // Plot Kinetic and Potential Energy vs Time
    int time1X = graph1X1 + static_cast<int>(t * timeScale);
    int kineticY = graphY1 - static_cast<int>(kineticEnergy * energyScale);
    int potentialY = graphY1 - static_cast<int>(potentialEnergy * energyScale);

    if (time1X < graph1X2)
    {
        // Draw lines for kinetic energy
        setcolor(RED);
        line(prevTime1X, prevKineticY, time1X, kineticY);

        // Draw lines for potential energy
        setcolor(BLUE);
        line(prevTime1X, prevPotentialY, time1X, potentialY);

        // Update previous points
        prevTime1X = time1X;
        prevKineticY = kineticY;
        prevPotentialY = potentialY;
    }

    // Plot Velocity vs Time
    int time2X = graph2X1 + static_cast<int>(t * timeScale);
    int velocityY = static_cast<int>((graphY1 + graphY2) / 2) - static_cast<int>(vy * velocityScale);

    if (time2X < graph2X2)
    {
        // Draw lines for vertical velocity
        setcolor(GREEN);
        line(prevTime2X, prevVelocityY, time2X, velocityY);

        // Update previous points
        prevTime2X = time2X;
        prevVelocityY = velocityY;
    }

    // Restore the saved color
    setcolor(savedColor);
}

