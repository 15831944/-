#pragma once
#include <windef.h>


// CMyProgress

class CMyProgress : public CProgressCtrl
{
	DECLARE_DYNAMIC(CMyProgress)

public:
	CMyProgress();
	virtual ~CMyProgress();
	
	void SetRange(int nStart, int nEnd);
	int SetPos(int nPos);
protected:
	DECLARE_MESSAGE_MAP()

public:
	COLORREF m_prgsColor; //���������Ȳ�����ɫ
	COLORREF m_freeColor; //������������ಿ����ɫ
	COLORREF m_prgsTextColor; //���Ȳ���������ɫ
	COLORREF m_freeTextColor; //�հײ���������ɫ

	int  m_iMin;    //����������Сֵ��ͨ����0
	int  m_iMax;    //�����������ֵ��ͨ����100
	int  m_iPos;    //��ǰ�Ľ���
	int  m_nBarWidth;  //���������
	afx_msg void OnPaint();
};


