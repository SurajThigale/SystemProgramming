/** \addtogroup LcuCfgConstants
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgConstantsTypes.h
 *  @brief     Data type that holds all the LCU's CfgConstants module configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef XCCFGCONSTANTSTYPES_H_
#define XCCFGCONSTANTSTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of XC CfgConstants module.
 * @requirements    SC_REQ-
 */
typedef struct { /*lint !e9045 */
    cfgVersion_t tConfigVersion;
    uint32_t ulConfigJobSpecificId;
} xcCfgConstants_t;

#endif /* XCCFGCONSTANTSTYPES_H_ */

/** @}  End of this module */
