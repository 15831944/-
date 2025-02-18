//
//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright 1992-2009 by Autodesk, Inc.  All rights reserved.
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//
//////////////////////////////////////////////////////////////////////////////
//
//  DESCRIPTION:  Header for Document and Document Manager Classes.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef ACDOCMAN_H
#define ACDOCMAN_H

#include <wtypes.h>  // for HINSTANCE

#include "acdb.h"
#pragma pack (push, 8)

class AcTransactionManager;
class AcDbDatabase;
class CDocument;
class AcEdInputPointManager;
class AcApStatusBar;
class AcApPlotReactor;
class AcRxIterator;

struct IDispatch;

// Name of Registered Service Object for constructing AutoCAD-specific
// instances.
//
#define AC_DOCUMENT_MANAGER_OBJ _T("AcApDocManager")

struct AcAp {

    // The value order of this enum is important - and determines
    // the level of locking.  Also, kWrite and kProtectedAutoWrite
    // share a common bit - so that (mode & kWrite) will be true
    // for either lock mode.
    //
    enum DocLockMode   {   kNone               = 0x00,
                           kAutoWrite          = 0x01,
                           kNotLocked          = 0x02,
                           kWrite              = 0x04,
                           kProtectedAutoWrite = 0x14,
                           kRead               = 0x20,
                           kXWrite             = 0x40  };
};

class AcLispAppInfo
{
public:
    ACHAR appFileName[_MAX_PATH];
    bool bUnloadable;
};

class ADESK_NO_VTABLE AcApDocument : public AcRxObject
{
public:
    ACRX_DECLARE_MEMBERS(AcApDocument);

    // Returns the full path and filename of the database
    //
    virtual const ACHAR*       fileName() const = 0;

    // Returns the associated MFC CDocument object 
    //
    virtual CDocument* cDoc() const = 0;

    virtual AcTransactionManager* transactionManager() const = 0;

    virtual AcDbDatabase*     database() const = 0;

    // These enum numbers are not random, but correspond to the
    // following formula:
    // enum value = (Release # - 12) * 4 + (0 for dwg, 1 for dxf, 2 for dwt,
    // 3 for dws, 4 for xml).  (and xml is obsolete)
    //
     enum SaveFormat {
        kUnknown = -1,      // Invalid format
        kR12_dxf = 1,       // AutoCAD R12/LT2 DXF (*.dxf)

        kR13_dwg = 4,       // AutoCAD R13/LT95 Drawing (*.dwg)
        kR13_dxf = 5,       // AutoCAD R13/LT95 DXF (*.dxf)

        kR14_dwg = 8,       // AutoCAD R14/LT97 Drawing (*.dwg)
        kR14_dxf = 9,       // AutoCAD R14/LT97 DXF (*.dxf)

        k2000_dwg = 12,      // AutoCAD 2000 Drawing (*.dwg)
        k2000_dxf = 13,      // AutoCAD 2000 DXF (*.dxf)
        k2000_Template = 14, // 2000 Drawing Template File (*.dwt)
        k2000_Standard = 15, // 2000(+) Drawing Standard File (*.dws)
        k2000_xml = 16,      // 2000(+) = DesignXML v1.0 File (*.xml)

        k2004_dwg = 24,      // AutoCAD 2004 Drawing (*.dwg)
        k2004_dxf = 25,      // AutoCAD 2004 DXF (*.dxf)
        k2004_Template = 26, // AutoCAD 2004 Template File (*.dwt)
        k2004_Standard = 27, // 2004 Drawing Standard File (*.dws)

        k2007_dwg = 36,      // AutoCAD 2007 Drawing (*.dwg)
        k2007_dxf = 37,      // AutoCAD 2007 DXF (*.dxf)
        k2007_Template = 38, // AutoCAD 2007 Template File (*.dwt)
        k2007_Standard = 39, // 2007 Drawing Standard File (*.dws)

        k2010_dwg = 48,      // AutoCAD 2010 Drawing (*.dwg)
        k2010_dxf = 49,      // AutoCAD 2010 DXF (*.dxf)
        k2010_Template = 50, // AutoCAD 2010 Template File (*.dwt)
        k2010_Standard = 51, // 2010 Drawing Standard File (*.dws)

