#ifndef AD_ACDB_H
#define AD_ACDB_H 1
// 
// (C) Copyright 1993-2009 by Autodesk, Inc. 
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
//
// acdb.h:   Definitions for AutoCAD Database Library which are
//           shared between Internal and 3rd party Developers

#include "adesk.h"
#include "AdAChar.h"    // for ACRX_T()
#include "rxobject.h"
#include "gepnt3d.h"

#ifdef ACAD_OBJID_INLINE_INTERNAL
#    include "dbidinln.h"
#else
#    include "dbidapps.h"
#endif

#include "dbboiler.h"

// String Constants
//
#define ACDB_SERVICES                  ACRX_T(/*MSG0*/"AcDbServices")
#define ACDB_MODEL_SPACE               ACRX_T(/*MSG0*/"*Model_Space")
#define ACDB_PAPER_SPACE               ACRX_T(/*MSG0*/"*Paper_Space")
#define ACDB_NULL_HANDLE               ACRX_T(/*MSG0*/"\0\0\0\0\0\0\0")
#define ACDB_BROKEN_HANDLE             ACRX_T(/*MSG0*/"FFFFFFFF")
#define ACDB_OPEN_BRACE_STR            ACRX_T(/*MSG0*/"{")
#define ACDB_OPEN_BRACE_CHAR           ACRX_T(/*MSG0*/'{')
#define ACDB_CLOSE_BRACE_STR           ACRX_T(/*MSG0*/"}")
#define ACDB_CLOSE_BRACE_CHAR          ACRX_T(/*MSG0*/'}')
#define ACDB_GROUP_DICTIONARY          ACRX_T(/*MSG0*/"ACAD_GROUP")
#define ACDB_MLINESTYLE_DICTIONARY     ACRX_T(/*MSG0*/"ACAD_MLINESTYLE")
#define ACDB_LAYOUT_DICTIONARY         ACRX_T(/*MSG0*/"ACAD_LAYOUT")
#define ACDB_PLOTSETTINGS_DICTIONARY   ACRX_T(/*MSG0*/"ACAD_PLOTSETTINGS")
#define ACDB_PLOTSTYLENAME_DICTIONARY  ACRX_T(/*MSG0*/"ACAD_PLOTSTYLENAME")
#define ACDB_MATERIAL_DICTIONARY       ACRX_T(/*MSG0*/"ACAD_MATERIAL")
#define ACDB_VISUALSTYLE_DICTIONARY    ACRX_T(/*MSG0*/"ACAD_VISUALSTYLE")
#define ACDB_COLOR_DICTIONARY          ACRX_T(/*MSG0*/"ACAD_COLOR")
#define ACDB_TABLESTYLE_DICTIONARY     ACRX_T(/*MSG0*/"ACAD_TABLESTYLE")
#define ACDB_EMBEDDED_OBJECT_STR       ACRX_T(/*MSG0*/"Embedded Object")
#define ACDB_LAYERSTATES_DICTIONARY    ACRX_T(/*MSG0*/"ACAD_LAYERSTATES")
#define ACDB_FIELD_DICTIONARY          ACRX_T(/*MSG0*/"ACAD_FIELD")
#define ACDB_FIELDLIST                 ACRX_T(/*MSG0*/"ACAD_FIELDLIST")
#define ACDB_ENHANCED_BLOCK            ACRX_T(/*MSG0*/"ACAD_ENHANCEDBLOCK")
#define ACDB_ENHANCED_BLOCK_EVALKEY    ACRX_T(/*MSG0*/"ACAD_ENHANCEDBLOCKEVALKEY")
#define ACDB_ENHANCED_BLOCK_REF        ACRX_T(/*MSG0*/"ACAD_ENHANCEDBLOCK_REFERENCE")
#define ACDB_SECTION_MANAGER           ACRX_T(/*MSG0*/"ACAD_SECTION_MANAGER")
#define ACDB_DATALINK                  ACRX_T(/*MSG0*/"ACAD_DATALINK")
#define ACDB_ANNOTATIONSCALES_COLLECTION ACRX_T(/*MSG0*/"ACDB_ANNOTATIONSCALES")
#define ACDB_MLEADERSTYLE_DICTIONARY   ACRX_T(/*MSG0*/"ACAD_MLEADERSTYLE")
#define ACDB_ASSOCNETWORK_DICTIONARY   ACRX_T(/*MSG0*/"ACAD_ASSOCNETWORK")

// struct Acad has been moved to the new header file acadstrc.h
//
#include "acadstrc.h"

#pragma pack(push, 8)

struct AcDb
{
    enum OpenMode          { kForRead           = 0,
                             kForWrite          = 1,
                             kForNotify         = 2 };

    enum CoordAxis         { kX                 = 0,
                             kY                 = 1,
                             kZ                 = 2 };

    enum CoordSystem       { kWorldCS           = 0,
                             kUserCS            = 1,
                             kCurDisplayCS      = 2,
                             kPaperDisplayCS    = 3,
                             kEntityCS          = 4 };

    enum Intersect         { kOnBothOperands    = 0,
                             kExtendThis        = 1,
                             kExtendArg         = 2,
                             kExtendBoth        = 3 };

    enum Visibility        { kInvisible         = 1,
                             kVisible           = 0 };

    typedef Adesk::Int16 DxfCode;
    enum                   { kDxfInvalid        = -9999,
                             kDxfXDictionary     = -6,
                             kDxfPReactors       = -5,
                             kDxfOperator        = -4,
                             kDxfXDataStart      = -3,
                             kDxfHeaderId        = -2,
                             kDxfFirstEntId      = -2,
                             kDxfEnd             = -1,
                             kDxfStart           = 0,
                             kDxfText            = 1,
                             kDxfXRefPath        = 1,
                             kDxfShapeName       = 2,
                             kDxfBlockName       = 2,
                             kDxfAttributeTag    = 2,
                             kDxfSymbolTableName = 2,
                             kDxfMstyleName      = 2,
                             kDxfSymTableRecName = 2,
                             kDxfAttributePrompt = 3,
                             kDxfDimStyleName    = 3,
                             kDxfLinetypeProse   = 3,
                             kDxfTextFontFile    = 3,
                             kDxfDescription     = 3,
                             kDxfDimPostStr      = 3,
                             kDxfTextBigFontFile = 4,
                             kDxfDimAPostStr     = 4,
                             kDxfCLShapeName     = 4,
                             kDxfSymTableRecComments = 4,
                             kDxfHandle          = 5,
                             kDxfDimBlk          = 5,
                             kDxfDimBlk1         = 6,
                             kDxfLinetypeName    = 6,
                             kDxfDimBlk2         = 7,
                             kDxfTextStyleName   = 7,
                             kDxfLayerName       = 8,
                             kDxfCLShapeText     = 9,

                             kDxfXCoord         = 10,

                             kDxfYCoord         = 20,

                             kDxfZCoord         = 30,

                             kDxfElevation      = 38,
                             kDxfThickness      = 39,

                             kDxfReal           = 40,
                             kDxfViewportHeight = 40,
                             kDxfTxtSize        = 40,
                             kDxfTxtStyleXScale = 41,
                             kDxfViewWidth      = 41,
                             kDxfViewportAspect = 41,
                             kDxfTxtStylePSize  = 42,
                             kDxfViewLensLength = 42,
                             kDxfViewFrontClip  = 43,
                             kDxfViewBackClip   = 44,
                             kDxfShapeXOffset   = 44,
                             kDxfShapeYOffset   = 45,
                             kDxfViewHeight     = 45,
                             kDxfShapeScale     = 46,
                             kDxfPixelScale     = 47,

                             kDxfLinetypeScale  = 48,

                             kDxfDashLength     = 49,
                             kDxfMlineOffset    = 49,
                             kDxfLinetypeElement = 49,

                             kDxfAngle          = 50,
                             kDxfViewportSnapAngle = 50, // deprecated
                             kDxfViewportTwist  = 51,

                             kDxfVisibility          = 60,
                             kDxfViewportGridDisplay = 60,
                             kDxfLayerLinetype       = 61,
                             kDxfViewportGridMajor   = 61,
                             kDxfColor               = 62,
                             // Removed codes intended
                             // only for internal
                             // use:  63-65
                             kDxfHasSubentities = 66,
                             kDxfViewportVisibility = 67,
                             kDxfViewportActive = 68,
                             kDxfViewportNumber = 69,

