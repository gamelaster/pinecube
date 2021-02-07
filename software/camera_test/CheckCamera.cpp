#define LOG_NDEBUG 0
#define LOG_TAG "CheckCamera"
#include <utils/Log.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <videodev2.h>
#include <linux/videodev.h>

int tryFmt(int fd) {
	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	for (int i = 0; i < 12; i++) {
		fmtdesc.index = i;
		if (-1 == ioctl (fd, VIDIOC_ENUM_FMT, &fmtdesc)) {
			break;
		}
		ALOGV("format index = %d, name = %s, v4l2 pixel format = %x(%s)\n", i, fmtdesc.description, fmtdesc.pixelformat,
				(char *)&fmtdesc.pixelformat);
	}
	return -1;
}

void trySize(int fd, int width, int height) {
	struct v4l2_format fmt;

	ALOGV("trySize w: %d, h: %d", width, height);

	memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width  = width;
    fmt.fmt.pix.height = height;

	//V4L2_PIX_FMT_MJPEG,V4L2_PIX_FMT_H264
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV21;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;

	int ret = ioctl(fd, VIDIOC_TRY_FMT, &fmt);
	if (ret < 0) {
		ALOGE("VIDIOC_TRY_FMT Failed: %s", strerror(errno));
		return ;
	}

	ALOGD("size is (%d, %d)", fmt.fmt.pix.width, fmt.fmt.pix.height);
}

int main(int argc, char *argv[]) {
	int err = 0;
	int fd ;
	char dev_node[64];
	bool csi_camera_valid = false;
	bool uvc_camera_valid = false;
	struct v4l2_capability cap;
	for (int i = 0; i < 255; ++i) {
		snprintf(dev_node, 64, "/dev/video%d", i);
		if (access(dev_node, F_OK)) {
			continue;
		}
		fd = open(dev_node, O_RDWR | O_NONBLOCK, 0);
		if (fd < 0) {
			continue;
		}
		if (ioctl (fd, VIDIOC_QUERYCAP, &cap) < 0) {
			close(fd);
			continue;
		}
		ALOGV("%s driver name %s", dev_node, cap.driver);
		if (strcmp((char*) cap.driver, "sunxi-vfe") == 0) {
			ALOGV("sunxi_csi device node is %s", dev_node);
			csi_camera_valid = true;
		} else if (strcmp((char*) cap.driver, "uvcvideo") == 0) {
			ALOGV("uvcvideo device node is %s", dev_node);
			uvc_camera_valid = true;
		}
		struct v4l2_frmsizeenum size_enum;
		size_enum.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		size_enum.pixel_format = V4L2_PIX_FMT_NV21;
		for (int i = 0; i < 20; i++) {
			size_enum.index = i;
			if (-1 == ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &size_enum)) {
				break;
			}
			ALOGV("format index = %d, size_enum: %dx%d", i, size_enum.discrete.width, size_enum.discrete.height);
			trySize(fd, size_enum.discrete.width, size_enum.discrete.height);
		}

		trySize(fd, 3840, 2160);
		tryFmt(fd);
		close(fd);
	}
	return 0;
}
