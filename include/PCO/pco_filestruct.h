//-----------------------------------------------------------------//
// Name        | PCO_FileStruct.h            | Type: ( ) source    //
//-------------------------------------------|       (*) header    //
// Project     | PCO file saver class        |       ( ) others    //
//-----------------------------------------------------------------//
// Purpose     | PCO - File Save Class                             //
//-----------------------------------------------------------------//
// Author      | FRE, Excelitas PCO GmbH                           //
//-----------------------------------------------------------------//
// Notes       |                                                   //
//-----------------------------------------------------------------//
// (c) 2021 Excelitas PCO GmbH * Donaupark 11 *                    //
// D-93309      Kelheim / Germany * Phone: +49 (0)9441 / 2005-0 *  //
// Fax: +49 (0)9441 / 2005-20 * Email: pco@excelitas.com           //
//-----------------------------------------------------------------//



#if !defined PCO_FILESTRUCT
#define PCO_FILESTRUCT

#include <pco_color_corr_coeff.h>

#define FILEVERSION100 100
#define FILEVERSION200 200
#define FILEVERSION300 300
#define FILEVERSION301 301
#define FILEVERSION302 302             // added 05.2005 / beginning with CamWare 2.13
#define FILEVERSION303 303             // added 04.2008 / beginning with CamWare 2.21
#define FILEVERSION304 304             // added 05.2009 / beginning with CamWare 2.50
#define FILEVERSION305 305             // added 07.2009 / beginning with CamWare 2.50/3.00
#define FILEVERSION306 306             // added 04.2010 / beginning with CamWare 3.03
#define FILEVERSION307 307             // added 03.2011 / beginning with CamWare 3.05
#define FILEVERSION308 308             // added 01.2012 / beginning with CamWare 3.08
#define FILEVERSION402 402             // added 03.2016 / beginning with CamWare 4.02
#define FILEVERSION405 405             // added 09.2017 / beginning with CamWare 4.05
#define FILEVERSION    FILEVERSION405  //

#define FILE_SPLIT         0x00000001  // Split doubleshutter
#define FILE_SINGLE        0x00000002  // Save single images
#define FILE_NOTLAST       0x00000004  // Image is not the last one to save/load.
#define FILE_COL           0x00000008  // Image is color.
#define FILE_DOUBLE        0x00000010  // Image is doubleshutter.
//obsolete #define FILE_USEDISK       0x00000020  // Moves the MMFile to the disk-working one
#define FILE_INITNEW       0x00000040  // Forces Lib to init with new file name
#define FILE_CLOSEONLY     0x00000080  // Close only
#define FILE_OPENONLY      0x00000100  // Open only
#define FILE_INFOONLY      0x00000200  // Get info only
//obsolete #define FILE_TIFF16RGB     0x00000400  // Save as 16bit RGB Tiff
#define FILE_ISFIRST       0x00000800  // Image is first one
#define FILE_ISLAST        0x00001000  // Image is not the last one to save/load.
#define FILE_AUTOCLDLG     0x00002000  // Auto close dialog
#define FILE_USEIREAD4FIFO 0x00004000  // Use ireadimage for fifo size when saving MM files
#define FILE_SPLIT_DOUBLE1 0x00008000  // Split file into two halves (this one is upper half)
#define FILE_SPLIT_DOUBLE2 0x00010000  // Split file into two halves (this one is lower half)

#define FILE_CODECDLGONLY  0x00020000  // Proceed directly to Codec Dialog
#define FILE_FASTTRANSFER  0x00040000  // Use two alternating buffers for fast image readout
#define FILE_METADATAISON  0x00080000  // Flag to indicate Metadatamode is on
#define FILE_TIMESTAMPISON 0x00100000  // Flag to indicate Timestampmode is on
#define FILE_NOTRELOADABLE 0x00200000  // Flag to indicate the file can't be reloaded
#define FILE_NEEDCONVERT   0x00400000  // Flag to indicate the file needs converted image
#define FILE_LOADCONVERT   0x00800000  // Flag to indicate that we're going to load convert data from single image
#define FILE_NOBILDSTRUCT  0x01000000  // Flag to indicate that we do not save our Bild structure
                                       // This flag is applicable with all file types but b16
