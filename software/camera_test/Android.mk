LOCAL_PATH:= $(call my-dir)

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
