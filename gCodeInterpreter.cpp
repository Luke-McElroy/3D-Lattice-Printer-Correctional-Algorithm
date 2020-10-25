#include <fstream>
#include "gCodeInterpreter.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

// ---------- Read GCode ---------- \\

// Set method for private fileLocation
void gCodeInterpreter::loadFile(std::string str) {
    fileLocation = str;
    readFile(fileLocation);
}

// Read file from specified location var, into vector 'data'
void gCodeInterpreter::readFile(std::string str) {

    std::ifstream readFile;
    readFile.open(fileLocation);

    if (!readFile.good()) {
        std::string err = "----- ERROR OPENING FILE -----\nFILE NAME: " + fileLocation + "\n";
        throw err;
    }
    else {
        std::cout << "READ GOOD: " << fileLocation << std::endl;
    }

    std::string gCodeCommand = "";
    while (!readFile.eof()) {
        gCodeCommand = "";
        readFile >> gCodeCommand;
        data.push_back(gCodeCommand);
    }
}

// ---------- Interpret GCode ---------- \\

// Reads x and y values
void gCodeInterpreter::interpret() {

    // set all values of latticeVisualisation to 0
    for (int i = 0; i < 480; i++) {
        for (int j = 0; j < 640; j++) {
            latticeVisualisation[i][j] = false;
        }
    }

    for (int i = 0; i < data.size(); i++) {
        double d;
        // check if first char is x or y
        // if x add to 2D array this way
        if (data[i].find('X') != std::string::npos) {
            // Checks movements extracts not reposition
            if (data[i + (int)1.0].find('E') != std::string::npos) {
                // extracts double
                const char* cstr = data[i].c_str();
                sscanf_s(cstr, "%*[^0-9]%lf", &d);
                // checks if the value was negative
                if (data[i].find('-') != std::string::npos) {
                    visualise('X', d, false);
                }
                else {
                    visualise('X', d, true);
                }
            }
        }
        // if y add to 2D array this way
        if (data[i].find('Y') != std::string::npos) {
            if (data[i + (int)1.0].find('E') != std::string::npos) {
                // extracts double
                const char* cstr = data[i].c_str();
                sscanf_s(cstr, "%*[^0-9]%lf", &d);
                // checks if the value was negative
                if (data[i].find('-') != std::string::npos) {
                    visualise('Y', d, false);
                }
                else {
                    visualise('Y', d, true);
                }
            }
        }
    }
}

// Populates 2D array
void gCodeInterpreter::visualise(char axis, double value, bool positive) {

    // Round 'value' variable for proof of concept - NOTE won't need to do this with a large 2D array
    int coordinate = int(value) * scale;

    std::cout << "\nAXIS:" << axis << "\nVALUE:" << coordinate << "\nPOSITIVE:" << positive;

    // set value to X if there is a print
    if (axis == 'X') {
        //if it's negative go left the grid not right
        if (!positive) {
            for (int i = 0; i < coordinate; i++) {
                latticeVisualisation[previousYcordinate][previousXcordinate - i] = true;
            }
            // adds thickness to print
            for (int i = 0; i < coordinate; i++) {
                for (int j = 0; j <= thickness; j++) {
                    latticeVisualisation[previousYcordinate - j][previousXcordinate - i] = true;
                }
            }
            previousXcordinate -= coordinate;
        }
        else {
            for (int i = 0; i < coordinate; i++) {
                latticeVisualisation[previousYcordinate][previousXcordinate + i] = true;
            }
            // adds thickness to print
            for (int i = 0; i < coordinate; i++) {
                for (int j = 0; j <= thickness; j++) {
                    latticeVisualisation[previousYcordinate - j][previousXcordinate + i] = true;
                }
            }
            previousXcordinate += coordinate;
        }
    }
    if (axis == 'Y') {
        //if it's negative go up the grid not down
        if (!positive) {
            for (int i = 0; i < coordinate; i++) {
                latticeVisualisation[previousYcordinate + i][previousXcordinate] = true;
            }
            // adds thickness to print
            for (int i = 0; i < coordinate; i++) {
                for (int j = 0; j <= thickness; j++) {
                    latticeVisualisation[previousYcordinate + i][previousXcordinate - j] = true;
                }
            }
            previousYcordinate += coordinate;
        }
        else {
            for (int i = 0; i < coordinate; i++) {
                latticeVisualisation[previousYcordinate - i][previousXcordinate] = true;
                /*for (int k = 0; k < thickness; k++) {
                    latticeVisualisation[previousYcordinate - i - k][previousXcordinate] = 'X';
                }*/
            }
            // adds thickness to print
            for (int i = 0; i < coordinate; i++) {
                for (int j = 0; j < thickness; j++) {
                    latticeVisualisation[previousYcordinate - i][previousXcordinate - j] = true;
                    /*for (int k = 0; k < thickness; k++) {
                        latticeVisualisation[previousYcordinate - i - k][previousXcordinate] = 'X';
                    }*/
                }
            }
            previousYcordinate -= coordinate;
        }
    }
}

// ---------- Display ---------- \\

// testing with open cv image output
void gCodeInterpreter::display() {
    Mat barr(480, 640, CV_8UC1, Scalar(0));
    for (int x = 0; x < 480; x++) {
        for (int y = 0; y < 640; y++) {
            if (latticeVisualisation[x][y] == true) {
                barr.at<uchar>(x, y) = 255;
            }
        }
    }

    // can get rid off if we use the image file instead
    imshow("barr", barr);
    waitKey(0);

    //change name and location to save image
    cv::imwrite("C:/Users/Luke/Desktop/Test.jpg", barr);
}

// ---------- Calibration ---------- \\

void gCodeInterpreter::setScale(float top_ppm, float bot_ppm) {
    scale = (int)((top_ppm + bot_ppm) / 2);
}

void gCodeInterpreter::setThickness() {
    thickness = (int)(scale/4);
}

// moves to center X coodinate (10 is largest Gcode movement, lattice X dimension)
void gCodeInterpreter::setStartingX() {
    previousXcordinate = 320-(scale*10);
}

// moves to center Y coodinate (10 is largest Gcode movement, lattice Y dimension)
void gCodeInterpreter::setStartingY() {
    previousYcordinate = (int)(240+((scale*10)/2));
}
