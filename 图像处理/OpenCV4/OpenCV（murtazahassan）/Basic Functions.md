# RGB 转为 灰度

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main() {
	string path = "Resources/test.png";
	Mat img = imread(path);
	
	Mat imgGray;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);

	imshow("Image", img);
	imshow("Image Gray", imgGray);

	waitKey();

	return 0;
}
```



# 添加模糊

## 高斯模糊

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main() {
	string path = "Resources/test.png";
	Mat img = imread(path);
	
	Mat imgGray, imgBlur;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(img, imgBlur,Size(7,7),5,0);

	imshow("Image", img);
	imshow("Image Gaussian Blur", imgBlur);
	imshow("Image Gray", imgGray);

	waitKey();

	return 0;
}
```



# 边缘检测

## Canny边缘检测器

> 边缘检测前，通常会进行模糊处理。

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main() {
	string path = "Resources/test.png";
	Mat img = imread(path);
	
	Mat imgBlur, imgCanny;
	GaussianBlur(img, imgBlur,Size(7,7),5,0);
	Canny(imgBlur, imgCanny, 50,150);

	imshow("Image", img);
	imshow("Image Gaussian Blur", imgBlur);
	imshow("Image Canny", imgCanny);

	waitKey();

	return 0;
}
```



# 扩大和侵蚀图像

https://www.bilibili.com/video/BV11A411T7rL?t=2689.2



# track bars（轨迹栏）



