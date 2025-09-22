/** \addtogroup LcuCfgWellAccessPort
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgWellAccessPortTypes.h
 *  @brief     Data type that holds all the LCU's CfgWellAccessPort module configuration data.
 *  @author    Apurva Pandit
 *
 */

#ifndef LCUCFGWELLACCESSPORTTYPES_H_
#define LCUCFGWELLACCESSPORTTYPES_H_

 /*
  * INCLUDEFILES
  */
#include "CfgWellAccessPort.h"
#include "CfgGeneralFubInternalTypes.h"

  /*
   * TYPE DEFINITIONS
   */

   /**
    * @brief           Configuration data of LCU CfgWellAccessPort module.
    * @requirements    SC_REQ-245473
    */
typedef struct { /*lint !e9045 */
    wellAccessPortInfo_t tWellAccessPortInfos[MULTI_NUMBER_OF_WELLDOORS_MAX];
} lcuCfgWellAccessPort_t;

#endif /* LCUCFGWELLACCESSPORTTYPES_H_ */

/** @}  End of this module */