/** \addtogroup IocCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      IocCfg_Globals.h
 *  @brief     Type that holds all global data of IOC Cfg FUB.
 *  @author    Frank Meier
 *
 */

#ifndef IOC_CFG_GLOBALS_H_
#define IOC_CFG_GLOBALS_H_

/*
 * INCLUDEFILES
 */

#include "IocCfgGeneral_Globals.h"
#include "IocCfgGeneralTypes.h"
#include "CfgSfSn.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * Static global data used by IOC.
 *
 * This data remains valid across evaluation cycles. It is attached
 * to the global static data as defined in the FUB shell.
 *
 * @requirements    SC_REQ-221219
 */
typedef struct {

    iocCfg_t tCfg;

    /** IOC CfgGeneral module static data */
    iocCfgGeneral_Globals_t tCfgGeneralGlobals;

    /* At startup, those two variables are set by persisted data.
     * Config-updates during run-time set (or clear) those values */
    enCfgValidity_t tConfigValid;
    uint32_t ulConfigCrc;

    safetyFunctionId_t tSfsOnSn[NUMBER_OF_SFS_ON_SN];

} CfgGlobals_t;

#endif /* IOC_CFG_GLOBALS_H_ */

/** @}  End of this module */
