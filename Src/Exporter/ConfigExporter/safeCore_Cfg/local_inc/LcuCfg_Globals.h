/** \addtogroup LcuCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfg_Globals.h
 *  @brief     Type that holds all global data of LCU Cfg FUB.
 *  @author    Frank Meier
 *
 */

#ifndef LCU_CFG_GLOBALS_H_
#define LCU_CFG_GLOBALS_H_

/*
 * INCLUDEFILES
 */

#include "LcuCfgGeneral_Globals.h"
#include "LcuCfgSf_Globals.h"
#include "LcuCfgGeneralTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * Static global data used by LCU.
 *
 * This data remains valid across evaluation cycles. It is attached
 * to the global static data as defined in the FUB shell.
 *
 * @requirements    SC_REQ-218611
 */
typedef struct {
    /** LCU CfgGeneral module static data */
    lcuCfgGeneral_Globals_t tCfgGeneralGlobals;
    /** LCU CfgSf module static data */
    lcuCfgSf_Globals_t tLcuCfgSf;

    /* At startup, those two variables are set by persisted data.
     * Config-updates during run-time set (or clear) those values */
    enCfgValidity_t tConfigValid;
    uint32_t ulConfigCrc;

    /**
     * @brief LCU configuration data.
     */
    lcuCfg_t tCfg;
} CfgGlobals_t;

#endif /* LCU_CFG_GLOBALS_H_ */

/** @}  End of this module */
