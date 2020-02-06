LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := unicornvm
LOCAL_SRC_FILES := ../../solib/libunicornvm.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := unicornvm_apitest
LOCAL_SHARED_LIBRARIES := unicornvm
LOCAL_SRC_FILES := ../apitest.cpp

include $(BUILD_EXECUTABLE)
