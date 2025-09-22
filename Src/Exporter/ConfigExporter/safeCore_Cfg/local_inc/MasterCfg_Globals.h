/** \addtogroup MasterCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfg_Globals.h
 *  @brief     Type that holds all global data of Master Cfg FUB.
 *  @author    Frank Meier
 *
 */

#ifndef MASTER_CFG_GLOBALS_H_
#define MASTER_CFG_GLOBALS_H_

/*
 * INCLUDEFILES
 */
#include "MasterCfgGeneral_Globals.h"
#include "MasterCfgSf_Globals.h"
#include "MasterCfgGeneralTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * Static global data used by Master CFG FUB.
 *
 * This data remains valid across evaluation cycles. It is attached
 * to the global static data as defined in the FUB shell.
 * @requirements    SC_REQ-218662
 */
typedef struct {
    /** Master CfgGeneral module static data */
    masterCfgGeneral_Globals_t tCfgGeneralGlobals;
    /** Master CfgSf module static data */
    masterCfgSf_Globals_t tMasterCfgSf;

    /* At startup, those two variables are set by persisted data.
     * Config-updates during run-time set (or clear) those values */
    enCfgValidity_t tConfigValid;
    uint32_t ulConfigCrc;

    /**
     * @brief           Master configuration data.
     *
     */
    masterCfg_t tCfg;
} CfgGlobals_t;

#endif /* MASTER_CFG_GLOBALS_H_ */

/** @}  End of this module */
