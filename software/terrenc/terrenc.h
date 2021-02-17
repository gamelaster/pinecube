#ifndef TERRENC_H
#define TERRENC_H

#include "terrenc_cedar.h"

enum VENC_INDEXTYPE
{
	VENC_IndexParamH264Param = 0x0
};

#define VENC_CODEC_H264 0

int* VideoEncCreate(int type);
int VideoEncSetParameter(void* encoder, enum VENC_INDEXTYPE parameterType, void* parameterData);

#endif