#pragma once
#include "CFrameInfo.h"
//ȫ�ֶ�д�ļ�

class CGlobalRWriteFile
{
public:
	CGlobalRWriteFile(void);
	~CGlobalRWriteFile(void);

	void clear();

	//��ȡͼ��������ļ�
	bool readFrameIniFile();
	//�༭ͼ���е������ļ�
	bool EditFrameIniFile();
	//����ͼ���е������ļ�
	bool addFrameIniFile(CString strName, CFrameInfo frameInfo);
	//ɾ��ͼ���е������ļ�
	bool delFrameIniByName(CString strName);
	//�������ļ��еõ����ݣ����û�оͷ���Ĭ��ֵ
	CFrameInfo getFrameInfoByName(CString strLrmName);

	map<int, CFrameInfo> getAllFrameInfo();
	void saveAllFrameInfo();

private:
	//�����ļ�
	map<CString, CFrameInfo> m_FrameIni;
	//��һ������Ϊsheet���ƣ��ڶ�������Ϊ�����ͼ�����
	map<int, CFrameInfo> m_AllFrame;
};

