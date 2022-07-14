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

	cout << img.size() << endl;	// 768 * 550
	
	Mat imgResized , imgResized1;
	resize(img, imgResized, Size(640,480));	// 长宽比改变
	resize(img, imgResized1, Size(),0.5,0.5);	// 缩放

	
	Rect roi(100, 100,300,250);
	Mat imgCroped = img(roi);

	imshow("Image", img);
	imshow("Image Resized", imgResized);
	imshow("Image imgResized1", imgResized1);
	imshow("Image imgCroped", imgCroped);

	waitKey();
	return 0;
}
```

