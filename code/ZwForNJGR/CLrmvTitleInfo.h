#pragma once

class CLrmvTitleInfo
{
public:
	CLrmvTitleInfo(void);
	~CLrmvTitleInfo(void);
	int startIndex() const;
	int endIndex() const;
	int type() const;
	CString name() const;

	void setStartIndex(int nIndex);
	void setEndIndex(int nIndex);
	void setType(int nType);
	void setName(CString strName);
private:
	int m_nStart;
	int m_nEnd;
	int m_nType;//����0Ϊ��ͨ���ݣ�1Ϊ��Ƶ���ݣ�2Ϊ������������
	CString m_strName;//ͷ������P0����
};
