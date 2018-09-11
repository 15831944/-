#include "StdAfx.h"
#include "Utility.h"
#include <shlwapi.h>
#include "BaseUtils.h"
#include "aclapi.h"
#include "CGlobalQuery.h"
#include "CGlobalRWriteFile.h"

extern CGlobalQuery gQuery;
extern CGlobalRWriteFile gRWriteFile;

CUtility::CUtility(void)
{
}

CUtility::~CUtility(void)
{
}

AcDbObjectId CUtility::getStyleIdByiniFile(CString strHead, CString strNo)
{
	CString buffer = gGlobal.GetIniValue(strHead, strNo);
	if (buffer.IsEmpty())//���Ϊ�գ�����null
	{
		return AcDbObjectId::kNull;
	}
	CString strDescription = MyParserString::SubString(buffer, _T(","), 0);
	CString strFont = MyParserString::SubString(buffer, _T(","), 1);
	CString strBigFont = MyParserString::SubString(buffer, _T(","), 2);
	CString strHeight = MyParserString::SubString(buffer, _T(","), 3);
	CString strScale = MyParserString::SubString(buffer, _T(","), 4);
	AcDbObjectId textStyleId = MySymble::CreateTextStyle(strDescription, strFont, strBigFont, MyTransFunc::StringToDouble(strScale), MyTransFunc::StringToDouble(strHeight), TRUE);

	return textStyleId;

}

