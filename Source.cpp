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

    md.setImg("C:\\Users\\Luke\\Downloads\\Project Resources\\perfectPrint640Purge.jpg");
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
                // Identifys a column
                if (pa.drawColumns[j - 1].length > xmean && pa.drawColumns[j].length < xmean) {
                    colNum++;
                    for (int i = 0; i < amountOfXLines; i++) {
                        columnsLines[columnsLines.size() - 1  - i].numOfLinesInCol = amountOfXLines;
                    }
                    amountOfXLines = 0;
                }
                // Line in a column
                else if (pa.drawColumns[j].length > xmean) {
                    XLine line;
                    line.columnNum = colNum;
                    line.x1 = pa.drawColumns[j].x1;
                    line.y1 = pa.drawColumns[j].y1;
                    line.x2 = pa.drawColumns[j].x2;
                    line.y2 = pa.drawColumns[j].y2;
                    line.length = pa.drawColumns[j].length;

                    columnsLines.push_back(line);
                }
            }
        }
    }

    for (int i = 0; i < amountOfXLines; i++) {
        columnsLines[columnsLines.size() - 1 - i].numOfLinesInCol = amountOfXLines;
    }
    amountOfXLines = 0;

    pa.xAlgorithm(columnsLines);

    // ---------- Y region ---------- \\

    double ysum = 0, ymean = 0;
    int amountOfYLines = 0, rowNum = 1;
    i = 0;
    std::vector<YLine> rowsLines;

    pa.calcLongestY();
    md.drawLine(pa.p1, pa.p2, 255, 0, 0);

    pa.calcYThickness();

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
        if (j == 0 && pa.drawRows[j].length > ymean) {
            // nothing

            YLine line;
            line.rowNum = rowNum;
            line.x1 = pa.drawRows[j].x1;
            line.y1 = pa.drawRows[j].y1;
            line.x2 = pa.drawRows[j].x2;
            line.y2 = pa.drawRows[j].y2;
            line.length = pa.drawRows[j].length;

            rowsLines.push_back(line);
        }
        else
        {
            if (amountOfYLines > 0) {
                // Identifys a row

                std::cout << "Amount of Y Lines " << amountOfYLines << std::endl;

                if (pa.drawRows[j - 1].length > ymean && pa.drawRows[j].length < ymean) {
                    rowNum++;

                    std::cout << "Row Num " << rowNum << std::endl;

                    // runs 11 times then fails
                    for (int i = 0; i < amountOfYLines; i++) {
                        rowsLines[rowsLines.size() - 1 - i].numOfLinesInRow = amountOfYLines;
                        std::cout << "Number of Lines in Row" << rowsLines[rowsLines.size() - 1 - i].numOfLinesInRow << std::endl;
                    }
                    amountOfYLines = 0;
                }
                // Line in a row
                else if (pa.drawRows[j].length > ymean) {
                    YLine line;
                    line.rowNum = rowNum;
                    line.x1 = pa.drawRows[j].x1;
                    line.y1 = pa.drawRows[j].y1;
                    line.x2 = pa.drawRows[j].x2;
                    line.y2 = pa.drawRows[j].y2;
                    line.length = pa.drawRows[j].length;

                    rowsLines.push_back(line);
                }
            }
        }
    }

    for (int i = 0; i < amountOfYLines; i++) {
        rowsLines[rowsLines.size() - 1 - i].numOfLinesInRow = amountOfYLines;
    }
    amountOfYLines = 0;
 
    int lastRow = rowNum - 1;

    for (int i = 0; i < rowsLines.size(); i++) {
        cv::Point p1;
        cv::Point p2;

        if (rowsLines[i].rowNum != lastRow) {
            p1.x = rowsLines[i].x1;
            p1.y = rowsLines[i].y1;
            p2.x = rowsLines[i].x2;
            p2.y = rowsLines[i].y2;

            md.drawLine(p1, p2, 255, 0, 0);
        }
        // If the itterator is up to the last row, then stop the line at the x value
        // of the first column
        else if (rowsLines[i].x1 < columnsLines[0].x1 && rowsLines[i].rowNum == lastRow) {
            
            for (int j = 0; j < rowsLines[i].numOfLinesInRow ; j++) {
                p1.x = columnsLines[0].x1;
                p1.y = rowsLines[i].y1;
                p2.x = rowsLines[i].x2;
                p2.y = rowsLines[i].y2;

                md.drawLine(p1, p2, 0, 0, 255);

                // Overwrite the line in the rowsline to not extend past the first column
                // test if this is right
                rowsLines[i].x1 = columnsLines[0].x1;
            }
        }
    }
    std::cout << "\n\n\n" << std::endl;

    pa.yAlgorithm(rowsLines);

    //for (int i = 0; i < rowsLines.size(); i++) {
    //    rowsLines[i].printYLine();
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