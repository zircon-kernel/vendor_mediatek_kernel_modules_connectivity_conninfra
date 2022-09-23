// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */

#define pr_fmt(fmt) KBUILD_MODNAME "@(%s:%d) " fmt, __func__, __LINE__

#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fb.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include "connv3.h"
#include "connv3_core.h"
#include <linux/ratelimit.h>

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/


/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
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

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
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

#define CONNINFRA_RST_RATE_LIMIT 0

#if CONNINFRA_RST_RATE_LIMIT
DEFINE_RATELIMIT_STATE(g_rs, HZ, 1);

#define DUMP_LOG() if (__ratelimit(&g_rs)) \
			pr_info("rst is ongoing")

#else
#define DUMP_LOG()
#endif

int connv3_pwr_on(enum connv3_drv_type drv_type)
{
	pr_info("[%s] drv=[%d]", __func__, drv_type);
	if (connv3_core_is_rst_locking()) {
		DUMP_LOG();
		return CONNV3_ERR_RST_ONGOING;
	}

#if ENABLE_PRE_CAL_BLOCKING_CHECK
	connv3_core_pre_cal_blocking();
#endif

	return connv3_core_power_on(drv_type);
}
EXPORT_SYMBOL(connv3_pwr_on);

int connv3_pwr_on_done(enum connv3_drv_type drv_type)
{
	pr_info("[%s] drv=[%d]", __func__, drv_type);
	if (connv3_core_is_rst_locking()) {
		DUMP_LOG();
		return CONNV3_ERR_RST_ONGOING;
	}

#if ENABLE_PRE_CAL_BLOCKING_CHECK
	connv3_core_pre_cal_blocking();
#endif

	return connv3_core_power_on_done(drv_type);
}
EXPORT_SYMBOL(connv3_pwr_on_done);


int connv3_pwr_off(enum connv3_drv_type drv_type)
{
	pr_info("[%s] drv=[%d]", __func__, drv_type);
	if (connv3_core_is_rst_locking()) {
		DUMP_LOG();
		return CONNV3_ERR_RST_ONGOING;
	}

#if ENABLE_PRE_CAL_BLOCKING_CHECK
	connv3_core_pre_cal_blocking();
#endif

	return connv3_core_power_off(drv_type);
}
EXPORT_SYMBOL(connv3_pwr_off);

int connv3_trigger_whole_chip_rst(enum connv3_drv_type who, char *reason)
{
	/* use schedule worker to trigger ??? */
	/* so that function can be returned immediately */
	int r;

	r = connv3_core_lock_rst();
	if (r >= CHIP_RST_START) {
		/* reset is ongoing */
		pr_warn("[%s] r=[%d] chip rst is ongoing\n", __func__, r);
		return 1;
	}
	pr_info("[%s] rst lock [%d] [%d] reason=%s", __func__, r, who, reason);

	connv3_core_trg_chip_rst(who, reason);

	return 0;
}
EXPORT_SYMBOL(connv3_trigger_whole_chip_rst);

int connv3_sub_drv_ops_register(enum connv3_drv_type type, struct connv3_sub_drv_ops_cb *cb)
{
	/* type validation */
	if (type < 0 || type >= CONNV3_DRV_TYPE_MAX) {
		pr_err("[%s] incorrect drv type [%d]", __func__, type);
		return -EINVAL;
	}
	pr_info("[%s] ----", __func__);
	connv3_core_subsys_ops_reg(type, cb);
	return 0;
}
EXPORT_SYMBOL(connv3_sub_drv_ops_register);

int connv3_sub_drv_ops_unregister(enum connv3_drv_type type)
{
	/* type validation */
	if (type < 0 || type >= CONNV3_DRV_TYPE_MAX) {
		pr_err("[%s] incorrect drv type [%d]", __func__, type);
		return -EINVAL;
	}
	pr_info("[%s] ----", __func__);
	connv3_core_subsys_ops_unreg(type);
	return 0;
}
EXPORT_SYMBOL(connv3_sub_drv_ops_unregister);

