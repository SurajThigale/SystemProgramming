/** \addtogroup GenericCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgTypes.h
 *  @brief     Cfg FUB specific data types used throughout the system.
 *  @author    Frank Meier
 *
 */

#ifndef CFGTYPES_H_
#define CFGTYPES_H_

/*
 * INCLUDE FILES
 */

#include <stdint.h>

/*
 * MACRO DEFINITIONS
 */

/**
 * @brief Configuration version constants
 *
 * @requirements
 *
 * SC_REQ-221836 FBRQ - CfgGeneral - provide configuration version type and constants
 * SC_REQ-221389 FBRQ - Provide constants for APS Sensors
 */

/* constant denoting no valid version */
#define CONFIG_VERSION_NONE ((cfgVersion_t)0U)
/* constant denoting any configuration version */
#define CONFIG_VERSION_ANY  ((cfgVersion_t)0x3feU)
/* bit mask to isolate valid version bits from a variable that can hold more than 10 bits */
#define CONFIG_VERSION_MASK ((cfgVersion_t)0x3ffU)

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief Configuration version type
 *
 * @requirements
 *
 * SC_REQ-221836 FBRQ - CfgGeneral - provide configuration version type and constants
 */
typedef uint16_t cfgVersion_t;

/**
 * @brief           Configuration parameter identifiers.
 *
 * @requirements    SC_REQ-218578
 *
 * @remarks
 * - Literal naming scheme:
 *   CfgParam_<name of configuration parameter in referenced specification>.
 * - Values are grouped according to the modules defined in Main Architecture.
 */
