/** \addtogroup XcCfgSr
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      XcCfgSrTypes.h
 *  @brief     Data type that holds all the XC's CfgSr module configuration data.
 *  @author    Frank Meier, Zijie Wang
 *  @reference Exchanger CFG Fub Functional Block Specification
 *
 */

#ifndef XC_CFGSR_TYPES_H_
#define XC_CFGSR_TYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/** Number of SRs on LCU */
#define NUMBER_OF_XC_SSR_ON_SN      20U          // As per Gen1.0


/**
 * @brief        Configuration data of XC CfgSr module.
 * @requirements SC_REQ-220133 */
typedef struct { /*lint !e9045 */
    int32_t lSCTS_MaxErrorTime;
    int32_t lSCTS_ErrorOverwriteErrorTemp;
    int32_t lSCTS_ErrorOverwriteWarnTemp;

    int32_t lTCBSS_MaxBlockActuationTime;
    int32_t lTCBSS_MaxUnblockActuationTime;

    int32_t lTCLSS_MaxCabinLockingTime;
    int32_t lTCLSS_LockingStatusUndefinedMaxTime;
    int32_t lTCLSS_MaxCabinUnlockingTime;
    int32_t lTCLSS_MaxUndefinedTime;

    int32_t lXCL_MaxLockActuationTime;
    int32_t lXCL_MaxUnlockActuationTime;
    int32_t lXCL_MaxBlockActuationTime;
    int32_t lXCL_MaxUnblockActuationTime;
    int32_t lXCL_MaxUndefinedTime;

    int32_t lXCGBSS_MaxMaintenanceTime;
    int32_t lXCGBSS_MaxAllowedReleaseTime;
    int32_t lXCGBSS_MaxAllowedActivationTime;

    int32_t lXCSARE_MaxFailureTime;

    int32_t lXCSN_WarnTemp;

    int32_t lXCISTO_ActuationTimeDuration;
    int32_t lXCISTO_DeActuationTimeDuration;
    uint16_t  usXCSN_Ssr_Enable[NUMBER_OF_XC_SSR_ON_SN];
} xcCfgSr_t;

#endif /* XC_CFGSR_TYPES_H_ */
/** @}  End of this module */
