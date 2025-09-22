/** \addtogroup GenericCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgGeneral.h
 *  @brief     Provides access to Safety Node specific configuration parameter data and handles
 *             configuration load, store and validation.
 *  @author    Frank Meier
 */

#ifndef CFGGENERAL_H_
#define CFGGENERAL_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include <stdint.h>
#include "UtilOpMode.h"
#include "CfgTypes.h"

/*
 * TYPE DEFINITIONS
 */
/**
 * @brief
 * Macro for identifying a not yet loaded Configuration CRC
 *
 * @requirements
 * SC_REQ-222085 - FBRQ - consistency check of configuration CRC
 */
#define INVALID_CONFIGURATION_CRC 0U

/**
 * @brief           Configuration data validity.
 * @requirements    SC_REQ-218578
 * */
typedef enum {
    /** Configuration data is invalid */
    CfgValidity_Invalid,
    /** Configuration data is valid */
    CfgValidity_Valid,
} enCfgValidity_t;

/**
 * @brief           Return type of CfgGeneral_loadConfiguration, CfgGeneral_storeConfiguration
 * @requirements    SC_REQ-218578
 */
typedef enum {
    /** Operation starting */
    CfgIoState_Begin,
    /** Operation in progress */
    CfgIoState_InProgress,
    /** Operation completed */
    CfgIoState_Completed,
    /** Operation failed due to checksum error */
    CfgIoState_CrcError,
    /** Default config version is loaded. I.e. config is not available from CS */
    CfgIoState_DefaultConfiguration,
    /** Requested version and loaded version do not match */
    CfgIoState_VersionMismatch,
    /** Persisted CRC and loaded CRC do not match */
    CfgIoState_PersistenceError,
} enCfgIoState_t;


/*
 * EXTERN FUNCTION DECLARATIONS
 */
extern bool
CfgGeneral_beginConfigurationLoading(void);

extern bool
CfgGeneral_initialize(void);

extern bool
CfgGeneral_isJobSpecificConfigurationLoaded(void);

extern void
CfgGeneral_resetJobSepecifcConfigurationLoaded(void);

extern enCfgValidity_t
CfgGeneral_getConfigurationValidity(void);

uint32_t
CfgGeneral_getConfigurationCrc(void);

void
CfgGeneral_setConfigurationCrc(uint32_t ulConfigCrc);

extern void
CfgGeneral_setConfigurationValidity(enCfgValidity_t tValidity);

extern enCfgIoState_t
CfgGeneral_loadConfiguration(void);

extern enCfgIoState_t
CfgGeneral_storeConfiguration(void);

extern uint32_t
CfgGeneral_calculateConfigurationChecksum(void);

uint32_t
CfgGeneral_getConfigCrcArea(const uint8_t** ppucStart);

uint32_t
CfgGeneral_calcCrc(
    uint32_t ulPrevious,
    const void* pvBuf,
    uint32_t ulLen);

cfgVersion_t
CfgGeneral_getLoadedConfigVersion(void);

uint32_t
CfgGeneral_getLoadedConfigCRC(void);


extern int32_t
CfgGeneral_get(enCfgParam_t tParam, uint32_t ulIndex);

extern bool
CfgGeneral_checkConfigVersion(void);

void
CfgGeneral_restorePersistedState(void);


#endif /* CFGGENERAL_H_ */

/** @}  End of this module */
