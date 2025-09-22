/** \addtogroup LcuCfgSf
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgSf_Globals.h
 *  @brief     Data type that holds all the LCU's CfgSf module global data.
 *  @author    Frank Meier
 *  @reference Generic Utilities Fub Functional Block Specification\n
 *             LCU CFG Fub Functional Block Specification
 */

#ifndef LCU_CFGSF_GLOBALS_H_
#define LCU_CFGSF_GLOBALS_H_

/*
 * INCLUDE FILES
 */
#include "CfgSfSn.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief       LCU CfgSf static module data.
 *
 * @requirements
 *
 * SC_REQ-222268 FBRQ - LcuCfgSf - global data
 */
typedef struct { /*lint !e9045 */
    /**
     * @brief       List of SFs on LCU.
     *
     * @details     Order of entries is given by enSnSf_t.
     * @remarks     Access via CfgSf_getSfs.
     */
    safetyFunctionId_t tSfsOnSn[NUMBER_OF_SFS_ON_SN];

    /**
     * @brief   List of supported operation modes per SF.
     *
     * @details
     * Stores the operation modes an SF is enabled in as a bit mask, where the bits are determined
     * from the operation mode (enOpMode_t) by (1 << opMode).
     * Order of entries is given by enSnSf_t.
     *
     * @remarks Access via CfgSf_getSfEnabledState.
     *  */
    /* No longer required since this data will now come from configuration
    uint32_t lOpModesPerSf[NUMBER_OF_SFS_ON_SN]; */
} lcuCfgSf_Globals_t;

#endif /* LCU_CFGSF_GLOBALS_H_ */

/** @}  End of this module */
