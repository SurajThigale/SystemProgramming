/** \defgroup GenericCfgIoc GenericCfgIoc SW Module Specification
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgIoc.h
 *  @brief     CfgIoc configuration specific definitions used by multiple Cfg FUBs.
 *  @author    Alexander Doehrmann
 */

#ifndef CFGIOC_H_
#define CFGIOC_H_

/*
 * INCLUDEFILES
 */
#include <stdbool.h>
#include <stdint.h>
#include "CfgTypes.h"

/*
 * MACRO DEFINITIONS
 */

/**
 * @brief           Defines to declare maximum array sizes required for the
*                   IOC configuration data.
 *
 * @requirements    SC_REQ-220539
 *
 */
#define MAX_WDSS_AVAILABLE      36U
#define MAX_WAPSS_AVAILABLE     36U
#define MAX_WSSS_AVAILABLE      36U
#define MAX_WWLSS_AVAILABLE     1U
#define MAX_PIMASS_AVAILABLE    1U
#define MAX_PIGASS_AVAILABLE    1U
#define MAX_PIWSS_AVAILABLE     2U
#define MAX_WPSS_AVAILABLE      10U
#define MAX_WSES_AVAILABLE      2U
#define MAX_FCSS_AVAILABLE      2U
#define MAX_WTSS_AVAILABLE      1U
#define MAX_UDSS_AVAILABLE      1U

#define MAX_WDSS_AND_WAPSS      36U

/**
 * @brief           Defines to declare a default value for an unpopulated entry.
 *
 * @requirements    SC_REQ-220539
 *
 */
#define IOC_ENTRY_NOT_POPULATED_INT16 0xffffu
#define IOC_ENTRY_NOT_POPULATED_INT8 0xffu

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief         Data types required for the IOC configuration data.
 *
 * @requirements  SC_REQ-220645
 *
 */
typedef uint16_t wellDoorIndex_t;
typedef uint16_t wellAccessPortIndex_t;
typedef uint16_t wellSegmentAreaSet_t;
typedef uint8_t  wellSegmentAreaIndex_t;
typedef uint8_t  wellWaterIndex_t;
typedef uint8_t  maintenanceAreaIndex_t;
typedef uint8_t  garageAreaIndex_t;
typedef uint8_t  walkwayAreaIndex_t;
typedef uint8_t  wallPanelAreaIndex_t;
typedef uint8_t  fireCompartmentIndex_t;
typedef uint8_t  translationalMovementAreaIndex_t;
typedef uint8_t  wellTemperatureIndex_t;


/**
 * @brief         Data structure required for the IOC configuration data.
 *
 * @requirements  SC_REQ-220645
 *
 */
typedef struct {
    wellDoorIndex_t                  tWellDoorLockedClosed[MAX_WDSS_AVAILABLE];
    wellAccessPortIndex_t            tWellAccessPortClosed[MAX_WAPSS_AVAILABLE];
    wellSegmentAreaSet_t             tWellSegmentAccessPortNotClosed[MAX_WSSS_AVAILABLE];
    wellSegmentAreaSet_t             tWellSegmentEmergencyStopTriggered[MAX_WSES_AVAILABLE];
    wellWaterIndex_t                 tWaterInWell[MAX_WWLSS_AVAILABLE];
    maintenanceAreaIndex_t           tPersonInMaintenanceArea[MAX_PIMASS_AVAILABLE];
    garageAreaIndex_t                tPersonInGarage[MAX_PIGASS_AVAILABLE];
    walkwayAreaIndex_t               tPersonInWalkway[MAX_PIWSS_AVAILABLE];
    wallPanelAreaIndex_t             tWallPanelClosed[MAX_WPSS_AVAILABLE];
    fireCompartmentIndex_t           tFireCompartmentRetracted[MAX_FCSS_AVAILABLE];
    wellTemperatureIndex_t           tWellTemperature[MAX_WTSS_AVAILABLE];
    uint8_t                          ucUnderpowerDetectionUsed[MAX_UDSS_AVAILABLE];
    uint8_t                          pad;
} iocDeviceMappingTable_t;

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
CfgIOC_initialize(void);

#ifdef SN_MASTER /* the master SN needs an extended interface for IOC cfg access */

extern uint32_t
CfgIOC_get(uint32_t ulIocIdx,
    enCfgParam_t tParam,
    uint32_t ulIndex);

#else

extern int32_t
CfgIOC_get(enCfgParam_t tParam,
    uint32_t ulIndex);

#endif

#endif /* CFGIOC_H_ */

/** @}  End of this module */
