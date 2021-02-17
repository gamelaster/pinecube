#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <terrenc.h>

#if 0
#define ENCODING_TO_H264	1

typedef struct _encoderInfo
{
	VideoEncoder* pVideoEnc;	//视频编码器指针
	int codecType;	//编码器 codec 类型

#if (1 == ENCODING_TO_H264)
	VencH264Param h264Param;
	VencHeaderData sps_pps_data;	//for VENC_BUFFERFLAG_KEYFRAME (Like I frame info)
#endif

	VencBaseConfig baseConfig;	//编码器基本初始化信息，包括是否做 scaler，颜色格式等
	
	VencAllocateBufferParam bufferParam;	//buffer空间，用于图像的处理缓存

	/*
	inputbuffer: 输入图像帧的buffer, buffer可由AllocInputBuffer()获得获取
	相应变量的说明：
		1. nID: 用来区分不同的 buffer；
		2. nPts：当前图像帧的时间戳；
		3. pAddrPhyY：当前图像帧 Y 分量的物理地址，配给硬件使用；
		4. pAddrPhyC：当前图像帧 C 分量的物理地址，配给硬件使用；
		5. pAddrVirY：当前图像帧 Y 分量的虚拟地址，可由 CPU 来搬移图像数据到此 buffer；
		6. pAddrVirC：当前图像帧 C 分量的虚拟地址，可由 CPU 来搬移图像数据到此 buffer；
	*/
	VencInputBuffer inputBuffer;

	/*
	outputBuffer：输出图像帧 buffer；
	结构体变量说明：
		1.nID：用来识别不同的 buffer：
		2.nPts：编码器不对时间戳信息做处理，输出 buffer 中的 pts 对应相应输入buffer 中的 pts；
		3.nSize0：输出码流的第一部分的大小；
		4.nSize1：输出码流的第二部分的大小；
		5.pData0：输出码流的第一部分的地址；
		6.pData1：输出码流的第二部分的地址；
	*/
	VencOutputBuffer outputBuffer;

	unsigned int sizeY;
	unsigned int sizeC;


}encoderInfo_t;



void _encoderInfoInit(encoderInfo_t* p)
{
	//clear all info
	memset(p, 0, sizeof(encoderInfo_t));

	
#if (1 == ENCODING_TO_H264)
	//codec type
	p->codecType = VENC_CODEC_H264;
	
	//set h264 encoding param
	memset(&p->h264Param, 0, sizeof(VencH264Param));
	p->h264Param.sProfileLevel.nProfile = VENC_H264ProfileMain;
	p->h264Param.sProfileLevel.nLevel = VENC_H264Level31;
	p->h264Param.bEntropyCodingCABAC = 1;	///* 0:CAVLC 1:CABAC*/
	p->h264Param.sQPRange.nMinqp = 10;
	p->h264Param.sQPRange.nMaxqp = 40;
	p->h264Param.nFramerate = 25; /* fps */
	p->h264Param.nBitrate = 4*1280*720; /* bps 码率(code rate): 4*1024*1024 */
	p->h264Param.nMaxKeyInterval = p->h264Param.nFramerate / 2;
	p->h264Param.nCodingMode = VENC_FIELD_CODING;	//VENC_FRAME_CODING;
#endif

	memset(&p->baseConfig, 0 ,sizeof(VencBaseConfig));
	p->baseConfig.nInputWidth= 1280;
	p->baseConfig.nInputHeight = 720;
	p->baseConfig.nStride = 1280;
	p->baseConfig.nDstWidth = 1280;
	p->baseConfig.nDstHeight = 720;
	p->baseConfig.eInputFormat = VENC_PIXEL_YUV420SP;

	
	//for YUV420
	p->sizeY = p->baseConfig.nInputWidth * p->baseConfig.nInputHeight;
	p->sizeC = p->sizeY/2;

	
	memset(&p->bufferParam, 0, sizeof(VencAllocateBufferParam));
	p->bufferParam.nBufferNum = 2;	//4;
	p->bufferParam.nSizeY = p->sizeY;
	p->bufferParam.nSizeC = p->sizeC;

}

void _debugShowBufferInfo(encoderInfo_t* p)
{
	printf("###################################################\n");
	printf("---------------------------------------------------\n");
	printf("inputBuffer:\n");
	printf(" nID:%d\n nPts:%ld\n nFlags:%ld\n ispPicVar:%d\n", \
		p->inputBuffer.nID, p->inputBuffer.nPts, p->inputBuffer.nFlag, p->inputBuffer.ispPicVar);
	printf(" PhyY:%p VirY:%p\n PhyC:%p VirC:%p\n", p->inputBuffer.pAddrPhyY, p->inputBuffer.pAddrVirY, p->inputBuffer.pAddrPhyC, p->inputBuffer.pAddrVirC);
		
	printf("outputBuffer:\n");
	printf(" nID:%d\n nPts:%d\n nFlags:%ld\n nSize0:%d\n nSize1:%d\n pData0:%p\n pData1:%p\n", \
		p->outputBuffer.nID, p->outputBuffer.nPts, p->outputBuffer.nFlag, \
		p->outputBuffer.nSize0, p->outputBuffer.nSize1, \
		p->outputBuffer.pData0, p->outputBuffer.pData1);
	printf("---------------------------------------------------\n");

}

