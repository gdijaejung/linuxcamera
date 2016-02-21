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

		bool Init(const string &recognitionConfigFile, const string &contourConfigFile, const string &roiFile);
		bool InitSkewRect(const cRectContour &contour, const float scale);
		bool ReadContourFile(const string &contourConfigFile);
		bool RecognitionSearch(const cv::Mat &src, OUT cv::Point &rawPos, const bool isSkewTransform=true, const bool isRoi=false);
		void ConvertUV(const cv::Point &pos, OUT float &outX, OUT float &outY);


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
	};
}
