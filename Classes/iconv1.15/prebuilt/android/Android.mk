LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := iconv_static
LOCAL_MODULE_FILENAME := iconv
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libiconv.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)
