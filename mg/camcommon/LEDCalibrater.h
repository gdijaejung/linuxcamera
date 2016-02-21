//
// 2015-12-05, jjuiddong
//
// �������� ������ 3���� LED�� �ν���, �ִ�,�ּ� �Ÿ�, ����, ũ�� ����
// �����, ������ �Ķ���͸� ã�Ƽ� �����Ѵ�.
//
#pragma once


namespace cvproc
{

	class cLEDCalibrater
	{
	public:
		cLEDCalibrater();
		virtual ~cLEDCalibrater();

		bool Init(cSearchPoint *searchPoint, const int calibrateSeconds);
		bool Update(const cv::Mat &src);


		cSearchPoint *m_searchPoint;
		cRecognitionConfig m_tmpRecogConfig;

		int m_calibrateSeconds;
		int m_startCalibrateTime;

		float m_minLineLength;
		float m_maxLineLength;
		float m_maxLineDot;
		float m_maxHorizontalDot;
	};

}