#define FILE_USEBILDPTR    0x02000000  // Flag to force save function to use the pointer supplied

// BOTTOM UP BGR is Bitmap format based and TOP DOWN RGB is tiff based. Other two cases aren't covered.
//#define FILE_DATAIS_BOTTOM_UP_BGR 0x02000000 // Data is bottom up BGR (if colored) format. Not needed as it is default for 8bit and all color formats!!!
// 16bit raw is always TOP DOWN!!
#define FILE_DATAIS_TOP_DOWN_RGB  0x04000000 // Data is top down RGB (if colored) format. Currently used for 48C3
// This dataformat assumes a contiguous buffer without padding like the bmp format

#define FILE_READ          0x80000000  // File Read

// Obsolete! Will be removed in future. Please use FILE_FILTER_ defines below
#define FILE_SAVE          0x00000001  // SAVE mask
#define FILE_LOAD          0x00000002  // LOAD mask
#define FILE_SAVE_ONLY     0x00000005  // SAVE mask
#define FILE_SAVE_SINGLE   0x00000010  // Only single file saver queried
#define FILE_ADD_CR        0x00000020  // Adds a CR after each entry to the string

#define FILE_FILTER_SAVE          0x00000001  // SAVE mask
#define FILE_FILTER_LOAD          0x00000002  // LOAD mask
#define FILE_FILTER_SAVE_ONLY     0x00000005  // SAVE mask
#define FILE_FILTER_SAVE_SINGLE   0x00000010  // Only single file saver queried
#define FILE_FILTER_ADD_CR        0x00000020  // Adds a CR after each entry to the string
#define FILE_FILTER_DIRECT_OK     0x00000040  // Only file types, which are usable for 'Direct record to file'
#define FILE_FILTER_ADD_ID        0x00000080  // Adds a the id of the dll at each entry to the string


#define FILETYPE_BASEMASK 0x0FF        // Mask for file type base
#define FILETYPE_BWMASK   0x100        // Mask for b/w
#define FILETYPE_RGBMASK  0x200        // Mask for RGB

#define FILETYPE_B16      0x001        // PCO 16bit RAW
#define FILETYPE_FTS16    0x002        // Fits 16bit RAW
#define FILETYPE_TIF16    0x003        // Tif 16bit RAW
#define FILETYPE_TIF16RGB 0x203        // Tif 3*16bit=48bit RGB
#define FILETYPE_TIF8     0x004        // Tif 8bit
#define FILETYPE_TIF8BW   0x104        // Tif 8bit b/w
#define FILETYPE_TIF8RGB  0x204        // Tif 8bit RGB
#define FILETYPE_BMP8     0x005        // Bmp
#define FILETYPE_BMP8BW   0x105        // Bmp 8bit b/w
#define FILETYPE_BMP8RGB  0x205        // Bmp 24bit RGB
#define FILETYPE_ASC16    0x006        // 16bit ASCII
#define FILETYPE_AVI8     0x007        // AVI
#define FILETYPE_AVI8BW   0x107        // AVI 8bit b/w
#define FILETYPE_AVI8RGB  0x207        // AVI 24bit RGB
#define FILETYPE_MPG8     0x008        // Mpeg
#define FILETYPE_MPG8BW   0x108        // Mpeg 8bit b/w
#define FILETYPE_MPG8RGB  0x208        // Mpeg 24bit RGB
#define FILETYPE_JPG8     0x009        // Jpeg
#define FILETYPE_JPG8BW   0x109        // Jpeg 8bit b/w
#define FILETYPE_JPG8RGB  0x209        // Jpeg 24bit RGB
#define FILETYPE_JP28     0x00A        // Jp2
#define FILETYPE_JP28BW   0x10A        // Jp2 8bit b/w
#define FILETYPE_JP28RGB  0x20A        // Jp2 24bit RGB
#define FILETYPE_WMV8     0x00B        // WMV
#define FILETYPE_WMV8BW   0x10B        // WMV 8bit b/w
#define FILETYPE_WMV8RGB  0x20B        // WMV 24bit RGB
#define FILETYPE_PCORAW   0x00C        // pco raw 16bit (bigtiff) file format
#define FILETYPE_MP4      0x00D        // MP4
#define FILETYPE_NOTAVAIL 0xFFF        // This file type is not known