        kNative = k2010_dwg  // Current dwg version is 2010
     };

    virtual SaveFormat formatForSave() const = 0;

    virtual AcAp::DocLockMode lockMode(bool bIncludeMyLocks =
                                           false) const = 0;
    virtual AcAp::DocLockMode myLockMode() const = 0;

    // Is there a command, lisp, script, or ARX command active 
    virtual bool    isQuiescent() const = 0;

    // Return the document's context pointer 
    virtual void * contextPtr() const = 0;

    // Return the document's input point manager
    virtual AcEdInputPointManager* inputPointManager() = 0;

    // Return the documents Window title
    virtual const ACHAR * docTitle() const = 0;

    // Returns a pointer to the IDispatch object associated with the CDocument object
    virtual IDispatch * GetIDispatch(bool bAddRef) = 0;

    // Return TRUE if the document is read only else false
    virtual bool isReadOnly() const = 0;

    // Access to the list of loaded Lisp applications
    virtual int GetCountOfLispList() const = 0;
    virtual AcLispAppInfo* GetItemOfLispList(int nIndex) const = 0;

    // Return the drawing status bar
    virtual AcApStatusBar* drawingStatusBar() const = 0;

    // Push/pop the current value of $DBMOD.  Non-virtual is intentional.
    void              pushDbmod();
    Acad::ErrorStatus popDbmod();

    // Convert the document status from read only to write if possible.
    //
    Acad::ErrorStatus upgradeDocOpen();

    // Convert the document status from writeable to read only.
    // If bPromptForSave is true - and the drawing is modified (based on dbmod)
    // the user is prompted to save the drawing. If bPromptForSave is
    // false and the drawing has been modified, an error is returned.
    //
    Acad::ErrorStatus downgradeDocOpen(bool bPromptForSave);
};

// These two functions are to be deprecated
Acad::ErrorStatus acedUpgradeDocOpen();
Acad::ErrorStatus acedDowngradeDocOpen(bool bPromptForSave);

class ADESK_NO_VTABLE AcApDocManagerReactor : public AcRxObject
{
public:
    ACRX_DECLARE_MEMBERS(AcApDocManagerReactor);

    // Fired when a document is first instantiated no database is available.
    virtual void              documentCreateStarted(AcApDocument* pDocCreating);
    // Fired when a document has been successfully created replaces kLoadDwgMsg.
    virtual void              documentCreated(AcApDocument* pDocCreating);
    // Fired when a document is about to be destroyed replaces kUnloadDwgMsg
    virtual void              documentToBeDestroyed(AcApDocument* pDocToDestroy);
    // Fired when a document is completely destroyed
    virtual void              documentDestroyed(const ACHAR* fileName);
    // Fired when a user action has cancelled a documents creation.
    // Only fired in MDI mode and after a documentCreateStarted
    virtual void              documentCreateCanceled(AcApDocument* pDocCreateCancelled);


    virtual void              documentLockModeWillChange(AcApDocument* ,
                                         AcAp::DocLockMode myCurrentMode,
                                         AcAp::DocLockMode myNewMode,
                                         AcAp::DocLockMode currentMode,
                                         const ACHAR* pGlobalCmdName);
    virtual void              documentLockModeChangeVetoed(AcApDocument* ,
                                         const ACHAR* pGlobalCmdName);
    virtual void              documentLockModeChanged(AcApDocument* ,
                                         AcAp::DocLockMode myPreviousMode,
                                         AcAp::DocLockMode myCurrentMode,
                                         AcAp::DocLockMode currentMode,
                                         const ACHAR* pGlobalCmdName);

    virtual void              documentBecameCurrent(AcApDocument* );
 
    // This reactor is fired PRIOR to a document becoming activated NOTE: The document
    // context will not be set yet see also documentActivated()
    virtual void              documentToBeActivated(AcApDocument* pActivatingDoc );
    // Parallel to documentToBeActivated
    virtual void              documentToBeDeactivated(AcApDocument* pDeActivatedDoc );

