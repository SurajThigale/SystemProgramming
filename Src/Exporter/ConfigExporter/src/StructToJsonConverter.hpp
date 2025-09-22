#pragma once

#include <iostream>
#include <string>
#include "MasterCfgGeneralTypes.h"
#include "IocCfgGeneralTypes.h"
#include "LcuCfgGeneralTypes.h"
#include "XcCfgGeneralTypes.h"
#include "XcCfgXcTypes.h"
#include <XcCfgGeneralTypes.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


// Function to convert point_t to JSON
json to_json(const point_t& point) {
    json jsonPoint;
    jsonPoint["lY"] = point.lY;
    jsonPoint["lZ"] = point.lZ;
    return jsonPoint;
}

// Function to convert area_t to JSON
json to_json(const area_t& area) {
    json jsonArea;
    jsonArea["tTopLeft"] = to_json(area.tTopLeft);
    jsonArea["tBottomRight"] = to_json(area.tBottomRight);
    return jsonArea;
}

json to_json(const masterCfgConstants_t& constants) {
    json jsonConstants;

    jsonConstants["lMULTI_NumberOfVerticalTracksInstalled"] = constants.lMULTI_NumberOfVerticalTracksInstalled;
    jsonConstants["lMULTI_NumberOfHorizontalTracksInstalled"] = constants.lMULTI_NumberOfHorizontalTracksInstalled;
    jsonConstants["lMULTI_NumberOfXCInstalled"] = constants.lMULTI_NumberOfXCInstalled;
    jsonConstants["lMULTI_NumberOfLCUInstalled"] = constants.lMULTI_NumberOfLCUInstalled;
    jsonConstants["lMULTI_NumberOfIOCInstalled"] = constants.lMULTI_NumberOfIOCInstalled;
    jsonConstants["lMULTI_NumberOfFireCompartmentsInstalled"] = constants.lMULTI_NumberOfFireCompartmentsInstalled;
    jsonConstants["lMULTI_NumberOfGarageAreasInstalled"] = constants.lMULTI_NumberOfGarageAreasInstalled;
    jsonConstants["lMULTI_NumberOfMaintenanceAreasInstalled"] = constants.lMULTI_NumberOfMaintenanceAreasInstalled;
    jsonConstants["lMULTI_NumberOfPropulsionSectionsInstalled"] = constants.lMULTI_NumberOfPropulsionSectionsInstalled;
    jsonConstants["lMULTI_NumberOfWallPanelAreasInstalled"] = constants.lMULTI_NumberOfWallPanelAreasInstalled;
    jsonConstants["lMULTI_NumberOfWalkwayAreasInstalled"] = constants.lMULTI_NumberOfWalkwayAreasInstalled;
    jsonConstants["lMULTI_NumberOfWellWaterAreasInstalled"] = constants.lMULTI_NumberOfWellWaterAreasInstalled;
    jsonConstants["lMULTI_NumberOfWellDoorsInstalled"] = constants.lMULTI_NumberOfWellDoorsInstalled;
    jsonConstants["lMULTI_NumberOfWellAccessPortsInstalled"] = constants.lMULTI_NumberOfWellAccessPortsInstalled;
    jsonConstants["lMULTI_NumberOfWellLocksInstalled"] = constants.lMULTI_NumberOfWellLocksInstalled;
    jsonConstants["lMULTI_NumberOfWellSegmentsInstalled"] = constants.lMULTI_NumberOfWellSegmentsInstalled;
    jsonConstants["lMSMA_MaxSubsystemInitWaitingTime"] = constants.lMSMA_MaxSubsystemInitWaitingTime;

    jsonConstants["tConfigVersion"] = constants.tConfigVersion;

    jsonConstants["ucSpare"] = { constants.ucSpare[0], constants.ucSpare[1] };
    jsonConstants["ulConfigJobSpecificId"] = constants.ulConfigJobSpecificId;

    return jsonConstants;
}

json to_json(const stoppingDistanceParam_t& param) {
    json jsonParam;
    jsonParam["lK1"] = param.lK1;
    jsonParam["lK2"] = param.lK2;
    jsonParam["lOffset"] = param.lOffset;
    return jsonParam;
}

json to_json(const stoppingDistanceParamSet_t& paramSet) {
    json jsonParamSet;

    // Convert each array of stoppingDistanceParam_t to JSON
    jsonParamSet["LoSD_upwards_BS"] = json::array();
    for (const auto& param : paramSet.LoSD_upwards_BS) {
        jsonParamSet["LoSD_upwards_BS"].push_back(to_json(param));
    }

    jsonParamSet["LoSD_downwards_BS"] = json::array();
    for (const auto& param : paramSet.LoSD_downwards_BS) {
        jsonParamSet["LoSD_downwards_BS"].push_back(to_json(param));
    }

    jsonParamSet["USD_downwards_BS"] = json::array();
    for (const auto& param : paramSet.USD_downwards_BS) {
        jsonParamSet["USD_downwards_BS"].push_back(to_json(param));
    }

    jsonParamSet["USD_upwards_BS"] = json::array();
    for (const auto& param : paramSet.USD_upwards_BS) {
        jsonParamSet["USD_upwards_BS"].push_back(to_json(param));
    }

    jsonParamSet["LeSD_rightwards_BS"] = json::array();
    for (const auto& param : paramSet.LeSD_rightwards_BS) {
        jsonParamSet["LeSD_rightwards_BS"].push_back(to_json(param));
    }

    jsonParamSet["LeSD_leftwards_BS"] = json::array();
    for (const auto& param : paramSet.LeSD_leftwards_BS) {
        jsonParamSet["LeSD_leftwards_BS"].push_back(to_json(param));
    }

    jsonParamSet["RSD_rightwards_BS"] = json::array();
    for (const auto& param : paramSet.RSD_rightwards_BS) {
        jsonParamSet["RSD_rightwards_BS"].push_back(to_json(param));
    }

    jsonParamSet["RSD_leftwards_BS"] = json::array();
    for (const auto& param : paramSet.RSD_leftwards_BS) {
        jsonParamSet["RSD_leftwards_BS"].push_back(to_json(param));
    }

    return jsonParamSet;
}

// Function to convert lcuPhysicalConstraints_t to JSON
json to_json(const lcuPhysicalConstraints_t& lcuPhysConstraints) {
    json jsonLcuPhysConstraints;

    jsonLcuPhysConstraints["tSize"] = to_json(lcuPhysConstraints.tSize);
    jsonLcuPhysConstraints["sPivotBearingOffsetY"] = lcuPhysConstraints.sPivotBearingOffsetY;
    jsonLcuPhysConstraints["sPivotBearingOffsetZ"] = lcuPhysConstraints.sPivotBearingOffsetZ;

    return jsonLcuPhysConstraints;
}

// Function to convert masterCfgLcu_t to JSON
json to_json(const masterCfgLcu_t& lcuConfig) {
    json jsonLcuConfig;

    // Convert each element of the array to JSON
    for (int i = 0; i < 2; ++i) {
        jsonLcuConfig["tPhysicalConstraints"].push_back(to_json(lcuConfig.tPhysicalConstraints[i]));
    }

    jsonLcuConfig["tStoppingDistanceParamSet"] = to_json(lcuConfig.tStoppingDistanceParamSet);
    jsonLcuConfig["cType"] = lcuConfig.cType;
    jsonLcuConfig["cPositionTranslation_TrackIsVertical"] = lcuConfig.cPositionTranslation_TrackIsVertical;
    jsonLcuConfig["lPositionTranslation_CenterAlignmentPosAlongTrack"] = lcuConfig.lPositionTranslation_CenterAlignmentPosAlongTrack;
    jsonLcuConfig["lVelocityFES"] = lcuConfig.lVelocityFES;
    return jsonLcuConfig;
}

json to_json(const enProtectionAreaType_t& protectionAreaType) {
    return static_cast<int>(protectionAreaType);
}

// Function to convert protectionArea_t to JSON
json to_json(const protectionArea_t& protectionArea) {
    json jsonProtectionArea;
    jsonProtectionArea["tArea"] = to_json(protectionArea.tArea);
    jsonProtectionArea["tType"] = protectionArea.tType;
    return jsonProtectionArea;
}

