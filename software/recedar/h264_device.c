#include "venc_device.h"
#include "recedar_hooks.h"
#include "EncAdapter.h"

#include <stdio.h>

// -RC status=implemented
void h264_init_default_param(h264_context* h264Context)
{
	h264Context->field_1E0 = 18;
	h264Context->bitrate = 0x200000;
	h264Context->framerate = 30000;
	h264Context->maxKeyInterval = 25;
	h264Context->maxQp = 45;
	h264Context->field_711 = 1;
	h264Context->field_1C8 = 1;
	h264Context->minQp = 10;
	h264Context->field_1D4 = 2;
	h264Context->field_1D8 = 30;
	h264Context->field_1DC = 30;
	h264Context->profileLevel.nProfile = VENC_H264ProfileMain;
	h264Context->profileLevel.nLevel = VENC_H264Level41;
	h264Context->field_6F4 = 4;
	h264Context->field_70F = 1;
	h264Context->field_710 = 1;
	h264Context->entropyCoding = 1;
	h264Context->field_6FE = 1;
	h264Context->field_70C = 4;
	h264Context->field_1A4 = 0;
	h264Context->field_1B4 = 0;
	h264Context->field_BC = 0;
	h264Context->forceKeyFrame = 0;
	h264Context->field_6F5 = 8;
	h264Context->field_6FC = 0;
	h264Context->field_6FF = 0;
	h264Context->field_700 = 0;
	h264Context->field_704 = 0;
	h264Context->field_708 = 0;
	h264Context->field_70D = 0;
	h264Context->field_5C = 4096;
	h264Context->field_60 = 4096;
	h264Context->field_D7C = 5;
	h264Context->rotation = 0;
	h264Context->superFrameConfig.eSuperFrameMode = VENC_SUPERFRAME_NONE;
}

// -RC status=hooked
int h264_get_ve_capability(h264_context* h264Context)
{
	return _h264_get_ve_capability(h264Context);
}

// -RC status=implemented
void* H264EncOpen()
{
	h264_context* h264Context = malloc(sizeof(h264_context));
	if (!h264Context) {
		fprintf(stderr, "[recedar] Create h264_context error.\n");
		return NULL;
	}
	memset(h264Context, 0, sizeof(h264_context));
	_h264_init_default_param(h264Context);
	h264Context->ic_version = EncAdapterGetICVersion();
	if (_h264_get_ve_capability(h264Context) < 0) {
		fprintf(stderr, "[recedar] the driver do not support ic\n");
		return NULL;
	}
	h264Context->vbvSize = 0x800000;
	return h264Context;
}

// -RC status=hooked
int H264Init(void *handle,VencBaseConfig* pBaseConfig)
{
	return _H264Init(handle, pBaseConfig);
}

// -RC status=hooked
int H264UnInit(void *handle)
{
	return _H264UnInit(handle);
}

// -RC status=hooked
void H264EncClose(void *handle)
{
	return _H264EncClose(handle);
}

// -RC status=hooked
int H264EncEncode(void *handle, VencInputBuffer* pInBuffer)
{
	return _H264EncEncode(handle, pInBuffer);
}

// -RC status=hooked
int H264GetParameter(void *handle, int indexType, void* param)
{
	return _H264GetParameter(handle, indexType, param);
}

// -RC status=hooked
int H264SetParameter(void *handle, int indexType, void* param)
{
	return _H264SetParameter(handle, indexType, param);
}

// -RC status=hooked
int H264ValidBitStreamFrameNum(void *handle)
{
	return _H264ValidBitStreamFrameNum(handle);
}

// -RC status=hooked
int H264GetOneBitstream(void *handle, VencOutputBuffer *pOutBuffer)
{
	return _H264GetOneBitstream(handle, pOutBuffer);
}

// -RC status=hooked
int H264FreeOneBitstream(void *handle, VencOutputBuffer *pOutBuffer)
{
	return _H264FreeOneBitstream(handle, pOutBuffer);
}


VENC_DEVICE h264_device = {
	"video encoder h264",
	H264EncOpen,
	H264Init,
	H264UnInit,
	H264EncClose,
	H264EncEncode,
	H264GetParameter,
	H264SetParameter,
	H264ValidBitStreamFrameNum,
	H264GetOneBitstream,
	H264FreeOneBitstream,
};