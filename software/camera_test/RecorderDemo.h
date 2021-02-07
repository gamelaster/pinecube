#ifndef RECORDER_DEMO_H_
#define RECORDER_DEMO_H_

#include <media/HerbMediaRecorder.h>

namespace android {

class RecorderDemo;

class RecordDemoListener
{
public:
	RecordDemoListener(){}
	virtual ~RecordDemoListener(){}
	virtual void onRecorderEvent(RecorderDemo* cmr, int what, int extra) = 0;
};

class RecorderDemo : public HerbMediaRecorder::OnInfoListener
					,public HerbMediaRecorder::OnErrorListener
					,public HerbMediaRecorder::OnDataListener {
public:
	RecorderDemo();
	virtual ~RecorderDemo();
	virtual void onInfo(HerbMediaRecorder *hmr, int what, int extra);
	virtual void onError(HerbMediaRecorder *hmr, int what, int extra);
	virtual void onData(HerbMediaRecorder *hmr, int what, int extra);

	void setListener(RecordDemoListener *listener);
	void setCamera(HerbCamera *camera);
	void setFrameRate(int32_t frameRate);//frames per second
	void setBitRate(int32_t bitRate);//bitspersecond
	void setVideoSize(Size *size);
	void setMaxDuration(int32_t maxDurationMs);
	void setMaxFileSize(int64_t maxFileSizeBytes);
	void setOutputFd(int32_t fd);

	status_t init();
	status_t start(void);
	status_t stop();
	status_t reset();

private:
	HerbMediaRecorder *mHMR;
	RecordDemoListener *mListener;
	HerbCamera *mHC;
	int64_t mMaxFileSize;
	int32_t mMaxDurationMs;
	int32_t mFrameRate;//fps
	int32_t mBitRate;//kbps
	int32_t mMuxerId;//which muxer?
	int32_t mStatus;
	int32_t mOutputFd;
	Size mVideoSize;
};

}
#endif//RECORDER_DEMO_H_
