#include "printAnalysis.h"
#include <cmath>
#include <iostream>
#include <vector>

int res = 5;

void printAnalysis::calcLongestX() {

    // ----- Find longest X length  ------ \\

    for (int i = 0; i < 640; i++) {
        // resets everthing to 0 to find new line length
        newLongestStartX = 0;
        newLongestStartY = 0;
        newLongestFinishX = 0;
        newLongestFinishY = 0;
        for (int j = 0; j < 480; j++) {
            //if the beginning of a line
            if (arr[i][j] == 't' && arr[i][j - 1] != 't') {
                newLongestStartX = j;
                newLongestStartY = i;
            }
            // if continued line
            else if (arr[i][j - 1] == 't' && arr[i][j] == 't') {
                newLongestFinishX = j;
                newLongestFinishY = i;
            }
            // if the line is bigger save the line
            if ((newLongestFinishX - newLongestStartX) > (currentLongestFinishX - currentLongestStartX)) {
                currentLongestStartX = newLongestStartX;
                currentLongestStartY = newLongestStartY;
                currentLongestFinishX = newLongestFinishX;
                currentLongestFinishY = newLongestFinishY;
            }
        }
    }

    p1.x = currentLongestStartX;
    p1.y = currentLongestStartY;
    p2.x = currentLongestFinishX;
    p2.y = currentLongestFinishY;
}

void printAnalysis::calcLongestY() {

    // ----- Find longest Y length  ------ \\

    for (int i = 0; i < 480; i++) {
        // resets everthing to 0 to find new line length
        newLongestStartX = 0;
        newLongestStartY = 0;
        newLongestFinishX = 0;
        newLongestFinishY = 0;
        for (int j = 0; j < 640; j++) {
            //if the beginning of a line
            if (arr[j][i] == 't' && arr[j - 1][i] != 't') {
                newLongestStartX = i;
                newLongestStartY = j;
            }
            // if continued line
            else if (arr[j - 1][i] == 't' && arr[j][i] == 't') {
                newLongestFinishX = i;
                newLongestFinishY = j;
            }
            // if the line is bigger save the line
            if ((newLongestFinishY - newLongestStartY) > (currentLongestFinishY - currentLongestStartY)) {
                currentLongestStartX = newLongestStartX;
                currentLongestStartY = newLongestStartY;
                currentLongestFinishX = newLongestFinishX;
                currentLongestFinishY = newLongestFinishY;
            }
        }
    }

    p1.x = currentLongestStartX;
    p1.y = currentLongestStartY;
    p2.x = currentLongestFinishX;
    p2.y = currentLongestFinishY;
}

void printAnalysis::calcXThickness() {
    double midpoint = (currentLongestStartX + currentLongestFinishX) / 2;
    midpointX = abs((int)midpoint);
    midpointY = currentLongestFinishY;

    int left = 0;
    int right = 0;

    // loop left until end of line every 10 pixels calc Y line
    while (arr[midpointY][midpointX - left] == 't') {
        
        topYpoint = midpointY;
        while (arr[topYpoint][midpointX - left] == 't') {
            topYpoint++;
        }
        botYpoint = midpointY;
        while (arr[botYpoint][midpointX - left] == 't') {
            botYpoint--;
        }

        p1.x = midpointX - left;
        p1.y = topYpoint;
        p2.x = midpointX - left;
        p2.y = botYpoint;

        thicknessXPoints.push_back(p1);
        thicknessXPoints.push_back(p2);

        Line line;
        line.x1 = midpointX - left;
        line.y1 = topYpoint;
        line.x2 = midpointX - left;
        line.y2 = botYpoint;

        //calc  line stuff
        line.length = abs(botYpoint - topYpoint);
        line.midpoint = (botYpoint + topYpoint) / 2;

        lines.push_back(line);
        
        left = left + res;
    }
    // loop right until end of line every 10 pixels calc Y line
    while (arr[midpointY][midpointX + right] == 't') {

        topYpoint = midpointY;
        while (arr[topYpoint][midpointX + right] == 't') {
            topYpoint++;
        }
        botYpoint = midpointY;
        while (arr[botYpoint][midpointX + right] == 't') {
            botYpoint--;
        }

        p1.x = midpointX + right;
        p1.y = topYpoint;
        p2.x = midpointX + right;
        p2.y = botYpoint;

        thicknessXPoints.push_back(p1);
        thicknessXPoints.push_back(p2);

        Line line;
        line.x1 = midpointX - left;
        line.y1 = topYpoint;
        line.x2 = midpointX - left;
        line.y2 = botYpoint;

        //calc  line stuff
        line.length = abs(botYpoint - topYpoint);
        line.midpoint = (botYpoint + topYpoint) / 2;

        lines.push_back(line);

        right = right + res;
    }
}

void printAnalysis::calcYThickness() {
    double midpoint = (currentLongestStartY + currentLongestFinishY) / 2;
    midpointY = abs((int)midpoint);
    midpointX = currentLongestFinishX;

    int up = 0;
    int down = 0;

    // up
    while (arr[midpointY - up][midpointX] == 't')
    {
        leftXpoint = midpointX;
        while (arr[midpointY - up][leftXpoint] == 't') {
            leftXpoint--;
        }
        rightXpoint = midpointX;
        while (arr[midpointY - up][rightXpoint] == 't') {
            rightXpoint++;
        }

        p1.x = leftXpoint;
        p1.y = midpointY - up;
        p2.x = rightXpoint;
        p2.y = midpointY - up;

        thicknessYPoints.push_back(p1);
        thicknessYPoints.push_back(p2);

        up = up + res;
    }

    // down
    while (arr[midpointY + down][midpointX] == 't')
    {
        leftXpoint = midpointX;
        while (arr[midpointY + down][leftXpoint] == 't') {
            leftXpoint--;
        }
        rightXpoint = midpointX;
        while (arr[midpointY + down][rightXpoint] == 't') {
            rightXpoint++;
        }

        p1.x = leftXpoint;
        p1.y = midpointY + down;
        p2.x = rightXpoint;
        p2.y = midpointY + down;

        thicknessYPoints.push_back(p1);
        thicknessYPoints.push_back(p2);

        down = down + res;
    }
}

double printAnalysis::calcMeanOfColumnLines(std::vector<Line>& vect) {
    // calculates mean
    int sum = 0;
    sum = 0;
    for (int i = 0; i < vect.size(); i++) {
        sum += vect[i].numOfLinesInCol;
    }
    double meanNumLinesInCol = sum / vect.size();

    return meanNumLinesInCol;
}

double printAnalysis::calcStandardDeviation(std::vector<Line> &vect, double mean) {
    // get standard deviation from mean
    int sum = 0;
    for (int i = 0; i < vect.size(); ++i) {
        sum += pow((vect[i].numOfLinesInCol - mean), 2);
    }
    double standarddeviation = sqrt(sum / (vect.size() - 1));

    return standarddeviation;
}
