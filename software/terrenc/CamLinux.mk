#
# 1. Set the path and clear environment
# 	TARGET_PATH := $(call my-dir)
# 	include $(ENV_CLEAR)
#
# 2. Set the source files and headers files
#	TARGET_SRC := xxx_1.c xxx_2.c
#	TARGET_INc := xxx_1.h xxx_2.h
#
# 3. Set the output target
#	TARGET_MODULE := xxx
#
# 4. Include the main makefile
#	include $(BUILD_BIN)
#
# Before include the build makefile, you can set the compilaion
# flags, e.g. TARGET_ASFLAGS TARGET_CFLAGS TARGET_CPPFLAGS
#

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
#include $(TOP)/frameworks/av/media/CedarX-Projects/Config.mk
#LOCAL_C_INCLUDE := example.h
#LOCAL_C_INCLUDES += \
#	${CEDARX_TOP}/../LIBRARY/CODEC/VIDEO/ENCODER/include
#LOCAL_MODULE:= libcedarxcomponents
#LOCAL_CFLAGS += $(CEDARX_EXT_CFLAGS)
#LOCAL_LDFLAGS += $(CEDARX_TOP)/../CedarAndroidLib/$(CEDARX_PREBUILD_LIB_PATH)/libvencoder.so
LOCAL_SRC_FILES := main.c terrenc.c
LOCAL_MODULE := terrenc
include $(BUILD_EXECUTABLE)