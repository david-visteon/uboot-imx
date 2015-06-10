#include <common.h>
#include <asm/io.h>
#include <asm/arch/mx6.h>
#include <asm/arch/mx6_pins.h>
#include <asm/arch/iomux-v3.h>

iomux_v3_cfg_t mx6q_uart_pads[] = {
	MX6Q_PAD_EIM_D24__UART3_TXD,            /* DO_TX_NMEA */
	MX6Q_PAD_EIM_D25__UART3_RXD,            /* DI_RX_NMEA */
	MX6Q_PAD_CSI0_DAT12__UART4_TXD,         /* DO_UART4_BT_TXD_4Mbps */
	MX6Q_PAD_CSI0_DAT13__UART4_RXD,         /* DI_UART4_BT_RXD_4Mbps */
	MX6Q_PAD_CSI0_DAT16__UART4_RTS,         /* DI_UART4_BT_RTS */
	MX6Q_PAD_CSI0_DAT17__UART4_CTS_DCE,  /* DO_UART4_BT_CTS */
	MX6Q_PAD_GPIO_7__UART2_TXD,             /* DO_TX_DEBUG */
	MX6Q_PAD_GPIO_8__UART2_RXD,             /* DI_RX_DEBUG */
};

iomux_v3_cfg_t mx6q_audio_pads[] = {
	MX6Q_PAD_DISP0_DAT20__AUDMUX_AUD4_TXC,   /* CLK_CODEC_BCLK_1536KHZ      */
	MX6Q_PAD_DISP0_DAT21__AUDMUX_AUD4_TXD,   /* DO_I2S_CODEC_DATOUT         */
	MX6Q_PAD_DISP0_DAT22__AUDMUX_AUD4_TXFS,  /* CLK_CODEC_WCLK_48KHZ        */
	MX6Q_PAD_DISP0_DAT23__AUDMUX_AUD4_RXD,   /* DI_I2S_CODEC_DATIN          */	
};

iomux_v3_cfg_t mx6q_pwm_pads[] = {
	MX6Q_PAD_DISP0_DAT8__PWM1_PWMO,         /* DO_PWM1_LVDS      */
	MX6Q_PAD_DISP0_DAT9__PWM2_PWMO,         /* DO_PWM2_LVDS      */
	MX6Q_PAD_SD4_DAT1__PWM3_PWMO,           /* DO_PWM3_LVDS_BL   */
	/* DO_PWM4_BKLT_DIM */
};

iomux_v3_cfg_t mx6q_usdhc2_pads[] = {
	MX6Q_PAD_SD2_CLK__USDHC2_CLK,		/* CLK_WLAN_50MHZ*/
	MX6Q_PAD_SD2_CMD__USDHC2_CMD,		/* DO_WLAN_CMD*/
	MX6Q_PAD_SD2_DAT0__USDHC2_DAT0,		/* DIO_WLAN_DAT0*/
	MX6Q_PAD_SD2_DAT1__USDHC2_DAT1,		/* DIO_WLAN_DAT1*/
	MX6Q_PAD_SD2_DAT2__USDHC2_DAT2,		/* DIO_WLAN_DAT2*/
	MX6Q_PAD_SD2_DAT3__USDHC2_DAT3,		/* DIO_WLAN_DAT3*/
};

iomux_v3_cfg_t mx6q_usdhc3_pads[] = {
	MX6Q_PAD_SD3_CLK__USDHC3_CLK,		/* DO_eMMC_CMD*/
	MX6Q_PAD_SD3_CMD__USDHC3_CMD,		/* DO_eMMC_CLK*/
	MX6Q_PAD_SD3_DAT0__USDHC3_DAT0,		/* DIO_eMMC_D0*/
	MX6Q_PAD_SD3_DAT1__USDHC3_DAT1,		/* DIO_eMMC_D1*/
	MX6Q_PAD_SD3_DAT2__USDHC3_DAT2,		/* DIO_eMMC_D2*/
	MX6Q_PAD_SD3_DAT3__USDHC3_DAT3,		/* DIO_eMMC_D3*/
	MX6Q_PAD_SD3_DAT4__USDHC3_DAT4,		/* DIO_eMMC_D4*/
	MX6Q_PAD_SD3_DAT5__USDHC3_DAT5,		/* DIO_eMMC_D5*/
	MX6Q_PAD_SD3_DAT6__USDHC3_DAT6,		/* DIO_eMMC_D6*/
	MX6Q_PAD_SD3_DAT7__USDHC3_DAT7,		/* DIO_eMMC_D7*/
	MX6Q_PAD_SD3_RST__USDHC3_RST,		/* DO_eMMC_RESET*/
};

