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

void printAnalysisAlgorithm() {
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
    md.drawLine(pa.p1, pa.p2, 0, 255, 0);

    pa.calcXThickness();
    for (int i = 0; i < pa.thicknessXPoints.size(); i++) {
        md.drawLine(pa.thicknessXPoints[i], pa.thicknessXPoints[i + 1], 0, 255, 0);
        i++;
    }

    // ---------- X region ---------- \\

    double xsum = 0, xmean = 0;
    int i = 0, amountOfXLines = 0, colNum = 1;
    std::vector<XLine> columnsLines;

    // mean of all lines
    for (i = 0; i < pa.drawColumns.size(); i++)
    {
        xsum += pa.drawColumns[i].length;
    }
    xmean = xsum / i;

    // Populating the information about a column within in the columnsLines vector
    for (int j = 0; j < pa.drawColumns.size(); j++)
    {
        if (pa.drawColumns[j].length > xmean) {
            amountOfXLines++;
        }
        if (j == 0) {
            // nothing
        }
        else
        {
            if (amountOfXLines > 0) {
                if (pa.drawColumns[j - 1].length > xmean && pa.drawColumns[j].length < xmean) {
                    XLine line;
                    line.numOfLinesInCol = amountOfXLines;
                    line.columnNum = colNum;
                    line.x1 = pa.drawColumns[j].x1;
                    line.y1 = pa.drawColumns[j].y1;
                    line.x2 = pa.drawColumns[j].x2;
                    line.y2 = pa.drawColumns[j].y2;

                    std::cout << "X value: " << line.x1 << std::endl;

                    int meanSum = 0;
                    // Calculates the mean of lines within a certain column
                    for (int i = 0; i < amountOfXLines; i++) {
                        line.length = pa.drawColumns[j - i].length;
                        meanSum += pa.drawColumns[j - i].length;
                    }
                    int meanLengthOfCol = meanSum / amountOfXLines;
                    line.meanLength = meanLengthOfCol;
                    columnsLines.push_back(line);

                    amountOfXLines = 0;
                    colNum++;
                }
            }
        }
    }

    pa.xAlgorithm(columnsLines);

    // ---------- Y region ---------- \\

    double ysum = 0, ymean = 0;
    int amountOfYLines = 0, rowNum = 1;
    i = 0;
    std::vector<YLine> rowsLines;

    pa.calcLongestY();
    md.drawLine(pa.p1, pa.p2, 255, 0, 0);

    pa.calcYThickness();
    for (int i = 0; i < pa.thicknessYPoints.size(); i++) {
        // Account for the purge by only drawing the row lines up to the 
        // first column which is the end of the purge
        if (pa.thicknessYPoints[i].x < columnsLines[0].x1) {
            pa.thicknessYPoints[i].x = columnsLines[0].x1;
            md.drawLine(pa.thicknessYPoints[i], pa.thicknessYPoints[i + 1], 0, 0, 255);
        }
        else {
            md.drawLine(pa.thicknessYPoints[i], pa.thicknessYPoints[i + 1], 0, 0, 255);
        }
        i++;
    }

    // mean of all lines
    for (i = 0; i < pa.drawRows.size(); i++)
    {
        ysum += pa.drawRows[i].length;
    }
    ymean = ysum / i;

    // Populating the information about a row within the rowsLines vector
    for (int j = 0; j < pa.drawRows.size(); j++)
    {
        if (pa.drawRows[j].length > ymean) {
            amountOfYLines++;
        }
        if (j == 0) {
            // nothing
        }
        else {
           
            if (amountOfYLines > 0) {
                
                if (pa.drawRows[j - 1].length > ymean && pa.drawRows[j].length < ymean) {
                    YLine line;
                    line.numOfLinesInRow = amountOfYLines;
                    line.rowNum = rowNum;
                    line.x1 = pa.drawRows[j].x1;
                    line.y1 = pa.drawRows[j].y1;
                    line.x2 = pa.drawRows[j].x2;
                    line.y2 = pa.drawRows[j].y2;

                    std::cout << "Y value: " << line.y1 << std::endl;

                    int meanSum = 0;
                    // Calculates the mean of lines within a certain column
                    for (int i = 0; i < amountOfYLines; i++) {
                        line.length = pa.drawRows[j - i].length;
                        meanSum += pa.drawRows[j - i].length;
                    }
                    int meanLengthOfRow = meanSum / amountOfYLines;
                    line.meanLength = meanLengthOfRow;

                    //std::cout << "line length: " << line.length << std::endl;

                    rowsLines.push_back(line);
                    amountOfYLines = 0;
                    rowNum++;
                }
            }
        }
    }
    
    //for (int i = 0; i < rowsLines.size(); i++)
    //    rowsLines[0].printYLine();

    // if there is a purge run until there is no material or it reaches the first column line

    ////THIS IS JUST THE FOR LOOP ABOVE, USED TO MAKE THE LINES IN THE FIRST ROW NOT GO PAST THE PURGE - IS IS SKETCHY
    //for (int i = 0; i < pa.thicknessYPoints.size(); i++) {
    //    // Account for the purge by only drawing the row lines up to the 
    //    // first column which is the end of the purge
    //    if (pa.thicknessYPoints[i].x < columnsLines[0].x1) {
    //        pa.thicknessYPoints[i].x = columnsLines[0].x1;
    //        md.drawLine(pa.thicknessYPoints[i], pa.thicknessYPoints[i + 1], 0, 0, 255);
    //    }
    //    else {
    //        md.drawLine(pa.thicknessYPoints[i], pa.thicknessYPoints[i + 1], 0, 0, 255);
    //    }
    //    cout << "X: " << pa.thicknessYPoints[i] << "\tY: " << pa.thicknessYPoints[i + 1] << endl;
    //    i++;
    //}

    std::cout << "num of lines in row: " << rowsLines[0].numOfLinesInRow << std::endl;

    for (int i = 0; i < rowsLines[0].numOfLinesInRow; i++)
    {
        
        if (pa.thicknessYPoints[i].x < columnsLines[0].x1) {
            pa.thicknessYPoints[i].x = columnsLines[0].x1;
            md.drawLine(pa.thicknessYPoints[i], pa.thicknessYPoints[i + 1], 0, 0, 255);
        }
    }

    pa.yAlgorithm(rowsLines);

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