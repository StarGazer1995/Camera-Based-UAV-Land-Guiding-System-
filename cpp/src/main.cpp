#include <iostream>
#include<opencv2/opencv.hpp>
#include "../headers/CalPhoto.h"
#include "../headers/Calibration.h"
#include "../headers/parameters.h"
#include <aruco/aruco.h>
#include <algorithm>

using namespace std;

void init(){
    char imgList[] = "../img/list.txt";
    char caliParameter[] = "./cali.yml";
    Calibration cal;
    cal.CameraCalibration(imgList,caliParameter);
}


int main() {
    /*
     * Initialize the system
     */
    cv::FileStorage configFile("./cali.yml",FileStorage::READ);
    cv::VideoCapture capture(0);
    if(configFile.isOpened()){
        cout<<"The camera is calibrated"<<endl;
    }else{
        cout<<"The camera is not calibrated/n";
        cout<<"The camera is starting to be calibrated";
        if(capture.isOpened()){
            cout<<"Camera is connected, calibration begins";
            init();
        }else{
            cout<<"Please connect the camera and restart the program";
            return -1;
        }
    }
    /*
     * Read the parameters for distortion
     */
    configFile["cameraMatrix"] >> cameraMatrix;
    configFile["distCoff"] >> distCoffe;
    aruco::CameraParameters cameraParameters;
    cameraParameters.CameraMatrix = cameraMatrix;
    cameraParameters.Distorsion = distCoffe;
    configFile.release();
    /*
     * Initializing Camera
     */
    CalPhoto calPhoto;
    calPhoto.Init(capture);
    cv::Mat img;
    cv::Mat QRcode;
    vector<string> decodeinfor;
    vector<vector<cv::Point2f>> corners;
    vector<aruco::Marker> markers;
    aruco::MarkerDetector detector;
    aruco::MarkerPoseTracker estimator;
    vector<cv::Mat> Tvec;
    vector<cv::Mat> Rvec;
    /*
     * Starting to detect QR code for identification
     */
    while(!capture.getExceptionMode()){
        capture >> img;
        if(!QRCodeDetector().detect(img,QRcode)){// if QR code is not identified: detection continue
            cameraParameters.CamSize = img.size();
            continue;
        }
        markers = detector.detect(img);//Starting to detect the aruco code
        if(!markers.empty()){//if detection is work
            for(int i = 0; i<markers.size();i++) {
                estimator.estimatePose(markers[0], cameraParameters, 0.01);//The 3rd parameter needs to be changed
                Rvec.push_back(estimator.getRvec());
                Tvec.push_back(estimator.getTvec());
            }
            for(int i = 1;i<Rvec.size();i++){
                cv::accumulate(Rvec[i],Rvec[0]);
                cv::accumulate(Tvec[i],Tvec[0]);
            }
            CalPhoto().mean(Rvec);
            CalPhoto().mean(Tvec);
        }
    }
    return 0;
}

