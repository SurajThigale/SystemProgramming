/** \addtogroup GenericCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgGeneralFubInternal.h
 *  @brief     Functions used internally by multiple Cfg FUBs and FUB modules.
 *  @author    Frank Meier, Alexander Doehrmann
 *  @reference Generic CFG Fub Functional Block Specification
 *
 */

#ifndef CFGGENERALFUBINTERNAL_H_
#define CFGGENERALFUBINTERNAL_H_

/*
 * INCLUDE FILES
 */
#include <stdbool.h>
#include <stdint.h>
#include "CfgTypes.h"
#include "CfgGeneralFubInternalTypes.h"
#if defined(SN_MASTER)
#include "../../fubs_master/fub_cfg/MasterCfg_Globals.h"
#elif defined(SN_LCU)
#include "../../fubs_lcu/fub_cfg/LcuCfg_Globals.h"
#elif defined(SN_XC)
#include "../../fubs_xc/fub_cfg/XcCfg_Globals.h"
#elif defined(SN_IOC)
#include "../../fubs_ioc/fub_cfg/IocCfg_Globals.h"
#else
#error "Undefined safety node"
#endif

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           FUB modules that handle configuration parameter data.
 * @requirements    SC_REQ-218578
 */
typedef enum {
    CfgParamModule_Constants,
    CfgParamModule_Lcu,
    CfgParamModule_PA,
    CfgParamModule_Sr,
    CfgParamModule_WellDoor,
    CfgParamModule_WellAccessPort,
    CfgParamModule_Well,
    CfgParamModule_Xc,
    CfgParamModule_Ioc,
} enCfgParamModule_t;

/*
 * EXTERN FUNCTION DECLARATIONS
 */

void
CfgGeneral_clearGlobals(void);

CfgGlobals_t*
CfgGeneral_getGlobals(void);

enCfgParamModule_t
CfgGeneral_getParamModuleForParam(enCfgParam_t tParam);

#endif /* CFGGENERALFUBINTERNAL_H_ */

/** @}  End of this module */
