#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := ledController

include $(IDF_PATH)/make/project.mk

CFLAGS      += -DLED_TYPE=LED_TYPE_WS2812 -DLED_MODE=LED_MODE_RGB
