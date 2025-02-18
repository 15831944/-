#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "MyBaseUtils.h"
#include "AcadStr.h"


Acad::ErrorStatus
MyBaseUtils::symbolIdToName(const AcDbObjectId& symbolId, CString& name)
{
        // shouldn't happen usually, but some debugging routines
        // want to print out this info
    if (symbolId == AcDbObjectId::kNull) {
        name = _T("AcDbObjectId::kNull");
        return Acad::eNullObjectId;
    }
        // get symbol name from symbol table record
    AcDbSymbolTableRecord* tblRec;
    Acad::ErrorStatus es;
    es = acdbOpenObject(tblRec, symbolId, AcDb::kForRead);
    if (es == Acad::eOk) {
        const TCHAR* locName;
        tblRec->getName(locName);
        name = locName;
        tblRec->close();
    }
    else {
        MyBaseUtils::rxErrorMsg(es);
        es = Acad::eInvalidInput;
    }

    return es;
}


Acad::ErrorStatus
MyBaseUtils::mlineStyleIdToName(const AcDbObjectId& mlineStyleId, CString& name)
{
        // get symbol name from symbol table record
    AcDbMlineStyle* mlineStyle;
    Acad::ErrorStatus es;
    es = acdbOpenObject(mlineStyle, mlineStyleId, AcDb::kForRead);
    if (es == Acad::eOk) {
        name = mlineStyle->name();
        mlineStyle->close();
    }
    else {
        MyBaseUtils::rxErrorMsg(es);
        es = Acad::eInvalidInput;
    }

    return es;
}


Acad::ErrorStatus
MyBaseUtils::nameToSymbolId(AcRxClass* symTblClass, LPCTSTR name,
                        AcDbObjectId& recordId, AcDbDatabase* db)
{
	ASSERT(symTblClass != NULL);
	ASSERT(db != NULL);

    AcDbSymbolTable* symTbl;
    if ((symTbl = openSymbolTable(symTblClass, AcDb::kForRead, db)) == NULL)
        return Acad::eInvalidInput;

    Acad::ErrorStatus es;
    es = symTbl->getAt(name, recordId);
    symTbl->close();
    return es;
}


bool
MyBaseUtils::symbolExists(AcRxClass* symTblClass, LPCTSTR name, AcDbDatabase* db)
{
	ASSERT(symTblClass != NULL);
	ASSERT(db != NULL);

    AcDbSymbolTable* symTbl;
    if ((symTbl = openSymbolTable(symTblClass, AcDb::kForRead, db)) == NULL)
        return false;

    bool symExists = symTbl->has(name);
    symTbl->close();
    return symExists;
}


AcDbSymbolTable*
MyBaseUtils::openSymbolTable(AcRxClass* symTblClass,
                                AcDb::OpenMode mode, AcDbDatabase* db)
{
	ASSERT(symTblClass != NULL);
	ASSERT(db != NULL);

    AcDbSymbolTable* symTbl = NULL;
    Acad::ErrorStatus es;

    if (symTblClass == AcDbBlockTableRecord::desc()) {
        AcDbBlockTable* blkTbl;
        es = db->getBlockTable(blkTbl, mode);
        symTbl = blkTbl;
    }
    else if (symTblClass == AcDbDimStyleTableRecord::desc()) {
        AcDbDimStyleTable* dimTbl;
        es = db->getDimStyleTable(dimTbl, mode);
        symTbl = dimTbl;
    }
    else if (symTblClass == AcDbLayerTableRecord::desc()) {
        AcDbLayerTable* layerTbl;
        es = db->getLayerTable(layerTbl, mode);
        symTbl = layerTbl;
    }
    else if (symTblClass == AcDbLinetypeTableRecord::desc()) {
        AcDbLinetypeTable* ltypeTbl;
        es = db->getLinetypeTable(ltypeTbl, mode);
        symTbl = ltypeTbl;
    }
    else if (symTblClass == AcDbTextStyleTableRecord::desc()) {
        AcDbTextStyleTable* textTbl;
        es = db->getTextStyleTable(textTbl, mode);
        symTbl = textTbl;
    }
    else if (symTblClass == AcDbRegAppTableRecord::desc()) {
        AcDbRegAppTable* appTbl;
        es = db->getRegAppTable(appTbl, mode);
        symTbl = appTbl;
    }
    else if (symTblClass == AcDbUCSTableRecord::desc()) {
        AcDbUCSTable* ucsTbl;
        es = db->getUCSTable(ucsTbl, mode);
        symTbl = ucsTbl;
    }
    else if (symTblClass == AcDbViewTableRecord::desc()) {
        AcDbViewTable* viewTbl;
        es = db->getViewTable(viewTbl, mode);
        symTbl = viewTbl;
    }
    else if (symTblClass == AcDbViewportTableRecord::desc()) {
        AcDbViewportTable* vportTbl;
        es = db->getViewportTable(vportTbl, mode);
        symTbl = vportTbl;
    }
    else
        es = Acad::eInvalidInput;    // passed in a class type that is illegal

    ASSERT(es == Acad::eOk);    // should never fail!
    if (es != Acad::eOk) {
        MyBaseUtils::rxErrorMsg(es);
        return NULL;
    }
    else
        return symTbl;
}


