#include "../headers/Calibration.h"
#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
void captureImage(){
    cv::VideoCapture cap(0);
    int delay = 30;
    string filename = "../cali/test.jpg";
    while(1){
        Mat img;
        cap>>img;
        cv::imshow("test",img);
        int key=waitKey(delay);
        if(27==(char)key) {
            cv::imwrite(filename, img);
            cout <<"image is saved"<<endl;
        }
    }
}
int main(){
    captureImage();
    return 0;

}