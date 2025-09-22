/** \addtogroup XcCfgGeneral
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      XcCfg_Globals.h
 *  @brief     Type that holds all global data of XC CfgGeneral module.
 *  @author    Frank Meier, Zijie Wang, Marius Groeger
 *  @reference Exchanger CFG Fub Functional Block Specification
 */

#ifndef XC_CFGGENERAL_GLOBALS_H_
#define XC_CFGGENERAL_GLOBALS_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include "CfgGeneral.h"

/*
 * TYPE DEFINITIONS
 */

/** XC CfgGeneral module static data */
typedef struct {
    /** Flag that indicates if a job specific configuration is loaded. */
    bool bJobSpecificConfigurationLoaded;

    /** Internal state of CfgGeneral_loadConfiguration */
    enCfgIoState_t tCfgIoState;

    /** Intermediate data for CfgGeneral_loadConfiguration. */
    uint32_t ulCrc;

} XcCfgGeneral_Globals_t;

#endif /* XC_CFGGENERAL_GLOBALS_H_ */
/** @}  End of this module */
