//
// 화면상에 있는 점을 찾아 인식한다.
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
		int m_skewWidth;	// SkewRect 의 폭
		int m_skewHeight;	// SkewRect 의 높이
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
		cv::Mat m_srcClone;	// 원본 영상의 복사본 (쓰레드에서 처리할 원본 이미지)
		bool m_isBusy;	// 쓰레드에서 연산을 하는 중이라면 true, 쓰레드에서 값을 설정
		bool m_isUpdatePos; // position 값을 업데이트 했다면 true, 쓰레드에서 값을 설정.
		bool m_isUpdateParam; // Searpoint 연산 요청이 들어오면 true
	};
}
