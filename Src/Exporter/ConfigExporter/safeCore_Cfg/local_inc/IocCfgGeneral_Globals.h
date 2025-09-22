/** \addtogroup IocCfgGeneral
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      IocCfgGeneral_Globals.h
 *  @brief     Type that holds all global data of IOC CfgGeneral module.
 *  @author    Frank Meier, Marius Groeger
 *
 */

#ifndef IOC_CFGGENERAL_GLOBALS_H_
#define IOC_CFGGENERAL_GLOBALS_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include "CfgGeneral.h"

/*
 * TYPE DEFINITIONS
 */

/** IOC CfgGeneral module static data. */
typedef struct {

    /** Flag that indicates if a job specific configuration is loaded. */
    bool bJobSpecificConfigurationLoaded;

    /** Internal state of CfgGeneral_loadConfiguration */
    enCfgIoState_t tCfgIoState;

} iocCfgGeneral_Globals_t;

#endif /* IOC_CFGGENERAL_GLOBALS_H_ */

/** @}  End of this module */
