#pragma once
#include "CNameCell.h"
class CCommonCell;

typedef vector<CCommonCell> COMVEC;
typedef pair<CCommonCell, CCommonCell> COMPAIR;
typedef map<CString, vector<CCommonCell> > MDMap;
typedef map<pair<int, CString>, vector<CCommonCell> > MRMAP;
typedef map<int, pair<CCommonCell, CCommonCell> > MPMAP;
typedef map<CString, vector<pair<CCommonCell, CCommonCell> > > MVMAP;

class CCommonCell
{
public:
	CCommonCell();
	~CCommonCell();
	void setDuanzi(CString strDuanzi);
	void setTexing(CString strTexing);
	void setQuxiang(CString strQuxiang);
	void setRealTexing(CString strRealQuxiang);
	void setType(int strType);
	void setCount(CString nCount);
	void setModQuxiang(CString strModQuxiang);
	void setTempQuxiang(CString strTempQuxiang);
	void setPreQuxiang(CString strPreQuxiang);//��һ��ȥ��
	void setRow(int nRow);
	void setCol(int nCol);
	void setSheetName(CString strSheetName);
	//void setExcelRow(int nExcelRow);
	//void setExcelCol(int nExcelCol);
	//void setNickName(CString strNickName);
	void setName(CString strName);
	void setNameCell(CNameCell nameCell);
	CString getDuanzi() const;
	CString getTexing() const;
	CString getQuxiang() const;
	CString getRealTexing() const;
	CString getChineseRealTexing() const;
	CString getModQuxiang() const;
	CString getTempQuxiang() const;
	CString getPreQuxiang() const;
	int getType() const;
	CString getCount() const;
	int getRow() const;
	int getCol() const;
	CString getSheetName() const;
	//int getExcelRow() const;
	//int getExcelCol() const;
	//CString getNickName() const;
	CString getName() const;
	CNameCell getNameCell() const;

	CCommonCell& operator=(const CCommonCell& node);
	//�ж�����comcell�Ƿ���ͬ
	bool IsEqual(CCommonCell comCell);

	CString getShortTexing() const;
	CString getModName() const;
	CString getShortName() const;
	CString getWeihao() const;
	CString getRealSheetName() const;
	//��ȡ�㼶����
	int getRlevel() const;
	//��ȡM��������
	bool getMFlag() const;
	bool getRFFlag() const;
private:
	CString m_strDuanzi;
	CString m_strTexing;
	CString m_strRealTexing;
	CString m_strQuxiang;
	CString m_strModQuxiang;
	CString m_strTempQuxiang;
	//0Ϊ��ͨ���ݣ�1ΪDp���ݣ�2ΪRF���ݣ�3Ϊ���ֹ����ݣ�
	//4Ϊģ������ݣ�5Ϊ���������ݣ�6ΪR�㼶���ݣ�
	//7ΪS���ݣ�8ΪM���ݣ�9ΪR+S����
	//10ΪR+M����,11Ϊ����������,12ΪLRMV��-1Ϊ��[D]������
	int m_nType;
	
	CString m_strCount;//����������ǵ�Ƶ�͸�Ƶ����ס���Զ�Ӧ������
	int m_nRow;
	int m_nCol;
	CString m_strSheetName;
	//int m_nExcelRow;
	//int m_nExcelCol;
	//CString m_strNickName;//����R�㼶��SM�㼶��ʱ��
	CString m_strName;//��item��Ӧ������
	CString m_strPreQuxiang;
	CNameCell m_nameCell;
};