                             kDxfInt16          = 70,
                             kDxfViewMode       = 71,
                             kDxfCircleSides    = 72,
                             kDxfViewportZoom   = 73,
                             kDxfViewportIcon   = 74,
                             kDxfViewportSnap   = 75,
                             kDxfViewportGrid   = 76,
                             kDxfViewportSnapStyle= 77,
                             kDxfViewportSnapPair= 78,

                             kDxfRegAppFlags    = 71,

                             kDxfTxtStyleFlags  = 71,
                             kDxfLinetypeAlign  = 72,
                             kDxfLinetypePDC    = 73,

                             kDxfInt32          = 90,
                             kDxfVertexIdentifier = 91,
                             // Subclass Section Marker
                             //
                             // to be followed by subclass name.
                             //
                             kDxfSubclass            = 100,
                             kDxfEmbeddedObjectStart = 101,
                             kDxfControlString       = 102,

                             // DimVarTableRecords have been using 5 for a
                             // string value.  With R13, they get a handle
                             // value as well.  Since 5 is already in use,
                             // we use 105 for this special case.
                             //
                             kDxfDimVarHandle = 105,

                             kDxfUCSOrg         = 110,
                             kDxfUCSOriX        = 111,
                             kDxfUCSOriY        = 112,

                             kDxfXReal          = 140,
                             kDxfViewBrightness = 141,
                             kDxfViewContrast   = 142,

                             // 64-bit integers can only be used with
                             // AcDbDwgVersion kDHL_1024 and higher.
                             //
                             kDxfInt64          = 160,

                             kDxfXInt16         = 170,
                             // 180 - 189 cannot be used

                             // 190-199 are invalid

                             kDxfNormalX        = 210,
                             kDxfNormalY        = 220,
                             kDxfNormalZ        = 230,

                             // 260-269 are invalid

                             kDxfXXInt16        = 270,

                             kDxfInt8                = 280,
                             kDxfRenderMode          = 281,
                             kDxfDefaultLightingType = 282,
                             kDxfShadowFlags         = 284,

                             kDxfBool                = 290,
                             kDxfDefaultLightingOn   = 292,

                             //  More string values 300-309
                             kDxfXTextString   = 300,

                             //  Arbitrary Binary Chunks 310-319
                             kDxfBinaryChunk   = 310,

                             //  Arbitrary Object Handles 320-329
                             kDxfArbHandle     = 320,

                             kDxfSoftPointerId    = 330, // 330-339
                             kDxfViewBackgroundId = 332, // softPointer to background of viewport and viewporttable record
                             kDxfShadePlotId      = 333, // softPointer to shade plot visual style or render preset
                             kDxfLiveSectionId      = 334, // softPointer to LiveSection of view, viewport and viewporttable record
                             kDxfLiveSectionName    = 309, // LiveSection Name

                             kDxfHardPointerId    = 340, // 340-349
                             kDxfObjVisualStyleId = 345,
                             kDxfVpVisualStyleId = 346,
                             kDxfMaterialId       = 347, // hardpointer reference to AcDbMaterial
                             kDxfVisualStyleId    = 348, // hardpointer reference to visual style
                             kDxfDragVisualStyleId = 349, // hardpointer reference to visual style

                             kDxfSoftOwnershipId  = 350, // 350-359

                             kDxfHardOwnershipId  = 360, // 360-369
                             kDxfSunId            = 361, // hardownership reference to sun object

                             // New base entity properties
                             // Lineweight is either an integer
                             // or "BYLAYER" or "BYBLOCK"
                             kDxfLineWeight        = 370,
                             kDxfPlotStyleNameType = 380,
                             kDxfPlotStyleNameId   = 390,
                             kDxfXXXInt16          = 400,

                             kDxfLayoutName     = 410,

                             // Extended color information for base entities

                             kDxfColorRGB       = 420,
                             kDxfColorName      = 430,

                             // New base entity property Alpha is an integer
                             kDxfAlpha          = 440,

                             kDxfGradientObjType  = 450,
                             kDxfGradientPatType  = 451,
                             kDxfGradientTintType = 452,
                             kDxfGradientColCount = 453,
                             kDxfGradientAngle    = 460,
                             kDxfGradientShift    = 461,
                             kDxfGradientTintVal  = 462,
                             kDxfGradientColVal   = 463,
                             kDxfGradientName     = 470,

                             kDxfFaceStyleId = 480,
                             kDxfEdgeStyleId = 481,

                             kDxfComment        = 999,

                             kDxfXdAsciiString  = 1000,
                             kDxfRegAppName     = 1001,
                             kDxfXdControlString = 1002,
                             kDxfXdLayerName    = 1003,
                             kDxfXdBinaryChunk  = 1004,
                             kDxfXdHandle       = 1005,

                             kDxfXdXCoord       = 1010,
                             kDxfXdYCoord       = 1020,
                             kDxfXdZCoord       = 1030,

                             kDxfXdWorldXCoord  = 1011,
                             kDxfXdWorldYCoord  = 1021,
                             kDxfXdWorldZCoord  = 1031,

                             kDxfXdWorldXDisp   = 1012,
                             kDxfXdWorldYDisp   = 1022,
                             kDxfXdWorldZDisp   = 1032,

                             kDxfXdWorldXDir    = 1013,
                             kDxfXdWorldYDir    = 1023,
                             kDxfXdWorldZDir    = 1033,

                             kDxfXdReal         = 1040,
                             kDxfXdDist         = 1041,
                             kDxfXdScale        = 1042,

                             kDxfXdInteger16    = 1070,
                             kDxfXdInteger32    = 1071 };

    // Primitives.
    //
    enum DwgDataType       { kDwgNull            = 0,
                             kDwgReal            = 1,
                             kDwgInt32           = 2,
                             kDwgInt16           = 3,
                             kDwgInt8            = 4,
                             kDwgText            = 5,
                             kDwgBChunk          = 6,
                             kDwgHandle          = 7,
                             kDwgHardOwnershipId = 8,
                             kDwgSoftOwnershipId = 9,
                             kDwgHardPointerId   = 10,
                             kDwgSoftPointerId   = 11,
                             kDwg3Real           = 12,
                             kDwgInt64           = 13,
                             kDwgNotRecognized   = 19 };

    // OsnapMask:  Specifies Osnap Operation.
    //
    enum OsnapMask         { kOsMaskEnd          = 1,
                             kOsMaskMid          = 2,
                             kOsMaskCen          = 4,
                             kOsMaskNode         = 8,
                             kOsMaskQuad         = 0x10,
                             kOsMaskInt          = 0x20,
                             kOsMaskIns          = 0x40,
                             kOsMaskPerp         = 0x80,
                             kOsMaskTan          = 0x100,
                             kOsMaskNear         = 0x200,
                             kOsMaskQuick        = 0x400,
                             kOsMaskApint        = 0x800,
                             kOsMaskImmediate    = 0x10000,
                             kOsMaskAllowTan     = 0x20000,
                             kOsMaskDisablePerp  = 0x40000,
                             kOsMaskRelCartesian = 0x80000,
                             kOsMaskRelPolar     = 0x100000,
                             kOsMaskNoneOverride = 0x200000 };

    // OsnapMode:  Specifies Object query.
    //             Binary and Virtual Operations not represented.
    //
    enum OsnapMode         { kOsModeEnd          = 1,
                             kOsModeMid          = 2,
                             kOsModeCen          = 3,
                             kOsModeNode         = 4,
                             kOsModeQuad         = 5, 
                             kOsModeIns          = 7,
                             kOsModePerp         = 8,
                             kOsModeTan          = 9,
                             kOsModeNear         = 10 };

    enum ShowImagesMode    { kNeverShow     = 0,
                                                 kAlwaysShow      = 1,
                             kBoundingBoxShow = 2 };

    enum SaveImagesMode    { kNeverSave     = 0,
                                                 kAlwaysSave      = 1 };

    enum TextHorzMode      { kTextLeft    = 0,   // TH_LEFT,
                             kTextCenter  = 1,   // TH_CENT,
                             kTextRight   = 2,   // TH_RIGHT,
                             kTextAlign   = 3,   // THV_ALIGN,
                             kTextMid     = 4,   // THV_MID,
                             kTextFit     = 5 }; // THV_FIT
 
    enum TextVertMode      { kTextBase    = 0,   // TV_BASE,
                             kTextBottom  = 1,   // TV_BOT,
                             kTextVertMid = 2,   // TV_MID,
                             kTextTop     = 3 }; // TV_TOP

    enum LineSpacingStyle  { kAtLeast = 1,
                             kExactly = 2 };

