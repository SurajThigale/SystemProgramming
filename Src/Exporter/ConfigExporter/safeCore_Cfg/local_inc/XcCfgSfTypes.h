/** \addtogroup XcCfgSf
 *  @{
 *
 *  @copyright (c) 2021; infoteam SET GmbH, all rights reserved
 *
 *  @file      XcCfgSfTypes.h
 *  @brief     Data type that holds all the XC's CfgSf module configuration data.
 *  @author    Frank Meier, Zijie Wang
 *  @reference Exchanger CFG Fub Functional Block Specification
 *
 */

#ifndef XCCFGSFTYPES_H_
#define XCCFGSFTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgSfSn.h"

/*
 * TYPE DEFINITIONS
 */

/**
 *  Configuration data of XC CfgSf module.
 */
typedef struct { /*lint !e9045 */
    /** List of SFs on LCU.
     *
     * Access via CfgSf_getSfs.
     */
    safetyFunctionId_t tSfsOnSn[NUMBER_OF_SFS_ON_SN];

    /** Table (SF, supported op modes).
     *
     * Access via CfgSf_getSfEnabledState. */
    //uint32_t lOpModesPerSf[NUMBER_OF_SFS_ON_SN];
} xcCfgSf_t;

#endif /* XCCFGSFTYPES_H_ */
/** @}  End of this module */
