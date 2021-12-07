#ifndef RECEDAR_HOOKS_H_
#define RECEDAR_HOOKS_H_

#define _GNU_SOURCE
#include <dlfcn.h>

#include "recedar.h"
#include "venc_device.h"
#include "h264_device.h"
#include "FrameBufferManager.h"
#include "isp.h"
#include <stdlib.h>

#ifdef RECEDAR_HOOKS_IMPLEMENT
#define RECEDAR_HOOKS_EXTERN  
#else
#define RECEDAR_HOOKS_EXTERN extern
#endif

RECEDAR_HOOKS_EXTERN VideoEncoder*   		(*_VideoEncCreate)(VENC_CODEC_TYPE eCodecType);
RECEDAR_HOOKS_EXTERN unsigned int    		(*_EncAdapterGetICVersion)(void);
RECEDAR_HOOKS_EXTERN int             		(*_VideoEncSetParameter)(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData);
RECEDAR_HOOKS_EXTERN int             		(*_VideoEncInit)(VideoEncoder* pEncoder, VencBaseConfig* pConfig);
RECEDAR_HOOKS_EXTERN int             		(*_AllocInputBuffer)(VideoEncoder* pEncoder, VencAllocateBufferParam *pBufferParam);
RECEDAR_HOOKS_EXTERN int             		(*_GetOneAllocInputBuffer)(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer);
RECEDAR_HOOKS_EXTERN int             		(*_FlushCacheAllocInputBuffer)(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer);
RECEDAR_HOOKS_EXTERN int             		(*_AddOneInputBuffer)(VideoEncoder* pEncoder, VencInputBuffer* pInputbuffer);
RECEDAR_HOOKS_EXTERN int             		(*_VideoEncodeOneFrame)(VideoEncoder* pEncoder);
RECEDAR_HOOKS_EXTERN int             		(*_AlreadyUsedInputBuffer)(VideoEncoder* pEncoder, VencInputBuffer* pBuffer);
RECEDAR_HOOKS_EXTERN int             		(*_ReturnOneAllocInputBuffer)(VideoEncoder* pEncoder,  VencInputBuffer *pInputbuffer);
RECEDAR_HOOKS_EXTERN int             		(*_VideoEncGetParameter)(VideoEncoder* pEncoder, VENC_INDEXTYPE indexType, void* paramData);
RECEDAR_HOOKS_EXTERN int             		(*_FreeOneBitStreamFrame)(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer);
RECEDAR_HOOKS_EXTERN int             		(*_GetOneBitstreamFrame)(VideoEncoder* pEncoder, VencOutputBuffer* pBuffer);
RECEDAR_HOOKS_EXTERN unsigned int 	 		(*_EncAdapterGetICVersion)(void);
RECEDAR_HOOKS_EXTERN int   			 		(*_EncAdapterInitialize)(void);
RECEDAR_HOOKS_EXTERN VENC_DEVICE *	 		(*_VencoderDeviceCreate)(VENC_CODEC_TYPE type);
RECEDAR_HOOKS_EXTERN void*	 		 		(*_H264EncOpen)();
RECEDAR_HOOKS_EXTERN int			 		(*_H264Init)(void *handle,VencBaseConfig* pBaseConfig);
RECEDAR_HOOKS_EXTERN int	 		 		(*_H264UnInit)(void *handle);
RECEDAR_HOOKS_EXTERN void			 		(*_H264EncClose)(void *handle);
RECEDAR_HOOKS_EXTERN int			 		(*_H264EncEncode)(void *handle, VencInputBuffer* pInBuffer);
RECEDAR_HOOKS_EXTERN int			 		(*_H264GetParameter)(void *handle, int indexType, void* param);
RECEDAR_HOOKS_EXTERN int			 		(*_H264SetParameter)(void *handle, int indexType, void* param);
RECEDAR_HOOKS_EXTERN int			 		(*_H264ValidBitStreamFrameNum)(void *handle);
RECEDAR_HOOKS_EXTERN int			 		(*_H264GetOneBitstream)(void *handle, VencOutputBuffer *pOutBuffer);
RECEDAR_HOOKS_EXTERN int			 		(*_H264FreeOneBitstream)(void *handle, VencOutputBuffer *pOutBuffer);
RECEDAR_HOOKS_EXTERN int			 		(*_H264FreeOneBitstream)(void *handle, VencOutputBuffer *pOutBuffer);
RECEDAR_HOOKS_EXTERN void			 		(*_h264_init_default_param)(h264_context* h264Context);
RECEDAR_HOOKS_EXTERN int 			 		(*_h264_get_ve_capability)(h264_context* h264Context);
RECEDAR_HOOKS_EXTERN FrameBufferManager* 	(*_FrameBufferManagerCreate)(int num);
RECEDAR_HOOKS_EXTERN int   					(*_EncAdapterLockVideoEngine)(void);
RECEDAR_HOOKS_EXTERN void  					(*_EncAdapterUnLockVideoEngine)(void);
RECEDAR_HOOKS_EXTERN isp_context*			(*_IspCreate)();
RECEDAR_HOOKS_EXTERN isp_context*			(*_SetIspBaseAddress)(isp_context *isp, int baseAddress);
RECEDAR_HOOKS_EXTERN void* 					(*_EncAdapterVeGetBaseAddress)(void);
RECEDAR_HOOKS_EXTERN int					(*_h264_check_capability)(h264_context *h264Context);
RECEDAR_HOOKS_EXTERN int					(*_H264InitMemory)(h264_context *h264Context);
RECEDAR_HOOKS_EXTERN int					(*_h264_init_regInfo)(h264_context *h264Context);
RECEDAR_HOOKS_EXTERN void					(*_h264_init_rc_quene)(h264_context* h264Context);
RECEDAR_HOOKS_EXTERN void					(*_h264_init_Poc)(h264_context* h264Context);
RECEDAR_HOOKS_EXTERN int					(*_h264_init_sps_pps)(h264_context *h264Context);

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
	HOOK_FUNCTION(H264EncOpen);
	HOOK_FUNCTION(H264Init);
	HOOK_FUNCTION(H264UnInit);
	HOOK_FUNCTION(H264EncClose);
	HOOK_FUNCTION(H264EncEncode);
	HOOK_FUNCTION(H264GetParameter);
	HOOK_FUNCTION(H264SetParameter);
	HOOK_FUNCTION(H264ValidBitStreamFrameNum);
	HOOK_FUNCTION(H264GetOneBitstream);
	HOOK_FUNCTION(H264FreeOneBitstream);
	HOOK_FUNCTION(h264_init_default_param);
	HOOK_FUNCTION(h264_get_ve_capability);
	HOOK_FUNCTION(FrameBufferManagerCreate);
	HOOK_FUNCTION(EncAdapterLockVideoEngine);
	HOOK_FUNCTION(EncAdapterUnLockVideoEngine);
	HOOK_FUNCTION(IspCreate);
	HOOK_FUNCTION(SetIspBaseAddress);
	HOOK_FUNCTION(EncAdapterVeGetBaseAddress);
	HOOK_FUNCTION(h264_check_capability);
	HOOK_FUNCTION(H264InitMemory);
	HOOK_FUNCTION(h264_init_regInfo);
	HOOK_FUNCTION(h264_init_rc_quene);
	HOOK_FUNCTION(h264_init_Poc);
	HOOK_FUNCTION(h264_init_sps_pps);
#undef HOOK_FUNCTION
}
#endif

#endif