    enum Planarity         { kNonPlanar = 0,
                             kPlanar    = 1,
                             kLinear    = 2 };

    enum PointIndex        { kMPoint1    = 0,
                             kMPoint2    = 1,
                             kMPoint3    = 2,
                             kMPoint4    = 3 };

    enum EdgeIndex         { kMEdge1    = 0,
                             kMEdge2    = 1,
                             kMEdge3    = 2,
                             kMEdge4    = 3 };

    enum Poly2dType        { k2dSimplePoly      = 0,
                             k2dFitCurvePoly    = 1,
                             k2dQuadSplinePoly  = 2,
                             k2dCubicSplinePoly = 3 };

    enum Poly3dType        { k3dSimplePoly      = 0,
                             k3dQuadSplinePoly  = 1,
                             k3dCubicSplinePoly = 2 };

    enum PolyMeshType      { kSimpleMesh        = 0,
                             kQuadSurfaceMesh   = 5,
                             kCubicSurfaceMesh  = 6,
                             kBezierSurfaceMesh = 8 };

    enum Vertex2dType      { k2dVertex          = 0,
                             k2dSplineCtlVertex = 1,
                             k2dSplineFitVertex = 2,
                             k2dCurveFitVertex =  3 };

    enum Vertex3dType      { k3dSimpleVertex    = 0,
                             k3dControlVertex   = 1,
                             k3dFitVertex       = 2 };

    enum SpaceValueType    { kUndefined,
                             kStubPtr,
                             kEndOfFile,
                             kBackwardBranch,
                             kForwardBranch,
                             kDetour };

    enum BoolOperType      { kBoolUnite      = 0, 
                             kBoolIntersect  = 1, 
                             kBoolSubtract   = 2 };

    typedef Adesk::UInt32    SubentType;
    enum                   { kNullSubentType   = 0,
                             kFaceSubentType   = 1,
                             kEdgeSubentType   = 2,
                             kVertexSubentType = 3, 

////////////////////////////////////////////////////////////
                                                          //
                   // Mline specific type                 //
                                                          //
                             kMlineSubentCache = 4,       //  Beauty is in the eye
                                                          //   of the beholder.
                                                          //
////////////////////////////////////////////////////////////
                             kClassSubentType  = 5
};

    typedef Adesk::UInt32    GeomId;
    enum                   { kNoGeom = 0 };

    enum GeomType          { kInvalidGeom = 0,
                             kPointGeom,
                             kLineGeom,
                             kCircleGeom,
                             kPlaneGeom };

    enum FilerType         { kFileFiler     = 0,
                             kCopyFiler     = 1,
                             kUndoFiler     = 2,
                             kBagFiler      = 3,
                             kIdXlateFiler      = 4,
                             kPageFiler         = 5,
                             kDeepCloneFiler    = 6,
                             kIdFiler           = 7,
                             kPurgeFiler        = 8,
                             kWblockCloneFiler  = 9 };

    enum FilerSeekType     { kSeekFromStart   = 0,
                             kSeekFromCurrent = 1,
                             kSeekFromEnd     = 2 };

    enum AcDbDwgVersion    { kDHL_MC0_0   = 0,
                             kDHL_AC1_2   = 1,
                             kDHL_AC1_40  = 2,
                             kDHL_AC1_50  = 3,
                             kDHL_AC2_20  = 4,
                             kDHL_AC2_10  = 5,
                             kDHL_AC2_21  = 6,
                             kDHL_AC2_22  = 7,
                             kDHL_1001    = 8,
                             kDHL_1002    = 9,      // AutoCAD 2.5
                             kDHL_1003    = 10,     // AutoCAD 2.6
                             kDHL_1004    = 11,     // Release 9
                             kDHL_1005    = 12,
                             kDHL_1006    = 13,     // Release 10
                             kDHL_1007    = 14,
                             kDHL_1008    = 15,
                             kDHL_1009    = 16,     // R11 and R12
                             kDHL_1010    = 17,
                             kDHL_1011    = 18,
                             kDHL_1012    = 19,     // R13
                             kDHL_1013    = 20,     // R14 mid version.
                             kDHL_1014    = 21,     // R14 final version.
                             kDHL_1500    = 22,     // 2000 development (Tahoe)
                             kDHL_1015    = 23,     // 2000 final version.
                             kDHL_1800a   = 24,     // 2004 mid version
                             kDHL_1800    = 25,     // 2004 final
                             kDHL_2100a   = 26,     // 2007 Development
                             kDHL_1021    = 27,     // 2007 final
                             kDHL_2400a   = 28,     // Gator Development
                             kDHL_1024    = 29,     // 2010 final
                             kDHL_Newest  = kDHL_1024,
                             kDHL_CURRENT = kDHL_Newest,
                             kDHL_Unknown = 32766,
                             kDHL_Max     = 32767 };

    // kDHL_1012, kMRelease0 = R13c0-3
    // kDHL_1012, kMRelease1 = R13c0-3
    // kDHL_1012, kMRelease4 = R13c4
    // kDHL_1012, kMRelease5 = R13c4_m
    // kDHL_1012, kMRelease6 = R13c4a

    // kDHL_1013, kMRelease0 = Sedona s000..s045
    // kDHL_1013, kMRelease1 = Sedona s046..s050
    // kDHL_1013, kMRelease2 = Sedona s051..s052
    // kDHL_1013, kMRelease3 = Sedona s053..s054
    // kDHL_1013, kMRelease4 = Sedona s055..s059
    // kDHL_1013, kMRelease5 = Sedona s060..s063
    // kDHL_1013, kMRelease6 = Sedona s064..
    // kDHL_1014, kMRelease0 = R14.0

