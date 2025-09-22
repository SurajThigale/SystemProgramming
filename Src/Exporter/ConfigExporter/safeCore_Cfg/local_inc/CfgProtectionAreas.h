/** \defgroup GenericCfgProtectionAreas GenericCfgProtectionAreas SW Module Specification
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgProtectionAreas.h
 *  @brief     Provides access to Safety Node specific CfgProtectionAreas configuration
 *             parameter data.
 *  @author    Frank Meier
 *
 */

#ifndef CFGPROTECTIONAREAS_H_
#define CFGPROTECTIONAREAS_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include <stdint.h>
#include "CfgTypes.h"
#include "UtilBaseTypes.h"

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
CfgProtectionAreas_initialize(void);

extern int32_t
CfgProtectionAreas_get(enCfgParam_t tParam, uint32_t ulIndex);

extern const protectionArea_t*
CfgProtectionAreas_getPA(enProtectionAreaType_t tType, uint32_t ulIndex);

#endif /* CFGPROTECTIONAREAS_H_ */

/** @}  End of this module */
