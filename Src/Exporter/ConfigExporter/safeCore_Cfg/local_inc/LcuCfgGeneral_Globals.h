/** \addtogroup LcuCfgGeneral
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgGeneral_Globals.h
 *  @brief     Type that holds all global data of LCU CfgGeneral module.
 *  @author    Frank Meier, Marius Groeger
 *
 */

#ifndef LCU_CFGGENERAL_GLOBALS_H_
#define LCU_CFGGENERAL_GLOBALS_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include "CfgGeneral.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief         LCU CfgGeneral module static data
 *
 * @requirements  SC_REQ-219034
 *
 */
typedef struct {

    /** Flag that indicates if a job specific configuration is loaded. */
    bool bJobSpecificConfigurationLoaded;

    /** Internal state of CfgGeneral_loadConfiguration */
    enCfgIoState_t tCfgIoState;

    /** Intermediate data for CfgGeneral_loadConfiguration. */
    uint32_t ulCrc;


} lcuCfgGeneral_Globals_t;

#endif /* LCU_CFGGENERAL_GLOBALS_H_ */

/** @}  End of this module */
