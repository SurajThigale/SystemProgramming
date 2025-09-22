/** \addtogroup LcuCfg
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgLcu.h
 *  @brief     LCU specific Cfg definitions.
 *  @author    Daniel Mueller
 *
 */


#ifndef SRC_FUBS_LCU_INCLUDE_LCUCFGLCU_H
#define SRC_FUBS_LCU_INCLUDE_LCUCFGLCU_H

/*
 * INCLUDE FILES
 */

#include "LcuCfgLcuTypes.h"

/*
 * EXTERN FUNCTION DECLARATIONS
 */
const lcuBrakeLut_t*
CfgLcu_getLutFn(uint32_t ulIndex);

const lcuBrakeLut_t*
CfgLcu_getLutXtarget(uint32_t ulIndex);

const lcuCfgApsPosition_t* CfgLcu_getApsPositionInfo(uint32_t ulIndex);

uint32_t CfgLcu_getNumOfCodeTapeInstalled(uint32_t ulIndex);

#endif /* SRC_FUBS_LCU_INCLUDE_LCUCFGLCU_H */

/** @}  End of this module */