AcDbObjectId CUtility::createBlkRef()
{
	//Ѱ���Ƿ��Ѿ�����
	double dTableHeight = gGlobal.GetIniValue(_T("��������"), _T("���߶�"), 0);
	double dFirstTableLength = gGlobal.GetIniValue(_T("��������"), _T("����һ�γ���"), 0);
	double dSecondTableLength =  gGlobal.GetIniValue(_T("��������"), _T("���ڶ��γ���"), 0);
	double dThirdTableLength  = gGlobal.GetIniValue(_T("��������"), _T("�������γ���"), 0);
	double dTableLength = dFirstTableLength + dSecondTableLength + dThirdTableLength;
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase(); 
	AcDbBlockTable *pBlkTable;
	AcDbObjectId blockId ; 
	pCurDb->getBlockTable(pBlkTable, AcDb::kForWrite); 

	CString strBlockname;
	strBlockname = _T("ZW_TABLE");
	AcDbBlockTableRecord* pBlkTblRec = new AcDbBlockTableRecord;
	pBlkTblRec->setName(strBlockname);
	AcDbObjectId blockTblRecId;
	Acad::ErrorStatus es = pBlkTable->add(blockTblRecId, pBlkTblRec);
	if (es == Acad::eDuplicateRecordName)
	{
		AcDbBlockTableRecord* pPreBlkTblRec = NULL;
		es = pBlkTable->getAt(strBlockname, pPreBlkTblRec, AcDb::kForWrite);
		if (es != Acad::eOk)
		{
			pBlkTable->close();
			return AcDbObjectId::kNull;
		}
		//ɾ������¼��ʵ��
		AcDbEntity* pEnt = NULL;
		AcDbBlockTableRecordIterator* blkIter;
		if (pPreBlkTblRec->newIterator(blkIter) != Acad::eOk)
		{
			pBlkTable->close();
			return AcDbObjectId::kNull;
		}
		for (; !blkIter->done(); blkIter->step()) 
		{
			if (blkIter->getEntity(pEnt, AcDb::kForWrite) == Acad::eOk)
			{
				if (pEnt->isKindOf(AcDbBlockBegin::desc())|| (pEnt->isKindOf(AcDbBlockEnd::desc())))
				{
					//�����в���
				}
				else
					pEnt->erase();
				pEnt->close();
			}
		}
		delete blkIter;
		pPreBlkTblRec->close();
		es = pBlkTable->getAt(strBlockname, pBlkTblRec, AcDb::kForWrite);
	}
	pBlkTable->close();

	AcGePoint3d pt1,pt2,pt3,pt4,pt5,pt6,pt7,basePt;
	basePt.set(0, 0, 0);
	acutPolar(asDblArray(basePt), 0, dTableLength, asDblArray(pt1));

	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbLine* pLine1 = new AcDbLine;
	pLine1->setDatabaseDefaults();
	pLine1->setStartPoint(basePt);
	pLine1->setEndPoint(pt1);
	pLine1->setColorIndex(0);
	pLine1->setLayer(_T("0"));
	pLine1->setLinetypeScale(1.0);
	pBlkTblRec->appendAcDbEntity(pLine1);
	pLine1->close();


	acutPolar(asDblArray(basePt), PI/2, dTableHeight, asDblArray(pt2));
	AcDbLine* pLine2 = new AcDbLine;
	pLine2->setDatabaseDefaults();
	pLine2->setStartPoint(basePt);
	pLine2->setEndPoint(pt2);
	pLine2->setColorIndex(0);
	pLine2->setLayer(_T("0"));
	pLine2->setLinetypeScale(1.0);
	pBlkTblRec->appendAcDbEntity(pLine2);
	pLine2->close();

	acutPolar(asDblArray(pt2), 0, dTableLength, asDblArray(pt3));

	AcDbLine* pLine3 = new AcDbLine;
	pLine3->setDatabaseDefaults();
	pLine3->setStartPoint(pt3);
	pLine3->setEndPoint(pt2);
	pLine3->setColorIndex(0);
	pLine3->setLayer(_T("0"));
	pLine3->setLinetypeScale(1.0);
	pBlkTblRec->appendAcDbEntity(pLine3);
	pLine3->close();


	AcDbLine* pLine4 = new AcDbLine;
	pLine4->setDatabaseDefaults();
	pLine4->setStartPoint(pt3);
	pLine4->setEndPoint(pt1);
	pLine4->setColorIndex(0);
	pLine4->setLayer(_T("0"));
	pLine4->setLinetypeScale(1.0);
	pBlkTblRec->appendAcDbEntity(pLine4);
	pLine4->close();

	acutPolar(asDblArray(basePt), 0, dFirstTableLength, asDblArray(pt5));
	acutPolar(asDblArray(pt2), 0,dFirstTableLength, asDblArray(pt4));

	AcDbLine* pLine5= new AcDbLine;
	pLine5->setDatabaseDefaults();
	pLine5->setStartPoint(pt4);
	pLine5->setEndPoint(pt5);
	pLine5->setColorIndex(0);
	pLine5->setLayer(_T("0"));
	pLine5->setLinetypeScale(1.0);
	pBlkTblRec->appendAcDbEntity(pLine5);
	pLine5->close();

	acutPolar(asDblArray(pt4), 0, dSecondTableLength, asDblArray(pt6));
	acutPolar(asDblArray(pt5), 0, dSecondTableLength, asDblArray(pt7));
	AcDbLine* pLine6= new AcDbLine;
	pLine6->setDatabaseDefaults();
	pLine6->setStartPoint(pt6);
	pLine6->setEndPoint(pt7);
	pLine6->setColorIndex(0);
	pLine6->setLayer(_T("0"));
	pLine6->setLinetypeScale(1.0);
	pBlkTblRec->appendAcDbEntity(pLine6);
	pLine6->close();

	pBlkTblRec->close();
	return pBlkTblRec->objectId();
}

AcDbObjectId CUtility::getContentBlkRef()
{
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase(); 
	AcDbBlockTable *pBlkTable;
	AcDbObjectId blockId ; 
	AcDbObjectId blockTblRecId;
	pCurDb->getBlockTable(pBlkTable, AcDb::kForRead); 
	if (pBlkTable->getAt(_T("ZW_TABLE"), blockTblRecId) != Acad::eOk)
	{
		pBlkTable->close();
		
		blockTblRecId = createBlkRef();
		return blockTblRecId;
	}
	pBlkTable->close();
	return blockTblRecId;
}

