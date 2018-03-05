
# inc path
ccflags-y += -I$(srctree)/$(src)/inc
ccflags-y += -I$(srctree)/$(src)/drv/mgwin/pub
ccflags-y += -I$(srctree)/$(src)/drv/mgwin/pub/linux
ccflags-y += -I$(srctree)/$(src)/drv/mihal/pub
ccflags-y += -I$(srctree)/$(src)/drv/mihal/inc
ccflags-y += -I$(srctree)/$(src)/hal/mgwin/inc/hal
ccflags-y += -I$(srctree)/$(src)/hal/mgwin/inc/drv
ccflags-y += -I$(srctree)/$(src)/inc/hal
ccflags-y += -I$(srctree)/$(src)/hdmitx

ccflags-y += -DMSOS_TYPE_NOS
ccflags-y += -DCONFIG_MBOOT

obj-y += hal/mgwin/src/hal/hal_scl_mgwin.o
obj-y += hal/mgwin/src/drv/drv_scl_mgwin.o
obj-y += hal/mgwin/src/drv/drv_scl_mgwin_io_wrapper.o
obj-y += drv/mihal/src/mhal_disp_pnl_tbl.o
obj-y += drv/mihal/src/mhal_disp.o
obj-y += hdmitx/mhal_hdmitx.o
obj-y += disp_verify.o





