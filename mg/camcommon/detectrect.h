//
// 2016-01-21, jjuiddong
//
// 사각형을 찾는다.
//
#pragma once


namespace cvproc
{

	class cDetectRect
	{
	public:
		cDetectRect();
		virtual ~cDetectRect();

		bool Init();
		bool Detect(const Mat &src);
		void UpdateParam(const cRecognitionEdgeConfig &recogConfig);


	public:
		bool m_show;
		cRectContour m_rect;

		int m_threshold1;
		int m_threshold2;
		int m_minArea;
		double m_minCos;
		double m_maxCos;

		vector<vector<Point> > m_contours;

		Mat m_gray;
		Mat m_edges;
		Mat m_binMat;
	};

}
