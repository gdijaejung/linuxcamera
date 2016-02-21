//
// 2016-01-21, jjuiddong
//
// �������ǿ��� ���Ǵ� QRCode �ν� Ŭ����
//
//  * ------------ *
//  |              |
//  |              |
//  |              |
//  |           *  |
//  * ------------ *
//
#pragma once

#include "detectrect.h"


namespace cvproc
{

	class cArQRCode
	{
	public:
		cArQRCode();
		virtual ~cArQRCode();

		bool Init(const string &recogFileName);
		bool Detect(const Mat &src);
		bool UpdateConfig(const string &recogFileName);


	public:
		bool m_dbgShow;
		cDetectRect m_detectEdge;
		cRecognitionEdgeConfig m_recogCfg;
		cSkewDetect m_skewDetect;
	};

}
