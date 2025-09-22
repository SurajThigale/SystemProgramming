/** \addtogroup LcuCfgConstants
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgConstantsTypes.h
 *  @brief     Data type that holds all the LCU's CfgConstants module configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef LCUCFGCONSTANTSTYPES_H_
#define LCUCFGCONSTANTSTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of LCU CfgConstants module.
 * @requirements    SC_REQ-218581
 */
typedef struct { /*lint !e9045 */
    int32_t lMULTI_NumberOfXCInstalled;
    int32_t lMULTI_NumberOfHorizontalTracksInstalled;
    int32_t lMULTI_NumberOfVerticalTracksInstalled;
    int32_t lMULTI_NumberOfGarageAreasInstalled;
    int32_t lMULTI_NumberOfMaintenanceAreasInstalled;
    int32_t lMULTI_NumberOfWellDoorsInstalled;
    int32_t lMULTI_NumberOfWellAccessPortsInstalled;

    cfgVersion_t tConfigVersion;
    /** ensure 32 bit alignment */
    uint8_t ucSpare[2];

    uint32_t ulConfigJobSpecificId;

    int32_t lLutFn_NumberOfEntriesMax;
    int32_t lLutXtarget_NumberOfEntriesMax;
    int32_t lLCUBrake_CalibrationCountRisingThreshold;
    int32_t lLCUBrake_CalibrationCountFallingThreshold;
    int32_t lLCUBrake_CalibrationDeltaP;
} lcuCfgConstants_t;

#endif /* LCUCFGCONSTANTSTYPES_H_ */

/** @}  End of this module */
