//
// 스크린에서 레이저 포인트를 찾아, 위치를 계산한다.
//
// 스크린은 흰색 바탕에 검은색 레이저 포인트만 있다고 가정한다.
//
#pragma once


class cDetectPoint
{
public:
	cDetectPoint(const string &windowName = "BinaryScreen");
	cDetectPoint(const cRecognitionConfig &recogConfig, const string &windowName = "BinaryScreen");
	virtual ~cDetectPoint();

	bool DetectPoint(const cv::Mat &src, cv::Point &out, bool isCvtGray=true);
	vector<cv::Point>& GetDetectPoints();
	void UpdateParam(const cRecognitionConfig &recogConfig);


	bool m_show;
	bool m_makeKeypointImage; // m_binWithKeypoints에 이진화된 이미지의 keypoint를 출력한다.
	bool m_printDetectPoint;
	int m_thresholdValue;
	string m_windowName;
	cv::Point m_pos;
	cv::SimpleBlobDetector::Params m_params;
	vector<cv::Point> m_points;
	cv::Mat m_binMat;
	cv::Mat m_binWithKeypoints;
};


inline vector<cv::Point>& cDetectPoint::GetDetectPoints() { return m_points; }
