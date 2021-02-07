LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

TEST_FROM_SERVER:=0

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils \
	libbinder \
	libmedia \
	libhwdisp \
	libmedia_proxy \
	libcamera_client \
	libstagefright_foundation

LOCAL_C_INCLUDES += \
	$(TOP)/frameworks/include/include_media \
	$(TOP)/frameworks/include/include_media/media \
	$(TOP)/device/softwinner/common/hardware/include
    		
LOCAL_SRC_FILES := \
	RecorderDemo.cpp \
	CameraDemo.cpp \
	CdrDemo.cpp

	
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := camera_test

include $(BUILD_EXECUTABLE)



include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils

LOCAL_C_INCLUDES += \
    $(TARGET_HARDWARE_INCLUDE)
    		
LOCAL_SRC_FILES := \
	CheckCamera.cpp
	
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := camera_check

include $(BUILD_EXECUTABLE)
