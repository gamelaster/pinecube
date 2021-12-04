#ifndef RECEDAR_HOOKS_H_
#define RECEDAR_HOOKS_H_

#include "recedar.h"
#include "venc_device.h"
#include <stdlib.h>

#ifdef RECEDAR_HOOKS_IMPLEMENT
#define RECEDAR_HOOKS_EXTERN  
#else
#define RECEDAR_HOOKS_EXTERN extern
#endif

RECEDAR_HOOKS_EXTERN VideoEncoder*   (*_VideoEncCreate)(VENC_CODEC_TYPE eCodecType);
RECEDAR_HOOKS_EXTERN unsigned int    (*_EncAdapterGetICVersion)(void);
RECEDAR_HOOKS_EXTERN int             (*_VideoEncSetParameter)(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData);
RECEDAR_HOOKS_EXTERN int             (*_VideoEncInit)(VideoEncoder* pEncoder, VencBaseConfig* pConfig);
RECEDAR_HOOKS_EXTERN int             (*_AllocInputBuffer)(VideoEncoder* pEncoder, VencAllocateBufferParam *pBufferParam);
RECEDAR_HOOKS_EXTERN int             (*_GetOneAllocInputBuffer)(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer);
RECEDAR_HOOKS_EXTERN int             (*_FlushCacheAllocInputBuffer)(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer);
RECEDAR_HOOKS_EXTERN int             (*_AddOneInputBuffer)(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer);
RECEDAR_HOOKS_EXTERN int             (*_VideoEncodeOneFrame)(VideoEncoder* pEncoder);
RECEDAR_HOOKS_EXTERN int             (*_AlreadyUsedInputBuffer)(VideoEncoder* pEncoder, VencInputBuffer* pBuffer);
RECEDAR_HOOKS_EXTERN int             (*_ReturnOneAllocInputBuffer)(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer);
RECEDAR_HOOKS_EXTERN int             (*_VideoEncGetParameter)(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData);
RECEDAR_HOOKS_EXTERN int             (*_FreeOneBitStreamFrame)(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer);
RECEDAR_HOOKS_EXTERN int             (*_GetOneBitstreamFrame)(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer);
RECEDAR_HOOKS_EXTERN unsigned int 	 (*_EncAdapterGetICVersion)(void);
RECEDAR_HOOKS_EXTERN int   			 (*_EncAdapterInitialize)(void);
RECEDAR_HOOKS_EXTERN VENC_DEVICE *	 (*_VencoderDeviceCreate)(VENC_CODEC_TYPE type);


extern void recedar_hooks_init();


#ifdef RECEDAR_HOOKS_IMPLEMENT
#undef RECEDAR_HOOKS_IMPLEMENT
void recedar_hooks_init()
{
#define HOOK_FUNCTION(name) \
	_##name = dlsym(RTLD_NEXT, "" #name); \
	if (_##name == NULL) { \
		fprintf(stderr, "Failed to hook " #name "\n"); \
		exit(1); \
	}

	HOOK_FUNCTION(VideoEncCreate);
	HOOK_FUNCTION(EncAdapterGetICVersion);
	HOOK_FUNCTION(VideoEncSetParameter);
	HOOK_FUNCTION(VideoEncInit);
	HOOK_FUNCTION(AllocInputBuffer);
	HOOK_FUNCTION(GetOneAllocInputBuffer);
	HOOK_FUNCTION(FlushCacheAllocInputBuffer);
	HOOK_FUNCTION(AddOneInputBuffer);
	HOOK_FUNCTION(VideoEncodeOneFrame);
	HOOK_FUNCTION(AlreadyUsedInputBuffer);
	HOOK_FUNCTION(ReturnOneAllocInputBuffer);
	HOOK_FUNCTION(VideoEncGetParameter);
	HOOK_FUNCTION(FreeOneBitStreamFrame);
	HOOK_FUNCTION(GetOneBitstreamFrame);
	HOOK_FUNCTION(EncAdapterGetICVersion);
	HOOK_FUNCTION(EncAdapterInitialize);
	HOOK_FUNCTION(VencoderDeviceCreate);
#undef HOOK_FUNCTION
}
#endif

#endif