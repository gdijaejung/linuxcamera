
#include "../stdafx.h"
#include "LEDCalibrater.h"

using namespace cv;
using namespace cvproc;


cLEDCalibrater::cLEDCalibrater()
{
}

cLEDCalibrater::~cLEDCalibrater()
{
}


bool cLEDCalibrater::Init(cSearchPoint *searchPoint, const int calibrateSeconds)
{
	m_searchPoint = searchPoint;
	m_tmpRecogConfig = searchPoint->m_recogConfig; // ���� ������ ���� ������ ���´�.

	m_calibrateSeconds = calibrateSeconds;
	m_startCalibrateTime = GetTickCount();

	m_minLineLength = FLT_MAX;
	m_maxLineLength = -FLT_MAX;
	m_maxLineDot = -FLT_MAX;
	m_maxHorizontalDot = -FLT_MAX;


	// �������� ū �������� LED�� �ν��ϰ� �ϰ�, ���� �ٿ�����, ������ ���� ã�´�.
// 	searchPoint->m_recogConfig.m_detectPointThreshold;
// 	searchPoint->m_recogConfig.m_detectPointMinArea;
// 	searchPoint->m_recogConfig.m_detectPointMaxArea;
// 	searchPoint->m_recogConfig.m_detectPointCircularity;
// 	searchPoint->m_recogConfig.m_detectPointMinConvexity;
// 	searchPoint->m_recogConfig.m_detectPointMinInertia;
// 	searchPoint->m_recogConfig.m_printDetectPoint;
// 	searchPoint->m_recogConfig.m_detectPointLoopCount;

	searchPoint->m_recogConfig.m_attr.detectPointMinLineDist = 10;
	searchPoint->m_recogConfig.m_attr.detectPointMaxLineDist = 500;

	searchPoint->m_recogConfig.m_attr.detectLineMaxDot = 0.5f;
	searchPoint->m_recogConfig.m_attr.detectLineMinAngle = 0.5f;
	searchPoint->m_recogConfig.m_attr.detectLineMinOldAngle = 0.5f;
	searchPoint->m_recogConfig.m_attr.detectLineMinDiffDistance = 1000;

	return true;
}


// 3���� IR LED�� �ν��� ��, �Ӽ����� �����Ѵ�.
bool cLEDCalibrater::Update(const cv::Mat &src)
{
	RETV(!m_searchPoint, false);

	if (((int)GetTickCount() - m_startCalibrateTime) > m_calibrateSeconds)
	{
		// �ø��극�̼� ����
		return true;
	}

	Point pos;
	if (m_searchPoint->RecognitionSearch(src, pos, false))
	{
		Point v1 = m_searchPoint->m_oldBestLine.m_contours[0] - m_searchPoint->m_oldBestLine.m_contours[1];
		Point v2 = m_searchPoint->m_oldBestLine.m_contours[1] - m_searchPoint->m_oldBestLine.m_contours[2];
		Point v3 = m_searchPoint->m_oldBestLine.m_contours[0] - m_searchPoint->m_oldBestLine.m_contours[2];

		const float len1 = (float)sqrt(v1.x*v1.x + v1.y*v1.y);
		const float len2 = (float)sqrt(v2.x*v2.x + v2.y*v2.y);

		m_minLineLength = min(len1, m_minLineLength);
		m_minLineLength = min(len2, m_minLineLength);
		m_maxLineLength = min(len1, m_maxLineLength);
		m_maxLineLength = min(len2, m_maxLineLength);

		const Point2f n1 = PointNormalize(v1);
		const Point2f n2 = PointNormalize(v2);
		const Point2f n3 = PointNormalize(v3);

		const float dt = n1.dot(n2);
		m_maxLineDot = MAX(abs(dt - 1.f), m_maxLineDot);

		const float dot1 = n3.dot(Point2f(1, 0));
		m_maxHorizontalDot = MAX(abs(dot1), m_maxHorizontalDot);	
	}

	return false;
}