    // kDHL_1500, kMRelease0  = Tahoe t010..t016
    // kDHL_1500, kMRelease1  = Tahoe t017
    // kDHL_1500, kMRelease2  = Tahoe t018, t019
    // kDHL_1500, kMRelease3  = Tahoe t020
    // kDHL_1500, kMRelease4  = Tahoe t021..t023
    // kDHL_1500, kMRelease5  = Tahoe t024
    // kDHL_1500, kMRelease6  = Tahoe t025..t027
    // kDHL_1500, kMRelease7  = Tahoe t028
    // kDHL_1500, kMRelease8  = Tahoe t029
    // kDHL_1500, kMRelease9  = Tahoe t030
    // kDHL_1500, kMRelease10 = Tahoe t031..t033
    // kDHL_1500, kMRelease11 = Tahoe t034
    // kDHL_1500, kMRelease12 = Tahoe t035..t036
    // kDHL_1500, kMRelease13 = Tahoe t037..t038
    // kDHL_1500, kMRelease14 = Tahoe t039
    // kDHL_1500, kMRelease15 = Tahoe t040..t041
    // kDHL_1500, kMRelease17 = Tahoe t042
    // kDHL_1500, kMRelease20 = Tahoe t047
    // kDHL_1500, kMRelease21 = Tahoe t048
    // kDHL_1500, kMRelease22 = Tahoe t049..t050
    // kDHL_1500, kMRelease23 = Tahoe t051
    // kDHL_1500, kMRelease24 = Tahoe t052..t053
    // kDHL_1500, kMRelease25 = Tahoe t054
    // kDHL_1500, kMRelease26 = Tahoe t055
    // kDHL_1500, kMRelease27 = Tahoe t056
    // kDHL_1500, kMRelease28 = not used
    // kDHL_1500, kMRelease29 = Tahoe t057
    // kDHL_1500, kMRelease30 = Tahoe t058
    // kDHL_1500, kMRelease31 = Tahoe t059
    // kDHL_1500, kMRelease32 = Tahoe t060
    // kDHL_1500, kMRelease33 = Tahoe t061
    // kDHL_1500, kMRelease34 = Tahoe t062..t063
    // kDHL_1500, kMRelease35 = Tahoe t064
    // kDHL_1500, kMRelease36 = Tahoe t065
    // kDHL_1500, kMRelease37 = Tahoe t066
    // kDHL_1500, kMRelease38 = Tahoe t067
    // kDHL_1500, kMRelease39 = Tahoe t068
    // kDHL_1500, kMRelease40 = Tahoe t069..t070 (fmt changed, tho this didn't)
    // kDHL_1500, kMRelease41 = Tahoe t071
    // kDHL_1500, kMRelease42 = Tahoe t072
    // kDHL_1500, kMRelease43 = Tahoe t073
    // kDHL_1500, kMRelease44 = Tahoe t074
    // kDHL_1500, kMRelease45 = Tahoe t075..t077
    // kDHL_1500, kMRelease46 = Tahoe t078
    // kDHL_1015, kMRelease0  = Tahoe t079..t080
    // kDHL_1015, kMRelease1  = Tahoe t081..t083
    // kDHL_1015, kMRelease2  = Tahoe t084..t086
    // kDHL_1015, kMRelease3  = Tahoe t087..t088
    // kDHL_1015, kMRelease4  = Tahoe t089..t091
    // kDHL_1015, kMRelease5  = Tahoe t092..t094
    // kDHL_1015, kMRelease6  = Tahoe t095..
    // kDHL_1015, kMRelease7  = Banff u002..
    // kDHL_1015, kMRelease8  = Banff u016..
    // kDHL_1015, kMRelease9  = Banff u028..
    // kDHL_1015, kMRelease10 = Banff u070..
    // kDHL_1015, kMRelease11 = Banff SP3...
    // kDHL_1015, kMRelease12 = Kirkland K001...
    // kDHL_1015, kMRelease13 = Kirkland K009 (Banff SP3 merge)
    // kDHL_1015, kMRelease14 = Kirkland K039...
    // kDHL_1015, kMRelease14 = Red Deer V001...V012
    // kDHL_1015, kMRelease15 = Red Deer V051+ SaveAs 2000
    // kDHL_1016, kMRelease16...17 unused by Red Deer
    // kDHL_1016, kMRelease18 = Red Deer V013...V039+
    // kDHL_1016, kMRelease19 = Red Deer V040..
    // kDHL_1800, kMRelease0  = Red Deer v032..v033 (not kDHL_CURRENT yet)
    // kDHL_1800, kMRelease1  = Red Deer v034..v050 (not kDHL_CURRENT yet)
    // kDHL_1800('AC401h'), kMRelease0  = Red Deer v051+ (kDHL_CURRENT)
    // kDHL_1800('AC402b'), kMRelease0  = Red Deer v062+ (kDHL_CURRENT)
    // kDHL_1800, kMRelease0 = RedDeer shipping
    // kDHL_1800, kMRelease1 = RedDeer service pack
    // kDHL_1800, kMRelease21 = Neo n020
    // kDHL_1800, kMRelease22 = Neo n021...
    // kDHL_1800, kMRelease23 = Neo n023...
    // kDHL_1800, kMRelease24 = Neo n025...Alpha1
    // kDHL_1800, kMRelease25 = Neo n026...
    // kDHL_1800, kMRelease26 = Neo n027...
    // kDHL_1800, kMRelease27 = Neo n030...Alpha2
    // kDHL_1800, kMRelease28 = Neo n031...
    // kDHL_1800, kMRelease29 = Neo n032...
    // kDHL_1800, kMRelease30 = Neo n034...Beta1    
    // kDHL_1800, kMRelease31 = Neo n035...
    // kDHL_1800, kMRelease32 = Neo n036...
    // kDHL_1800, kMRelease33 = Neo n041, N042, N041.100...
    // kDHL_1800, kMRelease34 = Neo n041.101....Beta2
    // kDHL_1800, kMRelease35 = Neo n043...    
    // kDHL_1800, kMRelease36 = Neo n045, n046, n047...    
    // kDHL_1800, kMRelease37 = Neo n046.100....Beta3
    // kDHL_1800, kMRelease38 = Neo n048... 
    // kDHL_1800, kMRelease39 = Neo n058...n063 (fcs)
    // kDHL_1800, kMRelease55 = Rio z002...z023
    // kDHL_1800, kMRelease56 = Rio z024 (alpha1)
    // kDHL_1800, kMRelease57 = Rio z025...
    // kDHL_1800, kMRelease58 = Rio z026...    
    // kDHL_1800, kMRelease59 = Rio z029...
    // kDHL_1800, kMRelease60 = Rio z030...
    // kDHL_1800, kMRelease61 = Rio z031...
    // kDHL_1800, kMRelease62 = Rio z032...    
    // kDHL_1800, kMRelease63 = Rio z033...
    // kDHL_1800, kMRelease64 = Rio z034... (beta 1)
    // kDHL_1800, kMRelease65 = Rio z035...
    // kDHL_1800, kMRelease66 = Rio z036...
    // kDHL_1800, kMRelease67 = Rio z038... (beta 2)
    // kDHL_1800, kMRelease68 = Rio z039...
    // kDHL_1800, kMRelease69 = Rio z041...
    // kDHL_1800, kMRelease70 = Rio z045... (beta 3)    
    // kDHL_1800, kMRelease71 = Rio z046...
    // kDHL_1800, kMRelease72 = Rio z048.100...    (beta 4)
    // kDHL_1800, kMRelease73 = Rio z049...   
    // kDHL_1800, kMRelease74 = Rio z050.100...    (beta 5)
    // kDHL_1800, kMRelease75 = Rio z051... 
    // kDHL_1800, kMRelease76 = Rio z052...Rio release candidate 1
    // kDHL_1800, kMRelease100 = PostRio ...  
    // kDHL_2100a ('AC701a'), kMRelease0  = PostRio (not kDHL_Current yet)  
    // kDHL_2100a, kMRelease10 = PostRio a032...
    // kDHL_2100a, kMRelease13 = PostRio a041
    // kDHL_1021, kMRelease0   = PostRio a043
    // kDHL_1021, kMRelease1   = PostRio shipping, Spago b001...b016
    // kDHL_1021, kMRelease2   = PostRio SP1
    // kDHL_1021, kMRelease3   = PostRio SP2 (Vista)
    // kDHL_1021, kMRelease15  = Spago b017...b028, alpha 2
    // kDHL_1021, kMRelease16  = Spago b029...
    // kDHL_1021, kMRelease17  = Spago b030...b031
    // kDHL_1021, kMRelease18 = Spago b032 (Beta 1)...
    // kDHL_1021, kMRelease19 = Spago b033...
    // kDHL_1021, kMRelease20 = Spago b036...(Beta 2)...
    // kDHL_1021, kMRelease21 = Spago b038...
    // kDHL_1021, kMRelease22 = Spago b039...
    // kDHL_1021, kMRelease23 = Spago b040...(Beta 3)...
    // kDHL_1021, kMRelease24 = Spago b041...
    // kDHL_1021, kMRelease25 = Spago b043...(Beta 4)... FCS
    // kDHL_1021, kMRelease45 = Raptor c004...
    // kDHL_1021, kMRelease46 = Raptor c030... (Alpha 1)
    // kDHL_1021, kMRelease47 = Raptor c041... (Beta 2)
    // kDHL_1021, kMRelease48 = Raptor c045... (Beta 3)
    // kDHL_1021, kMRelease49 = Raptor c049... (Beta 4)
    // kDHL_1021, kMRelease50 = Raptor c054... (RTM)
    // kDHL_1021, kMRelease100 = Gator  d006...d015
    // kDHL_1021, kMRelease101 = Gator  d016...d027
    // kDHL_1021, kMRelease102 = Gator  d028 (Alpha 1)
    // kDHL_2400a (''), kMRelease0  = Gator d016 (not kDHL_Current yet)  
    // kDHL_2400a (''), kMRelease1  = Gator d017...d022
    // kDHL_2400a (''), kMRelease2  = Gator d023...d027
    // kDHL_2400a (''), kMRelease3  = Gator d028 (Alpha 1)...d033
    // kDHL_1024, kMRelease0  = Gator d034 (Beta 1)
    // kDHL_1024, kMRelease1  = Gator d035 ..d038
    // kDHL_1024, kMRelease2  = Gator d039.. d044
    // kDHL_1024, kMRelease3  = Gator d045 (Beta 3)
    // kDHL_1024, kMRelease4  = Gator d046,d047
    // kDHL_1024, kMRelease5  = Gator d048 (Beta 4) .. d051
    // kDHL_1024, kMRelease6  = Gator d052 (RC1) ...
    
