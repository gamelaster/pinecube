#ifndef CDR_CONFIG_H_
#define CDR_CONFIG_H_

#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SCREEN_ROTATE_DIGREE (270)

//Valid size will displayed when dumping camera parameter.

//preview picture size
#define DEFAULT_CSI_PREIVEW_WIDTH (960)
#define DEFAULT_CSI_PREIVEW_HEIGHT (540)
#define DEFAULT_CSI_PREVIEW_FRAME_RATE (30)

#define DEFAULT_UVC_PREIVEW_WIDTH (640)
#define DEFAULT_UVC_PREIVEW_HEIGHT (360)
#define DEFAULT_UVC_PREVIEW_FRAME_RATE (30)

//camera video size, encoder input size
#define DEFAULT_CSI_CAMERA_VIDEO_WIDTH (1280)
#define DEFAULT_CSI_CAMERA_VIDEO_HEIGHT (720)

#define DEFAULT_UVC_CAMERA_VIDEO_WIDTH (1280)
#define DEFAULT_UVC_CAMERA_VIDEO_HEIGHT (720)


//take picture size
#define DEFAULT_PICTURE_WIDTH (2560)
#define DEFAULT_PICTURE_HEIGHT (1440)

//video size, encoder output size
#define DEFAULT_CSI_RECORDER_VIDEO_WIDTH (1920)
#define DEFAULT_CSI_RECORDER_VIDEO_HEIGHT (1080)
#define DEFAULT_CSI_RECRODER_FRAME_RATE (30)
#define DEFAULT_CSI_RECORDER_BIT_RATE (14*1024*1024)

#define DEFAULT_UVC_RECORDER_VIDEO_WIDTH (1280)
#define DEFAULT_UVC_RECORDER_VIDEO_HEIGHT (720)
#define DEFAULT_UVC_RECRODER_FRAME_RATE (30)
#define DEFAULT_UVC_RECORDER_BIT_RATE (14*1024*1024)

#define DEFAULT_RECORDER_MAX_FILE_SIZE_BYTE (3*1024*1024*1024ll)//3G
#define DEFAULT_RECORDER_MAX_FILE_DURATION_MS (15*60*1000)//15 minutes

#define DEFAULT_CSI_OUTPUT_FILE_PATH ("/mnt/extsd/video/cdr_csi.mp4")
#define DEFAULT_UVC_OUTPUT_FILE_PATH ("/mnt/extsd/video/cdr_uvc.mp4")

static void getScreenSize(int *x, int *y, int *w, int *h) {
	int fd = open("/dev/graphics/fb0", O_RDWR);
	struct fb_var_screeninfo var;

	ioctl(fd, FBIOGET_VSCREENINFO, &var);
	*x = 0;
	*y = 0;
	*w = var.xres;
	*h = var.yres;

	if(SCREEN_ROTATE_DIGREE == 90
		|| SCREEN_ROTATE_DIGREE == 270) {
		//we need rotate to orignal phsycial resolustion.
		*w = var.yres;
		*h = var.xres;
	}
	close(fd);
}

#endif //CDR_CONFIG_H_
