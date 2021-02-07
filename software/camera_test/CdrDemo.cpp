#define LOG_NDEBUG 0
#define LOG_TAG "CdrDemo"
#include <utils/Log.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <videodev2.h>
#include <linux/videodev.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <media/stagefright/foundation/ADebug.h>

#include "CdrConfig.h"
#include "CameraDemo.h"
#include "RecorderDemo.h"

using namespace android;

enum {
	DEMO_PREVIEW = 1<<0,
	DEMO_RECORDE = 1<<1,
};

enum CAMERA_TYPE {
	CAMERA_CSI = 1 << 0,
	CAMERA_UVC = 1 << 1,
};

static void printCommandLine() {
	printf("\ninput following character:\n");
	printf("\tr:start recorder\n");
	printf("\ts:stop  recorder\n");
	printf("\tp:start preview\n");
	printf("\tP:stop  preview\n");
	printf("\tq:exit demo\n");
}

class CdrDemo :public RecordDemoListener,
			   public CameraDemoListener {
public:
	CdrDemo();
	virtual ~CdrDemo();
	void setType(int32_t type);
	int32_t init(int cameraType);
	int32_t start();
	void loop();
	int32_t stop();
	int32_t reset();

	virtual void onRecorderEvent(RecorderDemo* cmr, int what, int extra);
	virtual void onCameraEvent(CameraDemo* cmr, int what, int extra);

private:
	int32_t initRecorder();
	int32_t startRecorder();
	int32_t stopRecorder();
	bool mStop;
	int32_t mFd;
	int32_t mDemoType;
	CameraDemo *mCSICamera;
	CameraDemo *mUVCCamera;
	RecorderDemo *mCSIRecorder;
	RecorderDemo *mUVCRecorder;
};

CdrDemo::CdrDemo()
	:mStop(false),
	 mFd(-1),
	 mDemoType(0),
	 mCSICamera(NULL),
	 mUVCCamera(NULL),
	 mCSIRecorder(NULL),
	 mUVCRecorder(NULL){

}

CdrDemo::~CdrDemo() {
	reset();
}

void CdrDemo::onRecorderEvent(RecorderDemo* cmr, int what, int extra) {
	//mStop = true;
}

void CdrDemo::onCameraEvent(CameraDemo* cmr, int what, int extra) {
	mStop = true;
}

void CdrDemo::setType(int32_t type) {
	mDemoType = type;
}

int32_t CdrDemo::initRecorder() {
	ALOGV("init recorder");
	if(mCSICamera) {
		mCSIRecorder = new RecorderDemo();
		mCSIRecorder->setListener(this);
		ALOGV("recording from csi camera");
		mCSIRecorder->setCamera(mCSICamera->getCamera());

		mCSIRecorder->setFrameRate(DEFAULT_CSI_RECRODER_FRAME_RATE);
		mCSIRecorder->setBitRate(DEFAULT_CSI_RECORDER_BIT_RATE);
		Size size(DEFAULT_CSI_RECORDER_VIDEO_WIDTH, DEFAULT_CSI_RECORDER_VIDEO_HEIGHT);
		mCSIRecorder->setVideoSize(&size);
		mCSIRecorder->setMaxDuration(DEFAULT_RECORDER_MAX_FILE_DURATION_MS);
		mCSIRecorder->setMaxFileSize(DEFAULT_RECORDER_MAX_FILE_SIZE_BYTE);

		ALOGV("recorder output file %s", DEFAULT_CSI_OUTPUT_FILE_PATH);
		mFd = open(DEFAULT_CSI_OUTPUT_FILE_PATH, O_CREAT | O_TRUNC | O_RDWR, 0644);
		CHECK(mFd >= 0);
		mCSIRecorder->setOutputFd(mFd);
		mCSIRecorder->init();
	}
	if(mUVCCamera) {
		mUVCRecorder = new RecorderDemo();
		mUVCRecorder->setListener(this);
		ALOGV("recording from uvc camera");
		mUVCRecorder->setCamera(mUVCCamera->getCamera());

		mUVCRecorder->setFrameRate(DEFAULT_UVC_RECRODER_FRAME_RATE);
		mUVCRecorder->setBitRate(DEFAULT_UVC_RECORDER_BIT_RATE);
		Size size(DEFAULT_UVC_RECORDER_VIDEO_WIDTH, DEFAULT_UVC_RECORDER_VIDEO_HEIGHT);
		mUVCRecorder->setVideoSize(&size);
		mUVCRecorder->setMaxDuration(DEFAULT_RECORDER_MAX_FILE_DURATION_MS);
		mUVCRecorder->setMaxFileSize(DEFAULT_RECORDER_MAX_FILE_SIZE_BYTE);

		ALOGV("recorder output file %s", DEFAULT_UVC_OUTPUT_FILE_PATH);
		mFd = open(DEFAULT_UVC_OUTPUT_FILE_PATH, O_CREAT | O_TRUNC | O_RDWR, 0644);
		CHECK(mFd >= 0);
		mUVCRecorder->setOutputFd(mFd);
		mUVCRecorder->init();
	}

	return 0;
}

