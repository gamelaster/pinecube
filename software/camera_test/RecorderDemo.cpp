#define LOG_NDEBUG 0
#define LOG_TAG "RecorderDemo"
#include <utils/Log.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <media/stagefright/foundation/ADebug.h>

#include "CdrConfig.h"
#include "RecorderDemo.h"

enum {
	RECORDER_DEMO_STATUS_IDLE = 0,
	RECORDER_DEMO_STATUS_PREPARED,
	RECORDER_DEMO_STATUS_STARTED,
	RECORDER_DEMO_STATUS_STOPPED,
};

namespace android {

RecorderDemo::RecorderDemo()
	:mHMR(NULL) {
	ALOGV("RecorderDemo()");
	reset();
}

RecorderDemo::~RecorderDemo() {
	ALOGV("~RecorderDemo()");
	reset();
}

void RecorderDemo::setCamera(HerbCamera *camera) {
	ALOGV("setCamera");
	mHC = camera;
}

void RecorderDemo::setFrameRate(int32_t frameRate) {
	ALOGV("setFrameRate");
	mFrameRate = frameRate;
}

void RecorderDemo::setBitRate(int32_t bitRate) {
	ALOGV("setBitRate");
	mBitRate = bitRate;
}

void RecorderDemo::setMaxDuration(int32_t maxDurationMs) {
	ALOGV("setMaxDuration");
	mMaxDurationMs = maxDurationMs;
}

void RecorderDemo::setMaxFileSize(int64_t maxFileSizeBytes) {
	ALOGV("setMaxFileSize");
	mMaxFileSize = maxFileSizeBytes;
}

void RecorderDemo::setVideoSize(Size *size) {
	ALOGV("setVideoSize");
	mVideoSize.height = size->height;
	mVideoSize.width = size->width;
}

void RecorderDemo::setOutputFd(int32_t fd) {
	ALOGV("setOutputFd");
	mOutputFd = fd;
}

status_t RecorderDemo::init() {
	ALOGV("init");
	if(mStatus != RECORDER_DEMO_STATUS_IDLE) {
		return INVALID_OPERATION;
	}

	//valid check.
	CHECK(mHC != NULL);
	CHECK_GE(mOutputFd, 0);
	CHECK_GT(mFrameRate, 0);
	CHECK_GT(mBitRate, 0);
	CHECK_GT(mVideoSize.width, 0);
	CHECK_GT(mVideoSize.height, 0);
	CHECK_GT(mMaxDurationMs, 0);
	CHECK_GT(mMaxFileSize, 0);

	mHMR = new HerbMediaRecorder();
	mHMR->setOnInfoListener(this);
	mHMR->setOnDataListener(this);
	mHMR->setOnErrorListener(this);

	status_t err;
	err = mHMR->setCamera(mHC);
    if (err != NO_ERROR) {
    	ALOGE("set camera failed");
		return err;
    }
    err = mHMR->setAudioSource(HerbMediaRecorder::AudioSource::MIC);
	if (err != NO_ERROR) {
		ALOGE("set audio source failed");
		return err;
	}

	err = mHMR->setVideoSource(HerbMediaRecorder::VideoSource::CAMERA);
	if (err != NO_ERROR) {
		ALOGE("set camera failed");
   		return err;
    }

	mMuxerId = mHMR->addOutputFormatAndOutputSink(HerbMediaRecorder::OutputFormat::MPEG_4,
			mOutputFd, mMaxFileSize, false);

	err = mHMR->setVideoFrameRate(mFrameRate);
	CHECK(err == 0);
	err = mHMR->setVideoEncodingBitRate(mBitRate);
	CHECK(err == 0);
	err = mHMR->setVideoSize(mVideoSize.width, mVideoSize.height);
	CHECK(err == 0);
	err = mHMR->setVideoEncoder(HerbMediaRecorder::VideoEncoder::H264);
	CHECK(err == 0);

	err = mHMR->setAudioEncoder(HerbMediaRecorder::AudioEncoder::AAC);
	CHECK(err == 0);

	mHMR->setMaxDuration(mMaxDurationMs);
	err = mHMR->prepare();
	if(err != NO_ERROR) {
		ALOGE("prepare failed");
	} else {
		mStatus = RECORDER_DEMO_STATUS_PREPARED;
	}

	return err;
}

status_t RecorderDemo::start() {
	ALOGV("start");
	if(mStatus != RECORDER_DEMO_STATUS_PREPARED) {
		return INVALID_OPERATION;
	}
	status_t err = mHMR->start();
	if(err == NO_ERROR) {
		mStatus = RECORDER_DEMO_STATUS_STARTED;
	}
	return err;
}

status_t RecorderDemo::stop() {
	ALOGV("stop");
	if(mStatus != RECORDER_DEMO_STATUS_STARTED) {
		return INVALID_OPERATION;
	}
	status_t err = mHMR->stop();
	if(err == NO_ERROR) {
		mStatus = RECORDER_DEMO_STATUS_STOPPED;
	}
	return err;
}

status_t RecorderDemo::reset() {
	ALOGV("reset");
	stop();

	if(mHMR) {
		mHMR->release();
		delete mHMR;
		mHMR = NULL;
	}
	mHC = NULL;
	mListener = NULL;
	mMaxFileSize = 0;
	mMaxDurationMs = 0;
	mFrameRate = 0;
	mBitRate = 0;
	mMuxerId = -1;
	mOutputFd = -1;
	mVideoSize.width = 0;
	mVideoSize.height = 0;
	mStatus = RECORDER_DEMO_STATUS_IDLE;
	return OK;
}

void RecorderDemo::setListener(RecordDemoListener *listener) {
	ALOGV("setListener");
	mListener = listener;
}

void RecorderDemo::onInfo(HerbMediaRecorder *hmr, int what, int extra)
{
	if(mListener) {
		mListener->onRecorderEvent(this, what, extra);
	}
}

void RecorderDemo::onError(HerbMediaRecorder *hmr, int what, int extra)
{
	if(mListener) {
		mListener->onRecorderEvent(this, what, extra);
	}
}

void RecorderDemo::onData(HerbMediaRecorder *hmr, int what, int extra)
{
	if(mListener) {
		mListener->onRecorderEvent(this, what, extra);
	}
}
}
