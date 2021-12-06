#ifndef H264_DEVICE_H_
#define H264_DEVICE_H_

// NOTE: There is something with aligning on this struct, and sometimes because of it things doesn't work correctly.
// Pack configuration should help, but I need to be careful when working with this struct :-)
// Also note that IDA loves to forgot some gaps when exporting the struct :D

//#pragma pack(push, 1)
typedef struct 
{
  int field_0;
  int pBSMamager;
  int ic_version;
  char gapC[52];
  VENC_PIXEL_FMT colorFormat;
  char gap44[24];
  int field_5C;
  int field_60;
  int lineWidth;
  int rotation;
  char gap6C[80];
  int field_BC;
  char gapC0[92];
  VencROIConfig roiConfig[4];
  char gap18C[24];
  int field_1A4;
  char gap1A8[4];
  int forceKeyFrame;
  int maxKeyInterval;
  int field_1B4;
  int bitrate;
  int framerate;
  char gap1C0[4];
  VENC_CODING_MODE codingMode;
  int field_1C8;
  int maxQp;
  int minQp;
  int field_1D4;
  int field_1D8;
  int field_1DC;
  int field_1E0;
  char gap1E4[1288];
  VencH264ProfileLevel profileLevel;
  char field_6F4;
  char field_6F5;
  char gap6F6[6];
  char field_6FC;
  char entropyCoding;
  char field_6FE;
  char field_6FF;
  char field_700;
  int field_704;
  int field_708;
  char field_70C;
  char field_70D;
  char gap70E;
  char field_70F;
  char field_710;
  char field_711;
  char gap712[1574];
  MotionParam motionParam;
  char gapD40[60];
  int field_D7C;
  int iFilter;
  char gapD84[188];
  int sliceHeight;
  VencH264FixQP fixQPconfig;
  VencCyclicIntraRefresh cyclicIntraRefresh;
  char gapE58[88];
  int field_EB0;
  char gapEB4[28];
  VENC_COLOR_SPACE rgb2yuv;
  VENC_YUV2YUV yuv2yuvConv;
  unsigned int vbvSize;
  __attribute__((aligned(8))) VencSuperFrameConfig superFrameConfig;
  char gapEEC[16];
  int frameLengthThreshold;
  int A20LowBands;
  int pskip;
  char gapF08[20];
  SKIP_FRAME skipFramesCount;
  T_LAYER numberTemporalSVC;
  int mVuiExtensionEnabled;
  char mAspectRatioEnabled;
  VencH264AspectRatio aspectRatioConfig;
  char gapF30;
  char mVideoSignalEnabled;
  VencH264VideoSignal videoSignalConfig;
  char gapF44[8];
  unsigned int fastEncEnabled;
} h264_context;
//#pragma pack(pop)
_Static_assert(sizeof(h264_context) == 0xF50, "h264_context doesn't have correct size");

extern VENC_DEVICE h264_device;





#endif