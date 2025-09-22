/** \addtogroup XcCfgXc
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      XcCfgXcTypes.h
 *  @brief     Data type that holds all the XC's CfgXc module configuration data.
 *  @author    Frank Meier, Zijie Wang
 *  @reference Exchanger CFG Fub Functional Block Specification
 *
 */

#ifndef XC_CFGXC_TYPES_H_
#define XC_CFGXC_TYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgXc.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 *  @brief        Configuration data of XC CfgXc module.
 *  @requirements SC_REQ-220129 */
typedef struct { /*lint !e9045 */

    //tsprInfo_t tTsprInfos[XC_NUMBER_OF_TSPR_INFOS];
    int16_t sLockingAngleTolerance;
    int32_t lXcSPRotationThreshold;
    int32_t lXcClockwiseOuterEdgeAngle;
    int32_t lXcCounterclockwiseOuterEdgeAngle;
    int32_t lXcClockwiseClearanceAngle;
    int32_t lXcCounterclockwiseClearanceAngle;
    /* Scaled value, unit is 1. The SARE integration time is this value multiplies 50ms */
    int32_t lXcSAREVelocityIntegrationTime;
    int32_t lXcSARE_MultiturnInitialValue;
    int32_t lXcSARE_ZeroDegreePosition;
    int32_t lXcSARE_NinetyDegreePosition;
    int32_t lXcAngularSpeedMax;
    int32_t lXcAngularSpeedInManualOpModeMax;
    int32_t lXC_MAR_BoundaryClockwise;
    int32_t lXC_MAR_BoundaryCounterclockwise;
    xcSD_ParamSet_t tXC_SD_ParamSet[2];   

} xcCfgXc_t;

#endif /* XC_CFGXC_TYPES_H_ */
/** @}  End of this module */
