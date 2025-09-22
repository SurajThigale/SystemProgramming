/** \defgroup GenericCfgWellDoor GenericCfgWellDoor SW Module Specification
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgWellDoor.h
 *  @brief     Provides access to Safety Node specific CfgWell configuration parameter data.
 *  @author    Frank Meier
 *
 */

#ifndef CFGWELLDOOR_H_
#define CFGWELLDOOR_H_

/*
 * INCLUDEFILES
 */
#include "UtilBase.h"
#include "CfgTypes.h"

/*
 * TYPE DEFINITIONS
 */


/**
 * @brief           Well door info
 * @requirements    SC_REQ-218569
 */
typedef struct {
    area_t tDoorZone; /**< door zone area */
    area_t tUcmZone; /**< UCM zone area */
    area_t tUnlockingZone; /**< unlocking zone area */
    uint16_t usAffectedSegments; /**< Bit field, telling which segment is active
                                 * if door is open with no LCU behind */
    /* padding to keep elements 32 bit aligned */
    uint16_t ucSpare;
} wellDoorInfo_t;

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
CfgWellDoor_initialize(void);

extern int32_t
CfgWellDoor_get(enCfgParam_t tParam, uint32_t ulIndex);

extern const wellDoorInfo_t*
CfgWellDoor_getWellDoorInfo(uint32_t ulIndex);

#endif /* CFGWELLDOOR_H_ */

/** @}  End of this module */