int32_t CdrDemo::startRecorder() {
	ALOGV("start recorder");
	int err = 0;
	if(mDemoType & DEMO_RECORDE) {
		if(mCSIRecorder) {
			err = mCSIRecorder->start();
		}
		if(!err && mUVCRecorder) {
			err = mUVCRecorder->start();
		}
	}
	return err;
}

int32_t CdrDemo::stopRecorder() {
	ALOGV("stop recorder");
	if(mDemoType & DEMO_RECORDE) {
		if(mCSIRecorder) {
			mCSIRecorder->stop();
			mCSIRecorder->reset();
			delete mCSIRecorder;
			mCSIRecorder = NULL;
		}
		if(mUVCRecorder) {
			mUVCRecorder->stop();
			mUVCRecorder->reset();
			delete mUVCRecorder;
			mUVCRecorder = NULL;
		}
	}
	return 0;
}

int32_t CdrDemo::init(int cameraType) {
	ALOGV("init");
	int x, y, w, h;
	getScreenSize(&x, &y, &w, &h);
	view_info csi_surface, uvc_surface;

	int csi_display_id = 0;
	int uvc_display_id = 0;

	//check if required camera is valid.

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
		close(fd);
	}

	if((cameraType & CAMERA_CSI) && !csi_camera_valid) {
		//csi is disabled
		ALOGE("cannot access /dev/video0, check driver and hardware");
		cameraType &=~CAMERA_CSI;
	}

	if((cameraType & CAMERA_UVC) && !uvc_camera_valid) {
		//uvc is disabled
		ALOGE("cannot access /dev/video1, check driver and hardware");
		cameraType &=~CAMERA_UVC;
	}

	ALOGV("screen (%d, %d, %d, %d)", x, y, w, h);
	if((cameraType & CAMERA_CSI) && (cameraType & CAMERA_UVC)) {
		//preview csi and uvc both.
		//set size according to physical screen.
		if(SCREEN_ROTATE_DIGREE == 270 ||
				SCREEN_ROTATE_DIGREE == 90) {
			csi_surface.x = 0;
			csi_surface.y = 0;
			csi_surface.w = w;
			csi_surface.h = h/2 - 10;

			uvc_surface.x = 0;
			uvc_surface.y = h/2 + 10;
			uvc_surface.w = w;
			uvc_surface.h = h/2 - 10;
		} else {
			csi_surface.x = 0;
			csi_surface.y = 0;
			csi_surface.w = w/2 - 10;
			csi_surface.h = h;

			uvc_surface.x = w/2 + 10;
			uvc_surface.y = 0;
			uvc_surface.w = w/2 - 10;
			uvc_surface.h = h;
		}

		csi_display_id = 0;
		uvc_display_id = 1;
	} else {
		//Actually we need one layer, set csi and uvc the same
		uvc_surface.x = csi_surface.x = x;
		uvc_surface.y = csi_surface.y = y;
		uvc_surface.w = csi_surface.w = w;
		uvc_surface.h = csi_surface.h = h;
		csi_display_id = uvc_display_id = 0;
	}
	//init CSI camera
	if(cameraType & CAMERA_CSI) {
		//csi or both
		mCSICamera = new CameraDemo;
		mCSICamera->setListener(this);
		mCSICamera->setWindowRect(csi_display_id, &csi_surface);
		err = mCSICamera->init(0);
		CHECK(err == 0);
//		mCSICamera->dumpParameter();
	}

	//init UVC camera
	if(cameraType & CAMERA_UVC) {
		//uvc or both
		mUVCCamera = new CameraDemo;
		mUVCCamera->setListener(this);
		mUVCCamera->setWindowRect(uvc_display_id, &uvc_surface);

		err = mUVCCamera->init(1);
		CHECK(err == 0);
//		mUVCCamera->dumpParameter();
	}

	if(mDemoType & DEMO_RECORDE) {
		err = initRecorder();
	}
	return err;
}