    enum MaintenanceReleaseVersion
                           { kMRelease0         = 0,
                             kMRelease1         = 1,
                             kMRelease2         = 2,
                             kMRelease3         = 3,
                             kMRelease4         = 4,
                             kMRelease5         = 5,
                             kMRelease6         = 6,
                             kMRelease7         = 7,
                             kMRelease8         = 8,
                             kMRelease9         = 9,
                             kMRelease10        = 10,
                             kMRelease11        = 11,
                             kMRelease12        = 12,
                             kMRelease13        = 13,
                             kMRelease14        = 14,
                             kMRelease15        = 15,
                             kMRelease16        = 16,
                             kMRelease17        = 17,
                             kMRelease18        = 18,
                             kMRelease19        = 19,
                             kMRelease20        = 20,
                             kMRelease21        = 21,
                             kMRelease22        = 22, 
                             kMRelease23        = 23,
                             kMRelease24        = 24,
                             kMRelease25        = 25,
                             kMRelease26        = 26,
                             kMRelease27        = 27,
                             kMRelease28        = 28,
                             kMRelease29        = 29,
                             kMRelease30        = 30,
                             kMRelease31        = 31,
                             kMRelease32        = 32,
                             kMRelease33        = 33,
                             kMRelease34        = 34,
                             kMRelease35        = 35,
                             kMRelease36        = 36,
                             kMRelease37        = 37,
                             kMRelease38        = 38,
                             kMRelease39        = 39,
                             kMRelease40        = 40,
                             kMRelease41        = 41,
                             kMReleaseFirstValid1500 = kMRelease41,
                             kMRelease42        = 42,
                             kMRelease43        = 43,
                             kMRelease44        = 44,
                             kMRelease45        = 45,
                             kMRelease46        = 46,
                             kMRelease47        = 47,
                             kMRelease48        = 48,
                             kMRelease49        = 49,
                             kMRelease50        = 50,
                             kMRelease51        = 51,
                             kMRelease52        = 52,
                             kMRelease53        = 53,
                             kMRelease54        = 54,
                             kMRelease55        = 55,
                             kMRelease56        = 56,
                             kMRelease57        = 57,
                             kMRelease58        = 58,
                             kMRelease59        = 59,
                             kMRelease60        = 60,
                             kMRelease61        = 61,
                             kMRelease62        = 62,
                             kMRelease63        = 63,
                             kMRelease64        = 64,
                             kMRelease65        = 65,
                             kMRelease66        = 66,
                             kMRelease67        = 67,
                             kMRelease68        = 68,
                             kMRelease69        = 69,
                             kMRelease70        = 70,
                             kMRelease71        = 71,
                             kMRelease72        = 72,
                             kMRelease73        = 73,
                             kMRelease74        = 74,
                             kMRelease75        = 75,
                             kMRelease76        = 76,                             
                             kMRelease100       = 100,
                             kMRelease101       = 101,
                             kMRelease102       = 102,
                             kMRelease103       = 103,
                             kMRelease104       = 104,
                             kMReleaseNewest    = kMRelease6,
                             kMReleaseCurrent   = kMReleaseNewest,
                             kMReleaseUnknown   = 126,
                             kMReleaseMax       = 127 };

    enum SaveType          { kNoSave = 0,
                             kR12Save,  // Only used for R12 DXF
                             kR13Save,  // No longer used
                             kR14Save,
                             k2000Save,
                             k2004Save,
                             k2007Save,
                             k2010Save };

    enum GroupOperType     { kNoOp           = 0,
                             kAdd            = 1,
                             kRemove         = 2,
                             kRename         = 3,
                             kOrdered        = 4,       // Members reordered
                             kGrpDesc        = 5,       // Description Modified
                             kSelect         = 6,       // Selectibility flag
                             kCreate         = 7,       // New group creation
                             kPrepend        = 8,
                             kInsert         = 9,
                             kGrpFlags       = 10 };

    // kDcInsert     => Database INSERT in which entities from the source
    //                  database could be moved into the destination database.
    // kDcInsertCopy => Database INSERT in which the source database is left
    //                  intact. Objects are copied to the destination database.
    //
    enum DeepCloneType     { kDcCopy           = 0,
                             kDcExplode        = 1,
                             kDcBlock          = 2,
                             kDcXrefBind       = 3,
                             kDcSymTableMerge  = 4,
                             kDcInsert         = 6,
                             kDcWblock         = 7,
                             kDcObjects        = 8,
                             kDcXrefInsert     = 9,
                             kDcInsertCopy     = 10,
                             kDcWblkObjects    = 11 };

    // Handling for duplicate Symbol and Dictionary records
    //
    enum DuplicateRecordCloning { 
                             kDrcNotApplicable  = 0,
                             kDrcIgnore         = 1,   // Keep existing
                             kDrcReplace        = 2,   // Use clone
                             kDrcXrefMangleName = 3,   // <xref>$0$<name>
                             kDrcMangleName     = 4,   // $0$<name>
                             kDrcUnmangleName   = 5 };

    enum XrefStatus        { kXrfNotAnXref     = 0,
                             kXrfResolved      = 1,
                             kXrfUnloaded      = 2,
                             kXrfUnreferenced  = 3,
                             kXrfFileNotFound  = 4,
                             kXrfUnresolved    = 5 };

    enum XrefNotificationStatus { 
                             kXrfNotifyNone = 0,
                             kXrfNotifyResolvedMatch = 1,
                             kXrfNotifyResolvedElsewhere = 2,
                             kXrfNotifyResolvedWithUpdate = 3,
                             kXrfNotifyResolvedUpdateAvailable = 4 };

    enum MeasurementValue {
                kEnglish = 0,
                kMetric  = 1,
    };

    enum UnitsValue {
        kUnitsUndefined     = 0,
        kUnitsInches        = 1,
        kUnitsFeet          = 2,
        kUnitsMiles         = 3,
        kUnitsMillimeters   = 4,
        kUnitsCentimeters   = 5,
        kUnitsMeters        = 6,
        kUnitsKilometers    = 7,
        kUnitsMicroinches   = 8,
        kUnitsMils          = 9,
        kUnitsYards         = 10,
        kUnitsAngstroms     = 11,
        kUnitsNanometers    = 12,
        kUnitsMicrons       = 13,
        kUnitsDecimeters    = 14,
        kUnitsDekameters    = 15,
        kUnitsHectometers   = 16,
        kUnitsGigameters    = 17,
        kUnitsAstronomical  = 18,
        kUnitsLightYears    = 19,
        kUnitsParsecs       = 20,
        kUnitsMax           = kUnitsParsecs,
    };

