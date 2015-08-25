/******************************************************************************
   Copyright (C) Roper Scientific, Inc. 1990-2012. All rights reserved.
******************************************************************************/

#ifndef _PVCAM_H
#define _PVCAM_H
/*********************** Constant & Type Definitions *************************/

/************************ Class 2: Data types ********************************/

/* GUID for FRAME_INFO structure */
typedef struct _TAG_PVCAM_FRAME_INFO_GUID
{
    uns32 f1;
    uns16 f2;
    uns16 f3;
    uns8  f4[8];
}
PVCAM_FRAME_INFO_GUID, PV_PTR_DECL PPVCAM_FRAME_INFO_GUID;

/* FRAME_INFO currently holding the sequential frame number and timestamp */
typedef struct _TAG_FRAME_INFO
{
    PVCAM_FRAME_INFO_GUID FrameInfoGUID;
    int16 hCam;
    int32 FrameNr;
    long64 TimeStamp;
    int32 ReadoutTime;
    long64 TimeStampBOF;
}
FRAME_INFO, PV_PTR_DECL PFRAME_INFO;

/********************** Class 0: Open Camera Modes ***************************/
/*
  Function: pl_cam_open()
*/
typedef enum
{
    OPEN_EXCLUSIVE
}
PL_OPEN_MODES;

/************************ Class 1: Error message size ************************/
#define ERROR_MSG_LEN  255      /* No error message will be longer than this */

/*********************** Class 2: Cooling type flags *************************/
/* used with the PARAM_COOLING_MODE parameter id.
*/
typedef enum
{
    NORMAL_COOL, CRYO_COOL
}
PL_COOL_MODES;

/************************** Class 2: Name/ID sizes ***************************/
#define CCD_NAME_LEN  17           /* Includes space for the null terminator */
#define MAX_ALPHA_SER_NUM_LEN 32   /* Includes space for the null terminator */
#define MAX_PP_NAME_LEN 32

/*********************** Class 2: MPP capability flags ***********************/
/* used with the PARAM_MPP_CAPABLE parameter id.                             */
typedef enum
{
    MPP_UNKNOWN, MPP_ALWAYS_OFF, MPP_ALWAYS_ON, MPP_SELECTABLE
}
PL_MPP_MODES;

/************************** Class 2: Shutter flags ***************************/
/* used with the PARAM_SHTR_STATUS parameter id.
*/
typedef enum
{
    SHTR_FAULT, SHTR_OPENING, SHTR_OPEN, SHTR_CLOSING, SHTR_CLOSED,
    SHTR_UNKNOWN
}
PL_SHTR_MODES;

/************************ Class 2: Pmode constants ***************************/
/* used with the PARAM_PMODE parameter id.                                   */
typedef enum
{
    PMODE_NORMAL, PMODE_FT, PMODE_MPP, PMODE_FT_MPP,
    PMODE_ALT_NORMAL, PMODE_ALT_FT, PMODE_ALT_MPP, PMODE_ALT_FT_MPP,
    PMODE_INTERLINE
}
PL_PMODES;

/************************ Class 2: Color support constants *******************/
/* used with the PARAM_COLOR_MODE parameter id.                              */
typedef enum
{
    COLOR_NONE, COLOR_RGGB = 2
}
PL_COLOR_MODES;

/************************ Class 2: Attribute IDs *****************************/
/*
  Function: pl_get_param()
*/
typedef enum
{
    ATTR_CURRENT, ATTR_COUNT, ATTR_TYPE, ATTR_MIN, ATTR_MAX, ATTR_DEFAULT,
    ATTR_INCREMENT, ATTR_ACCESS, ATTR_AVAIL
}
PL_PARAM_ATTRIBUTES;

/************************ Class 2: Access types ******************************/
/*
  Function: pl_get_param( ATTR_ACCESS )
*/
typedef enum
{
    ACC_ERROR, ACC_READ_ONLY, ACC_READ_WRITE, ACC_EXIST_CHECK_ONLY,
    ACC_WRITE_ONLY
}
PL_PARAM_ACCESS;
/* This enum is used by the access Attribute */

/************************ Class 2: I/O types *********************************/
/* used with the PARAM_IO_TYPE parameter id.                                 */
typedef enum
{
    IO_TYPE_TTL, IO_TYPE_DAC
}
PL_IO_TYPE;

/************************ Class 2: I/O direction flags ***********************/
/* used with the PARAM_IO_DIRECTION parameter id.                            */
typedef enum
{
    IO_DIR_INPUT, IO_DIR_OUTPUT, IO_DIR_INPUT_OUTPUT
}
PL_IO_DIRECTION;

/************************ Class 2: I/O port attributes ***********************/
typedef enum
{
    IO_ATTR_DIR_FIXED, IO_ATTR_DIR_VARIABLE_ALWAYS_READ
}
PL_IO_ACCESS;


/************************ Class 2: Readout Port ******************************/
/* used with the PARAM_READOUT_PORT parameter id.                            */
typedef enum
{
    READOUT_PORT_MULT_GAIN = 0,
    READOUT_PORT_NORMAL,
    READOUT_PORT_LOW_NOISE,
    READOUT_PORT_HIGH_CAP,
    /* deprecated */
    READOUT_PORT1 = 0,
    READOUT_PORT2 = 1
}
PL_READOUT_PORTS;

/************************ Class 2: Clearing mode flags ***********************/
/* used with the PARAM_CLEAR_MODE parameter id.                              */
typedef enum
{
    CLEAR_NEVER, CLEAR_PRE_EXPOSURE, CLEAR_PRE_SEQUENCE, CLEAR_POST_SEQUENCE,
    CLEAR_PRE_POST_SEQUENCE, CLEAR_PRE_EXPOSURE_POST_SEQ, MAX_CLEAR_MODE
}
PL_CLEAR_MODES;

/************************ Class 2: Shutter mode flags ************************/
/*
  Function: pl_set_param ( PARAM_SHTR_OPEN_MODE )
*/
typedef enum
{
    OPEN_NEVER, OPEN_PRE_EXPOSURE, OPEN_PRE_SEQUENCE, OPEN_PRE_TRIGGER,
    OPEN_NO_CHANGE
}
PL_SHTR_OPEN_MODES;

/************************ Class 2: Exposure mode flags ***********************/
/* used with the PARAM_EXPOSURE_MODE parameter id.
  Functions: pl_exp_setup_cont()
             pl_exp_setup_seq()

*/
typedef enum
{
    TIMED_MODE, STROBED_MODE, BULB_MODE, TRIGGER_FIRST_MODE, FLASH_MODE,
    VARIABLE_TIMED_MODE, INT_STROBE_MODE,
    MAX_EXPOSE_MODE
}
PL_EXPOSURE_MODES;

/**************** Class 2: Post Processing Feature Identifiers ****************/
/* used with the PARAM_PP_FEAT_ID parameter id.                              */

typedef enum
{
    PP_FEATURE_RING_FUNCTION,
    PP_FEATURE_BIAS,
    PP_FEATURE_BERT,
    PP_FEATURE_QUANT_VIEW,
    PP_FEATURE_BLACK_LOCK,
    PP_FEATURE_TOP_LOCK,
    PP_FEATURE_VARI_BIT,
    PP_FEATURE_DESPECKLE_HIGH,
    PP_FEATURE_DESPECKLE_LOW,
    PP_FEATURE_MAX
}
PP_FEATURE_IDS;

