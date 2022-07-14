# Color Dection

> 在hsv空间，可以获取关于颜色的更多信息。
>
> > - h：色调
> > - s：饱和度
> > - v：明度

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;


int main() {

	string path = "Resources/lambo.png";
	Mat img = imread(path);

	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	int hmin = 0, smin = 110, vmin = 153;
	int hmax = 19, smax = 240, vmax = 255;

	Mat mask;
	Scalar lower(hmin,smin,vmin);
	Scalar upper(hmax,smax,vmax);
	inRange(imgHSV,lower,upper,mask);

	imshow("Image", img);
	imshow("Image HSV", imgHSV);
	imshow("Image Mask", mask);

	waitKey();
	return 0;
}
```

![image-20220714131836538](Color%20Detection.assets/image-20220714131836538-16577759169721.png)

![image-20220714131843517](Color%20Detection.assets/image-20220714131843517.png)

![image-20220714131850305](Color%20Detection.assets/image-20220714131850305.png)



# Track Bar

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;


int hmin = 0, smin = 110, vmin = 153;
int hmax = 19, smax = 240, vmax = 255;
Mat mask;

int main() {

	string path = "Resources/lambo.png";
	Mat img = imread(path);

	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);


	namedWindow("TrackBars", (640, 200));
	createTrackbar("Hue Min", "TrackBars", &hmin, 179);
	createTrackbar("Hue Max", "TrackBars", &hmax, 179);
	createTrackbar("Sat Min", "TrackBars", &smin, 255);
	createTrackbar("Sat Max", "TrackBars", &smax, 255);
	createTrackbar("Val Min", "TrackBars", &vmin, 255);
	createTrackbar("Val Max", "TrackBars", &vmax, 255);

	while (true)
	{
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);

		imshow("Image", img);
		imshow("Image HSV", imgHSV);
		imshow("Image Mask", mask);

		waitKey(1);
	}
	return 0;
}
```

