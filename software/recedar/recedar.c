#define _GNU_SOURCE
#include "recedar.h"
#include <stdio.h>
#include <dlfcn.h>

static VideoEncoder*   (*_VideoEncCreate)(VENC_CODEC_TYPE eCodecType);
static unsigned int    (*_EncAdapterGetICVersion)(void);
static int             (*_VideoEncSetParameter)(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData);
static int             (*_VideoEncInit)(VideoEncoder* pEncoder, VencBaseConfig* pConfig);
static int             (*_AllocInputBuffer)(VideoEncoder* pEncoder, VencAllocateBufferParam *pBufferParam);
static int             (*_GetOneAllocInputBuffer)(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer);
static int             (*_FlushCacheAllocInputBuffer)(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer);
static int             (*_AddOneInputBuffer)(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer);
static int             (*_VideoEncodeOneFrame)(VideoEncoder* pEncoder);
static int             (*_AlreadyUsedInputBuffer)(VideoEncoder* pEncoder, VencInputBuffer* pBuffer);
static int             (*_ReturnOneAllocInputBuffer)(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer);
static int             (*_VideoEncGetParameter)(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData);
static int             (*_FreeOneBitStreamFrame)(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer);
static int             (*_GetOneBitstreamFrame)(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer);

// -RC status=hooked
VideoEncoder* VideoEncCreate(VENC_CODEC_TYPE eCodecType)
{
    return _VideoEncCreate(eCodecType);
}

// -RC status=hooked
unsigned int EncAdapterGetICVersion(void)
{
    return _EncAdapterGetICVersion();
}

// -RC status=hooked
int VideoEncSetParameter(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData)
{
    return _VideoEncSetParameter(pEncoder, indexType, paramData);
}

// -RC status=hooked
int VideoEncInit(VideoEncoder* pEncoder, VencBaseConfig* pConfig)
{
    return _VideoEncInit(pEncoder, pConfig);
}

// -RC status=hooked
int AllocInputBuffer(VideoEncoder* pEncoder, VencAllocateBufferParam *pBufferParam)
{
    return _AllocInputBuffer(pEncoder, pBufferParam);
}

// -RC status=hooked
int GetOneAllocInputBuffer(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer)
{
    return _GetOneAllocInputBuffer(pEncoder, pInputbuffer);
}

// -RC status=hooked
int FlushCacheAllocInputBuffer(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer)
{
    return _FlushCacheAllocInputBuffer(pEncoder, pInputbuffer);
}

// -RC status=hooked
int AddOneInputBuffer(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer)
{
    return _AddOneInputBuffer(pEncoder, pInputbuffer);
}

// -RC status=hooked
int VideoEncodeOneFrame(VideoEncoder* pEncoder)
{
    return _VideoEncodeOneFrame(pEncoder);
}

// -RC status=hooked
int AlreadyUsedInputBuffer(VideoEncoder* pEncoder, VencInputBuffer* pBuffer)
{
    return _AlreadyUsedInputBuffer(pEncoder, pBuffer);
}

// -RC status=hooked
int ReturnOneAllocInputBuffer(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer)
{
    return _ReturnOneAllocInputBuffer(pEncoder, pInputbuffer);
}

// -RC status=hooked
int VideoEncGetParameter(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData)
{
    return _VideoEncGetParameter(pEncoder, indexType, paramData);
}

// -RC status=hooked
int FreeOneBitStreamFrame(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer)
{
    return _FreeOneBitStreamFrame(pEncoder, pBuffer);
}

// -RC status=hooked
int GetOneBitstreamFrame(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer)
{
    return _GetOneBitstreamFrame(pEncoder, pBuffer);
}

void __attribute__ ((constructor)) recedar_init(void)
{
	_VideoEncCreate = dlsym(RTLD_NEXT, "VideoEncCreate");
	_EncAdapterGetICVersion = dlsym(RTLD_NEXT, "EncAdapterGetICVersion");
	_VideoEncSetParameter = dlsym(RTLD_NEXT, "VideoEncSetParameter");
	_VideoEncInit = dlsym(RTLD_NEXT, "VideoEncInit");
	_AllocInputBuffer = dlsym(RTLD_NEXT, "AllocInputBuffer");
	_GetOneAllocInputBuffer = dlsym(RTLD_NEXT, "GetOneAllocInputBuffer");
	_FlushCacheAllocInputBuffer = dlsym(RTLD_NEXT, "FlushCacheAllocInputBuffer");
	_AddOneInputBuffer = dlsym(RTLD_NEXT, "AddOneInputBuffer");
	_VideoEncodeOneFrame = dlsym(RTLD_NEXT, "VideoEncodeOneFrame");
	_AlreadyUsedInputBuffer = dlsym(RTLD_NEXT, "AlreadyUsedInputBuffer");
	_ReturnOneAllocInputBuffer = dlsym(RTLD_NEXT, "ReturnOneAllocInputBuffer");
	_VideoEncGetParameter = dlsym(RTLD_NEXT, "VideoEncGetParameter");
	_FreeOneBitStreamFrame = dlsym(RTLD_NEXT, "FreeOneBitStreamFrame");
	_GetOneBitstreamFrame = dlsym(RTLD_NEXT, "GetOneBitstreamFrame");
	printf("[recedar] reCedar initialized\n");
}

void __attribute__ ((destructor)) recedar_fini(void)
{

}