/**************** Class 2: Post Processing Module Identifiers ****************/
/* used with the PARAM_PP_PARAM_ID parameter id.                             */

#define PP_MAX_PARAMETERS_PER_FEATURE   10

typedef enum
{
    PP_PARAMETER_RF_FUNCTION         = (PP_FEATURE_RING_FUNCTION * PP_MAX_PARAMETERS_PER_FEATURE),
    PP_FEATURE_BIAS_ENABLED          = (PP_FEATURE_BIAS * PP_MAX_PARAMETERS_PER_FEATURE),
    PP_FEATURE_BIAS_LEVEL,
    PP_FEATURE_BERT_ENABLED          = (PP_FEATURE_BERT * PP_MAX_PARAMETERS_PER_FEATURE),
    PP_FEATURE_BERT_THRESHOLD,
    PP_FEATURE_QUANT_VIEW_ENABLED    = (PP_FEATURE_QUANT_VIEW * PP_MAX_PARAMETERS_PER_FEATURE),
    PP_FEATURE_QUANT_VIEW_E,
    PP_FEATURE_BLACK_LOCK_ENABLED    = (PP_FEATURE_BLACK_LOCK * PP_MAX_PARAMETERS_PER_FEATURE),
    PP_FEATURE_BLACK_LOCK_BLACK_CLIP,
    PP_FEATURE_TOP_LOCK_ENABLED      = (PP_FEATURE_TOP_LOCK * PP_MAX_PARAMETERS_PER_FEATURE),
    PP_FEATURE_TOP_LOCK_WHITE_CLIP,
    PP_FEATURE_VARI_BIT_ENABLED      = (PP_FEATURE_VARI_BIT * PP_MAX_PARAMETERS_PER_FEATURE),
    PP_FEATURE_VARI_BIT_BIT_DEPTH,
    PP_PARAMETER_ID_MAX
}
PP_PARAMETER_IDS;

/**************** Class 2: S.M.A.R.T Streaming  ****************/
/* used with the PARAM_PP_PARAM_ID parameter id.                             */
typedef struct
{
    uns16     entries;         /* The number of entries in the array         */
    uns32_ptr params;          /* The actual S.M.A.R.T. stream parameters    */
}
smart_stream_type, PV_PTR_DECL smart_stream_type_ptr;

/********************** Class 2: S.M.A.R.T Streaming Modes *******************/
typedef enum
{
    SMTMODE_ARBITRARY_ALL = 0,
    SMTMODE_MAX
}
PL_SMT_MODES;

/********************** Class 3: Readout status flags ************************/
/*
  Function: pl_exp_check_status()

    if NEWDATARDY or NEWDATAFIXED     READOUT_COMPLETE
    else if RUNNING                   ACQUISITION_IN_PROGRESS
    else if INITIALIZED or DONEDCOK   READOUT_NOT_ACTIVE
    else                              READOUT_FAILED

*/
typedef enum
{
    READOUT_NOT_ACTIVE,
    EXPOSURE_IN_PROGRESS,
    READOUT_IN_PROGRESS,
    READOUT_COMPLETE,                   /* Means frame available for a circular buffer acq */
    FRAME_AVAILABLE = READOUT_COMPLETE, /* New camera status indicating at least one frame is available */
    READOUT_FAILED,
    ACQUISITION_IN_PROGRESS,
    MAX_CAMERA_STATUS
}
PL_IMAGE_STATUSES;

/********************** Class 3: Abort Exposure flags ************************/
/*
  Function: pl_exp_abort()
*/
typedef enum
{
    CCS_NO_CHANGE = 0, CCS_HALT, CCS_HALT_CLOSE_SHTR, CCS_CLEAR,
    CCS_CLEAR_CLOSE_SHTR, CCS_OPEN_SHTR, CCS_CLEAR_OPEN_SHTR
}
PL_CCS_ABORT_MODES;

/************************ Class 3: EOF/BOF constants *************************/
/* used with the PARAM_BOF_EOF_ENABLE parameter id.                          */
typedef enum
{
    NO_FRAME_IRQS = 0, BEGIN_FRAME_IRQS, END_FRAME_IRQS, BEGIN_END_FRAME_IRQS
}
PL_IRQ_MODES;

/************************ Class 3: Continuous Mode constants *****************/
/*
  Function: pl_exp_setup_cont()
*/
typedef enum
{
    CIRC_NONE = 0, CIRC_OVERWRITE, CIRC_NO_OVERWRITE
}
PL_CIRC_MODES;

/************************ Class 3: Fast Exposure Resolution constants ********/
/* used with the PARAM_EXP_RES parameter id.                                 */
typedef enum
{
    EXP_RES_ONE_MILLISEC = 0, EXP_RES_ONE_MICROSEC, EXP_RES_ONE_SEC
}
PL_EXP_RES_MODES;

/************************ Class 3: I/O Script Locations **********************/
typedef enum
{
    SCR_PRE_OPEN_SHTR = 0, SCR_POST_OPEN_SHTR, SCR_PRE_FLASH, SCR_POST_FLASH,
    SCR_PRE_INTEGRATE, SCR_POST_INTEGRATE, SCR_PRE_READOUT, SCR_POST_READOUT,
    SCR_PRE_CLOSE_SHTR, SCR_POST_CLOSE_SHTR
}
PL_SRC_MODES;

/************************ Class 3: Callback constants *************************/

typedef enum
{
    PL_CALLBACK_BOF = 0,
    PL_CALLBACK_EOF,
    PL_CALLBACK_CHECK_CAMS,
    PL_CALLBACK_CAM_REMOVED,
    PL_CALLBACK_CAM_RESUMED,
    PL_CALLBACK_MAX
}
PL_CALLBACK_EVENT;

/************************* Class 3: Region Definition ************************/
typedef struct
{
    uns16 s1;                     /* First pixel in the serial register */
    uns16 s2;                     /* Last pixel in the serial register */
    uns16 sbin;                   /* Serial binning for this region */
    uns16 p1;                     /* First row in the parallel register */
    uns16 p2;                     /* Last row in the parallel register */
    uns16 pbin;                   /* Parallel binning for this region */
}
rgn_type, PV_PTR_DECL rgn_ptr;
typedef const rgn_type PV_PTR_DECL rgn_const_ptr;

/********************** Unsupported types ************************************/
typedef struct io_struct
{
    uns16 io_port;                        /* I/O port address                  */
    uns32 io_type;                        /* I/O port type (TTL, DAC, etc.)    */
    flt64 state;                          /* Desired output state for the port */
    struct io_struct PV_PTR_DECL next;    /* Linked list pointer               */
}
io_entry, PV_PTR_DECL io_entry_ptr;

typedef struct
{
    io_entry pre_open;
    io_entry post_open;
    io_entry pre_flash;
    io_entry post_flash;
    io_entry pre_integrate;
    io_entry post_integrate;
    io_entry pre_readout;
    io_entry post_readout;
    io_entry pre_close;
    io_entry post_close;
}
io_list, PV_PTR_DECL io_list_ptr, PV_PTR_DECL PV_PTR_DECL io_list_ptr_ptr;

