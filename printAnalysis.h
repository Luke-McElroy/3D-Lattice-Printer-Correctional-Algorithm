#pragma once
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

struct Line
{
	int x1, y1, x2, y2;
	int columnNum, numOfLinesInCol;
	double length, meanLength, midpoint;

	void printLine() {
		std::cout << "Column Number: " << columnNum << std::endl;
		std::cout << "Num of Lines in Column: " << numOfLinesInCol << std::endl;
		std::cout << "Mean Length of Column: " << meanLength << std::endl;
	}
};

class printAnalysis {
private:

	int currentLongestStartX = 0;
	int currentLongestFinishX = 0;
	int currentLongestStartY, currentLongestFinishY = 0;
	int newLongestStartX, newLongestStartY, newLongestFinishX, newLongestFinishY = 0;
	double midpoint = 0;
	int midpointX, midpointY = 0;
	int topYpoint, botYpoint = 0;
	int leftXpoint, rightXpoint = 0;

public:

	char arr[480][640];

	cv::Point p1;
	cv::Point p2;

	void calcLongestX();
	void calcLongestY();
	double calcStandardDeviation(std::vector<Line> &vect, double mean);
	double calcMeanOfColumnLines(std::vector<Line>& vect);

	std::vector <cv::Point> thicknessXPoints;
	std::vector <cv::Point> thicknessYPoints;
	void calcXThickness();
	void calcYThickness();

	std::vector <Line> lines;

};