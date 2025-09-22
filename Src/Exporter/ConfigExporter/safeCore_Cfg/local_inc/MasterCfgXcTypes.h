/** \addtogroup MasterCfgXc
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgXcTypes.h
 *  @brief     Data type that holds all the Master's CfgXc module configuration data.
 *  @author    Frank Meier, Zijie Wang
 *
 */

#ifndef MASTERCFGXCTYPES_H_
#define MASTERCFGXCTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgXc.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of Master CfgXc module.
 * @requirements    SC_REQ-218655
 */
typedef struct { /*lint !e9045 */
    xcInfo_t tXcInfo[MULTI_NUMBER_OF_XC_MAX];
    int32_t lXcMatchToleranceY;
    int32_t lXcMatchToleranceZ;
} masterCfgXc_t;

#endif /* MASTERCFGXCTYPES_H_ */

/** @}  End of this module */