    // Time zone enum names may change from time to time, but enum values must
    // not, as they are stored in drawings. Enum values support adding new zones,
    // as long as backward compatibility of drawings is taken into consideration.
    enum TimeZone {
        kInternationalDateLine      = -12000,   //(GMT-12:00) International Date Line West
        kMidwayIsland               = -11000,   //(GMT-11:00) Midway Island, Samoa
        kHawaii                     = -10000,   //(GMT-10:00) Hawaii
        kAlaska                     = -9000,    //(GMT-09:00) Alaska
        kPacific                    = -8000,    //(GMT-08:00) Pacific Time (US & Canada); Tijuana
        kMountain                   = -7000,    //(GMT-07:00) Mountain Time (US & Canada)
        kArizona                    = -7001,    //(GMT-07:00) Arizona
        kMazatlan                   = -7002,    //(GMT-07:00) Chihuahua, La Paz, Mazatlan
        kCentral                    = -6000,    //(GMT-06:00) Central Time (US & Canada)
        kCentralAmerica             = -6001,    //(GMT-06:00) Central America
        kMexicoCity                 = -6002,    //(GMT-06:00) Guadalajara, Mexico City, Monterrey
        kSaskatchewan               = -6003,    //(GMT-06:00) Saskatchewan
        kEastern                    = -5000,    //(GMT-05:00) Eastern Time (US & Canada)
        kIndiana                    = -5001,    //(GMT-05:00) Indiana (East)
        kBogota                     = -5002,    //(GMT-05:00) Bogota, Lima, Quito
        kAtlanticCanada             = -4000,    //(GMT-04:00) Atlantic Time (Canada)
        kCaracas                    = -4001,    //(GMT-04:00) Caracas, La Paz
        kSantiago                   = -4002,    //(GMT-04:00) Santiago
        kNewfoundland               = -3300,    //(GMT-03:30) Newfoundland
        kBrasilia                   = -3000,    //(GMT-03:00) Brasilia
        kBuenosAires                = -3001,    //(GMT-03:00) Buenos Aires, Georgetown
        kGreenland                  = -3002,    //(GMT-03:00) Greenland
        kMidAtlantic                = -2000,    //(GMT-02:00) Mid-Atlantic
        kAzores                     = -1000,    //(GMT-01:00) Azores
        kCapeVerde                  = -1001,    //(GMT-01:00) Cape Verde Is.
        kUTC                        =     0,    //(UTC) Universal Coordinated Time
        kGMT                        =     1,    //(GMT) Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London
        kMonrovia                   =     2,    //(GMT) Casablanca, Monrovia
        kBerlin                     =  1000,    //(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna
        kParis                      =  1001,    //(GMT+01:00) Brussels, Copenhagen, Madrid, Paris
        kPrague                     =  1002,    //(GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague
        kSarajevo                   =  1003,    //(GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb
        kWestCentralAfrica          =  1004,    //(GMT+01:00) West Central Africa
        kAthens                     =  2000,    //(GMT+02:00) Athens, Beirut, Istanbul, Minsk
        kEasternEurope              =  2001,    //(GMT+02:00) Bucharest
        kCairo                      =  2002,    //(GMT+02:00) Cairo
        kHarare                     =  2003,    //(GMT+02:00) Harare, Pretoria
        kHelsinki                   =  2004,    //(GMT+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius
        kJerusalem                  =  2005,    //(GMT+02:00) Jerusalem
        kMoscow                     =  3000,    //(GMT+03:00) Moscow, St. Petersburg, Volgograd
        kRiyadh                     =  3001,    //(GMT+03:00) Kuwait, Riyadh
        kBaghdad                    =  3002,    //(GMT+03:00) Baghdad
        kEastAfrica                 =  3003,    //(GMT+03:00) Nairobi
        kTehran                     =  3300,    //(GMT+03:30) Tehran
        kAbuDhabi                   =  4000,    //(GMT+04:00) Abu Dhabi, Muscat
        kCaucasus                   =  4001,    //(GMT+04:00) Baku, Tbilisi, Yerevan
        kKabul                      =  4300,    //(GMT+04:30) Kabul
        kEkaterinburg               =  5000,    //(GMT+05:00) Ekaterinburg
        kIslamabad                  =  5001,    //(GMT+05:00) Islamabad, Karachi, Tashkent
        kKolkata                    =  5300,    //(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi
        kKathmandu                  =  5450,    //(GMT+05:45) Kathmandu
        kAlmaty                     =  6000,    //(GMT+06:00) Almaty, Novosibirsk
        kDhaka                      =  6001,    //(GMT+06:00) Astana, Dhaka
        kSriLanka                   =  6002,    //(GMT+06:00) Sri Jayawardenepura
        kRangoon                    =  6300,    //(GMT+06:30) Rangoon
        kBangkok                    =  7000,    //(GMT+07:00) Bangkok, Hanoi, Jakarta
        kKrasnoyarsk                =  7001,    //(GMT+07:00) Krasnoyarsk
        kBeijing                    =  8000,    //(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi
        kSingapore                  =  8001,    //(GMT+08:00) Kuala Lumpur, Singapore
        kTaipei                     =  8002,    //(GMT+08:00) Taipei
        kIrkutsk                    =  8003,    //(GMT+08:00) Irkutsk, Ulaan Bataar
        kPerth                      =  8004,    //(GMT+08:00) Perth
        kTokyo                      =  9000,    //(GMT+09:00) Osaka, Sapporo, Tokyo
        kSeoul                      =  9001,    //(GMT+09:00) Seoul
        kYakutsk                    =  9002,    //(GMT+09:00) Yakutsk
        kAdelaide                   =  9300,    //(GMT+09:30) Adelaide
        kDarwin                     =  9301,    //(GMT+09:30) Darwin
        kSydney                     = 10000,    //(GMT+10:00) Canberra, Melbourne, Sydney
        kGuam                       = 10001,    //(GMT+10:00) Guam, Port Moresby
        kBrisbane                   = 10002,    //(GMT+10:00) Brisbane
        kHobart                     = 10003,    //(GMT+10:00) Hobart
        kVladivostock               = 10004,    //(GMT+10:00) Vladivostok
        kMagadan                    = 11000,    //(GMT+11:00) Magadan, Solomon Is., New Caledonia
        kWellington                 = 12000,    //(GMT+12:00) Auckland, Wellington
        kFiji                       = 12001,    //(GMT+12:00) Fiji, Kamchatka, Marshall Is.
        kTonga                      = 13000,    //(GMT+13:00) Nuku'alofa
    };

    // lineweights are in 100ths of a millimeter
    enum LineWeight        { kLnWt000          =   0,
                             kLnWt005          =   5,
                             kLnWt009          =   9,
                             kLnWt013          =  13,
                             kLnWt015          =  15,
                             kLnWt018          =  18,
                             kLnWt020          =  20,
                             kLnWt025          =  25,
                             kLnWt030          =  30,
                             kLnWt035          =  35,
                             kLnWt040          =  40,
                             kLnWt050          =  50,
                             kLnWt053          =  53,
                             kLnWt060          =  60,
                             kLnWt070          =  70,
                             kLnWt080          =  80,
                             kLnWt090          =  90,
                             kLnWt100          = 100,
                             kLnWt106          = 106,
                             kLnWt120          = 120,
                             kLnWt140          = 140,
                             kLnWt158          = 158,
                             kLnWt200          = 200,
                             kLnWt211          = 211,
                             kLnWtByLayer      = -1,
                             kLnWtByBlock      = -2,
                             kLnWtByLwDefault  = -3 };


    enum EndCaps           { kEndCapNone       =  0,
                             kEndCapRound      =  1,
                             kEndCapAngle      =  2,
                             kEndCapSquare     =  3 };

    enum JoinStyle         { kJnStylNone       =  0,
                             kJnStylRound      =  1,
                             kJnStylAngle      =  2,
                             kJnStylFlat       =  3 };

    enum PlotStyleNameType { kPlotStyleNameByLayer       = 0,
                             kPlotStyleNameByBlock       = 1,
                             kPlotStyleNameIsDictDefault = 2,
                             kPlotStyleNameById          = 3 };

    enum IndexCreation     { kNoIndex        = 0,
                             kIndexByLayer   = 1,
                             kIndexSpatially = 2 };

    enum LayerEvaluation   { kNoNewLayerEvaluation = 0,
                             kEvalNewXrefLayers    = 1,
                             kEvalAllNewLayers     = 2 };

    enum NewLayerNotification { kNoNewLayerNotification      = 0,
                                kNotifyOnPlot                = 1,
                                kNotifyOnOpen                = 2,
                                kNotifyOnXrefAttachAndReload = 4,
                                kNotifyOnLayerStateRestore   = 8,
                                kNotifyOnSave                = 16,
                                kNotifyOnInsert              = 32 };

    enum DimArrowFlags     { kFirstArrow  = 0x00,
                             kSecondArrow = 0x01 };

    enum DimblkFlags       { kDimblk = 0,
                             kDimblk1,
                             kDimblk2,
                             kDimLdrBlk };

    enum OrthographicView  { kNonOrthoView = 0,
                             kTopView      = 1,
                             kBottomView   = 2,
                             kFrontView    = 3,
                             kBackView     = 4,
                             kLeftView     = 5,
                             kRightView    = 6 };

    enum VpFreezeOps       { kFreeze = 0,
                             kThaw,
                             kReset };

    // If adding to the following enum you must change the array
    // in constant_cpp.h and the initialization string in constant.xmf.
    enum reservedStringEnumType { kByLayer = 0,
                             kByBlock      = 1,
                             kNone         = 2,
                             kContinuous   = 3,
                             kStandard     = 4, 
                             kNormal       = 5,
                             kDefPoints    = 6,
                             kDefault      = 7,
                             kMissing      = 8,
                             kByColor      = 9,
                             kModel        = 10,
                             kGlobal       = 11, //used for default global material - Randy Kintzley 06/08/2005
                             kVS2DWireframe = 12, // Visual Style UI names
                             kVS3DWireframe = 13,
                             kVS3DHidden   = 14,
                             kVSRealistic  = 15,
                             kVSConceptual = 16,
                             kTitle        = 17,    // Title cell style
                             kHeader       = 18,    // Header cell style
                             kData         = 19,    // Data cell style
                             kReservedStringCount };

    // SortEntsFlags:  Specifies when entity traversals are in sorted order
    //
    enum SortEntsFlags     { kSortEntsSelection  = 0x01,
                             kSortEntsOsnap      = 0x02,
                             kSortEntsRedraw     = 0x04,
                             kSortEntsSlide      = 0x08, // (obsolete)
                             kSortEntsRegen      = 0x10, 
                             kSortEntsPlot       = 0x20,
                             kSortEntsPsout      = 0x40 };

    enum DragStat          { kDragStart = 0,
                             kDragEnd   = 1,
                             kDragAbort = 2 };

