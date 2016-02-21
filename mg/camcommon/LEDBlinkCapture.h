//
// 2015-12-07, jjuiddong
//
// LED�� �����Ÿ����ؼ�, ���� ������ ��, �ֺ������� ��ġ�� ������,
// ���� ������ ��, �ֺ������� �����ϴ� ����� �Ѵ�.
//
#pragma once


namespace cvproc
{
	
	class cLEDBlinkCapture
	{
	public:
		cLEDBlinkCapture();
		virtual ~cLEDBlinkCapture();

		bool Init(const int portNum, const int baudRate, const int delayMilliseconds);
		void Update(const cv::Mat &src);
		void LEDOn();
		void LEDOff();


	protected:
		void DetectIgnorePoint(const cv::Mat &src);
		

	public:
		CSerial m_serial;
		int m_delayMilliseconds;
		int m_oldT;
		bool m_chCapture;
		bool m_chToggle;
		bool m_toggle;

		cv::Mat m_ledOn;
		cv::Mat m_ledOff;
	};

}
