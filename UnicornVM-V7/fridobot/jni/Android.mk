LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := plthooker
LOCAL_SRC_FILES := ../../../Common/PLTHooker/obj/local/armeabi-v7a/libplthooker.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := unicornvm
LOCAL_SRC_FILES := ../../solib/libunicornvm.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := hijackmain
LOCAL_SRC_FILES := hijackmain.cpp

LOCAL_CFLAGS += -fvisibility=hidden
LOCAL_CPPFLAGS += $(LOCAL_CFLAGS) -std=gnu++11
LOCAL_CONLYFLAGS += -std=gnu99

LOCAL_LDLIBS += -llog
LOCAL_SHARED_LIBRARIES := unicornvm
LOCAL_STATIC_LIBRARIES := plthooker

include $(BUILD_SHARED_LIBRARY)
