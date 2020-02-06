LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := plthooker

LOCAL_LDLIBS := -llog

LOCAL_SRC_FILES := \
                ../src/elf_common.cpp \
                ../src/elf_hooker.cpp \
                ../src/elf_module.cpp

LOCAL_C_INCLUDES :=

LOCAL_LDFLAGS :=

LOCAL_CFLAGS := -Wno-write-strings \
                -DHAVE_LITTLE_ENDIAN
                
include $(BUILD_STATIC_LIBRARY)