json to_json(const masterCfgProtectionAreas_t& protectionAreas) {
    json jsonProtectionAreas;

    for (const auto& area : protectionAreas.tFireCompartments) {
        jsonProtectionAreas["tFireCompartments"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tGarageAreas) {
        jsonProtectionAreas["tGarageAreas"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tIocAffectedArea) {
        jsonProtectionAreas["tIocAffectedArea"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tMaintenanceAreas) {
        jsonProtectionAreas["tMaintenanceAreas"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tPropulsionSections) {
        jsonProtectionAreas["tPropulsionSections"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tSwivelPlatforms) {
        jsonProtectionAreas["tSwivelPlatforms"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tTranslationalMovementAreas) {
        jsonProtectionAreas["tTranslationalMovementAreas"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tWalkwayAreas) {
        jsonProtectionAreas["tWalkwayAreas"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tWallPanelAreas) {
        jsonProtectionAreas["tWallPanelAreas"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tWellSegmentAreas) {
        jsonProtectionAreas["tWellSegmentAreas"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tWellDoorAreas) {
        jsonProtectionAreas["tWellDoorAreas"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tWellLocks) {
        jsonProtectionAreas["tWellLocks"].push_back(to_json(area));
    }

    for (const auto& area : protectionAreas.tWellWaterAreas) {
        jsonProtectionAreas["tWellWaterAreas"].push_back(to_json(area));
    }

    jsonProtectionAreas["lWorkingArea_ZPoint2Top"] = protectionAreas.lWorkingArea_ZPoint2Top;
    jsonProtectionAreas["lWorkingArea_ZPoint2Bottom"] = protectionAreas.lWorkingArea_ZPoint2Bottom;
    jsonProtectionAreas["lWorkingArea_YPoint2Right"] = protectionAreas.lWorkingArea_YPoint2Right;
    jsonProtectionAreas["lWorkingArea_YPoint2Left"] = protectionAreas.lWorkingArea_YPoint2Left;
    jsonProtectionAreas["lWorkingArea_ExtensionTop"] = protectionAreas.lWorkingArea_ExtensionTop;
    jsonProtectionAreas["lWorkingArea_ExtensionBottom"] = protectionAreas.lWorkingArea_ExtensionBottom;
    jsonProtectionAreas["lWorkingArea_ExtensionRight"] = protectionAreas.lWorkingArea_ExtensionRight;
    jsonProtectionAreas["lWorkingArea_ExtensionLeft"] = protectionAreas.lWorkingArea_ExtensionLeft;

    return jsonProtectionAreas;
}

json to_json(const masterCfgSr_t& srConfig) {
    json jsonSrConfig;

    jsonSrConfig["lMSMA_MaxTemperature"] = srConfig.lMSMA_MaxTemperature;
    jsonSrConfig["lIOCSN_MaxTemperature"] = srConfig.lIOCSN_MaxTemperature;
    jsonSrConfig["lLCUSN_MaxTemperature"] = srConfig.lLCUSN_MaxTemperature;
    jsonSrConfig["lSCTS_MaxErrorTime"] = srConfig.lSCTS_MaxErrorTime;
    jsonSrConfig["lSCTS_ErrorOverwriteErrorTemp"] = srConfig.lSCTS_ErrorOverwriteErrorTemp;
    jsonSrConfig["lSCTS_ErrorOverwriteWarnTemp"] = srConfig.lSCTS_ErrorOverwriteWarnTemp;
    jsonSrConfig["lMSMA_FireCompartmentRetractionFeedbackTimeout"] = srConfig.lMSMA_FireCompartmentRetractionFeedbackTimeout;

    jsonSrConfig["usMASN_Ssr_Enable"] = json::array();
    for (int i = 0; i < NUMBER_OF_MASTER_SSR_ON_SN; ++i) {
        jsonSrConfig["usMASN_Ssr_Enable"].push_back(srConfig.usMASN_Ssr_Enable[i]);
    }

    return jsonSrConfig;
}

json to_json(const wellDoorInfo_t& wellDoorInfo) {
    json jsonWellDoorInfo;

    jsonWellDoorInfo["tDoorZone"] = to_json(wellDoorInfo.tDoorZone);
    jsonWellDoorInfo["tUcmZone"] = to_json(wellDoorInfo.tUcmZone);
    jsonWellDoorInfo["tUnlockingZone"] = to_json(wellDoorInfo.tUnlockingZone);
    jsonWellDoorInfo["usAffectedSegments"] = wellDoorInfo.usAffectedSegments;
    jsonWellDoorInfo["ucSpare"] = wellDoorInfo.ucSpare;

    return jsonWellDoorInfo;
}


json to_json(const masterCfgWellDoor_t& wellDoorConfig) {
    json jsonWellDoorConfig;

    jsonWellDoorConfig["tWellDoorInfos"] = json::array();
    for (const auto& doorInfo : wellDoorConfig.tWellDoorInfos) {
        jsonWellDoorConfig["tWellDoorInfos"].push_back(to_json(doorInfo));
    }

    return jsonWellDoorConfig;
}

json to_json(const wellAccessPortInfo_t& wellAccessPortInfo) {
    json jsonWellAccessPortInfo;

    jsonWellAccessPortInfo["tDoorZone"] = to_json(wellAccessPortInfo.tDoorZone);
   
    return jsonWellAccessPortInfo;
}

// Function to convert xcInfo_t to JSON
json to_json(const xcInfo_t& xcInfo) {
    json jsonXcInfo;

    jsonXcInfo["tCenter"] = to_json(xcInfo.tCenter);
    jsonXcInfo["tArea"] = to_json(xcInfo.tArea);

    return jsonXcInfo;
}

// Function to convert masterCfgXc_t to JSON
json to_json(const masterCfgXc_t& xcConfig) {
    json jsonXcConfig;

    jsonXcConfig["tXcInfo"] = json::array();
    for (const auto& xcInfo : xcConfig.tXcInfo) {
        jsonXcConfig["tXcInfo"].push_back(to_json(xcInfo));
    }

    jsonXcConfig["lXcMatchToleranceY"] = xcConfig.lXcMatchToleranceY;
    jsonXcConfig["lXcMatchToleranceZ"] = xcConfig.lXcMatchToleranceZ;

    return jsonXcConfig;
}

json to_json(const iocDeviceMappingTable_t& deviceMappings) {
    json jsonDeviceMappings;

    jsonDeviceMappings["tWellDoorLockedClosed"] = deviceMappings.tWellDoorLockedClosed;
    jsonDeviceMappings["tWellAccessPortClosed"] = deviceMappings.tWellAccessPortClosed;
    jsonDeviceMappings["tWellSegmentAccessPortNotClosed"] = deviceMappings.tWellSegmentAccessPortNotClosed;
    jsonDeviceMappings["tWellSegmentEmergencyStopTriggered"] = deviceMappings.tWellSegmentEmergencyStopTriggered;
    jsonDeviceMappings["tWaterInWell"] = deviceMappings.tWaterInWell;
    jsonDeviceMappings["tPersonInMaintenanceArea"] = deviceMappings.tPersonInMaintenanceArea;
    jsonDeviceMappings["tPersonInGarage"] = deviceMappings.tPersonInGarage;
    jsonDeviceMappings["tPersonInWalkway"] = deviceMappings.tPersonInWalkway;
    jsonDeviceMappings["tWallPanelClosed"] = deviceMappings.tWallPanelClosed;
    jsonDeviceMappings["tFireCompartmentRetracted"] = deviceMappings.tFireCompartmentRetracted;
    jsonDeviceMappings["tWellTemperature"] = deviceMappings.tWellTemperature;
    jsonDeviceMappings["ucUnderpowerDetectionUsed"] = deviceMappings.ucUnderpowerDetectionUsed;

    return jsonDeviceMappings;
}

// Function to convert masterCfgIoc_t to JSON
json to_json(const masterCfgIoc_t& iocConfig) {
    json jsonIocConfig;

    jsonIocConfig["tDeviceMappings"] = json::array();
    for (const auto& deviceMappings : iocConfig.tDeviceMappings) {
        jsonIocConfig["tDeviceMappings"].push_back(to_json(deviceMappings));
    }

    return jsonIocConfig;
}

// Function to convert masterCfgWell_t to JSON
json to_json(const masterCfgWell_t& wellConfig) {
    json jsonWellConfig;

    jsonWellConfig["usWellAmbientTempMax"] = wellConfig.usWellAmbientTempMax;
    jsonWellConfig["usWellAmbientTempMaxHyst"] = wellConfig.usWellAmbientTempMaxHyst;
    jsonWellConfig["usWellAmbientTempMin"] = wellConfig.usWellAmbientTempMin;
    jsonWellConfig["ucSpare"] = wellConfig.ucSpare;
    jsonWellConfig["ulWellTemperatureSensorBottom_Z"] = wellConfig.ulWellTemperatureSensorBottom_Z;
    jsonWellConfig["ulWellTemperatureSensorTop_Z"] = wellConfig.ulWellTemperatureSensorTop_Z;

    return jsonWellConfig;
}


json to_json(const masterCfg_t& config) {
    json jsonConfig;

    jsonConfig["ulCrc"] = config.ulCrc;
    jsonConfig["tConstants"] = to_json(config.tConstants);
    jsonConfig["tLcu"] = to_json(config.tLcu);
    jsonConfig["tProtectionAreas"] = to_json(config.tProtectionAreas);
    jsonConfig["tSr"] = to_json(config.tSr);
    jsonConfig["tWellDoor"] = to_json(config.tWellDoor);
    jsonConfig["tXc"] = to_json(config.tXc);
    jsonConfig["tIoc"] = to_json(config.tIoc);
    jsonConfig["tWell"] = to_json(config.tWell);

    // Convert uint16_t array to JSON array
    jsonConfig["usMASN_Sf_Enable"] = json::array();
    for (int i = 0; i < 11; ++i) {
        jsonConfig["usMASN_Sf_Enable"].push_back(config.usMASN_Sf_Enable[i]);
    }

    return jsonConfig;
}

json to_json(const xcSD_Param_t& param) {
    json j;
    j["lXC_K1"] = param.lXC_K1;
    j["lXC_K2"] = param.lXC_K2;
    j["lXC_Offset"] = param.lXC_Offset;
    return j;
}

json to_json(const xcSD_ParamSet_t& paramSet) {
    json j;
    j["tXC_SD_clockwise"] = to_json(paramSet.tXC_SD_clockwise);
    j["tXC_SD_counterclockwise"] = to_json(paramSet.tXC_SD_counterclockwise);
    return j;
}

// Function to convert xcCfgXc_t to JSON
json to_json(const xcCfgXc_t& xcConfigXc) {
    json jsonXcConfigXc;

    // Serialize the array of tXC_SD_ParamSet[2]
    jsonXcConfigXc["tXC_SD_ParamSet"] = json::array();
    for (int i = 0; i < 2; ++i) {
        jsonXcConfigXc["tXC_SD_ParamSet"].push_back(to_json(xcConfigXc.tXC_SD_ParamSet[i]));
    }

    jsonXcConfigXc["sLockingAngleTolerance"] = xcConfigXc.sLockingAngleTolerance;
    jsonXcConfigXc["lXcSPRotationThreshold"] = xcConfigXc.lXcSPRotationThreshold;
    jsonXcConfigXc["lXcClockwiseOuterEdgeAngle"] = xcConfigXc.lXcClockwiseOuterEdgeAngle;
    jsonXcConfigXc["lXcCounterclockwiseOuterEdgeAngle"] = xcConfigXc.lXcCounterclockwiseOuterEdgeAngle;
    jsonXcConfigXc["lXcClockwiseClearanceAngle"] = xcConfigXc.lXcClockwiseClearanceAngle;
    jsonXcConfigXc["lXcCounterclockwiseClearanceAngle"] = xcConfigXc.lXcCounterclockwiseClearanceAngle;
    jsonXcConfigXc["lXcSAREVelocityIntegrationTime"] = xcConfigXc.lXcSAREVelocityIntegrationTime;
    jsonXcConfigXc["lXcSARE_MultiturnInitialValue"] = xcConfigXc.lXcSARE_MultiturnInitialValue;
    jsonXcConfigXc["lXcSARE_ZeroDegreePosition"] = xcConfigXc.lXcSARE_ZeroDegreePosition;
    jsonXcConfigXc["lXcSARE_NinetyDegreePosition"] = xcConfigXc.lXcSARE_NinetyDegreePosition;
    jsonXcConfigXc["lXcAngularSpeedMax"] = xcConfigXc.lXcAngularSpeedMax;
    jsonXcConfigXc["lXcAngularSpeedInManualOpModeMax"] = xcConfigXc.lXcAngularSpeedInManualOpModeMax;
    jsonXcConfigXc["lXC_MAR_BoundaryClockwise"] = xcConfigXc.lXC_MAR_BoundaryClockwise;
    jsonXcConfigXc["lXC_MAR_BoundaryCounterclockwise"] = xcConfigXc.lXC_MAR_BoundaryCounterclockwise;

    return jsonXcConfigXc;
}

// Function to convert xcCfgSr_t to JSON
json to_json(const xcCfgSr_t& xcConfigSr) {
    json jsonXcConfigSr;

    jsonXcConfigSr["lSCTS_MaxErrorTime"] = xcConfigSr.lSCTS_MaxErrorTime;
    jsonXcConfigSr["lSCTS_ErrorOverwriteErrorTemp"] = xcConfigSr.lSCTS_ErrorOverwriteErrorTemp;
    jsonXcConfigSr["lSCTS_ErrorOverwriteWarnTemp"] = xcConfigSr.lSCTS_ErrorOverwriteWarnTemp;
    jsonXcConfigSr["lTCBSS_MaxBlockActuationTime"] = xcConfigSr.lTCBSS_MaxBlockActuationTime;
    jsonXcConfigSr["lTCBSS_MaxUnblockActuationTime"] = xcConfigSr.lTCBSS_MaxUnblockActuationTime;
    jsonXcConfigSr["lTCLSS_MaxCabinLockingTime"] = xcConfigSr.lTCLSS_MaxCabinLockingTime;
    jsonXcConfigSr["lTCLSS_LockingStatusUndefinedMaxTime"] = xcConfigSr.lTCLSS_LockingStatusUndefinedMaxTime;
    jsonXcConfigSr["lTCLSS_MaxCabinUnlockingTime"] = xcConfigSr.lTCLSS_MaxCabinUnlockingTime;
    jsonXcConfigSr["lTCLSS_MaxUndefinedTime"] = xcConfigSr.lTCLSS_MaxUndefinedTime;
    jsonXcConfigSr["lXCL_MaxLockActuationTime"] = xcConfigSr.lXCL_MaxLockActuationTime;
    jsonXcConfigSr["lXCL_MaxUnlockActuationTime"] = xcConfigSr.lXCL_MaxUnlockActuationTime;
    jsonXcConfigSr["lXCL_MaxBlockActuationTime"] = xcConfigSr.lXCL_MaxBlockActuationTime;
    jsonXcConfigSr["lXCL_MaxUnblockActuationTime"] = xcConfigSr.lXCL_MaxUnblockActuationTime;
    jsonXcConfigSr["lXCL_MaxUndefinedTime"] = xcConfigSr.lXCL_MaxUndefinedTime;
    jsonXcConfigSr["lXCGBSS_MaxMaintenanceTime"] = xcConfigSr.lXCGBSS_MaxMaintenanceTime;
    jsonXcConfigSr["lXCGBSS_MaxAllowedReleaseTime"] = xcConfigSr.lXCGBSS_MaxAllowedReleaseTime;
    jsonXcConfigSr["lXCGBSS_MaxAllowedActivationTime"] = xcConfigSr.lXCGBSS_MaxAllowedActivationTime;
    jsonXcConfigSr["lXCSARE_MaxFailureTime"] = xcConfigSr.lXCSARE_MaxFailureTime;
    jsonXcConfigSr["lXCSN_WarnTemp"] = xcConfigSr.lXCSN_WarnTemp;
    jsonXcConfigSr["lXCISTO_ActuationTimeDuration"] = xcConfigSr.lXCISTO_ActuationTimeDuration;
    jsonXcConfigSr["lXCISTO_DeActuationTimeDuration"] = xcConfigSr.lXCISTO_DeActuationTimeDuration;

    /*jsonSrConfig["usXC_Ssr_Enable"] = json::array();
    for (int i = 0; i < NUMBER_OF_XC_SSR_ON_SN; ++i) {
        jsonSrConfig["usXC_Ssr_Enable"].push_back(srConfig.usXC_Ssr_Enable[i]);
    }*/

    for (const auto& srEnable : xcConfigSr.usXCSN_Ssr_Enable) {
        jsonXcConfigSr["usXCSN_Ssr_Enable"].push_back(srEnable);
    }

    return jsonXcConfigSr;
}

// Function to convert xcCfgConstants_t to JSON
json to_json(const xcCfgConstants_t& xcConfigConstants) {
    json jsonXcConfigConstants;

    jsonXcConfigConstants["tConfigVersion"] = xcConfigConstants.tConfigVersion;
    jsonXcConfigConstants["ulConfigJobSpecificId"] = xcConfigConstants.ulConfigJobSpecificId;

    return jsonXcConfigConstants;
}

// Function to convert xcCfg_t to JSON
json to_json(const xcCfg_t& xcConfig) {
    json jsonXcConfig;

    jsonXcConfig["ulCrc"] = xcConfig.ulCrc;
    jsonXcConfig["tConstants"] = to_json(xcConfig.tConstants);
    jsonXcConfig["tXcId"] = xcConfig.tXcId;
    jsonXcConfig["Spare1"] = xcConfig.Spare1;
    jsonXcConfig["tSr"] = to_json(xcConfig.tSr);
    jsonXcConfig["tXc"] = to_json(xcConfig.tXc);

    for (const auto& sfEnable : xcConfig.usXCSN_Sf_Enable) {
        jsonXcConfig["usXCSN_Sf_Enable"].push_back(sfEnable);
    }

    return jsonXcConfig;
}


// Function to convert iocCfgConstants_t to JSON
json to_json(const iocCfgConstants_t& iocConfigConstants) {
    json jsonIocConfigConstants;

    jsonIocConfigConstants["tConfigVersion"] = iocConfigConstants.tConfigVersion;
    jsonIocConfigConstants["ucSpare"] = iocConfigConstants.ucSpare;

    return jsonIocConfigConstants;
}

// Function to convert iocCfgIoc_t to JSON
json to_json(const iocCfgIoc_t& iocConfigIoc) {
    json jsonIocConfigIoc;

    jsonIocConfigIoc["tDeviceMappingTable"] = to_json(iocConfigIoc.tDeviceMappingTable);

    return jsonIocConfigIoc;
}


// Function to convert iocCfgSr_t to JSON
json to_json(const iocCfgSr_t& iocConfigSr) {
    json jsonIocConfigSr;

    jsonIocConfigSr["lSCTS_MaxErrorTime"] = iocConfigSr.lSCTS_MaxErrorTime;
    jsonIocConfigSr["lUDSS_MaxAllowedUnderpowerTime"] = iocConfigSr.lUDSS_MaxAllowedUnderpowerTime;
    jsonIocConfigSr["lWTSS_MaxInvalidTime"] = iocConfigSr.lWTSS_MaxInvalidTime;
    jsonIocConfigSr["lWTSS_OverwriteWarnTemp"] = iocConfigSr.lWTSS_OverwriteWarnTemp;
    jsonIocConfigSr["lWTSS_OverwriteErrTemp"] = iocConfigSr.lWTSS_OverwriteErrTemp;
    jsonIocConfigSr["lWTSS_MaxDiffValue"] = iocConfigSr.lWTSS_MaxDiffValue;
    jsonIocConfigSr["usIOCSN_Ssr_Enable"] = iocConfigSr.usIOCSN_Ssr_Enable;

    return jsonIocConfigSr;
}

// Function to convert iocCfgWell_t to JSON
json to_json(const iocCfgWell_t& iocConfigWell) {
    json jsonIocConfigWell;

    jsonIocConfigWell["lWell_AmbientTempMax"] = iocConfigWell.lWell_AmbientTempMax;
    jsonIocConfigWell["lWell_AmbientTempMaxHyst"] = iocConfigWell.lWell_AmbientTempMaxHyst;
    jsonIocConfigWell["lWell_AmbientTempMin"] = iocConfigWell.lWell_AmbientTempMin;

    return jsonIocConfigWell;
}

// Function to convert iocCfg_t to JSON
json to_json(const iocCfg_t& iocConfig) {
    json jsonIocConfig;

    jsonIocConfig["ulCrc"] = iocConfig.ulCrc;
    jsonIocConfig["tConstants"] = to_json(iocConfig.tConstants);
    jsonIocConfig["tIocId"] = iocConfig.tIocId;
    jsonIocConfig["ucSpare2"] = iocConfig.ucSpare2;
    jsonIocConfig["tIoc"] = to_json(iocConfig.tIoc);
    jsonIocConfig["tSr"] = to_json(iocConfig.tSr);
    jsonIocConfig["tWell"] = to_json(iocConfig.tWell);
    jsonIocConfig["usIOCSN_Sf_Enable"] = iocConfig.usIOCSN_Sf_Enable;

    return jsonIocConfig;
}

// Function to convert lcuCfgConstants_t to JSON
json to_json(const lcuCfgConstants_t& lcuConfigConstants) {
    json jsonLcuConfigConstants;

    jsonLcuConfigConstants["lMULTI_NumberOfXCInstalled"] = lcuConfigConstants.lMULTI_NumberOfXCInstalled;
    jsonLcuConfigConstants["lMULTI_NumberOfHorizontalTracksInstalled"] = lcuConfigConstants.lMULTI_NumberOfHorizontalTracksInstalled;
    jsonLcuConfigConstants["lMULTI_NumberOfVerticalTracksInstalled"] = lcuConfigConstants.lMULTI_NumberOfVerticalTracksInstalled;
    jsonLcuConfigConstants["lMULTI_NumberOfGarageAreasInstalled"] = lcuConfigConstants.lMULTI_NumberOfGarageAreasInstalled;
    jsonLcuConfigConstants["lMULTI_NumberOfMaintenanceAreasInstalled"] = lcuConfigConstants.lMULTI_NumberOfMaintenanceAreasInstalled;
    jsonLcuConfigConstants["lMULTI_NumberOfWellDoorsInstalled"] = lcuConfigConstants.lMULTI_NumberOfWellDoorsInstalled;
    jsonLcuConfigConstants["lMULTI_NumberOfWellAccessPortsInstalled"] = lcuConfigConstants.lMULTI_NumberOfWellAccessPortsInstalled;
    jsonLcuConfigConstants["tConfigVersion"] = lcuConfigConstants.tConfigVersion;
    jsonLcuConfigConstants["ucSpare"] = lcuConfigConstants.ucSpare;
    jsonLcuConfigConstants["ulConfigJobSpecificId"] = lcuConfigConstants.ulConfigJobSpecificId;
    jsonLcuConfigConstants["lLutFn_NumberOfEntriesMax"] = lcuConfigConstants.lLutFn_NumberOfEntriesMax;
    jsonLcuConfigConstants["lLutXtarget_NumberOfEntriesMax"] = lcuConfigConstants.lLutXtarget_NumberOfEntriesMax;
    jsonLcuConfigConstants["lLCUBrake_CalibrationCountRisingThreshold"] = lcuConfigConstants.lLCUBrake_CalibrationCountRisingThreshold;
    jsonLcuConfigConstants["lLCUBrake_CalibrationCountFallingThreshold"] = lcuConfigConstants.lLCUBrake_CalibrationCountFallingThreshold;
    jsonLcuConfigConstants["lLCUBrake_CalibrationDeltaP"] = lcuConfigConstants.lLCUBrake_CalibrationDeltaP;

    return jsonLcuConfigConstants;
}

// Function to convert collectiveLoadMax_t to JSON
json to_json(const collectiveLoadMax_t& collectiveLoadMax) {
    json jsonCollectiveLoadMax;

    jsonCollectiveLoadMax["usCASVerticalNrOfEvents"] = collectiveLoadMax.usCASVerticalNrOfEvents;
    jsonCollectiveLoadMax["usCASHorizontalNrOfEvents"] = collectiveLoadMax.usCASHorizontalNrOfEvents;
    jsonCollectiveLoadMax["usECASVerticalNrOfEvents"] = collectiveLoadMax.usECASVerticalNrOfEvents;
    jsonCollectiveLoadMax["usCESBrakingVerticalNrOfEvents"] = collectiveLoadMax.usCESBrakingVerticalNrOfEvents;
    jsonCollectiveLoadMax["usCESBrakingHorizontalNrOfEvents"] = collectiveLoadMax.usCESBrakingHorizontalNrOfEvents;
    jsonCollectiveLoadMax["usFESBrakingExtremeVerticalNrOfEvents"] = collectiveLoadMax.usFESBrakingExtremeVerticalNrOfEvents;
    jsonCollectiveLoadMax["usFESBrakingVerticalNrOfEvents"] = collectiveLoadMax.usFESBrakingVerticalNrOfEvents;
    jsonCollectiveLoadMax["usFESBrakingHorizontalNrOfEvents"] = collectiveLoadMax.usFESBrakingHorizontalNrOfEvents;
    jsonCollectiveLoadMax["usOverloadNrOfEvents"] = collectiveLoadMax.usOverloadNrOfEvents;
    jsonCollectiveLoadMax["usCrushloadNrOfEvents"] = collectiveLoadMax.usCrushloadNrOfEvents;
    jsonCollectiveLoadMax["usJumpingPassengerNrOfEvents"] = collectiveLoadMax.usJumpingPassengerNrOfEvents;
    jsonCollectiveLoadMax["ulPayloadChangeNrOfEvents"] = collectiveLoadMax.ulPayloadChangeNrOfEvents;
    jsonCollectiveLoadMax["usLcuTooFastInBufferAreaNrOfEvents"] = collectiveLoadMax.usLcuTooFastInBufferAreaNrOfEvents;
    jsonCollectiveLoadMax["ulBrk1ActivationWhenStoppedNrOfEvents"] = collectiveLoadMax.ulBrk1ActivationWhenStoppedNrOfEvents;
    jsonCollectiveLoadMax["ulBrk2ActivationWhenStoppedNrOfEvents"] = collectiveLoadMax.ulBrk2ActivationWhenStoppedNrOfEvents;
    jsonCollectiveLoadMax["ulBrk3ActivationWhenStoppedNrOfEvents"] = collectiveLoadMax.ulBrk3ActivationWhenStoppedNrOfEvents;
    jsonCollectiveLoadMax["ulBrk4ActivationWhenStoppedNrOfEvents"] = collectiveLoadMax.ulBrk4ActivationWhenStoppedNrOfEvents;
    jsonCollectiveLoadMax["usBrk1ActivationWhenMovingUpNrOfEvents"] = collectiveLoadMax.usBrk1ActivationWhenMovingUpNrOfEvents;
    jsonCollectiveLoadMax["usBrk2ActivationWhenMovingUpNrOfEvents"] = collectiveLoadMax.usBrk2ActivationWhenMovingUpNrOfEvents;
    jsonCollectiveLoadMax["usBrk3ActivationWhenMovingUpNrOfEvents"] = collectiveLoadMax.usBrk3ActivationWhenMovingUpNrOfEvents;
    jsonCollectiveLoadMax["usBrk4ActivationWhenMovingUpNrOfEvents"] = collectiveLoadMax.usBrk4ActivationWhenMovingUpNrOfEvents;
    jsonCollectiveLoadMax["usCESBrakingBrk1NrOfEvents"] = collectiveLoadMax.usCESBrakingBrk1NrOfEvents;
    jsonCollectiveLoadMax["usCESBrakingBrk2NrOfEvents"] = collectiveLoadMax.usCESBrakingBrk2NrOfEvents;
    jsonCollectiveLoadMax["usCESBrakingBrk3NrOfEvents"] = collectiveLoadMax.usCESBrakingBrk3NrOfEvents;
    jsonCollectiveLoadMax["usCESBrakingBrk4NrOfEvents"] = collectiveLoadMax.usCESBrakingBrk4NrOfEvents;
    jsonCollectiveLoadMax["usFESBrakingBrk1NrOfEvents"] = collectiveLoadMax.usFESBrakingBrk1NrOfEvents;
    jsonCollectiveLoadMax["usFESBrakingBrk2NrOfEvents"] = collectiveLoadMax.usFESBrakingBrk2NrOfEvents;
    jsonCollectiveLoadMax["usFESBrakingBrk3NrOfEvents"] = collectiveLoadMax.usFESBrakingBrk3NrOfEvents;
    jsonCollectiveLoadMax["usFESBrakingBrk4NrOfEvents"] = collectiveLoadMax.usFESBrakingBrk4NrOfEvents;

    return jsonCollectiveLoadMax;
}

// Define a to_json function for lcuCfgApsPosition_t
json to_json(const lcuCfgApsPosition_t& apsPosition) {
    json jsonApsPosition;

    jsonApsPosition["ucCodeTapeNumber"] = apsPosition.ucCodeTapeNumber;
    jsonApsPosition["spare"] = nlohmann::json::array();
    for (const auto& value : apsPosition.spare) {
        jsonApsPosition["spare"].push_back(value);
    }

    jsonApsPosition["ulRawCodeTapeLBLeft"] = apsPosition.ulRawCodeTapeLBLeft;
    jsonApsPosition["ulRawCodeTapeUBLeft"] = apsPosition.ulRawCodeTapeUBLeft;
    jsonApsPosition["ulRawCodeTapeLBRight"] = apsPosition.ulRawCodeTapeLBRight;
    jsonApsPosition["ulRawCodeTapeUBRight"] = apsPosition.ulRawCodeTapeUBRight;
    jsonApsPosition["ulRawCodeTapeLBLeft_Z"] = apsPosition.ulRawCodeTapeLBLeft_Z;
    jsonApsPosition["ulRawCodeTapeUBLeft_Z"] = apsPosition.ulRawCodeTapeUBLeft_Z;
    jsonApsPosition["ulRawCodeTapeLBRight_Z"] = apsPosition.ulRawCodeTapeLBRight_Z;
    jsonApsPosition["ulRawCodeTapeUBRight_Z"] = apsPosition.ulRawCodeTapeUBRight_Z;

    return jsonApsPosition;
}

// Function to convert lcuBrakeLut_t to JSON
json to_json(const lcuBrakeLut_t& lcuBrakeLut) {
    json jsonLcuBrakeLut;

    jsonLcuBrakeLut["sArg"] = lcuBrakeLut.sArg;
    jsonLcuBrakeLut["sValue"] = lcuBrakeLut.sValue;

    return jsonLcuBrakeLut;
}

// Function to convert lcuCfgLcu_t to JSON
json to_json(const lcuCfgLcu_t& lcuCfgLcu) {
    nlohmann::json jsonLcuCfgLcu;

    // Convert tPhysicalConstraints array
    jsonLcuCfgLcu["tPhysicalConstraints"] = nlohmann::json::array();
    for (const auto& physicalConstraints : lcuCfgLcu.tPhysicalConstraints) {
        jsonLcuCfgLcu["tPhysicalConstraints"].push_back(to_json(physicalConstraints));
    }

    // Add scalar fields
    jsonLcuCfgLcu["lParkingBrakeForceThreshold"] = lcuCfgLcu.lParkingBrakeForceThreshold;
    jsonLcuCfgLcu["lSpeedHorizMin"] = lcuCfgLcu.lSpeedHorizMin;
    jsonLcuCfgLcu["lSpeedVertMin"] = lcuCfgLcu.lSpeedVertMin;
    jsonLcuCfgLcu["lSpeedHorizNominal"] = lcuCfgLcu.lSpeedHorizNominal;
    jsonLcuCfgLcu["lSpeedVertNominal"] = lcuCfgLcu.lSpeedVertNominal;
    jsonLcuCfgLcu["lUpAccelNominal"] = lcuCfgLcu.lUpAccelNominal;
    jsonLcuCfgLcu["lDownAccelNominal"] = lcuCfgLcu.lDownAccelNominal;
    jsonLcuCfgLcu["lHorizAccelNominal"] = lcuCfgLcu.lHorizAccelNominal;
    jsonLcuCfgLcu["lSpeedUnloadedHorizMax"] = lcuCfgLcu.lSpeedUnloadedHorizMax;
    jsonLcuCfgLcu["lSpeedLoadedHorizMax"] = lcuCfgLcu.lSpeedLoadedHorizMax;
    jsonLcuCfgLcu["lSpeedVertMax"] = lcuCfgLcu.lSpeedVertMax;
    jsonLcuCfgLcu["lSpeedInManualOpHorizMax"] = lcuCfgLcu.lSpeedInManualOpHorizMax;
    jsonLcuCfgLcu["lSpeedInManualOpVertMax"] = lcuCfgLcu.lSpeedInManualOpVertMax;
    jsonLcuCfgLcu["lSlidingBrakeSpeedMax"] = lcuCfgLcu.lSlidingBrakeSpeedMax;
    jsonLcuCfgLcu["lEmergencyAutonomVertDecelMin"] = lcuCfgLcu.lEmergencyAutonomVertDecelMin;
    jsonLcuCfgLcu["lEmergencyAutonomVertDecelMax"] = lcuCfgLcu.lEmergencyAutonomVertDecelMax;
    jsonLcuCfgLcu["lNormalAutonomHorizDecelMin"] = lcuCfgLcu.lNormalAutonomHorizDecelMin;
    jsonLcuCfgLcu["lNormalAutonomHorizDecelMax"] = lcuCfgLcu.lNormalAutonomHorizDecelMax;
    jsonLcuCfgLcu["lNormalAutonomUpDecelMin"] = lcuCfgLcu.lNormalAutonomUpDecelMin;
    jsonLcuCfgLcu["lNormalAutonomUpDecelMax"] = lcuCfgLcu.lNormalAutonomUpDecelMax;
    jsonLcuCfgLcu["lNormalAutonomDownDecelMin"] = lcuCfgLcu.lNormalAutonomDownDecelMin;
    jsonLcuCfgLcu["lNormalAutonomDownDecelMax"] = lcuCfgLcu.lNormalAutonomDownDecelMax;
    jsonLcuCfgLcu["lControlledHorizDecelMin"] = lcuCfgLcu.lControlledHorizDecelMin;
    jsonLcuCfgLcu["lControlledHorizDecelModerate"] = lcuCfgLcu.lControlledHorizDecelModerate;
    jsonLcuCfgLcu["lControlledDownDecelModerate"] = lcuCfgLcu.lControlledDownDecelModerate;
    jsonLcuCfgLcu["lControlledDownDecelMin"] = lcuCfgLcu.lControlledDownDecelMin;
    jsonLcuCfgLcu["lForcedHorizDecelMin"] = lcuCfgLcu.lForcedHorizDecelMin;
    jsonLcuCfgLcu["lForcedDownDecelMin"] = lcuCfgLcu.lForcedDownDecelMin;
    jsonLcuCfgLcu["lPayloadDetectionThreshold"] = lcuCfgLcu.lPayloadDetectionThreshold;
    jsonLcuCfgLcu["lUpAccelMax"] = lcuCfgLcu.lUpAccelMax;
    jsonLcuCfgLcu["lDownAccelMax"] = lcuCfgLcu.lDownAccelMax;
    jsonLcuCfgLcu["lHorizAccelMax"] = lcuCfgLcu.lHorizAccelMax;
    jsonLcuCfgLcu["lUpDecelMax"] = lcuCfgLcu.lUpDecelMax;
    jsonLcuCfgLcu["lDownDecelMax"] = lcuCfgLcu.lDownDecelMax;
    jsonLcuCfgLcu["lHorizDecelMax"] = lcuCfgLcu.lHorizDecelMax;
    jsonLcuCfgLcu["lAps_maxAccelDeviation"] = lcuCfgLcu.lAps_maxAccelDeviation;
    jsonLcuCfgLcu["lAps_gravityOffset_1A"] = lcuCfgLcu.lAps_gravityOffset_1A;
    jsonLcuCfgLcu["lAps_gravityOffset_2A"] = lcuCfgLcu.lAps_gravityOffset_2A;
    jsonLcuCfgLcu["lAps_gravityOffset_1B"] = lcuCfgLcu.lAps_gravityOffset_1B;
    jsonLcuCfgLcu["lAps_gravityOffset_2B"] = lcuCfgLcu.lAps_gravityOffset_2B;
    jsonLcuCfgLcu["lAps_AccSensorOffset"] = lcuCfgLcu.lAps_AccSensorOffset;
    jsonLcuCfgLcu["lVelocityEcasPreBrakeMax"] = lcuCfgLcu.lVelocityEcasPreBrakeMax;
    jsonLcuCfgLcu["lVelocityNcasPreBrakeMax"] = lcuCfgLcu.lVelocityNcasPreBrakeMax;
    jsonLcuCfgLcu["lVelocitySfoPreBrakeMax"] = lcuCfgLcu.lVelocitySfoPreBrakeMax;
    jsonLcuCfgLcu["lVelocityNcasMin"] = lcuCfgLcu.lVelocityNcasMin;
    jsonLcuCfgLcu["lVelocityEcasMin"] = lcuCfgLcu.lVelocityEcasMin;
    jsonLcuCfgLcu["lVelocityFES"] = lcuCfgLcu.lVelocityFES;
    jsonLcuCfgLcu["lNormalCasActuationTime"] = lcuCfgLcu.lNormalCasActuationTime;
    jsonLcuCfgLcu["lEmergencyCasActuationTime"] = lcuCfgLcu.lEmergencyCasActuationTime;
    jsonLcuCfgLcu["lControlledPrimaryBrakeActuationTime"] = lcuCfgLcu.lControlledPrimaryBrakeActuationTime;
    jsonLcuCfgLcu["lForcedPrimaryBrakeActuationTime"] = lcuCfgLcu.lForcedPrimaryBrakeActuationTime;
    jsonLcuCfgLcu["lForcedRedundantBrakeActuationTime"] = lcuCfgLcu.lForcedRedundantBrakeActuationTime;
    jsonLcuCfgLcu["lSfoActuationTime"] = lcuCfgLcu.lSfoActuationTime;

    jsonLcuCfgLcu["lLCU_LoadBrakeCollectiveNextEventCountTime"] = lcuCfgLcu.lLCU_LoadBrakeCollectiveNextEventCountTime;
    jsonLcuCfgLcu["lLCU_LoadBrakeCollectiveDebouncingCyclesTime"] = lcuCfgLcu.lLCU_LoadBrakeCollectiveDebouncingCyclesTime;
    jsonLcuCfgLcu["lPBS_SafetyValvesDeEnergizedTime"] = lcuCfgLcu.lPBS_SafetyValvesDeEnergizedTime;

    // Convert tCollectiveLoadMax structure
    jsonLcuCfgLcu["tCollectiveLoadMax"] = to_json(lcuCfgLcu.tCollectiveLoadMax);

    jsonLcuCfgLcu["lOverloadThreshold"] = lcuCfgLcu.lOverloadThreshold;
    jsonLcuCfgLcu["lCrushloadThreshold"] = lcuCfgLcu.lCrushloadThreshold;
    jsonLcuCfgLcu["lPayload75kg"] = lcuCfgLcu.lPayload75kg;
    jsonLcuCfgLcu["lPayload225kg"] = lcuCfgLcu.lPayload225kg;
    jsonLcuCfgLcu["lPayload375kg"] = lcuCfgLcu.lPayload375kg;
    jsonLcuCfgLcu["lPayload525kg"] = lcuCfgLcu.lPayload525kg;
    jsonLcuCfgLcu["lPayload675kg"] = lcuCfgLcu.lPayload675kg;
    jsonLcuCfgLcu["lPayload825kg"] = lcuCfgLcu.lPayload825kg;
    jsonLcuCfgLcu["lPayloadMax_EconomyLCU"] = lcuCfgLcu.lPayloadMax_EconomyLCU;
    jsonLcuCfgLcu["lPayloadMax_PremiumLCU"] = lcuCfgLcu.lPayloadMax_PremiumLCU;
    jsonLcuCfgLcu["lPayloadMaxHyst"] = lcuCfgLcu.lPayloadMaxHyst;
    jsonLcuCfgLcu["lPayloadCrushed"] = lcuCfgLcu.lPayloadCrushed;
    jsonLcuCfgLcu["lPayloadCrushedHyst"] = lcuCfgLcu.lPayloadCrushedHyst;
    jsonLcuCfgLcu["lLoadedWeightMax"] = lcuCfgLcu.lLoadedWeightMax;
    jsonLcuCfgLcu["lLoadedWeightMaxHyst"] = lcuCfgLcu.lLoadedWeightMaxHyst;
    jsonLcuCfgLcu["lEmptyWeightMax_EconomyLCU"] = lcuCfgLcu.lEmptyWeightMax_EconomyLCU;
    jsonLcuCfgLcu["lEmptyWeightMax_PremiumLCU"] = lcuCfgLcu.lEmptyWeightMax_PremiumLCU;
    jsonLcuCfgLcu["lAmbientTempMax"] = lcuCfgLcu.lAmbientTempMax;
    jsonLcuCfgLcu["lAmbientTempMaxHyst"] = lcuCfgLcu.lAmbientTempMaxHyst;
    jsonLcuCfgLcu["lAvgEmptyWeightDigit_EconomyLCU"] = lcuCfgLcu.lAvgEmptyWeightDigit_EconomyLCU;
    jsonLcuCfgLcu["lAvgEmptyWeightDigit_PremiumLCU"] = lcuCfgLcu.lAvgEmptyWeightDigit_PremiumLCU;
    jsonLcuCfgLcu["lAvgEmptyWeightKg_EconomyLCU"] = lcuCfgLcu.lAvgEmptyWeightKg_EconomyLCU;
    jsonLcuCfgLcu["lAvgEmptyWeightKg_PremiumLCU"] = lcuCfgLcu.lAvgEmptyWeightKg_PremiumLCU;
    jsonLcuCfgLcu["lAvgOverloadWeightDigit_EconomyLCU"] = lcuCfgLcu.lAvgOverloadWeightDigit_EconomyLCU;
    jsonLcuCfgLcu["lAvgOverloadWeightDigit_PremiumLCU"] = lcuCfgLcu.lAvgOverloadWeightDigit_PremiumLCU;
    jsonLcuCfgLcu["lAvgOverloadWeightKg_EconomyLCU"] = lcuCfgLcu.lAvgOverloadWeightKg_EconomyLCU;
    jsonLcuCfgLcu["lAvgOverloadWeightKg_PremiumLCU"] = lcuCfgLcu.lAvgOverloadWeightKg_PremiumLCU;

    jsonLcuCfgLcu["lTargetDeceleration_Horizontal"] = lcuCfgLcu.lTargetDeceleration_Horizontal;
    jsonLcuCfgLcu["lTargetDeceleration_Down"] = lcuCfgLcu.lTargetDeceleration_Down;

    jsonLcuCfgLcu["lPrimaryBrakeStepFullActivationThreshold"] = lcuCfgLcu.lPrimaryBrakeStepFullActivationThreshold;
    jsonLcuCfgLcu["lBrakePositionOpenThreshold"] = lcuCfgLcu.lBrakePositionOpenThreshold;

    // Convert lBrakeStepActiveHydraulicPressureThreshold array
    jsonLcuCfgLcu["lBrakeStepActiveHydraulicPressureThreshold"] = nlohmann::json::array();
    for (const auto& pressureThreshold : lcuCfgLcu.lBrakeStepActiveHydraulicPressureThreshold) {
        jsonLcuCfgLcu["lBrakeStepActiveHydraulicPressureThreshold"].push_back(pressureThreshold);
    }

    // Convert tStoppingDistanceParamSet structure
    jsonLcuCfgLcu["tStoppingDistanceParamSet"] = to_json(lcuCfgLcu.tStoppingDistanceParamSet);

    jsonLcuCfgLcu["cType"] = lcuCfgLcu.cType;

    // Convert cPositionTranslation_TrackIsVertical array
    jsonLcuCfgLcu["cPositionTranslation_TrackIsVertical"] = nlohmann::json::array();
    for (const auto& trackIsVertical : lcuCfgLcu.cPositionTranslation_TrackIsVertical) {
        jsonLcuCfgLcu["cPositionTranslation_TrackIsVertical"].push_back(trackIsVertical);
    }

    // Convert lPositionTranslation_CenterAlignmentPosAlongTrack array
    jsonLcuCfgLcu["lPositionTranslation_CenterAlignmentPosAlongTrack"] = nlohmann::json::array();
    for (const auto& alignmentPos : lcuCfgLcu.lPositionTranslation_CenterAlignmentPosAlongTrack) {
        jsonLcuCfgLcu["lPositionTranslation_CenterAlignmentPosAlongTrack"].push_back(alignmentPos);
    }

    jsonLcuCfgLcu["lAPS_1A_Offset_Z_left"] = lcuCfgLcu.lAPS_1A_Offset_Z_left;
    jsonLcuCfgLcu["lAPS_1A_Offset_Y_left"] = lcuCfgLcu.lAPS_1A_Offset_Y_left;
    jsonLcuCfgLcu["lAPS_1B_Offset_Z_left"] = lcuCfgLcu.lAPS_1B_Offset_Z_left;
    jsonLcuCfgLcu["lAPS_1B_Offset_Y_left"] = lcuCfgLcu.lAPS_1B_Offset_Y_left;
    jsonLcuCfgLcu["lAPS_2A_Offset_Z_right"] = lcuCfgLcu.lAPS_2A_Offset_Z_right;
    jsonLcuCfgLcu["lAPS_2A_Offset_Y_right"] = lcuCfgLcu.lAPS_2A_Offset_Y_right;
    jsonLcuCfgLcu["lAPS_2B_Offset_Z_right"] = lcuCfgLcu.lAPS_2B_Offset_Z_right;
    jsonLcuCfgLcu["lAPS_2B_Offset_Y_right"] = lcuCfgLcu.lAPS_2B_Offset_Y_right;

    jsonLcuCfgLcu["lLCUBrake_x_c_trg_min_area_of_evaluation_parameter"] = lcuCfgLcu.lLCUBrake_x_c_trg_min_area_of_evaluation_parameter;
    jsonLcuCfgLcu["lLCUBrake_x_c_trg_max_area_of_evaluation_parameter"] = lcuCfgLcu.lLCUBrake_x_c_trg_max_area_of_evaluation_parameter;

    // Convert tApsPositionInfo array
    jsonLcuCfgLcu["tApsPositionInfo"] = nlohmann::json::array();
    for (const auto& apsPositionInfo : lcuCfgLcu.tApsPositionInfo) {
        jsonLcuCfgLcu["tApsPositionInfo"].push_back(to_json(apsPositionInfo));
    }

    jsonLcuCfgLcu["ulMULTI_NumberOfCodeTapeInstalled"] = lcuCfgLcu.ulMULTI_NumberOfCodeTapeInstalled;

    // Convert lLCUBrake_initialFnMax array
    jsonLcuCfgLcu["lLCUBrake_initialFnMax"] = nlohmann::json::array();
    for (const auto& initialFnMax : lcuCfgLcu.lLCUBrake_initialFnMax) {
        jsonLcuCfgLcu["lLCUBrake_initialFnMax"].push_back(initialFnMax);
    }

    // Convert lLCUBrake_initialPcontact array
    jsonLcuCfgLcu["lLCUBrake_initialPcontact"] = nlohmann::json::array();
    for (const auto& initialPcontact : lcuCfgLcu.lLCUBrake_initialPcontact) {
        jsonLcuCfgLcu["lLCUBrake_initialPcontact"].push_back(initialPcontact);
    }

    // Convert lLCUBrake_initialXcontact array
    jsonLcuCfgLcu["lLCUBrake_initialXcontact"] = nlohmann::json::array();
    for (const auto& initialXcontact : lcuCfgLcu.lLCUBrake_initialXcontact) {
        jsonLcuCfgLcu["lLCUBrake_initialXcontact"].push_back(initialXcontact);
    }

    // Convert lLCUBrake_initialPosition0 array
    jsonLcuCfgLcu["lLCUBrake_initialPosition0"] = nlohmann::json::array();
    for (const auto& initialPosition0 : lcuCfgLcu.lLCUBrake_initialPosition0) {
        jsonLcuCfgLcu["lLCUBrake_initialPosition0"].push_back(initialPosition0);
    }

    // Convert lLCUBrake_x_c_trg_max array
    jsonLcuCfgLcu["lLCUBrake_x_c_trg_max"] = nlohmann::json::array();
    for (const auto& x_c_trg_max : lcuCfgLcu.lLCUBrake_x_c_trg_max) {
        jsonLcuCfgLcu["lLCUBrake_x_c_trg_max"].push_back(x_c_trg_max);
    }

    // Convert lLCUBrake_x_c_trg_min array
    jsonLcuCfgLcu["lLCUBrake_x_c_trg_min"] = nlohmann::json::array();
    for (const auto& x_c_trg_min : lcuCfgLcu.lLCUBrake_x_c_trg_min) {
        jsonLcuCfgLcu["lLCUBrake_x_c_trg_min"].push_back(x_c_trg_min);
    }

    // Convert lLCUBrake_p_c_trg_min array
    jsonLcuCfgLcu["lLCUBrake_p_c_trg_min"] = nlohmann::json::array();
    for (const auto& p_c_trg_min : lcuCfgLcu.lLCUBrake_p_c_trg_min) {
        jsonLcuCfgLcu["lLCUBrake_p_c_trg_min"].push_back(p_c_trg_min);
    }

    // Convert lLCUBrake_p_c_trg_max array
    jsonLcuCfgLcu["lLCUBrake_p_c_trg_max"] = nlohmann::json::array();
    for (const auto& p_c_trg_max : lcuCfgLcu.lLCUBrake_p_c_trg_max) {
        jsonLcuCfgLcu["lLCUBrake_p_c_trg_max"].push_back(p_c_trg_max);
    }

    // Convert tLutFn array
    jsonLcuCfgLcu["tLutFn"] = nlohmann::json::array();
    for (const auto& lutFn : lcuCfgLcu.tLutFn) {
        jsonLcuCfgLcu["tLutFn"].push_back(to_json(lutFn));
    }

    // Convert tLutXtarget array
    jsonLcuCfgLcu["tLutXtarget"] = nlohmann::json::array();
    for (const auto& lutXtarget : lcuCfgLcu.tLutXtarget) {
        jsonLcuCfgLcu["tLutXtarget"].push_back(to_json(lutXtarget));
    }

    return jsonLcuCfgLcu;
}

// Implement to_json for lcuCfgProtectionAreas_t...
json to_json(const lcuCfgProtectionAreas_t& protectionAreas) {
    json jsonLcuCfgProtectionAreas;

    jsonLcuCfgProtectionAreas["tGarageAreas"] = json::array();
    for (const auto& garageArea : protectionAreas.tGarageAreas) {
        jsonLcuCfgProtectionAreas["tGarageAreas"].push_back(to_json(garageArea));
    }

    jsonLcuCfgProtectionAreas["tMaintenanceAreas"] = json::array();
    for (const auto& maintenanceArea : protectionAreas.tMaintenanceAreas) {
        jsonLcuCfgProtectionAreas["tMaintenanceAreas"].push_back(to_json(maintenanceArea));
    }

    jsonLcuCfgProtectionAreas["tTranslationalMovementAreas"] = json::array();
    for (const auto& translationalMovementArea : protectionAreas.tTranslationalMovementAreas) {
        jsonLcuCfgProtectionAreas["tTranslationalMovementAreas"].push_back(to_json(translationalMovementArea));
    }

    return jsonLcuCfgProtectionAreas;
}

nlohmann::json to_json(const lcuCfgSr_t& lcuCfgSr) {
    nlohmann::json jsonLcuCfgSr;

    jsonLcuCfgSr["lLCUAPS_MaxTravelDistanceWithCalculatedPosition"] = lcuCfgSr.lLCUAPS_MaxTravelDistanceWithCalculatedPosition;
    jsonLcuCfgSr["lLCUAPS_MaxTravelTimeWithCalculatedPosition"] = lcuCfgSr.lLCUAPS_MaxTravelTimeWithCalculatedPosition;
    jsonLcuCfgSr["lLCUAPS_MaxTravelTimeWithInjectedVelocity"] = lcuCfgSr.lLCUAPS_MaxTravelTimeWithInjectedVelocity;
    jsonLcuCfgSr["lLCUAPS_MinSpeedWithExtrapolation"] = lcuCfgSr.lLCUAPS_MinSpeedWithExtrapolation;
    jsonLcuCfgSr["lLCUBRK_WearoutLevelWarn"] = lcuCfgSr.lLCUBRK_WearoutLevelWarn;
    jsonLcuCfgSr["lLCUBRK_WearoutLevelError"] = lcuCfgSr.lLCUBRK_WearoutLevelError;
    jsonLcuCfgSr["lLCUBRK_FrictionLevelWarn"] = lcuCfgSr.lLCUBRK_FrictionLevelWarn;
    jsonLcuCfgSr["lLCUBRK_FrictionLevelError"] = lcuCfgSr.lLCUBRK_FrictionLevelError;
    jsonLcuCfgSr["lLCUBRK_StepForceDelta"] = lcuCfgSr.lLCUBRK_StepForceDelta;
    jsonLcuCfgSr["lLCUBRK_MaxRedundantBrakeFailureTime"] = lcuCfgSr.lLCUBRK_MaxRedundantBrakeFailureTime;
    jsonLcuCfgSr["lLCUBRK_MaxUpwardBrakeOpCount"] = lcuCfgSr.lLCUBRK_MaxUpwardBrakeOpCount;
    jsonLcuCfgSr["lLCUBRK_MaxPrimaryUBPLockingTime"] = lcuCfgSr.lLCUBRK_MaxPrimaryUBPLockingTime;
    jsonLcuCfgSr["lLCUBRK_MaxRedundantUBPLockingTime"] = lcuCfgSr.lLCUBRK_MaxRedundantUBPLockingTime;
    jsonLcuCfgSr["lLCUBRK_MaxPrimaryUBPUnlockingTime"] = lcuCfgSr.lLCUBRK_MaxPrimaryUBPUnlockingTime;
    jsonLcuCfgSr["lLCUBRK_MaxRedundantUBPUnlockingTime"] = lcuCfgSr.lLCUBRK_MaxRedundantUBPUnlockingTime;
    jsonLcuCfgSr["lLCUBSS_MaxBrakeSafetySensorFailureTime"] = lcuCfgSr.lLCUBSS_MaxBrakeSafetySensorFailureTime;
    jsonLcuCfgSr["lLCUBSS_MaxBrakeReleaseTimeWarn"] = lcuCfgSr.lLCUBSS_MaxBrakeReleaseTimeWarn;
    jsonLcuCfgSr["lLCUBSS_MaxBrakeReleaseTimeError"] = lcuCfgSr.lLCUBSS_MaxBrakeReleaseTimeError;
    jsonLcuCfgSr["lLCUBSS_MaxBrakeActuationTimeWarn"] = lcuCfgSr.lLCUBSS_MaxBrakeActuationTimeWarn;
    jsonLcuCfgSr["lLCUBSS_MaxBrakeActuationTimeError"] = lcuCfgSr.lLCUBSS_MaxBrakeActuationTimeError;
    jsonLcuCfgSr["lLCUBSS_dXtargetTolerance"] = lcuCfgSr.lLCUBSS_dXtargetTolerance;
    jsonLcuCfgSr["lLCUBSS_dXcontactTolerance"] = lcuCfgSr.lLCUBSS_dXcontactTolerance;
    jsonLcuCfgSr["lLCUBSS_PcontactMin"] = lcuCfgSr.lLCUBSS_PcontactMin;
    jsonLcuCfgSr["lLCUBSS_PcontactMax"] = lcuCfgSr.lLCUBSS_PcontactMax;
    jsonLcuCfgSr["lLCUCBSS_CabinLockingMaxBlockingTime"] = lcuCfgSr.lLCUCBSS_CabinLockingMaxBlockingTime;
    jsonLcuCfgSr["lLCUCBSS_CabinLockingMaxUnblockingTime"] = lcuCfgSr.lLCUCBSS_CabinLockingMaxUnblockingTime;
    jsonLcuCfgSr["lLCUCLSS_MaxLCUCabinLockingTime"] = lcuCfgSr.lLCUCLSS_MaxLCUCabinLockingTime;
    jsonLcuCfgSr["lLCUCLSS_MaxLCUCabinUnlockingTime"] = lcuCfgSr.lLCUCLSS_MaxLCUCabinUnlockingTime;
    jsonLcuCfgSr["lLCUCLSS_CabinLockingStatusUndefinedMaxTime"] = lcuCfgSr.lLCUCLSS_CabinLockingStatusUndefinedMaxTime;
    jsonLcuCfgSr["lLCUCTS_DiffOverwriteErrorTemp"] = lcuCfgSr.lLCUCTS_DiffOverwriteErrorTemp;
    jsonLcuCfgSr["lLCUCTS_DiffOverwriteWarnTemp"] = lcuCfgSr.lLCUCTS_DiffOverwriteWarnTemp;
    jsonLcuCfgSr["lLCUCTS_MaxDiffValue"] = lcuCfgSr.lLCUCTS_MaxDiffValue;
    jsonLcuCfgSr["lLCUCTS_MaxDiffTime"] = lcuCfgSr.lLCUCTS_MaxDiffTime;
    jsonLcuCfgSr["lLCUCTS_MaxZeroTime"] = lcuCfgSr.lLCUCTS_MaxZeroTime;
    jsonLcuCfgSr["lLCUCTS_ZeroOverwriteErrorTemp"] = lcuCfgSr.lLCUCTS_ZeroOverwriteErrorTemp;
    jsonLcuCfgSr["lLCUCTS_ZeroOverwriteWarnTemp"] = lcuCfgSr.lLCUCTS_ZeroOverwriteWarnTemp;
    jsonLcuCfgSr["lLCULW_MaxDiffValue"] = lcuCfgSr.lLCULW_MaxDiffValue;
    jsonLcuCfgSr["lLCULW_MinValue"] = lcuCfgSr.lLCULW_MinValue;
    jsonLcuCfgSr["lLCULW_MaxValue"] = lcuCfgSr.lLCULW_MaxValue;
    jsonLcuCfgSr["lLCULW_CrushLoadValue"] = lcuCfgSr.lLCULW_CrushLoadValue;
    jsonLcuCfgSr["lLCUSN_MaxTemperature"] = lcuCfgSr.lLCUSN_MaxTemperature;
    jsonLcuCfgSr["lLCUSN_PrimaryBrakeActiveWithoutCommandMaxTime"] = lcuCfgSr.lLCUSN_PrimaryBrakeActiveWithoutCommandMaxTime;
    jsonLcuCfgSr["lLCUSN_ReactionModuleSignalDelayTime"] = lcuCfgSr.lLCUSN_ReactionModuleSignalDelayTime;
    jsonLcuCfgSr["lSCTS_MaxErrorTime"] = lcuCfgSr.lSCTS_MaxErrorTime;
    jsonLcuCfgSr["lSCTS_ErrorOverwriteErrorTemp"] = lcuCfgSr.lSCTS_ErrorOverwriteErrorTemp;
    jsonLcuCfgSr["lSCTS_ErrorOverwriteWarnTemp"] = lcuCfgSr.lSCTS_ErrorOverwriteWarnTemp;

    // Assuming to_json function for array elements is available
    for (const auto& srEnable : lcuCfgSr.usLCUSN_Ssr_Enable) {
        jsonLcuCfgSr["usLCUSN_Ssr_Enable"].push_back(srEnable);
    }


    return jsonLcuCfgSr;
}

json to_json(const lcuCfgWellDoor_t& lcuCfgWellDoor) {
    json jsonLcuCfgWellDoor;

    jsonLcuCfgWellDoor["tWellDoorInfos"] = nlohmann::json::array();
    for (const auto& wellDoorInfo : lcuCfgWellDoor.tWellDoorInfos) {
        jsonLcuCfgWellDoor["tWellDoorInfos"].push_back(to_json(wellDoorInfo));
    }

    return jsonLcuCfgWellDoor;
}

json to_json(const lcuCfgWellAccessPort_t& lcuCfgWellAccessPort) {
    json jsonLcuCfgWellAccessPort;

    jsonLcuCfgWellAccessPort["tWellAccessPortInfos"] = nlohmann::json::array();
    for (const auto& wellAccessPortInfo : lcuCfgWellAccessPort.tWellAccessPortInfos) {
        jsonLcuCfgWellAccessPort["tWellAccessPortInfos"].push_back(to_json(wellAccessPortInfo));
    }

    return jsonLcuCfgWellAccessPort;
}


json to_json(const lcuCfgXc_t& lcuCfgXc) {
    nlohmann::json jsonLcuCfgXc;

    for (const auto& xcInfo : lcuCfgXc.tXcInfo) {
        jsonLcuCfgXc["tXcInfo"].push_back(to_json(xcInfo));
    }

    jsonLcuCfgXc["lXcMatchToleranceY"] = lcuCfgXc.lXcMatchToleranceY;
    jsonLcuCfgXc["lXcMatchToleranceZ"] = lcuCfgXc.lXcMatchToleranceZ;

    return jsonLcuCfgXc;
}

json to_json(const lcuCfg_t& lcuCfg) {
    json jsonLcuCfg;

    jsonLcuCfg["ulCrc"] = lcuCfg.ulCrc;
    jsonLcuCfg["tLcuId"] = lcuCfg.tLcuId;

    for (const auto& ucSpare : lcuCfg.ucSpare1) {
        jsonLcuCfg["ucSpare1"].push_back(ucSpare);
    }


    jsonLcuCfg["tConstants"] = to_json(lcuCfg.tConstants);
    jsonLcuCfg["tLcu"] = to_json(lcuCfg.tLcu);
    jsonLcuCfg["tProtectionAreas"] = to_json(lcuCfg.tProtectionAreas);
    jsonLcuCfg["tSr"] = to_json(lcuCfg.tSr);
    jsonLcuCfg["tWellDoor"] = to_json(lcuCfg.tWellDoor);
    jsonLcuCfg["tWellAccessPort"] = to_json(lcuCfg.tWellAccessPort);
    jsonLcuCfg["tXc"] = to_json(lcuCfg.tXc);

    // Assuming to_json function for array elements is available

    for (const auto& sfEnable : lcuCfg.usLCUSN_Sf_Enable) {
        jsonLcuCfg["usLCUSN_Sf_Enable"].push_back(sfEnable);
    }

    return jsonLcuCfg;
}