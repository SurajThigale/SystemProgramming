/** \defgroup GenericCfgSr GenericCfgSr SW Module Specification
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgSr.h
 *  @brief     CfgSr configuration specific definitions used by multiple Cfg FUBs.
 *  @author    Frank Meier
 */

#ifndef CFGSR_H_
#define CFGSR_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include <stdint.h>
#include "CfgTypes.h"

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
CfgSr_initialize(void);

extern int32_t
CfgSr_get(enCfgParam_t tParam, uint32_t ulIndex);

#endif /* LCUCFGSR_H_ */

/** @}  End of this module */
