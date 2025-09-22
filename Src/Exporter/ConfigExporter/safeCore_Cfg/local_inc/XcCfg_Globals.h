/** \addtogroup XcCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      XcCfg_Globals.h
 *  @brief     Type that holds all global data of XC Cfg FUB.
 *  @author    Frank Meier, Zijie Wang
 *  @reference Exchanger CFG Fub Functional Block Specification
 */

#ifndef XC_CFG_GLOBALS_H_
#define XC_CFG_GLOBALS_H_

/*
 * INCLUDEFILES
 */
#include "XcCfgGeneral_Globals.h"
#include "XcCfgGeneralTypes.h"
#include "XcCfgSfTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief  Static global data used by XC.
 *
 * @requirements SC_REQ-220871
 *
 * @remarks This data remains valid across evaluation cycles. It is attached
 * to the global static data as defined in the FUB shell.
 */
typedef struct {
    /** XC CFG FUB configuration data. */
    xcCfg_t tCfg;

    /** XC CfgSf module static data */
    xcCfgSf_t tXcCfgSf;

    /** XC CfgGeneral module static data */
    XcCfgGeneral_Globals_t tCfgGeneralGlobals;

    /* At startup, those two variables are set by persisted data.
     * Config-updates during run-time set (or clear) those values */
    enCfgValidity_t tConfigValid;
    uint32_t ulConfigCrc;

} CfgGlobals_t;

#endif /* XC_CFG_GLOBALS_H_ */
/** @}  End of this module */
