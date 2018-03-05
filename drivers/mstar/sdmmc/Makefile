obj-y += src/hal_card_regs.o
obj-y += src/hal_card_timer.o

ifeq ($(CONFIG_ARCH_INFINITY2),y)
obj-y += src/hal_sdmmc_v5.o
obj-y += src/hal_card_platform_iNF2.o
else ifeq ($(CONFIG_ARCH_CLEVELAND),y)
obj-y += src/hal_sdmmc_v5.o
obj-y += src/hal_card_platform_C4P.o
else ifeq ($(CONFIG_ARCH_INFINITY),y)
obj-y += src/hal_sdmmc_v5.o
obj-y += src/hal_card_platform_iNF.o
else ifeq ($(CONFIG_ARCH_INFINITY3),y)
obj-y += src/hal_sdmmc_v5.o
obj-y += src/hal_card_platform_iNF3.o
endif

obj-y += ms_sdmmc_ub.o
obj-y += ms_sdmmc_drv.o
obj-y += ms_sdmmc_verify.o