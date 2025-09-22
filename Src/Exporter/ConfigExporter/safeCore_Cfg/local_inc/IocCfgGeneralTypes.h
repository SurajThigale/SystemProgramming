/** \addtogroup IocCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      IocCfgGeneralTypes.h
 *  @brief     Data type that holds all the IOCollector CFG FUB configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef IOC_CFGGENERAL_TYPES_H_
#define IOC_CFGGENERAL_TYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneral.h"
#include "UtilBase.h"
#include "IocCfgIocTypes.h"
#include "IocCfgSrTypes.h"
#include "IocCfgWellTypes.h"
#include "IocCfgConstantsTypes.h"
//#include "CfgSfSn.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           IOC configuration data.
 * @requirements    SC_REQ-221218,
 *                  SC_REQ-222283
 */
typedef struct { /*lint !e9045 */
    uint32_t ulCrc;

    /** Configuration constants data */
    iocCfgConstants_t tConstants;

    /** own safety node id */
    tIocId_t tIocId;
    /** ensure 32 bit alignment */
    uint8_t ucSpare2[3];

    iocCfgIoc_t tIoc;
    iocCfgSr_t tSr;
    iocCfgWell_t tWell;
    uint16_t usIOCSN_Sf_Enable[2];
} iocCfg_t;

#endif /* IOC_CFGGENERAL_TYPES_H_ */
