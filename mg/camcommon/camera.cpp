
#include "../stdafx.h"
#include "camera.h"

cvproc::cCamera g_camera;

using namespace cv;
using namespace cvproc;

void* CameraThreadFunction(void* arg);


cCamera::cCamera()
	: m_show(false)
	,m_showFrame(true)
	,m_capture(NULL)
	,m_resolution(640, 480)
	, m_lastMovieTime(0)
	,m_oldTime(0)
	, m_fps(0)
	, m_handle(0)
	, m_threadLoop(true)
{
	// 카메라 렌즈 왜곡 보정 파라메터
	const double cx = 346.982762f;
	const double cy = 229.626156f;
	const double fx = 676.748756f;
	const double fy = 677.032333f;
	const double k1 = -0.331818f;
	const double k2 = -0.033314f;
	const double k3 = 0.f;
	const double p1 = -0.000313f;
	const double p2 = 0.000841f;
	const double skew_c = 0;
	m_camDistort.SetParams(cx, cy, fx, fy, k1, k2, k3, p1, p2, skew_c);

	pthread_mutex_init(&m_CriticalSection, NULL);
}

cCamera::~cCamera()
{
	m_threadLoop = false;
	if (m_handle)
	{
		pthread_join(m_handle, NULL);
		m_handle = 0;
	}

	cvDestroyWindow("camera");
	cvReleaseCapture(&m_capture);

	pthread_mutex_destroy(&m_CriticalSection);
}


// 카메라 초기화.
bool cCamera::Init(const int index, const cv::Size &resolution, const bool isThread ) 
// index=0, resolution = Size(640, 480), isThread = true
{
	const double cx = g_config.m_cx;
	const double cy = g_config.m_cy;
	const double fx = g_config.m_fx;
	const double fy = g_config.m_fy;
	const double k1 = g_config.m_k1;
	const double k2 = g_config.m_k2;
	const double k3 = g_config.m_k3;
	const double p1 = g_config.m_p1;
	const double p2 = g_config.m_p2;
	const double skew_c = g_config.m_skew_c;
	m_camDistort.SetParams(cx, cy, fx, fy, k1, k2, k3, p1, p2, skew_c);

	m_capture = cvCaptureFromCAM(index);
	if (!m_capture)
	{
		std::cout << " cCamera::Init Error!!" << std::endl;
		return false;
	}

	m_resolution = resolution;
	m_show = g_config.m_conf.showCamera;
	m_showFrame = g_config.m_conf.showCameraFrame;

	// if  thread is running, finish
	if (m_handle)
	{
		m_threadLoop = false;
		pthread_join(m_handle, NULL);
		m_handle = 0;
	}

	m_threadLoop = true;
	if (isThread)
	{
		pthread_create(&m_handle, NULL, CameraThreadFunction, this);
	}

	if (m_show)
	{
		cvNamedWindow("camera", 0);
		cvResizeWindow("camera", g_config.m_cameraRect.width, g_config.m_cameraRect.height);
		cvMoveWindow("camera", g_config.m_cameraRect.x, g_config.m_cameraRect.y);
	}

	std::cout << "Init Camera index= " << index << std::endl;
	std::cout << "\t- Resolution width= " << resolution.width << ", height= " << resolution.height << std::endl << std::endl;

	cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_WIDTH, resolution.width);
	cvSetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_HEIGHT, resolution.height);
//	cvSetCaptureProperty(m_capture, CV_CAP_PROP_POS_FRAMES, 90);
// 	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
// 	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
// 	cap.set(CV_CAP_PROP_FPS, 90);
// 	cap.set(CV_CAP_PROP_CONVERT_RGB, false);
	//cvSetCaptureProperty(m_capture, CV_CAP_PROP_POS_FRAMES, 60);
	//cvSetCaptureProperty(m_capture, CV_CAP_PROP_EXPOSURE, -6);
// 	std::cout << "Frame Width = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_WIDTH) << std::endl;
// 	std::cout << "Frame Height = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
// 	std::cout << "Frame FPS = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_FPS) << std::endl;
// 	std::cout << "Exposure = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_EXPOSURE) << std::endl;
// 	std::cout << "Format = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_FORMAT) << std::endl;
// 	std::cout << "Contrast = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_CONTRAST) << std::endl;
// 	std::cout << "Brightness = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_BRIGHTNESS) << std::endl;
// 	std::cout << "Saturation = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_SATURATION) << std::endl;
// 	std::cout << "Hue = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_HUE) << std::endl;
// 	std::cout << "Frames = " << cvGetCaptureProperty(m_capture, CV_CAP_PROP_POS_FRAMES) << std::endl;
// 	std::cout << std::endl;

	return true;
}


// 카메라 초기화.
bool cCamera::Init(const string &fileName, const cv::Size &resolution) // index=0, resolution = Size(640, 480)
{
	m_capture = cvCreateFileCapture(fileName.c_str());
	if (!m_capture)
		return false;

	m_movieFileName = fileName;
	m_resolution = resolution;
	m_show = g_config.m_conf.showCamera;
	m_showFrame = g_config.m_conf.showCameraFrame;

	if (m_show)
	{
		cvNamedWindow("camera", 0);
		cvResizeWindow("camera", g_config.m_cameraRect.width, g_config.m_cameraRect.height);
		cvMoveWindow("camera", g_config.m_cameraRect.x, g_config.m_cameraRect.y);
	}

	return true;
}