    // Notify  of changes caused by disableDocumentActivation() or enableDocumentActivation().
    // bActivation == TRUE when document activation is enabled FALSE when disabled.
    virtual void              documentActivationModified(bool bActivation);

    // This reactor is fired once a document has been activated and its context
    // successfully switched. NOTE: There are situations where document is
    // "activated" outside the normal UI user interaction. See also documentToBeActivated
    virtual void              documentActivated(AcApDocument* pActivatedDoc);
protected:
    AcApDocManagerReactor() : mVeto(0) {}
    Acad::ErrorStatus         veto();

private:
    Adesk::Int8               mVeto;

friend class AcApDocImpManager;
};


class ADESK_NO_VTABLE AcApDocumentIterator : public AcRxObject
{
public:
    ACRX_DECLARE_MEMBERS(AcApDocumentIterator);

    virtual bool    done() const = 0;
    virtual void              step() = 0;
    virtual AcApDocument*       document() = 0;
};


class ADESK_NO_VTABLE AcApDocManager : public AcRxObject
{
public:
    ACRX_DECLARE_MEMBERS(AcApDocManager);

    // curDocument() returns the document having current context.
    // mdiActiveDocument() returns the MDI active document.
    // curDocument() and mdiActiveDocument() can be different.
    // You can call curDocument() to make a document "current" 
    // without actually activating it. After finish your AcDbDatabase
    // operation under the temporary current document, call
    // setCurDocument(acDocManager->mdiActiveDocument())
    // to reset the MDI active document as the current document.
    virtual AcApDocument*       curDocument() const = 0;
    virtual AcApDocument*       mdiActiveDocument() const = 0;

    virtual bool    isApplicationContext() const = 0;

    virtual AcApDocument*       document(const AcDbDatabase* ) const = 0;

    virtual Acad::ErrorStatus lockDocument(AcApDocument* pDoc,
                                   AcAp::DocLockMode = AcAp::kWrite,
                                   const ACHAR* pGlobalCmdName = NULL,
                                   const ACHAR* pLocalCmdName = NULL,
                                   bool prompt = true) = 0;
    virtual Acad::ErrorStatus unlockDocument(AcApDocument* pDoc) = 0;

    virtual AcApDocumentIterator* newAcApDocumentIterator() = 0;

    virtual void              addReactor(AcApDocManagerReactor* ) = 0;
    virtual void              removeReactor(AcApDocManagerReactor* ) = 0;

    virtual Acad::ErrorStatus setDefaultFormatForSave(
                                AcApDocument::SaveFormat format) = 0;

    //          Allows an external application running under the Application context
    //          to make a document "current" without actually activating it.
    //          This current state is only valid during the context of the function
    //          calling this API. This enables safe manipulation of a documents
    //          data without the document actually being the top most or
    //          active document. 
    //          The default parameters "mode" and "activate" allow document locking
    //          and activation to occur. These are shortcuts to allow several
    //          steps to be accomplished with a single API and can also
    //          be accomplished with the individual API calls.  
    virtual Acad::ErrorStatus setCurDocument(AcApDocument* pDoc,
                                   AcAp::DocLockMode = AcAp::kNone,
                                   bool activate = false) = 0;
    // Activate target document
    virtual Acad::ErrorStatus activateDocument(AcApDocument* pAcTargetDocument,
                                               bool bPassScript = false) = 0;

    // Send string to target document to execute by default
    // activate document. Allow the string to be executed as soon as
    // possible if "bActivate" is false" and "bWrapUpInactiveDoc" is true.
    // "bEchoString" determines whether the sent string is echoed on the
    // command line.
    virtual Acad::ErrorStatus sendStringToExecute(AcApDocument* pAcTargetDocument,
                                       const ACHAR * pszExecute,
                                       bool bActivate = true,
                                       bool bWrapUpInactiveDoc = false,
                                       bool bEchoString = true) = 0;    

    // These two member functions allow synchronous access to NEW OPEN
    // They may only be called from the application context 
    // See also executeInApplicationContext() These APIs only
    // function in MDI mode.
    virtual Acad::ErrorStatus appContextNewDocument(const ACHAR *pszTemplateName) = 0;
    virtual Acad::ErrorStatus appContextOpenDocument(const ACHAR *pszDrawingName) = 0;

