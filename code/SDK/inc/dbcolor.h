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
// DESCRIPTION: True Color Definitions


// headers
#ifndef AD_DBCOLOR_H
#define AD_DBCOLOR_H 1

#include "adesk.h"
#include "acadstrc.h"
#include <assert.h>

#pragma pack (push, 8)


class AcCmEntityColor;


// It takes care of the color method, which is one of the following: 
// byBlock, byLayer, or byColor.
// Depending on this color method it stores RGB, ACI, or Layerindex.
//
// Note: To save memory I did the following:
//          It is not deriving from AcDbXObject. 
//          No virtual methods.
//          Color Method stored in last byte mRGBM.
class AcCmEntityColor
{
public:
    enum Color { kRed,
                 kGreen,
                 kBlue
    };

    // Color Method.
    enum ColorMethod {   kByLayer =0xC0, 
                         kByBlock,
                         kByColor,
                         kByACI,
                         kByPen,
                         kForeground,
                         kLayerOff,
                         // Run-time states
                         kLayerFrozen,
                         kNone
    };

    enum ACIcolorMethod {kACIbyBlock    = 0,
                         kACIforeground = 7,
                         kACIbyLayer    = 256,
                         // Run-time states
                         kACIclear      = 0,    
                         kACIstandard   = 7,
                         kACImaximum    = 255,
                         kACInone       = 257,
                         kACIminimum    = -255,
                         kACIfrozenLayer= -32700
    };

    // Blue, green, red, and Color Method (byBlock, byLayer, byColor).
    // Is stored that way for better performance. 
    union RGBM {
        Adesk::UInt32    whole;
        Adesk::Int16     indirect;
        struct {
            Adesk::UInt8 blue, 
                         green, 
                         red, 
                         colorMethod;
        };
    };

    AcCmEntityColor     ();
    AcCmEntityColor     (const AcCmEntityColor & color);
    AcCmEntityColor     (const ColorMethod eColorMethod);
    AcCmEntityColor     (const Adesk::UInt8 red, 
                         const Adesk::UInt8 green, 
                         const Adesk::UInt8 blue);

    AcCmEntityColor&    operator =  (const AcCmEntityColor& color);
    bool                operator == (const AcCmEntityColor& color) const;
    bool                operator != (const AcCmEntityColor& color) const;


    // Set/get components
    Acad::ErrorStatus   setColorMethod  (const ColorMethod eColorMethod);
    ColorMethod         colorMethod     () const;

    Acad::ErrorStatus   setColor        (const Adesk::UInt32 color);
    Adesk::UInt32       color           () const;   

    Acad::ErrorStatus   setColorIndex   (const Adesk::Int16 colorIndex);
    Adesk::Int16        colorIndex      () const;

    Acad::ErrorStatus   setLayerIndex   (const Adesk::Int16 layerIndex);
    Adesk::Int16        layerIndex      () const;   

    Acad::ErrorStatus   setPenIndex     (const Adesk::UInt16 penIndex);
    Adesk::UInt16       penIndex        () const;   

    // Set/get RGB components
    Acad::ErrorStatus   setRGB  (const Adesk::UInt8 red, 
                                 const Adesk::UInt8 green, 
                                 const Adesk::UInt8 blue);
    Acad::ErrorStatus   setRed  (const Adesk::UInt8 red);
    Acad::ErrorStatus   setGreen(const Adesk::UInt8 green);
    Acad::ErrorStatus   setBlue (const Adesk::UInt8 blue);
    Adesk::UInt8        red     () const;
    Adesk::UInt8        green   () const;
    Adesk::UInt8        blue    () const;

    // Method check
    bool                isByColor   () const;
    bool                isByLayer   () const;
    bool                isByBlock   () const;
    bool                isByACI     ()   const;
    bool                isByPen     ()  const;
    bool                isForeground() const;
    bool                isLayerOff  () const;
    // Run time states.
    bool                isLayerFrozen() const;
    bool                isNone      ()   const;
    bool                isLayerFrozenOrOff() const;

    // conversion
    Adesk::UInt32       trueColor () const;
    Adesk::UInt8        trueColorMethod () const;
    Acad::ErrorStatus   setTrueColor ();
    Acad::ErrorStatus   setTrueColorMethod ();



