
#include "stdafx.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	if (!g_config.Parse(g_configfileName))
	{
		cout << "press enter to exit program" << endl;
		getchar();
		return 0;
	}
 
	cvproc::cSearchPoint searchPoint1(1), searchPoint2(2);
	if (!searchPoint1.Init(g_recognition_configfilename1, g_contourfilename1, g_roi_filename1, true, true, true))
	{
		cout << "press enter to exit program" << endl;
		getchar();
		return 0 ;
	}

	if (!searchPoint2.Init(g_recognition_configfilename2, g_contourfilename2, g_roi_filename2, true, true, true))
	{
		cout << "press enter to exit program" << endl;
		getchar();
		return 0;
	}

	cout << "Check Camera1... index = " << g_config.m_conf.cameraPlayer1Index << endl;

	// 카메라, 스크린 초기화.
	cvproc::cCamera capture1;
// 	if (dxCapture1.Init(g_config.m_conf.cameraPlayer1Index, g_config.m_cameraWidth, g_config.m_cameraHeight, true))
	if (capture1.Init(0))
	{
		cout << "Detect Camera1" << endl;
	}
	else
	{
		cout << "Error!!! Not Detect Camera1" << endl;
		return 0;
	}

	capture1.m_show = true;

// 
// 	cout << "Check Camera2... index = " << g_config.m_conf.cameraPlayer2Index << endl;
// 	if (dxCapture2.Init(g_config.m_conf.cameraPlayer2Index, g_config.m_cameraWidth, g_config.m_cameraHeight, true))
// 	{
// 		cout << "Detect Camera2" << endl;
// 	}
// 

	network::cUDPClient standUdpComm;
 	cvproc::cStreamingSender streamSender;
 	cMGClient mgClient; // 개발자용 네트워크 통신 객체
 	mgClient.Init(&searchPoint1, &searchPoint2);


	// Server 모드이면, 시작되자마자 서버를 실행한다.
	if (g_config.m_conf.monitor_mode == 1)
	{
		cout << "----- MGClient Server Mode -----" << endl;
		mgClient.ConnectAsync((g_config.m_conf.monitor_mode == 0) ? true : false,
			g_config.m_conf.monitor_ip, g_config.m_conf.monitor_port);
	}
	else
	{
		cout << "----- MGClient Client Mode -----" << endl;
	}


	if (!standUdpComm.Init(g_config.m_udpClientConnectIp, g_config.m_udpClientConnectPort))
	{
		cout << "udp client connect Error!!" << endl;
		cout << "press enter to exit program" << endl;
		getchar();
		return 0;
	}


	if (!streamSender.Init(g_config.m_conf.stream_server_port,
		g_config.m_conf.stream_send_gray, g_config.m_conf.stream_send_compressed,
		g_config.m_conf.streaming_send_quality))
	{
		cout << "Steaming Sender Connect Error!!" << endl;
		return 0;
	}
	else
	{
		cout << "Success Connect Streaming Sender BindPort = " << g_config.m_conf.stream_server_port << endl;
	}



	cout << "Initialize Success, Start Loop" << endl;
