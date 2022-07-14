```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main() {

	Mat img(512, 512, CV_8SC3, Scalar(255, 255, 255));	// 512*512的三通道，深度8bit的图片

	// circle(img, Point(256, 256), 155, Scalar(0, 69, 255), 10);	// 外边框厚度为10的圆
	circle(img, Point(256, 256), 155, Scalar(0, 69, 255), FILLED);	// 填充

	//rectangle(img, Point(130, 226), Point(382, 286), Scalar(255, 255, 255),3);	等效写法：
	Rect roi(130, 226, 252, 60);
	rectangle(img, roi, Scalar(255, 255, 255),FILLED);	//类似与cicrle，也可以仅画外边框

	line(img, Point(130, 296), Point(382, 296),Scalar(255,255,255),2);

	putText(img, "Hello,world", Point(137, 262), FONT_HERSHEY_DUPLEX, 1.3, Scalar(0, 69, 255), 2);

	imshow("Image", img);

	waitKey();
	return 0;
}
```

