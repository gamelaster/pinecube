#include "venc_device.h"
#include "recedar_hooks.h"

#include "h264_device.h"

// -RC status=implemented
VENC_DEVICE *VencoderDeviceCreate(VENC_CODEC_TYPE type)
{
	VENC_DEVICE *vencoder_device_handle = malloc(sizeof(VENC_DEVICE));
	//VENC_DEVICE **video_encoder_devices = dlsym(RTLD_NEXT, "video_encoder_devices");

	if (vencoder_device_handle) {
		memcpy(vencoder_device_handle, &h264_device, sizeof(VENC_DEVICE));
		// TODO: Supports only H.264, of course :-)
		//memcpy(vencoder_device_handle, video_encoder_devices[type], sizeof(VENC_DEVICE));
	}
	return vencoder_device_handle;
	// return _VencoderDeviceCreate(type);
}