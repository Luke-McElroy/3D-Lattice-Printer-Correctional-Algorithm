#include "Vision.h"
#include "Config.h"
using namespace cv;

//<MATERIAL DETECTION>

void MaterialDetection::run() {
	processImg();
	createBoolArray();
}

void MaterialDetection::drawLine(cv::Point p1, cv::Point p2, int r, int g, int b)
{
	line(dsrc, p1, p2, Scalar(b, g, r), 1, 8, 0);
}

void MaterialDetection::displayCentered()
{
	imshow("centered lines", dsrc);
}

void MaterialDetection::setImg(std::string url) {
	src = imread(url, IMREAD_COLOR);
	
}

void MaterialDetection::setImg(Mat m) {
	src = m.clone();
}

void MaterialDetection::processImg() {
	int const thresholdVal = 70;
	int const maxVal = 255;
	Mat dst;
	std::vector<Mat> channels;
	split(src, channels);
	int limits[] = { 0,250,250 };
	for (int i = 0; i < 3; i++) {
		threshold(channels[i], channels[i], limits[i], 255, THRESH_BINARY);
	}
	merge(channels, dst);
	cvtColor(dst, bw, COLOR_BGR2GRAY);
	threshold(bw, bw, thresholdVal, maxVal, THRESH_BINARY);
	dsrc = bw.clone();
	cvtColor(dsrc, dsrc, COLOR_GRAY2BGR);
	imshow("img", dst);
	imshow("thresh", bw);
}

//Pretty sure this is a shit way to do this but it works for now
void MaterialDetection::createBoolArray() {
	uchar val;
	for (int x = 0; x < IMG_WIDTH; x++) {
		for (int y = 0; y < IMG_HEIGHT; y++) {
			val = bw.at<uchar>(y, x);
			if (val > 125) {
				arr[x][y] = true;
			}
			else {
				arr[x][y] = false;
			}

		}
	}
}

void MaterialDetection::displayBoolArr() {
	Mat barr(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, Scalar(0));
	for (int x = 0; x < IMG_WIDTH; x++) {
		for (int y = 0; y < IMG_HEIGHT; y++) {
			if (arr[x][y] == true) {
				barr.at<uchar>(y, x) = 255;
			}
		}
	}
	imshow("barr", barr);
	waitKey(0);
}

bool MaterialDetection::getBool(int x, int y) {
	return arr[x][y];
}


//</MATERIAL DETECTION>


//<CALIBRATION>

//ADJUST THIS TO CALIBRATIONS ACTUAL LENGTH
//USE MILLIMETERS
const float Calibration::REAL_LENGTH = 10;
float Calibration::top_ppmm = 0;
float Calibration::bot_ppmm = 0;

Calibration::Calibration()
{
	topAvg = 0;
	botAvg = 0;
	topCount = 0;
	botCount = 0;
}

double Calibration::euclidDist(const Vec4i& line) {
	return std::sqrt(static_cast<double>((line[0] - line[2]) * (line[0] - line[2]) + (line[1] - line[3]) * (line[1] - line[3])));
}

void Calibration::calcMeasurementRelation() {
	Mat dst,cdst;
	int lowThreshold = 50;
	const int ratio = 3;
	const int kernel_size = 3;
	const char* window_name = "edge map";

	Canny(src, dst, lowThreshold, lowThreshold * ratio, kernel_size);
	std::vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
	cvtColor(dst, cdst, COLOR_GRAY2BGR);
	
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
	}

	double topRelation = euclidDist(lines[0]) / REAL_LENGTH;
	double botRelation = euclidDist(lines[lines.size() - 1]) / REAL_LENGTH;

	std::cout << "top ppmm: " << topRelation << std::endl;
	std::cout << "bot ppmm: " << botRelation << std::endl;
	std::cout << "diff: " << abs(topRelation - botRelation) << std::endl;

	top_ppmm = topRelation;
	bot_ppmm = botRelation;
	imshow("detected", cdst);
	imshow("canny", dst);
}

void Calibration::setImg(String url) {
	src = imread(url, IMREAD_COLOR);
	imshow("src", src);
}

float Calibration::getTop_ppmm()
{
	return top_ppmm;
}

float Calibration::getBot_ppmm()
{
	return bot_ppmm;
}

float Calibration::getREAL_LENGTH() 
{
	return REAL_LENGTH;
}
//</CALIBRATION>


//<CAMERA>
void Camera::preview()
{
	//cap.open(0);
	int deviceID = 0;
	int apiID = cv::CAP_DSHOW;
	cap.open(deviceID, apiID);
	if (!cap.isOpened()) {
		std::cerr << ">>>>>ERROR: Opening camera" << std::endl;
	}
	else {
		std::cout << ">>>>> CAMERA OPENED SUCCESSFULLY" << std::endl;
	}
	std::cout << "begin display" << std::endl;
	waitKey(0);
	Mat f_Threshold, hsv;
	while (true)
	{
		cap.grab();
		cap.retrieve(frame, 0);

		if (frame.empty()) {
			std::cerr << ">>>>>ERROR! blank frame grabbed\n";
			break;
		}
		cvtColor(frame, hsv, COLOR_BGR2HSV);
		inRange(hsv, Scalar(110, 50, 50), Scalar(130, 255, 255), f_Threshold);

		imshow("HSV SHOW", f_Threshold);
		imshow("Live", frame);
		if (waitKey(30) >= 0) {
			destroyWindow("Live");
			destroyWindow("HSV SHOW");
			break;
		}
	}
}

//Captures an image to Camera::frame
Mat Camera::capture() {
	//cap.open(0);
	int deviceID = 0;
	int apiID = cv::CAP_DSHOW;
	cap.open(deviceID, apiID);
	if (!cap.isOpened()) {
		std::cerr << ">>>>>ERROR: Opening camera" << std::endl;
	}
	else {
		std::cout << ">>>>> CAMERA OPENED SUCCESSFULLY" << std::endl;
	}
	std::cout << "begin display" << std::endl;
	waitKey(0);
	cap.grab();
	cap.retrieve(frame, 0);

	if (frame.empty()) {
		throw ">>>>>ERROR! blank frame grabbed";
	}
	return frame;
}

Mat Camera::getLast() {
	if (frame.empty()) {
		throw ">>>>>ERROR! blank frame";
	}
	return frame;
}


//</CAMERA>
