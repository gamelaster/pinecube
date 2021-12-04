#ifndef RECEDAR_H_
#define RECEDAR_H_

#include <stddef.h>

typedef enum VENC_CODEC_TYPE
{
	VENC_CODEC_H264,
	VENC_CODEC_JPEG,
	VENC_CODEC_VP8,
} VENC_CODEC_TYPE;

typedef enum VENC_INDEXTYPE
{
	VENC_IndexParamBitrate				= 0x0,		/**< reference type: int */
	VENC_IndexParamFramerate,                		/**< reference type: int */
	VENC_IndexParamMaxKeyInterval,           		/**< reference type: int */
	VENC_IndexParamIfilter,                  		/**< reference type: int */            
	VENC_IndexParamRotation,                 		/**< reference type: int */
	VENC_IndexParamSliceHeight,              		/**< reference type: int */
	VENC_IndexParamForceKeyFrame,            		/**< reference type: int (write only)*/
	VENC_IndexParamMotionDetectEnable,             	/**< reference type: MotionParam(write only) */
	VENC_IndexParamMotionDetectStatus,             	/**< reference type: int(read only) */
	VENC_IndexParamRgb2Yuv,             			/**< reference type: VENC_COLOR_SPACE */
	VENC_IndexParamYuv2Yuv,             			/**< reference type: VENC_YUV2YUV */
	VENC_IndexParamROIConfig,						/**< reference type: VencROIConfig */
	VENC_IndexParamStride,              		    /**< reference type: int */
	VENC_IndexParamColorFormat,                     /**< reference type: VENC_PIXEL_FMT */
	VENC_IndexParamSize,                     		/**< reference type: VencSize(read only) */
	VENC_IndexParamSetVbvSize,                     	/**< reference type: setVbvSize(write only) */
	VENC_IndexParamVbvInfo,							/**< reference type: getVbvInfo(read only) */
	VENC_IndexParamSuperFrameConfig,                /**< reference type: VencSuperFrameConfig */ 
	VENC_IndexParamSetPSkip,						/**< reference type: unsigned int */ 

	/* check capabiliy */
	VENC_IndexParamMAXSupportSize,					/**< reference type: VencSize(read only) */
	VENC_IndexParamCheckColorFormat,				/**< reference type: VencCheckFormat(read only) */

	/* H264 param */
	VENC_IndexParamH264Param,						/**< reference type: VencH264Param */
	VENC_IndexParamH264SPSPPS,                    	/**< reference type: VencHeaderData (read only)*/
	VENC_IndexParamH264QPRange			= 0x100,	/**< reference type: VencQPRange */
	VENC_IndexParamH264ProfileLevel,              	/**< reference type: VencProfileLevel */
	VENC_IndexParamH264EntropyCodingCABAC,			/**< reference type: int(0:CAVLC 1:CABAC) */
	VENC_IndexParamH264CyclicIntraRefresh,			/**< reference type: VencCyclicIntraRefresh */
	VENC_IndexParamH264FixQP,						/**< reference type: VencH264FixQP */
	VENC_IndexParamH264SVCSkip,				        /**< reference type: VencH264SVCSkip */
	VENC_IndexParamH264AspectRatio,                 /**< reference type: VencH264AspectRatio */
	VENC_IndexParamH264FastEnc,                     /**< reference type: int */
	VENC_IndexParamH264VideoSignal,                /**< reference type: VencH264VideoSignal */

	/* jpeg param */
	VENC_IndexParamJpegQuality			= 0x200,	/**< reference type: int (1~100) */
	VENC_IndexParamJpegExifInfo,                  	/**< reference type: EXIFInfo */
	VENC_IndexParamJpegEncMode,                  	/**< reference type: 0:jpeg; 1:motion_jepg */

	/* VP8 param */
	VENC_IndexParamVP8Param,

    /* max one frame length */
    VENC_IndexParamSetFrameLenThreshold,

	/* decrease the a20 dram bands */
	VENC_IndexParamSetA20LowBands,

}VENC_INDEXTYPE;

