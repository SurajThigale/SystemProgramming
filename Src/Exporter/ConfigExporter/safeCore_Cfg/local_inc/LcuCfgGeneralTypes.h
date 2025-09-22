/** \addtogroup LcuCfgGeneral
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgGeneralTypes.h
 *  @brief     Data type that holds all the LCU CFG FUB configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef LCUCFGGENERALTYPES_H_
#define LCUCFGGENERALTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneral.h"
#include "LcuCfgConstantsTypes.h"
#include "LcuCfgLcuTypes.h"
#include "LcuCfgProtectionAreasTypes.h"
#include "LcuCfgSrTypes.h"
#include "LcuCfgWellDoorTypes.h"
#include "LcuCfgWellAccessPortTypes.h"
#include "LcuCfgXcTypes.h"
//#include "CfgSfSn.h"
/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           LCU configuration data.
 * @requirements    SC_REQ-218609,
 *                  SC_REQ-222283
 */
typedef struct { /*lint !e9045 */
    /** Checksum employed when data is transfered. */
    uint32_t ulCrc;

    /** own safety node id */
    tLcuId_t tLcuId;
    /** ensure 32 bit alignment */
    uint8_t ucSpare1[3];

    lcuCfgConstants_t tConstants;
    lcuCfgLcu_t tLcu;
    lcuCfgProtectionAreas_t tProtectionAreas;
    lcuCfgSr_t tSr;
    lcuCfgWellDoor_t tWellDoor;
    lcuCfgWellAccessPort_t tWellAccessPort;
    lcuCfgXc_t tXc;
    uint16_t usLCUSN_Sf_Enable[15];
} lcuCfg_t;

#endif /* LCUCFGGENERALTYPES_H_ */

/** @}  End of this module */
