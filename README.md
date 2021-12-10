# Robot-Line-Following (Hough Transform)

Robot Line Follower with [ Hough transform](https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_houghlines/py_houghlines.html " Hough transform") and QR Scanner using `C++` and `OpenCV` library.

### Problem definition
There is a continuous line with some intersections on the ground. There is a QR code in each intersection that shows the next direction. The robot is assigned to detect and track the line. In this case, the algorithm returns the center of the detected line to align the robot to the center and make it stable.
 
**Note that you can change the color of intended line to be detected by changing the HSV threshold using relevant sliders.**



### Step-by-Step solution

- Capturing real-time frame stream by opening the camera
- Applying preliminary filters to the stream (such as resizing to 160 x 120, reducing noise)
- Filtering the image by the color
- Detecting lines of each frame with Hough transform
- Checking if there is an intersection to make a decision
- Emitting signals containing the direction and position of the line found


 
### Requirements
- C++ Programming Language (version >=11)
- OpenCV (version 2.4.13 is preferred)
- Opencv-Zbar
- Qextserialport
- `QT Creator IDE`



### Run


1. Download the repository by clicking on[ this link](https://github.com/kimiaf1998/Robot-Line-Following/archive/refs/heads/master.zip " this link") then extract it, or clone it using:
```bash
git clone  https://github.com/kimiaf1998/Robot-Line-Following.git
```

2. Open the repository in QT Creator
3. Click the Run button from the IDE or press `CTRL+R` to start the application
4. Change HSV threshold by relevant sliders




### Screenshots


https://user-images.githubusercontent.com/47594854/144767114-3abc308e-41f3-4af5-b809-ff07cc1bbac4.mp4

