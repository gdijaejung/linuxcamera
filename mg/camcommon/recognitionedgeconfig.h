//
// 2016-01-21, jjuiddong
//
// �ܰ����� �����ϴµ� �ʿ��� �Ķ���͸� �а�, ���� ����� �Ѵ�.
//
#pragma once


struct sRecognitionEdgeConfigAttribute
{
	int threshold1;
	int threshold2;
	int minArea;
	double minCos;
	double maxCos;
};


class cRecognitionEdgeConfig : public common::cConfig
{
public:
	virtual void InitDefault() override;
	virtual void UpdateParseData() override;
	bool Write(const string &fileName);

	cRecognitionEdgeConfig& operator = (const cRecognitionEdgeConfig &rhs);


public:
	sRecognitionEdgeConfigAttribute m_attr;
};

