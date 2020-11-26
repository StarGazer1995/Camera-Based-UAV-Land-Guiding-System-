//
// Created by parallels on 10/15/20.
//

#ifndef SPECIALCOURSE_PARAMETERS_H
#define SPECIALCOURSE_PARAMETERS_H

#endif //SPECIALCOURSE_PARAMETERS_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <sys/time.h>
using namespace cv;
using namespace std;

struct MeasuredData {
    vector<cv::Vec3d> R;
    vector<cv::Vec3d> T;
};
static string GetLocalTimeWithMs(void)
{
    string defaultTime = "19700101000000000";
    try
    {
        struct timeval curTime;
        gettimeofday(&curTime, NULL);
        int milli = curTime.tv_usec / 1000;

        char buffer[80] = {0};
        struct tm nowTime;
        localtime_r(&curTime.tv_sec, &nowTime);//把得到的值存入临时分配的内存中，线程安全
        strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", &nowTime);

        char currentTime[84] = {0};
        snprintf(currentTime, sizeof(currentTime), "%s%03d", buffer, milli);

        return currentTime;
    }
    catch(const std::exception& e)
    {
        return defaultTime;
    }
    catch (...)
    {
        return defaultTime;
    }
}