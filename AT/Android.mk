LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	AT.c

LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE) \
	$(MTK_PATH_SOURCE)external/sensor-tools	\
	$(MTK_PATH_SOURCE)external/mhal/src/custom/inc \
	$(MTK_PATH_SOURCE)external/nvram/libnvram \
	$(MTK_PATH_SOURCE)/kernel/drivers/video	\
	$(MTK_PATH_CUSTOM)/hal/inc \
	$(MTK_PATH_CUSTOM)/kernel/imgsensor/inc \
	$(MTK_PATH_CUSTOM)/cgen/cfgfileinc \
	$(MTK_PATH_CUSTOM)/cgen/cfgdefault \
	system/core/include

LOCAL_CFLAGS += -DANDROID

#LOCAL_SHARED_LIBRARIES := libandroid_runtime libnativehelper libmedia libutils libcutils libc

ifeq ($(TARGET_SIMULATOR),true)
LOCAL_LDLIBS += -ldl
else
LOCAL_SHARED_LIBRARIES += libdl
endif

LOCAL_STATIC_LIBRARIES :=
#LOCAL_STATIC_LIBRARIES += libminzip libunz libmtdutil libmincrypt libm
#LOCAL_STATIC_LIBRARIES += libminiui libpixelflinger_static libpng libz libcutils
LOCAL_STATIC_LIBRARIES += libstdc++ libc 

LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE:=AT
LOCAL_ARM_MODE := arm

include $(BUILD_EXECUTABLE)