    enum GripStat          { kGripsDone          = 0,
                             kGripsToBeDeleted   = 1,
                             kDimDataToBeDeleted = 2 };

    enum DataLinkOption    { kDataLinkOptionNone            = 0,
                             kDataLinkOptionAnonymous       = 0x1,
                             kDataLinkOptionPersistCache    = 0x2,
                             kDataLinkOptionDisableInLT     = 0x4, // For internal use only
                             kDataLinkHasCutomData          = 0x8, // For internal use only
                           };

    enum PathOption         { kNoPath       = 1,
                              kRelativePath = 2,
                              kAbsolutePath = 3,
                              kPathAndFile  = 4,
                            };

    enum UpdateDirection   { kUpdateDirSourceToData   = 0x1,
                             kUpdateDirDataToSource   = 0x2,
                           };

    enum UpdateOption      { // Bits 0-15 are reserved for adapters to set 
                             // adapter specific update option
                             kUpdateOptionNone                                  = 0,
                             kUpdateOptionSkipFormat                            = 0x20000,
                             kUpdateOptionUpdateRowHeight                       = 0x40000,
                             kUpdateOptionUpdateColumnWidth                     = 0x80000,
                             kUpdateOptionAllowSourceUpdate                     = 0x100000,
                             kUpdateOptionForceFullSourceUpdate                 = 0x200000,
                             kUpdateOptionOverwriteContentModifiedAfterUpdate   = 0x400000,
                             kUpdateOptionOverwriteFormatModifiedAfterUpdate    = 0x800000,
                             kUpdateOptionForPreview                            = 0x1000000,
                             kUpdateOptionIncludeXrefs                          = 0x2000000,
                             kSkipFormatAfterFirstUpdate                        = 0x4000000,
                           };

    enum DataLinkGetSourceContext { kDataLinkGetSourceContextUnknown     = 0,
                                    kDataLinkGetSourceContextEtransmit   = 1,
                                    kDataLinkGetSourceContextXrefManager = 2,
                                    kDataLinkGetSourceContextFileWatcher = 3,
                                    kDataLinkGetSourceContextOther       = 4,
                                    kDataLinkGetSourceContextOrignalPath = (0x1 << 8),
                                  };

    // AcDbTable and AcTbTableStyle specific enum
    //
    enum CellType          { kUnknownCell           = 0,
                             kTextCell              = 1,
                             kBlockCell             = 2,
                             kMultipleContentCell   = 3};

    enum CellContentType   { kCellContentTypeUnknown = 0,
                             kCellContentTypeValue   = 0x1,
                             kCellContentTypeField   = 0x2,
                             kCellContentTypeBlock   = 0x4,
                           };

    enum CellEdgeMask      { kTopMask        = 1,
                             kRightMask      = 2,
                             kBottomMask     = 4,
                             kLeftMask       = 8 };

    enum CellMargin        { kCellMarginTop         = 0x1,
                             kCellMarginLeft        = 0x2,
                             kCellMarginBottom      = 0x4,
                             kCellMarginRight       = 0x8,
                             kCellMarginHorzSpacing = 0x10,
                             kCellMarginVertSpacing = 0x20 };

    enum CellContentLayout { kCellContentLayoutFlow              = 0x1,
                             kCellContentLayoutStackedHorizontal = 0x2,
                             kCellContentLayoutStackedVertical   = 0x4,
                           };

    enum CellState         { kCellStateNone             = 0,
                             kCellStateContentLocked    = 0x1,
                             kCellStateContentReadOnly  = 0x2,
                             kCellStateLinked           = 0x4,
                             kCellStateContentModifiedAfterUpdate = 0x8,
                             kCellStateFormatLocked     = 0x10,
                             kCellStateFormatReadOnly   = 0x20,
                             kCellStateFormatModifiedAfterUpdate = 0x40,
                             kAllCellStates             = (kCellStateContentLocked | kCellStateContentReadOnly |
                                                           kCellStateLinked | kCellStateContentModifiedAfterUpdate | 
                                                           kCellStateFormatLocked | kCellStateFormatReadOnly |
                                                           kCellStateFormatModifiedAfterUpdate),
                           };

    enum CellClass         { kCellClassNone     = 0,
                             kCellClassLabel    = 0x1,
                             kCellClassData     = 0x2,
                           };

    enum CellProperty      { kCellPropInvalid               = 0,
                             // Content properties
                             kCellPropDataType              = 0x1,
                             kCellPropDataFormat            = 0x2,
                             kCellPropRotation              = 0x4,
                             kCellPropScale                 = 0x8,
                             kCellPropAlignment             = 0x10,
                             kCellPropContentColor          = 0x20,
                             kCellPropTextStyle             = 0x40,
                             kCellPropTextHeight            = 0x80,
                             kCellPropAutoScale             = 0x100,
                             // Cell properties
                             kCellPropBackgroundColor       = 0x200,
                             kCellPropMarginLeft            = 0x400,
                             kCellPropMarginTop             = 0x800,
                             kCellPropMarginRight           = 0x1000,
                             kCellPropMarginBottom          = 0x2000,
                             kCellPropMarginHorzSpacing     = 0x20000,  // TODO: Re-order
                             kCellPropMarginVertSpacing     = 0x40000,  // TODO: Re-order
                             kCellPropContentLayout         = 0x4000,
                             // Row/col properties
                             kCellPropMergeAll              = 0x8000,
                             // Table properties
                             kCellPropFlowDirBtoT           = 0x10000,
                             // Combos
                             kCellPropDataTypeAndFormat     = (kCellPropDataType | kCellPropDataFormat),
                             kCellPropContent               = (kCellPropDataType | kCellPropDataFormat | kCellPropRotation |
                                                               kCellPropScale | kCellPropContentColor |  kCellPropTextStyle | 
                                                               kCellPropTextHeight | kCellPropAutoScale),
                             kCellPropBitProperties         = (kCellPropAutoScale | kCellPropMergeAll | kCellPropFlowDirBtoT),
                             kCellPropAll                   = (kCellPropDataType | kCellPropDataFormat | kCellPropRotation |
                                                               kCellPropScale | kCellPropAlignment | kCellPropContentColor | kCellPropBackgroundColor | 
                                                               kCellPropTextStyle | kCellPropTextHeight | kCellPropMarginLeft | kCellPropMarginTop |
                                                               kCellPropMarginRight | kCellPropMarginBottom | kCellPropMarginHorzSpacing | 
                                                               kCellPropMarginVertSpacing | kCellPropAutoScale | kCellPropMergeAll | 
                                                               kCellPropFlowDirBtoT | kCellPropContentLayout),
                           };

    enum GridProperty      { kGridPropInvalid           = 0,
                             kGridPropLineStyle         = 0x1,
                             kGridPropLineWeight        = 0x2,
                             kGridPropLinetype          = 0x4,
                             kGridPropColor             = 0x8,
                             kGridPropVisibility        = 0x10,
                             kGridPropDoubleLineSpacing = 0x20,
                             kGridPropAll               = (kGridPropLineStyle | kGridPropLineWeight | kGridPropLinetype |
                                                           kGridPropColor | kGridPropVisibility | kGridPropDoubleLineSpacing),
                           };

    enum GridLineType      { kInvalidGridLine       = 0,
                             kHorzTop               = 1,
                             kHorzInside            = 2,
                             kHorzBottom            = 4,
                             kVertLeft              = 8,
                             kVertInside            = 0x10,
                             kVertRight             = 0x20,
                             kHorzGridLineTypes     = kHorzTop | kHorzBottom | kHorzInside,
                             kVertGridLineTypes     = kVertLeft | kVertRight | kVertInside,
                             kOuterGridLineTypes    = kHorzTop | kHorzBottom | kVertLeft | kVertRight,
                             kInnerGridLineTypes    = kHorzInside | kVertInside,
                             kAllGridLineTypes      = kOuterGridLineTypes | kInnerGridLineTypes };

    enum GridLineStyle      { kGridLineStyleSingle = 1,
                              kGridLineStyleDouble = 2,
                            };

    // For internal use only
    enum CellOption        {
                             kCellOptionNone    = 0,
                             kInheritCellFormat = 0x1,
                           };

    enum SelectType        { kWindow         = 1,
                             kCrossing       = 2 };

    enum FlowDirection     { kTtoB           = 0,
                             kBtoT           = 1 };

