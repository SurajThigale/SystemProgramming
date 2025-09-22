/** \addtogroup MasterCfgIoc
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgIocTypes.h
 *  @brief     Data type that holds all the Master's CfgIoc module configuration data.
 *  @author    Daniel Mueller, Alexander Doehrmann
 *  @reference MasterArray CFG Fub Functional Block Specification
 *
 */

#ifndef MASTERCFGIOCTYPES_H_
#define MASTERCFGIOCTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneralFubInternalTypes.h"
#include "CfgIoc.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief
 * @requirements SC_REQ-221104
 */
typedef struct { /*lint !e9045 */
    iocDeviceMappingTable_t tDeviceMappings[MULTI_NUMBER_OF_IOC_MAX];
} masterCfgIoc_t;

#endif /* MASTERCFGIOCTYPES_H_ */

/** @}  End of this module */
