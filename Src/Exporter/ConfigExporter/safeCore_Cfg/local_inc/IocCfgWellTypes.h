/** \addtogroup IocCfgWell
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      IocCfgWellTypes.h
 *  @brief     Data type that holds all the IOC's CfgWell module configuration data.
 *  @author    Alexander Doehrmann
 *  @reference IOCollector CFG Fub Functional Block Specification
 *
 */

#ifndef IOC_CFGWELL_TYPES_H_
#define IOC_CFGWELL_TYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgWell.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 *  @brief        Configuration data of IOC CfgWell module.
 *
 *  @requirements SC_REQ-220643
 */
typedef struct { /*lint !e9045 */
    int32_t lWell_AmbientTempMax;
    int32_t lWell_AmbientTempMaxHyst;
    int32_t lWell_AmbientTempMin;
} iocCfgWell_t;

#endif /* IOC_CFGWELL_TYPES_H_ */
/** @}  End of this module */
