#include <iostream>
#include "Vision.h"
#include "gCodeInterpreter.h"
#include "printAnalysis.h"

using namespace std;

string imgloc;
float top_ppm, bot_ppm;

void materialDetection(string s) {
    MaterialDetection md;
    md.setImg(s);
    md.run();

    md.drawLine(cv::Point(50, 50), cv::Point(100, 100));
    md.displayCentered();

    md.displayBoolArr();
}

void gCodeInterpretation(string s) {
    gCodeInterpreter gc;
    gc.loadFile(s);

    gc.setScale(25, 25);
    gc.setThickness();
    gc.setStartingX();
    gc.setStartingY();

    gc.interpret();
    gc.display();
}

void camera() {
    Camera c;
    c.preview();
}

void calibration(string s) {
    Calibration c;
    c.setImg(s);
    c.calcMeasurementRelation();
    top_ppm = c.getTop_ppmm();
    bot_ppm = c.getBot_ppmm();
}

void cmd_change_img() {
    string s;
    cout << "input local location: ";
    cin >> s;

    imgloc = "C:\\Users\\Suker\\Pictures\\microshots\\tricep\\" + s;
}

void printAnalysisAlgorithm()
{
    printAnalysis pa;

    MaterialDetection md;
    md.setImg("C:\\Users\\Luke\\Downloads\\Project Resources\\u4.jpg");
    md.run();

    // create char array
    for (int i = 0; i < 480; i++) {
        for (int j = 0; j < 640; j++) {
            if (md.getBool(j, i)) {
                pa.arr[i][j] = 'f';
            }
            else {
                pa.arr[i][j] = 't';
            }
        }
    }

    pa.calcLongestX();
    md.drawLine(pa.p1, pa.p2);

    pa.calcXThickness();
    for (int i = 0; i < pa.thicknessXPoints.size(); i++) {
        md.drawLine(pa.thicknessXPoints[i], pa.thicknessXPoints[i + 1]);
        cout << "Top: " <<  pa.thicknessXPoints[i] << "\tBot: " << pa.thicknessXPoints[i + 1] << endl;
        i++;
    }

    // mean of all lines
    double sum = 0;
    int i;
    for (i = 0; i < pa.lines.size(); i++)
    {
        sum += pa.lines[i].length;
    }
    double mean = sum / i;

    std::vector<Line> columnsLines;

    int amountOfLines = 0;
    int colNum = 1;

    // Populating the information about a column within in the columnsLines vector
    for (int j = 0; j < pa.lines.size(); j++)
    {
        if (pa.lines[j].length > mean) {
            amountOfLines++;
        }
        if (amountOfLines > 0) {
            if (pa.lines[j - 1].length > mean && pa.lines[j].length < mean) {
                Line line;
                line.numOfLinesInCol = amountOfLines;
                line.columnNum = colNum;
                
                int meanSum = 0;
                // Calculates the mean of lines within a certain column
                for (int i = 0; i < amountOfLines; i++) {
                    line.length = pa.lines[j - i].length;
                    meanSum += pa.lines[j - i].length;
                }
                int meanLengthOfCol = meanSum / amountOfLines;
                line.meanLength = meanLengthOfCol;
                columnsLines.push_back(line);

                amountOfLines = 0;
                colNum++;
            }
        }
    }

    // Mean of all line lengths in columns
    sum = 0;
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
    if (columnsLines.size() < GUINumLines)
        std::cout << "Print failed - Line breakage detected" << std::endl;

    // ---------- Detect Thickness Variation ---------- \\

    double meanOfColumnLines = pa.calcMeanOfColumnLines(columnsLines);
    double stdDeviationOfColumnLines = pa.calcStandardDeviation(columnsLines, meanOfColumnLines);

    // if the thickness of a line is greater than one standard deviation from the average (16%) - bad print
    for (int i = 0; i < columnsLines.size(); i++)
    {
        if (columnsLines[i].numOfLinesInCol > meanOfColumnLines + stdDeviationOfColumnLines || columnsLines[i].numOfLinesInCol < meanOfColumnLines - stdDeviationOfColumnLines) {
            std::cout << "false: the print thickness varies more than 1 standard deviation." << std::endl;
        }
    }

    // ---------- DO THE SAME IN THE Y AXIS ---------- \\

    // remember that the purge will mess with the means and standard deviation

    //pa.calcLongestY();
    //md.drawLine(pa.p1, pa.p2);

    //cout << "\n\ncalculating Y: " << endl << endl;

    //pa.calcYThickness();
    //for (int i = 0; i < pa.thicknessYPoints.size(); i++) {
    //    md.drawLine(pa.thicknessYPoints[i], pa.thicknessYPoints[i + 1]);
    //    //cout << "X: " << pa.thicknessXPoints[i] << "\tY: " << pa.thicknessXPoints[i + 1] << endl;
    //    i++;
    //}

    md.displayCentered();
    cv::waitKey(0);
}

void FullProcess() {

    // ----- Camera Calibration ----- \\
    
    Camera cam;
    cam.preview();
    Calibration c;
    c.setImg("C:\\Users\\Luke\\Downloads\\Project Resources\\tle.png");
    c.calcMeasurementRelation();
    top_ppm = c.getTop_ppmm();
    bot_ppm = c.getBot_ppmm();

    // ----- Interpret GCode ----- \\

    gCodeInterpreter gc;
    // Load GCode file
    gc.loadFile("C:/Users/Luke/Downloads/Project Resources/singleLayerLattice.txt");
    // Set vairables for scaling
    gc.setScale(top_ppm, bot_ppm);
    gc.setThickness();
    gc.setStartingX();
    gc.setStartingY();
    // Transform to useful data
    gc.interpret();
    gc.display();

    // ----- Detect Material ----- \\

    MaterialDetection md;
    // takes image
    // md.setImg(cam.capture());
    md.setImg("C:\\Users\\Luke\\Downloads\\Project Resources\\u5.jpg");
    md.run();
    md.displayBoolArr();

    // ----- Run Comparitive Algorithm  ----- \\
    // working on conceptually in drive doc
}

int main(int argc, char* argv[])
{
    printAnalysisAlgorithm();

    while (true) {

        int i;
        cout << "OPTIONS:\n0.Exit\n1. Access Camera\n2. Run Calibration\n3. Interpret GCode File\n4. Detect Material\n5. Change Source img\n6. Run Full Process\n";
        cout << "Enter Command: ";
        cin >> i;

        switch (i) {
        case 0:
            return 0;
        case 1:
            camera();
            break;
        case 2:
            calibration("C:\\Users\\Luke\\Downloads\\Project Resources\\tle.png");
            break;
        case 3:
            gCodeInterpretation("C:/Users/Luke/Downloads/Project Resources/singleLayerLattice.txt");
            break;
        case 4:
            materialDetection("C:\\Users\\Luke\\Downloads\\Project Resources\\u4.jpg");
            break;
        case 5:
            cmd_change_img();
            break;
        case 6:
            FullProcess();
            break;
        }

        cv::waitKey(0);
    }

    return 0;
}
