```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

void getContours(Mat imgDilation, Mat img) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDilation, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	string objType;

	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);
		cout << i << ":" << area << endl;

		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			cout << i << "近似点数量:" << conPoly[i].size() << endl;


			//drawContours(img, contours, i, Scalar(255, 0, 255), 2);
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);

			boundRect[i] = boundingRect(conPoly[i]);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);

			int objCor = conPoly[i].size();
			if (objCor == 3) {
				objType = "Tri";
			}
			else if (objCor == 4) {
				float aspRatio = (float)boundRect[i].width / boundRect[i].height;
				
				if (0.95 < aspRatio && aspRatio < 1.05)
					objType = "Square";
				else
					objType = "Rect";
			}
			else if (objCor > 4) {
				objType = "Circle";
			}
			putText(img, objType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255), 1);
		}
	}
}

int main() {

	string path = "Resources/shapes.png";
	Mat img = imread(path);

	// Preprocess
	Mat imgGray, imgBlur, imgCanny;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);

	Mat imgDil;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	// End Preprocess

	getContours(imgDil, img);


	imshow("Image", img);
	imshow("Image Gray", imgGray);
	imshow("Image Blur", imgBlur);
	imshow("Image Canny", imgCanny);
	imshow("Image Dilation", imgDil);


	waitKey();

	return 0;
}
```

![image-20220714144726710](ShapesContour%20Detection.assets/image-20220714144726710.png)

