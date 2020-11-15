#include <iostream>
#include<opencv2/opencv.hpp>
#include "../headers/CalPhoto.h"
#include "../headers/Calibration.h"
#include <aruco/aruco.h>
#include<math.h>
using namespace std;

void init(){
    string imgList = "../cali/list.txt";
    string caliParameter = "../cali/cali.yml";
    Calibration cal;
    cal.CameraCalibration(imgList,caliParameter);
}
void rotationEstimate(cv::Mat &Rvet,double &x,double &y,double &z){
    auto deg = Rvet.at<double>(0,2);
    cv::Mat R(3,3,Rvet.type());
    cv::Rodrigues(Rvet,R);
    double sy = sqrt(R.at<double>(0,0)*R.at<double>(0,0)+R.at<double>(1,0)*R.at<double>(1,0));
    if(!(sy<0.000001)){
        x = atan2(R.at<double>(2,1),R.at<double>(2,2));
        y = atan2(-R.at<double>(2,0),sy);
        z = atan2(R.at<double>(1,0),R.at<double>(0,0));
    }else{
        x = atan2(-R.at<double>(1,2),R.at<double>(1,1));
        y = atan2(R.at<double>(2,0),sy);
        z = 0.0;
    }
    x = x * 180.0/M_PI;
    y = y * 180.0/M_PI;
    z = z * 180.0/M_PI;
    return;
}
void distanceEstimate(cv::Mat &Tvec,double &distance){
    distance= ((Tvec.at<double>(0,0,2)+0.02)*0.0254)*100;
    return;
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
    const cv::Mat cameraMatrix = configFile["cameraMatrix"].mat();
    const cv::Mat distCoff = configFile["distCoff"].mat();
    configFile.release();
    aruco::CameraParameters cameraParameters;
    cameraParameters.CameraMatrix = cameraMatrix;
    cameraParameters.Distorsion = distCoff;
    /*
     * Parameter setup
     */
    CalPhoto calPhoto;
    calPhoto.cameraMatrix = cameraMatrix;
    calPhoto.distCoff = distCoff;
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
            CalPhoto().mean(Rvec); //pose estimating, get rotation matrix
            CalPhoto().mean(Tvec); //pose estimating, get transpose matrix
            double x=0.0,y=0.0,z=0.0,distance=0.0;
            rotationEstimate(Rvec[0],x,y,z);
            distanceEstimate(Tvec[0],distance);
        }
    }
    return 0;
}

