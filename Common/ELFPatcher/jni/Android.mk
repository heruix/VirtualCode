LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := elfpatcher

LOCAL_LDLIBS :=

LOCAL_SRC_FILES := ../src/patchelf.cc

LOCAL_C_INCLUDES :=
LOCAL_LDFLAGS :=

LOCAL_CFLAGS := -DPAGESIZE=4096 -DPACKAGE_STRING=\"elfpatcher\"
LOCAL_CPPFLAGS += $(LOCAL_CFLAGS) -std=gnu++11
                
include $(BUILD_EXECUTABLE)