Acad::ErrorStatus
MyBaseUtils::collectSymbolIds(AcRxClass* symTblClass,
                            AcDbObjectIdArray& objIds, AcDbDatabase* db)
{
	ASSERT(symTblClass != NULL);
	ASSERT(db != NULL);

    Acad::ErrorStatus retCode = Acad::eInvalidInput;

    AcDbSymbolTable* symTbl = openSymbolTable(symTblClass, AcDb::kForRead, db);
    if (symTbl != NULL) {
            // get an iterator over this symbol Table
        AcDbSymbolTableIterator* tblIter;
        if (symTbl->newIterator(tblIter) == Acad::eOk) {
                // walk table and just collect all the objIds
                // of the entries
            if (symTbl->isKindOf(AcDbLayerTable::desc()))
                static_cast<AcDbLayerTableIterator*>(tblIter)->setSkipHidden(false);
            Acad::ErrorStatus es;
            AcDbObjectId tblRecId;
            for (; !tblIter->done(); tblIter->step()) {
                es = tblIter->getRecordId(tblRecId);
                if (es == Acad::eOk)
                    objIds.append(tblRecId);
            }
            delete tblIter;
            retCode = Acad::eOk;
        }
        symTbl->close();
    }
    return retCode;
}


Acad::ErrorStatus
MyBaseUtils::addNewSymbolAndClose(AcDbSymbolTable* symTbl,
                                        AcDbSymbolTableRecord* newRecord)
{
    Acad::ErrorStatus es;

    es = symTbl->add(newRecord);
    if (es == Acad::eOk)
        newRecord->close();
    else
        delete newRecord;
    
    return es;
}


Acad::ErrorStatus
MyBaseUtils::addNewLayer(LPCTSTR layerName, AcDbDatabase* db)
{
	ASSERT(db != NULL);

        // if layer already exists, then just return
    AcDbLayerTable* layTbl;
	Acad::ErrorStatus es = db->getSymbolTable(layTbl, AcDb::kForRead);
	if (es != Acad::eOk)
		return es;

    if (layTbl->has(layerName)) {
        layTbl->close();
        return Acad::eOk;
    }
        // upgrade to write
    es = layTbl->upgradeOpen();
    if (es != Acad::eOk) {
        ASSERT(0);
        layTbl->close();
        return es;
    }
        // make sure the name gets set ok
    AcDbLayerTableRecord* newRec = new AcDbLayerTableRecord;
    es = newRec->setName(layerName);
    if (es != Acad::eOk) {
        delete newRec;
        layTbl->close();
        return Acad::eInvalidInput;
    }
        // look up value for default linetype CONTINUOUS,
        // AcDbLayerTableRecord doesn't set this automatically (at least it didn't in Sedona)
    newRec->setLinetypeObjectId(db->continuousLinetype());

    es = layTbl->add(newRec);
    if (es != Acad::eOk)
        delete newRec;
    else
        newRec->close();

    layTbl->close();
    return es;
}


Acad::ErrorStatus
MyBaseUtils::defineNewBlock(LPCTSTR blkName, AcDbBlockTableRecord*& newBlkRec,
                            AcDbObjectId& newBlkRecId, AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcDbBlockTable* blkTbl;
	Acad::ErrorStatus es = db->getSymbolTable(blkTbl, AcDb::kForWrite);
    if (es != Acad::eOk)
        return es;

        // if this block already exists, erase its contents first
    if (blkTbl->getAt(blkName, newBlkRec, AcDb::kForWrite) == Acad::eOk) {
        newBlkRecId = newBlkRec->objectId();
        AcDbBlockTableRecordIterator* iter;
        es = newBlkRec->newIterator(iter);
        if (es != Acad::eOk) {
            MyBaseUtils::rxErrorMsg(es);
            newBlkRec->close();
        }
        else {
            AcDbEntity* ent;
            for (; !iter->done(); iter->step()) {
                if (iter->getEntity(ent, AcDb::kForWrite) == Acad::eOk) {
                    ent->erase();
                    ent->close();
                }
            }
            delete iter;
        }
    }
    else {    // create a new block table record and add it to the block table
        newBlkRec = new AcDbBlockTableRecord;
        newBlkRec->setPathName(AcadString::nullStr);    // constructor doesn't do it properly

        es = newBlkRec->setName(blkName);
        if (es == Acad::eOk)
            es = blkTbl->add(newBlkRecId, newBlkRec);

        if (es != Acad::eOk) {
            MyBaseUtils::rxErrorMsg(es);
            delete newBlkRec;
        }
    }

    blkTbl->close();    // doesn't need to be open anymore
    return es;
}


