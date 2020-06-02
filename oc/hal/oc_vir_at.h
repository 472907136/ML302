/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */
#ifndef __OC_VIR_AT_H__
#define __OC_VIR_AT_H__
#include "oc_def.h"


int oc_vir_at_send_cmd(oc_uint8_t *at_str);
char * oc_vir_at_check_str(const char *str);
void oc_vir_at_dump(void);
#endif // __OC_VIR_AT_H__


