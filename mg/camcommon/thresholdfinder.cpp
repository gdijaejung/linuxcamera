
#include "../stdafx.h"
#include "thresholdfinder.h"


using namespace cv;
using namespace cvproc;


cThresholdFinder::cThresholdFinder()
	: m_state(CALIB_DOWN)
	, m_searchPoint(NULL)
	, m_lpf(0.8f)
	, m_detectPoint(false)
{

}

cThresholdFinder::~cThresholdFinder()
{

}


void cThresholdFinder::Init(cSearchPoint *searchPoint)
{
	m_state = CALIB_DOWN;
	m_searchPoint = searchPoint;
	m_oldThresholdMax = 0;
	m_thresholdMax = 0;
	m_thresholdVariance = 0;
	m_varianceCount = 0;
	m_thresholdValue = 255;
	m_startCalibrationTime = GetTickCount();
}


// IR LED�� �ν� �� ������ threshold�� �ٲ۴�.
// IR LED�� �νĵǸ�, �ν��� �ȵ� ������ �����Ѵ�. �̷��� �ϸ鼭
// min, max threshold�� ���ϰ�, ������ threshold�� ���Ѵ�.
bool cThresholdFinder::Update(const cv::Mat &src)
{
	RETV(!m_searchPoint, false);

	switch (m_state)
	{
	case CALIB_UP:
	{
		++m_thresholdValue;
		if (m_thresholdValue > 255)
		{
			m_thresholdValue = 255;
			m_state = CALIB_DOWN;
		}
	}
	break;

	case CALIB_DOWN:
	{
		--m_thresholdValue;
		if (m_thresholdValue < 0)
		{
			m_thresholdValue = 0;
			m_state = CALIB_UP;
		}
	}
	break;

	case CALIB_COMPLETE:
		break;
	}

	Mat grayscaleMat = src.clone();
	cvtColor(src, grayscaleMat, CV_RGB2GRAY); // �÷��� ������� ��ȯ
	if (m_binaryImg.empty())
		m_binaryImg.create(grayscaleMat.size(), grayscaleMat.type());
	threshold(grayscaleMat, m_binaryImg, m_thresholdValue, 255, cv::THRESH_BINARY_INV);

	Point pos;
	m_detectPoint = false;
	if (m_searchPoint->RecognitionSearch(src, pos, false))
	{
		m_detectPoint = true;

		switch (m_state)
		{
		case CALIB_UP:
		case CALIB_DOWN:
		{
			// threshold�� ���� ���缭 �����.
			const int threshold = m_thresholdValue - m_searchPoint->m_recogConfig.m_attr.thresholdFinderOffset;
			if (m_thresholdMax == 0)
				m_thresholdMax = (float)(threshold);
			else
				m_thresholdMax = m_lpf*m_thresholdMax + ((1 - m_lpf)*threshold);

			// ǥ�� ������ ���Ѵ�.
			const float var = sqrt((m_thresholdMax - (float)threshold) * (m_thresholdMax - (float)threshold));
			m_thresholdVariance = m_lpf*m_thresholdVariance + ((1 - m_lpf)*var);

			// threshold ���� �������̶�� �ø��극�̼��� �����Ѵ�.
			if (m_thresholdVariance < 1.1f)
			{
				++m_varianceCount;

				if (m_varianceCount > 100)
				{
					// ǥ�������� 1�� �����ٸ� ����
					m_thresholdValue = (int)m_thresholdMax;
					m_state = CALIB_COMPLETE;
				}
			}
			else
			{
				m_varianceCount = 0;
			}

			if (m_state != CALIB_COMPLETE)
			{
				m_oldThresholdMax = m_thresholdMax;
				m_state = CALIB_DOWN;
				m_thresholdValue = 255;
			}
		}
		break;

		case CALIB_COMPLETE:
			break;
		}
	}

	if (m_state == CALIB_COMPLETE)
		return true;

	return false;
}

