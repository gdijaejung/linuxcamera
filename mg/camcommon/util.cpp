
//#include "camcommon.h"
#include "../stdafx.h"
#include "util.h"


using namespace cv;


/**
* Helper function to find a cosine of angle between vectors
* from pt0->pt1 and pt0->pt2
*/
double angle(cv::Point pt1, const cv::Point &pt2, const cv::Point &pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/**
* Helper function to display text in the center of a contour
*/
void setLabel(cv::Mat& im, const std::string &label, const std::vector<cv::Point>& contour, const cv::Scalar &color)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, color, thickness, 8);
}


void horizLines(cv::Mat &image, const int w, const int h, const int div)
{
	const bool startBlank = true;
	const int h2 = h / (div * 2);
	int yOffset = 0;

	for (int k = 0; k < div; ++k)
	{
		yOffset = k * h2 * 2;
		if (startBlank)
			yOffset += h2;

		for (int i = 0; i < h2; ++i)
			line(image, cv::Point(0, i + yOffset), cv::Point(w, i + yOffset), cv::Scalar(0, 0, 0));
	}
}


void verticalLines(cv::Mat &image, const int w, const int h, const int div)
{
	const bool startBlank = true;
	const int w2 = w / (div * 2);
	int xOffset = 0;

	for (int k = 0; k < div; ++k)
	{
		xOffset = k * w2 * 2;
		if (startBlank)
			xOffset += w2;

		for (int i = 0; i < w2; ++i)
			line(image, cv::Point(i + xOffset, 0), cv::Point(i + xOffset, h), cv::Scalar(0, 0, 0));
	}
}


// 체스보드 출력
void chessboard(cv::Mat &image, const int w, const int h, const int rows, const int cols)
{

}


