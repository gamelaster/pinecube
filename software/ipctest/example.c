/*
************** Camera Demo ******************
 *
 * date    :  2013.9.8
 * author  :  chenzhixiang PD4
 * fuction :  use v4l2 to control Camera , get a frame from buffer to do encode
 * 
 *
 *
******************** end ************************
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "example.h"

#include <sys/mman.h>
#include <linux/videodev2.h>
#include <time.h>

#define IPC 1
/*
************** Main flow ************************
 *
 *  openCameraDev 
 *  ¡ý
 *  setCaptureParams
 *  ¡ý
 *  setVideoParams
 *  ¡ý
 *  reqBufs
 *  ¡ý
 *  queryBuf
 *  ¡ý
 *  startStreaming
 *  ¡ý
 *	while(1) getPreviewFrame -> do encode ->releasePreviewFrame;
 *  ¡ý
 *	stopStreaming
 *  ¡ý
 *  unmapBuf
 *  ¡ý
 *  closeCameraDev
 *
******************** end ************************
*/

FILE* fdd;

int main(int argc, char *argv[])
{
	fdd = fopen("aaa.raw", "wb");

	printf ("IPC Demo\n");

	// init Camera
	printf ("open Camera \n");
	if(openCameraDev()!=0)
	{
		// failed
		printf ("open Camera fail\n");
		return -1;
	}
	
	// set 30 fps
	printf ("setCaptureParams \n");
	setCaptureParams(25);

	// set params ,such as height width , format
	printf ("setVideoParams \n");
	setVideoParams(width_test,height_test,mCaptureFormat);

	// set max buffer num and req v4l2 allot buf
	int buf_cnt = NB_BUFFER;
	printf ("reqBufs \n");
	if(reqBufs(&buf_cnt)!=0)
	{
		// failed
		printf ("reqBufs fail\n");
		return -1;
	}
	printf ("reqBufs scuess\n");

	mBufferCnt = buf_cnt;

	// v4l2 query buffers
	printf ("queryBuf \n");
	if (queryBuf()!=0)
	{
		// failed
		printf ("queryBuf fail\n");
		return -1;
	}
	printf ("queryBuf scuess\n");

	printf ("startStreaming \n");
	if (startStreaming()!=0)
	{
		// failed
		printf ("startStreaming fail\n");
		return -1;
	}
	printf ("startStreaming scuess\n");

	// start capture and encode every frame
	int loop=1;
	while (1)
	{
		printf ("start caputure one frame\n");
		int ret;
		time_t timep;
		struct v4l2_buffer buf;
		struct V4L2BUF_t v4l2_buf;
		memset(&buf, 0, sizeof(struct v4l2_buffer));
		printf ("start getPreviewFrame\n");
		ret = getPreviewFrame(&buf);
		if (ret != 0)
		{
			usleep(500000);
			printf ("getPreviewFrame fail\n");
			continue;
		}
		printf ("getPreviewFrame scuess\n");
		time(&timep);
		printf("time() : %d \n",timep);

		if(loop==24)
		{
			loop=0;
//			printf ("#######################################");
//			printf ("getBrightrenn and save");
//			getBrightness();
//			setBrightness(-1);
			//saveFrame((void *)mMapMem.mem[buf.index],buf.length);

		}
		else
		{
			printf ("loop is %d\n",loop);
			loop++;
		}
		//frame date v4l2_buf.addrPhyY  in (buf.m.offset & 0x0fffffff).
		//frame date v4l2_buf.addrVirY  in ((unsigned int)mMapMem.mem[buf.index]).
		//frame date length    in  buf.length;
		//frame date bytesused in  buf.bytesused
		//frmae date 
		printf("Bytes used: %d, length: %d, len2: %d\n", buf.bytesused, buf.length, mMapMem.length);
		saveFrame((void *)mMapMem.mem[buf.index],1382400);
		// do encode

		//release this frame buffer and put it reture the v4l2 buffer queue.
		printf ("start releasePreviewFrame\n");
		
		releasePreviewFrame(buf.index);
		printf ("getPreviewFrame releasePreviewFrame\n");
	}
	return 0;
}