typedef struct
{
    uns16 shutter_close_delay; /* # of milliseconds for the shutter to close   */
    uns16 shutter_open_delay;  /* # of milliseconds for the shutter to open    */
    uns16 rows;                /* Parallel size of the CCD active area         */
    uns16 cols;                /* Serial size of the CCD active area           */
    uns16 prescan;             /* Serial pixels before the active area         */
    uns16 postscan;            /* Serial pixels after the active area          */
    uns16 premask;             /* Parallel rows before the active area         */
    uns16 postmask;            /* Parallel rows after the active area          */
    uns16 preflash;            /* # of milliseconds to flash the diode ring    */
    uns16 clear_count;         /* # of times to clear the CCD before exposure  */
    uns16 preamp_delay;        /* # of milliseconds for the preamp to settle   */
    rs_bool mpp_selectable;    /* Indicates MPP mode can be selected           */
    rs_bool frame_selectable;  /* Indicates frame transfer can be selected     */
    int16 do_clear;            /* Clear: Never, Each Exposure, Each Sequence   */
    int16 open_shutter;        /* Open: Never, Each Exposure, Each Sequence    */
    rs_bool mpp_mode;          /* Enable or disable MPP mode                   */
    rs_bool frame_transfer;    /* Enable or disable frame transfer operation   */
    rs_bool alt_mode;          /* Enable or disable Alternate Parallel mode    */
    uns32 exp_res;             /* Exposure resolution                          */
    io_list_ptr io_hdr;        /* Pointer to list of I/O script control cmds   */
}
active_camera_type, PV_PTR_DECL active_camera_ptr;

/************************* SINGLE-BYTE MACROS ********************************/
/* These will pull out a single uns8 from either a two-uns8 integer quantity,*/
/* or a four-uns8 integer quantity.  They ARE NOT machine specific.  The     */
/* software using them is responsible for handling the interface requirements*/
/* of the NGC camera, which expects the high uns8 first, then the lower bytes*/
/* in order.  There are several macros:                                      */
/*                                                                           */
/*   ??16_BYTE extract one of two bytes from a two-uns8 integer input        */
/*   VAL_UNS16 produces a two-uns8 integer value from high & low uns8 input  */
/*   ??32_BYTE extracts one of four bytes from a four-uns8 integer input     */
/*   VAL_UNS32 produces a four-uns8 integer value from 4 input bytes         */

#define MS16_BYTE(two_byte_value) ((uns8) ((two_byte_value) >> 8))
#define LS16_BYTE(two_byte_value) ((uns8) (two_byte_value))

#define VAL_UNS16(ms_byte,ls_byte) ( (uns16)(((uns16)((uns8)(ms_byte))<<8) |\
                                             ((uns16)((uns8)(ls_byte)))) )

#define MS32_BYTE(four_byte_val) ((uns8) ((four_byte_val) >> 24))
#define MH32_BYTE(four_byte_val) ((uns8) ((four_byte_val) >> 16))
#define ML32_BYTE(four_byte_val) ((uns8) ((four_byte_val) >> 8))
#define LS32_BYTE(four_byte_val) ((uns8) (four_byte_val))

#define VAL_UNS32(ms_byte,mh_byte,ml_byte,ls_byte) (\
  ((uns32)((uns8)(ms_byte))<<24) |\
  ((uns32)((uns8)(mh_byte))<<16) |\
  ((uns32)((uns8)(ml_byte))<<8)  |\
  ((uns32)((uns8)(ls_byte)) ) )

/***************** Content which is needed to communicate **********************/
/***************** with the PVCAM DLLs *****************************************/

typedef int16 pm_script_hook (int16 hcam,
                              uns16 exp_total,
                              uns16 rgn_total,
                              rgn_const_ptr rgn_array,
                              int16 mode,
                              uns32 exposure_time,
                              uns32_ptr pixels, active_camera_ptr);

/* Data type used by pl_get_param with attribute type (ATTR_TYPE).           */
#define TYPE_INT16                  1
#define TYPE_INT32                  2
#define TYPE_FLT64                  4
#define TYPE_UNS8                   5
#define TYPE_UNS16                  6
#define TYPE_UNS32                  7
#define TYPE_UNS64                  8
#define TYPE_ENUM                   9
#define TYPE_BOOLEAN               11
#define TYPE_INT8                  12
#define TYPE_CHAR_PTR              13
#define TYPE_VOID_PTR              14
#define TYPE_VOID_PTR_PTR          15
#define TYPE_INT64                 16
#define TYPE_SMART_STREAM_TYPE     17
#define TYPE_SMART_STREAM_TYPE_PTR 18


/* defines for classes                                                       */
#define CLASS0      0          /* Camera Communications                      */
#define CLASS2      2          /* Configuration/Setup                        */
#define CLASS3      3          /* Data Acuisition                            */
#define CLASS93     93         /* Fast frame operations (PIV/ACCUM/Kinetics) */
/************************ Parameter IDs **************************************/
/* Format: TTCCxxxx, where TT = Data type, CC = Class, xxxx = ID number      */


/* DEVICE DRIVER PARAMETERS (CLASS 0) */

/*  Class 0 (next available index for class zero = 6) */

#define PARAM_DD_INFO_LENGTH        ((CLASS0<<16) + (TYPE_INT16<<24) + 1)
#define PARAM_DD_VERSION            ((CLASS0<<16) + (TYPE_UNS16<<24) + 2)
#define PARAM_DD_RETRIES            ((CLASS0<<16) + (TYPE_UNS16<<24) + 3)
#define PARAM_DD_TIMEOUT            ((CLASS0<<16) + (TYPE_UNS16<<24) + 4)
#define PARAM_DD_INFO               ((CLASS0<<16) + (TYPE_CHAR_PTR<<24) + 5)

/* Camera Parameters Class 2 variables */

/* Class 2 (next available index for class two = 544) */

/* CCD skip parameters                                                       */

/* ADC offset setting.                                                       */
#define PARAM_ADC_OFFSET            ((CLASS2<<16) + (TYPE_INT16<<24)     + 195)
/* CCD chip name.    */
#define PARAM_CHIP_NAME             ((CLASS2<<16) + (TYPE_CHAR_PTR<<24)  + 129)

#define PARAM_COOLING_MODE          ((CLASS2<<16) + (TYPE_ENUM<<24)      + 214)
#define PARAM_PREAMP_DELAY          ((CLASS2<<16) + (TYPE_UNS16<<24)     + 502)
#define PARAM_COLOR_MODE            ((CLASS2<<16) + (TYPE_ENUM<<24)      + 504)
#define PARAM_MPP_CAPABLE           ((CLASS2<<16) + (TYPE_ENUM<<24)      + 224)
#define PARAM_PREAMP_OFF_CONTROL    ((CLASS2<<16) + (TYPE_UNS32<<24)     + 507)
#define PARAM_SERIAL_NUM            ((CLASS2<<16) + (TYPE_UNS16<<24)     + 508)

