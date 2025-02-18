#pragma once
#include "CNameCell.h"
#include "CCommonCell.h"
#include <vector>
using namespace std;

class CTableData
{
public:
	CTableData(void);
	~CTableData(void);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void setNamCell(CNameCell nameCell);
	void setCommonCellVec(vector<vector<CCommonCell> > vec);
	void add(vector<CCommonCell> comCell);
	void clear();
	CNameCell getNameCell() const;
	vector<vector<CCommonCell> > getCommonVec() const;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

private:
	CNameCell m_nameCell;
	vector<vector<CCommonCell> > m_ComVec;
};