    enum RotationAngle     { kDegreesUnknown = -1,
                             kDegrees000     = 0,
                             kDegrees090     = 1,
                             kDegrees180     = 2,
                             kDegrees270     = 3 };

    enum CellAlignment     { kTopLeft        = 1,
                             kTopCenter      = 2,
                             kTopRight       = 3,
                             kMiddleLeft     = 4,
                             kMiddleCenter   = 5,
                             kMiddleRight    = 6,
                             kBottomLeft     = 7,
                             kBottomCenter   = 8,
                             kBottomRight    = 9 };

    enum RowType           { kUnknownRow     = 0, // TODO: May not be needed
                             kDataRow        = 1,
                             kTitleRow       = 2,
                             kHeaderRow      = 4,
                             kAllRowTypes = kDataRow | kTitleRow | kHeaderRow };

    enum TableStyleFlags   { kHorzInsideLineFirst  = 0x1,
                             kHorzInsideLineSecond = 0x2,
                             kHorzInsideLineThird  = 0x4,
                             kTableStyleModified   = 0x8,
                             kTableStyleDataTypeModified = 0x10,    // For internal use only
                           };

    enum TableBreakOption  { kTableBreakNone                    = 0,
                             kTableBreakEnableBreaking          = 0x1,
                             kTableBreakRepeatTopLabels         = 0x2,
                             kTableBreakRepeatBottomLabels      = 0x4,
                             kTableBreakAllowManualPositions    = 0x8,
                             kTableBreakAllowManualHeights      = 0x10,
                           };

    enum TableBreakFlowDirection { kTableBreakFlowRight         = 0x1,
                                   kTableBreakFlowDownOrUp      = 0x2,
                                   kTableBreakFlowLeft          = 0x4,
                                 };

    enum TableFillOption   { kTableFillNone                     = 0,
                             kTableFillRow                      = 0x1,
                             kTableFillReverse                  = 0x2,
                             kTableFillGenerateSeries           = 0x4,
                             kTableFillCopyContent              = 0x8,
                             kTableFillCopyFormat               = 0x10,
                             kTableFillOverwriteReadOnlyContent = 0x20,
                             kTableFillOverwriteReadOnlyFormat  = 0x40,
                           };

    enum TableCopyOption   { kTableCopyNone                                 = 0,
                             kTableCopyExpandOrContractTable                = 0x1,
                             kTableCopySkipContent                          = 0x2,
                             kTableCopySkipValue                            = 0x4,
                             kTableCopySkipField                            = 0x8,
                             kTableCopySkipFormula                          = 0x10,
                             kTableCopySkipBlock                            = 0x20,
                             kTableCopySkipDataLink                         = 0x40,
                             kTableCopySkipLabelCell                        = 0x80,
                             kTableCopySkipDataCell                         = 0x100,
                             kTableCopySkipFormat                           = 0x200,
                             kTableCopySkipCellStyle                        = 0x400,
                             kTableCopyConvertFormatToOverrides             = 0x800,
                             kTableCopySkipCellState                        = 0x1000,
                             kTableCopySkipContentFormat                    = 0x2000,
                             kTableCopySkipDissimilarContentFormat          = 0x4000,
                             kTableCopySkipGeometry                         = 0x8000,
                             kTableCopySkipMerges                           = 0x10000,
                             kTableCopyFillTarget                           = 0x20000,
                             kTableCopyOverwriteReadOnlyContent             = 0x40000,
                             kTableCopyOverwriteReadOnlyFormat              = 0x80000,
                             kTableCopyOverwriteContentModifiedAfterUpdate  = 0x100000,
                             kTableCopyOverwriteFormatModifiedAfterUpdate   = 0x200000,
                             kTableCopyOnlyContentModifiedAfterUpdate       = 0x400000,
                             kTableCopyOnlyFormatModifiedAfterUpdate        = 0x800000,
                             kTableCopyRowHeight                            = 0x1000000,
                             kTableCopyColumnWidth                          = 0x2000000,
                             kTableCopyFullCellState                        = 0x4000000,
                             kTableCopyForRountrip                          = 0x8000000,    // For internal use only
                             kTableCopyConvertFieldToValue                  = 0x10000000,   // For internal use only
                             kTableCopySkipFieldTranslation                 = 0x20000000,   // For internal use only
                           };

    enum TableIteratorOption { kTableIteratorNone               = 0,
                               kTableIteratorIterateSelection   = 0x1,
                               kTableIteratorIterateRows        = 0x2,
                               kTableIteratorIterateColumns     = 0x4,
                               kTableIteratorIterateDataLinks   = 0x80, // TODO: Re order
                               kTableIteratorReverseOrder       = 0x8,
                               kTableIteratorSkipReadOnlyContent= 0x10,
                               kTableIteratorSkipReadOnlyFormat = 0x20,
                               kTableIteratorSkipMerged         = 0x40,
                             };

    enum MergeCellStyleOption { kMergeCellStyleNone                         = 0,
                                kMergeCellStyleCopyDuplicates               = 0x1,
                                kMergeCellStyleOverwriteDuplicates          = 0x2,
                                kMergeCellStyleConvertDuplicatesToOverrides = 0x4,
                                kMergeCellStyleIgnoreNewStyles              = 0x8,
                             };

    enum TableHitItem        { kTableHitNone            = 0,
                               kTableHitCell            = 0x1,
                               kTableHitRowIndicator    = 0x2,
                               kTableHitColumnIndicator = 0x4,
                               kTableHitGridLine        = 0x8,
                             };

    enum RowTypes          { kAllRows = kDataRow | kTitleRow | kHeaderRow };
    enum GridLineTypes     { kAllGridLines = kAllGridLineTypes };


    // DrawOrderFlags:  Specifies draw order behavior when editing existing
    //                  entities or creating new "derived" entities
    //
    enum DrawOrderFlags    { kDrawOrderInPlaceEditOn  = 0x01,
                             kDrawOrderInheritanceOn  = 0x02 };

    // UpdateThumbnailFlags:  Specifies whether a drawing save includes
    //                        updating thumbnails for sheet views, model 
    //                        views, or sheets
    //
    enum UpdateThumbnailFlags { kUpdateModelViews   = 0x01,
                                kUpdateSheetViews   = 0x02,
                                kUpdateSheets       = 0x04,
                                kUpdateWhenAccessed = 0x08,
                                kUpdateWhenSaving   = 0x10 };

    // EnhancedBlockEvalType: Used in evaluating the graphs that support
    //                        Enhanced Blocks.  Specifies what sort of
    //                        operation this evaluation pass is supporting.
    //                        Used in an AcDbEvalContext container in 
    //                        conjunction with the ACDB_ENHANCED_BLOCK_EVALKEY.
    enum EnhancedBlockEvalType { kInitialization    = 1,
                                 kForUpdate         = 2,
                                 kForRefresh        = 3};

    // SliceModelType: Specifies flags for slicing solid, surface or region.
    enum SliceModelType    { kKeepBothSides    = 0x01,
                             kUseCurrentColor  = 0x02 };

    // CollisionType: Specifies how the entity interacts with collision
    //                detection.
    enum CollisionType     { kCollisionTypeNone  = 0,
                             kCollisionTypeSolid = 1 };

    // LoftParamType: Specifies settings that affect the shape of the 
    // lofted solid or surface.
    enum LoftParamType     { kLoftNoParam = 0x00,
                             kLoftNoTwist = 0x01,
                             kLoftAlignDirection = 0x02,
                             kLoftSimplify = 0x04,
                             kLoftClose = 0x08,
                             kLoftDefault = kLoftNoTwist | kLoftAlignDirection | kLoftSimplify };

    // LoftNormalsType: Specifies the normals of the lofted object where it
    // passes through cross section curves.
    enum LoftNormalsType  { kLoftRuled = 0,
                            kLoftSmooth = 1,
                            kLoftFirstNormal = 2,
                            kLoftLastNormal = 3,
                            kLoftEndsNormal = 4,
                            kLoftAllNormal = 5,
                            kLoftUseDraftAngles = 6 };

    enum ShadowFlags      { kShadowsCastAndReceive  = 0x00,
                            kShadowsDoesNotReceive  = 0x01,
                            kShadowsDoesNotCast     = 0x02,
                            kShadowsIgnore          = kShadowsDoesNotCast | kShadowsDoesNotReceive};

    enum LightingUnits    { kLightingUnitsGeneric = 0,
                            kLightingUnitsAmerican = 1,
                            kLightingUnitsInternational = 2 };

}; // struct AcDb

#pragma pack(pop)

#endif
