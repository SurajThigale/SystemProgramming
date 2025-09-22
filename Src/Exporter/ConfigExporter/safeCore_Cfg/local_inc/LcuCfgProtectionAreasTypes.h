/** \addtogroup LcuCfgProtectionAreas
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgProtectionAreasTypes.h
 *  @brief     Data type that holds all the LCU's CfgProtectionArea module configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef LCUCFGPROTECTIONAREASTYPES_H_
#define LCUCFGPROTECTIONAREASTYPES_H_

/*
 * INCLUDEFILES
 */

#include "UtilBase.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of LCU CfgProtectionAreas module.
 * @requirements    SC_REQ-218592
 */
typedef struct { /*lint !e9045 */
    protectionArea_t tGarageAreas[MULTI_NUMBER_OF_GARAGEAREAS_MAX];
    protectionArea_t tMaintenanceAreas[MULTI_NUMBER_OF_MAINTENANCEAREAS_MAX];
    protectionArea_t tTranslationalMovementAreas[MULTI_NUMBER_OF_TRACKS_MAX];

} lcuCfgProtectionAreas_t;


#endif /* LCUCFGPROTECTIONAREASTYPES_H_ */

/** @}  End of this module */
