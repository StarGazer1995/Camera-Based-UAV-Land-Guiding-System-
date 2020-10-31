//
// Created by parallels on 10/8/20.
//

#ifndef SPECIALCOURSE_CALIBRATION_H
#define SPECIALCOURSE_CALIBRATION_H

#endif //SPECIALCOURSE_CALIBRATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Calibration{
public:
    void CameraCalibration(const char infilepath[],const char outfilepath[]){
        /*
         * infilepath: path of images for calibration
         * outfilepath: path of calibration matrix
         */
        ifstream fin(infilepath);
        FileStorage f(outfilepath,FileStorage::WRITE);
        int image_cnt = 0;
        cv::Size img_size;
        cv::Size board_size = cv::Size(11,8);
        vector<cv::Point2f> corners;
        vector<vector<cv::Point2f>> image_points_seq;
        vector<cv::Point3f> objp;
        vector<vector<cv::Point3f>> object_point;
        Mat img;
        /* define the size of checkboard*/
        int CHECHBOARD[2]{6,9};
        string filename;
        vector<string> filenames;
        for(int i=0;i<CHECHBOARD[1];i++){
            for(int j = 0;j<CHECHBOARD[0];j++){
                objp.push_back(cv::Point3f(j,i,0));
            }
        }
        while(std::getline(fin,filename)){
            image_cnt++;
            img = imread(filename);
            filenames.push_back(filename);
            /*get the width and height of the 1st picture*/
            if(image_cnt==1){
                img_size.width = img.cols;
                img_size.height = img.rows;
            }

            /*trying to find the corner points*/
            if(cv::findChessboardCorners(img,board_size,corners)){

                cout<<"Cannot find any Chessboard";
                exit(1);
            }
            else{
                Mat view_gray;
                cvtColor(img, view_gray, COLOR_RGB2GRAY);
                cv::TermCriteria criteria = cv::TermCriteria(TermCriteria::EPS | TermCriteria::Type::MAX_ITER, 30, 0.001);
                cornerSubPix(view_gray, corners, Size(5, 5), Size(-1, -1), criteria);

                object_point.push_back(objp);
                image_points_seq.push_back(corners);  // 保存亚像素角点
                /* 在图像上显示角点位置 */
                drawChessboardCorners(view_gray, board_size, corners, false); // 用于在图片中标记角点
                imshow("Camera Calibration", view_gray);       // 显示图片
                waitKey(500); //暂停0.5S
            }

        }
        cv::Mat cameraMatrix,distCoff,RotationMatrix,VectorTranscationMatrix;
        cv::calibrateCamera(object_point,image_points_seq,cv::Size(img.cols,img.rows),cameraMatrix,distCoff,RotationMatrix,VectorTranscationMatrix);

        /*Write Calibration matrices*/
        f<<"cameraMatrix: "<<"{"<<cameraMatrix<<"}/n";
        f<<"distCoff: "<<"{"<<distCoff<<"}/n";
        f<<"RotationMatrix: "<<"{"<<RotationMatrix<<"}/n";
        f<<"VectorTranscationMatrix: "<<"{"<<VectorTranscationMatrix<<"}/n";
        f.release();
        return;

    }

};