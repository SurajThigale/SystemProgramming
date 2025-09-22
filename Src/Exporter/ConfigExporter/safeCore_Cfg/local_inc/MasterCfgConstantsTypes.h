/** \addtogroup MasterCfgConstants
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgConstantsTypes.h
 *  @brief     Data type that holds all the Master's CfgConstants module configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef MASTERCFGCONSTANTSTYPES_H_
#define MASTERCFGCONSTANTSTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of Master CfgConstants module.
 * @requirements    SC_REQ-218635
 */
typedef struct { /*lint !e9045 */
    int32_t lMULTI_NumberOfVerticalTracksInstalled;
    int32_t lMULTI_NumberOfHorizontalTracksInstalled;
    int32_t lMULTI_NumberOfXCInstalled;
    int32_t lMULTI_NumberOfLCUInstalled;
    int32_t lMULTI_NumberOfIOCInstalled;
    int32_t lMULTI_NumberOfFireCompartmentsInstalled;
    int32_t lMULTI_NumberOfGarageAreasInstalled;
    int32_t lMULTI_NumberOfMaintenanceAreasInstalled;
    int32_t lMULTI_NumberOfPropulsionSectionsInstalled;
    int32_t lMULTI_NumberOfWallPanelAreasInstalled;
    int32_t lMULTI_NumberOfWalkwayAreasInstalled;
    int32_t lMULTI_NumberOfWellWaterAreasInstalled;
    int32_t lMULTI_NumberOfWellDoorsInstalled;
    int32_t lMULTI_NumberOfWellAccessPortsInstalled;
    int32_t lMULTI_NumberOfWellLocksInstalled;
    int32_t lMULTI_NumberOfWellSegmentsInstalled;
    int32_t lMSMA_MaxSubsystemInitWaitingTime;

    cfgVersion_t tConfigVersion;
    /** ensure 32 bit alignment */
    uint8_t ucSpare[2];

    uint32_t ulConfigJobSpecificId;
} masterCfgConstants_t;

#endif /* MASTERCFGCONSTANTSTYPES_H_ */

/** @}  End of this module */