// contours 를 순서대로 정렬한다.
// 0 ---------- 1
// |            |
// |            |
// |            |
// 2 ---------- 3
void OrderedContours(const vector<cv::Point> &src, OUT vector<cv::Point> &dst)
{
	// 가장 큰 박스를 찾는다.
	cv::Point center = src[0];
	center += src[1];
	center += src[2];
	center += src[3];

	// 중심점 계산
	center.x /= 4;
	center.y /= 4;

	vector<bool> check(4, false);
	vector<bool> check2(4, false);

	// 시계방향으로 left top 부터 순서대로 0, 1, 2, 3 위치에 포인트가 저장된다.
	for (int k = 0; k < 4; ++k)
	{
		if (!check[0] && (src[k].x < center.x) && (src[k].y < center.y))
		{
			dst[0] = src[k];
			check[0] = true;
			check2[k] = true;
		}
		if (!check[3] && (src[k].x < center.x) && (src[k].y > center.y))
		{
			dst[3] = src[k];
			check[3] = true;
			check2[k] = true;
		}
		if (!check[1] && (src[k].x > center.x) && (src[k].y < center.y))
		{
			dst[1] = src[k];
			check[1] = true;
			check2[k] = true;
		}
		if (!check[2] && (src[k].x > center.x) && (src[k].y > center.y))
		{
			dst[2] = src[k];
			check[2] = true;
			check2[k] = true;
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		if (!check[i])
		{
			for (int k = 0; k < 4; ++k)
			{
				if (!check2[k])
				{
					check[i] = true;
					check2[k] = true;
					dst[i] = src[k];
				}
			}
		}
	}


}


// contours 를 x가 작은 순서대로 정렬한다.
// 0 --- 1 --- 2 --- N
void OrderedLineContours(const vector<cv::Point> &src, OUT vector<cv::Point> &dst)
{
	dst = src;
	for (u_int i = 0; i < dst.size() - 1; ++i)
	{
		for (u_int k = i + 1; k < dst.size(); ++k)
		{
			if (dst[i].x > dst[k].x)
			{
				cv::Point tmp = dst[i];
				dst[i] = dst[k];
				dst[k] = tmp;
			}
		}
	}
}


// 선을 그린다.
void DrawLines(cv::Mat &dst, const vector<cv::Point> &lines, const cv::Scalar &color, const int thickness,
	const bool isLoop)
{
	if (lines.size() < 2)
		return;

	for (u_int i = 0; i < lines.size() - 1; ++i)
		line(dst, lines[i], lines[i + 1], color, thickness);

	if (isLoop)
		line(dst, lines[lines.size() - 1], lines[0], color, thickness);
}



// hhttp://www.gisdeveloper.co.kr/15
// AP1 - AP2 를 지나는 직선
// AP3 - AP4 를 지나는 직선
// 두 직선의 교점을 찾아 IP에 저장한다.
// 교점이 없다면 false를 리턴한다.
bool GetIntersectPoint(const cv::Point2f& AP1, const cv::Point2f& AP2,
	const cv::Point2f& BP1, const cv::Point2f& BP2, cv::Point2f* IP)
{
	float t;
	float s;
	float under = (BP2.y - BP1.y)*(AP2.x - AP1.x) - (BP2.x - BP1.x)*(AP2.y - AP1.y);
	if (under == 0) return false;

	float _t = (BP2.x - BP1.x)*(AP1.y - BP1.y) - (BP2.y - BP1.y)*(AP1.x - BP1.x);
	float _s = (AP2.x - AP1.x)*(AP1.y - BP1.y) - (AP2.y - AP1.y)*(AP1.x - BP1.x);

	t = _t / under;
	s = _s / under;

	if (t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;
	if (_t == 0 && _s == 0) return false;

	IP->x = AP1.x + t * (float)(AP2.x - AP1.x);
	IP->y = AP1.y + t * (float)(AP2.y - AP1.y);

	return true;
}


cv::Point2f Pt2Pt2f(const cv::Point &pt)
{
	return cv:: Point2f((float)pt.x, (float)pt.y);
}

cv::Point Pt2f2Pt(const cv::Point2f &pt)
{
	return cv::Point((int)pt.x, (int)pt.y);
}


// Skew 변환 행렬을 계산해서 리턴한다.
// scale 값 만큼 contours를 늘려 잡는다.
cv::Mat SkewTransform(const vector<cv::Point> &contours, const int width, const int height, const float scale)
{
	vector<Point2f> quad(4);
	quad[0] = Point2f(0, 0);
	quad[1] = Point2f((float)width, (float)0);
	quad[2] = Point2f((float)width, (float)height);
	quad[3] = Point2f((float)0, (float)height);


	Point2f ip; // 중점
	if (!GetIntersectPoint(contours[0], contours[2], contours[3], contours[1], &ip))
		return Mat();

	const Point2f offset1 = (Pt2Pt2f(contours[0]) - ip) * (scale - 1.f);
	const Point2f offset2 = (Pt2Pt2f(contours[1]) - ip) * (scale - 1.f);
	const Point2f offset3 = (Pt2Pt2f(contours[2]) - ip) * (scale - 1.f);
	const Point2f offset4 = (Pt2Pt2f(contours[3]) - ip) * (scale - 1.f);

	vector<Point2f> screen(4);
	screen[0] = Pt2Pt2f(contours[0]) + offset1;
	screen[1] = Pt2Pt2f(contours[1]) + offset2;
	screen[2] = Pt2Pt2f(contours[2]) + offset3;
	screen[3] = Pt2Pt2f(contours[3]) + offset4;

	const cv::Mat tm = getPerspectiveTransform(screen, quad);
	return tm;
}


// 격자무늬 선을 그린다.
void ShowBoxLines(cv::Mat &dst, const int rows, const int cols, const bool isClear)
{
	const float w = (float)dst.cols;
	const float h = (float)dst.rows;
	const float cx = w / cols;
	const float cy = h / rows;

	if (isClear)
		dst.setTo(cv::Scalar(255, 255, 255));

	for (float x = 0; x <= w; x += cx)
	{
		line(dst, cv::Point((int)x, 0), cv::Point((int)x, (int)h), cv::Scalar(0, 0, 0));
	}

	for (float y = 0; y < h; y += cy)
	{
		line(dst, cv::Point(0, (int)y), cv::Point((int)w, (int)y), cv::Scalar(0, 0, 0));
	}
}


cv::Point2f PointNormalize(const cv::Point &pos)
{
	const float d = (float)cv::norm(pos);
	return cv::Point2f((float)pos.x / d, (float)pos.y / d);
}


// Gdiplus::Bitmap* IplImageToBitmap(IplImage *iplImage)
// {
// 	Gdiplus::Bitmap *bitmap = ::new Gdiplus::Bitmap(640, 480, PixelFormat24bppRGB);
// 	IplImageToBitmap(iplImage, bitmap);
// 	return bitmap;
// }

// 
// void IplImageToBitmap(IplImage *iplImage, Gdiplus::Bitmap *out)
// {
// 	Gdiplus::BitmapData bitmapData;
// 	bitmapData.Width = out->GetWidth();
// 	bitmapData.Height = out->GetHeight();
// 	bitmapData.Stride = 3 * bitmapData.Width;
// 	bitmapData.PixelFormat = out->GetPixelFormat();
// 	bitmapData.Scan0 = (VOID*)iplImage->imageData;
// 	bitmapData.Reserved = NULL;
// 
// 	Gdiplus::Status s = out->LockBits(
// 		&Gdiplus::Rect(0, 0, out->GetWidth(), out->GetHeight()),
// 		Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeUserInputBuf,
// 		PixelFormat24bppRGB, &bitmapData);
// 	if (s == Gdiplus::Ok)
// 		out->UnlockBits(&bitmapData);
// }

// 
// IplImage* BitmapToIplImage(Gdiplus::Bitmap *bitmap)
// {
// // 	IplImage* image = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
// // 	CopyMemory(image->imageData, bitmap->, size);
// // 	cvFlip(m_iplImage);
// 	return NULL;
// }
// 

// lineContour로 묶여진 사각형을 정규화 한다.
// cRectContour ContourNormalize(cLineContour contourLines[4])
// {
// 	cRectContour contour;
// 	vector<cv::Point> centers(4);
// 	for (int i = 0; i < 4; ++i)
// 	{
// 		contourLines[i].Normalize();
// 		centers[i] = contourLines[i].Center();
// 	}
// 
// 	// 정규화
// 	contour.Init(centers);
// 
// 	cLineContour lines[4]; // 정규화된 사각형을 저장한다.
// 	for (int i = 0; i < 4; ++i)
// 	{
// 		cv::Point center = contourLines[i].Center();
// 		for (int k = 0; k < 4; ++k)
// 		{
// 			// 가장 근접한 line을 찾는다.
// 			cv::Point dist = (contour.m_contours[k] - center);
// 			const float len = (float)sqrt(dist.x*dist.x + dist.y*dist.y);
// 			if (len < 10)
// 			{
// 				lines[k] = contourLines[i];
// 				break;
// 			}
// 		}
// 	}
// 
// 	// 0 -------- 1
// 	// |          |
// 	// |    +     |
// 	// |          |
// 	// 3 -------- 2
// 	cRectContour retVal;
// 	retVal.m_contours[0] = lines[0].m_contours[0];
// 	retVal.m_contours[1] = lines[1].m_contours[2];
// 	retVal.m_contours[2] = lines[2].m_contours[2];
// 	retVal.m_contours[3] = lines[3].m_contours[0];
// 
// 	return retVal;
// }
// 
// 
// // lineContour로 묶여진 사각형을 정규화 한다.
// // [0] center pos
// // [1] width/2
// // [2] height/2
// //          |
// //          |
// //          |
// //   <------X-------->
// //          |
// //          |
// //          |
// cRectContour ContourNormalize2(cLineContour contourLines[3])
// {
// 	vector<cv::Point> centers(3);
// 	for (int i = 0; i < 3; ++i)
// 	{
// 		contourLines[i].Normalize();
// 		centers[i] = contourLines[i].Center();
// 	}
// 
// 	const int w1 = abs(contourLines[1].m_contours[0].x - centers[0].x);
// 	const int w2 = abs(contourLines[1].m_contours[2].x - centers[0].x);
// 	const int w = max(w1, w2);
// 	const int h = abs(centers[2].y - centers[0].y);
// 
// 	// 0 -------- 1
// 	// |          |
// 	// |    +     |
// 	// |          |
// 	// 3 -------- 2
// 	cRectContour retVal;
// 	retVal.m_contours[0] = centers[0] + Point(-w, -h);
// 	retVal.m_contours[1] = centers[0] + Point(w, -h);
// 	retVal.m_contours[2] = centers[0] + Point(-w, h);
// 	retVal.m_contours[3] = centers[0] + Point(w, h);
// 	retVal.Normalize();
// 
// 	return retVal;
// }

void sleep_ms(int milliseconds)
{
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}
