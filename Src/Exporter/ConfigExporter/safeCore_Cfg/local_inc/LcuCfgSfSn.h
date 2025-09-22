/** \addtogroup LcuCfgSf
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgSfSn.h
 *  @brief     LCU specific CfgSf definitions.
 *  @author    Frank Meier
 *  @reference Generic Utilities Fub Functional Block Specification\n
 *             LCU CFG Fub Functional Block Specification
 */

#ifndef CFGSFSN_LCU_H_
#define CFGSFSN_LCU_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>

/*
 * MACRO DEFINITIONS
 */
/**
 * @brief           Number of Safety Functions on LCU.
 * @requirements    SC_REQ-219279
 *                  SC_REQ-219277
 */
#define NUMBER_OF_SFS_ON_SN  15U

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Safety Functions running on LCU.
 * @requirements    SC_REQ-219280
 *                  SC_REQ-219278
 */
typedef enum {
    /* SnSf_LCU_012, Gen2.0 */
    /* SnSf_LCU_013, Gen2.0 */
    SnSf_LCU_021,
    SnSf_LCU_027,
    SnSf_LCU_032,
    SnSf_LCU_033,
    /* SnSf_LCU_034, Gen2.0 */
    SnSf_LCU_035,
    SnSf_LCU_036,
    SnSf_LCU_044,
    SnSf_LCU_045,
    /* SnSf_LCU_046, Gen2.0 */
    SnSf_LCU_049,
    /* SnSf_LCU_050, Gen2.0 */
    /* SnSf_LCU_051, Gen2.0 */
    /* SnSf_LCU_052, Gen2.0 */
    /* SnSf_LCU_059, Gen2.0 */

    SnSf_MULTI_007,
    SnSf_MULTI_008_translatory,
    SnSf_LCU_029, /* Gen2.0 */
    SnSf_MULTI_008_rotatory,
    SnSf_MULTI_009_translatory,
    SnSf_MULTI_009_rotatory,
} enSnSf_t;

#endif /* CFGSFSN_LCU_H_ */

/** @}  End of this module */
