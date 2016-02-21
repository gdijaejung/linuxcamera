//
// 2015-12-05
// 
// ������ ����ȭ �� ��, ����ϴ� threshold ���� �ڵ����� ã���ִ� ������ �Ѵ�.
// 
// ���ܼ� LED 3���� �ν��� �� �ִ� threshold �� �߿�, ���� ū ���� ã�´�.
//
#pragma once


namespace cvproc
{

	class cThresholdFinder
	{
	public:
		cThresholdFinder();
		virtual ~cThresholdFinder();

		void Init(cSearchPoint *searchPoint);
		bool Update(const cv::Mat &src);


		// threshold calibration
		enum THRESHOLD_CALIBRATION_STATE
		{
			CALIB_UP,
			CALIB_DOWN,
			CALIB_COMPLETE,
		};

		THRESHOLD_CALIBRATION_STATE m_state;
		cSearchPoint *m_searchPoint; // reference
		cv::Mat m_binaryImg;
		int m_thresholdValue;
		int m_startCalibrationTime; // �ø��극�̼��� ������ ���� �ð� ����
		float m_thresholdMax; // IR LED�� �ν��� �� �ִ� �ִ� thresold ���� ���
		float m_oldThresholdMax;
		float m_thresholdVariance; // threshold ��ȭ ������ �����Ѵ�.
		int m_varianceCount;
		float m_lpf; // low pass filter constant
		bool m_detectPoint; // LED�� �߰ߵǸ� true�� �ȴ�.
	};

}
