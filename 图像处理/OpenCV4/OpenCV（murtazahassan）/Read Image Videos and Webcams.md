# Read and Show Image

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;

int main() {
	string path = "Resources/test.png";
	cv::Mat img = cv::imread(path);
	
	cv::imshow("Image", img);
	
	cv::waitKey();
	return 0;
}
```



# Videos

> 视频，是图片的序列。

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;

int main() {
	string path = "Resources/test_video.mp4";
	cv::VideoCapture cap(path);

	cv::Mat img;

	while (true)
	{
		cap.read(img);
		if(!img.empty())
			cv::imshow("Image", img);

		cv::waitKey(1);
	}

	return 0;
}
```



# Camera

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;

int main() {
    // the id number of camera
	cv::VideoCapture cap(0);

	cv::Mat img;

	while (true)
	{
		cap.read(img);
		if(!img.empty())
			cv::imshow("Image", img);

		cv::waitKey(1);
	}


	return 0;
}
```

