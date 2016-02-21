//
// ī�޶� �Կ��� ������ ��������, �����쿡 ����ϴ� ����� �Ѵ�.
// ī�޶� ���� �ְ��� �����ϴ� ����� �Ѵ�.
//
#pragma once


namespace cvproc
{

	class cCamera
	{
	public:
		cCamera();
		virtual ~cCamera();

		bool Init(const int index=0, const cv::Size &resolution=cv::Size(640,480));
		bool Init(const string &fileName, const cv::Size &resolution = cv::Size(640, 480));
		IplImage* GetCapture();
		cv::Mat& GetCaptureUndistortion();
		cv::Mat& GetCaptureUndistortion(cv::Mat &src);
		cv::Mat& GetMovieCapture();
	
		IplImage* ShowCapture();
		cv::Mat& ShowCaptureUndistortion();
		cv::Mat& ShowCaptureUndistortion(const cv::Mat &skewTransform, OUT cv::Mat &dst);
		const cv::Size& Resolution() const;


	protected:
		bool m_show;
		bool m_showFrame;
		CvCapture* m_capture;
		cv::Mat m_undistortCapture;
		CameraDistortion m_camDistort;
		cv::Size m_resolution; // ī�޶� �ػ�
		cv::Mat m_movieImage;
		int m_lastMovieTime; // 30 fps ������ ���� �ð�
		string m_movieFileName;

		// fps
		int m_oldTime;
		int m_incTime;
		int m_fps;
	};

	inline const cv::Size& cCamera::Resolution() const { return m_resolution;  }
}
