/** \defgroup GenericCfgWell GenericCfgWell SW Module Specification
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgWell.h
 *  @brief     Provides access to Safety Node specific CfgWell configuration parameter data.
 *  @author    Frank Meier
 *
 */

#ifndef CFGWELL_H_
#define CFGWELL_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include <stdint.h>
#include "CfgTypes.h"

#define WTSS_GLOBAL_IDX_BOTTOM  0u
#define WTSS_GLOBAL_IDX_TOP     1u

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
CfgWell_initialize(void);

extern int32_t
CfgWell_get(enCfgParam_t tParam, uint32_t ulIndex);

#endif /* CFGWELL_H_ */

/** @}  End of this module */
