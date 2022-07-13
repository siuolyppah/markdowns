https://www.bilibili.com/video/BV11A411T7rL?t=284.1



VC++ Directories:

1. Add Build Directories（包含目录）: D:\opencv\build\include

2. Add Library Directories（库目录）: D:\opencv\build\x64\vc15\lib

3. Add Linker input：opencv_world455d.lib

   > - d for debug; 
   > - without d for release.



测试程序：

```C++
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	string path = "Resources/test.png";

	Mat img = imread(path);
	imshow("Image", img);

	waitKey(0);
	return 0;
}
```

