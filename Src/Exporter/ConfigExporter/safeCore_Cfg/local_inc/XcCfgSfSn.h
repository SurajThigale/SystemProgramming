/** \addtogroup XcCfgSf
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgSfSn.h
 *  @brief     XC specific CfgSf definitions.
 *  @author    Zijie Wang
 *  @reference Exchanger CFG Fub Functional Block Specification
 *
 */

#ifndef CFGSFSN_XC_H_
#define CFGSFSN_XC_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include <stdint.h>

/*
 * MACRO DEFINITIONS
 */

/**
 *  @brief        Number of Safety Functions on XC
 *  @requirements SC_REQ-220136
 *                SC_REQ-219277
 */
#define NUMBER_OF_SFS_ON_SN  7U

/*
 * TYPE DEFINITIONS
 */

/**
 *  @brief        Safety Functions running on XC
 *  @requirements SC_REQ-220137
 *                SC_REQ-219278
 */
typedef enum {
    SnSf_SWIVELPLATFORM_005,
    SnSf_SWIVELPLATFORM_010,
    //SnSf_SWIVELPLATFORM_014,
    //SnSf_SWIVELPLATFORM_015,
    SnSf_SWIVELPLATFORM_016,
    SnSf_SWIVELPLATFORM_019,
    SnSf_SWIVELPLATFORM_023,
    SnSf_XC_MULTI_007_SwivelPlatform,
    //SnSf_XC_MULTI_007_ShaftEquipment,
    SnSf_XC_MULTI_008_SwivelPlatform,
    //SnSf_XC_MULTI_008_ShaftEquipment,
    SnSf_XC_MULTI_009_SwivelPlatform,
    //SnSf_XC_MULTI_009_ShaftEquipment,
} enSnSf_t;

#endif /* CFGSFSN_XC_H_ */
/** @}  End of this module */
