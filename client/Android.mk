#XashXT
#Copyright (c) mittorn

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := client

include $(XASH3D_CONFIG)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a-hard)
LOCAL_MODULE_FILENAME = libclient_hardfp
endif

# Xash doesn't have any voice chat, so remove it from gamedll too.
LOCAL_CFLAGS += -D_LINUX -DCLIENT_WEAPONS -DNO_VOICEGAMEMGR -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -D_snprintf=snprintf \
	-fno-exceptions -Wextra -Wno-write-strings -fpermissive -DNX32 -DLINUX -DCLIENT_DLL=1 -fsigned-char

LOCAL_CPPFLAGS := $(LOCAL_CFLAGS) -frtti

SRCS :=
SRCS += ./ammo.cpp
SRCS += ./ammo_secondary.cpp
SRCS += ./ammohistory.cpp
SRCS += ./battery.cpp
SRCS += ./cdll_int.cpp
SRCS += ../game_shared/common.cpp
SRCS += ./death.cpp
SRCS += ./flashlight.cpp
SRCS += ./geiger.cpp
SRCS += ./health.cpp
SRCS += ./hud.cpp
SRCS += ./hud_msg.cpp
SRCS += ./hud_redraw.cpp
SRCS += ./hud_update.cpp
SRCS += ./input.cpp
SRCS += ./input_xash3d.cpp
SRCS += ../game_shared/mathlib.cpp
SRCS += ../game_shared/matrix.cpp
SRCS += ./menu.cpp
SRCS += ./message.cpp
SRCS += ./motd.cpp
SRCS += ./parsemsg.cpp
SRCS += ../game_shared/parser.cpp
SRCS += ../game_shared/playermove.cpp
SRCS += ./render/r_backend.cpp
SRCS += ./render/r_beams.cpp
SRCS += ./render/r_bloom.cpp
SRCS += ./render/r_cull.cpp
SRCS += ./render/r_debug.cpp
SRCS += ./render/r_grass.cpp
SRCS += ./render/r_light.cpp
SRCS += ./render/r_main.cpp
SRCS += ./render/r_mirror.cpp
SRCS += ./render/r_misc.cpp
SRCS += ./render/r_monitor.cpp
SRCS += ./render/r_movie.cpp
SRCS += ./render/r_opengl.cpp
SRCS += ./render/r_particle.cpp
SRCS += ./render/r_portal.cpp
SRCS += ./render/r_shadows.cpp
SRCS += ./render/r_sprite.cpp
SRCS += ./render/r_studio.cpp
SRCS += ./render/r_surf.cpp
SRCS += ./render/r_tempents.cpp
SRCS += ./render/r_view.cpp
SRCS += ./render/r_warp.cpp
SRCS += ./render/r_weather.cpp
SRCS += ./saytext.cpp
SRCS += ./scoreboard.cpp
SRCS += ./status_icons.cpp
SRCS += ./statusbar.cpp
SRCS += ../game_shared/stringlib.cpp
SRCS += ./text_message.cpp
SRCS += ./train.cpp
SRCS += ./utils.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/. \
		    $(LOCAL_PATH)/wpn_shared \
		    $(LOCAL_PATH)/../common \
		    $(LOCAL_PATH)/../engine/common \
		    $(LOCAL_PATH)/../engine \
		    $(LOCAL_PATH)/../public \
		    $(LOCAL_PATH)/../pm_shared \
			$(LOCAL_PATH)/../game_shared \
			$(LOCAL_PATH)/../server \
			$(LOCAL_PATH)/../server/monsters \
			$(LOCAL_PATH)/../server/physics \
			$(LOCAL_PATH)/../phys_shared \
			$(LOCAL_PATH)/render

LOCAL_SRC_FILES := $(SRCS)

include $(BUILD_SHARED_LIBRARY)