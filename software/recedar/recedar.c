#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>

#include "recedar.h"

#include "FrameBufferManager.h"
#include "venc_device.h"
#include "EncAdapter.h"

#define RECEDAR_HOOKS_IMPLEMENT
#include "recedar_hooks.h"

#define FRAME_BUFFER_NUM 4

typedef struct VencContext
{
   VENC_DEVICE*         pVEncDevice;
   void*                pEncoderHandle;
   FrameBufferManager*  pFBM;
   VencBaseConfig    	baseConfig;
   unsigned int		    nFrameBufferNum;
   VencHeaderData		headerData;
   VencInputBuffer      curEncInputbuffer;
   VENC_CODEC_TYPE 		codecType;
   unsigned int         ICVersion;
   int                  bInit;
   unsigned int 		unknown1; // TODO: ???
}VencContext;

_Static_assert(sizeof(VencContext) == 0xF0, "VencContext doesn't have correct size");

// -RC status=implemented
VideoEncoder* VideoEncCreate(VENC_CODEC_TYPE eCodecType)
{
    VencContext* venc_ctx = NULL;

    if (EncAdapterInitialize() != 0) {
    	fprintf(stderr, "[recedar] can not set up video engine runtime environment.\n");
    	return NULL;
    }

    venc_ctx = (VencContext*) malloc(sizeof(VencContext));
    if (!venc_ctx) {
    	fprintf(stderr, "[recedar] malloc VencContext fail!\n");
    	return NULL;
    }
    memset(venc_ctx, 0, sizeof(VencContext));

    venc_ctx->nFrameBufferNum = FRAME_BUFFER_NUM;
	venc_ctx->codecType = eCodecType;
	venc_ctx->ICVersion = EncAdapterGetICVersion();
	venc_ctx->bInit = 0;
	
	venc_ctx->pVEncDevice = VencoderDeviceCreate(eCodecType);
	if(venc_ctx->pVEncDevice == NULL)
	{
		free(venc_ctx);
		return NULL;
	}

	venc_ctx->pEncoderHandle = venc_ctx->pVEncDevice->open();

	// TODO: Not needed to do it right now :-)
	/* if(!venc_ctx->pEncoderHandle)
	{
		VencoderDeviceDestroy(venc_ctx->pVEncDevice);
		venc_ctx->pVEncDevice = NULL;
		free(venc_ctx);
		return NULL;
	} */

	return (VideoEncoder*)venc_ctx;
    // return _VideoEncCreate(eCodecType);
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
	recedar_hooks_init();
	printf("[recedar] reCedar initialized\n");
}

void __attribute__ ((destructor)) recedar_fini(void)
{

}