int CUtility::getRowByPt(AcGePoint3d pt)
{
	int nCount = 0;
	CString strFrameType = CBaseUtils::frameType();
	double m_dFrameLength,m_dFrameHeight;
	m_dFrameLength = CBaseUtils::frameLength();
	m_dFrameHeight = CBaseUtils::frameHeight();
	//�ƶ�insertPt;
	double dVer = CBaseUtils::verFrameSpace();
	int nRow =abs(ceil((pt.y-m_dFrameHeight)/(m_dFrameHeight + dVer))) + 1;
	return nRow;
}


int CUtility::getRowAndColByPt(AcGePoint3d pt, int& nRow, int& nCol)
{
	int nCount = 0;
	CString strFrameType = CBaseUtils::frameType();
	double m_dFrameLength,m_dFrameHeight, dTemp;
	//�ƶ�insertPt;
	getIniFromFile(_T("A0"), m_dFrameLength, m_dFrameHeight, dTemp);
	int nHxFrameCount = CBaseUtils::hxFrameCount();

	double dVer = CBaseUtils::verFrameSpace();
	nRow = abs(ceil((pt.y-m_dFrameHeight)/(m_dFrameHeight + dVer))) + 1;
	double dHor = CBaseUtils::horFrameSpace();
	nCol = ceil((pt.x+ m_dFrameLength)/(m_dFrameLength + dHor));
	
	nCount = (nRow-1)*nHxFrameCount + nCol;
	return nCount;
}

double CUtility::getAddData(AcGePoint3d pt)
{
	int nCount = 0;
	CString strFrameType = CBaseUtils::frameType();
	double m_dFrameLength,m_dFrameHeight;
	m_dFrameLength = CBaseUtils::frameLength();
	m_dFrameHeight = CBaseUtils::frameHeight();
	double dVer = CBaseUtils::verFrameSpace();
	//�ƶ�insertPt;
	int nRow =  abs(ceil((pt.y-m_dFrameHeight)/(m_dFrameHeight + dVer))) + 1;
	double dHeight = m_dFrameHeight - (pt.y + (nRow-1)*(m_dFrameHeight + dVer));
	return dHeight;
}

CString CUtility::GetLocalUserDocPath()  
{  
	TCHAR m_lpszDefaultDir[MAX_PATH];  
	TCHAR szDocument[MAX_PATH] = {0};     
	memset(m_lpszDefaultDir, 0, _MAX_PATH);  

	LPITEMIDLIST pidl = NULL;  
	SHGetSpecialFolderLocation(NULL,   CSIDL_PERSONAL,   &pidl);  
	if   (pidl && SHGetPathFromIDList(pidl, szDocument))  
	{     
		GetShortPathName(szDocument,m_lpszDefaultDir, _MAX_PATH);  
	}  

	CString wsR = m_lpszDefaultDir;  

	return wsR;  
}  

CString CUtility::getFileNameByFullName(CString strFullName)
{
	CString strOut;
	int nFind = strFullName.ReverseFind(_T('\\'));
	if (nFind < 0)
	{
		return strFullName;
	}
	strOut = strFullName.Right(strFullName.GetLength() - nFind -1);
	return strOut;
}

CString CUtility::getFilePath()
{
	CString strBmPath = getBmPath();
	if (!PathIsDirectory(strBmPath))  
	{  
		::CreateDirectory(strBmPath, 0);  
	}  
	CString strBmNo = gGlobal.GetIniValue(_T("ͼ������"), _T("BM"));
	strBmPath = strBmPath + strBmNo + _T("\\");
	if (!PathIsDirectory(strBmPath))
	{
		::CreateDirectory(strBmPath, 0);
	}
	CString strdate = getDate();
	CString strFilePath = strBmPath + strdate + _T("\\");
	if (!PathIsDirectory(strFilePath))  
	{  
		::CreateDirectory(strFilePath, 0);  
	}  
	return strFilePath;
}

CString CUtility::getLogPath()
{
	return getBmPath();
}

CString CUtility::getBmPath()
{
	CString strOutPath;
	strOutPath = gGlobal.GetIniValue(_T("��������"), _T("Ĭ��·��"));
	if (strOutPath.IsEmpty())
	{
		strOutPath = MyBaseUtils::GetAppPath() + _T("BM���Ŀ¼\\");;
	}
	if (!PathIsDirectory(strOutPath))  
	{  
		CreateDirectoryWithUserFullControlACL(strOutPath);
	}  
	return strOutPath;
}