/*
 * name:         openCameraDev 
 * description:  use init Camera Device,default is "video0"
 * parms:        null
 * result:       return 0 if success ,else (such as -1) is false
*/
int openCameraDev()
{
	int ret = -1;
	mCameraFd = -1;
	struct v4l2_input inp;
	struct v4l2_capability cap;
	
	// open V4L2 device
	mCameraFd = open("../../dev/video0", O_RDWR | O_NONBLOCK, 0);
	if (mCameraFd == -1) 
	{ 
		// do something fail tips in application
		printf ("open video0 fail\n");
		return -1; 
	}
	printf ("open video0 scuess it is %d\n",mCameraFd);

	// check v4l2 device capabilities
	ret = ioctl (mCameraFd, VIDIOC_QUERYCAP, &cap);
	if (ret < 0) 
	{ 
		// Error opening device: unable to query device.
		printf ("unable to query device\n");
		return -1; 
    } 
	printf ("query scuess\n");

	if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) 
	{ 
		// Error opening device: video capture not supported.
		printf ("video capture not supported\n");
		return -1;
    } 
	printf ("support video scuess\n");

	if ((cap.capabilities & V4L2_CAP_STREAMING) == 0) 
	{ 
        // Capture device does not support streaming i/o.
		printf ("Capture device does not support streaming i/o\n");
        return -1;
    } 
	printf ("open video0 streaming scuess\n");


	inp.index = 0;//must 0 
	if (-1 == ioctl (mCameraFd, VIDIOC_S_INPUT, &inp))
	{
		//VIDIOC_S_INPUT error!
		return -1;
	}

	// try want to use capture format 
	if(tryFmt(V4L2_PIX_FMT_YUYV) == 0)
	{
		mCaptureFormat = V4L2_PIX_FMT_YUYV;	
		printf ("mCaptureFormat = V4L2_PIX_FMT_YUYV\n");
	}
	else
	{
		return -1;
	}
	mCaptureFormat = V4L2_PIX_FMT_NV12;	
	return 0;
}

/*
 * name:         closeCameraDev 
 * description:  use close Cmaera Device when exit application
 * parms:        null
 * result:       void
*/
void closeCameraDev()
{
	if (mCameraFd != NULL)
	{
		close(mCameraFd);
		mCameraFd = NULL;
	}
}


/*
 * name:         tryFmt 
 * description:  try to set a Pixel format if it can use
 * parms:        format . detial see Pixel format in videodev2.h
 * result:       return 0 if success ,else (such as -1) is false
*/
int tryFmt(int format)
{	
	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int i;
	for(i=0;i<12;i++)
	{
		fmtdesc.index = i;
		if (-1 == ioctl (mCameraFd, VIDIOC_ENUM_FMT, &fmtdesc))
		{
			break;
		}

		if (fmtdesc.pixelformat == format)
		{
			return 0;
		}
	}
	return -1;
}

/*
 * name:         setCaptureParams  
 * arguments:    int frameRate. such like 30 (fps)
 * description:  
 * result:       return 0 if success ,else (such as -1) is false
*/
int setCaptureParams(int frameRate)
{
	int ret;
	struct v4l2_streamparm params;
	mFrameRate = frameRate;
	params.parm.capture.timeperframe.numerator = 1;
	params.parm.capture.timeperframe.denominator = mFrameRate;
	params.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	params.parm.capture.capturemode = V4L2_MODE_VIDEO;

	ret = ioctl(mCameraFd, VIDIOC_S_PARM, &params);

	//v4l2setCaptureParams failed
	if (ret < 0)
	{
		printf ("v4l2setCaptureParams failed\n");
		return -1;
	}
	printf ("v4l2setCaptureParams scuess\n");
	return 0;
}



/*
 * name:         setVideoParams
 * arguments:    1. int width
 *				 2. int heigth
 *				 3. unsigned int pix_fmt . such like V4L2_PIX_FMT_YUYV... 
 *				 support detial see 'Pixel format' define in videodev2.h
 * description:  use ioctl to set video device for specify params like w,h,and pix_fmt
 * result:       return 0 if success ,else (such as -1) is false
*/
int setVideoParams(int width, int height, unsigned int pix_fmt)
{
	int ret;
	struct v4l2_format format;

	memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    format.fmt.pix.width  = width;
    format.fmt.pix.height = height;
	format.fmt.pix.pixelformat = mCaptureFormat;
	format.fmt.pix.field = V4L2_FIELD_NONE;
	printf ("start setVideoParams \n");
	ret = ioctl(mCameraFd, VIDIOC_S_FMT, &format); 
	if (ret < 0) 
	{ 
		printf ("setVideoParams failed\n");
		return ret; 
	} 
	printf ("setVideoParams scuess\n");

	mFrameWidth = format.fmt.pix.width;
	mFrameHeight = format.fmt.pix.height;

	return 0;
}

