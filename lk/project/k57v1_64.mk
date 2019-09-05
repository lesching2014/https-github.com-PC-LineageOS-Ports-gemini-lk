#
LOCAL_DIR := $(GET_LOCAL_DIR)
TARGET := k57v1_64
MODULES += app/mt_boot \
           dev/lcm
MTK_EMMC_SUPPORT = yes
MTK_KERNEL_POWER_OFF_CHARGING = yes
DEFINES += MTK_NEW_COMBO_EMMC_SUPPORT
DEFINES += MTK_GPT_SCHEME_SUPPORT
MTK_PUMP_EXPRESS_PLUS_SUPPORT := no
MTK_BQ25896_SUPPORT := yes
MTK_LCM_PHYSICAL_ROTATION = 0
CUSTOM_LK_LCM="nt35695B_fhd_dsi_cmd_auo_nt50358 nt35695_fhd_dsi_cmd_truly_nt50358"
#nt35595_fhd_dsi_cmd_truly_nt50358 = yes
MTK_SECURITY_SW_SUPPORT = yes
MTK_VERIFIED_BOOT_SUPPORT = yes
MTK_SEC_FASTBOOT_UNLOCK_SUPPORT = yes
BOOT_LOGO := fhd
DEBUG := 2
#DEFINES += WITH_DEBUG_DCC=1
DEFINES += WITH_DEBUG_UART=1
#DEFINES += WITH_DEBUG_FBCON=1
#DEFINES += MACH_FPGA_NO_DISPLAY=y
CUSTOM_LK_USB_UNIQUE_SERIAL=no
MTK_TINYSYS_SCP_SUPPORT = no
#DEFINES += NO_POWER_OFF=y
#DEFINES += MACH_FPGA=y
#DEFINES += FPGA_BOOT_FROM_LK=y
MTK_PROTOCOL1_RAT_CONFIG = C/Lf/Lt/W/T/G
MTK_GOOGLE_TRUSTY_SUPPORT=no
MTK_AB_OTA_UPDATER = no
MTK_DM_VERITY_OFF = no
