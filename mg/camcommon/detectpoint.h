//
// ��ũ������ ������ ����Ʈ�� ã��, ��ġ�� ����Ѵ�.
//
// ��ũ���� ��� ������ ������ ������ ����Ʈ�� �ִٰ� �����Ѵ�.
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
	bool m_makeKeypointImage; // m_binWithKeypoints�� ����ȭ�� �̹����� keypoint�� ����Ѵ�.
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
