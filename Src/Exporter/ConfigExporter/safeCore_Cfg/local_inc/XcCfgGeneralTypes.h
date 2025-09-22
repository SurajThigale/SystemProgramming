/** \addtogroup XcCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      XcCfgGeneralTypes.h
 *  @brief     Data type that holds all the XC CFG FUB configuration data.
 *  @author    Frank Meier, Zijie Wang
 *  @reference Exchanger CFG Fub Functional Block Specification
 */

#ifndef XC_CFGGENERAL_TYPES_H_
#define XC_CFGGENERAL_TYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneral.h"
#include "XcCfgSrTypes.h"
#include "XcCfgXcTypes.h"
#include "XcCfgConstantsTypes.h"

/*
 * TYPE DEFINITIONS
 */
#define NUMBER_OF_SFS_ON_XCSN  7U

/**
 * @brief        Full XC CFG FUB configuration data.
 *               Combined from the configuration data of the FUB's sub modules.
 *
 * @requirements SC_REQ-220868
 *
 */
typedef struct { /*lint !e9045 */
    /** Checksum employed when data is transfered. */
    uint32_t ulCrc;

    xcCfgConstants_t tConstants;

    /** own safety node id */
    tXcId_t tXcId;
    /** ensure 32 bit alignment */
    uint8_t Spare1[3];

    xcCfgSr_t tSr;
    xcCfgXc_t tXc;
    uint16_t usXCSN_Sf_Enable[8];
} xcCfg_t;

#endif /* XC_CFGGENERAL_TYPES_H_ */
/** @}  End of this module */