int main()
{
	int ret = 0, testNum = 0;
	
	//global info
	encoderInfo_t g;

	_encoderInfoInit(&g);
	
	FILE *in_file = NULL;
	FILE *out_file = NULL;

	if(VENC_CODEC_H264 == g.codecType)
	{
		in_file = fopen("./aaa.raw", "r");
		if(in_file == NULL)
		{
			printf("open in_file fail\n");
			return -1;
		}
		
		out_file = fopen("./1080p.264", "wb");
		if(out_file == NULL)
		{
			printf("open out_file fail\n");
			return -1;
		}
	}

	//1. 视频编码器支持创建多个编码器，支持多路编码
	g.pVideoEnc = VideoEncCreate(g.codecType);
	if(!g.pVideoEnc)
	{
		printf("VideoEncCreate");
		goto out;
	}
	
	//2. 设置编码信息
	if(VENC_CODEC_H264 == g.codecType)
	{
		VideoEncSetParameter(g.pVideoEnc, VENC_IndexParamH264Param, &g.h264Param);
	}

	//3. 初始化视频编码器
	ret = VideoEncInit(g.pVideoEnc, &g.baseConfig);
	if(ret)
		printf("VideoEncInit");
	
	//4 获取buffer空间，用于图像的处理缓存
	ret = AllocInputBuffer(g.pVideoEnc, &g.bufferParam);


	//5.1 保存每一帧
	for(testNum=0; testNum< g.h264Param.nFramerate; testNum++)
	{
	
		unsigned int _sizeY, _sizeC;
		
		ret = GetOneAllocInputBuffer(g.pVideoEnc, &g.inputBuffer);
		if(ret)
			printf("GetOneAllocInputBuffer");
			
		_sizeY = fread(g.inputBuffer.pAddrVirY, 1, g.sizeY, in_file);
		_sizeC = fread(g.inputBuffer.pAddrVirC, 1, g.sizeC, in_file);

		if((_sizeY != g.sizeY) || (_sizeC != g.sizeC))
		{
			fseek(in_file, 0L, SEEK_SET);
			_sizeY = fread(g.inputBuffer.pAddrVirY, 1, g.sizeY, in_file);
			_sizeC = fread(g.inputBuffer.pAddrVirC, 1, g.sizeC, in_file);
		}
		
		//g.inputBuffer.bEnableCorp = 0;
		//g.inputBuffer.nPts = testNum;

		//5.2 
		/*
			当调用 GetOneAllocInputBuffer 获取到由 AllocInputBuffer 申请的输入图像帧
			buffer的时，如果通过 CPU 来搬移输入的图像帧数据到此buffer，在把此 buffer
			送给编码器之前，需要调用此接口来保证 dram 和 cache 中的数据一致性；
		*/
		
		ret = FlushCacheAllocInputBuffer(g.pVideoEnc, &g.inputBuffer);
		if(ret)
			printf("FlushCacheAllocInputBuffer");

		//6. 入队列，把需要处理的图像地址放入到，pVideoEnc对应的队列地址上
		ret = AddOneInputBuffer(g.pVideoEnc, &g.inputBuffer);
		if(ret)
			printf("AddOneInputBuffer");
		
		
		_debugShowBufferInfo(&g);

		//7. 编码
		ret = VideoEncodeOneFrame(g.pVideoEnc);
		if(ret)
			printf("VideoEncodeOneFrame, ret: %d.", ret);

		//8. 出队列，获取处理队列中编码后的图像数据到inputBuffer中
		ret = AlreadyUsedInputBuffer(g.pVideoEnc, &g.inputBuffer);
		if(ret)
			printf("AlreadyUsedInputBuffer");
		
		ret = ReturnOneAllocInputBuffer(g.pVideoEnc, &g.inputBuffer);
		if(ret)
			printf("ReturnOneAllocInputBuffer");

		//9. 获取到out
		ret = GetOneBitstreamFrame(g.pVideoEnc, &g.outputBuffer);
		if(ret)
			printf("GetOneBitstreamFrame");

		//10. 写入文件
		if (VENC_CODEC_H264 == g.codecType)
		{
			if (g.outputBuffer.nFlag & VENC_BUFFERFLAG_KEYFRAME) {
				printf("get an KEY FRAME., frameNum: %d.", testNum);
				VideoEncGetParameter(g.pVideoEnc, VENC_IndexParamH264SPSPPS, &g.sps_pps_data);
				fwrite(g.sps_pps_data.pBuffer, 1, g.sps_pps_data.nLength, out_file);
			}
		}
		
		if(g.outputBuffer.nSize0)
			fwrite(g.outputBuffer.pData0, 1, g.outputBuffer.nSize0, out_file);
		
		if(g.outputBuffer.nSize1)
			fwrite(g.outputBuffer.pData1, 1, g.outputBuffer.nSize1, out_file);

		FreeOneBitStreamFrame(g.pVideoEnc, &g.outputBuffer);


		if((VENC_FIELD_CODING == g.h264Param.nCodingMode) && (VENC_CODEC_H264 == g.codecType))
		{
			GetOneBitstreamFrame(g.pVideoEnc, &g.outputBuffer);
			printf("oubuffer size: %d, %d", g.outputBuffer.nSize0, g.outputBuffer.nSize1);
			
			if(g.outputBuffer.nSize0)
				fwrite(g.outputBuffer.pData0, 1, g.outputBuffer.nSize0, out_file);

			if(g.outputBuffer.nSize1)
				fwrite(g.outputBuffer.pData1, 1, g.outputBuffer.nSize1, out_file);
				
			FreeOneBitStreamFrame(g.pVideoEnc, &g.outputBuffer);
		}
		
	}

out:
	if(out_file)
		fclose(out_file);
	if(in_file)
		fclose(in_file);

	return 0;
}
#else
int main()
{
	VideoEncCreate(VENC_CODEC_H264);
	// At the moment, video enc instance is global
	VideoEncSetParameter(NULL, VENC_IndexParamH264Param, NULL);
	//VideoEncInit(NULL, &g.baseConfig);
	return 0;
}
#endif