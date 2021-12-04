#include "venc_device.h"
#include "recedar_hooks.h"

// -RC status=hooked
int H264EncOpen()
{
	return _H264EncOpen();
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