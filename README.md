# Robot-Line-Following

Robot Line Follower with [ Hough transform](https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_houghlines/py_houghlines.html " Hough transform") using `C++` and `OpenCV` library.

### Problem definition
There is a continues line with some intersections on the ground and robot is assinged to detect and track the line. In this case, the algorithm return the center position of the detected line to adjust itself to the center and be stable.
 
**Note that you can change the color of intended line to be detected by changing the HSV threshold using relevant sliders.**



### Step-by-Step solution

- Capturing real-time frame stream by opening the camera
- Applying preliminary filters to the stream (such as resizing to 160 x 120, reducing noise)
- Filtering the image by selected color
- Processing each frame to check if the gates exist in the frame
- Emitting direction signals when the gate found, based on the color of the detected gate.



### Requirements
- C++ Programming Language (version >=11)
- OpenCV (version 2.4.13 is preferred)
- `QT Creator IDE`



### Run


1. Download the repository by clicking on[ this link](https://github.com/kimiaf1998/U-Shaped-Gate-Detection/archive/refs/heads/master.zip " this link") then extract it, or clone it using:
```bash
git clone  https://github.com/kimiaf1998/U-Shaped-Gate-Detection.git
```

2. Open the repository in QT Creator
3. Click the Run button from the IDE or press `CTRL+R` to start the application
4. Change yellow and red HSV threshold by relevant sliders




### Screenshots

| <img src="https://github.com/kimiaf1998/U-Shaped-Gate-Detection/blob/master/screenshots/Screenshot%20from%202021-12-03%2017-47-23.png" width="240">  | <img src="https://github.com/kimiaf1998/U-Shaped-Gate-Detection/blob/master/screenshots/Screenshot%20from%202021-12-03%2017-47-48.png" width="240">  |  <img src="https://github.com/kimiaf1998/U-Shaped-Gate-Detection/blob/master/screenshots/Screenshot%20from%202021-12-03%2017-49-26.png" width="240"> |
| ------------ | ------------ | ------------ |
| <img src="https://github.com/kimiaf1998/U-Shaped-Gate-Detection/blob/master/screenshots/Screenshot%20from%202021-12-03%2017-49-39.png" width="240"> | <img src="https://github.com/kimiaf1998/U-Shaped-Gate-Detection/blob/master/screenshots/Screenshot%20from%202021-12-03%2017-48-47.png" width="240"> | <img src="https://github.com/kimiaf1998/U-Shaped-Gate-Detection/blob/master/samples/20190301_153334.jpg" width="240"> |
