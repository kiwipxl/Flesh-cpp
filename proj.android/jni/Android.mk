LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(COCOS2DX_ROOT))
$(call import-add-path,$(COCOS2DX_ROOT)/external)
$(call import-add-path,$(COCOS2DX_ROOT)/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp


ROOT_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../proj.win32/src/*.cpp)
ROOT_SRC_FILES := $(ROOT_SRC_FILES:$(LOCAL_PATH)/%=%)

DEBUG_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../proj.win32/src/debug/*.cpp)
DEBUG_SRC_FILES := $(DEBUG_SRC_FILES:$(LOCAL_PATH)/%=%)

NETWORK_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../proj.win32/src/network/*.cpp)
NETWORK_SRC_FILES := $(NETWORK_SRC_FILES:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := $(ROOT_SRC_FILES)
LOCAL_SRC_FILES += $(DEBUG_SRC_FILES)
LOCAL_SRC_FILES += $(NETWORK_SRC_FILES)


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../proj.win32/src/

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