/* CCD Dimensions and physical characteristics                               */
/* pre and post dummies of CCD.                                              */
#define PARAM_PREMASK               ((CLASS2<<16) + (TYPE_UNS16<<24)     +  53)
#define PARAM_PRESCAN               ((CLASS2<<16) + (TYPE_UNS16<<24)     +  55)
#define PARAM_POSTMASK              ((CLASS2<<16) + (TYPE_UNS16<<24)     +  54)
#define PARAM_POSTSCAN              ((CLASS2<<16) + (TYPE_UNS16<<24)     +  56)
#define PARAM_PIX_PAR_DIST          ((CLASS2<<16) + (TYPE_UNS16<<24)     + 500)
#define PARAM_PIX_PAR_SIZE          ((CLASS2<<16) + (TYPE_UNS16<<24)     +  63)
#define PARAM_PIX_SER_DIST          ((CLASS2<<16) + (TYPE_UNS16<<24)     + 501)
#define PARAM_PIX_SER_SIZE          ((CLASS2<<16) + (TYPE_UNS16<<24)     +  62)
#define PARAM_SUMMING_WELL          ((CLASS2<<16) + (TYPE_BOOLEAN<<24)   + 505)
#define PARAM_FWELL_CAPACITY        ((CLASS2<<16) + (TYPE_UNS32<<24)     + 506)
/* Y dimension of active area of CCD chip */
#define PARAM_PAR_SIZE              ((CLASS2<<16) + (TYPE_UNS16<<24)     +  57)
/* X dimension of active area of CCD chip */
#define PARAM_SER_SIZE              ((CLASS2<<16) + (TYPE_UNS16<<24)     +  58)
/* X dimension of active area of CCD chip */
#define PARAM_ACCUM_CAPABLE         ((CLASS2<<16) + (TYPE_BOOLEAN<<24)   + 538)
#define PARAM_FLASH_DWNLD_CAPABLE   ((CLASS2<<16) + (TYPE_BOOLEAN<<24)   + 539)

/* General parameters */

/* Readout time of current ROI, in ms */
#define PARAM_READOUT_TIME          ((CLASS2<<16) + (TYPE_FLT64<<24)     + 179)

/* CAMERA PARAMETERS (CLASS 2) */
#define PARAM_CLEAR_CYCLES          ((CLASS2<<16) + (TYPE_UNS16<<24)     + 97)
#define PARAM_CLEAR_MODE            ((CLASS2<<16) + (TYPE_ENUM<<24)      + 523)
#define PARAM_FRAME_CAPABLE         ((CLASS2<<16) + (TYPE_BOOLEAN<<24)   + 509)
#define PARAM_PMODE                 ((CLASS2<<16) + (TYPE_ENUM <<24)     + 524)
#define PARAM_CCS_STATUS            ((CLASS2<<16) + (TYPE_INT16<<24)     + 510)

/* This is the actual temperature of the detector. This is only a get, not a */
/* set                                                                       */
#define PARAM_TEMP                  ((CLASS2<<16) + (TYPE_INT16<<24)     + 525)
/* This is the desired temperature to set. */
#define PARAM_TEMP_SETPOINT         ((CLASS2<<16) + (TYPE_INT16<<24)     + 526)
#define PARAM_CAM_FW_VERSION        ((CLASS2<<16) + (TYPE_UNS16<<24)     + 532)
#define PARAM_HEAD_SER_NUM_ALPHA    ((CLASS2<<16) + (TYPE_CHAR_PTR<<24)  + 533)
#define PARAM_PCI_FW_VERSION        ((CLASS2<<16) + (TYPE_UNS16<<24)     + 534)
#define PARAM_CAM_FW_FULL_VERSION   ((CLASS2<<16) + (TYPE_CHAR_PTR<<24)  + 534)

/* Exsposure mode, timed strobed etc, etc */
#define PARAM_EXPOSURE_MODE         ((CLASS2<<16) + (TYPE_ENUM<<24)      + 535)
#define PARAM_EXPOSE_OUT_MODE       ((CLASS2<<16) + (TYPE_ENUM<<24)      + 560)

/* SPEED TABLE PARAMETERS (CLASS 2) */

#define PARAM_BIT_DEPTH             ((CLASS2<<16) + (TYPE_INT16<<24)     + 511)
#define PARAM_GAIN_INDEX            ((CLASS2<<16) + (TYPE_INT16<<24)     + 512)
#define PARAM_SPDTAB_INDEX          ((CLASS2<<16) + (TYPE_INT16<<24)     + 513)
/* define which port (amplifier on shift register) to use. */
#define PARAM_READOUT_PORT          ((CLASS2<<16) + (TYPE_ENUM<<24)      + 247)
#define PARAM_PIX_TIME              ((CLASS2<<16) + (TYPE_UNS16<<24)     + 516)

/* SHUTTER PARAMETERS (CLASS 2) */

#define PARAM_SHTR_CLOSE_DELAY      ((CLASS2<<16) + (TYPE_UNS16<<24)     + 519)
#define PARAM_SHTR_OPEN_DELAY       ((CLASS2<<16) + (TYPE_UNS16<<24)     + 520)
#define PARAM_SHTR_OPEN_MODE        ((CLASS2<<16) + (TYPE_ENUM <<24)     + 521)
#define PARAM_SHTR_STATUS           ((CLASS2<<16) + (TYPE_ENUM <<24)     + 522)

/* I/O PARAMETERS (CLASS 2) */

#define PARAM_IO_ADDR               ((CLASS2<<16) + (TYPE_UNS16<<24)     + 527)
#define PARAM_IO_TYPE               ((CLASS2<<16) + (TYPE_ENUM<<24)      + 528)
#define PARAM_IO_DIRECTION          ((CLASS2<<16) + (TYPE_ENUM<<24)      + 529)
#define PARAM_IO_STATE              ((CLASS2<<16) + (TYPE_FLT64<<24)     + 530)
#define PARAM_IO_BITDEPTH           ((CLASS2<<16) + (TYPE_UNS16<<24)     + 531)

/* GAIN MULTIPLIER PARAMETERS (CLASS 2) */

#define PARAM_GAIN_MULT_FACTOR      ((CLASS2<<16) + (TYPE_UNS16<<24)     + 537)
#define PARAM_GAIN_MULT_ENABLE      ((CLASS2<<16) + (TYPE_BOOLEAN<<24)   + 541)

/*  POST PROCESSING PARAMETERS (CLASS 2) */
#define PARAM_PP_FEAT_NAME          ((CLASS2<<16) + (TYPE_CHAR_PTR<<24)    +  542)
#define PARAM_PP_INDEX              ((CLASS2<<16) + (TYPE_INT16<<24)    +  543)
#define PARAM_ACTUAL_GAIN           ((CLASS2<<16) + (TYPE_UNS16<<24)    +  544)
#define PARAM_PP_PARAM_INDEX        ((CLASS2<<16) + (TYPE_INT16<<24)    +  545)
#define PARAM_PP_PARAM_NAME         ((CLASS2<<16) + (TYPE_CHAR_PTR<<24)    +  546)
#define PARAM_PP_PARAM              ((CLASS2<<16) + (TYPE_UNS32<<24)    +  547)
#define PARAM_READ_NOISE            ((CLASS2<<16) + (TYPE_UNS16<<24)    +  548)
#define PARAM_PP_FEAT_ID            ((CLASS2<<16) + (TYPE_UNS16<<24)    +  549)
#define PARAM_PP_PARAM_ID           ((CLASS2<<16) + (TYPE_UNS16<<24)    +  550)


/*  S.M.A.R.T. STREAMING PARAMETERS (CLASS 2) */
#define PARAM_SMART_STREAM_MODE_ENABLED     ((CLASS2<<16) + (TYPE_BOOLEAN<<24)    +  700)
#define PARAM_SMART_STREAM_MODE             ((CLASS2<<16) + (TYPE_UNS16<<24)    +  701)
#define PARAM_SMART_STREAM_EXP_PARAMS       ((CLASS2<<16) + (TYPE_VOID_PTR<<24)    +  702)
#define PARAM_SMART_STREAM_DLY_PARAMS       ((CLASS2<<16) + (TYPE_VOID_PTR<<24)    +  703)

