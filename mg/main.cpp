
#include "stdafx.h"
//#include <iostream>
// #include "mgudpcommunication.h"
// #include "mgstandudpcomm.h"

using namespace std;


int main(int argc, char **argv)
{
	if (!g_config.Parse(g_configfileName))
	{
		cout << "press enter to exit program" << endl;
		getchar();
		return 0;
	}
 
	cvproc::cSearchPoint searchPoint1(1), searchPoint2(2);
	if (!searchPoint1.Init(g_recognition_configfilename1, g_contourfilename1, g_roi_filename1))
	{
		cout << "press enter to exit program" << endl;
		getchar();
		return 0 ;
	}

	if (!searchPoint2.Init(g_recognition_configfilename2, g_contourfilename2, g_roi_filename2))
	{
		cout << "press enter to exit program" << endl;
		getchar();
		return 0;
	}

	const MG_TYPE::TYPE mgGameType = (g_config.m_projectName == "machinegun_stand") ? MG_TYPE::STAND : MG_TYPE::RIDER;

	cout << "Check Camera1... index = " << g_config.m_conf.cameraPlayer1Index << endl;

// 	// ī�޶�, ��ũ�� �ʱ�ȭ.
// 	cDxCapture dxCapture1, dxCapture2;
// 	if (dxCapture1.Init(g_config.m_conf.cameraPlayer1Index, g_config.m_cameraWidth, g_config.m_cameraHeight, true))
// 	{
// 		cout << "Detect Camera1" << endl;
// 	}
// 
// 	cout << "Check Camera2... index = " << g_config.m_conf.cameraPlayer2Index << endl;
// 	if (dxCapture2.Init(g_config.m_conf.cameraPlayer2Index, g_config.m_cameraWidth, g_config.m_cameraHeight, true))
// 	{
// 		cout << "Detect Camera2" << endl;
// 	}
// 
// 
// 	cout << "Create shmem....";
// 	cSharedMemoryCommunication sharedMem;
// 	if (sharedMem.Init(g_config.m_sharedMemName))
// 	{
// 		cout << " Ok" << endl;
// 
// 		// �ʱⰪ ����
// 		mgnetwork::sSharedMemData data;
// 		data.tryConnectServer = false;
// 		*(mgnetwork::sSharedMemData*)sharedMem.GetMemoryPtr() = data;
// 	}
// 	else
// 	{
// 		cout << "Fail!" << endl;
// 		cout << "press enter to exit program" << endl;
// 		getchar();
// 		return;
// 	}
// 
// 	cMGUDPCommunication riderUdpComm;
// 	cMGStandUDPComm standUdpComm;
// 	cvproc::cStreamingSender streamSender;
// 	cMGClient mgClient; // �����ڿ� ��Ʈ��ũ ��� ��ü
// 	mgClient.Init(&searchPoint1, &searchPoint2);
// 
// 	int streamingStartTime = 0; // ���� �̻� �ð����� ��Ʈ������ �ϸ� �ڵ����� ����� �Ѵ�. 1�ð�
// 
// 
// 	// Server ����̸�, ���۵��ڸ��� ������ �����Ѵ�.
// 	if (g_config.m_conf.monitor_mode == 1)
// 	{
// 		cout << "----- MGClient Server Mode -----" << endl;
// 		mgClient.ConnectAsync((g_config.m_conf.monitor_mode == 0) ? true : false,
// 			g_config.m_conf.monitor_ip, g_config.m_conf.monitor_port);
// 	}
// 	else
// 	{
// 		cout << "----- MGClient Client Mode -----" << endl;
// 	}
// 
// 
// 	if (mgGameType == MG_TYPE::RIDER)
// 	{
// 		if (!riderUdpComm.Init())
// 		{
// 			cout << "udp server bind Error!!" << endl;
// 			cout << "press enter to exit program" << endl;
// 			getchar();
// 			return;
// 		}
// 	}
// 	else if (mgGameType == MG_TYPE::STAND)
// 	{
// 		if (!standUdpComm.Init(g_config.m_udpClientConnectIp, g_config.m_udpClientConnectPort))
// 		{
// 			cout << "udp client connect Error!!" << endl;
// 			cout << "press enter to exit program" << endl;
// 			getchar();
// 			return;
// 		}
// 	}
// 	else
// 	{
// 		cout << "undefined project name";
// 		cout << "press enter to exit program" << endl;
// 		getchar();
// 		return;
// 	}
// 
// 	cout << "Initialize Success, Start Loop" << endl;
// 
// 	dxCapture1.m_showFrame = g_config.m_conf.showCameraThreadFrame;
// 	dxCapture2.m_showFrame = g_config.m_conf.showCameraThreadFrame;
// 
// 	cPIDControl point1PID, point2PID;
// 
// 	int oldT = timeGetTime();
// 	int fps = 0;
// 	int incT = 0;
// 	int fpsTime = 0;
// 	float disp_x1 = 0, disp_y1 = 0, disp_x2 = 0, disp_y2 = 0;
// 	Point detectRawPos1(0, 0), detectRawPos2(0, 0);
// 
// 	cout << "Start Camera Loop" << endl;
// 
// 	bool pause = false;
// 	while (1)
// 	{
// 		const int curT = timeGetTime();
// 		const int deltaT = curT - oldT;
// 		incT += deltaT;
// 		fpsTime += deltaT;
// 		oldT = curT;
// 
// 		// Calc FPS
// 		if (fpsTime > 1000)
// 		{
// 			if (g_config.m_conf.showCameraFrame)
// 				printf("fps = %d\n", fps);
// 			fps = 0;
// 			fpsTime = 0;
// 		}
// 
// 		// Frame Delay
// 		if (incT < g_config.m_conf.cameraFrameDelay)
// 			continue;
// 		incT = 0;
// 		fps++;
// 
// 		// debug game mode
// 		if ((g_config.m_gameMode == 1) || g_config.m_conf.showCamera)
// 		{
// 			const int key = cvWaitKey(1);
// 			if (key == ' ')
// 			{
// 				pause = !pause;
// 			}
// 			else if (key == VK_ESCAPE)
// 			{
// 				break;
// 			}
// 			else if (key == 'r')
// 			{
// 				g_config.Parse(g_configfileName);
// 			}
// 		}
// 
// 		Mat src1(dxCapture1.GetCloneBufferToImage());
// 		Mat src2(dxCapture2.GetCloneBufferToImage());
// 
// 		//-------------------------------------------------------------------------
// 		// IR LED Detect
// 		Point2f out1, out2;
// 		// 		searchPoint1.RecognitionSearch(src1, detectRawPos1, true, true);
// 		// 		searchPoint2.RecognitionSearch(src2, detectRawPos2, true, true);
// 		searchPoint1.RecognitionSearch(src1, detectRawPos1, true, false);
// 		searchPoint2.RecognitionSearch(src2, detectRawPos2, true, false);
// 
// 		searchPoint1.ConvertUV(detectRawPos1, disp_x1, disp_y1);
// 		searchPoint2.ConvertUV(detectRawPos2, disp_x2, disp_y2);
// 
// 
// 		//------------------------------------------------------------------------
// 		// PID Control
// 		point1PID.m_Kp = g_config.m_conf.pidControlP;
// 		point1PID.m_Ki = g_config.m_conf.pidControlI;
// 		point1PID.m_Kd = g_config.m_conf.pidControlD;
// 
// 		point2PID.m_Kp = g_config.m_conf.pidControlP;
// 		point2PID.m_Ki = g_config.m_conf.pidControlI;
// 		point2PID.m_Kd = g_config.m_conf.pidControlD;
// 
// 		const Point2f pos1 = point1PID.Update(Point2f(disp_x1, disp_y1));
// 		const Point2f pos2 = point2PID.Update(Point2f(disp_x2, disp_y2));
// 		disp_x1 = pos1.x;
// 		disp_y1 = pos1.y;
// 		disp_x2 = pos2.x;
// 		disp_y2 = pos2.y;
// 		//------------------------------------------------------------------------
// 
// 
// 		if (mgGameType == MG_TYPE::RIDER)
// 		{
// 			//-------------------------------------------------------------------------------------------------
// 			// �ӽŰ� ���κ��忡�� ��Ŷ�� �޾Ƽ�, ����Ŭ���̾�Ʈ���� ������.
// 			// Packet Receive
// 			sMGDevice1Packet packet1;
// 			sMGDevice2Packet packet2;
// 			sMGDevice3Packet packet3;
// 			riderUdpComm.GetDeviceRecvData(packet1, packet2, packet3);
// 
// 			SMGCameraData packet;
// 			packet.x1 = disp_x1;
// 			packet.y1 = disp_y1;
// 			packet.x2 = disp_x2;
// 			packet.y2 = disp_y2; // ���� ����Ű�� ��ġ 0 ~ 1 ���� ��. ȭ���� ���� �Ʒ��� {0,0}
// 			packet.fire1 = (packet3.player1Fire == '1') ? 1 : 0; // �÷���1 �ݹ�, 1:�ݹ�, 0:Nothing~
// 			packet.fire2 = 0; // �÷���2 �ݹ�, 1:�ݹ�, 0:Nothing~
// 			packet.start1 = (packet1.player1StartSwitch == '1') ? 1 : 0; // �÷���1 ��ŸƮ��ư On/Off, 1:On, 0:Off
// 			packet.start2 = (packet2.player2StartSwitch == '1') ? 1 : 0; // �÷���2 ��ŸƮ��ư On/Off, 1:On, 0:Off
// 
// 			// MachineGun Multi ����.
// 			// 			packet.steer = 0; // �ڵ� {-1 ~ 1 ���̰�}
// 			// 			packet.accel = 0; // ���� ��� {0~1 ���̰�}
// 			// 			packet.break0 = 0; // �극��ũ ���
// 			// 			packet.reset = 0; // ����, {1: ����}
// 			// 			packet.backward = 0; // ����, {1:����}
// 
// 			riderUdpComm.SendPacket((BYTE*)&packet, sizeof(packet));
// 			//-------------------------------------------------------------------------------------------------
// 		}
// 		else if (mgGameType == MG_TYPE::STAND)
// 		{
// 			SMGCameraData packet;
// 			packet.x1 = disp_x1;
// 			packet.y1 = disp_y1;
// 			packet.x2 = disp_x2;
// 			packet.y2 = disp_y2; // ���� ����Ű�� ��ġ 0 ~ 1 ���� ��. ȭ���� ���� �Ʒ��� {0,0}
// 
// 			standUdpComm.SendPacket((BYTE*)&packet, sizeof(packet));
// 		}
// 
// 
// 		//---------------------------------------------------------------------------------------
// 		// Check Shared Memory
// 		if (((mgnetwork::sSharedMemData*)sharedMem.GetMemoryPtr())->tryConnectServer)
// 		{
// 			if (!g_config.Parse(g_configfileName))
// 				return;
// 
// 			mgClient.ConnectAsync(!g_config.m_conf.monitor_mode, g_config.m_conf.monitor_ip, g_config.m_conf.monitor_port);
// 			((mgnetwork::sSharedMemData*)sharedMem.GetMemoryPtr())->tryConnectServer = false;
// 		}
// 
// 
// 		//---------------------------------------------------------------------------------------
// 		// Monitor Networking
// 		mgClient.Update();
// 
// 
// 		//---------------------------------------------------------------------------------------
// 		// Debugging
// 		if (g_config.m_conf.showCamera)
// 			imshow("player1", src1);
// 		if (g_config.m_conf.showCamera)
// 			imshow("player2", src2);
// 
// 		if (g_config.m_conf.stream_player1_enable || g_config.m_conf.stream_player2_enable)
// 		{
// 			// ������ �ٲ��, ������ ���� �ٽ� �����Ѵ�.
// 			if (g_config.m_changeStreamingOption)
// 			{
// 				g_config.m_changeStreamingOption = false;
// 				streamSender.Close();
// 			}
// 
// 			if (!streamSender.IsConnect(g_config.m_conf.stream_send_udp))
// 			{
// 				cout << "Try Connet Steaming Sender... " << endl;
// 
// 				if (!streamSender.Init(g_config.m_conf.stream_send_udp,
// 					g_config.m_conf.stream_send_ip, g_config.m_conf.stream_send_port,
// 					g_config.m_conf.stream_send_gray, g_config.m_conf.stream_send_compressed,
// 					g_config.m_conf.streaming_send_quality))
// 				{
// 					cout << "Steaming Sender Connect Error!!" << endl;
// 				}
// 				else
// 				{
// 					streamingStartTime = curT;
// 					cout << "Success Connect Streaming Sender" << endl;
// 				}
// 			}
// 			else
// 			{
// 				streamSender.Send(g_config.m_conf.stream_player1_enable ? src1 : src2);
// 
// 				// 1�ð��̻� ��Ʈ�����ϸ�, �ڵ����� �����.
// 				if (curT - streamingStartTime > 3600000)
// 				{
// 					cout << "Timeup!! Steaming Auto Close!!" << endl;
// 
// 					g_config.m_conf.stream_player1_enable = false;
// 					g_config.m_conf.stream_player2_enable = false;
// 					streamSender.Close();
// 				}
// 			}
// 		}
// 		//---------------------------------------------------------------------------------------
// 
// 	}
// 
// 	// Release COM
// 	CoUninitialize();

	return 0;
}
