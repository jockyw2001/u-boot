CONFIG_SYS_CONFIG_NAME := $(subst ",,$(CONFIG_SYS_CONFIG_NAME))

ccflags-y += -I$(srctree)/$(src)/$(CONFIG_SYS_CONFIG_NAME)
ccflags-y += -I$(srctree)/$(src)
ccflags-y += -I$(srctree)/gpio/$(CONFIG_SYS_CONFIG_NAME)/gpio.h

obj-y += $(CONFIG_SYS_CONFIG_NAME)/hal_pwm.o
obj-y += ms_pwm.o
