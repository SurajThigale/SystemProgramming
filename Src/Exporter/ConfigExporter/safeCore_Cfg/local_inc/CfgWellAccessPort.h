/** \defgroup GenericCfgWellAccessPort GenericCfgWellAccessPort SW Module Specification
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgWellAccessPort.h
 *  @brief     Provides access to Safety Node specific CfgWell configuration parameter data.
 *  @author    Apurva Pandit
 *
 */

#ifndef CFGWELLACCESSPORT_H_
#define CFGWELLACCESSPORT_H_

 /*
  * INCLUDEFILES
  */
#include "UtilBase.h"
#include "CfgTypes.h"

  /*
   * TYPE DEFINITIONS
   */


   /**
    * @brief           Well Access Port info
    * @requirements    SC_REQ-245465
    */
typedef struct {
    area_t tDoorZone; /**< door zone area */
} wellAccessPortInfo_t;

/*
 * EXTERN FUNCTION DECLARATIONS
 */
extern bool
CfgWellAccessPort_initialize(void);

extern int32_t
CfgWellAccessPort_get(enCfgParam_t tParam, uint32_t ulIndex);

extern const wellAccessPortInfo_t*
CfgWellAccessPort_getWellAccessPortInfo(uint32_t ulIndex);

#endif /* CFGWELLACCESSPORT_H_ */

/** @}  End of this module */