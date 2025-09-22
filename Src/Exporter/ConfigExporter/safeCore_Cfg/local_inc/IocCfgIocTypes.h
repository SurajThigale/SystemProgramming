/** \addtogroup IocCfgIoc
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      IocCfgIocTypes.h
 *  @brief     Data type that holds all the IOC's CfgIoc module configuration data.
 *  @author    Alexander Doehrmann
 *  @reference IOCollector CFG Fub Functional Block Specification
 *
 */

#ifndef IOC_CFGIOC_TYPES_H_
#define IOC_CFGIOC_TYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgIoc.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 *  @brief        Configuration data of IOC CfgIoc module.
 *
 *  @requirements SC_REQ-220645
 *                SC_REQ-221229
 */
typedef struct { /*lint !e9045 */
    iocDeviceMappingTable_t tDeviceMappingTable;
} iocCfgIoc_t;

#endif /* IOC_CFGIOC_TYPES_H_ */
/** @}  End of this module */
