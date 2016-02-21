
#include "../stdafx.h"
#include "rectcontour.h"


using namespace cv;


cRectContour::cRectContour() :
	m_contours(4)
{

}

cRectContour::cRectContour(const vector<cv::Point> &contours) :
	m_contours(4)
{
	Init(contours);
}

cRectContour::cRectContour(const vector<cv::KeyPoint> &contours) :
	m_contours(4)
{
	Init(contours);
}


cRectContour::cRectContour(const cv::Point &center, const float size) :
	m_contours(4)
{
	Init(center, size);
}

cRectContour::~cRectContour()
{

}


// 초기화.
bool cRectContour::Init(const vector<cv::Point> &contours)
{
	OrderedContours(contours, m_contours);
	return true;
}
bool cRectContour::Init(const vector<cv::Point2f> &contours)
{
	vector<cv::Point> ct(4);
	ct[0] = contours[0];
	ct[1] = contours[1];
	ct[2] = contours[2];
	ct[3] = contours[3];
	return Init(ct);
}


// 초기화.
bool cRectContour::Init(const vector<cv::KeyPoint> &keypoints)
{
	if (keypoints.size() < 4)
		return false;

	vector<Point> keys(4);
	for (u_int i = 0; i < keypoints.size(); ++i)
		keys[i] = Pt2f2Pt(keypoints[i].pt);

	OrderedContours(keys, m_contours);

	return true;
}


// 중점 center를 중심으로 size만큼 키워진 사각형을 만든다.
//
// 0 -------- 1
// |          |
// |    +     |
// |          |
// 3 -------- 2
//
bool cRectContour::Init(const cv::Point &center, const float size)
{
	m_contours[0] = center + Point(-(int)(size / 2.f), -(int)(size / 2.f));
	m_contours[1] = center + Point((int)(size / 2.f), -(int)(size / 2.f));
	m_contours[2] = center + Point((int)(size / 2.f), (int)(size / 2.f));
	m_contours[3] = center + Point(-(int)(size / 2.f), (int)(size / 2.f));
	return true;
}

void cRectContour::Normalize()
{
	vector<Point> keys(4);
	for (u_int i = 0; i < m_contours.size(); ++i)
		keys[i] = m_contours[i];
	OrderedContours(keys, m_contours);
}


// 박스 출력.
void cRectContour::Draw(cv::Mat &dst, const cv::Scalar &color, const int thickness)
// color = cv::Scalar(0, 0, 0), thickness = 1
{
	DrawLines(dst, m_contours, color, thickness);
}


// 사각형의 중점을 리턴한다.
Point cRectContour::Center() const
{
	Point center;
	for (auto &pt : m_contours)
		center += pt;
	
	center = Point(center.x / m_contours.size(), center.y / m_contours.size());
	return center;
}


// 사각형의 중점을 중심으로 스케일한다.
void cRectContour::ScaleCenter(const float scale)
{
	const Point center = Center();

	for (u_int i=0; i < m_contours.size(); ++i)
	{
		m_contours[i] = center + ((m_contours[i] - center) * scale);
	}
}


// 가로 세로 각각 스케일링 한다.
// 0 -------- 1
// |          |
// |    +     |
// |          |
// 3 -------- 2
void cRectContour::Scale(const float vscale, const float hscale)
{

	// 가로 스케일링
	vector<cv::Point> tmp1(4);
	tmp1[0] = m_contours[1] + (m_contours[0] - m_contours[1]) * hscale;
	tmp1[1] = m_contours[0] + (m_contours[1] - m_contours[0]) * hscale;
	tmp1[2] = m_contours[3] + (m_contours[2] - m_contours[3]) * hscale;
	tmp1[3] = m_contours[2] + (m_contours[3] - m_contours[2]) * hscale;

	// 세로 스케일링
	vector<cv::Point> tmp2(4);
	tmp2[0] = m_contours[3] + (m_contours[0] - m_contours[3]) * vscale;
	tmp2[3] = m_contours[0] + (m_contours[3] - m_contours[0]) * vscale;
	tmp2[1] = m_contours[2] + (m_contours[1] - m_contours[2]) * vscale;
	tmp2[2] = m_contours[1] + (m_contours[2] - m_contours[1]) * vscale;

	m_contours[0] = (tmp1[0] + tmp2[0]) * 0.5f;
	m_contours[1] = (tmp1[1] + tmp2[1]) * 0.5f;
	m_contours[2] = (tmp1[2] + tmp2[2]) * 0.5f;
	m_contours[3] = (tmp1[3] + tmp2[3]) * 0.5f;
}


// index 번째 포인터를 리턴한다.
Point cRectContour::At(const int index) const
{
	return m_contours[index];
}


int cRectContour::Width() const
{
	return (int)(((m_contours[1].x - m_contours[0].x) + 
		(m_contours[2].x - m_contours[3].x)) * 0.5f);
}


int cRectContour::Height() const
{
	return (int)abs(((m_contours[3].y - m_contours[0].y) + 
		(m_contours[2].y - m_contours[1].y)) * 0.5f);
}


cRectContour& cRectContour::operator= (const cRectContour &rhs)
{
	if (this != &rhs)
	{
		m_contours = rhs.m_contours;
	}

	return *this;
}