typedef enum VENC_PIXEL_FMT
{
	VENC_PIXEL_YUV420SP,
    VENC_PIXEL_YVU420SP,
    VENC_PIXEL_YUV420P,
    VENC_PIXEL_YVU420P,
	VENC_PIXEL_YUV422SP,
    VENC_PIXEL_YVU422SP,
    VENC_PIXEL_YUV422P,
    VENC_PIXEL_YVU422P,
	VENC_PIXEL_YUYV422,
	VENC_PIXEL_UYVY422,
	VENC_PIXEL_YVYU422,
	VENC_PIXEL_VYUY422,
	VENC_PIXEL_ARGB,
    VENC_PIXEL_RGBA,
    VENC_PIXEL_ABGR,
    VENC_PIXEL_BGRA,
    VENC_PIXEL_TILE_32X32,
    VENC_PIXEL_TILE_128X32,
}VENC_PIXEL_FMT;


typedef struct VencBaseConfig
{
	unsigned int		nInputWidth;
	unsigned int		nInputHeight;
	unsigned int		nDstWidth;
	unsigned int		nDstHeight;
	unsigned int        nStride;
	VENC_PIXEL_FMT  	eInputFormat;
}VencBaseConfig;

typedef struct VencAllocateBufferParam
{
    unsigned int   nBufferNum;
	unsigned int   nSizeY;
	unsigned int   nSizeC;
}VencAllocateBufferParam;


typedef struct VencRect
{
	int nLeft;
	int nTop;
	int nWidth;
	int nHeight;
}VencRect;

/* support 4 ROI region */
typedef struct VencROIConfig
{
	int                     bEnable;
	int                		index; /* (0~3) */
	int                     nQPoffset;
	VencRect                sRect;
}VencROIConfig;

typedef struct VencInputBuffer
{
	unsigned long  nID;
	long long  	   nPts;
	unsigned int   nFlag;
	unsigned char* pAddrPhyY;
	unsigned char* pAddrPhyC;
	unsigned char* pAddrVirY;
	unsigned char* pAddrVirC;
	int 		   bEnableCorp;
	VencRect       sCropInfo;

	int			   ispPicVar;
	VencROIConfig  roi_param[4];
}VencInputBuffer;

typedef struct FrameInfo
{
	int         	CurrQp;
	int             avQp;
	int             nGopIndex;
	int             nFrameIndex;
	int             nTotalIndex;
}FrameInfo;

typedef struct VencHeaderData
{
	unsigned char*  pBuffer;
	unsigned int	nLength;
}VencHeaderData;

typedef struct VencOutputBuffer
{
	int			   nID;
	long long  	   nPts;
	unsigned int   nFlag;
	unsigned int   nSize0;
	unsigned int   nSize1;
	unsigned char* pData0;
	unsigned char* pData1;

	FrameInfo	   frame_info;
}VencOutputBuffer;

typedef void* VideoEncoder;
VideoEncoder* VideoEncCreate(VENC_CODEC_TYPE eCodecType);
int VideoEncSetParameter(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData);
int VideoEncInit(VideoEncoder* pEncoder, VencBaseConfig* pConfig);
int AllocInputBuffer(VideoEncoder* pEncoder, VencAllocateBufferParam *pBufferParam);
int GetOneAllocInputBuffer(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer);
int FlushCacheAllocInputBuffer(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer);
int AddOneInputBuffer(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer);
int VideoEncodeOneFrame(VideoEncoder* pEncoder);
int AlreadyUsedInputBuffer(VideoEncoder* pEncoder, VencInputBuffer* pBuffer);
int ReturnOneAllocInputBuffer(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer);
int VideoEncGetParameter(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData);
int FreeOneBitStreamFrame(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer);
int GetOneBitstreamFrame(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer);


#endif