iomux_v3_cfg_t i2c_pads[] = {
	MX6Q_PAD_EIM_D28__I2C1_SDA,		/* DIO_I2C1_SDA*/
	MX6Q_PAD_EIM_D21__I2C1_SCL,		/* DIO_I2C1_SCL_100KHz*/
	MX6Q_PAD_KEY_ROW3__I2C2_SDA,		/* DIO_I2C2_SDA*/
	MX6Q_PAD_KEY_COL3__I2C2_SCL,		/* DIO_I2C2_SCL_100KHz*/
	MX6Q_PAD_GPIO_5__I2C3_SCL,		/* DIO_I2C3_SCL_50KHz*/
	MX6Q_PAD_GPIO_16__I2C3_SDA,		/* DIO_I2C3_SDA*/
};

iomux_v3_cfg_t mx6q_usb_pads[] = {
	MX6Q_PAD_EIM_D30__USBOH3_USBH1_OC, 	/*DI_HOST1_OC*/
	MX6Q_PAD_EIM_D31__USBOH3_USBH1_PWR,	/*DO_HOST1_PWR*/
	/* DO_USB_H3_DATA */
	/* DI_PG_USB */
	/* DO_USB_H3_STROBE */
	/* DO_USB_HUBLOCPWR */
	MX6Q_PAD_KEY_COL4__USBOH3_USBOTG_OC,	/* DI_USBOTG_OC       */
	MX6Q_PAD_KEY_ROW4__USBOH3_USBOTG_PWR,	/* DO_USBOTG_PWR      */
	//MX6Q_PAD_KEY_ROW4__GPIO_4_15,		/* DO_USB_OTG_PWR as GPIO*/
	/* DO_HSIC_HUB_RESET */
};

iomux_v3_cfg_t mx6q_ipu_pads[] = {
	MX6Q_PAD_EIM_A24__IPU2_CSI1_D_19,	/* BUS_DVI_DAT15         */
	MX6Q_PAD_EIM_A23__IPU2_CSI1_D_18,	/* BUS_DVI_DAT14         */
	MX6Q_PAD_EIM_A22__IPU2_CSI1_D_17,	/* BUS_DVI_DAT13         */
	MX6Q_PAD_EIM_A21__IPU2_CSI1_D_16,	/* BUS_DVI_DAT12         */
	MX6Q_PAD_EIM_A20__IPU2_CSI1_D_15,	/* BUS_DVI_DAT11         */
	MX6Q_PAD_EIM_A19__IPU2_CSI1_D_14,	/* BUS_DVI_DAT10         */
	MX6Q_PAD_EIM_A18__IPU2_CSI1_D_13,	/* BUS_DVI_DAT9          */
	MX6Q_PAD_EIM_A17__IPU2_CSI1_D_12,	/* BUS_DVI_DAT8          */
	MX6Q_PAD_EIM_A16__IPU2_CSI1_PIXCLK,	/* CLK_DVI_PIXCLK_27MHZ  */
};

iomux_v3_cfg_t ecspi_pads[] = {
	MX6Q_PAD_EIM_CS0__ECSPI2_SCLK,
	MX6Q_PAD_EIM_CS1__ECSPI2_MOSI,
	MX6Q_PAD_EIM_OE__ECSPI2_MISO,
	MX6Q_PAD_EIM_RW__ECSPI2_SS0,		/*DO_VIP_CS#*/
};

iomux_v3_cfg_t wdi_pads[] = {
	MX6Q_PAD_EIM_D23__GPIO_3_23,           /* DO_WDI_DISABLE */
};

iomux_v3_cfg_t ccm_clko_pads[] = {
	MX6Q_PAD_CSI0_MCLK__CCM_CLKO,        /* CLK_WL 32.768KHz */
};


void iomux_init(void)
{
	mxc_iomux_v3_setup_multiple_pads( mx6q_uart_pads, ARRAY_SIZE( mx6q_uart_pads));	
	mxc_iomux_v3_setup_multiple_pads( mx6q_audio_pads, ARRAY_SIZE(mx6q_audio_pads ));
	//mxc_iomux_v3_setup_multiple_pads( mx6q_pwm_pads, ARRAY_SIZE( mx6q_pwm_pads));
	mxc_iomux_v3_setup_multiple_pads( mx6q_usdhc2_pads, ARRAY_SIZE( mx6q_usdhc2_pads));
	mxc_iomux_v3_setup_multiple_pads( mx6q_usdhc3_pads, ARRAY_SIZE( mx6q_usdhc3_pads));
	mxc_iomux_v3_setup_multiple_pads( i2c_pads, ARRAY_SIZE( i2c_pads));
	mxc_iomux_v3_setup_multiple_pads( mx6q_usb_pads, ARRAY_SIZE( mx6q_usb_pads));
	mxc_iomux_v3_setup_multiple_pads( mx6q_ipu_pads, ARRAY_SIZE( mx6q_ipu_pads));
	mxc_iomux_v3_setup_multiple_pads( ecspi_pads, ARRAY_SIZE( ecspi_pads));
	mxc_iomux_v3_setup_multiple_pads( wdi_pads, ARRAY_SIZE( wdi_pads));
	mxc_iomux_v3_setup_multiple_pads( ccm_clko_pads, ARRAY_SIZE( ccm_clko_pads));
}
