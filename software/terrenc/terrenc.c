#include "terrenc.h"
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>

int veDriverFd = -1; //gVeDriverFd
unsigned int veMemoryMap = 0;
unsigned int engineMemoryAddress = 0;

void VeSetDramType()
{
	// In S3, there is 32-DDR2 or 16-DDR3
	// TODO: Change this to something normal when there will be VE register struct (or other type of memory map description)
	write32(veMemoryMap, (read32(veMemoryMap) & 0xFFFCFFFF) | 0x20000);
	// TODO: Mutex
}

void VeReset(void) {
	ioctl(veDriverFd, IOCTL_RESET_VE, 0);
	VeSetDramType();
}

int VeInitialize(void) {
	int ret = 0;
	// TODO: Make VE initialization check.
	veDriverFd = open("/dev/cedar_dev", O_RDWR);
	if (veDriverFd < 0) {
		printf("Can't open /dev/cedar_dev\n");
		veDriverFd = -1;
		goto err;
	}
	ret = ioctl(veDriverFd, IOCTL_SET_REFCOUNT, 0);
	if (ret < 0) {
		close(veDriverFd);
		printf("IOCTL_SET_REFCOUNT failed\n");
		goto err;
	}

	ret = ioctl(veDriverFd, IOCTL_ENGINE_REQ, 0);
	if (ret < 0) {
		close(veDriverFd);
		printf("IOCTL_ENGINE_REQ failed\n");
		goto err;
	}

	/* ret = ioctl(veDriverFd, IOCTL_GET_ENV_INFO, 0); // This is no longer required due currect Cedar implementation uses ION.
	if (ret < 0) {
		close(veDriverFd);
		printf("IOCTL_GET_ENV_INFO failed\n");
		goto err;
	} */

	veMemoryMap = (unsigned int)mmap(NULL, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, veDriverFd, veMemoryMap); // last arg should be just zero?
	VeReset();


	// /dev/ifm is kernel module which exposes area with some identification number, nothing more is known.
	int ifmFd = open("/dev/ifm", O_RDWR);
	if (ifmFd < 0) {
		printf("Can't read ifm\n");
		abort(); // Looks like it's very important
	}
	int id = -1;
	read(ifmFd, &id, 1);
	close(ifmFd);

	printf("IFM ID: %d\n", id);
	if (id > 0 && id != 3 && id != 7) {
		printf("Bad IFM\n");
		abort();
	}

	return 0;
err:
	return -1;
}

int EncAdapterInitialize(void) {
	if (VeInitialize() < 0) {
		return -1;
	}
	/*if (MemAdapterOpen() < 0) {
		return -1;
	}*/ // TODO:
	//a0 = SecureMemAdapterOpen();
	//SecureMemAdapterClose();

	return 0;
}

/*
   S3 H264 Capabilities:
	0x1681              ; ICVersion
	1                   ; color_space_support
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1
	0, 0, 0
	0x1000              ; maxSupportedDstWidth
	0x1000              ; maxSupportedDstHeight
*/

struct {
	VencH264ProfileLevel profileLevel;
	VENC_CODING_MODE codingMode;
	uint32_t frameRate; // * 1000
	uint32_t bitrate;
	uint32_t maxKeyInterval;
	VencQPRange qpRange;
	VencBaseConfig baseConfig;
	uint8_t entropyCoding;
} h264_encoder_singleton;

void H264EncOpen()
{
	// this function in original driver allocatest really big struct
	// also, the library have struct of supported h264 capabilities
	// Looks like nothing important happens here, but this can change later...
}