    // static methods for reuse in other classes.
    static  Acad::ErrorStatus   setColorMethod(RGBM* rgbm, const ColorMethod eColorMethod);
    static  ColorMethod         colorMethod   (const RGBM* rgbm);

    static  Acad::ErrorStatus   setColor      (RGBM* rgbm, const Adesk::UInt32 color);
    static  Adesk::UInt32       color         (const RGBM* rgbm);   

    static  Acad::ErrorStatus   setColorIndex (RGBM* rgbm, const Adesk::Int16 colorIndex);
    static  Adesk::Int16        colorIndex    (const RGBM* rgbm);

    static  Acad::ErrorStatus   setLayerIndex (RGBM* rgbm, const Adesk::Int16 layerIndex);
    static  Adesk::Int16        layerIndex    (const RGBM* rgbm);   

    static  Acad::ErrorStatus   setPenIndex   (RGBM* rgbm, const Adesk::UInt16 penIndex);
    static  Adesk::UInt16       penIndex      (const RGBM* rgbm);   

    // Set/get RGB components
    static Acad::ErrorStatus    setRGB  (RGBM* rgbm,
                                         const Adesk::UInt8 red, 
                                         const Adesk::UInt8 green, 
                                         const Adesk::UInt8 blue);
    static Acad::ErrorStatus    setRed  (RGBM* rgbm, const Adesk::UInt8 red);
    static Acad::ErrorStatus    setGreen(RGBM* rgbm, const Adesk::UInt8 green);
    static Acad::ErrorStatus    setBlue (RGBM* rgbm, const Adesk::UInt8 blue);
    static Adesk::UInt8         red     (const RGBM* rgbm);
    static Adesk::UInt8         green   (const RGBM* rgbm);
    static Adesk::UInt8         blue    (const RGBM* rgbm);

    // Method check
    static  bool                isByColor   (const RGBM* rgbm);
    static  bool                isByLayer   (const RGBM* rgbm);
    static  bool                isByBlock   (const RGBM* rgbm);
    static  bool                isByACI     (const RGBM* rgbm);
    static  bool                isByPen     (const RGBM* rgbm);
    static  bool                isForeground(const RGBM* rgbm);
    static  bool                isLayerOff  (const RGBM* rgbm);
    // Run time states.
    static  bool                isLayerFrozen(const RGBM* rgbm);
    static  bool                isNone      (const RGBM* rgbm);



    // Look up mapping between ACI and RGB.
    static Adesk::UInt32        trueColor       (const RGBM* rgbm);
    static Adesk::UInt8         trueColorMethod (const Adesk::Int16);
    static Acad::ErrorStatus    setTrueColor    (RGBM* rgbm);
    static Acad::ErrorStatus    setTrueColorMethod (RGBM* rgbm);

    static Adesk::UInt32        lookUpRGB       (const Adesk::UInt8 colorIndex);
    static Adesk::UInt8         lookUpACI       (const Adesk::UInt8 red,
                                                 const Adesk::UInt8 green,
                                                 const Adesk::UInt8 blue); 

protected:
    Acad::ErrorStatus           setIndirect (const Adesk::Int16 Indirect);
    Adesk::Int16                indirect() const;   
    RGBM mRGBM;

public:
    // The Look Up Table (LUT) provides a mapping between ACI and RGB 
    // and contains each ACI's representation in RGB.
    static const Adesk::UInt8 mLUT[256][3]; 
};

// AcCmEntityColor inline
//

// Default color method is kByColor.
inline 
AcCmEntityColor::AcCmEntityColor()
{
    mRGBM.whole = 0;
    mRGBM.colorMethod = kByColor;
}

inline
AcCmEntityColor::AcCmEntityColor (const AcCmEntityColor & color)
{
    mRGBM.whole = color.mRGBM.whole;
}


// parameter:   eColorMethod    Color method information (byBlock, byLayer, byColor).
inline 
AcCmEntityColor::AcCmEntityColor(const ColorMethod eColorMethod)
{
    mRGBM.whole = 0;
    mRGBM.colorMethod = static_cast<Adesk::UInt8>(eColorMethod);
}

