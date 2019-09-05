#define LOG_TAG "DUMP"

#include <platform/ddp_info.h>
#include <platform/ddp_log.h>
#include <platform/ddp_path.h>
#include <platform/ddp_dump.h>
#include <platform/disp_drv_platform.h>
#include "platform/ddp_reg.h"


static char *ddp_signal_0(int bit)
{
	switch (bit) {
		case 31:
			return "dpi0_sel_mm_dpi0";
		case 30:
			return "dis0_sel_mm_dsi0";
		case 29:
			return "rdma1_sout1_mm_dpi0_sin2";
		case 28:
			return "rdma1_sout0_mm_dsi0_sin2";
		case 27:
			return "rdma1_mm_rdma1_sout";
		/* case 26: return "bit26-unused"; */
		case 25:
			return "ovl1_mout1_wdma1";
		case 24:
			return "ovl1_mout0_rdma1";
		case 23:
			return "ovl1_mm_ovl1_mout";
		case 22:
			return "wdma0_sel_wdma0";
		case 21:
			return "ufoe_mout2_wdma0_sin2";
		case 20:
			return "ufoe_mout1_dpi0_sin0";
		case 19:
			return "ufoe_mout0_dsi0_sin0";
		case 18:
			return "ufoe_mm_ufoe_mout";
		case 17:
			return "ufoe_sel_mm_ufoe";
		case 16:
			return "rdma0_sout3_dpi0_sin1";
		case 15:
			return "rdma0_sout2_dsi0_sin1";
		case 14:
			return "rdma0_sout1_color_sin0";
		case 13:
			return "rdma0_sout0_ufoe_sin0";
		case 12:
			return "rdma0_mm_rdma0_sout";
		case 11:
			return "dither_mout2_wdma0_sin1";
		case 10:
			return "dither_mout1_ufoe_sin1";
		case 9:
			return "dither_mout0_rdma0";
		case 8:
			return "dither_mm_dither_mout";
		/* case 7:  return "bit7-unused"; */
		case 6:
			return "aal_mm_gamma";
		case 5:
			return "ccorr_mm_aal";
		case 4:
			return "color_mm_ccorr";
		case 3:
			return "color_sel_mm_color";
		case 2:
			return "ovl0_mout1_wdma0_sin0";
		case 1:
			return "ovl0_mout0_color_sin1";
		/* case 0:  return "bit0-unused"; */
		default:
			return NULL;
	}
}

static char *ddp_signal_1(int bit)
{
	switch (bit) {
		case 15:
			return "ovl0_sel_ovl0_mout";
		case 14:
			return "ovl1_mout2_ovl0_sel";
		case 13:
			return "ovl1_sout1_ovl0_sel";
		case 12:
			return "ovl1_sout0_ovl1_4L";
		case 11:
			return "ovl1_2L_OVL1_sout";
		case 10:
			return "ovl0_sout1_ovl1_2L";
		case 9:
			return "ovl0_sout0_ovl0_sel";
		case 8:
			return "ovl0_ovl0_sout";
		default:
			return NULL;
	}
}

static char *ddp_greq_name(int bit)
{
	switch (bit) {
		case 10:
			return "mdp_wrot";
		case 9:
			return "mdp_wdma";
		case 8:
			return "mdp_rdma";
		case 7:
			return "ovl1_2L";
		case 6:
			return "ovl0_2L";
		case 5:
			return "wdma1";
		case 4:
			return "rdma1";
		case 3:
			return "ovl1";
		case 2:
			return "wdma0";
		case 1:
			return "rdma0";
		case 0:
			return "ovl0";
		default:
			return NULL;
	}
}

// 1 means SMI dose not grant, maybe SMI hang
static int ddp_greq_check(int reg_val)
{
	if (reg_val&0x3f) {
		DDPMSG("smi greq not grant module: ");
	} else {
		return 0;
	}

	if (reg_val&0x1) DDPMSG("disp_wdma1,");
	if (reg_val&0x1) DDPMSG("disp_rdma1,");
	if (reg_val&0x1) DDPMSG("disp_ovl1,");
	if (reg_val&0x1) DDPMSG("disp_wdma0,");
	if (reg_val&0x1) DDPMSG("disp_rdma0,");
	if (reg_val&0x1) DDPMSG("disp_ovl0,");
	DDPMSG("\n");

	return 0;
}

static char* ddp_get_mutex_module_name(unsigned int bit)
{
	switch (bit) {
		case  7:
			return "ovl0";
		case  8:
			return "ovl1";
		case  9:
			return "rdma0";
		case 10:
			return "rdma1";
		case 11:
			return "wdma0";
		case 12:
			return "color0";
		case 13:
			return "ccorr";
		case 14:
			return "aal";
		case 15:
			return "gamma";
		case 16:
			return "dither";
		case 17:
			return "wdma1";
		case 18:
			return "pwm0";
		case 19:
			return "ovl0_2l";
		case 20:
			return "ovl1_2l";
		default:
			return "mutex-unknown";
	}
}

