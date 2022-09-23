/*  SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2022 MediaTek Inc.
 */

#ifndef _CONN_ADAPTOR_H_
#define _CONN_ADAPTOR_H_

#include <linux/types.h>
#include <linux/mm.h>
#include "conn_drv.h"

enum conn_adaptor_drv_gen {
	CONN_ADAPTOR_DRV_GEN_CONNAC_2,
	CONN_ADAPTOR_DRV_GEN_CONNAC_3,
	CONN_ADAPTOR_DRV_GEN_SIZE
};

#define CONN_SUPPOPRT_DRV_WIFI_TYPE_BIT	(0x1 << CONN_ADAPTOR_DRV_WIFI)
#define CONN_SUPPOPRT_DRV_BT_TYPE_BIT	(0x1 << CONN_ADAPTOR_DRV_BT)
#define CONN_SUPPOPRT_DRV_GPS_TYPE_BIT	(0x1 << CONN_ADAPTOR_DRV_GPS)

struct conn_adaptor_drv_gen_cb {
	u32 drv_radio_support;

	u32 (*get_chip_id)(void);
	u32 (*get_adie_id)(void);

	/* suspend/resume */
	void (*plat_suspend_notify)(void);
	void (*plat_resume_notify) (void);

	/* on/off */
	void (*power_on_off_notify)(int on_off);

	/* coredump */
	void (*set_coredump_mode) (int mode);

	/* mmap for coredump */
	int (*coredump_mmap)(struct file *pFile, struct vm_area_struct *pVma);

	/* dbg read/write */
	int (*dump_power_state)(uint8_t *buf, u32 buf_sz);

};

int conn_adaptor_register_drv_gen(enum conn_adaptor_drv_gen drv_gen, struct conn_adaptor_drv_gen_cb* cb);
int conn_adaptor_unregister_drv_gen(enum conn_adaptor_drv_gen drv_gen);


#endif /* _CONN_ADAPTOR_H_ */