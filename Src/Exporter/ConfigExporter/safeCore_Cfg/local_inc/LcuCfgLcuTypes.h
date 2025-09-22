/** \addtogroup LcuCfgLcu
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      LcuCfgLcuTypes.h
 *  @brief     Data type that holds all the LCU's CfgLcu module configuration data.
 *  @author    Frank Meier, Daniel Mueller
 *
 */

#ifndef LCUCFGLCUTYPES_H_
#define LCUCFGLCUTYPES_H_

/*
 * INCLUDEFILES
 */
#include "CfgLimits.h"
#include "CfgLcu.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * MACRO DEFINITIIONS
 */

/**
 * @brief
 * Constants for handling configuration data of Lcu APS
 *
 * @requirements
 * SC_REQ-221389 - FBRQ - Provide constants for APS Sensors
 */
#define INVALID_TRACK_ID    0xFFU
#define INVALID_OFFSET      0xFFFFU
#define APS_NUMBER_OF_CODETAPES_MAX 30U
#define INVALID_CODE_TAPE_NUMBER    0xFFU

/**
 * @brief        Maximum number of elements in a LCUBrake lookup table (lut)
 *
 * @requirements SC_REQ-222132
 */
#define LCUBRAKE_LUT_NUMBER_OF_ENTRIES_MAX_LIMIT 16U

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief
 * Enumeration for identifying a specific Cedes Box
 *
 * @requirements
 * SC_REQ-221360 - FBRQ - Provide data structure for identifying a CEDES box
 */
typedef enum {
    Cedes_Box1 = 0,/**< Cedes_Box1 */
    Cedes_Box2 = 1,/**< Cedes_Box2 */
} enCedesBox_t;

/**
 * @brief
 * Enumeration for identifying a CEDES Acceleration error state
 *
 * @requirements
 * SC_REQ-245451 - FBRQ - Provide data structure for identifying a CEDES Acceleration error state
 */
typedef enum {
    Acc_Data_No_Error = 0,/**< No error */
    Acc_Data_Out_Of_5g_To_6g_Range = 1,/**< Out of ±5g to ±6g range error */
    Acc_Data_Outside_6g_Range = 2,/**< Outside ±6g range error */
} enAccDataErr_t;

/**
 * @brief
 * Datatype for describing the relative position on track of a Lcu
 *
 * @requirements
 * SC_REQ-221359 - FBRQ - IOAps - Type for describing relative position on track
 *
 */
typedef uint32_t rawApsPosition_t;

/**
 * @brief            Position translation data of Lcu APS
 *
 * @requirements     SC_REQ-221388
 */
typedef struct { /*lint !e9045 */
    uint8_t ucCodeTapeNumber;
    uint8_t spare[3];
    uint32_t ulRawCodeTapeLBLeft;
    uint32_t ulRawCodeTapeUBLeft;
    uint32_t ulRawCodeTapeLBRight;
    uint32_t ulRawCodeTapeUBRight;
    uint32_t ulRawCodeTapeLBLeft_Z;
    uint32_t ulRawCodeTapeUBLeft_Z;
    uint32_t ulRawCodeTapeLBRight_Z;
    uint32_t ulRawCodeTapeUBRight_Z;
} lcuCfgApsPosition_t;

/**
 * @brief        LCUBrake lookup table data
 *
 * @requirements SC_REQ-222131
 */
typedef struct { /*lint !e9045 */
    int16_t sArg;
    int16_t sValue;
} lcuBrakeLut_t;

/**
 * @brief           Configuration data of LCU CfgLcu module.
 * @requirements    SC_REQ-218584
 */