/*
 * name:         reqBufs
 * arguments:    int *buf_cnt. require buffer number.usually between 3 and 6. 
 * description:  it require v4l2 to allow number of buffer to save frame data. 
 * result:       return 0 if success ,else (such as -1) is false
*/
int reqBufs(int * buf_cnt)
{
	int ret;
	struct v4l2_requestbuffers rb;
	
	memset(&rb, 0, sizeof(rb));
    rb.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    rb.memory = V4L2_MEMORY_MMAP; 
    rb.count  = *buf_cnt; 
	
	ret = ioctl(mCameraFd, VIDIOC_REQBUFS, &rb); 
    if (ret < 0) 
	{ 
		return ret;
    } 

	*buf_cnt = rb.count;

	return 0;
}

/*
 * name:         queryBuf
 * arguments:    null
 * description:  1.Assigned to data cache into a physical address after VIDIOC_REQBUFS.
 *				 2.use mmap to mapping CameraFd(dev/video0).
 *				 3.use mMapMem save each of frame cache address.
 * result:       return 0 if success ,else (such as -1) is false
*/
int queryBuf()
{
	int ret;
	struct v4l2_buffer buf;
	int i;
	for (i=0; i < mBufferCnt; i++) 
	{  
        memset (&buf, 0, sizeof (struct v4l2_buffer)); 
		buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
		buf.memory = V4L2_MEMORY_MMAP; 
		buf.index  = i; 
		
		ret = ioctl (mCameraFd, VIDIOC_QUERYBUF, &buf); 
        if (ret < 0) 
		{ 
            return ret; 
        } 

 
        mMapMem.mem[i] = mmap (0, buf.length, 
                            PROT_READ | PROT_WRITE, 
                            MAP_SHARED, 
                            mCameraFd, 
                            buf.m.offset); 
		mMapMem.length = buf.length;
 
        if (mMapMem.mem[i] == MAP_FAILED) 
		{ 
            return -1; 
        } 

		// start with all buffers in queue
        ret = ioctl(mCameraFd, VIDIOC_QBUF, &buf); 
        if (ret < 0) 
		{ 
            return ret; 
        } 
	} 

	return 0;
}

/*
 * name:         unmapBuf
 * arguments:    null. 
 * description:  use unmapBuf when exit app. 
 * result:       return 0 if success ,else (such as -1) is false
*/
int unmapBuf()
{
	int ret;
	int i;
	for ( i = 0; i < mBufferCnt; i++) 
	{
		ret = munmap(mMapMem.mem[i], mMapMem.length);
        if (ret < 0) 
		{
			return ret;
		}

		mMapMem.mem[i] = NULL;
	}
	return 0;
}

/*
 * name:         startStreaming
 * arguments:    null. 
 * description:  use startStreaming after queruBuf and then camera wiil start capture.
 * result:       return 0 if success ,else (such as -1) is false
*/
int startStreaming()
{
	int ret; 
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	
  	ret = ioctl (mCameraFd, VIDIOC_STREAMON, &type); 
	if (ret < 0) 
	{  
		return ret; 
	} 
	return 0;
}

/*
 * name:         stopStreaming
 * arguments:    null. 
 * description:  use stopStreaming when stop capture.
 * result:       return 0 if success ,else (such as -1) is false
*/
int stopStreaming()
{
	int ret; 
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	
	ret = ioctl (mCameraFd, VIDIOC_STREAMOFF, &type); 
	if (ret < 0) 
	{ 
		return ret; 
	} 
	return 0;
}

/*
 * name:         getPreviewFrame
 * arguments:    struct v4l2_buffer *buf. detail see v4l2_buffer define in videodev2.h
 * description:  1.use getPreviewFrame to get a Caputre Frame from V4L2 frame buffer.
 *				 2.addrPhyY is in the buf.m.offset.
				   addrVirY	is in the mMapMem.mem[buf.index] after 'queryBuf' call
 * result:       return 0 if success ,else (such as -1) is false
*/
int getPreviewFrame(struct v4l2_buffer *buf)
{
	int ret;
	
	buf->type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    buf->memory = V4L2_MEMORY_MMAP; 
 
    ret = ioctl(mCameraFd, VIDIOC_DQBUF, buf); 
    if (ret < 0) 
	{ 
        //VIDIOC_DQBUF Failed 
        return -1; 			// can not return false
    }
	return 0;
}