Acad::ErrorStatus
MyBaseUtils::defineNewBlock(LPCTSTR blkName, AcDbBlockTableRecord*& newBlkRec,
                            AcDbDatabase* db)
{
    AcDbObjectId newBlkRecId;
    return defineNewBlock(blkName, newBlkRec, newBlkRecId, db);
}


Acad::ErrorStatus
MyBaseUtils::defineNewBlock(LPCTSTR blkName, AcDbObjectId& newBlkRecId, AcDbDatabase* db)
{
    AcDbBlockTableRecord* newBlkRec;
    Acad::ErrorStatus es = defineNewBlock(blkName, newBlkRec, newBlkRecId, db);
    if (es == Acad::eOk)
        newBlkRec->close();
    return es;
}


Acad::ErrorStatus
MyBaseUtils::defineNewAnonymousBlock(AcDbBlockTableRecord*& newBlkRec,
                                AcDbObjectId& newBlkRecId, AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcDbBlockTable* blkTbl;
	Acad::ErrorStatus es = db->getSymbolTable(blkTbl, AcDb::kForWrite);
    if (es != Acad::eOk)
        return es;

    newBlkRec = new AcDbBlockTableRecord;
    newBlkRec->setPathName(AcadString::nullStr);    // constructor doesn't do it properly

    es = newBlkRec->setName(_T("*U"));
    if (es == Acad::eOk)
        es = blkTbl->add(newBlkRecId, newBlkRec);

    if (es != Acad::eOk) {    // make sure everything went ok
        MyBaseUtils::rxErrorMsg(es);
        delete newBlkRec;
        newBlkRec = NULL;    // don't let caller get bad value
    }

    blkTbl->close();    // doesn't need to be open anymore
    return es;
}


Acad::ErrorStatus
MyBaseUtils::defineNewAnonymousBlock(AcDbBlockTableRecord*& newBlkRec, AcDbDatabase* db)
{
    AcDbObjectId newBlkRecId;
    return defineNewAnonymousBlock(newBlkRec, newBlkRecId, db);
}


Acad::ErrorStatus
MyBaseUtils::defineNewAnonymousBlock(AcDbObjectId& newBlkRecId, AcDbDatabase* db)
{
    AcDbBlockTableRecord* newBlkRec;
    Acad::ErrorStatus es = defineNewAnonymousBlock(newBlkRec, newBlkRecId, db);
    if (es == Acad::eOk)
        newBlkRec->close();
    return es;
}


Acad::ErrorStatus
MyBaseUtils::openBlockDef(LPCTSTR blkName, AcDbBlockTableRecord*& blkRec,
                                    AcDb::OpenMode mode, AcDbDatabase* db)
{
    ASSERT(blkName != NULL);
	ASSERT(db != NULL);

    AcDbBlockTable* blkTbl;
	Acad::ErrorStatus es = db->getSymbolTable(blkTbl, AcDb::kForRead);
    if (es != Acad::eOk)
        return es;

        // open named block
    es = blkTbl->getAt(blkName, blkRec, mode);
    if (es != Acad::eKeyNotFound)
        MyBaseUtils::rxErrorMsg(es);        // print error message if its an unexpected error

    blkTbl->close();    // doesn't need to be open anymore
    return es;
}


AcDbBlockTableRecord*
MyBaseUtils::openCurrentSpaceBlock(AcDb::OpenMode mode, AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcDbBlockTableRecord* blkRec;

    Acad::ErrorStatus es = acdbOpenObject(blkRec, db->currentSpaceId(), mode);
    if (es != Acad::eOk)
        return NULL;
    else
        return blkRec;
}


Acad::ErrorStatus
MyBaseUtils::addToCurrentSpace(AcDbEntity* newEnt)
{
	return addToCurrentSpace(newEnt, acdbHostApplicationServices()->workingDatabase());
}