// 
// 	dxCapture1.m_showFrame = g_config.m_conf.showCameraThreadFrame;
// 	dxCapture2.m_showFrame = g_config.m_conf.showCameraThreadFrame;
// 
	cPIDControl point1PID, point2PID;

	int oldT = GetTickCount();
	int fps = 0;
	int calcPoint1 = 0, calcPoint2 = 0;
	int incT = 0;
	int fpsTime = 0;
	float disp_x1 = 0, disp_y1 = 0;
	float disp_x2 = 0, disp_y2 = 0;
	cv::Point detectRawPos1(0, 0), detectRawPos2(0, 0);

	cout << "Start Camera Loop" << endl;

	bool pause = false;
	while (1)
	{
		const int curT = GetTickCount();
		const int deltaT = curT - oldT;
		incT += deltaT;
		fpsTime += deltaT;
		oldT = curT;

		// Calc FPS
		if (fpsTime > 1000)
		{
			if (g_config.m_conf.showCameraFrame)
			{
				cout << "fps = " << fps << " , calcPos1 Count = " << calcPoint1 << "  , calcPos2 Count = " << calcPoint2 << endl;
			}
			fps = 0;
			calcPoint1 = 0;
			calcPoint2 = 0;
			fpsTime = 0;
		}

		// Frame Delay
		if (incT < g_config.m_conf.cameraFrameDelay)
			continue;
		incT = 0;
		fps++;

		// debug game mode
		if ((g_config.m_gameMode == 1) || g_config.m_conf.showCamera)
		{
			const int key = cvWaitKey(1);
			if (key == ' ')
			{
				pause = !pause;
			}
			else if (key == 27)
			{
				break;
			}
			else if (key == 'r')
			{
				g_config.Parse(g_configfileName);
			}
		}
 
		Mat src1(capture1.GetCapture());
//		Mat src1(capture1.ShowCapture());
// 		Mat src2(dxCapture2.GetCloneBufferToImage());
		Mat src2 = src1.clone();

		// debug
//		searchPoint1.m_detectPoint.m_makeKeypointImage = true;

		//-------------------------------------------------------------------------
		// IR LED Detect
// 		if (searchPoint1.RecognitionSearch(src1, detectRawPos1, true, true))
// 			++calcPoint1;
// 		if (searchPoint2.RecognitionSearch(src2, detectRawPos2, true, true))
// 			++calcPoint2;
		if (searchPoint1.Update(src1, detectRawPos1))
			++calcPoint1;
		if (searchPoint2.Update(src2, detectRawPos2))
			++calcPoint2;

 		searchPoint1.ConvertUV(detectRawPos1, disp_x1, disp_y1);
 		searchPoint2.ConvertUV(detectRawPos2, disp_x2, disp_y2);
 
 
		//------------------------------------------------------------------------
		// PID Control
		point1PID.m_Kp = g_config.m_conf.pidControlP;
		point1PID.m_Ki = g_config.m_conf.pidControlI;
		point1PID.m_Kd = g_config.m_conf.pidControlD;

		point2PID.m_Kp = g_config.m_conf.pidControlP;
		point2PID.m_Ki = g_config.m_conf.pidControlI;
		point2PID.m_Kd = g_config.m_conf.pidControlD;

 		const Point2f pos1 = point1PID.Update(Point2f(disp_x1, disp_y1));
 		const Point2f pos2 = point2PID.Update(Point2f(disp_x2, disp_y2));
		disp_x1 = pos1.x;
		disp_y1 = pos1.y;
		disp_x2 = pos2.x;
		disp_y2 = pos2.y;
 		//------------------------------------------------------------------------

		SMGCameraData packet;
		packet.x1 = disp_x1;
		packet.y1 = disp_y1;
		packet.x2 = disp_x2;
		packet.y2 = disp_y2; // 총이 가르키는 위치 0 ~ 1 사이 값. 화면의 왼쪽 아래가 {0,0}

		standUdpComm.SendData((BYTE*)&packet, sizeof(packet));

//		cout << "detectRawPos1 =  " << detectRawPos1.x << ", " << detectRawPos1.y << ", disp_xy1 = " << disp_x1 << ", " << disp_y1 << endl;

		if (!searchPoint1.m_detectPoint.m_binWithKeypoints.empty())
			imshow("keypoints", searchPoint1.m_detectPoint.m_binWithKeypoints);


		//------------------------------------------------------------------------
		// Monitor Networking
		mgClient.Update();

		if (g_config.m_conf.stream_player >= 0)
		{
			streamSender.CheckPacket();
			streamSender.Send((g_config.m_conf.stream_player == 0) ? src1 : src2);
		}
 	}

	return 0;
}
