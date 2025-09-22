/** \addtogroup IocCfgSf
 *  @{
 *
 *  @copyright (c) 2023: thyssenkrupp Technology Center India, all rights reserved
 *
 *  @file      CfgSfSn.h
 *  @brief     IOC specific CfgSf definitions.
 *  @author    Piyush Zade
 *  @reference Generic Utilities Fub Functional Block Specification\n
 *             IOC CFG Fub Functional Block Specification
 */

#ifndef CFGSFSN_IOC_H_
#define CFGSFSN_IOC_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>

/*
 * MACRO DEFINITIONS
 */
/**
 * @brief           Number of Safety Functions on IOC.
 * @requirements    SC_REQ-219277
 */
#define NUMBER_OF_SFS_ON_SN  2U

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Safety Functions running on IOC.
 * @requirements    SC_REQ-219278
 *                  SC_REQ-222282
 */
typedef enum
{
    SnSf_MULTI_007,
    SnSf_MULTI_008,

} enSnSf_t;

#endif /* CFGSFSN_IOC_H_ */

/** @}  End of this module */
