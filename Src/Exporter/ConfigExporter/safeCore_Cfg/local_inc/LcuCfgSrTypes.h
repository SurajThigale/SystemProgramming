/** \addtogroup LcuCfgSr
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgSrTypes.h
 *  @brief     Data type that holds all the LCU's CfgSr module configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef LCUCFGSRTYPES_H_
#define LCUCFGSRTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneralFubInternalTypes.h"

/*
 * MACRO DEFINITIONS
 */
/** Number of SRs on LCU */
#define NUMBER_OF_LCU_SSR_ON_SN      19U          // As per Gen1.0
/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of LCU CfgSr module.
 * @requirements    SC_REQ-218596
 */
typedef struct { /*lint !e9045 */
    int32_t lLCUAPS_MaxTravelDistanceWithCalculatedPosition;
    int32_t lLCUAPS_MaxTravelTimeWithCalculatedPosition;
    int32_t lLCUAPS_MaxTravelTimeWithCalculatedVelocity;
    int32_t lLCUAPS_MaxTravelTimeWithInjectedVelocity;
    int16_t lLCUAPS_MinSpeedWithExtrapolation;
    int32_t lLCUBRK_WearoutLevelWarn;
    int32_t lLCUBRK_WearoutLevelError;
    int32_t lLCUBRK_FrictionLevelWarn;
    int32_t lLCUBRK_FrictionLevelError;
    int32_t lLCUBRK_StepForceDelta;
    int32_t lLCUBRK_MaxRedundantBrakeFailureTime;
    int32_t lLCUBRK_MaxUpwardBrakeOpCount;
    int32_t lLCUBRK_MaxPrimaryUBPLockingTime;
    int32_t lLCUBRK_MaxRedundantUBPLockingTime;
    int32_t lLCUBRK_MaxPrimaryUBPUnlockingTime;
    int32_t lLCUBRK_MaxRedundantUBPUnlockingTime;
    int32_t lLCUBSS_MaxBrakeSafetySensorFailureTime;
    int32_t lLCUBSS_MaxBrakeReleaseTimeWarn;
    int32_t lLCUBSS_MaxBrakeReleaseTimeError;
    int32_t lLCUBSS_MaxBrakeActuationTimeWarn;
    int32_t lLCUBSS_MaxBrakeActuationTimeError;
    int32_t lLCUBSS_dXtargetTolerance;
    int32_t lLCUBSS_dXcontactTolerance;
    int32_t lLCUBSS_PcontactMin;
    int32_t lLCUBSS_PcontactMax;
    int32_t lLCUCBSS_CabinLockingMaxBlockingTime;
    int32_t lLCUCBSS_CabinLockingMaxUnblockingTime;
    int32_t lLCUCLSS_MaxLCUCabinLockingTime;
    int32_t lLCUCLSS_MaxLCUCabinUnlockingTime;
    int32_t lLCUCLSS_CabinLockingStatusUndefinedMaxTime;
    int32_t lLCUCTS_DiffOverwriteErrorTemp;
    int32_t lLCUCTS_DiffOverwriteWarnTemp;
    int32_t lLCUCTS_MaxDiffValue;
    int32_t lLCUCTS_MaxDiffTime;
    int32_t lLCUCTS_MaxZeroTime;
    int32_t lLCUCTS_ZeroOverwriteErrorTemp;
    int32_t lLCUCTS_ZeroOverwriteWarnTemp;
    int32_t lLCULW_MaxDiffValue;
    int32_t lLCULW_MinValue;
    int32_t lLCULW_MaxValue;
    int32_t lLCULW_CrushLoadValue;
    int32_t lLCUSN_MaxTemperature;
    int32_t lLCUSN_PrimaryBrakeActiveWithoutCommandMaxTime;
    int32_t lLCUSN_ReactionModuleSignalDelayTime;
    int32_t lSCTS_MaxErrorTime;
    int32_t lSCTS_ErrorOverwriteErrorTemp;
    int32_t lSCTS_ErrorOverwriteWarnTemp;
    uint16_t usLCUSN_Ssr_Enable[NUMBER_OF_LCU_SSR_ON_SN];

} lcuCfgSr_t;


#endif /* LCUCFGSRTYPES_H_ */

/** @}  End of this module */
