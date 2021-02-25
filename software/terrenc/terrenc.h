#ifndef TERRENC_H
#define TERRENC_H

#include <stdint.h>
#include "terrenc_cedar.h"

enum VENC_INDEXTYPE
{
	VENC_IndexParamH264Param = 0x0
};

#define VENC_CODEC_H264 0

int* VideoEncCreate(int type);
int VideoEncSetParameter(void* encoder, enum VENC_INDEXTYPE parameterType, void* parameterData);
int VideoEncInit(void *encoder, VencBaseConfig *cfg);

// Helpers

static inline void write8(uint32_t addr, uint8_t val)
{
	*((volatile uint8_t*)addr) = val;
}

static inline void write32(uint32_t addr, uint32_t val)
{
	*((volatile uint32_t*)addr) = val;
}

static inline uint32_t read32(uint32_t addr)
{
	return *((volatile uint32_t*)addr);
}

// Registers
#define REG_VE_CTRL 0x00
#define REG_VE_VERSION 0xF0

#define REG_VE_AVC 0xb00
#define REG_VE_AVC_TRIG 0x18
#define REG_VE_AVC_STATUS 0x1c
#define REG_VE_AVC_BITS_DATA 0x20
#define REG_VE_AVC_VLE_ADDR 0x80
#define REG_VE_AVC_VLE_ADDR_END 0x84
#define REG_VE_AVC_VLE_BIT_OFFSET 0x88
#define REG_VE_AVC_VLE_BIT_MAXIMUM 0x8C
#define REG_VE_AVC_VLE_LENGTH 0x90

#endif