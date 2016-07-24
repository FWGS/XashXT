#XashXT port
#Copyright (c) mittorn

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := server

include $(XASH3D_CONFIG)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a-hard)
LOCAL_MODULE_FILENAME = libserver_hardfp
endif

# Xash doesn't have any voice chat, so remove it from gamedll too.
LOCAL_CFLAGS += -D_LINUX -DCLIENT_WEAPONS -DNO_VOICEGAMEMGR -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -D_snprintf=snprintf \
	-fno-exceptions -Wextra -Wno-write-strings -fpermissive -DNX32 -DLINUX -fsigned-char

LOCAL_CPPFLAGS := $(LOCAL_CFLAGS) -frtti

SRCS :=
SRCS += ./monsters/aflock.cpp
SRCS += ./monsters/agrunt.cpp
SRCS += ./airtank.cpp
SRCS += ./monsters/animating.cpp
SRCS += ./monsters/animation.cpp
SRCS += ./monsters/apache.cpp
SRCS += ./monsters/barnacle.cpp
SRCS += ./monsters/barney.cpp
SRCS += ./monsters/bigmomma.cpp
SRCS += ./monsters/bloater.cpp
SRCS += ./bmodels.cpp
SRCS += ./monsters/bullsquid.cpp
SRCS += ./buttons.cpp
SRCS += ./cbase.cpp
SRCS += ./client.cpp
SRCS += ./combat.cpp
SRCS += ../game_shared/common.cpp
SRCS += ./monsters/controller.cpp
SRCS += ./crossbow.cpp
SRCS += ./crowbar.cpp
SRCS += ./monsters/defaultai.cpp
SRCS += ./dll_int.cpp
SRCS += ./doors.cpp
SRCS += ./effects.cpp
SRCS += ./egon.cpp
SRCS += ./explode.cpp
SRCS += ./monsters/flyingmonster.cpp
SRCS += ./func_break.cpp
SRCS += ./func_monitor.cpp
SRCS += ./func_tank.cpp
SRCS += ./game.cpp
SRCS += ./gamerules.cpp
SRCS += ./monsters/gargantua.cpp
SRCS += ./gauss.cpp
SRCS += ./monsters/genericmonster.cpp
SRCS += ./ggrenade.cpp
SRCS += ./globals.cpp
SRCS += ./glock.cpp
SRCS += ./monsters/gman.cpp
SRCS += ./monsters/h_ai.cpp
SRCS += ./h_battery.cpp
SRCS += ./h_cycler.cpp
SRCS += ./handgrenade.cpp
SRCS += ./monsters/hassassin.cpp
SRCS += ./monsters/headcrab.cpp
SRCS += ./healthkit.cpp
SRCS += ./monsters/hgrunt.cpp
SRCS += ./monsters/hornet.cpp
SRCS += ./hornetgun.cpp
SRCS += ./monsters/houndeye.cpp
SRCS += ./monsters/ichthyosaur.cpp
SRCS += ./monsters/islave.cpp
SRCS += ./items.cpp
SRCS += ./monsters/leech.cpp
SRCS += ./lights.cpp
SRCS += ./mapents.cpp
SRCS += ./maprules.cpp
SRCS += ../game_shared/mathlib.cpp
SRCS += ../game_shared/matrix.cpp
SRCS += ./physics/meshdesc.cpp
SRCS += ./monsters/monstermaker.cpp
SRCS += ./monsters/monsters.cpp
SRCS += ./monsters/monsterstate.cpp
SRCS += ./mortar.cpp
SRCS += ./mp5.cpp
SRCS += ./multiplay_gamerules.cpp
SRCS += ./monsters/nihilanth.cpp
SRCS += ./monsters/nodes.cpp
#SRCS += ./novodex.cpp
#SRCS += ./physics/NxUserStream.cpp
SRCS += ./monsters/osprey.cpp
SRCS += ../game_shared/parser.cpp
SRCS += ./pathcorner.cpp
SRCS += ./physents.cpp
SRCS += ./physic.cpp
SRCS += ./plane.cpp
SRCS += ./plats.cpp
SRCS += ./player.cpp
SRCS += ./monsters/playermonster.cpp
SRCS += ../game_shared/playermove.cpp
SRCS += ./python.cpp
SRCS += ./monsters/rat.cpp
SRCS += ./monsters/roach.cpp
SRCS += ./rpg.cpp
SRCS += ./satchel.cpp
SRCS += ./saverestore.cpp
SRCS += ./monsters/schedule.cpp
SRCS += ./monsters/scientist.cpp
SRCS += ./monsters/scripted.cpp
SRCS += ./shotgun.cpp
SRCS += ./singleplay_gamerules.cpp
SRCS += ./skill.cpp
SRCS += ./sound.cpp
SRCS += ./monsters/soundent.cpp
SRCS += ./spectator.cpp
SRCS += ./monsters/spritemonster.cpp
SRCS += ./monsters/squadmonster.cpp
SRCS += ./squeakgrenade.cpp
SRCS += ./stats.cpp
SRCS += ../game_shared/stringlib.cpp
SRCS += ./strings.cpp
SRCS += ./subs.cpp
SRCS += ./monsters/talkmonster.cpp
SRCS += ./teamplay_gamerules.cpp
SRCS += ./monsters/tentacle.cpp
#SRCS += ./physics/trace.cpp
SRCS += ./triggers.cpp
SRCS += ./tripmine.cpp
SRCS += ./monsters/turret.cpp
SRCS += ./util.cpp
SRCS += ../game_shared/vector.cpp
SRCS += ./weapons.cpp
SRCS += ./world.cpp
SRCS += ./monsters/xen.cpp
SRCS += ./monsters/zombie.cpp

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
			$(LOCAL_PATH)/../phys_shared

LOCAL_SRC_FILES := $(SRCS)

include $(BUILD_SHARED_LIBRARY)