typedef struct { /*lint !e9045 */
    /** contains an entry for each enLcuType_t */
    lcuPhysicalConstraints_t tPhysicalConstraints[2];

    int32_t lParkingBrakeForceThreshold;
    int32_t lSpeedHorizMin;
    int32_t lSpeedVertMin;
    int32_t lSpeedHorizNominal;
    int32_t lSpeedVertNominal;
    int32_t lUpAccelNominal;
    int32_t lDownAccelNominal;
    int32_t lHorizAccelNominal;
    int32_t lSpeedUnloadedHorizMax;
    int32_t lSpeedLoadedHorizMax;
    int32_t lSpeedVertMax;
    int32_t lSpeedInManualOpHorizMax;
    int32_t lSpeedInManualOpVertMax;
    int32_t lSlidingBrakeSpeedMax;
    int32_t lEmergencyAutonomVertDecelMin;
    int32_t lEmergencyAutonomVertDecelMax;
    int32_t lNormalAutonomHorizDecelMin;
    int32_t lNormalAutonomHorizDecelMax;
    int32_t lNormalAutonomUpDecelMin;
    int32_t lNormalAutonomUpDecelMax;
    int32_t lNormalAutonomDownDecelMin;
    int32_t lNormalAutonomDownDecelMax;
    int32_t lControlledHorizDecelMin;
    int32_t lControlledHorizDecelModerate;
    int32_t lControlledDownDecelModerate;
    int32_t lControlledDownDecelMin;
    int32_t lForcedHorizDecelMin;
    int32_t lForcedDownDecelMin;
    int32_t lPayloadDetectionThreshold;
    int32_t lUpAccelMax;
    int32_t lDownAccelMax;
    int32_t lHorizAccelMax;
    int32_t lUpDecelMax;
    int32_t lDownDecelMax;
    int32_t lHorizDecelMax;
    int32_t lAps_maxAccelDeviation;
    int32_t lAps_gravityOffset_1A;
    int32_t lAps_gravityOffset_2A;
    int32_t lAps_gravityOffset_1B;
    int32_t lAps_gravityOffset_2B;
    int32_t lAps_AccSensorOffset;

    int32_t lVelocityEcasPreBrakeMax;
    int32_t lVelocityNcasPreBrakeMax;
    int32_t lVelocitySfoPreBrakeMax;
    int32_t lVelocityNcasMin;
    int32_t lVelocityEcasMin;
    int32_t lVelocityFES;

    int32_t lNormalCasActuationTime;
    int32_t lEmergencyCasActuationTime;
    int32_t lControlledPrimaryBrakeActuationTime;
    int32_t lForcedPrimaryBrakeActuationTime;
    int32_t lForcedRedundantBrakeActuationTime;
    int32_t lSfoActuationTime;
    int32_t lLCU_NoMovementTime;
    int32_t lLCU_LoadBrakeCollectiveNextEventCountTime;
    int32_t lLCU_LoadBrakeCollectiveDebouncingCyclesTime;

    collectiveLoadMax_t tCollectiveLoadMax;

    int32_t lOverloadThreshold;
    int32_t lCrushloadThreshold;
    int32_t lPayload75kg;
    int32_t lPayload225kg;
    int32_t lPayload375kg;
    int32_t lPayload525kg;
    int32_t lPayload675kg;
    int32_t lPayload825kg;
    int32_t lPayloadMax_EconomyLCU;
    int32_t lPayloadMax_PremiumLCU;
    int32_t lPayloadMaxHyst;
    int32_t lPayloadCrushed;
    int32_t lPayloadCrushedHyst;
    int32_t lLoadedWeightMax;
    int32_t lLoadedWeightMaxHyst;
    int32_t lEmptyWeightMax_EconomyLCU;
    int32_t lEmptyWeightMax_PremiumLCU;
    int32_t lAmbientTempMax;
    int32_t lAmbientTempMaxHyst;
    int32_t lAvgEmptyWeightDigit_EconomyLCU;
    int32_t lAvgEmptyWeightDigit_PremiumLCU;
    int32_t lAvgEmptyWeightKg_EconomyLCU;
    int32_t lAvgEmptyWeightKg_PremiumLCU;
    int32_t lAvgOverloadWeightDigit_EconomyLCU;
    int32_t lAvgOverloadWeightDigit_PremiumLCU;
    int32_t lAvgOverloadWeightKg_EconomyLCU;
    int32_t lAvgOverloadWeightKg_PremiumLCU;

    int32_t lPBS_SafetyValvesDeEnergizedTime;

    int32_t lTargetDeceleration_Horizontal;
    int32_t lTargetDeceleration_Down;
    int32_t lPrimaryBrakeStepFullActivationThreshold;
    int32_t lBrakePositionOpenThreshold;
    int32_t lBrakeStepActiveHydraulicPressureThreshold[MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU];

    stoppingDistanceParamSet_t tStoppingDistanceParamSet;

    int8_t cType; /* enLcuType_t */

    int8_t cPositionTranslation_TrackIsVertical[MULTI_NUMBER_OF_TRACKS_MAX];
    int32_t lPositionTranslation_CenterAlignmentPosAlongTrack[MULTI_NUMBER_OF_TRACKS_MAX];
    int32_t lAPS_1A_Offset_Z_left;
    int32_t lAPS_1A_Offset_Y_left;
	int32_t lAPS_1B_Offset_Z_left;
    int32_t lAPS_1B_Offset_Y_left;
    int32_t lAPS_2A_Offset_Z_right;
    int32_t lAPS_2A_Offset_Y_right;
    int32_t lAPS_2B_Offset_Z_right;
    int32_t lAPS_2B_Offset_Y_right;

    lcuCfgApsPosition_t tApsPositionInfo[APS_NUMBER_OF_CODETAPES_MAX];
    uint32_t ulMULTI_NumberOfCodeTapeInstalled;

    int32_t lLCUBrake_initialFnMax[MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU];
    int32_t lLCUBrake_initialPcontact[MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU];
    int32_t lLCUBrake_initialXcontact[MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU];
    int32_t lLCUBrake_initialPosition0[MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU];
    lcuBrakeLut_t tLutFn[LCUBRAKE_LUT_NUMBER_OF_ENTRIES_MAX_LIMIT];
    lcuBrakeLut_t tLutXtarget[LCUBRAKE_LUT_NUMBER_OF_ENTRIES_MAX_LIMIT];

    int32_t lLCUBrake_x_c_trg_max[MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU];
    int32_t lLCUBrake_x_c_trg_min[MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU];
    int32_t lLCUBrake_p_c_trg_min[MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU];
    int32_t lLCUBrake_p_c_trg_max[MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU];
    int32_t lLCUBrake_x_c_trg_min_area_of_evaluation_parameter;
    int32_t lLCUBrake_x_c_trg_max_area_of_evaluation_parameter;

} lcuCfgLcu_t;


#endif /* LCUCFGLCUTYPES_H_ */

/** @}  End of this module */
