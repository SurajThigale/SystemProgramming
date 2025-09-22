/** \addtogroup IocCfgConstants
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      IocCfgConstantsTypes.h
 *  @brief     Data type that holds all the IOC's CfgConstants module configuration data.
 *  @author    Daniel Mueller
 *
 */

#ifndef IOCCFGCONSTANTSTYPES_H_
#define IOCCFGCONSTANTSTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of IOC CfgConstants module.
 * @requirements    SC_REQ-222093 - FBRQ - Define configuration data structure (IOC CfgConstants)
 */
typedef struct { /*lint !e9045 */
    cfgVersion_t tConfigVersion;
    /** ensure 32 bit alignment */
    uint8_t ucSpare[2];
} iocCfgConstants_t;

#endif /* IOCCFGCONSTANTSTYPES_H_ */

/** @}  End of this module */
