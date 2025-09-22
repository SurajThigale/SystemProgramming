/** \defgroup GenericCfgSf GenericCfgSf SW Module Specification
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgSf.h
 *  @brief     Provides access to Safety Node specific CfgSf configuration data.
 *  @author    Frank Meier
 */

#ifndef CFGSF_H_
#define CFGSF_H_

/*
 * INCLUDEFILES
 */
#include "UtilBase.h"
#include "UtilOpMode.h"
#include "CfgSfSn.h"

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
CfgSf_initialize(void);

/* all SFs running on a SN */
extern const safetyFunctionId_t*
CfgSf_getSfs(void);

extern bool
CfgSf_getSfEnabledState(tSfIndex_t tSfIndex, enOpMode_t tOpMode);

extern bool
CfgSf_doesSfRequireConfirmation(tSfIndex_t tSfIndex);

extern bool
CfgSf_maySfBeBypassed(tSfIndex_t tSfIndex);

extern int32_t CfgSf_get(enCfgParam_t tParam, uint32_t ulIndex);


#endif /* CFGSF_H_ */

/** @}  End of this module */
