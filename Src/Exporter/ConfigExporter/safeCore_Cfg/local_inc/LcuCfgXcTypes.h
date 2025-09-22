/** \addtogroup LcuCfgXc
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgXcTypes.h
 *  @brief     Data type that holds all the LCU CfgXc module configuration data.
 *  @author    Frank Meier, Zijie Wang
 *
 */

#ifndef LCUCFGXCTYPES_H_
#define LCUCFGXCTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgXc.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of LCU CfgXc module.
 * @requirements    SC_REQ-218603
 */
typedef struct { /*lint !e9045 */
    xcInfo_t tXcInfo[MULTI_NUMBER_OF_XC_MAX];
    int32_t lXcMatchToleranceY;
    int32_t lXcMatchToleranceZ;
} lcuCfgXc_t;

#endif /* LCUCFGXCTYPES_H_ */

/** @}  End of this module */
