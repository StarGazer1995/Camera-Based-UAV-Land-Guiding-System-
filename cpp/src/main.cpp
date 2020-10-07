#include <iostream>
#include<opencv2/opencv.hpp>
#include "../headers/CalPhoto.h"

using namespace std;
using namespace cv;

int main(int argc,char **argv) {
    VideoCapture capture(0);
    namedWindow("test");
    while (true) {
        Mat frame;
        capture >> frame;
        imshow("test", frame);
        waitKey(30);
    }
    return 0;
}

