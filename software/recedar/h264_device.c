#include "venc_device.h"
#include "recedar_hooks.h"
#include "EncAdapter.h"

#include <stdio.h>

// -RC status=hooked
void h264_init_default_param(h264_context* h264Context)
{
	_h264_init_default_param(h264Context);	
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
	h264Context->field_ED8 = 0x800000;
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