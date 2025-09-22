/** \addtogroup MasterCfgLcu
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgLcuTypes.h
 *  @brief     Data type that holds all the Master's CfgLcu module configuration data.
 *  @author    Frank Meier
 *  @reference MasterArray CFG Fub Functional Block Specification
 */

#ifndef MASTERCFGLCUTYPES_H_
#define MASTERCFGLCUTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgLimits.h"
#include "CfgLcu.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of Master CfgLcu module.
 * @requirements    SC_REQ-218638
 */
typedef struct { /*lint !e9045 */
    /** Physical constraints for each LCU type of enLcuType_t. */
    lcuPhysicalConstraints_t tPhysicalConstraints[2];

    stoppingDistanceParamSet_t tStoppingDistanceParamSet;

    int8_t cType[MULTI_NUMBER_OF_LCU_MAX]; /* enLcuType_t */

    int8_t cPositionTranslation_TrackIsVertical[MULTI_NUMBER_OF_TRACKS_MAX];
    int32_t lPositionTranslation_CenterAlignmentPosAlongTrack[MULTI_NUMBER_OF_TRACKS_MAX];
    int32_t lVelocityFES;
} masterCfgLcu_t;


#endif /* MASTERCFGLCUTYPES_H_ */

/** @}  End of this module */
