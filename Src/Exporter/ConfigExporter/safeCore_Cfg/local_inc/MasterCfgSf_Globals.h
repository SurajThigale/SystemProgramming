/** \addtogroup MasterCfgSf
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgSf_Globals.h
 *  @brief     Data type that holds all the MasterArray's CfgSf module global data.
 *  @author    Frank Meier
 *
 */

#ifndef MASTER_CFGSF_GLOBALS_H_
#define MASTER_CFGSF_GLOBALS_H_

/*
 * INCLUDEFILES
 */
#include "CfgSfSn.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief       Master CfgSf static module data
 * @remarks     This data is static and actually constant, but due to the way
 *              static data is handled in the B&R system it must be dynamically
 *              initialized (see #CfgSf_initialize).
 */
typedef struct { /*lint !e9045 */
    /**
     * @brief       List of SFs on Master.
     *
     * @details     Order of entries is given by enSnSf_t.
     * @remarks     Access via #CfgSf_getSfs.
     */
    safetyFunctionId_t tSfsOnSn[NUMBER_OF_SFS_ON_SN];

    /**
     * @brief   List of supported operation modes per SF.
     *
     * @details
     * Stores the operation modes an SF is enabled in as a bit mask, where the bits are determined
     * from the operation mode (type enOpMode_t) by (1 << opMode).
     * Order of entries is given by enSnSf_t.
     *
     * @remarks Access via #CfgSf_getSfEnabledState.
     *  */

    /* No longer required since this data will now come from configuration
    uint32_t lOpModesPerSf[NUMBER_OF_SFS_ON_SN]; */
} masterCfgSf_Globals_t;

#endif /* MASTER_CFGSF_GLOBALS_H_ */

/** @}  End of this module */