#define FILETYPE_COLOR    0xFF00       // Mask for color type
#define FILETYPE_MULTI    0x80000000   // Supports multiple images in one file
#define FILETYPE_MULTI_ONLY 0x40000000 // Supports multiple images only in one file and no single files

// Lower type byte is reserved for b/w types and upper byte for color
// b/w types are 8, 16, 24, 32 bit, where 8, 16 are realized at the moment
#define FILETYPE_BIT8     0x0001        // 8bit, b/w,
#define FILETYPE_BIT16    0x0002        // 16bit, b/w
//#define FILETYPE_BIT24    0x0004        // 24bit, b/w, future use
//#define FILETYPE_BIT32    0x0008        // 32bit, b/w, future use

// color types are 24, 32, 48, 64, 72, 96, 128 bit, where 24, 32, 48 are realized at the moment
#define FILETYPE_BIT24C3   0x0100        // 24bit (3x8), color
#define FILETYPE_BIT32C4   0x0200        // 32bit (4x8), color
#define FILETYPE_BIT48C3   0x0400        // 48bit (3x16), color
//#define FILETYPE_BIT64C4   0x0800        // 64bit (4x16), color, future use
//#define FILETYPE_BIT72C3   0x1000        // 72bit (3x24), color, future use
//#define FILETYPE_BIT96C4   0x2000        // 96bit (4x24), color, future use
//#define FILETYPE_BIT96C3   0x4000        // 96bit (3x32), color, future use
//#define FILETYPE_BIT128C4  0x8000        // 128bit(4x32), color, future use

#define FILETYPE_DIRECT_OK  0x10000        // Filetype is fast enough to be used for direct record to file


// These types can be combined to whatever the plug-in is able to save/load

#define DATACOMPRESSION_UNCOMPRESSED       0 // Data is uncompressed or decompressed
#define DATACOMPRESSION_16TO12SQUAREROOT   1 // Data is compr. to 12bit by root function
#define DATACOMPRESSION_ALREADY_DECOMPRESSED 0x80000000 // Data is already decompressed

#define DATACOMPRESSION_VERSION 1      // Version number of compression

// Structure packing
#pragma pack(push,4)

typedef struct
{
  WORD wx, wy;                         // x,y - coordinates
  WORD wsh, wos;                       // Slope, offset of pixels
  WORD wcorr;                          // Correction factor
  BYTE ucneighbour, ucd1;              // Neighbour (0 means no hotpixel neighbour, Bits-1248: clockwise
  WORD wd1, wd2;                       // starting with 12 oclock(1). ---Rest is dummies
}str_hpx;

#define HEADERLEN 256                  // in DWORDs, 1024 bytes
#define HEADERLEN_IN_BYTE 1024         // in BYTE

