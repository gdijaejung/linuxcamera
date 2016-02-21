//
// 영상을 윈도우에 출력한다.
// 쓰레드로 작동한다.
//
#pragma once


class cMovie
{
public:
	cMovie();
	virtual ~cMovie();

	bool Init(const string &name, const int width, const int height, const bool isThreadMode = false, const bool isFullScreen = false);
	void Draw(const cv::Mat &image);


public:
	string m_name;
	bool m_isThreadMode;
	int m_width;
	int m_height;
	cv::Mat m_image;

	pthread_t m_handle;
	bool m_threadLoop;
	pthread_mutex_t m_criticalSection;
};

