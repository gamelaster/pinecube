#define LOG_NDEBUG 0
#define LOG_TAG "CameraDemo"
#include <utils/Log.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <media/stagefright/foundation/ADebug.h>

#include "CdrConfig.h"
#include "CameraDemo.h"

namespace android {

enum {
	CAMERA_DEMO_STATUS_IDLE = 0,
	CAMERA_DEMO_STATUS_PREVIEWING,
	CAMERA_DEMO_STATUS_STOPPED
};
enum {
    CAMERA_WATERMARK_DISABLE = 0,
    CAMERA_WATERMARK_ALL,
    CAMERA_WATERMARK_ONLY_MAIN_CH,
    CAMERA_WATERMARK_ONLY_SUB_CH,
    CAMERA_WATERMARK_LIST_END,
};

CameraDemo::CameraDemo()
	:mHC(NULL),
	 mCD(NULL),
	 mLayer(-1),
	 mStatus(CAMERA_DEMO_STATUS_IDLE),
	 mListener(NULL) {
	ALOGV("CameraDemo");
}

CameraDemo::~CameraDemo() {
	ALOGV("~CameraDemo");
	reset();
}

void CameraDemo::setWindowRect(int displayId, view_info *surface) {
	//we have to create display client for each layer, otherwise
	//we'll never release a pre-created layer since native display
	//client will cover old surface with new one.
	mCD = new CedarDisplay(displayId);
	mLayer = mCD->requestSurface(surface);
	ALOGV("layer %d, surface (%d, %d, %d, %d)", mLayer, surface->x, surface->y, surface->w, surface->h);
}

int32_t CameraDemo::init(int cameraId) {

	ALOGV("init");

	mHC = HerbCamera::open(cameraId); //0:video0 1:viedo1

	ALOGV("layer is %d", mLayer);
	HerbCamera::Parameters params;
	mHC->getParameters(&params);
	params.setVideoCaptureBufferNum(10);

	params.setPreviewFpsRange(5000,120000);
	int previewWidth, previewHeight;
	int videoWidth, videoHeight;

	int frameRate;
	if(cameraId == 0) {
		//csi
		previewWidth = DEFAULT_CSI_PREIVEW_WIDTH;
		previewHeight = DEFAULT_CSI_PREIVEW_HEIGHT;
		videoWidth = DEFAULT_CSI_CAMERA_VIDEO_WIDTH;
		videoHeight = DEFAULT_CSI_CAMERA_VIDEO_HEIGHT;
		frameRate = DEFAULT_CSI_PREVIEW_FRAME_RATE;
	} else if(cameraId == 1) {
		//uvc
		previewWidth = DEFAULT_UVC_PREIVEW_WIDTH;
		previewHeight = DEFAULT_UVC_PREIVEW_HEIGHT;
		videoWidth = DEFAULT_UVC_CAMERA_VIDEO_WIDTH;
		videoHeight = DEFAULT_UVC_CAMERA_VIDEO_HEIGHT;
		frameRate = DEFAULT_UVC_PREVIEW_FRAME_RATE;
	} else {
		return -1;
	}
	ALOGV("preview size (%d, %d), video size (%d, %d)", previewWidth, previewHeight, videoWidth, videoHeight);
	params.setPreviewSize(previewWidth, previewHeight);
	params.setVideoSize(videoWidth, videoHeight);
	params.setPreviewFrameRate(frameRate);
	params.setPreviewFlip(HerbCamera::Parameters::PREVIEW_FLIP::NoFlip);
	params.setPictureMode(CameraParameters::AWEXTEND_PICTURE_MODE_FAST);
	params.setPictureSizeMode(CameraParameters::AWEXTEND_PICTURE_SIZE_MODE::UseParameterPictureSize);
	params.setPreviewRotation(SCREEN_ROTATE_DIGREE);
	mHC->setParameters(&params);
	mHC->setPreviewDisplay(mLayer);
	mCameraId = cameraId;
	return 0;
}

int32_t CameraDemo::startPreview() {
	ALOGV("startPreview");
	const char *waterMark = "0,0,1";
	//0:disable, 1:all, 2:main, 3:sub
	//mHC->setWaterMark(1, SCREEN_ROTATE_DIGREE, waterMark);
	int32_t err = mHC->startPreview();
	if(err == 0) {
		//close UI layer
		mCD->open(8, 1);
		mCD->open(8, 0);
		mCD->open(mLayer, 1);
		mStatus = CAMERA_DEMO_STATUS_PREVIEWING;
	}
	return err;
}

int32_t CameraDemo::stopPreview() {
	ALOGV("stopPreview");
	if(mStatus != CAMERA_DEMO_STATUS_PREVIEWING) {
		return -1;
	}
	//open UI layer
	mCD->open(8, 1);

	mCD->open(mLayer, 0);
	mHC->stopPreview();
	//mHC->setWaterMark(0, SCREEN_ROTATE_DIGREE, NULL);
	mStatus = CAMERA_DEMO_STATUS_STOPPED;
	return 0;
}

int32_t CameraDemo::reset() {
	ALOGV("reset, mLayer %d", mLayer);
	stopPreview();

	if (mHC) {
		mHC->release();
		delete mHC;
		mHC = NULL;
	}

	if(mCD) {
		if(mLayer >= 0) {
			ALOGV("releaseSurface, mLayer %d", mLayer);
			mLayer = mCD->releaseSurface(mLayer);
			mLayer = -1;
		}
		delete mCD;
		mCD = NULL;
	}

	mStatus = CAMERA_DEMO_STATUS_IDLE;
	return 0;
}

void CameraDemo::dumpParameter() {
	HerbCamera::Parameters params;
	mHC->getParameters(&params);
	params.dump();
}

void CameraDemo::setListener(CameraDemoListener *listener) {
	mListener = listener;
}

void CameraDemo::onPictureTaken(void *data, int size, HerbCamera* pCamera) {
	ALOGV("onPictureTaken");
}

void CameraDemo::onError(int error, HerbCamera *pCamera) {
	ALOGV("onError");
	if(mListener) {
		mListener->onCameraEvent(this, error, 0);
	}
}

}