#define DUMMYNUM  96                   //
typedef struct
{
  WORD* pic12;                         // original image pointer  // 4 byte on 32bit OS; 8 byte on 64bit
  SYSTEMTIME sTime;                    // shows the exact time stamp of the image  // 20 byte
  int        iTicks;                   // milliseconds gone after start in stime
  int        iXRes;                    // X Resolution
  int        iYRes;                    // Y Resolution                             // 32 byte
  char cText[40];                      // text which should be placed inside the image// 72 byte
  bool       bDouble;                  // shows if pic is Doubleshutter image
  bool       bThisisT0;                // True: This image is T0 image; False: -
  bool       bWeHaveT0;                // True: Image stream has got T0
  BYTE       bBytePerPixel;            // since bool is only one byte, we've to fill up to four bytes// 76 byte
  int        iBWMin;                   // Lut bw min                               // 80 byte
  int        iBWMax;                   // Lut bw max
  int        iBWLut;                   // Lut lin log
  int        iRMin;                    // red min                                  // 92 byte
  int        iRMax;                    // red max
  int        iGMin;                    // green min                                // 100 byte
  int        iGMax;                    // green max
  int        iBMin;                    // blue min
  int        iBMax;                    // blue max                                 // 112 byte
  int        iColLut;                  // Lut lin log color
  int        iColor;                   // image from Color-CCD: 1 otherwise 0      // 120 byte
  int        iVersion;                 // Version of b16 extended info
  // V200: ENDE
  int        iBWMin2;                   // Lut bw min
  int        iBWMax2;                   // Lut bw max                              // 132 byte
  int        iBWLut2;                   // Lut lin log
  int        iRMin2;                    // red min                                 // 140 byte
  int        iRMax2;                    // red max
  int        iGMin2;                    // green min
  int        iGMax2;                    // green max                               // 152 byte
  int        iBMin2;                    // blue min
  int        iBMax2;                    // blue max                                // 160 byte
  int        iColLut2;                  // Lut lin log color
  bool       bAlignUpper;               // Align MSB (0-MSB is bit14, 1-MSB is bit 16)
  bool       bDummy2[3];                // since bool is only one byte, we've to fill up to four bytes // 168 byte
  // V300: ENDE
  double     dGammaLut;                 // Gamma value b/w
  double     dGammaLutC;                // Gamma value color
  double     dGammaLut2;                // Gamma value b/w 2
  double     dGammaLutC2;               // Gamma value color 2                     // 200 byte
  int        iColorPatternType;         // Demosaicking type for the color pattern
  int        iBitRes;                   // Bit resolution of image                 // 208 byte
  // V301: ENDE
  double     dSaturation;               // Color saturation common for both ds images // 216 byte
  // V302/V303: ENDE
  int        iSaturation;               // new color saturation                    // 220
  int        iVibrance;
  int        iColorTemp;
  int        iColorTint;
  int        iContrast;
  int        iGamma;                    // new Gamma                               // 240
  // V304: ENDE
  SRGBCOLCORRCOEFF strColorCoeff;       // 9*double->72 byte                       // 312
  // V305: ENDE

  int        iHasTimeStamp;             // Image has got a timestamp // 316

  WORD   wHasMetaData;               // 0->no meta data available, 1->has meta data
  WORD   wSENSOR_CONV_FACTOR;        // sensor conversions factor in e-/ct, 0 if unknown
  WORD   wCAMERA_TYPE;               // type of pco camera taking the images, see SDK, 0 if unknown
  WORD   wDARK_OFFSET;               // nominal dark offset in counts, 0xFFFF if unknown, current dark offset may differ
  // 324

  BYTE   bIMAGE_COUNTER_BCD[4];      // 0x00000001 to 0x99999999, where first byte is least sign. byte
  BYTE   bIMAGE_TIME_US_BCD[3];      // 0x000000 to 0x999999, where first byte is least significant byte
  BYTE   bIMAGE_TIME_STATUS;         // 0x00 = internal osc, 0x01 = synced by IRIG, 0x02 = synced by master
  // 332

  WORD   wEXPOSURE_TIME_BASE;        // timebase ns/us/ms for following exposure time
  SHORT  sSENSOR_TEMPERATURE;        // current sensor temperature in degree celsius, 0x8000 if not known
  DWORD  dwEXPOSURE_TIME;            // exposure time in ns/us/ms  according to timebase
  // 340

  DWORD  dwFRAMERATE_MILLIHZ;        // framerate in mHz, 0 if unknown or not
  BYTE   bBINNING_X;                 // binning in x direction, 0x00 if unknown or n/a
  BYTE   bBINNING_Y;                 // binning in y direction, 0x00 if unknown or n/a
  BYTE   bTRIGGER_MODE;              // exposure trigger mode, see PCO SDK
  BYTE   bCAMERA_SYNC_MODE;          // see PCO SDK
  // 348

  DWORD  dwSENSOR_READOUT_FREQUENCY; // sensor readout frequency in Hz, 0 if unknown
  DWORD  dwCAMERA_SERIAL_NO;         // camera serial no, 0 if unknown
  // 356
  // V306: ENDE
  BYTE   bSYNC_STATUS;
  BYTE   bImage_Type;
  WORD   wColorPattern;

  WORD   wCAMERA_SUBTYPE;            // sub-type of pco camera taking the images, see SDK, 0 if unknown
  WORD   wMetaDataDummy1;
  DWORD  dwEVENT_NUMBER;             // application dependent event number
// 64 bytes ..
  WORD   wIMAGE_SIZE_X_Offset;       // actual x offset in case of ROI (X0) in x direction (horizontal)
  WORD   wIMAGE_SIZE_Y_Offset;       // actual y offset in case of ROI (Y0) in y direction (vertical)

  DWORD  dwMetaDataDummy[16];
  // 436

  DWORD  dwCompression;              // File format compression 0: none
  DWORD  dwCompressionVersion;       // 0
  DWORD  dwPredictorStart;           // Compression predictor: 0
  DWORD  dwRandomStart;              // Random lut start value
  DWORD  dwRandomIndex;              // Random lut index for this image
  // 456
  __int32 iDiffToT0;                 // Difference to T0 image if applicable
  // 460
  char   cFluorophore[36];           // Fluorophore name (CCI-Overlay)
  WORD   wSeqSize;
  WORD   wWavelength;                // Fluorphore wavelength
  DWORD  dwRGBColor;                 // COLORREF based; 0x00RRGGBB; Base color for Pseudo-LUT (CCI-Overlay)
  WORD   wNumInSeq;
  WORD   wNumSeq;
  // 504

  WORD   wIntensifierVoltage;        // Intensifier voltage of Dicam
  WORD   wGatingMode;                // Gating mode of Dicam
  DWORD  dwPhosphorDecay;            // Phosphor decay of Dicam
  DWORD  zzDummy[DUMMYNUM];
}Bild;// ATTENTION: 32 Bytes free, otherwise headerlen in file12 must be adapted!
//   Headerlen         = 1024
// - old Header          = 128
// New header length     = 896
// - Bild (w.o. WORD*)   = 448
// results in free bytes = 448

