//
// UDP�� ���۵Ǵ� ���������� ������
//
//
#pragma once




//--------------------------------------------------------------------------------------------------------------
// �ӽŰ� ���κ��忡�� ������ UDP ��Ŷ ��������
// Stand Version


struct sMGDevicePacket
{
	char header; // $
	char comma1; // ,
	char deviceNumber; // 9
	char zero1; // 0
	char comma2; // ,

	char player1Fire;			// 0/1
	char player1FireEvent;		// 0/1
	char player1Reload;			// 0/1
	char player1Start;			// 0/1
	char player1UpMotor;		// 0/1
	char player1DownMotor;		// 0/1
	char player1UpSensor;		// 0/1
	char player1DownSensor;		// 0/1
	char player1EmergencySwitch;// 0/1
	char coin;
	char space1;

	char player2Fire;			// 0/1
	char player2FireEvent;		// 0/1
	char player2Reload;			// 0/1
	char player2Start;			// 0/1
	char player2UpMotor;		// 0/1
	char player2DownMotor;		// 0/1
	char player2UpSensor;		// 0/1
	char player2DownSensor;		// 0/1
	char player2EmergencySwitch;// 0/1
	char space3;
	char space4;

	char comma3;
	char at;

	char cr[2];
};



//--------------------------------------------------------------------------------------------------------------
// �ӽŰ� �Ŵ������� ���� Ŭ���̾�Ʈ���� ������ ��Ŷ ��������

struct SMGCameraData
{
	float x1, y1, x2, y2; // ���� ����Ű�� ��ġ 0 ~ 1 ���� ��. ȭ���� ���� �Ʒ��� {0,0}
	int fire1; // �÷���1 �ݹ�, 1:�ݹ�, 0:Nothing~
	int fire2; // �÷���2 �ݹ�, 1:�ݹ�, 0:Nothing~
	int reload1; // �÷���1 ���ε�, 1:���ε�, 0:Nothing~
	int reload2; // �÷���2 ���ε�, 1:���ε�, 0:Nothing~
	int start1; // �÷���1 ��ŸƮ��ư On/Off, 1:On, 0:Off
	int start2; // �÷���2 ��ŸƮ��ư On/Off, 1:On, 0:Off
	int credit; // ���� �÷��� �� �� �ִ� ȸ��
	int coinCount; // ������ ���� ����
	int coinPerGame; // �� ���Ӵ� ���� ����
};

