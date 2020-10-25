#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Vision.h"

class gCodeInterpreter
{
private:

    // Reading and Interpreting GCode
    std::string fileLocation;
    void readFile(std::string str);

    // Visualisation
    void visualise(char axis, double value, bool positive);
    bool latticeVisualisation[480][640];

    // Scalability
    int scale = 1;
    int thickness = 1;

    // Starting position centre of grid
    int previousXcordinate = 320;
    int previousYcordinate = 240;

public:

    // Reading and Interpreting G Code
    std::vector <std::string> data;
    void loadFile(std::string str);
    void interpret();

    // Visualisation
    void display();

    // Calibration
    void setScale(float top_ppm, float bot_ppm);
    void setThickness();
    void setStartingX();
    void setStartingY();
};