// Default color method is kByColor.
// parameter:   red, green, blue
inline  
AcCmEntityColor::AcCmEntityColor(const Adesk::UInt8 red, 
                                 const Adesk::UInt8 green, 
                                 const Adesk::UInt8 blue)
{
    mRGBM.red   = red;
    mRGBM.green = green;
    mRGBM.blue  = blue;
    mRGBM.colorMethod = kByColor;
}

inline AcCmEntityColor& 
AcCmEntityColor::operator= (const AcCmEntityColor & color)
{
    mRGBM.whole = color.mRGBM.whole;
    return *this;
}

inline bool
AcCmEntityColor::operator==(const AcCmEntityColor& color) const
{
    return mRGBM.whole == color.mRGBM.whole;
}

inline bool
AcCmEntityColor::operator!=(const AcCmEntityColor& color) const
{
    return mRGBM.whole != color.mRGBM.whole;
}

// get Color Method
inline AcCmEntityColor::ColorMethod     
AcCmEntityColor::colorMethod() const
{
    return (ColorMethod) mRGBM.colorMethod;
}

// get RGB
//
// return value:    Depending on color method (fourth byte):
//                  kByLayer:   Layer index in first and second byte.
//                  kByColor:   RGB and color method.
//                              First byte blue, second byte green and third byte red.
//                              Fourth byte is color method.
//
//                  kByACI:     ACI index in first and second byte.
//
//                  kByBlock,   
//                  kForeground: Only color method (fourth byte) is valid.    
inline Adesk::UInt32    
AcCmEntityColor::color () const
{
    return mRGBM.whole;
}

// return value:    Layer index.
inline Adesk::Int16
AcCmEntityColor::layerIndex () const
{
    assert (mRGBM.colorMethod == kByLayer ||
            mRGBM.colorMethod == kLayerOff); 

    return mRGBM.indirect;
}

// return value:    Layer index.
inline Adesk::UInt16
AcCmEntityColor::penIndex () const
{
    return (Adesk::UInt16) mRGBM.indirect;
}

// get red
inline Adesk::UInt8    
AcCmEntityColor::red  () const
{
    return mRGBM.red;
}

// get green
inline Adesk::UInt8
AcCmEntityColor::green() const
{
    return mRGBM.green;
}

// get blue
inline Adesk::UInt8    
AcCmEntityColor::blue () const
{
    return mRGBM.blue;
}

inline bool
AcCmEntityColor::isByColor() const
{
    return mRGBM.colorMethod == kByColor; 
}

inline bool
AcCmEntityColor::isByLayer() const
{
    return (mRGBM.colorMethod  == kByLayer ||
            (mRGBM.colorMethod == kByACI   && 
             mRGBM.indirect    == kACIbyLayer)) ? true : false;    
}

inline bool
AcCmEntityColor::isByBlock() const
{
    return (mRGBM.colorMethod  == kByBlock ||
            (mRGBM.colorMethod == kByACI   && 
             mRGBM.indirect    == kACIbyBlock)) ? true : false;    
}

inline bool
AcCmEntityColor::isByACI()   const
{
    return mRGBM.colorMethod == kByACI;
}

inline bool
AcCmEntityColor::isByPen()   const
{
    return mRGBM.colorMethod == kByPen;
}

inline bool
AcCmEntityColor::isForeground()   const
{
    return (mRGBM.colorMethod  == kForeground ||
            (mRGBM.colorMethod == kByACI      && 
             mRGBM.indirect    == kACIforeground)) ? true : false;    
}

inline bool
AcCmEntityColor::isLayerOff() const
{   
    return (mRGBM.colorMethod  == kLayerOff ||
            (mRGBM.colorMethod == kByACI && 
             mRGBM.indirect    <  0      && // layer off for ACI is negative
             mRGBM.indirect    != kACIfrozenLayer)) ? true : false;    
}

inline bool
AcCmEntityColor::isLayerFrozen() const
{   
    return (mRGBM.colorMethod  == kLayerFrozen ||
            (mRGBM.colorMethod == kByACI       && 
             mRGBM.indirect    == kACIfrozenLayer)) ? true : false;    
}

inline bool
AcCmEntityColor::isLayerFrozenOrOff() const
{
    return isLayerFrozen() || isLayerOff();
}

