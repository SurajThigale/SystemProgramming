/** \addtogroup MasterCfgWell
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgWellTypes.h
 *  @brief     Data type that holds all the Master's CfgWell module configuration data.
 *  @author    Alexander Doehrmann
 *  @reference MasterArray CFG Fub Functional Block Specification
 *
 */

#ifndef MASTERCFGWELLTYPES_H_
#define MASTERCFGWELLTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneralFubInternalTypes.h"
#include "CfgWell.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           cfg data needed for the CfgWell module
 * @requirements    SC_REQ-221329 - FBRQ - CfgWell data structure
 */
typedef struct { /*lint !e9045 */
    uint16_t usWellAmbientTempMax;
    uint16_t usWellAmbientTempMaxHyst;
    uint16_t usWellAmbientTempMin;
    /** ensure 32 bit alignment */
    uint8_t ucSpare[2];
    uint32_t ulWellTemperatureSensorBottom_Z;
    uint32_t ulWellTemperatureSensorTop_Z;
} masterCfgWell_t;

#endif /* MASTERCFGWELLTYPES_H_ */

/** @}  End of this module */
