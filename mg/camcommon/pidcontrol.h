//
// PID ���� Ŭ����
// 
// 2���� ���� ���� �� �� �ִ�.
//
#pragma once


class cPIDControl
{
public:
	cPIDControl();
	virtual ~cPIDControl();

	cv::Point2f Update(const cv::Point2f &pos);


public:
	cv::Point2f m_originalPos;
	cv::Point2f m_Pos;
	cv::Point2f m_oldDiff;

	float m_Kp; // proportional 
	float m_Ki; // integral
	float m_Kd; // derivative
};
