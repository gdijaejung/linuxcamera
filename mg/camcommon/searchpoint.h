//
// ȭ��� �ִ� ���� ã�� �ν��Ѵ�.
//
#pragma once


namespace cvproc
{
	class cSearchPoint
	{
	public:
		cSearchPoint(const int id);
		virtual ~cSearchPoint();

		bool Init(const string &recognitionConfigFile, const string &contourConfigFile, const string &roiFile, 
			const bool isSkewTransform = true, const bool isRoi = false, const bool isThread = false);
		bool InitSkewRect(const cRectContour &contour, const float scale);
		bool Update(const cv::Mat &src, OUT cv::Point &rawPos);
		bool ReadContourFile(const string &contourConfigFile);
		bool RecognitionSearch(const cv::Mat &src, OUT cv::Point &rawPos, const bool isSkewTransform=true, const bool isRoi=false);
		void ConvertUV(const cv::Point &pos, OUT float &outX, OUT float &outY);
		bool IsBusy();


	public:
		bool m_isSkewTransform;
		bool m_isRoi;
		cRoi m_roi;
		cSkewDetect m_skewDetect;
		cDetectPoint m_detectPoint;
		cRecognitionConfig m_recogConfig;
		int m_maxDiffCount;
		int m_skewWidth;	// SkewRect �� ��
		int m_skewHeight;	// SkewRect �� ����
		cv::Point m_oldRawPos;
		cLineContour m_oldBestLine;
		bool m_show;
		string m_windowName;

		// thread
		pthread_t m_handle;
		pthread_mutex_t m_CriticalSection;
		bool m_threadLoop;
		int m_sleepMillis; // default = 10

		// sync variable
		cv::Mat m_srcClone;	// ���� ������ ���纻 (�����忡�� ó���� ���� �̹���)
		bool m_isBusy;	// �����忡�� ������ �ϴ� ���̶�� true, �����忡�� ���� ����
		bool m_isUpdatePos; // position ���� ������Ʈ �ߴٸ� true, �����忡�� ���� ����.
		bool m_isUpdateParam; // Searpoint ���� ��û�� ������ true
	};
}