int32_t CdrDemo::start() {
	ALOGV("start");
	int err = 0;
	if(mDemoType & DEMO_PREVIEW) {
		if(mCSICamera) {
			err = mCSICamera->startPreview();
			CHECK(err == 0);
		}
		if(mUVCCamera) {
			err = mUVCCamera->startPreview();
			CHECK(err == 0);
		}
	}

	if(mDemoType & DEMO_RECORDE) {
		err = startRecorder();
	}
	return err;
}

void CdrDemo::loop() {

	int err;
	char buf[3];
	ALOGV("loop");
	printCommandLine();

	while (!mStop) {
		buf[0] = '\0';
		//get keyboard input.
		fd_set rfds;
		struct timeval tv;
		int retval;
		/* Watch stdin (fd 0) to see when it has input. */
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
        /* Wait up to five seconds. */
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        retval = select(1, &rfds, NULL, NULL, &tv);
		if (retval > 0) {
			//read
			read(0, buf, 3);
			switch (buf[0]) {
			case 'q':
				mStop = true;
				break;
			case 'r':
				//start recorder
				if (!(mDemoType & DEMO_RECORDE)) {
					mDemoType |= DEMO_RECORDE;
					initRecorder();
					startRecorder();
				}
				break;
			case 's':
				//stop recorder
				if (mDemoType & DEMO_RECORDE) {
					stopRecorder();
					mDemoType &= ~DEMO_RECORDE;
				}
				break;
			case 'p':
				//stop preview
				if (!(mDemoType & DEMO_PREVIEW)) {
					if(mCSICamera) {
						err = mCSICamera->startPreview();
						CHECK(err == 0);
					}
					if(mUVCCamera) {
						err = mUVCCamera->startPreview();
						CHECK(err == 0);
					}
					mDemoType |= DEMO_PREVIEW;
				}
				break;
			case 'P':
				//stop preview
				if (mDemoType & DEMO_PREVIEW) {
					if(mCSICamera) {
						err = mCSICamera->stopPreview();
						CHECK(err == 0);
					}
					if(mUVCCamera) {
						err = mUVCCamera->stopPreview();
						CHECK(err == 0);
					}
					mDemoType &= ~DEMO_PREVIEW;
				}
				break;
			default:
				printCommandLine();
				break;
			}
		}
	}
}

int32_t CdrDemo::stop() {
	int err = 0;
	ALOGV("stop");
	mStop = true;
	if(mDemoType & DEMO_RECORDE) {
		err = stopRecorder();
	}

	if(mDemoType & DEMO_PREVIEW) {
		if(mCSICamera) {
			err = mCSICamera->stopPreview();
			CHECK(err == 0);
		}
		if(mUVCCamera) {
			err = mUVCCamera->stopPreview();
			CHECK(err == 0);
		}
	}

	if(mFd >= 0) {
		close(mFd);
		mFd = -1;
	}
	return err;
}

