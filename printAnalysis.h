#pragma once
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

class Line {
	public:
		int x1, y1, x2, y2;
		double length, meanLength, midpoint;
	
	public:
		void printLine() {
			std::cout << "x1: " << x1 << " " << "y1: " << y1 << std::endl;
			std::cout << "x2: " << x2 << " " << "y2: " << y2 << std::endl;
			std::cout << "Length: " << length << std::endl;
			std::cout << "MeanLength: " << meanLength << std::endl;
			std::cout << "Midpoint: " << midpoint << std::endl;
		}
};

class XLine : public Line {
	public:
		int columnNum, numOfLinesInCol;

	void printXLine() {
		Line::printLine();
		std::cout << "Column Number: " << columnNum << std::endl;
		std::cout << "Num of Lines in Column: " << numOfLinesInCol << std::endl;
		std::cout << "Mean Length of Column: " << meanLength << std::endl;
	}
};

class YLine : public Line {
	public:
		int rowNum, numOfLinesInRow;

	void printYLine() {
		Line::printLine();
		std::cout << "Row Number: " << rowNum << std::endl;
		std::cout << "Num of Lines in Row: " << numOfLinesInRow << std::endl;
		std::cout << "Mean Length of Row: " << meanLength << std::endl;
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

	double calcMeanOfColumnLines(std::vector<XLine> &vect);
	double calcStandardDeviation(std::vector<XLine> &vect, double mean);
	
	double calcMeanOfRowLines(std::vector<YLine> &vect);
	double calcStandardDeviation(std::vector<YLine> &vect, double mean);

	std::vector <cv::Point> thicknessXPoints;
	std::vector <cv::Point> thicknessYPoints;

	void calcXThickness();
	void calcYThickness();

	void xAlgorithm(std::vector<XLine>& columnsLines);
	void yAlgorithm(std::vector<YLine>& rowsLines);

	std::vector <Line> drawColumns;
	std::vector <Line> drawRows;
};