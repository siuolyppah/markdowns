```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/objdetect.hpp>

#include <iostream>

using namespace std;
using namespace cv;



int main() {

	string path = "Resources/test.png";
	Mat img = imread(path);

	CascadeClassifier faceCascade;
	faceCascade.load("Resources/haarcascade_frontalface_default.xml");	// 2:17:50

	if (faceCascade.empty()) {
		cout << "XML加载失败" << endl;
	}

	vector<Rect> faces;
	faceCascade.detectMultiScale(img, faces, 1.1, 10);
	for (int i = 0; i < faces.size(); i++) {
		rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);
	}


	imshow("Image", img);


	waitKey();

	return 0;
}
```

![image-20220714150700186](Face%20Detection.assets/image-20220714150700186.png)