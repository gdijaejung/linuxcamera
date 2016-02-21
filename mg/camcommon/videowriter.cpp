
#include "../stdafx.h"
#include "videowriter.h"

using namespace cv;
using namespace cvproc;


cVideoWriter::cVideoWriter() :
	m_videoWriter(NULL)
	, m_lastRecordTime(0)
{
}

cVideoWriter::~cVideoWriter()
{
	Close();
}


bool cVideoWriter::Init(const string &fileName, const int width, const int height)
{
	Close();
	m_videoWriter = cvCreateVideoWriter(fileName.c_str(), 0, SAVE_FPS, cvSize(width, height));

	if (!m_videoWriter)
		return false;

	return true;
}


void cVideoWriter::Record(const cv::Mat &img)
{
	RET(!m_videoWriter);

	const static int elapseT = 1000 / SAVE_FPS;

	const int curT = GetTickCount();
	if (m_lastRecordTime == 0)
		m_lastRecordTime = curT;
	if (curT - m_lastRecordTime > elapseT)
	{
		m_lastRecordTime = curT;
		const IplImage tmp(img);
		cvWriteFrame(m_videoWriter, &tmp);
	}
}


void cVideoWriter::Close()
{
	if (m_videoWriter)
	{
		cvReleaseVideoWriter(&m_videoWriter);
		m_videoWriter = NULL;
	}
}
