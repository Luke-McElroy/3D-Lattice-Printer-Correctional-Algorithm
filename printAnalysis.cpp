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

        drawColumns.push_back(line);
        
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
        line.x1 = midpointX + right;
        line.y1 = topYpoint;
        line.x2 = midpointX + right;
        line.y2 = botYpoint;

        //calc  line stuff
        line.length = abs(botYpoint - topYpoint);
        line.midpoint = (botYpoint + topYpoint) / 2;

        drawColumns.push_back(line);

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

        Line line;
        line.x1 = leftXpoint;
        line.y1 = midpointY - up;
        line.x2 = rightXpoint;
        line.y2 = midpointY - up;

        //calc  line stuff
        line.length = abs(leftXpoint - rightXpoint);
        line.midpoint = (leftXpoint + rightXpoint) / 2;

        drawRows.push_back(line);

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

        Line line;
        line.x1 = leftXpoint;
        line.y1 = midpointY + down;
        line.x2 = rightXpoint;
        line.y2 = midpointY + down;

        //calc  line stuff
        line.length = abs(leftXpoint - rightXpoint);
        line.midpoint = (leftXpoint + rightXpoint) / 2;

        drawRows.push_back(line);

        down = down + res;
    }
}


double printAnalysis::calcMeanOfColumnLines(std::vector<XLine>& vect) {
    // calculates mean
    int sum = 0;
    sum = 0;
    for (int i = 0; i < vect.size(); i++) {
        sum += vect[i].numOfLinesInCol;
    }
    double meanNumLinesInCol = sum / vect.size();

    return meanNumLinesInCol;
}

double printAnalysis::calcStandardDeviation(std::vector<XLine> &vect, double mean) {
    // get standard deviation from mean
    int sum = 0;
    for (int i = 0; i < vect.size(); ++i) {
        sum += pow((vect[i].numOfLinesInCol - mean), 2);
    }

    int size = vect.size() - 1;
    if (vect.size() - 1 < 1)
        size = 1;

    double standardDeviation = sqrt(sum / size);

    return standardDeviation;
}

double printAnalysis::calcMeanOfRowLines(std::vector<YLine>& vect) {
    // calculates mean
    int sum = 0;
    for (int i = 0; i < vect.size(); i++) {
        sum += vect[i].numOfLinesInRow;
    }
    double meanNumLinesInRow = sum / vect.size();

    return meanNumLinesInRow;
}

double printAnalysis::calcStandardDeviation(std::vector<YLine>& vect, double mean) {
    // get standard deviation from mean
    int sum = 0;
    for (int i = 0; i < vect.size(); ++i) {
        sum += pow((vect[i].numOfLinesInRow - mean), 2);
    }

    int size = vect.size() - 1;
    if (vect.size() - 1 < 1)
        size = 1;

    double standardDeviation = sqrt(sum / size);

    return standardDeviation;
}

void printAnalysis::xAlgorithm(std::vector<XLine> &columnsLines) {
    // Mean of all line lengths in columns
    int sum = 0;
    for (int i = 0; i < columnsLines.size(); i++) {
        sum += columnsLines[i].meanLength;
    }
    double meanOfAllCols = sum / columnsLines.size();
    std::cout << "Mean of all Cols: " << meanOfAllCols << std::endl;


    // ---------- Detect material Breakage ---------- \\

    // Compares the mean length of each individual column with the mean of
    // the overall mean of the columns to detect a breakage
    int counter = 0;
    for (int i = 0; i < columnsLines.size(); i++) {
        for (int j = 0; j < columnsLines[i].length; j++) {
            if (columnsLines[i].length < meanOfAllCols) {
                counter++;
            }
        }
        if (counter == columnsLines[i].numOfLinesInCol) {
            std::cout << "Print failed - Line breakage detected" << std::endl;
        }
        counter = 0;
    }

    // TO DO - map to gui value
    int GUINumLines = 5;
    // if there are less columns than GUINumLines then there is a break
    if (columnsLines.size() < GUINumLines) {
        std::cout << "Print failed - Line breakage detected" << std::endl;
    }

    // ---------- Detect Thickness Variation ---------- \\

    double meanOfColumnLines = calcMeanOfColumnLines(columnsLines);
    double stdDeviationOfColumnLines = calcStandardDeviation(columnsLines, meanOfColumnLines);

    // if the thickness of a line is greater than one standard deviation from the average (16%) - bad print
    for (int i = 0; i < columnsLines.size(); i++)
    {
        if (columnsLines[i].numOfLinesInCol > meanOfColumnLines + stdDeviationOfColumnLines || columnsLines[i].numOfLinesInCol < meanOfColumnLines - stdDeviationOfColumnLines) {
            std::cout << "false: the print thickness varies more than 1 standard deviation." << std::endl;
        }
    }
}

void printAnalysis::yAlgorithm(std::vector<YLine>& rowsLines) {
    // Mean of all line lengths in columns
    int sum = 0;
    for (int i = 0; i < rowsLines.size(); i++) {
        sum += rowsLines[i].meanLength;
    }
    double meanOfAllRows = sum / rowsLines.size();
    std::cout << "Mean of all Rows: " << meanOfAllRows << std::endl;


    // ---------- Detect material Breakage ---------- \\

    // Compares the mean length of each individual column with the mean of
    // the overall mean of the columns to detect a breakage
    int counter = 0;
    for (int i = 0; i < rowsLines.size(); i++) {
        for (int j = 0; j < rowsLines[i].length; j++) {
            if (rowsLines[i].length < meanOfAllRows) {
                counter++;
            }
        }
        if (counter == rowsLines[i].numOfLinesInRow) {
            std::cout << "Print failed - Line breakage detected" << std::endl;
        }
        counter = 0;
    }

    // TO DO - map to gui value
    int GUINumLines = 5;
    // if there are less columns than GUINumLines then there is a break
    if (rowsLines.size() < GUINumLines) {
        std::cout << "Print failed - Line breakage detected" << std::endl;
    }

    // ---------- Detect Thickness Variation ---------- \\

    double meanOfRowLines = calcMeanOfRowLines(rowsLines);
    double stdDeviationOfRowLines = calcStandardDeviation(rowsLines, meanOfRowLines);

    // if the thickness of a line is greater than one standard deviation from the average (16%) - bad print
    for (int i = 0; i < rowsLines.size(); i++)
    {
        if (rowsLines[i].numOfLinesInRow > meanOfRowLines + stdDeviationOfRowLines || rowsLines[i].numOfLinesInRow < meanOfRowLines - stdDeviationOfRowLines) {
            std::cout << "false: the print thickness varies more than 1 standard deviation." << std::endl;
        }
    }
}

// -------- Templating for when we are bored -------
//template <class T>
//T calcMeanOfLines(std::vector<T>& vect) {
//    // calculates mean
//    int sum = 0; 
//    for (int i = 0; i < vect.size(); i++) {
//        sum += vect[i].numoflinesincol;
//    }
//    double meannumlinesincol = sum / vect.size();
//
//    return meannumlinesincol;
//}
//
//template <class T>
//T getStandardDeviation(std::vector<T>& vect, double mean) {
//    int sum = 0;
//    for (int i = 0; i < vect.size(); ++i) {
//        sum += pow((vect[i].numOfLinesInCol - mean), 2);
//    }
//
//    int size = vect.size() - 1;
//    if (vect.size() - 1 < 1)
//        size = 1;
//
//    double standarddeviation = sqrt(sum / size);
//
//    return standarddeviation;
//}

