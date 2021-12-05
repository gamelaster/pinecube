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
  char gapC[80];
  int field_5C;
  int field_60;
  __attribute__((aligned(8))) int field_68;
  char gap6C[80];
  int field_BC;
  char gapC0[228];
  int field_1A4;
  char gap1A8[4];
  int field_1AC;
  int field_1B0;
  int field_1B4;
  int field_1B8;
  int field_1BC;
  char gap1C0[8];
  int field_1C8;
  int field_1CC;
  int field_1D0;
  int field_1D4;
  int field_1D8;
  int field_1DC;
  int field_1E0;
  char gap1E4[1288];
  int field_6EC;
  int field_6F0;
  char field_6F4;
  char field_6F5;
  char gap6F6[6];
  char field_6FC;
  char field_6FD;
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
  char gap712[1642];
  int field_D7C;
  char gapD80[344];
  int field_ED8;
  __attribute__((aligned(8))) int field_EE0;
  char gapEE4[107];
  char field_1;
} h264_context;
//#pragma pack(pop)
_Static_assert(sizeof(h264_context) == 0xF50, "h264_context doesn't have correct size");

extern VENC_DEVICE h264_device;





#endif