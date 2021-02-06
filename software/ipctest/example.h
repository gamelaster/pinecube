#include <linux/videodev2.h>

#define NB_BUFFER 7
#define V4L2_MODE_VIDEO				0x0002	/*  For video capture */
#define SUNXI_MEM_ALLOC 1
#define SUNXI_MEM_FREE 3
#define uint8_t unsigned char
#define width_test 1280
#define height_test 720

typedef struct v4l2_mem_map_t{
		void *	mem[NB_BUFFER]; 
		int 	length;
}v4l2_mem_map_t;

v4l2_mem_map_t	mMapMem;


typedef struct V4L2BUF_t
{
	unsigned int	addrPhyY;		// physical Y address of this frame
	unsigned int	addrPhyC;		// physical C address of this frame
	unsigned int	addrVirY;		// virtual Y address of this frame
	unsigned int	addrVirC;		// virtual C address of this frame
	unsigned int	width;
	unsigned int	height;
	int 			index;			// DQUE id number
	long long		timeStamp;		// time stamp of this frame
	int				format;
	
	int 			refCnt; 		// used for releasing this frame
}V4L2BUF_t;


V4L2BUF_t mV4l2buf[NB_BUFFER];

// v4l2 device handle
int mCameraFd; 

unsigned int mCaptureFormat;


/* Frame width */
int mFrameWidth;
/* Frame height */
int mFrameHeight;

int	mFrameRate;

// actually buffer counts
int	mBufferCnt;

int openCameraDev();
void closeCameraDev();
int tryFmt(int format);
int setCaptureParams(int frameRate);
int setVideoParams(int width, int height, unsigned int pix_fmt);
int reqBufs(int * buf_cnt);
int unmapBuf();
int queryBuf();
int startStreaming();
int stopStreaming();
int getPreviewFrame(struct v4l2_buffer *buf);
void releasePreviewFrame(int index);
int getFrameRate();
int setImageEffect(int effect);
int setWhiteBalance(int wb);
int setBrightness(int bh);
int setContrast(int ct);
int setSaturation(int st);
int setHue(int hu);
void YUYVToYU12(const void* yuyv, void *yu12, int width, int height);
void YUYVToNV12(const void* yuyv, void *nv12, int width, int height);
void YUYVToNV21(const void* yuyv, void *nv21, int width, int height);
void saveFrame(const void* frame,int length);