#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

#define WINDOW_NAME "【程序窗口】"

void onMouseChange(int event, int x, int y, int flags, void* param);

bool g_isDraw;
Rect g_rect;
Point g_position;

int main()
{
	//参数准备
	Mat srcImg(500, 500, CV_8UC3, Scalar::all(0));
	Mat mask = srcImg(Rect(100, 100, 50, 50));
	mask = Scalar(0, 0, 255);
	mask = srcImg(Rect(200, 200, 50, 50));
	mask = Scalar(0, 255, 0);
	mask = srcImg(Rect(300, 300, 50, 50));
	mask = Scalar(255, 0, 0);
	g_rect = Rect(-1, -1, 0, 0);
	//设置鼠标事件
	namedWindow(WINDOW_NAME);
	setMouseCallback(WINDOW_NAME, onMouseChange, (void*)&srcImg);//一个类型的指针可以转换成其他类型的指针
	//循环显示图片
	while (1) {
		Mat tmpImg = srcImg.clone();
		if (g_isDraw) {
			cv::rectangle(tmpImg, g_rect, Scalar(255, 255, 255));
		}
		//绘制文字,保证只要鼠标在图片上面就会绘制这个图片
		char text[100];
		sprintf_s(text,"pos:(%d,%d),RGB:(%d %d %d)", g_position.y, g_position.x, tmpImg.at<Vec3b>(g_position.x, g_position.y)[2], tmpImg.at<Vec3b>(g_position.x, g_position.y)[1], tmpImg.at<Vec3b>(g_position.x, g_position.y)[0]);
		if (g_position.x >= 0 && g_position.x < srcImg.cols && g_position.y >= 0 && g_position.y < srcImg.rows) {
			putText(tmpImg, text, g_position, FONT_HERSHEY_PLAIN, 1.0, Scalar(255,255,255));
		}
		imshow(WINDOW_NAME, tmpImg);
		if (waitKey(10) == 27) break;
	}
}

void onMouseChange(int event, int x, int y, int flags, void* param)
{
	Mat& Img = *(cv::Mat*)param;
	switch (event)
	{
	case EVENT_MOUSEMOVE://记录当前坐标，并且显示当前坐标
	{
		if (g_isDraw) {
			g_rect.width = x - g_rect.x;
			g_rect.height = y - g_rect.y;
		}

		g_position.y = y;
		g_position.x = x;
	}
	break;
	case EVENT_LBUTTONDOWN:
	{
		g_rect = Rect(x, y, 0, 0);
		g_isDraw = true;
	}
	break;
	case EVENT_LBUTTONUP:
	{
		if (g_rect.width < 0) {
			g_rect.x += g_rect.width;
			g_rect.width *= -1;
		}
		if (g_rect.height < 0) {
			g_rect.y += g_rect.height;
			g_rect.height *= -1;
		}
		g_isDraw = false;
		cv::rectangle(Img, g_rect, Scalar(255, 255, 255));
	}
	break;
	}
}