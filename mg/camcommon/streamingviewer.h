//
// 2015-12-07, jjuiddong
//
// cStreamingReceiver�� ����� ����� ������, Ŭ���̾�Ʈ�� ���ú� ������
// ������, �̹��� ������ �޾ƿ��� ����� �Ѵ�.
//
#pragma once


namespace cvproc
{

	class cStreamingViewer
	{
	public:
		cStreamingViewer();
		virtual ~cStreamingViewer();

		bool Init(const bool isUDP, const string &serverIp, const int port);
		cv::Mat& Update();


		//network::cUDPServer m_udpServer;
		network::cTCPClient	m_tcpServer;
		bool m_isUDP;
		cv::Mat m_src;
		cv::Mat m_finalImage;		// ���� �̹���
		BYTE *m_rcvBuffer;
	};

}