uint64_t H264EncSetParamter(void* h264_encoder, enum VENC_INDEXTYPE parameterType, void* parameterData)
{
	if (parameterType == VENC_IndexParamH264Param) {
		VencH264Param* param = (VencH264Param*)parameterData;
		h264_encoder_singleton.profileLevel = param->sProfileLevel;
		h264_encoder_singleton.codingMode = param->nCodingMode;
		h264_encoder_singleton.frameRate = param->nFramerate * 1000;
		h264_encoder_singleton.bitrate = param->nBitrate;
		h264_encoder_singleton.maxKeyInterval = param->nMaxKeyInterval;
		h264_encoder_singleton.qpRange = param->sQPRange;
		h264_encoder_singleton.entropyCoding = param->bEntropyCodingCABAC;
		if (param->sProfileLevel.nProfile == VENC_H264ProfileBaseline || param->bEntropyCodingCABAC == 0) { // 0 = CAVLC, 1 = CABAC
			// Returns VENC?IndexParamBitRate
			// TODO: h264_encoder.field_6FD = 0;
		}
	}
	return 0;
}


int* VideoEncCreate(int type)
{
	if (EncAdapterInitialize() != 0) {
		fprintf(stderr, "Can't setup video engine.\n");
		return NULL;
	}
	printf("VE Version: %08X\n", read32(veMemoryMap + REG_VE_VERSION));
	H264EncOpen();
	return 0;
}

int VideoEncSetParameter(void* encoder, enum VENC_INDEXTYPE parameterType, void* parameterData)
{
	H264EncSetParamter(NULL, parameterType, parameterData);
	return 0;
}

int h264_check_capability(void *encoder)
{
	// TODO: Implement
	return 0;
}

int H264InitMemory(void* encoder)
{

	return 0;
}

void VeEnableEncoder()
{
	// TODO: Make it look better
	uint32_t ve_ctrl = *(uint32_t*)(veMemoryMap + REG_VE_CTRL);
	ve_ctrl |= 0x40u; // Enables ISP
	ve_ctrl |= 0x80u; // Enables AVC encoder
	// NOTE: For some reason, in blobs, the engine is not selected O_o
	*(uint32_t*)(veMemoryMap + REG_VE_CTRL) = ve_ctrl;
}

int PutBits(uint32_t data, uint8_t bits_count, uint32_t avcAddress)
{
	{ // CheckRegValue_h264
		uint8_t good = 0;
		int retries;
		for (retries = 10000; retries > 0; retries--) {
			if ((read32(avcAddress + REG_VE_AVC_STATUS) & 0x200) == 0x200) { // TODO: Unknown state
				good = 1;
				break;
			}
		}
		if (!good) {
			printf("PutBits failed\n");
			return -1;
		}
		// 0x200
	}
	write32(avcAddress + REG_VE_AVC_BITS_DATA, data);
	write32(avcAddress + REG_VE_AVC_TRIG, ((bits_count & 0x1F) << 8) | 0x01); // Put Bits mode + bits size
	return 0;
}

int PutUnsignedExponential(uint32_t exp, uint32_t avcAddress)
{
	exp++;
	return PutBits(exp, (32 - __builtin_clz(exp)) * 2 - 1, avcAddress);
}

int PutVlcSE(int exp, uint32_t avcAddress)
{
	exp = 2 * exp - 1;
	exp ^= (exp >> 31);
	return PutUnsignedExponential(exp, avcAddress);
}

int RbspTrailingBits(uint32_t avcAddress)
{
  uint32_t vle_length;

  PutBits(1, 1, avcAddress);
  while (read32(avcAddress + REG_VE_AVC_STATUS) == 0) ;
  vle_length = read32(avcAddress + REG_VE_AVC_VLE_LENGTH);
  PutBits(0, 8 - (vle_length & 7), avcAddress);
  return 8 - (vle_length & 7) + vle_length;
}

void h264_start_code(uint8_t nal_ref_idc, uint8_t nal_unit_type, uint32_t avcAddress)
{
	PutBits(0, 8, avcAddress); // start code
	PutBits(0, 8, avcAddress);
	PutBits(0, 8, avcAddress);
	PutBits(1, 8, avcAddress);
	// If anyone looking here, this is good reading: https://yumichan.net/video-processing/video-compression/introduction-to-h264-nal-unit/
	PutBits((nal_ref_idc << 5) | (nal_unit_type << 0), 8, avcAddress);  // seq_parameter_set

}

