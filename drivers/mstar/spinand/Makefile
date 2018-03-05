CONFIG_SYS_CONFIG_NAME := $(subst ",,$(CONFIG_SYS_CONFIG_NAME))
#obj-y += $(CONFIG_SYS_CONFIG_NAME)/drvSPINAND_uboot.o
obj-y += spi_nand.o
spi_nand-objs := drvSPINAND_hal.o \
                 drvSPINAND.o \
                 drvSPINAND_api.o \
                 spinand.o



ccflags-y += -I$(srctree)/$(src)/inc/common
ccflags-y += -I$(srctree)/$(src)/inc/config
ccflags-y += -I$(srctree)/$(src)/inc
