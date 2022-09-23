/*  SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2022 MediaTek Inc.
 */

#ifndef _CONNV3_H_
#define _CONNV3_H_

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/

/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/
enum connv3_drv_type {
	CONNV3_DRV_TYPE_BT = 0,
	CONNV3_DRV_TYPE_WIFI = 1,
	CONNV3_DRV_TYPE_MODEM = 2,
	CONNV3_DRV_TYPE_MAX
};

#define CONNV3_CB_RET_CAL_PASS_POWER_OFF 0x0
#define CONNV3_CB_RET_CAL_PASS_POWER_ON  0x2
#define CONNV3_CB_RET_CAL_FAIL_POWER_OFF 0x1
#define CONNV3_CB_RET_CAL_FAIL_POWER_ON  0x3

/* bus hang error define */
#define CONNINFRA_INFRA_BUS_HANG			0x1
#define CONNINFRA_AP2CONN_RX_SLP_PROT_ERR	0x2
#define CONNINFRA_AP2CONN_TX_SLP_PROT_ERR	0x4
#define CONNINFRA_AP2CONN_CLK_ERR			0x8
#define CONNINFRA_INFRA_BUS_HANG_IRQ		0x10

#define CONNV3_ERR_RST_ONGOING			-0x7788
#define CONNINFRA_ERR_WAKEUP_FAIL			-0x5566

#define CONNINFRA_POWER_ON_D_DIE_FAIL	-0x1111
#define CONNINFRA_POWER_ON_A_DIE_FAIL	-0x2222
#define CONNINFRA_POWER_ON_CONFIG_FAIL	-0x3333

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/

/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/

/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/


/* power on/off */
int connv3_pwr_on(enum connv3_drv_type drv_type);
int connv3_pwr_on_done(enum connv3_drv_type drv_type);
int connv3_pwr_off(enum connv3_drv_type drv_type);


/* chip reset
 * return:
 *    <0: error
 *    =0: triggered
 *    =1: ongoing
 */
int connv3_trigger_whole_chip_rst(enum connv3_drv_type drv, char *reason);

/* whole chip reset callback
 * return:
 *    =0: success
 *    !0: fail
 */
struct connv3_whole_chip_rst_cb {
	int (*pre_whole_chip_rst)(enum connv3_drv_type drv, char *reason);
	int (*post_whole_chip_rst)(void);
};


/* PMIC state */
void connv3_update_pmic_state(enum connv3_drv_type drv);

/* subsys callback register */
struct connv3_pre_calibration_cb {
	int (*pwr_on_cb)(void);
	int (*do_cal_cb)(void);
};

struct connv3_power_on_cb {
	int (*pre_power_on)(void);
	int (*power_on_notify)(void);
};

struct connv3_sub_drv_ops_cb {
	/* power on */
	struct connv3_power_on_cb pwr_on_cb;
	/* chip reset */
	struct connv3_whole_chip_rst_cb rst_cb;

	/* calibration */
	struct connv3_pre_calibration_cb pre_cal_cb;
};

int connv3_sub_drv_ops_register(enum connv3_drv_type drv_type, struct connv3_sub_drv_ops_cb *cb);
int connv3_sub_drv_ops_unregister(enum connv3_drv_type drv_type);

/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/

#endif /* _CONNINFRA_H_ */