char* ddp_get_fmt_name(DISP_MODULE_ENUM module, unsigned int fmt)
{
	if (module==DISP_MODULE_WDMA0 || module==DISP_MODULE_WDMA1) {
		switch (fmt) {
			case 0:
				return "rgb565";
			case 1:
				return "rgb888";
			case 2:
				return "rgba8888";
			case 3:
				return "argb8888";
			case 4:
				return "uyvy";
			case 5:
				return "yuy2";
			case 7:
				return "y-only";
			case 8:
				return "iyuv";
			case 12:
				return "nv12";
			default:
				DDPMSG("ddp_get_fmt_name, unknown fmt=%d, module=%d\n", fmt, module);
				return "unknown";
		}
	} else if (module==DISP_MODULE_OVL0 || module==DISP_MODULE_OVL1) {
		switch (fmt) {
			case 0:
				return "rgb565";
			case 1:
				return "rgb888";
			case 2:
				return "rgba8888";
			case 3:
				return "argb8888";
			case 4:
				return "uyvy";
			case 5:
				return "yuyv";
			default:
				DDPMSG("ddp_get_fmt_name, unknown fmt=%d, module=%d\n", fmt, module);
				return "unknown";
		}
	} else if (module==DISP_MODULE_RDMA0 || module==DISP_MODULE_RDMA1 || module==DISP_MODULE_RDMA2) { // todo: confirm with designers
		switch (fmt) {
			case 0:
				return "rgb565";
			case 1:
				return "rgb888";
			case 2:
				return "rgba8888";
			case 3:
				return "argb8888";
			case 4:
				return "uyvy";
			case 5:
				return "yuyv";
			default:
				DDPMSG("ddp_get_fmt_name, unknown fmt=%d, module=%d\n", fmt, module);
				return "unknown";
		}
	} else if (module==DISP_MODULE_MUTEX) {
		switch (fmt) {
			case 0:
				return "single";
			case 1:
				return "dsi0_vdo";
			case 2:
				return "dsi1_vdo";
			case 3:
				return "dpi";
			default:
				DDPMSG("ddp_get_fmt_name, unknown fmt=%d, module=%d\n", fmt, module);
				return "unknown";
		}
	} else {
		DDPMSG("ddp_get_fmt_name, unknown module=%d\n", module);
	}

	return "unknown";
}

static char *ddp_clock_0(int bit)
{
	switch (bit) {
		case 0:
			return "smi_common, ";
		case 1:
			return "smi_larb0, ";
		case 10:
			return "ovl0, ";
		case 11:
			return "ovl1, ";
		case 12:
			return "rdma0, ";
		case 13:
			return "rdma1, ";
		case 14:
			return "wdma0, ";
		case 15:
			return "color, ";
		case 16:
			return "ccorr, ";
		case 17:
			return "aal, ";
		case 18:
			return "gamma, ";
		case 19:
			return "dither, ";
		case 21:
			return "ufoe_mout, ";
		case 22:
			return "wdma1, ";
		case 23:
			return "ovl0_2L, ";
		case 24:
			return "ovl1_2L, ";
		case 25:
			return "ovl0_mout, ";
		default:
			return NULL;
	}
}

static char* ddp_clock_1(int bit)
{
	switch (bit) {
		case 0:
			return "disp_pwm_mm, ";
		case 1:
			return "disp_pwm_26m, ";
		case 2:
			return "dsi_engine, ";
		case 3:
			return "dsi_digital, ";
		case 4:
			return "dpi_pixel, ";
		case 5:
			return "dpi_engine, ";
		default :
			return "";
	}
}