CString CUtility::getDate()
{
	time_t timer; 
	time(&timer); 
	tm* t_tm = localtime(&timer);  

	CString strTime;

	strTime.Format(_T("%d%d%d"), t_tm->tm_year+1900, t_tm->tm_mon + 1, t_tm->tm_mday);
	return strTime;
}

CString CUtility::getExcelPos(int nRow, int nCol)
{
	CString strOut,strTmp;
	strTmp.Format(_T("$%c"), nCol+64);
	
	strOut = strTmp;

	strTmp.Format(_T("$%d"), nRow);
	strOut += strTmp;
	return strOut;
}

CString CUtility::getFrameIniFile()
{
	CString strAppPath = MyBaseUtils::GetAppPath() + _T("frame.xml");
	return strAppPath;
}

void CUtility::UpdatePageNo(int nBegin)
{
	CString buffer = gGlobal.GetIniValue(_T("��������"), _T("2"));
	if (buffer.IsEmpty())//���Ϊ�գ�����null
	{
		return;
	}
	CString strDescription = MyParserString::SubString(buffer, _T(","), 0);
	resbuf* filter = acutBuildList(-4,_T("<and"),RTDXF0,_T("TEXT"),  7,  strDescription, -4,_T("and>"), RTNONE);
	ads_name ssname;
	int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	if (sslen < 1)
	{
		return;
	}
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	CString strBlockName;
	AcDbEntity* pEnt = NULL;
	int nCount;
	//MyBaseUtils::GetVar(_T("USERI1"), &nCount);
	nCount = CBaseUtils::frameCount() - CBaseUtils::firstCount();
	CString strCount;
	strCount.Format(_T("%d"), nCount);
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity((AcDbEntity*&)pEnt, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		if (pEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText = AcDbText::cast(pEnt);
			CString strText = pText->textString();
			int nTempNo = MyTransFunc::StringToInt(strText);
			if (nTempNo == 0)
			{
				pText->close();
				continue;
			}
			if ((nBegin > nCount) && (nBegin==nTempNo))
			{
				pText->upgradeOpen();
				nTempNo--;
				strText.Format(_T("%d"), nTempNo);
				pText->setTextString(strText);
				pText->close();
			}
			else
			{
				if (nTempNo < nBegin)
				{
					pText->close();
				}
				else
				{
					pText->upgradeOpen();
					nTempNo--;
					strText.Format(_T("%d"), nTempNo);
					pText->setTextString(strText);
					pText->close();
				}
			}
			
		}
		else
		{
			pEnt->close();
		}
	}

	acedSSFree(ssname);
}


int CUtility::ParserStringComma(CString strInput, CString& strTexing)
{
	int ndex = strInput.Find(_T(':'));	
	if (ndex <= 0)
	{
		ndex = strInput.Find(_T('��'));
	}
	strTexing = strInput.Right(strInput.GetLength() - ndex - 1);
	return ndex;
}

bool CUtility::IsIgnoreErrorFlag(CCommonCell cell)
{
	CString strText = cell.getTexing();
	if (cell.getQuxiang().IsEmpty())//���ȥ��Ϊ��
	{
		int nFind = strText.Find(_T("[D]"));
		int nIndex = strText.Find(_T("T"));
		if (strText.IsEmpty())//����Ϊ��
		{
			//������
			return false;
		}
		else if (nFind >= 0)//��D
		{
			//������
			return false;
		}
		else if (nIndex >= 0)//��T
		{
			return false;
		}
		else	
		{
			return true;
		}
	}
	return false;
}

