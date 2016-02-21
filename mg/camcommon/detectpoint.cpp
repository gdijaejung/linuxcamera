
#include "../stdafx.h"
#include "detectpoint.h"
#include "config.h"


using namespace cv;

cDetectPoint::cDetectPoint(const string &windowName) 
	: m_show(false)
	, m_makeKeypointImage(false)
	, m_windowName(windowName)
	, m_pos(0, 0)
{
	m_show = g_config.m_conf.showDetectPoint;

	if (m_show)
	{
		namedWindow(m_windowName, 0);
		cvResizeWindow(m_windowName.c_str(), g_config.m_detectPointBinaryScreenRect.width, g_config.m_detectPointBinaryScreenRect.height);
		cvMoveWindow(m_windowName.c_str(), g_config.m_detectPointBinaryScreenRect.x, g_config.m_detectPointBinaryScreenRect.y);
	}
}


cDetectPoint::cDetectPoint(const cRecognitionConfig &recogConfig, const string &windowName) 
	: m_show(false)
	, m_windowName(windowName)
	, m_pos(0,0)
{
	m_show = g_config.m_conf.showDetectPoint;

	if (m_show)
	{
		namedWindow(m_windowName, 0);
		cvResizeWindow(m_windowName.c_str(), g_config.m_detectPointBinaryScreenRect.width, g_config.m_detectPointBinaryScreenRect.height);
		cvMoveWindow(m_windowName.c_str(), g_config.m_detectPointBinaryScreenRect.x, g_config.m_detectPointBinaryScreenRect.y);
	}

	UpdateParam(recogConfig);
}

cDetectPoint::~cDetectPoint()
{
	cvDestroyWindow(m_windowName.c_str());
}


// �Ķ���������� ȯ�漳�� ���Ϸκ��� ������Ʈ �Ѵ�.
void cDetectPoint::UpdateParam(const cRecognitionConfig &recogConfig)
{
	m_show = g_config.m_conf.showDetectPoint;
	m_printDetectPoint = recogConfig.m_attr.printDetectPoint;

	//	SimpleBlobDetector::Params params;
	//	ZeroMemory(&m_params, sizeof(m_params));

	m_params.thresholdStep = 10;
	m_params.minRepeatability = 0;
	m_params.minDistBetweenBlobs = 10;

	// Change thresholds
	m_params.minThreshold = 1;
	m_params.maxThreshold = 20;

	// Filter by Area.
	m_params.filterByArea = true;
	m_params.minArea = recogConfig.m_attr.detectPointMinArea;
	m_params.maxArea = recogConfig.m_attr.detectPointMaxArea;

	// Filter by Circularity
	m_params.filterByCircularity = true;
	m_params.minCircularity = recogConfig.m_attr.detectPointCircularity;
	m_params.maxCircularity = 1;

	// Filter by Convexity
	m_params.filterByConvexity = true;
	m_params.minConvexity = recogConfig.m_attr.detectPointMinConvexity;
	m_params.maxConvexity = 1;

	// Filter by Inertia
	m_params.filterByInertia = true;
	m_params.minInertiaRatio = recogConfig.m_attr.detectPointMinInertia;
	m_params.maxInertiaRatio = 1.f;

	m_thresholdValue = recogConfig.m_attr.detectPointThreshold;
	//m_params = params;
}


// src �̹����󿡼� ���� ã�´�.
// ����Ʈ�� ã�Ҵٸ� �ش� ��ġ�� �����ϰ�, �׷��� �ʴٸ�, ���� ��ġ�� �����Ѵ�.
bool cDetectPoint::DetectPoint(const Mat &src, Point &out, bool isCvtGray)
{
	Mat grayscaleMat = src.clone();
	if (isCvtGray)
		cvtColor(src, grayscaleMat, CV_RGB2GRAY); // �÷��� ������� ��ȯ

	if (m_binMat.empty())
		m_binMat.create(grayscaleMat.size(), grayscaleMat.type());
	threshold(grayscaleMat, m_binMat, m_thresholdValue, 255, cv::THRESH_BINARY_INV);

	//----------------------------------------------------------------------------------------------
	// ��ũ�� �̹������� �����͸� ã�´�.
	cv::SimpleBlobDetector detector(m_params);

	// Detect blobs.
	std::vector<KeyPoint> keypoints;
	detector.detect(m_binMat, keypoints);

	// ����Ʈ�� ã�Ҵٸ� �ش� ��ġ�� �����ϰ�, �׷��� �ʴٸ�, ���� ��ġ�� �����Ѵ�.
	Point pos = (keypoints.empty()) ? m_pos : Pt2f2Pt(keypoints[0].pt);

	if (keypoints.size() >= 2)
		pos = keypoints[1].pt;

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	if ((m_show || m_makeKeypointImage) && !keypoints.empty())
	{
		drawKeypoints(m_binMat, keypoints, m_binWithKeypoints, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		if (m_show)
			imshow(m_windowName, m_binWithKeypoints);
	}

	if (keypoints.empty())
	{
		m_points.clear();
	}
	else
	{
		m_points.resize(keypoints.size());
		for (u_int i = 0; i < keypoints.size(); ++i)
			m_points[i] = keypoints[ i].pt;
	}

	m_pos = pos;
	out = pos;

	if (!keypoints.empty() && m_printDetectPoint)
		printf("pt.x = %d, pt.y = %d \n", pos.x, pos.y);

	return true;
}