/*
 * name:         releasePreviewFrame
 * arguments:    int index.release Freame index. 
 * description:  use releasePreviewFrame after getPreviewFrame and encode the frame data, 
 *				 then it will release this frame and rejoin the v4l2 frame buffer queue.
 * result:       void
*/
void releasePreviewFrame(int index)
{
	int ret;
	struct v4l2_buffer buf;
	memset(&buf, 0, sizeof(struct v4l2_buffer));
	buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	buf.memory = V4L2_MEMORY_MMAP; 
	buf.index = index;
		
	ret = ioctl(mCameraFd, VIDIOC_QBUF, &buf); 
	if (ret != 0) 
	{
	   //"releasePreviewFrame: VIDIOC_QBUF Failed
	}
}

/*
 * name:         getFrameRate
 * arguments:    null. 
 * description:  get present Frame Rate if camera support ,else is return default 30(fps);
 * result:       return 0 if success ,else (such as -1) is false
*/
int getFrameRate()
{
	int ret = -1;

	struct v4l2_streamparm parms;
	parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl (mCameraFd, VIDIOC_G_PARM, &parms);
	if (ret < 0) 
	{
		//VIDIOC_G_PARM getFrameRate error
		return ret;
	}

	int numerator = parms.parm.capture.timeperframe.numerator;
	int denominator = parms.parm.capture.timeperframe.denominator;
	
	if (numerator != 0
		&& denominator != 0)
	{
		return denominator / numerator;
	}
	else
	{
		//unsupported frame rate
		return 30;
	}
}



/*
 * name:         setImageEffect
 * arguments:    int effect. support effect see enum v4l2_colorfx in videodev2.h
 * description:  set Camera capture ImageEffect,default is as V4L2_COLORFX_NONE
 * result:       return 0 if success ,else (such as -1) is false
*/
int setImageEffect(int effect)
{
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_COLORFX;
	ctrl.value = effect;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		//setImageEffect failed!
		;
	else 
		//setImageEffect ok
		;

	return ret;
}


/*
 * name:         setWhiteBalance
 * arguments:    int wb.
 * description:  set Camera capture WhiteBalance.
 * result:       return 0 if success ,else (such as -1) is false.
*/
int setWhiteBalance(int wb)
{
	struct v4l2_control ctrl;
	int ret = -1;

	ctrl.id = V4L2_CID_DO_WHITE_BALANCE;
	ctrl.value = wb;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		//setWhiteBalance failed
		;
	else 
		//setWhiteBalance ok
		;
	return ret;
}


int setBrightness(int bh)
{
	struct v4l2_control ctrl;
	int ret = -1;
	ctrl.id = V4L2_CID_BRIGHTNESS;
	ctrl.value = bh;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
	{
		//setBrightness failed
		printf ("setBrightness failed\n");
	}
	return ret;
}

int getBrightness()
{
	struct v4l2_control ctrl;
	int ret = -1;
	ctrl.id = V4L2_CID_BRIGHTNESS;
	ctrl.value = 0;
	ret = ioctl(mCameraFd, VIDIOC_G_CTRL, &ctrl);
	if (ret < 0)
	{
		//getBrightness failed
		printf ("getBrightness failed\n");
		return -1;
	}
	return ctrl.value;
}
int setContrast(int ct)
{
	struct v4l2_control ctrl;
	int ret = -1;
	ctrl.id = V4L2_CID_CONTRAST;
	ctrl.value = ct;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
	{
		//setContrast failed
		printf ("setContrast failed\n");
	}	
	return ret;
}

int getContrast()
{
	struct v4l2_control ctrl;
	int ret = -1;
	ctrl.id = V4L2_CID_CONTRAST;
	ret = ioctl(mCameraFd, VIDIOC_G_CTRL, &ctrl);
	if (ret < 0)
	{
		//getBrightness failed
		printf ("getContrast failed\n");
		return -1;
	}
	return ctrl.value;
}


int setSaturation(int st)
{
	struct v4l2_control ctrl;
	int ret = -1;
	ctrl.id = V4L2_CID_SATURATION;
	ctrl.value = st;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
	{
		//setSaturation failed
		printf ("setSaturation failed\n");
	}
	return ret;
}

