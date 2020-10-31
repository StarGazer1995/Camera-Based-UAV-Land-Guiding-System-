# Project Discription

This project is a special course offered to the author. The project is mainly about the Computer Vision. The whole project is to develop a camera based landing guidance system for drones. It assembled by mainly 3 parts:**Calibration, Object Detection, and Post Estimation. **

## Calibration 

The camera used in the system is a stero camera designed for the raspberry pi, which does not provide the intrinsic matrix and distortion coefficient matrix.

## Object Detection 

### Land mark detection and measure



### Target[ QR code] measure

We use the similar theorem to measure the discance of the target. The equation could be written down like the following
$$
F = \frac{P*D}{W}
$$
Where F stands for the focal length of the camera, D is the distance between a specific object and the camera, P stands for the width( in pixel) of the Object, and W counts for the actual width of the object.

We could calculate the distance between the object and the camera if we know the Focal Length, the with in pixel and in actual of the object. So, the equation could be written like the following:
$$
D = \frac{F*W}{P}
$$

## Postion Estimation

Pose estimation plays a very important role in the computer vision field. It serves for Robot navigation, Argumented Reality, and etc. Since we are navigating a drone to land on a specific place, a pose estimation is needed. After the identification of the drone, we will start to process estimating the pose of it. In this section, we will use ARUCO markers to do the job. 

ARUCO markers are markers that contain pose information. By scanning these markers, the pose of the drone could thus be estimated.

### Mathmatics Theorem

Flying target has six free domains, pitch, row, and yaw. These six domains count of three rotations in xyz axises. Since the QR code ,embeded on a drone, is a square, the opsition of the drone could be calculated out by mearsuring the difference between the width and height of the mark in pixel.

To estimate the pose of an object within a signal image. PNP method is employed in this project. The classic form of PNP problem is to determine the 6D DoF 