void h264_init_sps_pps(void* encoder)
{
	VeEnableEncoder();
	uint32_t avcAddress = veMemoryMap + REG_VE_AVC;

	write32(avcAddress + REG_VE_AVC_TRIG, 0x00); // TODO: Unknown state
	//write32(avcAddress + REG_VE_AVC_VLE_ADDR, bitStreamPhyAddr);
	//write32(avcAddress + REG_VE_AVC_VLE_ADDR_END, bitStreamPhyAddr);
	//write32(avcAddress + REG_VE_AVC_VLE_BIT_OFFSET, 8 * writeOffset);
	//write32(avcAddress + REG_VE_AVC_VLE_BIT_MAXIMUM, 8 * (freeBufferSize + writeOffset));
	// engineAddress->h264_ctrl = *(_DWORD *)&enc->prob_slice_type; TODO:
	{ // InitSPS
		h264_start_code(3, 7, avcAddress); // 0x67, nal_ref_idc (3) and nal_unit_type (7)

		PutBits(h264_encoder_singleton.profileLevel.nProfile, 8, avcAddress); // Profile
		PutBits(0, 8, avcAddress); // Constraints + reserved bits
		PutBits(h264_encoder_singleton.profileLevel.nLevel, 8, avcAddress); // Level
		PutUnsignedExponential(0, avcAddress); // seq_parameter_set_id
		if (h264_encoder_singleton.profileLevel.nProfile == VENC_H264ProfileHigh) {
			PutUnsignedExponential(1, avcAddress); //chroma_format_idc
			// TODO: (optional) residual_colour_transform_flag
			PutUnsignedExponential(0, avcAddress); // bit_depth_luma_minus8
			PutUnsignedExponential(0, avcAddress); // bit_depth_chroma_minus8
			PutBits(0, 1, avcAddress); // qpprime_y_zero_transform_bypass_flag
			PutBits(0, 1, avcAddress); // seq_scaling_matrix_present_flag
			// TODO: (optional) seq_scaling_matrix_present_flag
		}
		PutUnsignedExponential(4 - 4, avcAddress); // log2_max_frame_num_minus4, default value is 4 (h264_init_default_param)
		PutUnsignedExponential(0, avcAddress); // pic_order_cnt_type, default value is 0 (h264_init_default_param)
		if (0 /* pic_order_cnt_type */ == 0) {
			PutUnsignedExponential(8 - 4, avcAddress); // log2_max_pic_order_cnt_lsb_minus4, default value is 8 (h264_init_default_param)
		} // TODO: pic_order is one
		uint8_t num_ref_frames = /* TODO:!!! */ 1; // num_ref_frames, default value is 1 (h264_init_default_param)
		if (h264_encoder_singleton.codingMode == VENC_FIELD_CODING) {
			num_ref_frames++;
		}
		PutUnsignedExponential(num_ref_frames, avcAddress); // num_ref_frames
		uint8_t isSkipFrameEnabled = 0; // Disabled by default
		PutBits(isSkipFrameEnabled, 1, avcAddress); // gaps_in_frame_num_value_allowed_flag
		PutUnsignedExponential(h264_encoder_singleton.baseConfig.nDstWidth - 1, avcAddress); // pic_width_in_mbs_minus1
		if (h264_encoder_singleton.codingMode == VENC_FIELD_CODING) {
			PutUnsignedExponential((h264_encoder_singleton.baseConfig.nDstHeight >> 1) - 1, avcAddress);
			PutBits(0, 1, avcAddress); // frame_mbs_only_flag
			PutBits(0, 1, avcAddress); // mb_adaptive_frame_field_flag
		} else {
			PutUnsignedExponential(h264_encoder_singleton.baseConfig.nDstHeight - 1, avcAddress); // pic_width_in_mbs_minus1
			PutBits(1, 1, avcAddress); // frame_mbs_only_flag
		}
		PutBits(1, 1, avcAddress); // direct_8x8_inference_flag
		if (1) {
			/* if ( enc->dstWidthNormalized == enc->vencBaseConfig.nDstWidth
			&& enc->dstHeightNormalized == enc->vencBaseConfig.nDstHeight ) */
			// TODO: Needs to be implemented
			PutBits(0, 1, avcAddress); // frame_cropping_flag
		} else {
			PutBits(1, 1, avcAddress); // frame_cropping_flag
			// TODO: Needs to be implemented
		}
		uint8_t vui_present = 0;
		PutBits(vui_present, 1, avcAddress);
		if (vui_present) {
			// TODO: implement
		}
		RbspTrailingBits(avcAddress);
	}
	{ // InitPPS
		h264_start_code(3, 8, avcAddress); // 0x68, nal_ref_idc (3) and nal_unit_type (8)
		PutUnsignedExponential(0, avcAddress); // pic_parameter_set_id
		PutUnsignedExponential(0, avcAddress); // seq_parameter_set_id
		PutBits(1 /* TODO: */, 1, avcAddress); // entropy_coding_mode_flag
		PutBits(0, 1, avcAddress); // pic_order_present_flag
		PutUnsignedExponential(0, avcAddress); // num_slice_groups_minus
		uint8_t num_ref_idx_l0_active = 1; // Default: 1
		if (h264_encoder_singleton.entropyCoding == 1) { // CABAC
			num_ref_idx_l0_active = (2 * (num_ref_idx_l0_active + 1));
		}
		PutUnsignedExponential(num_ref_idx_l0_active - 1, avcAddress);// num_ref_idx_l0_active_minus1
		uint8_t num_ref_idx_l1_active = 1; // Default: 1
		if (h264_encoder_singleton.entropyCoding == 1) { // CABAC
			num_ref_idx_l1_active = (2 * (num_ref_idx_l1_active + 1));
		}
		PutUnsignedExponential(num_ref_idx_l1_active - 1, avcAddress);// num_ref_idx_l1_active_minus1
		PutBits(0, 1, avcAddress); // weighted_pred_flag
		PutVlcSE(0, avcAddress); // pic_init_qp_minus26
  		PutVlcSE(0, avcAddress); // pic_init_qs_minus26
  		PutBits(1, 1, avcAddress); // deblocking_filter_control_present_flag
  		uint8_t constrained_intra_pred_flag = 0; // Default: 0
  		PutBits(constrained_intra_pred_flag, 1, avcAddress); // constrained_intra_pred_flag
  		PutBits(0, 1, avcAddress); // redundant_pic_cnt_present_flag
		if (h264_encoder_singleton.profileLevel.nProfile == VENC_H264ProfileHigh && 0 /* transform_8x8_mode_flag, default: 0 */) {
  			// TODO:
		}
		RbspTrailingBits(avcAddress);
	}
	write32(avcAddress + REG_VE_AVC_TRIG, 0x02); // TODO: Unknown state

	// TODO: Copy spsPps buffer to BitStream buffer
	// EncAdapterMemFlushCache
	// EncAdapterDisableEncoder ???
}

void H264Init(void *encoder, VencBaseConfig *cfg)
{
	engineMemoryAddress = veMemoryMap + 0xB00;
	memcpy(&h264_encoder_singleton.baseConfig, cfg, sizeof(VencBaseConfig));
	// IspCreate
	h264_check_capability(encoder);
	H264InitMemory(encoder);
	h264_init_sps_pps(encoder);
}

int VideoEncInit(void *encoder, VencBaseConfig *cfg)
{
	// FrameBufferManagerCreate
	H264Init(encoder, cfg);
	return 0;
}

