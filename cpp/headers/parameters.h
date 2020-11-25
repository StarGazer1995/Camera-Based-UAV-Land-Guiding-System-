//
// Created by parallels on 10/15/20.
//

#ifndef SPECIALCOURSE_PARAMETERS_H
#define SPECIALCOURSE_PARAMETERS_H

#endif //SPECIALCOURSE_PARAMETERS_H
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;
class dataclaster {
public:
    struct MeasuredData {
        vector<cv::Vec3d> R;
        vector<cv::Vec3d> T;
    };
};
struct MeasuredData {
    vector<cv::Vec3d> R;
    vector<cv::Vec3d> T;
};