![image-20220714125721854](Warp%20Perspective%EF%BC%88%E9%80%8F%E8%A7%86%E5%8F%98%E6%8D%A2%EF%BC%89.assets/image-20220714125721854.png)

![image-20220714125727489](Warp%20Perspective%EF%BC%88%E9%80%8F%E8%A7%86%E5%8F%98%E6%8D%A2%EF%BC%89.assets/image-20220714125727489.png)

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;


int main() {

	string path = "Resources/cards.jpg";
	Mat img = imread(path);

	Point2f src[4] = {
		{529,142},
		{771,190},
		{405,395},
		{674,457}
	};	// 左上、右上、左下、右下

	float w = 250, h = 350;
	Point2f dest[4] = {
		{0.0f,0.0f},
		{w,0.0f},
		{0.0f,h},
		{w,h}
	};

	Mat mat, imgWrap;
	mat = getPerspectiveTransform(src, dest);
	warpPerspective(img, imgWrap, mat, Point(w, h));

	for (int i = 0; i < 4; i++) {
		circle(img, src[i], 10, Scalar(0, 255, 255), FILLED);
	}

	imshow("Image", img);
	imshow("Image Wrap", imgWrap);

	waitKey();
	return 0;
}
```

> 伪射变换，mat是关系变幻矩阵。



