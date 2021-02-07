#ifndef CAMERA_DEMO_H_
#define CAMERA_DEMO_H_

#include <display/CedarDisplay.h>
#include <media/HerbCamera.h>
namespace android {
class CameraDemo;
class CameraDemoListener
{
public:
	CameraDemoListener(){}
	virtual ~CameraDemoListener(){}
	virtual void onCameraEvent(CameraDemo* cmr, int what, int extra) = 0;
};

class CameraDemo :public HerbCamera::PictureCallback,
				  public HerbCamera::ErrorCallback {
public:
	CameraDemo();
	virtual ~CameraDemo();
	void setWindowRect(int displayId, view_info *surface);
	/*cameraId: 0 for csi, 1 for uvc*/
	int32_t init(int cameraId);
	int32_t startPreview();
	int32_t stopPreview();
	int32_t reset();
	void setListener(CameraDemoListener *listener);
	void dumpParameter();

	HerbCamera *getCamera() {return mHC;}
	//inherit
	virtual void onPictureTaken(void *data, int size, HerbCamera* pCamera);
	virtual void onError(int error, HerbCamera *pCamera);

private:
	int32_t mCameraId;
	HerbCamera *mHC;
	CedarDisplay *mCD;
	int32_t mLayer;
	int32_t mStatus;
	CameraDemoListener *mListener;
};
}
#endif //CAMERA_DEMO_H_