void CUtility::EditignoreError(AcDbObjectIdArray objIdArr)
{
	int nColor = gGlobal.GetIniValue(_T("��֤����"), _T("��ɫ"), 0);
	AcDbObjectId objId;
	AcDbEntity* pEnt = NULL;
	for (int i=0; i<objIdArr.length(); i++)
	{
		objId = objIdArr.at(i);
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite) != Acad::eOk)
		{
			continue;
		}
		pEnt->setColorIndex(nColor);
		pEnt->close();
	}
}
void CUtility::DwgZoomExtent( AcDbDatabase *pDb )
{
	assert (pDb);

	// ���ģ�Ϳռ�����ʵ�����С��Χ��
	AcDbExtents ext = GetModelSpaceExtent(pDb);

	AcDbViewportTable* pViewportTable = NULL;
	if (pDb->getViewportTable(pViewportTable, AcDb::kForWrite) == Acad::eOk)
	{
		AcDbViewportTableRecord *pRecord = NULL;
		if (pViewportTable->getAt(TEXT("*ACTIVE"), pRecord, AcDb::kForWrite) == Acad::eOk)
		{
			AcGePoint3d center;
			center.x = (ext.minPoint().x + ext.maxPoint().x)/2;
			center.y = (ext.minPoint().y + ext.maxPoint().y)/2;
			center.z = (ext.minPoint().z + ext.maxPoint().z)/2;

			double height = ext.maxPoint().y - ext.minPoint().y;
			double width = ext.maxPoint().x - ext.minPoint().x;
			pRecord->setCenterPoint(center.convert2d(AcGePlane::kXYPlane));
			pRecord->setHeight(height * 1.2);
			pRecord->setWidth(width * 1.2);
			pRecord->close();
		}				
		pViewportTable->close();
	}
}

AcDbExtents CUtility::GetModelSpaceExtent( AcDbDatabase *pDb )
{
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);

	// ���ģ�Ϳռ�Ŀ���¼
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	AcDbExtents extent;
	Acad::ErrorStatus es = extent.addBlockExt(pBlkTblRcd);
	pBlkTblRcd->close();

	// ���ͼ�����ݿⲻ�ǵ�ǰ�Ĺ������ݿ⣬��ʱ��ֱ�ӻ�ȡģ�Ϳռ�ķ�Χ��ʧ��
	if (es != Acad::eOk)
	{
		AcDbObjectIdArray allEnts = GetAllEntityIds(NULL, pDb);
		for (int i = 0; i < allEnts.length(); i++)
		{
			AcDbEntity *pEnt = NULL;
			if (acdbOpenObject(pEnt, allEnts[i], AcDb::kForRead) == Acad::eOk)
			{
				AcDbExtents ext;
				if (pEnt->getGeomExtents(ext) == Acad::eOk)
				{
					extent.addExt(ext);
				}

				pEnt->close();
			}
		}
	}

	return extent;
}

