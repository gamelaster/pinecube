#ifndef H264_DEVICE_H_
#define H264_DEVICE_H_

// NOTE: There is something with aligning on this struct, and sometimes because of it things doesn't work correctly.
// Pack configuration should help, but I need to be careful when working with this struct :-)
// Also note that IDA loves to forgot some gaps when exporting the struct :D

#pragma pack(push, 1)
typedef struct
{
	int field_0;
	int pBSMamager;
	int ic_version;
	char gapC[3788];
	int field_ED8;
	char gapEDC[115];
	char field_1;
} h264_context;
#pragma pack(pop)
_Static_assert(sizeof(h264_context) == 0xF50, "h264_context doesn't have correct size");

extern VENC_DEVICE h264_device;





#endif