int getSaturation()
{
	struct v4l2_control ctrl;
	int ret = -1;
	ctrl.id = V4L2_CID_SATURATION;
	ret = ioctl(mCameraFd, VIDIOC_G_CTRL, &ctrl);
	if (ret < 0)
	{
		//getSaturation failed
		printf ("getSaturation failed\n");
		return -1;
	}
	return ctrl.value;
}


int setHue(int hu)
{
	struct v4l2_control ctrl;
	int ret = -1;
	ctrl.id = V4L2_CID_HUE;
	ctrl.value = hu;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
	{
		//setHue failed
		printf ("setHue failed\n");
	}
	return ret;
}

int getHue()
{
	struct v4l2_control ctrl;
	int ret = -1;
	ctrl.id = V4L2_CID_HUE;
	ret = ioctl(mCameraFd, VIDIOC_G_CTRL, &ctrl);
	if (ret < 0)
	{
		//getSaturation failed
		printf ("getHue failed\n");
		return -1;
	}
	return ctrl.value;
}


void YUYVToNV12(const void* yuyv, void *nv12, int width, int height)
{
	unsigned char* Y	= (unsigned char*)nv12;
	unsigned char* UV = (unsigned char*)Y + width * height;
	int i,j;
	memset((void *)Y,0x00,width * height*1.5);
	printf ("YUYVToNV12\n");
	for(i = 0; i < height; i += 2)
	{
		printf ("%d\n",i);
		for (j = 0; j < width; j++)
		{
			*(unsigned char*)((unsigned char*)Y + i * width + j) = *(unsigned char*)((unsigned char*)yuyv + i * width * 2 + j * 2);
			*(unsigned char*)((unsigned char*)Y + (i + 1) * width + j) = *(unsigned char*)((unsigned char*)yuyv + (i + 1) * width * 2 + j * 2);
			*(unsigned char*)((unsigned char*)UV + ((i * width) >> 1) + j) = *(unsigned char*)((unsigned char*)yuyv + i * width * 2 + j * 2 + 1);
		}
	}
}

void YUYVToNV21(const void* yuyv, void *nv21, int width, int height)
{
	unsigned char* Y	= (unsigned char*)nv21;
	unsigned char* VU = (unsigned char*)Y + width * height;
	int i;
	for(i = 0; i < height; i += 2)
	{
		int j;
		for (j = 0; j < width; j++)
		{
			*(unsigned char*)((unsigned char*)Y + i * width + j) = *(unsigned char*)((unsigned char*)yuyv + i * width * 2 + j * 2);
			*(unsigned char*)((unsigned char*)Y + (i + 1) * width + j) = *(unsigned char*)((unsigned char*)yuyv + (i + 1) * width * 2 + j * 2);

			if (j % 2)
			{
				if (j < width - 1)
				{
					*(unsigned char*)((unsigned char*)VU + ((i * width) >> 1) + j) = *(unsigned char*)((unsigned char*)yuyv + i * width * 2 + (j + 1) * 2 + 1);
				}
			}
			else
			{
				if (j > 1)
				{
					*(unsigned char*)((unsigned char*)VU + ((i * width) >> 1) + j) = *(unsigned char*)((unsigned char*)yuyv + i * width * 2 + (j - 1) * 2 + 1); 		
				}
			}
		}
	}
}

void YUYVToYU12(const void* yuyv, void *yu12, int width, int height)
{
	uint8_t* Y	= (uint8_t*)yu12;
	uint8_t* U  = (uint8_t*)Y + width * height; //uv ¿ªÍ·µØÖ·
	uint8_t* V  = (uint8_t*)Y + width*height*5/4;
	 int i;
	for( i = 0; i < height; i += 2)
	{
//		printf ("%d\n",i);
		int j;
		for ( j = 0; j < width; j++)
		{
			*(uint8_t*)((uint8_t*)Y + i * width + j) = *(uint8_t*)((uint8_t*)yuyv + i * width * 2 + j * 2);

			*(uint8_t*)((uint8_t*)Y + (i + 1) * width + j) = *(uint8_t*)((uint8_t*)yuyv + (i + 1) * width * 2 + j * 2);

			*(uint8_t*)((uint8_t*)U + ((i * width)/4) + j) = *(uint8_t*)((uint8_t*)yuyv + i * width * 2 + j * 4 + 1);

			*(uint8_t*)((uint8_t*)V + ((i * width)/4) + j) = *(uint8_t*)((uint8_t*)yuyv + i * width * 2 + j * 4 + 3);

		}
	}
}

void saveFrame(const void* frame,int length)
{
	fwrite(frame, 1, length, fdd);

}

