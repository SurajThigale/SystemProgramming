/** \addtogroup IocCfgSr
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      IocCfgSrTypes.h
 *  @brief     Data type that holds all the IOC's CfgSr module configuration data.
 *  @author    Alexander Doehrmann
 *  @reference IOCollector CFG Fub Functional Block Specification
 *
 */

#ifndef IOC_CFGSR_TYPES_H_
#define IOC_CFGSR_TYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgSr.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * MACRO DEFINITIONS
 */
/** Number of SRs on IOC */
#define NUMBER_OF_IOC_SSR_ON_SN      1U          // As per Gen1.0

/*
 * TYPE DEFINITIONS
 */

/**
 *  @brief        Configuration data of IOC CfgSr module.
 *
 *  @requirements SC_REQ-220642
 */
typedef struct { /*lint !e9045 */
    int32_t lSCTS_MaxErrorTime;
    int32_t lUDSS_MaxAllowedUnderpowerTime;
    int32_t lWTSS_MaxInvalidTime;
    int32_t lWTSS_OverwriteWarnTemp;
    int32_t lWTSS_OverwriteErrTemp;
    int32_t lWTSS_MaxDiffValue;
    uint16_t usIOCSN_Ssr_Enable[NUMBER_OF_IOC_SSR_ON_SN];
} iocCfgSr_t;

#endif /* IOC_CFGSR_TYPES_H_ */
/** @}  End of this module */
