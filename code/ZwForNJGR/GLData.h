#pragma once
#include "CCommonCell.h"
#include "CNameCell.h"
#include <vector>
using namespace std;

class CGLData
{
public:
	CGLData(void);
	~CGLData(void);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void setNamCell(CNameCell nameCell);
	void setCommonCellVec(vector<CCommonCell>  vec);
	void add(CCommonCell comCell);
	void setType(int nType);
	void setNumber(CString nNum);

	CNameCell getNameCell() const;
	vector<CCommonCell > getCommonVec() const;
	int type() const;
	CString number() const;
	CGLData& operator=(const CGLData& node);
	void clear();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

private:
	CNameCell m_nameCell;
	vector<CCommonCell>  m_ComVec;
	int m_nType;//�����������ͣ�0��ʾ�����ˣ�1��ʾ12���ˣ�2��ʾ24���ˣ�3��ʾ48����
	CString m_strNumber;//���
};