inline bool
AcCmEntityColor::isNone()   const
{
    return (mRGBM.colorMethod  == kNone ||
            (mRGBM.colorMethod == kByACI       && 
             mRGBM.indirect    == kACInone)) ? true : false;    
}

// get indirect
//
// return value:    Indirect can be used for layer index and others.
inline Adesk::Int16    
AcCmEntityColor::indirect () const
{
    assert (mRGBM.colorMethod != kByColor); 
    return mRGBM.indirect;
}

// set indirect
// 
// parameter:   Indirect can be used for layer index and others.
inline Acad::ErrorStatus             
AcCmEntityColor::setIndirect  (const Adesk::Int16 Indirect)
{
    assert (mRGBM.colorMethod != kByColor); 
    mRGBM.indirect = Indirect;
    return Acad::eOk;
}

#pragma pack (pop)


class ADESK_NO_VTABLE AcCmColorBase
{
public:

    virtual ~AcCmColorBase() {}

    virtual AcCmEntityColor::ColorMethod  colorMethod() const = 0;;
    virtual Acad::ErrorStatus   setColorMethod(AcCmEntityColor::ColorMethod eColorMethod) = 0;

    virtual bool                isByColor() const = 0;
    virtual bool                isByLayer() const = 0;
    virtual bool                isByBlock() const = 0;
    virtual bool                isByACI()   const = 0;
    virtual bool                isByPen () const = 0;
    virtual bool                isForeground()   const = 0;

    virtual Adesk::UInt32       color() const = 0;
    virtual Acad::ErrorStatus   setColor(Adesk::UInt32 color) = 0;

    virtual Acad::ErrorStatus   setRGB  (Adesk::UInt8 red, 
                                         Adesk::UInt8 green, 
                                         Adesk::UInt8 blue) = 0;
    virtual Acad::ErrorStatus   setRed  (Adesk::UInt8 red) = 0;
    virtual Acad::ErrorStatus   setGreen(Adesk::UInt8 green) = 0;
    virtual Acad::ErrorStatus   setBlue (Adesk::UInt8 blue) = 0;
    virtual Adesk::UInt8        red  () const = 0;
    virtual Adesk::UInt8        green() const = 0;
    virtual Adesk::UInt8        blue () const = 0;

    virtual Adesk::UInt16       colorIndex() const = 0;
    virtual Acad::ErrorStatus   setColorIndex(Adesk::UInt16 colorIndex) = 0;
    virtual Adesk::UInt16       penIndex() const = 0;
    virtual Acad::ErrorStatus   setPenIndex (Adesk::UInt16 penIndex) = 0;

    virtual Acad::ErrorStatus   setNames(const ACHAR *colorName,
                                         const ACHAR *bookName = NULL) = 0;
    virtual const ACHAR *       colorName(void) const = 0;
    virtual const ACHAR *       bookName(void) const = 0;
    virtual const ACHAR *       colorNameForDisplay(void) = 0;
    virtual bool                hasColorName(void) const = 0;
    virtual bool                hasBookName(void) const = 0;

};


class AcCmTransparency {
public:

    enum transparencyMethod {
                kByLayer = 0,
                kByBlock,
                kByAlpha,


                kErrorValue     // must be last in enum
            };

    // Some Transparency constants
    enum {  kTransparencyByLayer    = (unsigned long)kByLayer,
            kTransparencyByBlock    = (unsigned long)kByBlock,
            kTransparencySolid      = ((unsigned long)(kByAlpha | (0xff << 24))),
            kTransparencyClear      = (unsigned long)kByAlpha
    };

    AcCmTransparency() { mAM.whole = kTransparencyByLayer; } // set all bytes
    AcCmTransparency(Adesk::UInt8 alpha);
    AcCmTransparency(double alphaPercent);
    AcCmTransparency(const AcCmTransparency& other) { mAM.whole = other.mAM.whole; }
    ~AcCmTransparency() {}

    AcCmTransparency& operator=(const AcCmTransparency& other);
    bool operator==(const AcCmTransparency& other) const;
    bool operator!=(const AcCmTransparency& other) const;

    void setAlpha(Adesk::UInt8 alpha);
    void setAlphaPercent(double alphaPercent);

    void setMethod(transparencyMethod method);

    Adesk::UInt8 alpha(void) const;
    double alphaPercent(void) const;