int32_t CdrDemo::reset() {
	ALOGV("reset");
	if(mCSICamera) {
		mCSICamera->reset();
		delete mCSICamera;
		mCSICamera = NULL;
	}

	if(mUVCCamera) {
		mUVCCamera->reset();
		delete mUVCCamera;
		mUVCCamera = NULL;
	}

	if(mCSIRecorder) {
		mCSIRecorder->reset();
		delete mCSIRecorder;
		mCSIRecorder = NULL;
	}

	if(mUVCRecorder) {
		mUVCRecorder->reset();
		delete mUVCRecorder;
		mUVCRecorder = NULL;
	}

	mDemoType = 0;
	mStop = false;
	return 0;
}
////////////////////////////////////////////////////////////////
//singleton for catching exeception
static CdrDemo * demo = NULL;

static void printUsage(const char *app) {
	printf("Usage: %s <-c camera source> <-r | -p>\n", app);
	printf("	   camera source:0 for csi, 1 for uvc, 2 for both\n");
}

static void signal_handler(int signo) {
	ALOGV("catch signal %d", signo);
	switch(signo) {
	case SIGHUP:
	case SIGABRT:
	case SIGKILL:
	case SIGTERM:
	case SIGINT:
		if(demo) {
			demo->stop();
			demo->reset();
			delete demo;
			demo = NULL;
		}
		exit(1);
		break;
	}
}

static void testCsiProterty() {
	struct v4l2_capability cap;
	struct v4l2_frmsizeenum size_enum;
	size_enum.type = V4L2_FRMSIZE_TYPE_STEPWISE;
	size_enum.pixel_format = V4L2_PIX_FMT_NV21;
	char dev_node[64];
	int fd = -1;
	int err;
	for (int node = 0; node < 2; ++node) {
		snprintf(dev_node, 64, "/dev/video%d", node);
		err = access(dev_node, F_OK);
		if (err != 0) {
			continue;
		}
		fd = open(dev_node, O_RDWR | O_NONBLOCK, 0);
		if (fd < 0) {
			continue;
		}
		err = ioctl (fd, VIDIOC_QUERYCAP, &cap);
		if (err < 0) {
			close(fd);
			continue;
		}

		ALOGV("%s driver name %s", dev_node, cap.driver);
		for (int i = 0; i < 20; i++) {
			size_enum.index = i;
			if (-1 == ioctl (fd, VIDIOC_ENUM_FRAMESIZES, &size_enum)) {
				break;
			}
			ALOGD("min %dx%d, max %dx%d, step %dx%d",
				size_enum.stepwise.min_width,  size_enum.stepwise.min_height,
				size_enum.stepwise.step_width, size_enum.stepwise.step_height,
				size_enum.stepwise.max_width,  size_enum.stepwise.max_height);
			ALOGD("format index = %d, size_enum: %dx%d", i, size_enum.discrete.width, size_enum.discrete.height);
		}
		close(fd);
	}
}

int main(int argc, char *argv[]) {
	sp<ProcessState> proc(ProcessState::self());
	ProcessState::self()->startThreadPool();
    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGINT, signal_handler);

    int type = 0;
    if(argc <= 1) {
    	printUsage(argv[0]);
    	return 0;
    }
    int cameraSource = 0;
    int opt;
    while ((opt = getopt(argc, argv, "c:rp")) != -1) {
        switch (opt) {
        case 'r':
        	type |= DEMO_RECORDE;
            break;
        case 'p':
        	type |= DEMO_PREVIEW;
            break;
        case 'c':
    		if(*optarg == '1') {
    			cameraSource |= CAMERA_UVC;
    		} else if(*optarg == '2') {
    			cameraSource |= (CAMERA_CSI|CAMERA_UVC);
    		} else {
    			cameraSource |= CAMERA_CSI;
    		}
        	break;
        default:
        	printUsage(argv[0]);
        	exit(0);
        }
    }

	ALOGV("********** demo start ****************");
	ALOGV("camera source %d", cameraSource);
	demo = new CdrDemo();
	demo->setType(type);
	int err;

	err = demo->init(cameraSource);
	CHECK(err == 0);

	err = demo->start();
	CHECK(err == 0);

	demo->loop();

	err = demo->stop();
	CHECK(err == 0);

	demo->reset();
	delete demo;

	ALOGV("********** demo finished *************");
	return 0;
}
