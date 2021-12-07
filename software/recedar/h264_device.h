#ifndef H264_DEVICE_H_
#define H264_DEVICE_H_

#include "isp.h"

// NOTE: There is something with aligning on this struct, and sometimes because of it things doesn't work correctly.
// Pack configuration should help, but I need to be careful when working with this struct :-)
// Also note that IDA loves to forgot some gaps when exporting the struct :D

//#pragma pack(push, 1)
typedef struct 
{
	isp_context *isp;
	int pBSMamager; // field_4 - None
	int ic_version; // field_8 - None
	int veBaseAddress; // field_C - None
	int avcBaseAddress; // field_10 - None
	char gap14[24];
	VencBaseConfig baseConfig; // field_2C - Separate copy of baseConfig VencContext
	int dstWidthNormalized; // field_44 - nDstWidth + 15 & 0xFFFFFFF0
	int dstHeightNormalized; // field_48 - nDstHeight + 15 & 0xFFFFFFF0
	int inputWidthDiv16; // field_4C - (nInputWidth + 15) >> 4
	int inputHeightDiv16; // field_50 - (nInputHeight + 15) >> 4
	int dstWidthDiv16; // field_54 - (nDstWidth + 15) >> 4
	int dstHeightDiv16; // field_58 - (nDstHeight + 15) >> 4
	int field_5C; // field_5C - None
	int field_60; // field_60 - None
	int lineWidth; // field_64 - Line width value of the picture in memory
	int rotation; // field_68 - None
	char gap6C[80];
	int field_BC; // field_BC - None
	int isInputHeightDivid16; // field_C0 - None
	char gapC4[88];
	VencROIConfig roiConfig[4]; // field_11C - None
	char gap18C[24];
	int field_1A4; // field_1A4 - None
	char gap1A8[4];
	int forceKeyFrame; // field_1AC - None
	int maxKeyInterval; // field_1B0 - None
	int field_1B4; // field_1B4 - None
	int bitrate; // field_1B8 - in BPS
	int framerate; // field_1BC - FPS * 1000
	char gap1C0[4];
	VENC_CODING_MODE codingMode; // field_1C4 - None
	int field_1C8; // field_1C8 - None
	int maxQp; // field_1CC - None
	int minQp; // field_1D0 - None
	int field_1D4; // field_1D4 - None
	int field_1D8; // field_1D8 - None
	int field_1DC; // field_1DC - None
	int field_1E0; // field_1E0 - None
	char gap1E4[1288];
	VencH264ProfileLevel profileLevel; // field_6EC - None
	char field_6F4; // field_6F4 - None
	char field_6F5; // field_6F5 - None
	char gap6F6[6];
	char field_6FC; // field_6FC - None
	char entropyCoding; // field_6FD - 0: CAVLC, 1: CABAC
	char field_6FE; // field_6FE - None
	char field_6FF; // field_6FF - None
	char field_700; // field_700 - None
	int field_704; // field_704 - None
	int field_708; // field_708 - None
	char field_70C; // field_70C - None
	char field_70D; // field_70D - None
	char gap70E;
	char field_70F; // field_70F - None
	char field_710; // field_710 - None
	char field_711; // field_711 - skipFramesCount: SKIP_4 -> 2, SKIP_2 -> 1, NO_SKIP -> 1, SKIP_8 -> 4 , if pskip is positive, then this value * 10 (check H264 Set Param)
	char gap712[1574];
	MotionParam motionParam; // field_D38 - None
	char gapD40[60];
	int field_D7C; // field_D7C - None
	int iFilter; // field_D80 - None
	char gapD84[188];
	int sliceHeight; // field_E40 - Set the height of a slice, one frame of image can support multiple slices, the unit is pixel, 16 alignment;
	VencH264FixQP fixQPconfig; // field_E44 - None
	VencCyclicIntraRefresh cyclicIntraRefresh; // field_E50 - None
	char gapE58[88];
	int field_EB0; // field_EB0 - something about capability to config color space
	char gapEB4[28];
	VENC_COLOR_SPACE rgb2yuv; // field_ED0 - None
	VENC_YUV2YUV yuv2yuvConv; // field_ED4 - None
	unsigned int vbvSize; // field_ED8 - Preset application VBV (coded output) buffer size
	__attribute__((aligned(8))) VencSuperFrameConfig superFrameConfig; // field_EE0 - None
	char gapEEC[16];
	int frameLengthThreshold; // field_EFC - None
	int A20LowBands; // field_F00 - None
	int pskip; // field_F04 - Frame Switch
	char gapF08[20];
	SKIP_FRAME skipFramesCount; // field_F1C - something pskip related
	T_LAYER numberTemporalSVC; // field_F20 - None
	int mVuiExtensionEnabled; // field_F24 - related to video signal and aspect ratio (VUI?)
	char mAspectRatioEnabled; // field_F28 - None
	VencH264AspectRatio aspectRatioConfig; // field_F2A - None
	char gapF30;
	char mVideoSignalEnabled; // field_F31 - related to video signal
	VencH264VideoSignal videoSignalConfig; // field_F34 - look AW doc
	char gapF44[8];
	unsigned int fastEncEnabled; // field_F4C - None
} h264_context;
//#pragma pack(pop)
_Static_assert(sizeof(h264_context) == 0xF50, "h264_context doesn't have correct size");

extern VENC_DEVICE h264_device;





#endif