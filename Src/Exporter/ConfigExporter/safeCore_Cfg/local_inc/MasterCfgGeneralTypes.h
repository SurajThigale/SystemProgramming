/** \addtogroup MasterCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgGeneralTypes.h
 *  @brief     Data type that holds all the Master CFG FUB configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef MASTERCFGGENERALTYPES_H_
#define MASTERCFGGENERALTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneral.h"
#include "MasterCfgConstantsTypes.h"
#include "MasterCfgLcuTypes.h"
#include "MasterCfgProtectionAreasTypes.h"
#include "MasterCfgSrTypes.h"
#include "MasterCfgWellDoorTypes.h"
#include "MasterCfgXcTypes.h"
#include "MasterCfgIocTypes.h"
#include "MasterCfgWellTypes.h"
//#include "CfgSfSn.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Master configuration data.
 * @requirements    SC_REQ-218661,
 *                  SC_REQ-222283
 */
typedef struct { /*lint !e9045 */
    /** Checksum employed when data is transfered. */
    uint32_t ulCrc;

    masterCfgConstants_t tConstants;
    masterCfgLcu_t tLcu;
    masterCfgProtectionAreas_t tProtectionAreas;
    masterCfgSr_t tSr;
    masterCfgWellDoor_t tWellDoor;
    masterCfgXc_t tXc;
    masterCfgIoc_t tIoc;
    masterCfgWell_t tWell;
    uint16_t usMASN_Sf_Enable[11];
} masterCfg_t;


#endif /* MASTERCFGGENERALTYPES_H_ */

/** @}  End of this module */