Acad::ErrorStatus
MyBaseUtils::addToCurrentSpace(AcDbEntity* newEnt, AcDbDatabase* db)
{
    ASSERT(newEnt != NULL);
	ASSERT(db != NULL);

    AcDbBlockTableRecord* blkRec = openCurrentSpaceBlock(AcDb::kForWrite, db);

    ASSERT(blkRec != NULL);

    if (blkRec == NULL)
        return Acad::eInvalidInput;

        // append new entity to current space block
    Acad::ErrorStatus es = blkRec->appendAcDbEntity(newEnt);
    if (es != Acad::eOk) {
        acutPrintf(_T("\nERROR: could not add entity to current space (%s)"), MyBaseUtils::rxErrorStr(es));
    }

    blkRec->close();
    return es;
}


Acad::ErrorStatus
MyBaseUtils::addToCurrentSpaceAndClose(AcDbEntity* newEnt, AcDbDatabase* db)
{
    Acad::ErrorStatus es;

    es = addToCurrentSpace(newEnt, db);
    if (es == Acad::eOk)
        newEnt->close();
    else
        delete newEnt;

    return es;
}


Acad::ErrorStatus
MyBaseUtils::addToCurrentSpaceAndClose(AcDbEntity* newEnt)
{
	return addToCurrentSpaceAndClose(newEnt, acdbHostApplicationServices()->workingDatabase());
}


bool
MyBaseUtils::isOnLockedLayer(AcDbObjectId& id, bool printMsg)
{
    AcDbEntity* ent;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(ent, id, AcDb::kForRead);
    if (es == Acad::eOk) {
        bool result = isOnLockedLayer(ent, printMsg);
        ent->close();
        return result;
    }

    return true;  // by default we "lock" data
}


bool
MyBaseUtils::isOnLockedLayer(AcDbEntity* ent, bool printMsg)
{
    AcDbObject* obj;
    AcDbLayerTableRecord* layer;
    bool isLocked = false;
    Acad::ErrorStatus es;

    es = acdbOpenAcDbObject(obj, ent->layerId(), AcDb::kForRead);
    if (es == Acad::eOk) {
        layer = AcDbLayerTableRecord::cast(obj);
        if (layer)
            isLocked = layer->isLocked();
        else {
            ASSERT(0);
        }
        obj->close();
    }
    else {
        ASSERT(0);
        MyBaseUtils::rxErrorMsg(es);
    }

    if (isLocked && printMsg) {
        acutPrintf(_T("\nSelected entity is on a locked layer."));
    }

    return isLocked;
}


Acad::ErrorStatus MyBaseUtils::AddtoModelSpace(AcDbEntity*pEnt, AcDbObjectId&retId, AcDbDatabase* pDb/* =NULL */)
{
	Acad::ErrorStatus es;
	AcDbBlockTable* pBlockTable;	//块表指针
	if(pDb==NULL)
		pDb=acdbCurDwg();

	es=pDb->getBlockTable(pBlockTable,AcDb::kForRead);	
	if(es!=Acad::eOk)
		return es;

	AcDbBlockTableRecord* pBlockTableRecord;	//块表记录指针
	es=pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord,
		AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		pBlockTable->close();
		return es;
	}
	pBlockTable->close();	//关闭块表指针

	es=pBlockTableRecord->appendAcDbEntity(retId,pEnt);
	if(es!=Acad::eOk)
	{
		pBlockTableRecord->close();
		return es;
	}

	pBlockTableRecord->close();	//关闭模型空间块表记录指针
	return Acad::eOk;
}

Acad::ErrorStatus MyBaseUtils::AddtoModelSpaceAndClose(AcDbEntity*pEnt, AcDbObjectId&retId, AcDbDatabase* pDb/* =NULL */)
{
	Acad::ErrorStatus es;
	AcDbBlockTable* pBlockTable;	//块表指针
	if(pDb==NULL)
		pDb=acdbCurDwg();

	es=pDb->getBlockTable(pBlockTable,AcDb::kForRead);	
	if(es!=Acad::eOk)
		return es;

	AcDbBlockTableRecord* pBlockTableRecord;	//块表记录指针
	es=pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord,
		AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		pBlockTable->close();
		return es;
	}
	pBlockTable->close();	//关闭块表指针

	es=pBlockTableRecord->appendAcDbEntity(retId,pEnt);
	if(es!=Acad::eOk)
	{
		pBlockTableRecord->close();
		return es;
	}

	pBlockTableRecord->close();	//关闭模型空间块表记录指针
	pEnt->close();
	return Acad::eOk;
}

