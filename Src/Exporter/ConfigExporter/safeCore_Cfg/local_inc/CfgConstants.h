/** \defgroup GenericCfgConstants GenericCfgConstants SW Module Specification
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgConstants.h
 *  @brief     Provides access to Safety Node specific CfgConstants configuration parameter data.
 *  @author    Frank Meier
 *
 */

#ifndef CFGCONSTANTS_H_
#define CFGCONSTANTS_H_

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
CfgConstants_initialize(void);

extern int32_t
CfgConstants_get(enCfgParam_t tParam, uint32_t ulIndex);

#endif /* CFGCONSTANTS_H_ */

/** @}  End of this module */