static void mutex_dump_reg(void)
{
	int i = 0;

	DDPMSG("=DISP MUTEX REGS=\n");
	DDPMSG("INTEN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MUTEX_INTEN));
	DDPMSG("INTSTA=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MUTEX_INTSTA));
	for (i = 0; i < 6; i++) {
		DDPMSG("Mutex%d:\n", i);
		DDPMSG("EN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MUTEX0_EN + 0x20 * i));
		DDPMSG("RST=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MUTEX0_RST + 0x20 * i));
		DDPMSG("MOD=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MUTEX0_MOD + 0x20 * i));
		DDPMSG("SOF=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MUTEX0_SOF + 0x20 * i));
	}

	DDPMSG("DEBUG_OUT_SEL=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DEBUG_OUT_SEL));
}

static void mutex_dump_analysis(void)
{
	int i=0;
	int j=0;
	char mutex_module[512]= {'\0'};
	char * p = NULL;
	int len = 0;
	DDPMSG("=DISP Mutex Analysis=\n");
	for (i = 0; i < 5; i++) {
		p = mutex_module;
		len = 0;
		if ( DISP_REG_GET(DISP_REG_CONFIG_MUTEX_MOD(i))!=0 &&
		        ((DISP_REG_GET(DISP_REG_CONFIG_MUTEX_EN(i)+0x20*i)==1 &&
		          DISP_REG_GET(DISP_REG_CONFIG_MUTEX_SOF(i)+0x20*i)!=SOF_SINGLE ) ||
		         DISP_REG_GET(DISP_REG_CONFIG_MUTEX_SOF(i)+0x20*i)==SOF_SINGLE)) {
			len = sprintf(p,"MUTEX%d:mode=%s,module=(", i, ddp_get_fmt_name(DISP_MODULE_MUTEX, DISP_REG_GET(DISP_REG_CONFIG_MUTEX_SOF(i))));
			p += len;
			for (j=6; j<=18; j++) {
				if ((DISP_REG_GET(DISP_REG_CONFIG_MUTEX_MOD(i))>>j)&0x1) {
					len = sprintf(p,"%s,", ddp_get_mutex_module_name(j));
					p += len;
				}
			}
			DDPMSG("%s)\n",mutex_module);
		}
	}
}

static void  mmsys_config_dump_reg(void)
{
	DDPMSG("=DISP Config=\n");
	DDPMSG("MMSYS_INTEN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_INTEN));
	DDPMSG("MMSYS_INTSTA=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_INTSTA));
	DDPMSG("MFG_APB_TX_CON=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MFG_APB_TX_CON));
//    DDPMSG("PWM_APB_ERR_ADDR =0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_PWM_APB_ERR_ADDR));
	DDPMSG("OVL0_MOUT_EN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_OVL0_MOUT_EN));
	DDPMSG("OVL1_MOUT_EN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_OVL1_MOUT_EN));
	DDPMSG("DITHER_MOUT_EN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_DITHER_MOUT_EN));
	DDPMSG("UFOE_MOUT_EN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_UFOE_MOUT_EN));
	DDPMSG("MMSYS_MOUT_RST=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_MOUT_RST));
	DDPMSG("COLOR0_SIN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_COLOR0_SEL_IN));
	DDPMSG("WDMA0_SIN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_WDMA0_SEL_IN));
	DDPMSG("UFOE_SIN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_UFOE_SEL_IN));
	DDPMSG("DSI0_SIN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DSI0_SEL_IN));
	DDPMSG("DPI0_SIN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DPI0_SEL_IN));
	DDPMSG("RDMA0_SOUT_SIN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_RDMA0_SOUT_SEL_IN));
	DDPMSG("RDMA1_SOUT_SIN=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_RDMA1_SOUT_SEL_IN));
	DDPMSG("MM_MISC=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_MISC));
	DDPMSG("MM_CG_CON0=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_CG_CON0));
	DDPMSG("MM_CG_CON1=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_CG_CON1));
	DDPMSG("MM_HW_DCM_DIS0=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_HW_DCM_DIS0));
	DDPMSG("MM_HW_DCM_DIS1=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_HW_DCM_DIS1));
	DDPMSG("MM_SW0_RST_B=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_SW0_RST_B));
	DDPMSG("MM_SW1_RST_B=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_SW1_RST_B));
	DDPMSG("MM_LCM_RST_B=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_LCM_RST_B));
	DDPMSG("LB4_AXI_CFG_WD=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_LARB4_AXI_ASIF_CFG_WD));
	DDPMSG("LB4_AXI_CFG_RD=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_LARB4_AXI_ASIF_CFG_RD));
	DDPMSG("MM_DBG_OUT_SEL=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_DEBUG_OUT_SEL));
	DDPMSG("MM_DUMMY0=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_DUMMY0));
	DDPMSG("MM_DUMMY1=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_DUMMY1));
	DDPMSG("MM_DUMMY2=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_DUMMY2));
	DDPMSG("MM_DUMMY3=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_MMSYS_DUMMY3));
	DDPMSG("DISP_VALID_0=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_DL_VALID_0));
	DDPMSG("DISP_VALID_1=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_DL_VALID_1));
	DDPMSG("DISP_READY_0=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_DL_READY_0));
	DDPMSG("DISP_READY_1=0x%x\n", DISP_REG_GET(DISP_REG_CONFIG_DISP_DL_READY_1));
}

/* ------ clock:
Before power on mmsys:
CLK_CFG_0_CLR (address is 0x10000048) = 0x80000000 (bit 31).
Before using DISP_PWM0 or DISP_PWM1:
CLK_CFG_1_CLR(address is 0x10000058)=0x80 (bit 7).
Before using DPI pixel clock:
CLK_CFG_6_CLR(address is 0x100000A8)=0x80 (bit 7).

Only need to enable the corresponding bits of MMSYS_CG_CON0 and MMSYS_CG_CON1 for the modules:
smi_common, larb0, mdp_crop, fake_eng, mutex_32k, pwm0, pwm1, dsi0, dsi1, dpi.
Other bits could keep 1. Suggest to keep smi_common and larb0 always clock on.

--------valid & ready
example:
ovl0 -> ovl0_mout_ready=1 means engines after ovl_mout are ready for receiving data
        ovl0_mout_ready=0 means ovl0_mout can not receive data, maybe ovl0_mout or after engines config error
ovl0 -> ovl0_mout_valid=1 means engines before ovl0_mout is OK,
        ovl0_mout_valid=0 means ovl can not transfer data to ovl0_mout, means ovl0 or before engines are not ready.
*/

static void  mmsys_config_dump_analysis(void)
{
	unsigned int i = 0;
	unsigned int reg = 0;
	char clock_on[512]= {'\0'};
	char * pos = NULL;
	char *name;
	int len = 0;

	unsigned int valid0 = DISP_REG_GET(DISP_REG_CONFIG_DISP_DL_VALID_0);
	unsigned int valid1 = DISP_REG_GET(DISP_REG_CONFIG_DISP_DL_VALID_1);
	unsigned int ready0 = DISP_REG_GET(DISP_REG_CONFIG_DISP_DL_READY_0);
	unsigned int ready1 = DISP_REG_GET(DISP_REG_CONFIG_DISP_DL_READY_1);
	unsigned int greq   = DISP_REG_GET(DISP_REG_CONFIG_SMI_LARB0_GREQ);

	DDPMSG("=DISP MMSYS_CONFIG ANALYSIS=\n");
	DDPMSG("mmsys clock=0x%x,CG_CON0=0x%x,CG_CON1=0x%x\n",
	       DISP_REG_GET(DISP_REG_CLK_CFG_0_MM_CLK),
	       DISP_REG_GET(DISP_REG_CONFIG_MMSYS_CG_CON0),
	       DISP_REG_GET(DISP_REG_CONFIG_MMSYS_CG_CON1));
	if ((DISP_REG_GET(DISP_REG_CLK_CFG_0_MM_CLK)>>31)&0x1) {
		DDPERR("mmsys clock abnormal!\n");
	}
#if 0
	DDPMSG("PLL clock=0x%x\n", DISP_REG_GET(DISP_REG_VENCPLL_CON0));
	if (!(DISP_REG_GET(DISP_REG_VENCPLL_CON0)&0x1)) {
		DDPERR("PLL clock abnormal!!\n");
	}
#endif
	reg = DISP_REG_GET(DISP_REG_CONFIG_MMSYS_CG_CON0);
	for (i = 0; i < 32; i++) {
		if ((reg & (1 << i)) == 0) {
			name = ddp_clock_0(i);
			strncat(clock_on, name, sizeof(clock_on));
		}
	}

	reg = DISP_REG_GET(DISP_REG_CONFIG_MMSYS_CG_CON1);
	for (i = 0; i < 32; i++) {
		if ((reg & (1 << i)) == 0) {
			name = ddp_clock_1(i);
			strncat(clock_on, name, sizeof(clock_on));
		}
	}
	DDPMSG("clock on modules:%s\n", clock_on);

	// DDPMSG("clock_ef setting:%u,%u\n", DISP_REG_GET(DISP_REG_CONFIG_C09),DISP_REG_GET(DISP_REG_CONFIG_C10));
#if 0
	DDPMSG("clock_mm setting:%u \n",
	       DISP_REG_GET(DISP_REG_CONFIG_C11));
	if (DISP_REG_GET(DISP_REG_CONFIG_C11)&0xff000000!=0xff000000) {
		DDPMSG("error, MM clock bit 24~bit31 should be 1, but real value=0x%x", DISP_REG_GET(DISP_REG_CONFIG_C11));
	}
#endif

	if (((DISP_REG_GET(DISP_REG_CONFIG_C09)>>7)&0x7)==5 &&
	        ((DISP_REG_GET(DISP_REG_RDMA_SIZE_CON_0)&0xfff)>1200 ||
	         (DISP_REG_GET(DISP_REG_RDMA_SIZE_CON_1)&0xfffff)>1920)) {
		DDPERR("check clock1 setting error:(120,192),(%d, %d)\n",
		       DISP_REG_GET(DISP_REG_RDMA_SIZE_CON_0)&0xfff,
		       DISP_REG_GET(DISP_REG_RDMA_SIZE_CON_1)&0xfffff);
	} else if (((DISP_REG_GET(DISP_REG_CONFIG_C09)>>8)&0x3)==3 &&
	           ((DISP_REG_GET(DISP_REG_RDMA_SIZE_CON_0)&0xfff)>800 ||
	            (DISP_REG_GET(DISP_REG_RDMA_SIZE_CON_1)&0xfffff)>1280)) {
		DDPERR("check clock1 setting error:(80,128),(%d, %d)\n",
		       DISP_REG_GET(DISP_REG_RDMA_SIZE_CON_0)&0xfff,
		       DISP_REG_GET(DISP_REG_RDMA_SIZE_CON_1)&0xfffff);
	}
	if (((DISP_REG_GET(DISP_REG_CONFIG_C10)>>7)&0x7)==5 &&
	        ((DISP_REG_GET(DISP_REG_OVL_ROI_SIZE+DISP_INDEX_OFFSET*1)&0xfff)>1200 ||
	         ((DISP_REG_GET(DISP_REG_OVL_ROI_SIZE+DISP_INDEX_OFFSET*1)>>16)&0xfff)>1920)) {
		DDPERR("check clock2 setting error:(120,192),(%d, %d)\n",
		       DISP_REG_GET(DISP_REG_OVL_ROI_SIZE+DISP_INDEX_OFFSET*1)&0xfff,
		       (DISP_REG_GET(DISP_REG_OVL_ROI_SIZE+DISP_INDEX_OFFSET*1)>>16)&0xfff);
	} else if (((DISP_REG_GET(DISP_REG_CONFIG_C10)>>8)&0x3)==3 &&
	           ((DISP_REG_GET(DISP_REG_OVL_ROI_SIZE+DISP_INDEX_OFFSET*1)&0xfff)>800 ||
	            ((DISP_REG_GET(DISP_REG_OVL_ROI_SIZE+DISP_INDEX_OFFSET*1)>>16)&0xfff)>1280)) {
		DDPERR("check clock2 setting error:(80,128),(%d, %d)\n",
		       DISP_REG_GET(DISP_REG_OVL_ROI_SIZE+DISP_INDEX_OFFSET*1)&0xfff,
		       (DISP_REG_GET(DISP_REG_OVL_ROI_SIZE+DISP_INDEX_OFFSET*1)>>16)&0xfff);
	}

	DDPMSG("valid0=0x%x, valid1=0x%x, ready0=0x%x, ready1=0x%x, greq=0%x\n",
	       valid0,valid1,ready0,ready1, greq);
	for (i = 0; i < 32; i++) {
		name = ddp_signal_0(i);
		if (!name)
			continue;

		pos = clock_on;
		pos += sprintf(pos, "%25s: ", name);

		if ((valid0 & (1 << i)))
			pos += sprintf(pos, "%10s,", "valid");
		else
			pos += sprintf(pos, "%10s,", "not valid");

		if ((ready0 & (1 << i)))
			pos += sprintf(pos, "%10s", "ready");
		else
			pos += sprintf(pos, "%10s", "not ready");

		DDPMSG("%s\n", clock_on);
	}

	for (i = 0; i < 32; i++) {
		name = ddp_signal_1(i);
		if (!name)
			continue;

		pos = clock_on;
		pos += sprintf(pos, "%25s: ", name);

		if ((valid1 & (1 << i)))
			pos += sprintf(pos, "%10s,", "valid");
		else
			pos += sprintf(pos, "%10s,", "not valid");

		if ((ready1 & (1 << i)))
			pos += sprintf(pos, "%10s", "ready");
		else
			pos += sprintf(pos, "%10s", "not ready");

		DDPMSG("%s\n", clock_on);
	}

	/* greq: 1 means SMI dose not grant, maybe SMI hang */
	if (greq)
		DDPMSG("smi greq not grant module: ");
	else
		return 0;

	clock_on[0] = '\0';
	for (i = 0; i < 32; i++) {
		name = ddp_greq_name(i);
		if (!name)
			continue;
		strncat(clock_on, name, sizeof(clock_on));
		DDPMSG("%s\n", clock_on);
	}
}

static void gamma_dump_reg(void)
{
	DDPMSG("=DISP GAMMA REGS=\n");
	DDPMSG("EN=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_EN));
	DDPMSG("RESET=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_RESET));
	DDPMSG("INTEN=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_INTEN));
	DDPMSG("INTSTA=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_INTSTA));
	DDPMSG("STATUS=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_STATUS));
	DDPMSG("CFG=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_CFG));
	DDPMSG("IN_COUNT=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_INPUT_COUNT));
	DDPMSG("OUT_COUNT=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_OUTPUT_COUNT));
	DDPMSG("CHKSUM=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_CHKSUM));
	DDPMSG("SIZE=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_SIZE));
	DDPMSG("DUMMY_REG=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_DUMMY_REG));
	DDPMSG("LUT=0x%x\n", DISP_REG_GET(DISP_REG_GAMMA_LUT));
}

static void gamma_dump_analysis(void)
{
	DDPMSG("=DISP GAMMA ANALYSIS=\n");
	DDPMSG("gamma:en=%d,wh(%dx%d),in(%d,%d),out(%d,%d)\n",
	       DISP_REG_GET(DISP_REG_GAMMA_EN),
	       (DISP_REG_GET(DISP_REG_GAMMA_SIZE)>>16)&0x1fff,
	       DISP_REG_GET(DISP_REG_GAMMA_SIZE)&0x1fff,
	       DISP_REG_GET(DISP_REG_GAMMA_INPUT_COUNT)&0x1fff,
	       (DISP_REG_GET(DISP_REG_GAMMA_INPUT_COUNT)>>16)&0x1fff,
	       DISP_REG_GET(DISP_REG_GAMMA_OUTPUT_COUNT)&0x1fff,
	       (DISP_REG_GET(DISP_REG_GAMMA_OUTPUT_COUNT)>>16)&0x1fff);
}

static void merge_dump_reg(void)
{
#if 0
	DDPMSG("=DISP MERGE REGS=\n");
	DDPMSG("MERGE_EN=0x%x\n", DISP_REG_GET(DISP_REG_MERGE_ENABLE));
	DDPMSG("MERGE_SW_RESET=0x%x\n", DISP_REG_GET(DISP_REG_MERGE_SW_RESET));
	DDPMSG("MERGE_DEBUG=0x%x\n", DISP_REG_GET(DISP_REG_MERGE_DEBUG));
	return ;
#endif
}

static void merge_dump_analysis(void)
{
#if 0
	DDPMSG("=DISP MERGE ANALYSIS=\n");
	DDPMSG("merge:en=%d,debug=0x%x\n",DISP_REG_GET(DISP_REG_MERGE_ENABLE),DISP_REG_GET(DISP_REG_MERGE_DEBUG));
	return ;
#endif
}

static void color_dump_reg(DISP_MODULE_ENUM module)
{
	int index =0;
	if (DISP_MODULE_COLOR0==module) {
		index = 0;
	} else if (DISP_MODULE_COLOR1==module) {
		DDPMSG("error: DISP COLOR%d dose not exist!\n", index);
		return;
	}
	DDPMSG("=DISP COLOR%d REGS=\n", index);
	DDPMSG("CFG_MAIN=0x%x\n", DISP_REG_GET(DISP_COLOR_CFG_MAIN));
	DDPMSG("PXL_CNT_MAIN=0x%x\n", DISP_REG_GET(DISP_COLOR_PXL_CNT_MAIN));
	DDPMSG("LINE_CNT_MAIN=0x%x\n", DISP_REG_GET(DISP_COLOR_LINE_CNT_MAIN));
	DDPMSG("START=0x%x\n", DISP_REG_GET(DISP_COLOR_START));
	DDPMSG("INTER_IP_W=0x%x\n", DISP_REG_GET(DISP_COLOR_INTERNAL_IP_WIDTH));
	DDPMSG("INTER_IP_H=0x%x\n", DISP_REG_GET(DISP_COLOR_INTERNAL_IP_HEIGHT));
}

static void color_dump_analysis(DISP_MODULE_ENUM module)
{
	int index =0;

	if (DISP_MODULE_COLOR0==module) {
		index = 0;
	} else if (DISP_MODULE_COLOR1==module) {
		DDPMSG("error: DISP COLOR%d dose not exist!\n", index);
		return;
	}
	DDPMSG("=DISP COLOR%d ANALYSIS=\n", index);
	DDPMSG("color%d:bypass=%d,w=%d,h=%d,pixel_cnt=%d,line_cnt=%d\n",
	       index,
	       (DISP_REG_GET(DISP_COLOR_CFG_MAIN)>>7)&0x1,
	       DISP_REG_GET(DISP_COLOR_INTERNAL_IP_WIDTH),
	       DISP_REG_GET(DISP_COLOR_INTERNAL_IP_HEIGHT),
	       DISP_REG_GET(DISP_COLOR_PXL_CNT_MAIN)&0xffff,
	       (DISP_REG_GET(DISP_COLOR_LINE_CNT_MAIN)>>16)&0x1fff);
}

static void aal_dump_reg(void)
{
	DDPMSG("=DISP AAL REGS=\n");
	DDPMSG("EN=0x%x\n", DISP_REG_GET(DISP_AAL_EN));
	DDPMSG("INTEN=0x%x\n", DISP_REG_GET(DISP_AAL_INTEN));
	DDPMSG("INTSTA=0x%x\n", DISP_REG_GET(DISP_AAL_INTSTA));
	DDPMSG("CFG=0x%x\n", DISP_REG_GET(DISP_AAL_CFG));
	DDPMSG("IN_CNT=0x%x\n", DISP_REG_GET(DISP_AAL_IN_CNT));
	DDPMSG("OUT_CNT=0x%x\n", DISP_REG_GET(DISP_AAL_OUT_CNT));
	DDPMSG("SIZE=0x%x\n", DISP_REG_GET(DISP_AAL_SIZE));
	DDPMSG("CABC_00=0x%x\n", DISP_REG_GET(DISP_AAL_CABC_00));
	DDPMSG("CABC_02=0x%x\n", DISP_REG_GET(DISP_AAL_CABC_02));
	DDPMSG("STATUS_00=0x%x\n", DISP_REG_GET(DISP_AAL_STATUS_00));
	DDPMSG("STATUS_01=0x%x\n", DISP_REG_GET(DISP_AAL_STATUS_00 + 0x4));
	DDPMSG("STATUS_31=0x%x\n", DISP_REG_GET(DISP_AAL_STATUS_32 - 0x4));
	DDPMSG("STATUS_32=0x%x\n", DISP_REG_GET(DISP_AAL_STATUS_32));
	DDPMSG("DRE_GAIN_FILTER_00=0x%x\n", DISP_REG_GET(DISP_AAL_DRE_GAIN_FILTER_00));
	DDPMSG("DRE_MAPPING_00=0x%x\n", DISP_REG_GET(DISP_AAL_DRE_MAPPING_00));
}

static void aal_dump_analysis(void)
{
	DDPMSG("=DISP AAL ANALYSIS=\n");
	DDPMSG("aal:bypass=%d,relay=%d,en=%d,wh(%dx%d),in(%d,%d),out(%d,%d)\n",
	       DISP_REG_GET(DISP_AAL_EN)==0x0,
	       DISP_REG_GET(DISP_AAL_CFG)&0x01,
	       DISP_REG_GET(DISP_AAL_EN),
	       (DISP_REG_GET(DISP_AAL_SIZE)>>16)&0x1fff,
	       DISP_REG_GET(DISP_AAL_SIZE)&0x1fff,
	       DISP_REG_GET(DISP_AAL_IN_CNT)&0x1fff,
	       (DISP_REG_GET(DISP_AAL_IN_CNT)>>16)&0x1fff,
	       DISP_REG_GET(DISP_AAL_OUT_CNT)&0x1fff,
	       (DISP_REG_GET(DISP_AAL_OUT_CNT)>>16)&0x1fff);
}

static void pwm_dump_reg(DISP_MODULE_ENUM module)
{
	int index = 0;
	unsigned int reg_base = 0;
	if (module == DISP_MODULE_PWM0) {
		index = 0;
		reg_base = DISPSYS_PWM0_BASE;
	} else {
		index = 1;
		reg_base = DISPSYS_PWM1_BASE;
	}
	DDPMSG("=DISP PWM%d REGS=\n", index);
	DDPMSG("EN=0x%x\n", DISP_REG_GET(reg_base + DISP_PWM_EN_OFF));
	DDPMSG("CON_0=0x%x\n", DISP_REG_GET(reg_base + DISP_PWM_CON_0_OFF));
	DDPMSG("CON_1=0x%x\n", DISP_REG_GET(reg_base + DISP_PWM_CON_1_OFF));
	DDPMSG("DEBUG=0x%x\n", DISP_REG_GET(reg_base + 0x28));
}

static void pwm_dump_analysis(DISP_MODULE_ENUM module)
{
	int index = 0;
	unsigned int reg_base = 0;
	if (module == DISP_MODULE_PWM0) {
		index = 0;
		reg_base = DISPSYS_PWM0_BASE;
	} else {
		index = 1;
		reg_base = DISPSYS_PWM1_BASE;
	}
	DDPMSG("=DISP PWM%d ANALYSIS=\n", index);
	DDPMSG("pwm clock=%d\n", (DISP_REG_GET(DISP_REG_CLK_CFG_1_CLR)>>7)&0x1);
}

static void od_dump_reg(void)
{
	DDPMSG("=DISP OD REGS=\n");
	DDPMSG("EN=0x%x\n", DISP_REG_GET(DISP_REG_OD_EN));
	DDPMSG("RESET=0x%x\n", DISP_REG_GET(DISP_REG_OD_RESET));
	DDPMSG("INTEN=0x%x\n", DISP_REG_GET(DISP_REG_OD_INTEN));
	DDPMSG("INTSTA=0x%x\n", DISP_REG_GET(DISP_REG_OD_INTSTA));
	DDPMSG("STATUS=0x%x\n", DISP_REG_GET(DISP_REG_OD_STATUS));
	DDPMSG("CFG=0x%x\n", DISP_REG_GET(DISP_REG_OD_CFG));
	DDPMSG("INPUT_COUNT=0x%x\n", DISP_REG_GET(DISP_REG_OD_INPUT_COUNT));
	DDPMSG("OUTPUT_COUNT=0x%x\n", DISP_REG_GET(DISP_REG_OD_OUTPUT_COUNT));
	DDPMSG("CHKSUM=0x%x\n", DISP_REG_GET(DISP_REG_OD_CHKSUM));
	DDPMSG("SIZE=0x%x\n", DISP_REG_GET(DISP_REG_OD_SIZE));
	DDPMSG("HSYNC_WIDTH=0x%x\n", DISP_REG_GET(DISP_REG_OD_HSYNC_WIDTH));
	DDPMSG("VSYNC_WIDTH=0x%x\n", DISP_REG_GET(DISP_REG_OD_VSYNC_WIDTH));
	DDPMSG("MISC=0x%x\n", DISP_REG_GET(DISP_REG_OD_MISC));
	DDPMSG("DUMMY_REG=0x%x\n", DISP_REG_GET(DISP_REG_OD_DUMMY_REG));
}

static void od_dump_analysis(void)
{
	DDPMSG("=DISP OD ANALYSIS=\n");
	DDPMSG("od:wh(%dx%d),bypass=%d\n",
	       (DISP_REG_GET(DISP_REG_OD_SIZE)>>16)&0xffff,
	       DISP_REG_GET(DISP_REG_OD_SIZE)&0xffff,
	       DISP_REG_GET(DISP_REG_OD_CFG)&0x1);
}

static void ccorr_dump_reg(void)
{
	DDPMSG("=DISP CCORR REGS=\n");
	DDPMSG("EN=0x%x\n", DISP_REG_GET(DISP_REG_CCORR_EN));
	DDPMSG("CFG=0x%x\n", DISP_REG_GET(DISP_REG_CCORR_CFG));
	DDPMSG("IN_CNT=0x%x\n", DISP_REG_GET(DISP_REG_CCORR_IN_CNT));
	DDPMSG("OUT_CNT=0x%x\n", DISP_REG_GET(DISP_REG_CCORR_OUT_CNT));
	DDPMSG("SIZE=0x%x\n", DISP_REG_GET(DISP_REG_CCORR_SIZE));
}

static void ccorr_dump_analyze(void)
{
	DDPMSG("ccorr:en=%d,config=%d,wh(%dx%d),in(%d,%d),out(%d,%d)\n",
	       DISP_REG_GET(DISP_REG_CCORR_EN),
	       DISP_REG_GET(DISP_REG_CCORR_CFG),
	       (DISP_REG_GET(DISP_REG_CCORR_SIZE)>>16)&0x1fff,
	       DISP_REG_GET(DISP_REG_CCORR_SIZE)&0x1fff,
	       DISP_REG_GET(DISP_REG_CCORR_IN_CNT)&0x1fff,
	       (DISP_REG_GET(DISP_REG_CCORR_IN_CNT)>>16)&0x1fff,
	       DISP_REG_GET(DISP_REG_CCORR_OUT_CNT)&0x1fff,
	       (DISP_REG_GET(DISP_REG_CCORR_OUT_CNT)>>16)&0x1fff);
}

static void dither_dump_reg(void)
{
	DDPMSG("=DISP DITHER REGS=\n");
	DDPMSG("EN=0x%x\n", DISP_REG_GET(DISP_REG_DITHER_EN));
	DDPMSG("CFG=0x%x\n", DISP_REG_GET(DISP_REG_DITHER_CFG));
	DDPMSG("IN_CNT=0x%x\n", DISP_REG_GET(DISP_REG_DITHER_IN_CNT));
	DDPMSG("OUT_CNT=0x%x\n", DISP_REG_GET(DISP_REG_DITHER_OUT_CNT));
	DDPMSG("SIZE=0x%x\n", DISP_REG_GET(DISP_REG_DITHER_SIZE));
}

static void dither_dump_analyze(void)
{
	DDPMSG("dither:en=%d,config=%d,w=%d,h=%d,in(%d,%d),out(%d,%d)\n",
	       DISP_REG_GET(DISPSYS_DITHER_BASE + 0x000),
	       DISP_REG_GET(DISPSYS_DITHER_BASE + 0x020),
	       (DISP_REG_GET(DISP_REG_DITHER_SIZE)>>16)&0x1fff,
	       DISP_REG_GET(DISP_REG_DITHER_SIZE)&0x1fff,
	       DISP_REG_GET(DISP_REG_DITHER_IN_CNT)&0x1fff,
	       (DISP_REG_GET(DISP_REG_DITHER_IN_CNT)>>16)&0x1fff,
	       DISP_REG_GET(DISP_REG_DITHER_OUT_CNT)&0x1fff,
	       (DISP_REG_GET(DISP_REG_DITHER_OUT_CNT)>>16)&0x1fff);
}

static void ufoe_dump_reg(void)
{
	//DDPMSG("==DISP UFOE REGS==\n");
	//DDPMSG("(0x000)UFOE_START =0x%x\n", DISP_REG_GET(DISP_REG_UFO_START));
	return;
}

static void ufoe_dump_analysis(void)
{
	//DDPMSG("==DISP UFOE ANALYSIS==\n");
	//DDPMSG("ufoe: bypass=%d,0x%08x\n",
	//    DISP_REG_GET(DISP_REG_UFO_START)==0x4,
	//    DISP_REG_GET(DISP_REG_UFO_START));
	return;
}

static void dsi_dump_reg(DISP_MODULE_ENUM module)
{
	int i =0;

	if (module == DISP_MODULE_DSI0 || module == DISP_MODULE_DSIDUAL) {
		DDPMSG("=DISP DSI0 REGS=\n");
		for (i = 0; i < 20*16; i += 16) {
			DDPMSG("DSI0+%04x: 0x%08x  0x%08x  0x%08x  0x%08x\n", i, INREG32(DISPSYS_DSI0_BASE + i), INREG32(DISPSYS_DSI0_BASE + i + 0x4), INREG32(DISPSYS_DSI0_BASE + i + 0x8), INREG32(DISPSYS_DSI0_BASE + i + 0xc));
		}
		DDPMSG("DSI0 CMDQ+0x200: 0x%08x  0x%08x  0x%08x  0x%08x\n", INREG32(DISPSYS_DSI0_BASE + 0x200), INREG32(DISPSYS_DSI0_BASE + 0x200 + 0x4), INREG32(DISPSYS_DSI0_BASE + 0x200 + 0x8), INREG32(DISPSYS_DSI0_BASE + 0x200 + 0xc));
	}

	if (module == DISP_MODULE_DSI1 || module == DISP_MODULE_DSIDUAL) {
		DDPMSG("=DISP DSI1 REGS=\n");
		for (i = 0; i < 20*16; i += 16) {
			DDPMSG("DSI1+%04x: 0x%08x  0x%08x  0x%08x  0x%08x\n", i, INREG32(DISPSYS_DSI1_BASE + i), INREG32(DISPSYS_DSI1_BASE + i + 0x4), INREG32(DISPSYS_DSI1_BASE + i + 0x8), INREG32(DISPSYS_DSI1_BASE + i + 0xc));
		}
		DDPMSG("DSI1 CMDQ+0x200: 0x%08x  0x%08x  0x%08x  0x%08x\n", INREG32(DISPSYS_DSI1_BASE + 0x200), INREG32(DISPSYS_DSI1_BASE + 0x200 + 0x4), INREG32(DISPSYS_DSI1_BASE + 0x200 + 0x8), INREG32(DISPSYS_DSI1_BASE + 0x200 + 0xc));
	}

	return ;
}

static void dpi_dump_analysis(void)
{
	DDPMSG("=DISP DPI ANALYSIS=\n");
	DDPMSG("clock=0x%x\n", DISP_REG_GET(DISP_REG_CLK_CFG_6_DPI));
#if 0
	if ((DISP_REG_GET(DISP_REG_VENCPLL_CON0)>>7)&0x1) {
		DDPMSG("DPI clock abnormal!!\n");
	}
#endif
	DDPMSG("clock_clear=%d\n", (DISP_REG_GET(DISP_REG_CLK_CFG_6_CLR)>>7)&0x1);
	return;
}


int ddp_dump_reg(DISP_MODULE_ENUM module)
{
	switch (module) {
		case DISP_MODULE_WDMA0:
		case DISP_MODULE_WDMA1:
			//WDMADump(module);
			break;
		case DISP_MODULE_RDMA0:
		case DISP_MODULE_RDMA1:
		case DISP_MODULE_RDMA2:
			RDMADump(module);
			break;
		case DISP_MODULE_OVL0:
		case DISP_MODULE_OVL1:
		case DISP_MODULE_OVL0_2L:
		case DISP_MODULE_OVL1_2L:
			OVLDump(module);
			break;
		case DISP_MODULE_GAMMA:
			gamma_dump_reg();
			break;
		case DISP_MODULE_CONFIG:
			mmsys_config_dump_reg();
			break;
		case DISP_MODULE_MUTEX:
			mutex_dump_reg();
			break;
		case DISP_MODULE_MERGE:
			merge_dump_reg();
			break;
		case DISP_MODULE_SPLIT0:
		case DISP_MODULE_SPLIT1:
			split_dump_reg(module);
			break;
		case DISP_MODULE_COLOR0:
		case DISP_MODULE_COLOR1:
			color_dump_reg(module);
			break;
		case DISP_MODULE_AAL:
			aal_dump_reg();
			break;
		case DISP_MODULE_PWM0:
		case DISP_MODULE_PWM1:
			pwm_dump_reg(module);
			break;
		case DISP_MODULE_UFOE:
			ufoe_dump_reg();
			break;
		case DISP_MODULE_OD:
			od_dump_reg();
			break;
		case DISP_MODULE_DSI0:
		case DISP_MODULE_DSI1:
		case DISP_MODULE_DSIDUAL:
			dsi_dump_reg(module);
			break;
		case DISP_MODULE_DPI:
			break;
		case DISP_MODULE_CCORR:
			ccorr_dump_reg();
			break;
		case DISP_MODULE_DITHER:
			dither_dump_reg();
			break;
		default:
			DDPMSG("DDP error, dump_reg unknown module=%d\n", module);
	}
	return 0;
}

int ddp_dump_analysis(DISP_MODULE_ENUM module)
{
	switch (module) {
		case DISP_MODULE_WDMA0:
		case DISP_MODULE_WDMA1:
			//wdma_dump_analysis(module);
			break;
		case DISP_MODULE_RDMA0:
		case DISP_MODULE_RDMA1:
		case DISP_MODULE_RDMA2:
			rdma_dump_analysis(module);
			break;
		case DISP_MODULE_OVL0:
		case DISP_MODULE_OVL1:
		case DISP_MODULE_OVL0_2L:
		case DISP_MODULE_OVL1_2L:
			ovl_dump_analysis(module);
			break;
		case DISP_MODULE_GAMMA:
			gamma_dump_analysis();
			break;
		case DISP_MODULE_CONFIG:
			mmsys_config_dump_analysis();
			break;
		case DISP_MODULE_MUTEX:
			mutex_dump_analysis();
			break;
		case DISP_MODULE_MERGE:
			merge_dump_analysis();
			break;
		case DISP_MODULE_SPLIT0:
		case DISP_MODULE_SPLIT1:
			split_dump_analysis(module);
			break;
		case DISP_MODULE_COLOR0:
		case DISP_MODULE_COLOR1:
			color_dump_analysis(module);
			break;
		case DISP_MODULE_AAL:
			aal_dump_analysis();
			break;
		case DISP_MODULE_UFOE:
			ufoe_dump_analysis();
			break;
		case DISP_MODULE_OD:
			od_dump_analysis();
			break;
		case DISP_MODULE_PWM0:
		case DISP_MODULE_PWM1:
			pwm_dump_analysis(module);
			break;
		case DISP_MODULE_DSI0:
		case DISP_MODULE_DSI1:
		case DISP_MODULE_DSIDUAL:
			break;
		case DISP_MODULE_DPI:
			dpi_dump_analysis();
			break;
		case DISP_MODULE_CCORR:
			ccorr_dump_analyze();
			break;
		case DISP_MODULE_DITHER:
			dither_dump_analyze();
			break;
		default:
			DDPMSG("DDP error, dump_analysis unknown module=%d\n", module);
	}
	return 0;
}
