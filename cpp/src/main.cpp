#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/aruco.hpp>
#include "../headers/CalPhoto.h"
#include "../headers/Calibration.h"
#include "../headers/parameters.h"
#include<ctime>
#include<math.h>
using namespace std;

void init(){
    string imgList = "../cali/imglists.txt";
    string caliParameter = "../cali/cali.yml";
    Calibration cal;
    cal.CameraCalibration(imgList,caliParameter);
}
void rotationEstimate(cv::Vec3d &Rvet,double &x,double &y,double &z){
    auto deg = Rvet[2];
    cv::Mat R(3,3,CV_16F);
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
void distanceEstimate(cv::Vec3d &Tvec,double &distance){
    distance= Tvec[2];
    return;
}

int main() {
    /*
     * Initialize the system
     */
    cv::FileStorage configFile("../cali/cali.yaml",FileStorage::READ);
    cv::FileStorage data("./data.yaml",FileStorage::WRITE);
    cv::VideoCapture capture(0);
    if(configFile.isOpened()){
        clog<<"The camera is calibrated"<<endl;
    }else{
        clog<<"The camera is not calibrated\n"<<endl;
        clog<<"The camera is starting to be calibrated"<<endl;
        if(capture.isOpened()){
            clog<<"Camera is connected, calibration begins";
            init();
        }else{
            clog<<"Please connect the camera and restart the program";
            return -1;
        }
    }
    /*
     * Read the parameters for distortion
     */
    const cv::Mat cameraMatrix = configFile["cameraMatrix"].mat();
    const cv::Mat distCoff = configFile["distCoff"].mat();
    configFile.release();
    /*
     * Parameter setup
     */
    CalPhoto calPhoto;
    calPhoto.cameraMatrix = cameraMatrix;
    calPhoto.distCoff = distCoff;
    //calPhoto.Init(capture);
    cv::Mat img;
    cv::Mat QRcode;
    vector<string> decodeinfor;
    vector<vector<cv::Point2f>> corners;
    vector<cv::Mat> cornera;
    vector<int> ids;
    MeasuredData MeasuredData;
    Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    /*
     * Define the landing area
     */
    clog<<"Do you want to define the landing area(default area is set to the center of the image)[yes/no]"<<endl;
    string ans ="";
    while(!(ans=="yes"||ans=="no")){
        cin>>ans;
    }
    if(ans=="no")
        clog<<"use the Default Setting"<<endl;
    else{
        clog<<"Starting to find a place to land"<<endl;
        calPhoto.Init(capture);
    }
    /*
     * Starting to detect QR code for identification
     */
    while(!capture.getExceptionMode()){
        capture >> img;
        cv::imshow("raw",img);
        cv::waitKey(30);
        string msg = QRCodeDetector().detectAndDecode(img,QRcode);
        if(msg ==""){// if QR code is not identified: detection continue
            continue;
        }else{
            clog<<"the message is:\n"<<"\t"<<msg<<endl;
            cv::aruco::detectMarkers(img,dict,corners,ids);//Starting to detect the aruco code
            if(!ids.empty()){//if detection is work
                cv::aruco::estimatePoseSingleMarkers(corners,20,cameraMatrix,distCoff,MeasuredData.R,MeasuredData.T);
                cv::aruco::drawAxis(img,cameraMatrix,distCoff,MeasuredData.R,MeasuredData.T,10);
                cv::imshow("pose",img);
                //cv::accumulate(Rvec,Rvec[0]);
                //cv::accumulate(Tvec,Tvec[0]);
                //calPhoto.mean(Rvec);
                //calPhoto.mean(Tvec);
                clog<<"Rvec is "<< MeasuredData.R[0]<<endl;
                clog<<"Distance is "<<MeasuredData.T[0]<<endl;
                //double x=0.0,y=0.0,z=0.0,distance=0.0;
                //rotationEstimate(Rvec[0],x,y,z);
                //distanceEstimate(Tvec[0],distance);
                string t = GetLocalTimeWithMs();
                cv::FileStorage data("./data.yaml",cv::FileStorage::APPEND);
                data<<"time_"+ t <<"{:"<<"message"<<msg<<"Rvec"<<MeasuredData.R<<"Tvec"<<MeasuredData.T<<"}";
                data.release();
                int key = cv::waitKey(30);
                if(32==(uchar)key){
                    t.append(".jpg");
                    cv::imwrite(t,img);
                }
            }
        }
    }
    return 0;
}

