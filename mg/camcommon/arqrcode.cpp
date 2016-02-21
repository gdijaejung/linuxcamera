
#include "stdafx.h"
#include "arqrcode.h"


using namespace cvproc;


cArQRCode::cArQRCode() :
	m_dbgShow(false)
	, m_skewDetect(500, 500)
{
}

cArQRCode::~cArQRCode()
{
}


// Ŭ���� �ʱ�ȭ
bool cArQRCode::Init(const string &recogFileName)
{
	if (!UpdateConfig(recogFileName))
		return false;
	
	return true;
}


// ȯ�漳�� ���� ������Ʈ
bool cArQRCode::UpdateConfig(const string &recogFileName)
{
	if (!m_recogCfg.Read(recogFileName))
		return false;

	m_detectEdge.UpdateParam(m_recogCfg);
	return true;
}


// QR �ڵ� �ν�
bool cArQRCode::Detect(const Mat &src)
{
 	m_detectEdge.m_show = m_dbgShow;

	if (m_detectEdge.Detect(src))
	{
		m_skewDetect.Init(m_detectEdge.m_rect, 1, 500, 500);

		Mat matSkew = m_skewDetect.Transform(src);
		imshow("skew window", matSkew);
		return true;
	}

	return false;
}