typedef struct
{
  char ucPco[4];
  unsigned int uiFileLen;
  unsigned int uiHeaderLen;
  unsigned int uiXRes;
  unsigned int uiYRes;
  unsigned int uiLutSign;
  unsigned int uiColor;
  unsigned int uiBWMin;
  unsigned int uiBWMax;
  unsigned int uiBWLut;
  unsigned int uiRMin;
  unsigned int uiRMax;
  unsigned int uiGMin;
  unsigned int uiGMax;
  unsigned int uiBMin;
  unsigned int uiBMax;
  unsigned int uiColLut;
  unsigned int uiDS;
  unsigned int uiDummy[HEADERLEN-18];
}B16_HEADER;

// Structure packing
#pragma pack(pop)


#endif //PCO_FILESTRUCT

#if defined CREATE_REBUILDSTRUCT_FUNC
#if !defined REBUILDSTRUCT
#define REBUILDSTRUCT
void RebuildStruct(Bild* pb16,
                   int iAlignUpper /* 0:file, 1:FALSE, 2:TRUE*/,
                   int iBitRes /* 0: file, others set to value */)
{
  if((pb16->iVersion > FILEVERSION) || (pb16->iVersion <= FILEVERSION100))
  {
    pb16->iXRes = 0;
    pb16->iYRes = 0;
  }
  if(pb16->iVersion < FILEVERSION200)
  {
    pb16->iBWMin2 = pb16->iBWMin;                   // Lut bw min
    pb16->iBWMax2 = pb16->iBWMax;                   // Lut bw max
    pb16->iBWLut2 = pb16->iBWLut;                   // Lut lin log
    pb16->iRMin2 = pb16->iRMin;                    // red min
    pb16->iRMax2 = pb16->iRMax;                    // red max
    pb16->iGMin2 = pb16->iGMin;                    // green min
    pb16->iGMax2 = pb16->iGMax;                    // green max
    pb16->iBMin2 = pb16->iBMin;                    // blue min
    pb16->iBMax2 = pb16->iBMax;                    // blue max
    pb16->iColLut2 = pb16->iColLut;                  // Lut lin log color
    if(iAlignUpper == 0)
      pb16->bAlignUpper = FALSE;
    if(iAlignUpper == 1)
      pb16->bAlignUpper = FALSE;
    if(iAlignUpper == 2)
      pb16->bAlignUpper = TRUE;
  }

  if(pb16->iVersion < FILEVERSION300)
  {
    pb16->dGammaLut = 1.0;                 // Gamma value b/w
    pb16->dGammaLutC = 1.0;                // Gamma value color
    pb16->dGammaLut2 = 1.0;                // Gamma value b/w 2
    pb16->dGammaLutC2 = 1.0;               // Gamma value color 2
  }

  if(pb16->iVersion < FILEVERSION301)
  {
    if((pb16->iColorPatternType < 0) || (pb16->iColorPatternType > 3))
      pb16->iColorPatternType = 0;
    if(iBitRes == 0)
      pb16->iBitRes = 14;                    // Assume 14bit resolution
    else
      pb16->iBitRes = iBitRes;
  }

  if(pb16->iVersion < FILEVERSION302)
  {
    pb16->dSaturation = 100;
    if(pb16->iBitRes == 12)
      pb16->bAlignUpper = FALSE;
  }
  if(pb16->iVersion < FILEVERSION304)
  {
    pb16->iSaturation = 0;
    pb16->iVibrance = 0;
    pb16->iColorTemp = 5000;
    pb16->iColorTint = 0;
    pb16->iContrast = 0;
    pb16->iGamma = 100;
  }
  if(pb16->iVersion < FILEVERSION305)
  {
    pb16->strColorCoeff.da11 = 1.0; pb16->strColorCoeff.da12 = 0.0; pb16->strColorCoeff.da13 = 0.0;
    pb16->strColorCoeff.da21 = 0.0; pb16->strColorCoeff.da22 = 1.0; pb16->strColorCoeff.da23 = 0.0;
    pb16->strColorCoeff.da31 = 0.0; pb16->strColorCoeff.da32 = 0.0; pb16->strColorCoeff.da33 = 1.0;
  }
  if(pb16->iVersion < FILEVERSION306)
  {
    pb16->iHasTimeStamp = 0;
    pb16->wHasMetaData = 0;
    pb16->wSENSOR_CONV_FACTOR = 0;
    pb16->wCAMERA_TYPE = 0;
    pb16->wDARK_OFFSET = 0;
    pb16->bIMAGE_COUNTER_BCD[0] = 0;
    pb16->bIMAGE_COUNTER_BCD[1] = 0;
    pb16->bIMAGE_COUNTER_BCD[2] = 0;
    pb16->bIMAGE_COUNTER_BCD[3] = 0;
    pb16->bIMAGE_TIME_US_BCD[0] = 0;
    pb16->bIMAGE_TIME_US_BCD[1] = 0;
    pb16->bIMAGE_TIME_US_BCD[2] = 0;
    pb16->bIMAGE_TIME_STATUS = 0;
    pb16->wEXPOSURE_TIME_BASE = 0;
    pb16->sSENSOR_TEMPERATURE = (-32768);
    pb16->dwEXPOSURE_TIME = 0;
    pb16->dwFRAMERATE_MILLIHZ = 0;
    pb16->bBINNING_X = 0;
    pb16->bBINNING_Y = 0;
    pb16->bTRIGGER_MODE = 0;
    pb16->bCAMERA_SYNC_MODE = 0;
    pb16->dwSENSOR_READOUT_FREQUENCY = 0;
    pb16->dwCAMERA_SERIAL_NO = 0;
  }
  if(pb16->iVersion < FILEVERSION307)
  {
    for(int i = 0; i < 16; i++)
      pb16->dwMetaDataDummy[i] = 0;

    pb16->dwCompression = 0;           // File format compression 0: none
    pb16->dwCompressionVersion = 0;    // 0
    pb16->dwPredictorStart = 0;        // Compression predictor: 0
    pb16->dwRandomStart = 0;              // Random lut start value
    pb16->dwRandomIndex = 0;              // Random lut index for this image
  }
  if(pb16->iVersion < FILEVERSION308)  // 308 has got new tint calculation
  {
    if(pb16->iColorTint < 0)
    {
      pb16->iColorTint /= 2;
    }
  }

  bool bchanged = false;
  if(pb16->bBINNING_X < 0)
  {
    pb16->bBINNING_X = 0;
    bchanged = true;
  }
  if(pb16->bBINNING_X > 32)
  {
    pb16->bBINNING_X = 32;
    bchanged = true;
  }
  if(pb16->bBINNING_Y < 0)
  {
    pb16->bBINNING_Y = 0;
    bchanged = true;
  }
  if(pb16->bBINNING_Y > 32)
  {
    pb16->bBINNING_Y = 32;
    bchanged = true;
  }
  if (pb16->bCAMERA_SYNC_MODE < 0)
  {
    pb16->bCAMERA_SYNC_MODE = 0;
    bchanged = true;
  }
  if(pb16->bCAMERA_SYNC_MODE > 100)
  {
    pb16->bCAMERA_SYNC_MODE = 100;
    bchanged = true;
  }
  if(pb16->bImage_Type < 0)
  {
    pb16->bImage_Type = 0;
    bchanged = true;
  }
  if(pb16->bImage_Type > 100)
  {
    pb16->bImage_Type = 0;
    bchanged = true;
  }
  if(pb16->bSYNC_STATUS < 0)
  {
    pb16->bSYNC_STATUS = 0;
    bchanged = true;
  }
  if(pb16->bSYNC_STATUS > 100)
  {
    pb16->bSYNC_STATUS = 0;
    bchanged = true;
  }
  if(pb16->bTRIGGER_MODE < 0)
  {
    pb16->bTRIGGER_MODE = 0;
    bchanged = true;
  }
  if(pb16->bTRIGGER_MODE > 100)
  {
    pb16->bTRIGGER_MODE = 0;
    bchanged = true;
  }
  if(pb16->iBitRes < 0)
  {
    pb16->iBitRes = 16;
    bchanged = true;
  }
  if(pb16->iBitRes > 16)
  {
    pb16->iBitRes = 16;
    bchanged = true;
  }

  if ((pb16->bBytePerPixel == 0) || (pb16->bBytePerPixel > 2))
  {
    pb16->bBytePerPixel = (BYTE)((pb16->iBitRes + 7) / 8);
    bchanged = true;
  }

  if(pb16->iColorPatternType < 0)
  {
    pb16->iColorPatternType = 0;
    bchanged = true;
  }
  if(pb16->iColorPatternType > 100)
  {
    pb16->iColorPatternType = 0;
    bchanged = true;
  }

  if(bchanged)
  {
    snprintf(pb16->cText, 40, "SETTINGS ADAPTED DUE TO WRONG VALUES!");
  }



  if((pb16->iColorTemp < 1000) || (pb16->iColorTemp > 20000))
    pb16->iColorTemp = 6500;
  double dcheckcol = 0.0;
  dcheckcol += pb16->strColorCoeff.da11; dcheckcol += pb16->strColorCoeff.da12; dcheckcol += pb16->strColorCoeff.da13;
  dcheckcol += pb16->strColorCoeff.da21; dcheckcol += pb16->strColorCoeff.da22; dcheckcol += pb16->strColorCoeff.da23;
  dcheckcol += pb16->strColorCoeff.da31; dcheckcol += pb16->strColorCoeff.da32; dcheckcol += pb16->strColorCoeff.da33;

  if((dcheckcol <= 0) || (dcheckcol > 20.0))
  {
    pb16->strColorCoeff.da11 = 1.0; pb16->strColorCoeff.da12 = 0.0; pb16->strColorCoeff.da13 = 0.0;
    pb16->strColorCoeff.da21 = 0.0; pb16->strColorCoeff.da22 = 1.0; pb16->strColorCoeff.da23 = 0.0;
    pb16->strColorCoeff.da31 = 0.0; pb16->strColorCoeff.da32 = 0.0; pb16->strColorCoeff.da33 = 1.0;
  }
  if((pb16->iGamma < 40) || (pb16->iGamma > 250))
    pb16->iGamma = 100;
  memset(&pb16->zzDummy[0], 0, DUMMYNUM * sizeof(DWORD));

  if (pb16->iVersion < FILEVERSION402)
    pb16->bThisisT0 = false;
  if(pb16->iVersion < FILEVERSION405)
  {
    pb16->bWeHaveT0 = false;
    pb16->iDiffToT0 = 0;
  }

  pb16->iVersion = FILEVERSION;
}
#endif
#else
extern void RebuildStruct(Bild* pb16,
                   int iAlignUpper /* 0:file, 1:FALSE, 2:TRUE*/,
                   int iBitRes /* 0: file, others set to value */);
#endif
