CONFIG_SYS_CONFIG_NAME := $(subst ",,$(CONFIG_SYS_CONFIG_NAME))

ccflags-y += -I$(srctree)/$(src)/$(CONFIG_SYS_CONFIG_NAME)

obj-y += $(CONFIG_SYS_CONFIG_NAME)/mhal_noe.o
obj-y += mdrv_noe.o
obj-y += mii_mgr.o
obj-y += cmd_noe.o
