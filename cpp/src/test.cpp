#include "../headers/Calibration.h"
#include "../headers/CalPhoto.h"
#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>
#include<opencv2/aruco.hpp>
#include<vector>
#include<ctime>
#include "../headers/parameters.h"
using namespace std;
using namespace cv;
static ostream& operator<<(ostream& out,MeasuredData &measuredData){
    out<<"{"<<endl;
    out<<"R"<<measuredData.R[0]<<",";
    out<<"T"<<measuredData.T[0]<<"}";
}
void generateAruco(){
    Mat marker;
    Mat paper = Mat::ones(2970,2100,CV_8U)*255;
    cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    int id = 33;/*
    for(int x = 100;x<paper.cols;x+=(paper.cols-marker.cols-200)){
        for(int y = 100;y<paper.rows;y+=paper.rows-marker.rows-200){
            aruco::drawMarker(dict,id,400,marker,1);
            cv::Rect rect = cv::Rect(x,y,marker.cols,marker.rows);
            marker.copyTo(paper(rect));
            id++;
        }
    }*/
    int x=50,y=400;
    aruco::drawMarker(dict,id,2000,marker,1);
    cv::Rect rect = cv::Rect(x,y,marker.cols,marker.rows);
    marker.copyTo(paper(rect));
    imshow("marker",paper);
    imwrite("./aruco_test.jpg",paper);
    waitKey();
}
void captureImage(){
    cv::VideoCapture cap(0);
    fstream listwrite("../cali/test_list.txt",ios_base::out);
    int delay = 30;
    string filename[] = {"../cali/test_1.jpg\r","../cali/test_2.jpg\r","../cali/test_3.jpg\r","../cali/test_4.jpg\r",
    "../cali/test_5.jpg\r","../cali/test_6.jpg\r","../cali/test_7.jpg\r","../cali/test_8.jpg\r","../cali/test_9.jpg\r",
    "../cali/test_10.jpg\r","../cali/test_11.jpg\r","../cali/test_12.jpg\r","../cali/test_13.jpg\r","../cali/test_14.jpg\r",
    "../cali/test_15.jpg\r","../cali/test_16.jpg\r","../cali/test_17.jpg\r","../cali/test_18.jpg\r","../cali/test_20.jpg\r"};
    for(int i = 0;i< 1;){
        Mat img;
        cap>>img;
        cv::imshow("test",img);
        int key=cv::waitKey(delay);
        if(32==(char)key) {
            cv::imwrite(filename[i], img);
            cout <<"calibrate image"<< i <<" is saved"<<endl;
            listwrite<<filename[i];
            i++;
        }

    }
}
void QRcodedetect(){
    QRCodeDetector qrcode;
    cv::VideoCapture cap(0);
    int delay = 30;
    Mat img;
    while(cap.isOpened()){
        Mat qr;
        cap>>img;
        cv::imshow("img",img);
        cv::waitKey(delay);
        bool flag = qrcode.detect(img,qr);
        if(flag){
            string msg = qrcode.decode(img,qr);
            if (msg!=""){
                cout<<msg;
                cv::waitKey(1000);
            }
        }
    }
}
void arucodecode() {
    cv::VideoCapture cap(0);
    string filename = "../cali/cali.yaml";
    cv::FileStorage cameramatrix(filename, cv::FileStorage::READ);
    cv::FileStorage data("./data.yaml",cv::FileStorage::WRITE);
    Mat markers;
    Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    vector<int> markerids;
    vector<vector<cv::Point2f>> coners;
    vector<cv::Vec3d> rvec, tvec;
    MeasuredData MeasuredData;
    cv::Mat cameraMatrix,distCoff;
    cameramatrix["cameraMatrix"]>>cameraMatrix;
    cameramatrix["distCoff"]>>distCoff;
    while (1) {
        cap>>markers;
        cv::imshow("raw",markers);
        cv::aruco::detectMarkers(markers, dict, coners, markerids);
        if (!markerids.empty()) {
            cv::aruco::drawDetectedMarkers(markers, coners, markerids);
            cv::aruco::estimatePoseSingleMarkers(coners, 20,cameraMatrix,distCoff, rvec, tvec);
            cv::aruco::drawAxis(markers,cameraMatrix,distCoff, rvec, tvec,10);
            clog<< "rvec is " << rvec[0]<<" "<<endl;
            clog<<"tvec is"<< tvec[0]<< " "<<endl;
            cv::imshow("detect",markers);
        }
        int key = cv::waitKey(30);
        if(32==(uchar)key){
            time_t now = time(0);
            string t = ctime(&now);
            t.pop_back();
            t.append(".jpg");
            MeasuredData.R = rvec;
            MeasuredData.T = tvec;
            data<<t<<MeasuredData;
            cv::imwrite(t,img);
        }
    }
    //aruco::detectMarkers(markers,dict,)
}

void calibrate(){
    Calibration().CameraCalibration("../cali/imglists.txt","../cali/cali.yaml");
}
void gettime(){
    time_t now = time(0);
    string t = ctime(&now);
    t.pop_back();
    //t.replace(t.begin(),t.end()," ","_");
    t.append(".jpg");
    clog<<t<<endl;
}

int main(){
    //captureImage();
    //calibrate();
    //QRcodedetect();
    //generateAruco();
    arucodecode();
    //gettime();
    //cout<<"hello world"<<endl;
    return 0;

}