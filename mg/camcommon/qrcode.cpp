
#include "stdafx.h"
#include "qrcode.h"


using namespace cvproc;


cQRCode::cQRCode() :
	m_dbgShow(false)
{
}

cQRCode::~cQRCode()
{
}


// Ŭ���� �ʱ�ȭ
bool cQRCode::Init(const string &recogFileName)
{
	if (!UpdateConfig(recogFileName))
		return false;

	return true;
}


// ȯ������ ������Ʈ
bool cQRCode::UpdateConfig(const string &recogFileName)
{
	if (!m_recogCfg.Read(recogFileName))
		return false;

	m_detectPoint.UpdateParam(m_recogCfg);
	return true;
}


// QRCode�� �ν��Ѵ�.
// �߰��ϸ� true�� �����Ѵ�.
bool cQRCode::Detect(const Mat &src)
{
	m_detectPoint.m_makeKeypointImage = m_dbgShow;

	Point pos;
	if (m_detectPoint.DetectPoint(src, pos))
	{
		// �ﰢ�� ó��
		vector<cv::Point> tri;
		if (m_detectPoint.m_points.size() == 3)
		{
			CheckTriangle(m_detectPoint.m_points, tri);
		}

		if (m_dbgShow)
		{
			Mat dst = m_detectPoint.m_binWithKeypoints.clone();
			if (m_detectPoint.m_points.size() >= 3)
			{
				cLineContour lines(m_detectPoint.m_points);
				lines.Draw(dst, Scalar(255, 0, 0), 2, true);
			}

			static const Scalar colors[3] = {
				Scalar(0, 0, 255),
				Scalar(0, 255, 0),
				Scalar(255, 0, 0),
			};
			for (u_int i = 0; i < tri.size(); ++i)
			{
				circle(dst, tri[i], 8, colors[i], 2);
			}
			
			imshow("qrcode", dst);
		}

		return true;
	}
	else
	{
		if (m_dbgShow)
			imshow("qrcode", m_detectPoint.m_binMat);
	}

	return false;
}


// ���� �ﰢ���̶�� true�� �����Ѵ�.
// ���� �ﰢ���̶��, ���̰� �ϳ��� 90�� ���� �Ѵ�.
// Out : �����ﰢ���̶�� �ǴܵǸ�, ������ �������� �ð�������� 
//		 ������� �����ؼ� �����Ѵ�.
//
//        *p0 --- *p1
//        |      /
//        |    /
//        |  /
//        * p2
//
bool cQRCode::CheckTriangle(const vector<cv::Point> &points, vector<cv::Point> &out)
{
	RETV(points.size() != 3, false);

	const Vector3 p1((float)points[0].x, (float)points[0].y, 0);
	const Vector3 p2((float)points[1].x, (float)points[1].y, 0);
	const Vector3 p3((float)points[2].x, (float)points[2].y, 0);

	//            v3
	//        *p2 --- *p3
	//        |      /
	//      v1|    / v2
	//        |  /
	//        * p1
	//

 	Vector3 v12 = p2 - p1;
 	Vector3 v13 = p3 - p1;
 	Vector3 v23 = p3 - p2;
	const float len12 = v12.Length();
	const float len13 = v13.Length();
	const float len23 = v23.Length();
	v12.Normalize();
	v13.Normalize();
	v23.Normalize();

	const float d1 = v12.DotProduct(v13);
	const float d2 = v23.DotProduct(-v12);
	const float d3 = (-v23).DotProduct(-v13);
	const float minD = min(d1, min(d2,d3));

 	using namespace std;
 	cout << "d1 = " << d1 << endl;
	cout << "d2 = " << d2 << endl;
	cout << "d3 = " << d3 << endl;
	cout << "minD = " << minD << endl;

 	cout << "len1 = " << len12 << endl;
 	cout << "len2 = " << len13 << endl;
	cout << "len3 = " << len23 << endl;
	
	// ���� ū ���� 90���� ������ �����ﰢ���� ���Ѵ�.
	// dotproduct �� ���� 90�� ��������, 0�� ������.
	// �ƴ϶��, false�� ����
	if (abs(minD) > 0.25f) 
		return false;

	int arr[3];
	if (minD == d1) // v1-v2 -> p1 = ����
	{
		arr[0] = 0;

		if (v12.CrossProduct(v13).z > 0)
		{
			arr[1] = 1;
			arr[2] = 2;
		}
		else
		{
			arr[1] = 2;
			arr[2] = 1;
		}
	}
	else if (minD == d2) // v1-v3 -> p2 = ����
	{
		arr[0] = 1;

		if (v23.CrossProduct(-v12).z > 0)
		{
			arr[1] = 2;
			arr[2] = 0;
		}
		else
		{
			arr[1] = 0;
			arr[2] = 2;
		}
	}
	else // p3 = ����
	{
		arr[0] = 2;

		if ((-v23).CrossProduct(-v13).z > 0)
		{
			arr[1] = 0;
			arr[2] = 1;
		}
		else
		{
			arr[1] = 1;
			arr[2] = 0;
		}
	}

	out.resize(3);
	out[0] = points[arr[0]];
	out[1] = points[arr[1]];
	out[2] = points[arr[2]];

	return true;
}
