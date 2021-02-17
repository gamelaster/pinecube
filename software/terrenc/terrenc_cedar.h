#ifndef TERRENC_CEDAR_H
#define TERRENC_CEDAR_H

#define CONFIG_ARCH_SUN8IW8P1

enum IOCTL_CMD {
	IOCTL_UNKOWN = 0x100,
	IOCTL_GET_ENV_INFO,
	IOCTL_WAIT_VE_DE,
	IOCTL_WAIT_VE_EN,
	IOCTL_RESET_VE,
	IOCTL_ENABLE_VE,
	IOCTL_DISABLE_VE,
	IOCTL_SET_VE_FREQ,
	
	IOCTL_CONFIG_AVS2 = 0x200,
	IOCTL_GETVALUE_AVS2 ,
	IOCTL_PAUSE_AVS2 ,
	IOCTL_START_AVS2 ,
	IOCTL_RESET_AVS2 ,
	IOCTL_ADJUST_AVS2,
	IOCTL_ENGINE_REQ,
	IOCTL_ENGINE_REL,
	IOCTL_ENGINE_CHECK_DELAY,
	IOCTL_GET_IC_VER,
	IOCTL_ADJUST_AVS2_ABS,
	IOCTL_FLUSH_CACHE,
	IOCTL_SET_REFCOUNT,

	IOCTL_READ_REG = 0x300,
	IOCTL_WRITE_REG,
	
	IOCTL_SET_VOL = 0x400,

#if defined CONFIG_ARCH_SUN8IW8P1
	IOCTL_WAIT_JPEG_DEC = 0x500,
#endif
};

typedef enum VENC_H264PROFILETYPE
{
    VENC_H264ProfileBaseline  = 66,         /**< Baseline profile */
    VENC_H264ProfileMain      = 77,         /**< Main profile */
    VENC_H264ProfileHigh      = 100,   		/**< High profile */
}VENC_H264PROFILETYPE;


/** 
 * H264 level types
 */
typedef enum VENC_H264LEVELTYPE
{
    VENC_H264Level1   = 10,     /**< Level 1 */
    VENC_H264Level11  = 11,     /**< Level 1.1 */
    VENC_H264Level12  = 12,     /**< Level 1.2 */
    VENC_H264Level13  = 13,     /**< Level 1.3 */
    VENC_H264Level2   = 20,     /**< Level 2 */
    VENC_H264Level21  = 21,     /**< Level 2.1 */
    VENC_H264Level22  = 22,     /**< Level 2.2 */
    VENC_H264Level3   = 30,     /**< Level 3 */
    VENC_H264Level31  = 31,     /**< Level 3.1 */
    VENC_H264Level32  = 32,     /**< Level 3.2 */
    VENC_H264Level4   = 40,     /**< Level 4 */
    VENC_H264Level41  = 41,     /**< Level 4.1 */
    VENC_H264Level42  = 42,     /**< Level 4.2 */
    VENC_H264Level5   = 50,     /**< Level 5 */
    VENC_H264Level51  = 51,     /**< Level 5.1 */
}VENC_H264LEVELTYPE;

typedef struct VencH264ProfileLevel
{
	VENC_H264PROFILETYPE	nProfile;
	VENC_H264LEVELTYPE		nLevel;
}VencH264ProfileLevel;

typedef struct VencQPRange
{
	int	nMaxqp;
	int	nMinqp;
}VencQPRange;

typedef enum VENC_CODING_MODE
{
  VENC_FRAME_CODING         = 0,
  VENC_FIELD_CODING         = 1,
}VENC_CODING_MODE;

typedef struct VencH264Param
{
	VencH264ProfileLevel	sProfileLevel;
	int                 	bEntropyCodingCABAC; /* 0:CAVLC 1:CABAC*/
	VencQPRange   			sQPRange;
	int                     nFramerate; /* fps*/
	int                     nBitrate;   /* bps*/
	int                     nMaxKeyInterval;
	VENC_CODING_MODE        nCodingMode;
}VencH264Param;


#endif