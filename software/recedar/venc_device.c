#include "venc_device.h"
#include "recedar_hooks.h"

// -RC status=hooked
VENC_DEVICE *VencoderDeviceCreate(VENC_CODEC_TYPE type)
{
	return _VencoderDeviceCreate(type);
}