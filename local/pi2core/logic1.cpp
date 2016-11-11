/*********************************************************************
 *  machineVisionMain.cpp
 *  Machine Vision Project - Hand Gesture control web browser
 *
 *  Created on: 2015 Feb
 *  Submitted by:  Yan Liu
 *                 Colten Normore
 *  Version:       1.0.0
 *  This project can be Shared Everywhere
 *********************************************************************/

#include "logic.h"
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;
#define PI 3.14159265
const string trackingHSVApplication = "Tracking HSV Value";
//initial min and max HSV filter values. These value will be changed using HSV track GUI
//This GUI will detect the HSGf value that using the "Color Ball Tracking" Application
int Hue_Min = 0;
int Hue_Max = 256;
int Sat_Min = 0;
int Sat_Max = 256;
int Val_Min = 0;
int Val_Max = 256;

void morphologicalImgProc(cv::Mat &frame);
string integerToString(int num);
int angleToCenter(const CvPoint &v1, const CvPoint &v2);
void doAction(int totalAngleOfFinger, int fingerSize);
void createHSVApp();
void trackHand(cv::Mat src, cv::Mat &dest);

logic::logic(controllerInterface *controller, visionInterface *vision) {
    this->c_module = controller;
    this->v_module = vision;
}

logic::~logic() {
    // TODO Auto-generated destructor stub
}

void logic::runLogic(){
    while (true) {
        Mat cameraFrame,  hsvFrame, thresholdFrame;
        v_module->getFrame(cameraFrame);
        //hst tool
        createHSVApp();
        //switch the RGB to HSV space, combined with background substraction
        cv::cvtColor(cameraFrame, hsvFrame, CV_BGR2HSV);
        
        cv::inRange(hsvFrame, Scalar(Hue_Min, Sat_Min, Val_Min), Scalar(Hue_Max, Sat_Max, Val_Max), thresholdFrame);
        cv::imshow("Camera Threshold Module", thresholdFrame);
        //testing in the blue glove on hand
        //need to adjust before the live demo
        //cv::inRange(hsvFrame, Scalar(58, 58, 95), Scalar(133, 154, 256),thresholdFrame);
        
        //blur image to remove basic imperfections
        medianBlur(thresholdFrame, thresholdFrame, 3);
        
        //do the morphological image processing
        //closing the frame
        morphologicalImgProc(thresholdFrame);
        
        //track the hand, put the bounding box around the hand
        //calculate the center point of the hand
        trackHand(thresholdFrame, cameraFrame);
        
        imshow("Hand_Gesture_Detection", cameraFrame);
        
        //release the memory
        cameraFrame.release();
        thresholdFrame.release();
        if (waitKey(10) >= 0)
            break;
    }
}


//------------------------------------------------------Helper Function
void createHSVApp() {
    namedWindow(trackingHSVApplication, 0);
    char trackingHSV[50];
    sprintf(trackingHSV, "Hue_Min", Hue_Min);
    sprintf(trackingHSV, "Hue_Max", Hue_Max);
    sprintf(trackingHSV, "Sat_Min", Sat_Min);
    sprintf(trackingHSV, "Sat_Max", Sat_Max);
    sprintf(trackingHSV, "Val_Min", Val_Min);
    sprintf(trackingHSV, "Val_Max", Val_Max);
    
    cv::createTrackbar("Hue_Min", trackingHSVApplication, &Hue_Min, Hue_Max);
    cv::createTrackbar("Hue_Max", trackingHSVApplication, &Hue_Max, Hue_Max);
    cv::createTrackbar("Sat_Min", trackingHSVApplication, &Sat_Min, Sat_Max);
    cv::createTrackbar("Sat_MAX", trackingHSVApplication, &Sat_Max, Sat_Max);
    cv::createTrackbar("Val_MIN", trackingHSVApplication, &Val_Min, Val_Max);
    cv::createTrackbar("Val_MAX", trackingHSVApplication, &Val_Max, Val_Max);
}

//--------------------------------------------------------------------


//calculate the angle between two points
int angleToCenter(const CvPoint &finger, const CvPoint &center) {
    float y_angle = center.y - finger.y; //center = 1;
    float x_angle = finger.x - center.x;// tip =2;
    float theta = atan(y_angle/ x_angle);
    int angleFinger = (int) round( theta * 180 / PI);
    return angleFinger;
}

//convert the integer to string
string integerToString(int num) {
    stringstream strings;
    strings << num;
    string s = strings.str();
    return s;
}

//morphological Image processing
//Erosion -> dilation -> closing the frame ensure to get the better performance
void morphologicalImgProc(cv::Mat &frame) {
    cv::Mat element = getStructuringElement(MORPH_ELLIPSE, CvSize(9, 9), Point(5, 5));
    cv::dilate(frame, frame, element);
    cv::erode(frame, frame, element);
    morphologyEx(frame, frame, cv::MORPH_OPEN, element);
    //morphologyEx(frame, frame, cv::MORPH_CLOSE, element);
}

