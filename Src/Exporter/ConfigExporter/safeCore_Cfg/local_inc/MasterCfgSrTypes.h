/** \addtogroup MasterCfgSr
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgSrTypes.h
 *  @brief     Provides access to Master specific CfgSr configuration parameter data.
 *  @author    Frank Meier
 *
 */

#ifndef MASTERCFGSRTYPES_H_
#define MASTERCFGSRTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgSr.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * MACRO DEFINITIONS
 */
/** Number of SRs on Master */
#define NUMBER_OF_MASTER_SSR_ON_SN      3U          // As per Gen1.0
/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of Master CfgSr module.
 * @requirements    SC_REQ-218648
 */
typedef struct { /*lint !e9045 */
    int32_t lMSMA_MaxTemperature;
    int32_t lIOCSN_MaxTemperature;
    int32_t lLCUSN_MaxTemperature;
    int32_t lSCTS_MaxErrorTime;
    int32_t lSCTS_ErrorOverwriteErrorTemp;
    int32_t lSCTS_ErrorOverwriteWarnTemp;
    int32_t lMSMA_FireCompartmentRetractionFeedbackTimeout;
    uint16_t usMASN_Ssr_Enable[NUMBER_OF_MASTER_SSR_ON_SN];
} masterCfgSr_t;

#endif /* MASTERCFGSRTYPES_H_ */

/** @}  End of this module */