    bool isByAlpha(void) const { return (mAM.method == kByAlpha); }
    bool isByBlock(void) const { return (mAM.method == kByBlock); }
    bool isByLayer(void) const { return (mAM.method == kByLayer); }
    bool isClear(void) const;
    bool isSolid(void) const;

    Adesk::UInt32   serializeOut(void) const { return mAM.whole; }
    void            serializeIn(Adesk::UInt32);

private: 

    union AM {
        struct {
            Adesk::UInt8    alpha;          // low byte
            Adesk::UInt8    reserved1;    
            Adesk::UInt8    reserved2;
            Adesk::UInt8    method;         // high byte
        };
        Adesk::UInt32       whole;
    };

    AM mAM;
};


class AcCmDialogColor : public AcCmColorBase
{
public:

    AcCmDialogColor();
    AcCmDialogColor(const AcCmDialogColor& other);
    AcCmDialogColor(const AcCmColorBase& other);
    virtual ~AcCmDialogColor();

    AcCmDialogColor& operator=(const AcCmDialogColor& inputColor);
    AcCmDialogColor& operator=(const AcCmColorBase& inputColor);

    bool operator ==(const AcCmDialogColor& color) const;
    bool operator !=(const AcCmDialogColor& color) const;
    bool operator ==(const AcCmColorBase& color) const;
    bool operator !=(const AcCmColorBase& color) const;

    virtual AcCmEntityColor::ColorMethod  colorMethod() const;
    virtual Acad::ErrorStatus   setColorMethod(AcCmEntityColor::ColorMethod eColorMethod);

    virtual bool                isByColor() const;
    virtual bool                isByLayer() const;
    virtual bool                isByBlock() const;
    virtual bool                isByACI() const;
    virtual bool                isByPen () const;
    virtual bool                isForeground() const;

    virtual Adesk::UInt32       color() const;
    virtual Acad::ErrorStatus   setColor(Adesk::UInt32 color);

    virtual Acad::ErrorStatus   setRGB  (Adesk::UInt8 red, 
                                         Adesk::UInt8 green, 
                                         Adesk::UInt8 blue);
    virtual Acad::ErrorStatus   setRed  (Adesk::UInt8 red);
    virtual Acad::ErrorStatus   setGreen(Adesk::UInt8 green);
    virtual Acad::ErrorStatus   setBlue (Adesk::UInt8 blue);
    virtual Adesk::UInt8        red  () const;
    virtual Adesk::UInt8        green() const;
    virtual Adesk::UInt8        blue () const;

    virtual Adesk::UInt16       colorIndex() const;
    virtual Acad::ErrorStatus   setColorIndex(Adesk::UInt16 colorIndex);
    virtual Adesk::UInt16       penIndex() const;
    virtual Acad::ErrorStatus   setPenIndex (Adesk::UInt16 penIndex);

    virtual Acad::ErrorStatus   setNames(const ACHAR *colorName,
                                         const ACHAR *bookName = NULL);
    virtual const ACHAR *       colorName(void) const;
    virtual const ACHAR *       bookName(void) const;
    virtual const ACHAR *       colorNameForDisplay(void);
    virtual bool                hasColorName(void) const;
    virtual bool                hasBookName(void) const;


private:

    enum NameFlags {    kNoNames            = 0x00,
                        kHasColorName       = 0x01,
                        kHasBookName        = 0x02
                    };

    AcCmEntityColor::RGBM   mRGBM;
    ACHAR *                 mColorName;
    ACHAR *                 mBookName;
    Adesk::UInt8            mNameFlags;
};

namespace AcCm
{
    enum DialogTabs
    {   
        //these flags can be OR-ed 
        kACITab = 1,
        kTrueColorTab = 2,
        kColorBookTab = 4,
    };

    enum ACIColors
    {   
        kACIByBlock     = 0,
        kACIRed         = 1,
        kACIYellow      = 2,
        kACIGreen       = 3,
        kACICyan        = 4,
        kACIBlue        = 5,
        kACIMagenta     = 6,
        kACIForeground  = 7,
        kACIByLayer     = 256,
    };
}

#define ADSK_ACCMENTITYCOLOR_DEFINED
#include "acarrayhelper.h"

#endif // AD_DBCOLOR_H 
