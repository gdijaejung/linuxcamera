
#include "../stdafx.h"
#include "movie.h"


using namespace cv;


void* MovieThreadFunction(void* arg);


cMovie::cMovie() 
	: m_isThreadMode(false)
	, m_width(0)
	, m_height(0)
	, m_threadLoop(true)
{
	pthread_mutex_init(&m_criticalSection, NULL);
}

cMovie::~cMovie()
{
	m_threadLoop = false;
	if (m_handle)
	{
		pthread_join(m_handle, NULL);
		m_handle = 0;
	}

	pthread_mutex_destroy(&m_criticalSection);

	cvDestroyWindow(m_name.c_str());
}


// 클래스 초기화
bool cMovie::Init(const string &name, const int width, const int height,
	const bool isThreadMode, const bool isFullScreen)
	// isThread = false, isFullScreen = false
{
	m_name = name;
	m_width = width;
	m_height = height;
	m_isThreadMode = isThreadMode;

	cvNamedWindow(name.c_str(), 0);
	cvResizeWindow(name.c_str(), width, height);
	if (isFullScreen)
		cvSetWindowProperty(name.c_str(), CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	if (isThreadMode)
	{
		//m_handle = (HANDLE)_beginthreadex(NULL, 0, MovieThreadFunction, this, 0, (unsigned*)&m_threadId);
		pthread_create(&m_handle, NULL, MovieThreadFunction, this);
	}

	return true;
}


// 출력할 영상을 복사한다.
void cMovie::Draw(const cv::Mat &image)
{
	cAutoCS cs(m_criticalSection);
	m_image = image.clone();
}


// DxShow 쓰레드
void* MovieThreadFunction(void* arg)
{
	cMovie *movie = (cMovie*)arg;

	while (movie->m_threadLoop)
	{
		cAutoCS cs(movie->m_criticalSection);
		imshow(movie->m_name.c_str(), movie->m_image);

		cvWaitKey(1);
	}

	return 0;
}
