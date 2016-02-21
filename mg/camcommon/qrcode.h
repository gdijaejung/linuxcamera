//
// 2016-01-19, jjuiddong
//
//   * ------------ *
//   |            /
//   |         /
//   |      /
//   |    /
//   |  /
//   |/
//   *
//
// 3���� ������ �ѷ����� QR Code�� �ν��Ѵ�.
// ���� QR Code�ʹ� �ٸ���.
//
//
#pragma once


namespace cvproc
{

	class cQRCode
	{
	public:
		cQRCode();
		virtual ~cQRCode();

		bool Init(const string &recogFileName);
		bool Detect(const Mat &src);
		bool UpdateConfig(const string &recogFileName);


	protected:
		bool CheckTriangle(const vector<cv::Point> &points, vector<cv::Point> &out);


	public:
		bool m_dbgShow;
		cDetectPoint m_detectPoint;
		cRecognitionConfig m_recogCfg;
	};

}