// 카메라가 인식한 영상을 가져온다.
Mat& cCamera::GetCapture()
{
	if (!m_capture)
		return m_camImage;

	if (m_handle)
	{
		// already copy camera image in thread function
	}
	else
	{
		IplImage *src = cvQueryFrame(m_capture);
		if (m_camImage.empty())
		{
			m_camImage = Mat(src).clone();
		}
		else
		{
			memcpy(m_camImage.data, src->imageData, m_camImage.step*m_camImage.rows);
		}
	}

	return m_camImage;
}


Mat& cCamera::GetCaptureThreadSafe()
{
	if (!m_capture)
		return m_camImage;

	cAutoCS cs(m_CriticalSection);

//	std::cout << "GetCaptureThreadSafe() cvQueryFrame()" << std::endl;

	cvGrabFrame(m_capture);
	IplImage *src = cvRetrieveFrame(m_capture);
//	IplImage *src = cvQueryFrame(m_capture);

	if (!src)
		return m_camImage;

//	std::cout << "GetCaptureThreadSafe() in" << std::endl;

	if (m_camImage.empty())
	{
//		std::cout << "empty" << std::endl;
		m_camImage = Mat(src).clone();
	}
	else
	{
//		std::cout << "copy" << std::endl;
		memcpy(m_camImage.data, src->imageData, m_camImage.step*m_camImage.rows);
	}
	
	return m_camImage;
}


Mat& cCamera::GetMovieCapture()
{
	if (!m_capture)
		return m_movieImage;

	const static int elapseT = 1000 / 15;
	const int curT = GetTickCount();
	if (m_lastMovieTime == 0)
		m_lastMovieTime = curT;
	if ((curT - m_lastMovieTime) > elapseT)
	{
// 		if (m_movieImage.empty())
// 		{
// 			m_movieImage.create(m_resolution, CV_8UC3);
// 		}

		m_lastMovieTime = curT;
		IplImage *img = cvQueryFrame(m_capture);
		if (!img)
		{
			if (!m_movieFileName.empty())
			{
				cvReleaseCapture(&m_capture);
				m_capture = cvCreateFileCapture(m_movieFileName.c_str());
 				img = cvQueryFrame(m_capture);
			}
		}

		m_movieImage = Mat(img);
	}

	return m_movieImage;
}


// 카메라 렌즈에 의한 왜곡되지 않는 영상을 리턴한다.
Mat& cCamera::GetCaptureUndistortion()
{
	IplImage *image = cvQueryFrame(m_capture);
	if (!image)
		return m_undistortCapture;

	Mat src(image);	
	if (m_undistortCapture.empty())
		m_undistortCapture = src.clone();

	m_camDistort.UndistortImage(src, m_undistortCapture);

	return m_undistortCapture;
}

cv::Mat& cCamera::GetCaptureUndistortion(cv::Mat &src)
{
	if (m_undistortCapture.empty())
		m_undistortCapture = src.clone();

	m_camDistort.UndistortImage(src, m_undistortCapture);

	return m_undistortCapture;
}


// 카메라가 촬영한 영상을 윈도우에 출력한다.
cv::Mat& cCamera::ShowCapture()
{
	cv::Mat& image = GetCapture();

	if (m_show && !image.empty())
		imshow("camera", image);

	return image;
}


// 카메라가 촬영한 영상을 보정하여 윈도우에 출력한다.
Mat& cCamera::ShowCaptureUndistortion()
{
	if (m_showFrame)
	{
		const int curT = GetTickCount();
		const int deltaT = curT - m_oldTime;
		m_incTime += deltaT;
		m_fps++;
		if (m_incTime > 1000)
		{
			printf("fps : %d\n", m_fps);
			m_incTime = 0;
			m_fps = 0;
		}
		m_oldTime = curT;
	}


	Mat &image = GetCaptureUndistortion();
	if (image.empty())
		return image;

	if (m_show)
		imshow("camera", image);
	return image;
}


// 카메라가 촬영한 영상을 보정하여 윈도우에 출력한다.
// 인자로 넘어온 skewTransform 값으로 특정 영역의 영상을 복사해 dst에 저장한다.
cv::Mat& cCamera::ShowCaptureUndistortion(const cv::Mat &skewTransform, OUT cv::Mat &dst)
{
	Mat &image = GetCaptureUndistortion();
	if (image.empty())
		return image;

	cv::warpPerspective(image, dst, skewTransform, dst.size());

	if (m_show)
		imshow("camera", image);
	return image;
}


// UDP 서버 쓰레드
void* CameraThreadFunction(void* arg)
{
	cCamera *camera = (cCamera*)arg;

	while (camera->m_threadLoop)
	{
		camera->GetCaptureThreadSafe();

		common::sleep_ms(20);
	}

	return 0;
}