/* Digital image correction parameters (CLASS 2) */
#define PARAM_COLUMN_CORRECTION_STATE        ((CLASS2<<16) + (TYPE_BOOLEAN<<24)    +  704)

/* ACQUISITION PARAMETERS (CLASS 3) */
#define PARAM_EXP_TIME              ((CLASS3<<16) + (TYPE_UNS16<<24)     +   1)
#define PARAM_EXP_RES               ((CLASS3<<16) + (TYPE_ENUM<<24)      +   2)
#define PARAM_EXP_MIN_TIME          ((CLASS3<<16) + (TYPE_FLT64<<24)     +   3)
#define PARAM_EXP_RES_INDEX         ((CLASS3<<16) + (TYPE_UNS16<<24)     +   4)

/* PARAMETERS FOR  BEGIN and END of FRAME Interrupts */
#define PARAM_BOF_EOF_ENABLE        ((CLASS3<<16) + (TYPE_ENUM<<24)      +   5)
#define PARAM_BOF_EOF_COUNT         ((CLASS3<<16) + (TYPE_UNS32<<24)     +   6)
#define PARAM_BOF_EOF_CLR           ((CLASS3<<16) + (TYPE_BOOLEAN<<24)   +   7)

/* Test to see if hardware/software can perform circular buffer */
#define PARAM_CIRC_BUFFER           ((CLASS3<<16) + (TYPE_BOOLEAN<<24)   + 299)

#define PARAM_CURRENT_PVTIME        ((CLASS3<<16) + (TYPE_INT16<<24)     + 167)

/**************************** Function Prototypes ****************************/

#ifndef PV_EMBEDDED

