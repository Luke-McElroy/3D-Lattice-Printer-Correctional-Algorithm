#pragma once
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/videoio.hpp>
#include <iostream>

class MaterialDetection
{
private:
	//vars
	cv::Mat src,bw,dsrc;
	
	//funcs
	cv::Mat& csr(cv::Mat& i);
	void processImg();
	void createBoolArray();
	bool arr[640][480];
	
public:
	void drawLine(cv::Point p1, cv::Point p2);
	void displayCentered();
	void setImg(std::string);
	void setImg(cv::Mat);
	void run();
	void displayBoolArr();
	bool getBool(int x, int y);
};

class Calibration
{
private:
	cv::Mat src;
	int topAvg, botAvg, topCount, botCount;
	int baseX, baseY;
	bool img[640][480];
	int avgThickness;
	bool checkNoise(int x, int y);
	double euclidDist(const cv::Vec4i& line);
	//Initialization moved to .cpp
	static const float REAL_LENGTH;
	static float top_ppmm, bot_ppmm;

public:

	Calibration();
	void getImg();
	void setImg(std::string s);
	void calcThick();
	void calcMeasurementRelation();
	void calcSizeRelation();
	static float getTop_ppmm();
	static float getBot_ppmm();
	static float getREAL_LENGTH();
};

class Camera
{
private:
	cv::Mat frame;
	cv::VideoCapture cap;
public:
	void preview();
	cv::Mat capture();
	cv::Mat getLast();
};
