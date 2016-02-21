
#include "../stdafx.h"
#include "CameraDistortion.h"
#include "use_opencv.h"

using namespace cv;


CameraDistortion::CameraDistortion(void)
{
	cx=cy=fx=fy=k1=k2=k3=p1=p2=skew_c=0;
}

CameraDistortion::~CameraDistortion(void)
{
}

void CameraDistortion::UndistortImage(const Mat& frame, Mat& frame_undistort)
{
	int w = frame.cols;
	int h = frame.rows;
	//int c = frame.channels();

	TypedMat<unsigned char> md = frame;
	TypedMat<unsigned char> mu = frame_undistort;

	if(frame.channels()>1)
	{
		for(int y=0; y<h; y++)
		{
			unsigned char * dst = mu.GetPtr(y);
			for(int x=0; x<w; x++)
			{
				int px = x;
				int py = y;
				DistortPixel(px, py);

				if(px>=0 && py>=0 && px<w && py<h)
				{
					unsigned char * src = &md(py,px,0);

					*dst++ = src[0];
					*dst++ = src[1];
					*dst++ = src[2];
				}
				else
				{
					*dst++ = 0;
					*dst++ = 0;
					*dst++ = 0;
				}
			}
		}
	}
	else
	{
		for(int y=0; y<h; y++)
		{
			for(int x=0; x<w; x++)
			{
				int px = x;
				int py = y;
				DistortPixel(px, py);
				if(px>=0 && py>=0 && px<w && py<h) mu[y][x] = md[py][px];
				else mu[y][x] = 0;
			}
		}
	}
}

void CameraDistortion::SetParams(double _cx, double _cy, double _fx, double _fy, double _k1, double _k2, double _k3, double _p1, double _p2, double _skew_c)
{
	cx = _cx;
	cy = _cy;
	fx = _fx;
	fy = _fy;
	k1 = _k1;
	k2 = _k2;
	k3 = _k3;
	p1 = _p1;
	p2 = _p2;
	skew_c = _skew_c;
}

void CameraDistortion::Normalize(double &x, double& y)
{
	double y_n = (y-cy)/fy;
	double x_n = (x-cx)/fx - skew_c*y_n;

	x = x_n;
	y = y_n;
}

void CameraDistortion::Denormalize(double &x, double& y)
{
	double x_p = fx*(x + skew_c*y) + cx;
	double y_p = fy*y + cy;

	x = x_p;
	y = y_p;
}

void CameraDistortion::Distort(double& x, double& y)
{
	double r2 = x*x + y*y;
	double radial_d = 1 + k1*r2 + k2*r2*r2 + k3*r2*r2*r2;
	double x_d = radial_d*x + 2*p1*x*y + p2*(r2 + 2*x*x);
	double y_d = radial_d*y + p1*(r2 + 2*y*y) + 2*p2*x*y;

	x = x_d;
	y = y_d;
}

void CameraDistortion::DistortPixel(int& px, int& py)
{
	double x = px;
	double y = py;

	Normalize(x, y);
	Distort(x, y);
	Denormalize(x, y);

	px = (int)(x + 0.5);
	py = (int)(y + 0.5);
}

void CameraDistortion::UndistortPixel(int& px, int& py)
{
	double xd = px;
	double yd = py;

	Normalize(xd, yd);
	double xu = xd;
	double yu = yd;

	double err_thr = (0.1/fx)*(0.1/fx) + (0.1/fy)*(0.1/fy);

	while(1)
	{
		double xud = xu;
		double yud = yu;
		Distort(xud, yud);

		double err_x = xud - xd;
		double err_y = yud - yd;
		double err = err_x*err_x + err_y*err_y;

		xu = xu - err_x;
		yu = yu - err_y;

		if(err<err_thr) break;
	}

	Denormalize(xu, yu);

	px = (int)(xu + 0.5);
	py = (int)(yu + 0.5);
}