AcDbObjectIdArray CUtility::GetAllEntityIds( const TCHAR* layerName, AcDbDatabase *pDb )
{
	AcDbObjectIdArray entIds;		// ����������ʵ�弯��
	bool bFilterLayer = false;			// �Ƿ���Ҫ����ͼ��
	AcDbObjectId layerId;
	// ���ָ��ͼ��Ķ���ID
	if (layerName != NULL)
	{
		AcDbLayerTable *pLayerTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLayerTbl, AcDb::kForRead);
		if (!pLayerTbl->has(layerName))
		{
			pLayerTbl->close();
			return entIds;
		}
		pLayerTbl->getAt(layerName, layerId);
		pLayerTbl->close();

		bFilterLayer = true;
	}

	// ��ÿ��
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);

	// ���ģ�Ϳռ�Ŀ���¼
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	// ���������������η���ģ�Ϳռ��ÿһ��ʵ��
	AcDbBlockTableRecordIterator *it = NULL;
	pBlkTblRcd->newIterator(it);
	for (it->start(); !it->done(); it->step())
	{
		AcDbEntity *pEnt = NULL;
		Acad::ErrorStatus es = it->getEntity(pEnt, AcDb::kForRead);
		if (es == Acad::eOk)
		{
			if (bFilterLayer)				// ����ͼ��
			{
				if (pEnt->layerId() == layerId)
				{
					entIds.append(pEnt->objectId());
				}				
			}
			else
			{
				entIds.append(pEnt->objectId());
			}

			pEnt->close();
		}
		else
		{
			acutPrintf(TEXT("\nCDwgDatabaseUtil::GetAllEntityIds�д�ʵ��ʧ��(�������:%d)."), (int)es);
		}
	}
	delete it;
	pBlkTblRcd->close();

	return entIds;
}
void CUtility::ZoomExtent()
{
	// ��õ�ǰͼ��������ʵ�����С��Χ��
	AcDbBlockTable *pBlkTbl;
	AcDbBlockTableRecord *pBlkTblRcd;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	AcDbExtents extent;		// ģ�Ϳռ�İ�Χ��
	extent.addBlockExt(pBlkTblRcd);
	pBlkTblRcd->close();

	// ��Χ���ǳ����壬�����屻�任��DCS��֮��ÿ�����㶼�п����������С�ǵ�
	AcGePoint3dArray verts;
	verts.append(extent.minPoint());
	verts.append(AcGePoint3d(extent.maxPoint().x, extent.minPoint().y, extent.minPoint().z));
	verts.append(AcGePoint3d(extent.maxPoint().x, extent.maxPoint().y, extent.minPoint().z));
	verts.append(AcGePoint3d(extent.minPoint().x, extent.maxPoint().y, extent.minPoint().z));
	verts.append(AcGePoint3d(extent.minPoint().x, extent.minPoint().y, extent.maxPoint().z));
	verts.append(AcGePoint3d(extent.maxPoint().x, extent.minPoint().y, extent.maxPoint().z));
	verts.append(extent.maxPoint());
	verts.append(AcGePoint3d(extent.minPoint().x, extent.maxPoint().y, extent.maxPoint().z));

	// ������������нǵ�ת�Ƶ�DCS��
	for (int i = 0; i < verts.length(); i++)
	{
		verts[i] = WcsToDcsPoint(verts[i]);
	}

	// ������нǵ���DCS����С�İ�Χ����
	double xmin = 1.0E30, ymin = 1.0E30;
	double xmax = -1.0E30, ymax = -1.0E30;
	for (int i = 0; i < verts.length(); i++)
	{
		xmin = min(xmin, verts[i].x);
		ymin = min(ymin, verts[i].y);
		xmax = max(xmax, verts[i].x);
		ymax = max(ymax, verts[i].y);
	}

	AcDbViewTableRecord view;
	GetCurrentView(view);

	// ������ͼ�����ĵ�
	view.setCenterPoint(AcGePoint2d((xmin + xmax) / 2, (ymin + ymax) / 2));

	// ������ͼ�ĸ߶ȺͿ��
	view.setHeight(fabs(ymax - ymin));
	view.setWidth(fabs(xmax - xmin));

	// ����ͼ��������Ϊ��ǰ��ͼ
	Acad::ErrorStatus es = acedSetCurrentView(&view, NULL);
}

