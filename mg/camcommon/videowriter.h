//
// 2015-12-29, jjuiddong
//
// ������ ��ȭ�ϴ� ����� �Ѵ�. 
// �����ӿ� ���缭 ��ȭ �Ѵ�. �⺻������ 30���������� �����Ѵ�.
//
#pragma once


namespace cvproc
{

	class cVideoWriter
	{
	public:
		cVideoWriter();
		virtual ~cVideoWriter();

		bool Init(const string &fileName, const int width=640, const int height=480);
		void Record(const cv::Mat &img);
		void Close();

		
		enum {SAVE_FPS=15};
		CvVideoWriter *m_videoWriter;
		int m_lastRecordTime;
	};

}