//the important function to track the hand, the algorithm is described in the report
void trackHand(cv::Mat src, cv::Mat &dest) {
    //initialization local variables
    Rect boundRect;
    unsigned int largestObj = 0;
    int boundingBoxHeight = 0;
    vector<vector<Point> > contours; //store all the contours
    vector<Vec4i> hierarchy;
    vector<Point> convexHullPoint;
    vector<Point> fingerPoint;
    Point centerP;
    unsigned int numObjects = 0;
    double area = 0;
    double maxArea = 0;
    bool handFound = false;
    //find all the contours in the threshold Frame
    
    findContours(src, contours, hierarchy, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    numObjects = (unsigned int)hierarchy.size();
    for (unsigned int i = 0; i < contours.size(); i++) {
        Mat tempContour = Mat(contours[i]);
        area = contourArea(tempContour);
        if (area > maxArea) {
            maxArea = area;
            largestObj = i;
        }
    }
    if( maxArea > 4000){
        handFound = true;
        boundRect = boundingRect(contours[largestObj]);
        //draw the boundary of the object
        drawContours(dest, contours, largestObj, Scalar(0, 0, 255), 3, 8,hierarchy);
        //find the convex points for the largest object which is hand
        convexHull(contours[largestObj], convexHullPoint, true, true);
        //approxPolyDP( Mat(contours[largestObj]), contours[largestObj], 3, true );
        //use moment method to find the center point
        Moments moment = moments(Mat(contours[largestObj]), true);
        int centerX = moment.m10 / moment.m00;
        int centerY = moment.m01 / moment.m00;
        Point centerPoint(centerX, centerY);
        centerP = centerPoint;
        Point printPoint(centerX, centerY + 15);
        circle(dest, centerPoint, 8, Scalar(255, 0, 0), CV_FILLED);
        //put the BoundingBox in the contour region
        rectangle(dest, boundRect, Scalar(0, 0, 255), 2, 8, 0);
        boundingBoxHeight = boundRect.height;
        if (handFound) {
            int countHullPoint = (int)convexHullPoint.size();
            int maxdist = 0;
            int pos = 0;
            for (int j = 1; j < countHullPoint; j++) {
                pos = j;
                if (centerP.y >= convexHullPoint[j].y && centerP.y >= convexHullPoint[pos].y) {
                    pos = j;
                    int dist = (centerP.x - convexHullPoint[j].x)^ 2 + (centerP.y - convexHullPoint[j].y) ^ 2;
                    if (  abs(convexHullPoint[j-1].x - convexHullPoint[j].x) < 12){
                        if ( dist > maxdist){
                            maxdist = dist;
                            pos = j;
                        }
                    }
                    else if( j == 0 || abs(convexHullPoint[j-1].x - convexHullPoint[j].x) >= 12 ){
                        fingerPoint.push_back(convexHullPoint[pos]);
                        cv::line(dest,centerP, convexHullPoint[pos],Scalar(0, 255, 0), 3, CV_AA, 0);
                        circle(dest, convexHullPoint[pos], 8, Scalar(255, 0, 0), CV_FILLED);
                        pos = j;
                    }
                }
            }
            
            //get the size the fingers, and calculate the total angle of these fingers
            int countFinger = (unsigned int)fingerPoint.size();
            int angle = 0;
            if( countFinger <= 5){
                for ( int x = 0; x < countFinger; x++){
                    angle = angle + abs (angleToCenter(fingerPoint[x], centerP) );
                }
            }
            doAction( angle, countFinger);
            putText(dest, integerToString(countFinger), printPoint, 1, 5, Scalar(0, 255, 0), 1, 5, false);
        }
    }
}

//action performed based on the number of fingers and the total angle
//1.  5 fingers && total angle: 270 - 285
//2.  4 fingers && total angle: 240 - 255
//3.  3 fingers && total angle: 190 - 210
//4.  2 fingers && total angle: 120 - 130
//5.  1 finger && total angle:  65 - 75
void doAction(const int totalAngleOfFinger, const int fingerSize){
    int l_motor =0, r_motor = 0;
    if( totalAngleOfFinger>= 270 && totalAngleOfFinger <= 285 && (fingerSize == 5  )){
        l_motor = 30;
        r_motor = 30;
    }
    else if( totalAngleOfFinger >= 190 && totalAngleOfFinger <= 210 && fingerSize == 3){
        l_motor = 0;
        r_motor = 0;
    }
    //    else if( totalAngleOfFinger >= 240 && totalAngleOfFinger <= 255 && fingerSize == 4)
    //
    //    else if( totalAngleOfFinger >= 120 && totalAngleOfFinger <= 130 &&  (fingerSize == 2 ))
    //
    //    else if( totalAngleOfFinger >= 65 && totalAngleOfFinger <= 75 &&  (fingerSize == 1 ) )
    //
}