void CUtility::GetCurrentView( AcDbViewTableRecord &view )
{
	struct resbuf rb;
	struct resbuf wcs, ucs, dcs;	// ת������ʱʹ�õ�����ϵͳ���

	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	// ��õ�ǰ�ӿڵ�"�鿴"ģʽ
	acedGetVar(TEXT("VIEWMODE"), &rb);
	view.setPerspectiveEnabled((rb.resval.rint & 1) != 0);
	view.setFrontClipEnabled((rb.resval.rint & 2) != 0);
	view.setBackClipEnabled((rb.resval.rint & 4) != 0);
	view.setFrontClipAtEye((rb.resval.rint & 16) == 0);

	// ��ǰ�ӿ�����ͼ�����ĵ㣨UCS���꣩
	acedGetVar(TEXT("VIEWCTR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
	view.setCenterPoint(AcGePoint2d(rb.resval.rpoint[X], 
		rb.resval.rpoint[Y]));	

	// ��ǰ�ӿ�͸��ͼ�еľ�ͷ���೤�ȣ���λΪ���ף�
	acedGetVar(TEXT("LENSLENGTH"), &rb);
	view.setLensLength(rb.resval.rreal);

	// ��ǰ�ӿ���Ŀ����λ�ã��� UCS �����ʾ��
	acedGetVar(TEXT("TARGET"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	view.setTarget(AcGePoint3d(rb.resval.rpoint[X], 
		rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// ��ǰ�ӿڵĹ۲췽��UCS��
	acedGetVar(TEXT("VIEWDIR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
	view.setViewDirection(AcGeVector3d(rb.resval.rpoint[X], 
		rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// ��ǰ�ӿڵ���ͼ�߶ȣ�ͼ�ε�λ��
	acedGetVar(TEXT("VIEWSIZE"), &rb);
	view.setHeight(rb.resval.rreal);
	double height = rb.resval.rreal;

	// ������Ϊ��λ�ĵ�ǰ�ӿڵĴ�С��X �� Y ֵ��
	acedGetVar(TEXT("SCREENSIZE"), &rb);
	view.setWidth(rb.resval.rpoint[X] / rb.resval.rpoint[Y] * height);

	// ��ǰ�ӿڵ���ͼŤת��
	acedGetVar(TEXT("VIEWTWIST"), &rb);
	view.setViewTwist(rb.resval.rreal);

	// ��ģ��ѡ������һ������ѡ���Ϊ��ǰ
	acedGetVar(TEXT("TILEMODE"), &rb);
	int tileMode = rb.resval.rint;
	// ���õ�ǰ�ӿڵı�ʶ��
	acedGetVar(TEXT("CVPORT"), &rb);
	int cvport = rb.resval.rint;

	// �Ƿ���ģ�Ϳռ����ͼ
	bool paperspace = ((tileMode == 0) && (cvport == 1)) ? true : false;
	view.setIsPaperspaceView(paperspace);

	if (!paperspace)
	{
		// ��ǰ�ӿ���ǰ�����ƽ�浽Ŀ��ƽ���ƫ����
		acedGetVar(TEXT("FRONTZ"), &rb);
		view.setFrontClipDistance(rb.resval.rreal);

		// ��õ�ǰ�ӿں������ƽ�浽Ŀ��ƽ���ƫ��ֵ
		acedGetVar(TEXT("BACKZ"), &rb);
		view.setBackClipDistance(rb.resval.rreal);
	}
	else
	{
		view.setFrontClipDistance(0.0);
		view.setBackClipDistance(0.0);
	}
}

AcGePoint3d CUtility::WcsToDcsPoint( const AcGePoint3d &point )
{
	// ת������������	
	AcGePoint3d pt;
	struct resbuf rbFrom, rbTo;
	rbFrom.restype = RTSHORT;
	rbFrom.resval.rint = 0; // from WCS
	rbTo.restype = RTSHORT;
	rbTo.resval.rint = 2; // to DCS

	acedTrans(asDblArray(point), &rbFrom, &rbTo, Adesk::kFalse, asDblArray(pt));

	return pt;
}

CString CUtility::RemoveString(CString strInput, CString strTemp)
{
	int nTmp2 = strInput.Find(strTemp);
	if (nTmp2 >= 0)
	{
		int nTmp1 = strInput.Find(_T("]"), nTmp2);
		CString strTmp = strInput.Mid(nTmp2, nTmp1-nTmp2+1);

		strInput.Replace(strTmp, _T(""));
	}
	
	return strInput;
}
BOOL CUtility::CreateDirectoryWithUserFullControlACL(LPCTSTR lpPath)
{
	if(!CreateDirectory(lpPath,NULL))
		return FALSE;

	HANDLE hDir = CreateFile(lpPath,READ_CONTROL|WRITE_DAC,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	if(hDir == INVALID_HANDLE_VALUE)
		return FALSE; 

	ACL* pOldDACL;
	SECURITY_DESCRIPTOR* pSD = NULL;
	GetSecurityInfo(hDir, SE_FILE_OBJECT , DACL_SECURITY_INFORMATION,NULL, NULL, &pOldDACL, NULL, (void**)&pSD);

	PSID pSid = NULL;
	SID_IDENTIFIER_AUTHORITY authNt = SECURITY_NT_AUTHORITY;
	AllocateAndInitializeSid(&authNt,2,SECURITY_BUILTIN_DOMAIN_RID,DOMAIN_ALIAS_RID_USERS,0,0,0,0,0,0,&pSid);

	EXPLICIT_ACCESS ea={0};
	ea.grfAccessMode = GRANT_ACCESS;
	ea.grfAccessPermissions = GENERIC_ALL;
	ea.grfInheritance = CONTAINER_INHERIT_ACE|OBJECT_INHERIT_ACE;
	ea.Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea.Trustee.ptstrName = (LPTSTR)pSid;

	ACL* pNewDACL = 0;
	DWORD err = SetEntriesInAcl(1,&ea,pOldDACL,&pNewDACL);

	if(pNewDACL)
		SetSecurityInfo(hDir,SE_FILE_OBJECT,DACL_SECURITY_INFORMATION,NULL, NULL, pNewDACL, NULL);

	FreeSid(pSid);
	LocalFree(pNewDACL);
	LocalFree(pSD);
	LocalFree(pOldDACL);
	CloseHandle(hDir);

	return TRUE;
}

void CUtility::getIniFromFile(CString strProperty, double& dX, double& dY, double& dSize)
{
	bool bRet;
	CString buffer = gGlobal.GetIniValue(_T("ͼ��������Ϣ"), strProperty);
	if (buffer.IsEmpty())
	{
		return;
	}
	CString strDescription = MyParserString::SubString(buffer, _T(","), 0);
	CString strFont = MyParserString::SubString(buffer, _T(","), 1);
	CString strBigFont = MyParserString::SubString(buffer, _T(","), 2);
	dX = MyTransFunc::StringToDouble(strDescription);
	dY = MyTransFunc::StringToDouble(strFont);
	dSize = MyTransFunc::StringToDouble(strBigFont);
}

bool CUtility::ParserStringAndGetStartAndEnd(CString strInput, int& nStart, int& nEnd)
{
	bool bRet = true;
	//����ָ����Ĭ���� ~
	int nFind = strInput.Find(_T("~"));
	CString strTmp;
	strTmp = strInput.Left(nFind);
	nStart = MyTransFunc::StringToInt(strTmp);

	strTmp = strInput.Right(strInput.GetLength()-nFind - 1);
	nEnd = MyTransFunc::StringToInt(strTmp);
	return bRet;
}

void CUtility::doIt()
{
	gQuery.draw();
	gRWriteFile.saveAllFrameInfo();
}


void CUtility::preView()
{
	gQuery.draw();
	gRWriteFile.saveAllFrameInfo();
}

void CUtility::UpdatePageNo()
{
	resbuf* filter = acutBuildList(-4,_T("<and"),RTDXF0,_T("INSERT"), 2, _T("A*"), -4,_T("and>"), RTNONE);
	ads_name ssname;
	int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	if (sslen < 1)
	{
		return;
	}
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	CString strBlockName;
	AcDbBlockReference* pBlkRef = NULL;
	int nCount;
	//MyBaseUtils::GetVar(_T("USERI1"), &nCount);
	nCount = CBaseUtils::frameCount() - 1;
	CString strCount;
	strCount.Format(_T("%d"), nCount);
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity((AcDbEntity*&)pBlkRef, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}

		MyBaseUtils::symbolIdToName(pBlkRef->blockTableRecord(), strBlockName);
		AcGePoint3d pt = pBlkRef->position();
		pBlkRef->close();
		if (strBlockName.Find(_T("A")) >= 0)
		{
			double dTextSize = 3.0;
			double dX = 0.0;
			double dY = 0.0;
			CUtility::getIniFromFile(_T("����"), dX, dY, dTextSize);
			pt.x = pt.x - dX;
			pt.y = pt.y + dY;
			pt.z = 0;
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			AcDbObjectId textStyleId = CUtility::getStyleIdByiniFile(_T("��������"), _T("2"));
			MyDrawEntity::DrawText(pt, strCount, dTextSize, textStyleId, AcDb::kTextMid);
		}

	}

	acedSSFree(ssname);
}

bool CUtility::verifyOther()
{
	bool bRet = gQuery.doIt();
	gQuery.writeErrorLog();
	return bRet;
}