typedef enum {
    /* CfgConstants module */
    CfgParam_MULTI_NumberOfVerticalTracksInstalled,
    CfgParam_MULTI_NumberOfHorizontalTracksInstalled,
    CfgParam_MULTI_NumberOfXCInstalled,
    CfgParam_MULTI_NumberOfLCUInstalled,
    CfgParam_MULTI_NumberOfIOCInstalled,
    CfgParam_MULTI_NumberOfFireCompartmentsInstalled,
    CfgParam_MULTI_NumberOfGarageAreasInstalled,
    CfgParam_MULTI_NumberOfMaintenanceAreasInstalled,
    CfgParam_MULTI_NumberOfPropulsionSectionsInstalled,
    CfgParam_MULTI_NumberOfWallPanelAreasInstalled,
    CfgParam_MULTI_NumberOfWalkwaysInstalled,
    CfgParam_MULTI_NumberOfWellWaterAreasInstalled,
    CfgParam_MULTI_NumberOfWellDoorsInstalled,
    CfgParam_MULTI_NumberOfWellAccessPortsInstalled,
    CfgParam_MULTI_NumberOfWellLocksInstalled,
    CfgParam_MULTI_NumberOfWellSegmentsInstalled,
    CfgParam_MSMA_MaxSubsystemInitWaitingTime,
    CfgParam_ConfigVersion,
    CfgParam_ConfigJsId,
    CfgParam_LutFn_NumberOfEntriesMax,
    CfgParam_LutXtarget_NumberOfEntriesMax,
    CfgParam_LCUBrake_CalibrationCountRisingThreshold,
    CfgParam_LCUBrake_CalibrationCountFallingThreshold,
    CfgParam_LCUBrake_CalibrationDeltaP,

    /* CfgLcu module */
    CfgParam_LCU_AlignmentPoint2Top_EconomyLCU,
    CfgParam_LCU_AlignmentPoint2Bottom_EconomyLCU,
    CfgParam_LCU_AlignmentPoint2Right_EconomyLCU,
    CfgParam_LCU_AlignmentPoint2Left_EconomyLCU,
    CfgParam_LCU_AlignmentPoint2Top_PremiumLCU,
    CfgParam_LCU_AlignmentPoint2Bottom_PremiumLCU,
    CfgParam_LCU_AlignmentPoint2Right_PremiumLCU,
    CfgParam_LCU_AlignmentPoint2Left_PremiumLCU,
    CfgParam_LCU_PivotBearingOffsetY_EconomyLCU,
    CfgParam_LCU_PivotBearingOffsetZ_EconomyLCU,
    CfgParam_LCU_PivotBearingOffsetY_PremiumLCU,
    CfgParam_LCU_PivotBearingOffsetZ_PremiumLCU,
    CfgParam_LCU_ParkingBrakeForceThreshold,
    CfgParam_LCU_SpeedHorizMin,
    CfgParam_LCU_SpeedVertMin,
    CfgParam_LCU_SpeedHorizNominal,
    CfgParam_LCU_SpeedVertNominal,
    CfgParam_LCU_UpAccelNominal,
    CfgParam_LCU_DownAccelNominal,
    CfgParam_LCU_HorizAccelNominal,
    CfgParam_LCU_SpeedUnloadedHorizMax,
    CfgParam_LCU_SpeedLoadedHorizMax,
    CfgParam_LCU_SpeedVertMax,
    CfgParam_LCU_SpeedInManualOpHorizMax,
    CfgParam_LCU_SpeedInManualOpVertMax,
    CfgParam_LCU_SlidingBrakeSpeedMax,

    CfgParam_LCU_EmergencyAutonomVertDecelMin,
    CfgParam_LCU_EmergencyAutonomVertDecelMax,
    CfgParam_LCU_NormalAutonomHorizDecelMin,
    CfgParam_LCU_NormalAutonomHorizDecelMax,
    CfgParam_LCU_NormalAutonomUpDecelMin,
    CfgParam_LCU_NormalAutonomUpDecelMax,
    CfgParam_LCU_NormalAutonomDownDecelMin,
    CfgParam_LCU_NormalAutonomDownDecelMax,
    CfgParam_LCU_ControlledHorizDecelMin,
    CfgParam_LCU_ControlledHorizDecelModerate,
    CfgParam_LCU_ControlledDownDecelModerate,
    CfgParam_LCU_ControlledDownDecelMin,
    CfgParam_LCU_ForcedHorizDecelMin,
    CfgParam_LCU_ForcedDownDecelMin,
    CfgParam_LCU_PayloadDetectionThreshold,
    CfgParam_LCU_UpAccelMax,
    CfgParam_LCU_DownAccelMax,
    CfgParam_LCU_HorizAccelMax,
    CfgParam_LCU_UpDecelMax,
    CfgParam_LCU_DownDecelMax,
    CfgParam_LCU_HorizDecelMax,
    CfgParam_LCU_APS_MaxAccelDeviation,
    CfgParam_LCU_APS_GravityOffset_1A,
    CfgParam_LCU_APS_GravityOffset_1B,
    CfgParam_LCU_APS_GravityOffset_2A,
    CfgParam_LCU_APS_GravityOffset_2B,
    CfgParam_LCU_APS_AccSensorOffset,

    CfgParam_LCU_VelocityEcasPreBrakeMax,
    CfgParam_LCU_VelocityNcasPreBrakeMax,
    CfgParam_LCU_VelocitySfoPreBrakeMax,
    CfgParam_LCU_VelocityNcasMin,
    CfgParam_LCU_VelocityEcasMin,
    CfgParam_LCU_VelocityFES,

    CfgParam_LCU_NormalCasActuationTime,
    CfgParam_LCU_EmergencyCasActuationTime,
    CfgParam_LCU_ControlledPrimaryBrakeActuationTime,
    CfgParam_LCU_ForcedPrimaryBrakeActuationTime,
    CfgParam_LCU_ForcedRedundantBrakeActuationTime,
    CfgParam_LCU_SfoActuationTime,
    CfgParam_LCU_NoMovementTime,
    CfgParam_LCU_LoadBrakeCollectiveNextEventCountTime,
    CfgParam_LCU_LoadBrakeCollectiveDebouncingCyclesTime,

    CfgParam_LCU_CollectiveLoadMax_CASVertical_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_CASHorizontal_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_ECASVertical_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_ECASHorizontal_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_CESBrakingVertical_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_CESBrakingHorizontal_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_FESBrakingExtremeVertical_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_FESBrakingVertical_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_FESBrakingHorizontal_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_Overload_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_Crushload_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_JumpingPassenger_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_PayloadChange_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_LcuTooFastInBufferArea_NrOfEvents,

    CfgParam_LCU_CollectiveLoadMax_Brk1ActivationWhenStopped_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_Brk2ActivationWhenStopped_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_Brk3ActivationWhenStopped_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_Brk4ActivationWhenStopped_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_Brk1ActivationWhenMovingUp_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_Brk2ActivationWhenMovingUp_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_Brk3ActivationWhenMovingUp_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_Brk4ActivationWhenMovingUp_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_CESBrakingBrk1_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_CESBrakingBrk2_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_CESBrakingBrk3_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_CESBrakingBrk4_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_FESBrakingBrk1_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_FESBrakingBrk2_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_FESBrakingBrk3_NrOfEvents,
    CfgParam_LCU_CollectiveLoadMax_FESBrakingBrk4_NrOfEvents,

    CfgParam_LCU_CollectiveLoad_SensorValue75kg,
    CfgParam_LCU_CollectiveLoad_SensorValue225kg,
    CfgParam_LCU_CollectiveLoad_SensorValue375kg,
    CfgParam_LCU_CollectiveLoad_SensorValue525kg,
    CfgParam_LCU_CollectiveLoad_SensorValue675kg,
    CfgParam_LCU_CollectiveLoad_SensorValue825kg,
    CfgParam_LCU_CollectiveLoad_OverloadThreshold,
    CfgParam_LCU_CollectiveLoad_CrushloadThreshold,

    CfgParam_LCU_PayloadMax_EconomyLCU,
    CfgParam_LCU_PayloadMax_PremiumLCU,
    CfgParam_LCU_PayloadMaxHyst,
    CfgParam_LCU_PayloadCrushed,
    CfgParam_LCU_PayloadCrushedHyst,
    CfgParam_LCU_LoadedWeightMax,
    CfgParam_LCU_LoadedWeightMaxHyst,
    CfgParam_LCU_EmptyWeightMax_EconomyLCU,
    CfgParam_LCU_EmptyWeightMax_PremiumLCU,
    CfgParam_LCU_AmbientTempMax,
    CfgParam_LCU_AmbientTempMaxHyst,
    CfgParam_LCU_AvgEmptyWeightDigit_EconomyLCU,
    CfgParam_LCU_AvgEmptyWeightDigit_PremiumLCU,
    CfgParam_LCU_AvgEmptyWeightKg_EconomyLCU,
    CfgParam_LCU_AvgEmptyWeightKg_PremiumLCU,
    CfgParam_LCU_AvgOverloadWeightDigit_EconomyLCU,
    CfgParam_LCU_AvgOverloadWeightDigit_PremiumLCU,
    CfgParam_LCU_AvgOverloadWeightKg_EconomyLCU,
    CfgParam_LCU_AvgOverloadWeightKg_PremiumLCU,

    CfgParam_LCU_PBS_SafetyValvesDeEnergizedTime,

    CfgParam_LCU_TargetDeceleration_Horizontal,
    CfgParam_LCU_TargetDeceleration_Down,
    CfgParam_LCU_PrimaryBrakeStepFullActivationThreshold,
    CfgParam_LCU_BrakePositionOpenThreshold,
    CfgParam_LCU_BrakeStepActiveHydraulicPressureThreshold,

    CfgParam_LCU_LoSD_downwards_BS_K1,
    CfgParam_LCU_LoSD_downwards_BS_K2,
    CfgParam_LCU_LoSD_downwards_BS_KOffset,
    CfgParam_LCU_USD_downwards_BS_K1,
    CfgParam_LCU_USD_downwards_BS_K2,
    CfgParam_LCU_USD_downwards_BS_KOffset,
    CfgParam_LCU_USD_upwards_BS_K1,
    CfgParam_LCU_USD_upwards_BS_K2,
    CfgParam_LCU_USD_upwards_BS_KOffset,
    CfgParam_LCU_LeSD_rightwards_BS_K1,
    CfgParam_LCU_LeSD_rightwards_BS_K2,
    CfgParam_LCU_LeSD_rightwards_BS_KOffset,
    CfgParam_LCU_LeSD_leftwards_BS_K1,
    CfgParam_LCU_LeSD_leftwards_BS_K2,
    CfgParam_LCU_LeSD_leftwards_BS_KOffset,
    CfgParam_LCU_RSD_rightwards_BS_K1,
    CfgParam_LCU_RSD_rightwards_BS_K2,
    CfgParam_LCU_RSD_rightwards_BS_KOffset,
    CfgParam_LCU_RSD_leftwards_BS_K1,
    CfgParam_LCU_RSD_leftwards_BS_K2,
    CfgParam_LCU_RSD_leftwards_BS_KOffset,

    CfgParam_LCU_Type,

    CfgParam_LCU_PositionTranslation_TrackIsVertical,
    CfgParam_LCU_PositionTranslation_CenterAlignmentPosAlongTrack,
    CfgParam_LCU_APS_1A_Offset_Z_left,
    CfgParam_LCU_APS_1A_Offset_Y_left,
    CfgParam_LCU_APS_1B_Offset_Z_left,
    CfgParam_LCU_APS_1B_Offset_Y_left,
    CfgParam_LCU_APS_2A_Offset_Z_right,
    CfgParam_LCU_APS_2A_Offset_Y_right,
    CfgParam_LCU_APS_2B_Offset_Z_right,
    CfgParam_LCU_APS_2B_Offset_Y_right,

    CfgParam_LCUBrake_InitialFnMax,
    CfgParam_LCUBrake_InitialPcontact,
    CfgParam_LCUBrake_InitialXcontact,
    CfgParam_LCUBrake_InitialPosition0,
    /* LCUBrake_LutFn/LutXtarget can only be read via special getter, thus there are not CfgParam
     * values */
    CfgParam_LCUBrake_x_c_trg_max,
    CfgParam_LCUBrake_x_c_trg_min,
    CfgParam_LCUBrake_p_c_trg_min,
    CfgParam_LCUBrake_p_c_trg_max,
    CfgParam_LCUBrake_x_c_trg_min_area_of_evaluation_parameter,
    CfgParam_LCUBrake_x_c_trg_max_area_of_evaluation_parameter,

    /* CfgProtectionAreas module */
    CfgParam_FireCompartment_TopLeft_Z,
    CfgParam_FireCompartment_TopLeft_Y,
    CfgParam_FireCompartment_BottomRight_Z,
    CfgParam_FireCompartment_BottomRight_Y,

    CfgParam_GarageArea_TopLeft_Z,
    CfgParam_GarageArea_TopLeft_Y,
    CfgParam_GarageArea_BottomRight_Z,
    CfgParam_GarageArea_BottomRight_Y,

    CfgParam_MaintenanceArea_TopLeft_Z,
    CfgParam_MaintenanceArea_TopLeft_Y,
    CfgParam_MaintenanceArea_BottomRight_Z,
    CfgParam_MaintenanceArea_BottomRight_Y,

    CfgParam_PropulsionSection_TopLeft_Z,
    CfgParam_PropulsionSection_TopLeft_Y,
    CfgParam_PropulsionSection_BottomRight_Z,
    CfgParam_PropulsionSection_BottomRight_Y,

    CfgParam_PropulsionSection_TrackStartIndex,

    CfgParam_SwivelPlatform_TopLeft_Z,
    CfgParam_SwivelPlatform_TopLeft_Y,
    CfgParam_SwivelPlatform_BottomRight_Z,
    CfgParam_SwivelPlatform_BottomRight_Y,

    CfgParam_TranslationalMovementArea_TopLeft_Z,
    CfgParam_TranslationalMovementArea_TopLeft_Y,
    CfgParam_TranslationalMovementArea_BottomRight_Z,
    CfgParam_TranslationalMovementArea_BottomRight_Y,

    CfgParam_WalkwayArea_TopLeft_Z,
    CfgParam_WalkwayArea_TopLeft_Y,
    CfgParam_WalkwayArea_BottomRight_Z,
    CfgParam_WalkwayArea_BottomRight_Y,

    CfgParam_WallPanelArea_TopLeft_Z,
    CfgParam_WallPanelArea_TopLeft_Y,
    CfgParam_WallPanelArea_BottomRight_Z,
    CfgParam_WallPanelArea_BottomRight_Y,

    CfgParam_WellAccessPort_TopLeft_Z,
    CfgParam_WellAccessPort_TopLeft_Y,
    CfgParam_WellAccessPort_BottomRight_Z,
    CfgParam_WellAccessPort_BottomRight_Y,

    CfgParam_WellDoor_TopLeft_Z,
    CfgParam_WellDoor_TopLeft_Y,
    CfgParam_WellDoor_BottomRight_Z,
    CfgParam_WellDoor_BottomRight_Y,

    CfgParam_WellLock_TopLeft_Z,
    CfgParam_WellLock_TopLeft_Y,
    CfgParam_WellLock_BottomRight_Z,
    CfgParam_WellLock_BottomRight_Y,

    CfgParam_WellWaterArea_TopLeft_Z,
    CfgParam_WellWaterArea_TopLeft_Y,
    CfgParam_WellWaterArea_BottomRight_Z,
    CfgParam_WellWaterArea_BottomRight_Y,

    CfgParam_WorkingArea_ZPoint2Top,
    CfgParam_WorkingArea_ZPoint2Bottom,
    CfgParam_WorkingArea_YPoint2Right,
    CfgParam_WorkingArea_YPoint2Left,

    CfgParam_WorkingArea_ExtensionTop,
    CfgParam_WorkingArea_ExtensionBottom,
    CfgParam_WorkingArea_ExtensionRight,
    CfgParam_WorkingArea_ExtensionLeft,

    /* CfgSR module */
    CfgParam_IOCSN_MaxTemperature,

    CfgParam_LCUAPS_MaxTravelDistanceWithCalculatedPosition,
    CfgParam_LCUAPS_MaxTravelTimeWithCalculatedPosition,
    CfgParam_LCUAPS_MaxTravelTimeWithCalculatedVelocity,
    CfgParam_LCUAPS_MaxTravelTimeWithInjectedVelocity,
    CfgParam_LCUAPS_MinSpeedWithExtrapolation,

    CfgParam_LCUBRK_WearoutLevelWarn,
    CfgParam_LCUBRK_WearoutLevelError,
    CfgParam_LCUBRK_FrictionLevelWarn,
    CfgParam_LCUBRK_FrictionLevelError,
    CfgParam_LCUBRK_StepForceDelta,
    CfgParam_LCUBRK_MaxRedundantBrakeFailureTime,
    CfgParam_LCUBRK_MaxUpwardBrakeOpCount,
    CfgParam_LCUBRK_MaxPrimaryUBPLockingTime,
    CfgParam_LCUBRK_MaxRedundantUBPLockingTime,
    CfgParam_LCUBRK_MaxPrimaryUBPUnlockingTime,
    CfgParam_LCUBRK_MaxRedundantUBPUnlockingTime,

    CfgParam_LCUBSS_MaxBrakeSafetySensorFailureTime,
    CfgParam_LCUBSS_MaxBrakeReleaseTimeWarn,
    CfgParam_LCUBSS_MaxBrakeReleaseTimeError,
    CfgParam_LCUBSS_MaxBrakeActuationTimeWarn,
    CfgParam_LCUBSS_MaxBrakeActuationTimeError,
    CfgParam_LCUBSS_dXtargetTolerance,
    CfgParam_LCUBSS_dXcontactTolerance,
    CfgParam_LCUBSS_PcontactMin,
    CfgParam_LCUBSS_PcontactMax,

    CfgParam_LCUCBSS_CabinLockingMaxBlockingTime,
    CfgParam_LCUCBSS_CabinLockingMaxUnblockingTime,

    CfgParam_LCUCLSS_MaxLCUCabinLockingTime,
    CfgParam_LCUCLSS_MaxLCUCabinUnlockingTime,
    CfgParam_LCUCLSS_CabinLockingStatusUndefinedMaxTime,

    CfgParam_LCUCTS_DiffOverwriteErrorTemp,
    CfgParam_LCUCTS_DiffOverwriteWarnTemp,
    CfgParam_LCUCTS_MaxDiffValue,
    CfgParam_LCUCTS_MaxDiffTime,
    CfgParam_LCUCTS_MaxZeroTime,
    CfgParam_LCUCTS_ZeroOverwriteErrorTemp,
    CfgParam_LCUCTS_ZeroOverwriteWarnTemp,

    CfgParam_LCULW_MaxDiffValue,
    CfgParam_LCULW_MinValue,
    CfgParam_LCULW_MaxValue,
    CfgParam_LCULW_CrushLoadValue,

    CfgParam_LCUSN_MaxTemperature,
    CfgParam_LCUSN_PrimaryBrakeActiveWithoutCommandMaxTime,
    CfgParam_LCUSN_ReactionModuleSignalDelayTime,

    CfgParam_MSMA_MaxTemperature,
    CfgParam_MSMA_FireCompartmentRetractionFeedbackTimeout,

    CfgParam_SCTS_MaxErrorTime,
    CfgParam_SCTS_ErrorOverwriteErrorTemp,
    CfgParam_SCTS_ErrorOverwriteWarnTemp,

    CfgParam_TCBSS_MaxBlockActuationTime,
    CfgParam_TCBSS_MaxUnblockActuationTime,

    CfgParam_TCLSS_MaxCabinLockingTime,
    CfgParam_TCLSS_LockingStatusUndefinedMaxTime,
    CfgParam_TCLSS_MaxCabinUnlockingTime,
    CfgParam_TCLSS_MaxUndefinedTime,

    CfgParam_UDSS_MaxAllowedUnderpowerTime,

    CfgParam_WTSS_MaxInvalidTime,
    CfgParam_WTSS_OverwriteWarnTemp,
    CfgParam_WTSS_OverwriteErrTemp,
    CfgParam_WTSS_MaxDiffValue,

    CfgParam_XCL_MaxLockActuationTime,
    CfgParam_XCL_MaxUnlockActuationTime,
    CfgParam_XCL_MaxBlockActuationTime,
    CfgParam_XCL_MaxUnblockActuationTime,
    CfgParam_XCL_MaxUndefinedTime,

    CfgParam_XCGBSS_MaxMaintenanceTime,
    CfgParam_XCGBSS_MaxAllowedReleaseTime,
    CfgParam_XCGBSS_MaxAllowedActivationTime,

    CfgParam_XCSARE_MaxFailureTime,

    CfgParam_XCSN_WarnTemp,

    CfgParam_XCISTO_ActuationTimeDuration,
    CfgParam_XCISTO_DeActuationTimeDuration,

    CfgParam_Ssr_Enable,
    CfgParam_Sf_Enable,

    /* CfgWellDoor module */
    CfgParam_WellDoor_DoorZone_AlignmentPoint2Top,
    CfgParam_WellDoor_DoorZone_AlignmentPoint2Bottom,
    CfgParam_WellDoor_DoorZone_AlignmentPoint2Right,
    CfgParam_WellDoor_DoorZone_AlignmentPoint2Left,
    CfgParam_WellDoor_DoorZone_TopLeftZ,
    CfgParam_WellDoor_DoorZone_TopLeftY,
    CfgParam_WellDoor_DoorZone_BottomRightZ,
    CfgParam_WellDoor_DoorZone_BottomRightY,
    CfgParam_WellDoor_UCMZone_AlignmentPoint2Top,
    CfgParam_WellDoor_UCMZone_AlignmentPoint2Bottom,
    CfgParam_WellDoor_UCMZone_AlignmentPoint2Right,
    CfgParam_WellDoor_UCMZone_AlignmentPoint2Left,
    CfgParam_WellDoor_UCMZone_TopLeftZ,
    CfgParam_WellDoor_UCMZone_TopLeftY,
    CfgParam_WellDoor_UCMZone_BottomRightZ,
    CfgParam_WellDoor_UCMZone_BottomRightY,
    CfgParam_WellDoor_UnlockingZone_AlignmentPoint2Top,
    CfgParam_WellDoor_UnlockingZone_AlignmentPoint2Bottom,
    CfgParam_WellDoor_UnlockingZone_AlignmentPoint2Right,
    CfgParam_WellDoor_UnlockingZone_AlignmentPoint2Left,
    CfgParam_WellDoor_UnlockingZone_TopLeftZ,
    CfgParam_WellDoor_UnlockingZone_TopLeftY,
    CfgParam_WellDoor_UnlockingZone_BottomRightZ,
    CfgParam_WellDoor_UnlockingZone_BottomRightY,
    CfgParam_WellDoor_AlligmentPointZ,
    CfgParam_WellDoor_AlligmentPointY,
    CfgParam_WellDoor_AffectedSegments,

    /* CfgWellAccessPort module */
    CfgParam_WellAccessPort_DoorZone_TopLeft_Z,
    CfgParam_WellAccessPort_DoorZone_TopLeft_Y,
    CfgParam_WellAccessPort_DoorZone_BottomRight_Z,
    CfgParam_WellAccessPort_DoorZone_BottomRight_Y,

    /* CfgWell module */
    CfgParam_Well_AmbientTempMax,
    CfgParam_Well_AmbientTempMaxHyst,
    CfgParam_Well_AmbientTempMin,
    CfgParam_Well_TemperatureSensorBottom_Z,
    CfgParam_Well_TemperatureSensorTop_Z,

    /* CfgXc module */
    CfgParam_XC_CenterPoint2Top,
    CfgParam_XC_CenterPoint2Bottom,
    CfgParam_XC_CenterPoint2Right,
    CfgParam_XC_CenterPoint2Left,
    CfgParam_XC_MatchToleranceY,
    CfgParam_XC_MatchToleranceZ,
    CfgParam_XC_SPRotationThreshold,
    CfgParam_XC_CenterZ,
    CfgParam_XC_CenterY,
    CfgParam_XC_TopLeftZ,
    CfgParam_XC_TopLeftY,
    CfgParam_XC_BottomRightZ,
    CfgParam_XC_BottomRightY,
    CfgParam_XC_LockingAngleTolerance,
    CfgParam_XC_ClockwiseOuterEdgeAngle,
    CfgParam_XC_CounterclockwiseOuterEdgeAngle,
    CfgParam_XC_ClockwiseClearanceAngle,
    CfgParam_XC_CounterclockwiseClearanceAngle,
    CfgParam_XCSARE_VelocityIntegrationTime,
    cfgParam_XCSARE_MultiturnInitialValue,
    cfgParam_XCSARE_ZeroDegreePosition,
    cfgParam_XCSARE_NinetyDegreePosition,
    CfgParam_XC_AngularSpeedMax,
    CfgParam_XC_AngularSpeedInManualOpModeMax,
    CfgParam_TSPRy_StartAngle,
    CfgParam_TSPRy_EndAngle,
    CfgParam_TSPRy_AngularVelocityMaxAtStart,
    CfgParam_TSPRy_AngularVelocityMaxAtEnd,

    /* CfgIoc module */
    CfgParam_WellDoorLockedClosed,
    CfgParam_WellAccessPortClosed,
    CfgParam_WellSegmentAccessPortNotClosed,
    CfgParam_WaterInWell,
    CfgParam_PersonInMaintenanceArea,
    CfgParam_PersonInGarage,
    CfgParam_PersonInWalkway,
    CfgParam_WallPanelClosed,
    CfgParam_WellSegmentEmergencyStopTriggered,
    CfgParam_FireCompartmentRetracted,
    CfgParam_WellTemperature,
    CfgParam_UnderpowerDetectionUsed,
} enCfgParam_t;

#endif /* CFGTYPES_H_ */

/** @}  End of this module */
