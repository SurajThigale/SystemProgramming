/** \addtogroup MasterCfgGeneral
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgGeneral_Globals.h
 *  @brief     Type that holds all global data of Master CfgGeneral module.
 *  @author    Frank Meier, Marius Groeger
 *
 */

#ifndef MASTER_CFGGENERAL_GLOBALS_H_
#define MASTER_CFGGENERAL_GLOBALS_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include "CfgGeneral.h"

/*
 * TYPE DEFINITIONS
 */

/** Master CfgGeneral module static data. */
typedef struct {
    /** Flag that indicates if a job specific configuration is loaded. */
    bool bJobSpecificConfigurationLoaded;

    /** Internal state of CfgGeneral_loadConfiguration */
    enCfgIoState_t tCfgIoState;

} masterCfgGeneral_Globals_t;

#endif /* MASTER_CFGGENERAL_GLOBALS_H_ */

/** @}  End of this module */
