#pragma once
#include "MyExcel.h"
//���ɽ��߱� �ͽ��߹�ϵ��

class CGenerateJxb
{
public:
	CGenerateJxb(void);
	~CGenerateJxb(void);
	//���ý��߱�����
	void setType(bool bIsJxb);

	bool JudgeIsEmpty();
	bool openExcel();

	bool writeJxb();
	bool writeJxbgxb();
	bool writeDpInfoToExcel(int& nRow);
	bool writeRfInfoToExcel(int& nRow);
	bool writeMTjxbToExcel(int& nRow);
	//���߱�
	bool writeRMTJxbToExcel(int& nRow);
	//���߹�ϵ��
	bool writeMTJxgxbToExcel(int& nRow);
	bool writeTInfoToExcel(int& nRow);
private:
	//�ȴ�����߱�ı��
	void doJxbExcel();
	//������
	int skipRow(int nRow);
	bool insertRow(int nRow);

private:
	bool m_bIsJxb;//�Ƿ���߱�
	CMyExcel m_Excel;
	int m_nSize;//dp��Rf�Ĵ�С
	CString m_strFile;
	MPMAP m_dpOutInfo;
	MPMAP m_rfOutInfo;
	map<CString, vector<pair<CCommonCell, CCommonCell> > > m_mtOutPutMap;
};