#ifdef PV_C_PLUS_PLUS
extern "C"
{
#endif

    /*****************************************************************************/
    /*****************************************************************************/
    /*                                                                           */
    /*             Class 0: Camera Communications Function Prototypes            */
    /*                                                                           */
    /*****************************************************************************/
    /*****************************************************************************/

    /*****************************************************************************/
    /* rs_bool (RETURN)  All functions that return a rs_bool return TRUE for     */
    /*                   success and FALSE for failure.  If a failure occurs     */
    /*                   pl_error_code() and pl_error_message() can be used to   */
    /*                   determine the cause.                                    */
    /*****************************************************************************/

    /*****************************************************************************/
    /* pvcam_version     Version number of the PVCAM library                     */
    /*                     16 bits = MMMMMMMMrrrrTTTT where MMMMMMMM = Major #,  */
    /*                     rrrr = Minor #, and TTTT = Trivial #                  */
    /*****************************************************************************/

    rs_bool PV_DECL pl_pvcam_get_ver (uns16_ptr pvcam_version);
    rs_bool PV_DECL pl_pvcam_init (void);
    rs_bool PV_DECL pl_pvcam_uninit (void);

    /*****************************************************************************/
    /* hcam              Camera handle returned from pl_cam_open()               */
    /* cam_num           Camera number Range: 0 through (totl_cams-1)            */
    /* camera_name       Text name assigned to a camera (with RSConfig)          */
    /* totl_cams         Total number of cameras in the system                   */
    /* o_mode            Mode to open the camera in (must be OPEN_EXCLUSIVE)     */
    /*****************************************************************************/

    rs_bool PV_DECL pl_cam_check (int16 hcam);
    rs_bool PV_DECL pl_cam_close (int16 hcam);
    rs_bool PV_DECL pl_cam_get_name (int16 cam_num, char_ptr camera_name);
    rs_bool PV_DECL pl_cam_get_total (int16_ptr totl_cams);
    rs_bool PV_DECL pl_cam_open (char_ptr camera_name, int16_ptr hcam,
                                 int16 o_mode);

    rs_bool PV_DECL pl_cam_register_callback (int16 hcam, PL_CALLBACK_EVENT CallbackEvent, void *Callback);
    rs_bool PV_DECL pl_cam_register_callback_ex (int16 hcam, PL_CALLBACK_EVENT CallbackEvent, void *Callback, void *Context);
    rs_bool PV_DECL pl_cam_register_callback_ex2(int16 hcam, PL_CALLBACK_EVENT CallbackEvent, void *Callback);
    rs_bool PV_DECL pl_cam_register_callback_ex3(int16 hcam, PL_CALLBACK_EVENT CallbackEvent, void *Callback, void *Context);
    rs_bool PV_DECL pl_cam_deregister_callback (int16 hcam, PL_CALLBACK_EVENT CallbackEvent);

    /*****************************************************************************/
    /*****************************************************************************/
    /*                                                                           */
    /*                Class 1: Error Reporting Function Prototypes               */
    /*                                                                           */
    /*****************************************************************************/
    /*****************************************************************************/

    /*****************************************************************************/
    /* int16 (RETURN)    pl_error_code(void) returns the error code of the last  */
    /*                   pl_ function call.                                      */
    /* err_code          Unique ID of the error: returned from pl_error_code()   */
    /* msg               Text description of err_code.                           */
    /*****************************************************************************/

    int16   PV_DECL pl_error_code (void);
    rs_bool PV_DECL pl_error_message (int16 err_code, char_ptr msg);


    /*****************************************************************************/
    /*****************************************************************************/
    /*                                                                           */
    /*              Class 2: Configuration/Setup Function Prototypes             */
    /*                                                                           */
    /*****************************************************************************/
    /*****************************************************************************/

    /*****************************************************************************/
    /* param_id          ID of the parameter to get or set (PARAM_...)           */
    /* param_attribute   Attribute of the parameter to get (ATTR_...)            */
    /* param_value       Value to get or set                                     */
    /* index             Index of enumeration Range: 0 through N-1 ... where N   */
    /*                     is retrieved with get_param(...,ATTR_COUNT,...)       */
    /* value             Numerical value of enumeration                          */
    /* desc              Text description of enumeration                         */
    /* length            Length of text description of enumeration               */
    /*****************************************************************************/

    rs_bool PV_DECL pl_get_param (int16 hcam, uns32 param_id,
                                  int16 param_attribute, void_ptr param_value);
    rs_bool PV_DECL pl_set_param (int16 hcam, uns32 param_id,
                                  void_ptr param_value);
    rs_bool PV_DECL pl_get_enum_param (int16 hcam, uns32 param_id, uns32 index,
                                       int32_ptr value, char_ptr desc,
                                       uns32 length);
    rs_bool PV_DECL pl_enum_str_length (int16 hcam, uns32 param_id, uns32 index,
                                        uns32_ptr length);
    rs_bool PV_DECL pl_pp_reset (int16 hcam);

    rs_bool PV_DECL pl_create_smart_stream_struct(smart_stream_type_ptr *array,
            uns16 entries);

    rs_bool PV_DECL pl_release_smart_stream_struct(smart_stream_type_ptr *array);

    rs_bool PV_DECL pl_create_frame_info_struct(PFRAME_INFO * new_frame);

    rs_bool PV_DECL pl_release_frame_info_struct(PFRAME_INFO frame_to_delete);



    /*****************************************************************************/
    /*****************************************************************************/
    /*                                                                           */
    /*               Class 3: Data Acquisition Function Prototypes               */
    /*                                                                           */
    /*****************************************************************************/
    /*****************************************************************************/

    /*****************************************************************************/
    /* pixel_stream      Buffer to hold image(s)                                 */
    /* byte_cnt          Size of bufer to hold images (in bytes)                 */
    /* exp_total         Total number of exposures to take                       */
    /* rgn_total         Total number of regions defined for each image          */
    /* rgn_array         Array of regions (must be rgn_total in size)            */
    /*                     s1    starting pixel in the serial register           */
    /*                     s2    ending pixel in the serial register             */
    /*                     sbin  serial binning for this region                  */
    /*                     p1    starting pixel in the parallel register         */
    /*                     p2    ending pixel in the parallel register           */
    /*                     pbin  parallel binning for this region                */
    /* exp_mode          Mode for capture (TIMED_MODE, STROBED_MODE, ...)        */
    /* exposure_time     Time to expose in selected exposure resolution          */
    /*                     Default is milliseconds (see PARAM_EXP_RES)           */
    /* exp_bytes         Value returned from PVCAM specifying the required       */
    /*                     number of bytes to allocate for the capture           */
    /* buffer_mode       Circular buffer mode (CIRC_OVERWRITE,...)               */
    /* size              Size of continuous capture pixel_stream                 */
    /*                     (must be a multiple of byte_cnt)                      */
    /* status            Status of the current capture (EXPOSURE_IN_PROGRESS,...)*/
    /* bytes_arrived     Number of bytes that have arrived.  For continuous      */
    /*                     mode this is the number of bytes that have arrived    */
    /*                     this time through the buffer.                         */
    /* buffer_cnt        Number of times through the buffer (continuous mode)    */
    /* frame             Pointer to the requested image                          */
    /* cam_state         State to set the camera in (CCS_NO_CHANGE,...)          */
    /* hbuf              Standard image buffer                                   */
    /* exposure          Exposure # to unravel, 65535 for All, else exposure #   */
    /* array_list        Array of Pointers that will get the unraveled images    */
    /*                     in the same order as the regions.                     */
    /* tlimit            Time in milliseconds to wait for a transfer             */
    /*****************************************************************************/

    rs_bool PV_DECL pl_exp_init_seq (void);
    rs_bool PV_DECL pl_exp_uninit_seq (void);
    rs_bool PV_DECL pl_exp_setup_seq (int16 hcam, uns16 exp_total,
                                      uns16 rgn_total, rgn_const_ptr rgn_array,
                                      int16 exp_mode, uns32 exposure_time,
                                      uns32_ptr exp_bytes);
    rs_bool PV_DECL pl_exp_start_seq (int16 hcam, void_ptr pixel_stream);
    rs_bool PV_DECL pl_exp_setup_cont (int16 hcam, uns16 rgn_total,
                                       rgn_const_ptr rgn_array, int16 exp_mode,
                                       uns32 exposure_time, uns32_ptr exp_bytes,
                                       int16 buffer_mode);
    rs_bool PV_DECL pl_exp_start_cont (int16 hcam, void_ptr pixel_stream,
                                       uns32 size);
    rs_bool PV_DECL pl_exp_check_status (int16 hcam, int16_ptr status,
                                         uns32_ptr bytes_arrived);
    rs_bool PV_DECL pl_exp_check_cont_status (int16 hcam, int16_ptr status,
            uns32_ptr bytes_arrived,
            uns32_ptr buffer_cnt);
    rs_bool PV_DECL pl_exp_check_cont_status_ex (int16 hcam, int16_ptr status, uns32_ptr byte_cnt, uns32_ptr buffer_cnt, PFRAME_INFO pFrameInfo);
    rs_bool PV_DECL pl_exp_get_latest_frame (int16 hcam, void_ptr_ptr frame);
    rs_bool PV_DECL pl_exp_get_latest_frame_ex (int16 hcam, void_ptr_ptr frame, PFRAME_INFO pFrameInfo);
    rs_bool PV_DECL pl_exp_get_oldest_frame (int16 hcam, void_ptr_ptr frame);
    rs_bool PV_DECL pl_exp_get_oldest_frame_ex (int16 hcam, void_ptr_ptr frame, PFRAME_INFO pFrameInfo);
    rs_bool PV_DECL pl_exp_unlock_oldest_frame (int16 hcam);
    rs_bool PV_DECL pl_exp_stop_cont (int16 hcam, int16 cam_state);
    rs_bool PV_DECL pl_exp_abort (int16 hcam, int16 cam_state);
    rs_bool PV_DECL pl_exp_finish_seq (int16 hcam, void_ptr pixel_stream,
                                       int16 hbuf);

    /*****************************************************************************/
    /* addr              Specifies which I/O address to control                  */
    /* state             Specifies the value to write to the register            */
    /* location          Specifies when to control the I/O (SCR_PRE_FLASH,...)   */
    /*****************************************************************************/
    rs_bool PV_DECL pl_io_script_control (int16 hcam, uns16 addr, flt64 state,
                                          uns32 location);
    rs_bool PV_DECL pl_io_clear_script_control (int16 hcam);

    /*****************************************************************************/
    /* The following functions are obsolete and their corresponding PARAM_       */
    /* parameters should be used with pl_get_param(), pl_set_param(),            */
    /* pl_get_enum_param(), and pl_enum_str_length()                             */
    /*****************************************************************************/

    rs_bool PV_DECL pl_dd_get_info (int16 hcam, int16 bytes, char_ptr text);
    /* Use PARAM_DD_INFO                                              */
    rs_bool PV_DECL pl_dd_get_info_length (int16 hcam, int16_ptr bytes);
    /* Use PARAM_DD_INFO_LENGTH                                       */
    rs_bool PV_DECL pl_dd_get_ver (int16 hcam, uns16_ptr dd_version);
    /* Use PARAM_DD_VERSION                                           */
    rs_bool PV_DECL pl_dd_get_retries (int16 hcam, uns16_ptr max_retries);
    rs_bool PV_DECL pl_dd_set_retries (int16 hcam, uns16 max_retries);
    /* Use PARAM_DD_RETRIES                                           */
    rs_bool PV_DECL pl_dd_get_timeout (int16 hcam, uns16_ptr m_sec);
    rs_bool PV_DECL pl_dd_set_timeout (int16 hcam, uns16 m_sec);
    /* Use PARAM_DD_TIMEOUT                                           */
    rs_bool PV_DECL pl_ccd_get_adc_offset (int16 hcam, int16_ptr offset);
    rs_bool PV_DECL pl_ccd_set_adc_offset (int16 hcam, int16 offset);
    /* Use PARAM_ADC_OFFSET                                           */
    rs_bool PV_DECL pl_ccd_get_chip_name (int16 hcam, char_ptr chip_name);
    /* Use PARAM_CHIP_NAME                                            */
    rs_bool PV_DECL pl_ccd_get_clear_cycles (int16 hcam, uns16_ptr clear_cycles);
    rs_bool PV_DECL pl_ccd_set_clear_cycles (int16 hcam, uns16 clr_cycles);
    /* Use PARAM_CLEAR_CYCLES                                         */
    rs_bool PV_DECL pl_ccd_get_clear_mode (int16 hcam, int16_ptr clear_mode);
    rs_bool PV_DECL pl_ccd_set_clear_mode (int16 hcam, int16 ccd_clear);
    /* Use PARAM_CLEAR_MODE                                           */
    rs_bool PV_DECL pl_ccd_get_color_mode (int16 hcam, uns16_ptr color_mode);
    /* Use PARAM_COLOR_MODE                                           */
    rs_bool PV_DECL pl_ccd_get_cooling_mode (int16 hcam, int16_ptr cooling);
    /* Use PARAM_COOLING_MODE                                         */
    rs_bool PV_DECL pl_ccd_get_frame_capable (int16 hcam,
            rs_bool_ptr frame_capable);
    /* Use PARAM_FRAME_CAPABLE                                        */
    rs_bool PV_DECL pl_ccd_get_fwell_capacity (int16 hcam,
            uns32_ptr fwell_capacity);
    /* Use PARAM_FWELL_CAPACITY                                       */
    rs_bool PV_DECL pl_ccd_get_mpp_capable (int16 hcam, int16_ptr mpp_capable);
    /* Use PARAM_MPP_CAPABLE                                          */
    rs_bool PV_DECL pl_ccd_get_preamp_dly (int16 hcam, uns16_ptr preamp_dly);
    /* Use PARAM_PREAMP_DELAY                                         */
    rs_bool PV_DECL pl_ccd_get_preamp_off_control (int16 hcam,
            uns32_ptr preamp_off_control);
    rs_bool PV_DECL pl_ccd_set_preamp_off_control (int16 hcam,
            uns32 preamp_off_control);
    /* Use PARAM_PREAMP_OFF_CONTROL                                   */
    rs_bool PV_DECL pl_ccd_get_preflash (int16 hcam, uns16_ptr pre_flash);
    /* Use PARAM_PREFLASH                                             */
    rs_bool PV_DECL pl_ccd_get_pmode (int16 hcam, int16_ptr pmode);
    rs_bool PV_DECL pl_ccd_set_pmode (int16 hcam, int16 pmode);
    /* Use PARAM_PMODE                                                */
    rs_bool PV_DECL pl_ccd_get_premask (int16 hcam, uns16_ptr pre_mask);
    /* Use PARAM_PREMASK                                              */
    rs_bool PV_DECL pl_ccd_get_prescan (int16 hcam, uns16_ptr prescan);
    /* Use PARAM_PRESCAN                                              */
    rs_bool PV_DECL pl_ccd_get_postmask (int16 hcam, uns16_ptr post_mask);
    /* Use PARAM_POSTMASK                                             */
    rs_bool PV_DECL pl_ccd_get_postscan (int16 hcam, uns16_ptr postscan);
    /* Use PARAM_POSTSCAN                                             */
    rs_bool PV_DECL pl_ccd_get_par_size (int16 hcam, uns16_ptr par_size);
    /* Use PARAM_PAR_SIZE                                             */
    rs_bool PV_DECL pl_ccd_get_ser_size (int16 hcam, uns16_ptr ser_size);
    /* Use PARAM_SER_SIZE                                             */
    rs_bool PV_DECL pl_ccd_get_serial_num (int16 hcam, uns16_ptr serial_num);
    /* Use PARAM_SERIAL_NUM                                           */
    rs_bool PV_DECL pl_ccs_get_status (int16 hcam, int16_ptr ccs_status);
    /* Use PARAM_CCS_STATUS                                           */
    rs_bool PV_DECL pl_ccd_get_summing_well (int16 hcam,
            rs_bool_ptr s_well_exists);
    /* Use PARAM_SUMMING_WELL                                         */
    rs_bool PV_DECL pl_ccd_get_tmp (int16 hcam, int16_ptr cur_tmp);
    rs_bool PV_DECL pl_ccd_get_tmp_range (int16 hcam, int16_ptr tmp_hi_val,
                                          int16_ptr tmp_lo_val);
    /* Use PARAM_TEMP                                                 */
    rs_bool PV_DECL pl_ccd_get_tmp_setpoint (int16 hcam, int16_ptr tmp_setpoint);
    rs_bool PV_DECL pl_ccd_set_tmp_setpoint (int16 hcam, int16 tmp_setpoint);
    /* Use PARAM_TEMP_SETPOINT                                        */
    rs_bool PV_DECL pl_ccd_set_readout_port (int16 , int16 );
    rs_bool PV_DECL pl_ccd_get_pix_par_dist (int16 hcam, uns16_ptr pix_par_dist);
    /* Use PARAM_PIX_PAR_DIST                                         */
    rs_bool PV_DECL pl_ccd_get_pix_par_size (int16 hcam, uns16_ptr pix_par_size);
    /* Use PARAM_PIX_PAR_SIZE                                         */
    rs_bool PV_DECL pl_ccd_get_pix_ser_dist (int16 hcam, uns16_ptr pix_ser_dist);
    /* Use PARAM_PIX_SER_DIST                                         */
    rs_bool PV_DECL pl_ccd_get_pix_ser_size (int16 hcam, uns16_ptr pix_ser_size);
    /* Use PARAM_PIX_SER_SIZE                                         */
    rs_bool PV_DECL pl_spdtab_get_bits (int16 hcam, int16_ptr spdtab_bits);
    /* Use PARAM_BIT_DEPTH                                            */
    rs_bool PV_DECL pl_spdtab_get_gain (int16 hcam, int16_ptr spdtab_gain);
    rs_bool PV_DECL pl_spdtab_set_gain (int16 hcam, int16 spdtab_gain);
    rs_bool PV_DECL pl_spdtab_get_max_gain (int16 hcam,
                                            int16_ptr spdtab_max_gain);
    /* Use PARAM_GAIN_INDEX                                           */
    rs_bool PV_DECL pl_spdtab_get_num (int16 hcam, int16_ptr spdtab_num);
    rs_bool PV_DECL pl_spdtab_set_num (int16 hcam, int16 spdtab_num);
    /* Use PARAM_SPDTAB_INDEX                                         */
    rs_bool PV_DECL pl_spdtab_get_entries (int16 hcam, int16_ptr spdtab_entries);
    /* Use PARAM_SPDTAB_INDEX (ATTR_MAX)                              */
    rs_bool PV_DECL pl_spdtab_get_port (int16 hcam, int16_ptr spdtab_port);
    rs_bool PV_DECL pl_spdtab_get_port_total (int16 hcam, int16_ptr total_ports);
    /* Use PARAM_READOUT_PORT                                         */
    rs_bool PV_DECL pl_spdtab_get_time (int16 hcam, uns16_ptr spdtab_time);
    /* Use PARAM_PIX_TIME                                             */
    rs_bool PV_DECL pl_shtr_get_close_dly (int16 hcam, uns16_ptr shtr_close_dly);
    rs_bool PV_DECL pl_shtr_set_close_dly (int16 hcam, uns16 shtr_close_dly);
    /* Use PARAM_SHTR_CLOSE_DELAY                                     */
    rs_bool PV_DECL pl_shtr_get_open_dly (int16 hcam, uns16_ptr shtr_open_dly);
    rs_bool PV_DECL pl_shtr_set_open_dly (int16 hcam, uns16 shtr_open_dly);
    /* Use PARAM_SHTR_OPEN_DELAY                                      */
    rs_bool PV_DECL pl_shtr_get_open_mode (int16 hcam, int16_ptr shtr_open_mode);
    rs_bool PV_DECL pl_shtr_set_open_mode (int16 hcam, int16 shtr_open_mode);
    /* Use PARAM_SHTR_OPEN_MODE                                       */
    rs_bool PV_DECL pl_shtr_get_status (int16 hcam, int16_ptr shtr_status);
    /* Use PARAM_SHTR_STATUS                                          */
    rs_bool PV_DECL pl_exp_get_time_seq (int16 hcam, uns16_ptr exp_time);
    rs_bool PV_DECL pl_exp_set_time_seq (int16 hcam, uns16 exp_time);
    /* Use PARAM_EXP_TIME                                             */
    rs_bool PV_DECL pl_exp_check_progress (int16 hcam, int16_ptr status,
                                           uns32_ptr bytes_arrived);
    /* Use pl_exp_check_status or pl_exp_check_cont_status */

    /*****************************************************************************/
    /* Other functions that should not be used.  They are included for           */
    /* compatibility reasons.                                                    */
    /*****************************************************************************/
    rs_bool PV_DECL pl_exp_set_cont_mode (int16 hcam, int16 mode);
    rs_bool PV_DECL pl_subsys_do_diag (int16 hcam, uns8 subsys_id,
                                       uns16_ptr err_code);
    rs_bool PV_DECL pl_subsys_get_id (int16 hcam, uns8 subsys_id,
                                      uns16_ptr part_num, uns8_ptr revision);
    rs_bool PV_DECL pl_subsys_get_name (int16 hcam, uns8 subsys_id,
                                        char_ptr subsys_name);
    rs_bool PV_DECL pl_exp_get_driver_buffer (int16 hcam,
                                              void_ptr_ptr pixel_stream,
                                              uns32_ptr byte_cnt);
    rs_bool PV_DECL pl_buf_init (void);
    rs_bool PV_DECL pl_buf_uninit (void);

    rs_bool PV_DECL pl_buf_alloc (int16_ptr hbuf, int16 exp_total,
                                  int16 bit_depth, int16 rgn_total,
                                  rgn_const_ptr rgn_array);
    rs_bool PV_DECL pl_buf_get_exp_date (int16 hbuf, int16 exp_num,
                                         int16_ptr year, uns8_ptr month,
                                         uns8_ptr day, uns8_ptr hour,
                                         uns8_ptr min, uns8_ptr sec,
                                         uns16_ptr msec);
    rs_bool PV_DECL pl_buf_set_exp_date (int16 hbuf, int16 exp_num, int16 year,
                                         uns8 month, uns8 day, uns8 hour,
                                         uns8 min, uns8 sec, uns16 msec);
    rs_bool PV_DECL pl_buf_get_exp_time (int16 hbuf, int16 exp_num,
                                         uns32_ptr exp_msec);
    rs_bool PV_DECL pl_buf_get_exp_total (int16 hbuf, int16_ptr total_exps);
    rs_bool PV_DECL pl_buf_get_img_bin (int16 himg, int16_ptr ibin,
                                        int16_ptr jbin);
    rs_bool PV_DECL pl_buf_get_img_handle (int16 hbuf, int16 exp_num,
                                           int16 img_num, int16_ptr himg);
    rs_bool PV_DECL pl_buf_get_img_ofs (int16 himg, int16_ptr s_ofs,
                                        int16_ptr p_ofs);
    rs_bool PV_DECL pl_buf_get_img_ptr (int16 himg, void_ptr_ptr img_addr);
    rs_bool PV_DECL pl_buf_get_img_size (int16 himg, int16_ptr x_size,
                                         int16_ptr y_size);
    rs_bool PV_DECL pl_buf_get_img_total (int16 hbuf, int16_ptr totl_imgs);
    rs_bool PV_DECL pl_buf_get_size (int16 hbuf, int32_ptr buf_size);
    rs_bool PV_DECL pl_buf_free (int16 hbuf);
    rs_bool PV_DECL pl_buf_get_bits (int16 hbuf, int16_ptr bit_depth);
    rs_bool PV_DECL pl_exp_unravel (int16 hcam, uns16 exposure,
                                    void_ptr pixel_stream, uns16 rgn_total,
                                    rgn_const_ptr rgn_array,
                                    uns16_ptr * array_list);
    rs_bool PV_DECL pl_exp_wait_start_xfer (int16 hcam, uns32 tlimit);
    rs_bool PV_DECL pl_exp_wait_end_xfer (int16 hcam, uns32 tlimit);

    rs_bool PV_DECL pv_cam_get_ccs_mem (int16 hcam, uns16_ptr size);
    rs_bool PV_DECL pv_cam_send_debug (int16 hcam, char_ptr debug_str,
                                       uns16 reply_len, char_ptr reply_str);
    rs_bool PV_DECL pv_cam_write_read (int16 hcam, uns8 c_class, uns16 write_bytes,
                                       uns8_ptr write_array, uns8_ptr read_array);
    rs_bool PV_DECL pv_dd_active (int16 hcam, void_ptr pixel_stream);
    rs_bool PV_DECL pv_exp_get_bytes (int16 hcam, uns32_ptr exp_bytes);
    rs_bool PV_DECL pv_exp_get_script (int16 hcam, rs_bool_ptr script_valid);
    rs_bool PV_DECL pv_exp_get_status (int16 hcam, int16_ptr status,
                                       uns32_ptr byte_cnt, uns32_ptr frame_cnt);
    rs_bool PV_DECL pv_exp_set_bytes (int16 hcam, uns32 frame_count,
                                      uns32 seq_bytes, void_ptr pixel_stream);
    rs_bool PV_DECL pv_exp_set_script (int16 hcam, rs_bool script_valid);
    rs_bool PV_DECL pv_set_error_code (int16 omode,int16 err_code);
    rs_bool PV_DECL pv_cam_do_reads (int16 hcam);
    rs_bool  PV_DECL pv_free (void_ptr block, int16 heap);
    void_ptr PV_DECL pv_malloc (uns32 size, int16 heap);
    void_ptr PV_DECL pv_realloc(void_ptr block, uns32 size, int16 heap);
    rs_bool PV_DECL pv_script_set_hook (pm_script_hook PV_PTR_DECL pfn);
    rs_bool PV_DECL pv_ccd_get_accum_capable (int16 hcam,
                                              rs_bool_ptr accum_capable);
    rs_bool PV_DECL pv_exp_get_frames (int16 hcam, uns32_ptr exp_frames);
    rs_bool PV_DECL pv_exp_set_frames (int16 hcam, uns32 exp_frames);
    rs_bool PV_DECL pv_exp_set_no_readout_timeout (int16 hcam);
    rs_bool PV_DECL pv_exp_reset_no_readout_timeout (int16 hcam);
    rs_bool PV_DECL pm_cam_write_read (int16 hcam, uns8 c_class, uns16 write_bytes,
                                       uns8_ptr write_array, uns8_ptr read_array);
    rs_bool PV_DECL pl_ddi_get_ver (uns16_ptr ddi_version);
    rs_bool PV_DECL pl_cam_get_diags (int16 hcam);

    /*****************************************************************************/
    /* End of function definitions.                                              */
    /*****************************************************************************/


#ifdef PV_C_PLUS_PLUS
};
#endif

#endif /* PV_EMBEDDED */

#endif /* _PVCAM_H */
