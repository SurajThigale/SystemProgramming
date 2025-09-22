/** \addtogroup MasterCfgSf
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgSfSn.h
 *  @brief     Master specific CfgSf definitions.
 *  @author    Frank Meier
 *
 */

#ifndef CFGSFSN_MASTER_H_
#define CFGSFSN_MASTER_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include <stdint.h>

/*
 * MACRO DEFINITIONS
 */
/**
 * @brief           Number of Safety Functions on LCU.
 * @requirements    SC_REQ-219291
 *                  SC_REQ-219277
 */
#define NUMBER_OF_SFS_ON_SN  9U

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Safety Functions running on Master.
 * @requirements    SC_REQ-219292
 *                  SC_REQ-219278
 */
typedef enum {
    /* SnSf_LCU_001, Gen2.0 */
    /* SnSf_LCU_002, Gen2.0 */
    SnSf_LCU_004,
    SnSf_LCU_006,
    /* SnSf_LCU_008, Gen2.0 */
    /* SnSf_LCU_015, Gen2.0 */
    /* SnSf_LCU_016, Gen2.0 */
    /* SnSf_LCU_022, Gen2.0 */
    /* SnSf_LCU_023, Gen2.0 */
    /* SnSf_LCU_024, Gen2.0 */
    /* SnSf_LCU_026, Gen2.0 */
    SnSf_LCU_037,
    /* SnSf_LCU_038, Gen2.0 */
    SnSf_LCU_039,
    /* SnSf_LCU_042, Gen2.0 */
    /* SnSf_LCU_043, Gen2.0 */
    /* SnSf_LCU_047, Gen2.0 */
    /* SnSf_LCU_048, Gen2.0 */
    SnSf_LCU_053,
    /* SnSf_LCU_054, Gen2.0 */
    /* SnSf_LCU_056, Gen2.0 */
    /* SnSf_LCU_060, Gen2.0 */
    /* SnSf_LCU_061, Gen2.0 */

    /* SfType_FOC_ROT is currently not implemented */
    /* SnSf_ServiceLCU_009, */
    /* SnSf_ServiceLCU_013, */

    /* SnSf_SWIVELPLATFORM_001, Gen2.0 */
    /* SnSf_SWIVELPLATFORM_004, Gen2.0 */
    /* SnSf_SWIVELPLATFORM_005, Gen2.0 */
    /* SnSf_SWIVELPLATFORM_007, Gen2.0 */
    /* SnSf_SWIVELPLATFORM_010, Gen2.0 */
    /* SnSf_SWIVELPLATFORM_017, Gen2.0 */
    /* SnSf_SWIVELPLATFORM_020, Gen2.0 */
    /* SnSf_SWIVELPLATFORM_021, Gen2.0 */

    /* SnSf_SHAFTEQUIPMENT_001, Gen2.0 */
    /* SnSf_SHAFTEQUIPMENT_002, Gen2.0 */
    /* SnSf_SHAFTEQUIPMENT_004, Gen2.0 */
    /* SnSf_SHAFTEQUIPMENT_006, Gen2.0 */

    SnSf_MULTI_001,
    /* SnSf_MULTI_003, Gen2.0 */
    SnSf_MULTI_004,
    SnSf_MULTI_007,
    SnSf_MULTI_008,
    SnSf_LCU_025,
    SnSf_SWIVELPLATFORM_022,
} enSnSf_t;

#endif /* CFGSFSN_MASTER_H_ */

/** @}  End of this module */