    // Open a drawing and switch to the layout or view specified, if any.
    // This will become a pure virtual method in a future release.
    struct DocOpenParams {
        union { // drawing file name
            const ACHAR * mpszFileName;
            const wchar_t * mpwszFileName;
        };
        union { // layout or view name, or handle string
            const ACHAR * mpszViewName;
            const wchar_t * mpwszViewName;
            const double * mpCoords;    // or world coords to zoom to
        };
        enum {kUnknown = 0,             // not used
              kDefaultView = 1,         // open in last saved view
              kLayoutName = 2,          // specify layout by name
              kViewName = 3,            // specify view by name
              kLayoutHandle = 4,        // specify layout by acdb handle
              kViewHandle = 5,          // specify view by acdb handle
              kViewWorldCoords = 6};
        BYTE mnInitialViewType;
        enum {  kDisallowReadOnly       = 0x01,
                kRequireReadOnly        = 0x02,
                kFileNameArgIsUnicode   = 0x04,
                kViewNameArgIsUnicode   = 0x08,
                kUseUIOnErrors          = 0x10,
                kOwnerControlled        = 0x20, };
        int  mnFlags;       // open it as readonly, etc
        const wchar_t * mpwszPassword;
        void  * mpOwnerData;
        void  * mpUnused;
        int  mnUnused;
        const wchar_t * mpwszTitle;
    };
    // This will be enabled in the next release where we can change
    // the application binary compatibility.
    // virtual Acad::ErrorStatus appContextOpenDocument(
    //                 const DocOpenParams *pParams);

    // These three member functions provide standard, full user interaction,
    // to the NEW OPEN and CLOSE commands. They are asynchronous so if called
    // from a document context they will suspend the currently executing routine
    // until the orginal document context is re-activated. 
    virtual Acad::ErrorStatus newDocument() = 0;
    virtual Acad::ErrorStatus openDocument() = 0;
    virtual Acad::ErrorStatus closeDocument(AcApDocument* pAcTargetDocument) = 0;

    // Return the state of the documents input buffer in characters pending.
    // A return value of zero indicates the document is not "currently"
    // processing. Used only in conjunction with sendStringToExecute()
    // for automated testing purposes. Return -1 if error
    virtual int    inputPending(AcApDocument* pAcTargetDocument) = 0;

    // APIs to allow control of whether a user can activate another
    // document. May be used by commands that process for long periods of time
    // and wish to allow special cancelation checking without the chance
    // the user might be able to activate another document.
    // Aplications MUST be sure to restore activation when complete.
    // Attempts to disable or enable when already in the requested state
    // will return an error
    virtual Acad::ErrorStatus disableDocumentActivation() = 0;
    virtual Acad::ErrorStatus enableDocumentActivation() = 0;
    virtual bool isDocumentActivationEnabled() = 0;

    // Allows a void function() to be called from the application context
    virtual void executeInApplicationContext(void (*procAddr)(void *), void *pData ) const = 0;

    // Return the number of currently open documents
    virtual int documentCount() const = 0;

    // Handle setting/saving/restoring of resouce file handles
    // pushResouceHandle pushes the current resource handle onto the
    // current document's resource handle stack and makes the specified
    // handle current.  pushAcadResourceHandle pushes the current resource
    // handle onto the current document's resource handle stack and makes
    // AutoCAD's resource file current.  popResourceHandle makes the
    // resource file handle on top of the stack current and removes it from
    // the stack.


    virtual void pushResourceHandle( HINSTANCE hNewResourceHandle) = 0;
    virtual void pushAcadResourceHandle() = 0;
    virtual void popResourceHandle() = 0;
    // Causes a document waiting for input to return to the caller 
    virtual Acad::ErrorStatus sendModelessInterrupt(AcApDocument* pAcTargetDocument) = 0;
};


AcApDocManager* acDocManagerPtr();
#define acDocManager acDocManagerPtr()

inline AcApDocument* curDoc() { return ::acDocManager->curDocument(); }

#pragma pack (pop)
#endif // ACDOCMAND_H

