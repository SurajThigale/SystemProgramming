/** \addtogroup LcuCfgWellDoor
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgWellDoorTypes.h
 *  @brief     Data type that holds all the LCU's CfgWellDoor module configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef LCUCFGWELLDOORTYPES_H_
#define LCUCFGWELLDOORTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgWellDoor.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of LCU CfgWellDoor module.
 * @requirements    SC_REQ-218599
 */
typedef struct { /*lint !e9045 */
    wellDoorInfo_t tWellDoorInfos[MULTI_NUMBER_OF_WELLDOORS_MAX];
} lcuCfgWellDoor_t;

#endif /* LCUCFGWELLDOORTYPES_H_ */

/** @}  End of this module */
