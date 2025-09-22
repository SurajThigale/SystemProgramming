/** \addtogroup MasterCfgWellDoor
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgWellDoorTypes.h
 *  @brief     Data type that holds all the Master's CfgWellDoor module configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef MASTERCFGWELLDOORTYPES_H_
#define MASTERCFGWELLDOORTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgWellDoor.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of Master CfgWellDoor module.
 * @requirements    SC_REQ-218651
 */
typedef struct { /*lint !e9045 */
    wellDoorInfo_t tWellDoorInfos[MULTI_NUMBER_OF_WELLDOORS_MAX];
} masterCfgWellDoor_t;

#endif /* MASTERCFGWELLDOORTYPES_H_ */

/** @}  End of this module */
