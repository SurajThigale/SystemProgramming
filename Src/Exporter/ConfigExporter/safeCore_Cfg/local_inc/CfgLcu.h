/** \defgroup GenericCfgLcu GenericCfgLcu SW Module Specification
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgLcu.h
 *  @brief     Provides access to Safety Node specific CfgLcu configuration parameter data.
 *  @author    Frank Meier
 *
 */

#ifndef CFGLCU_H_
#define CFGLCU_H_

/*
 * INCLUDEFILES
 */
#include "CfgTypes.h"
#include "UtilBaseTypes.h"

/*
 *  MACRO DEFINITIONS
 */

/**
 * @brief           Index into stopping distance parameter array for controlled autonomous stop.
 * @requirements    SC_REQ-218563
 */
#define LCU_STOPPING_DISTANCE_PARAM_CAS 0U
/**
 * @brief           Index into stopping distance parameter array for controlled emergency stop.
 * @requirements    SC_REQ-218563
 */
#define LCU_STOPPING_DISTANCE_PARAM_CES 1U
/**
 * @brief           Index into stopping distance parameter array for forced emergency stop.
 * @requirements    SC_REQ-218563
 */
#define LCU_STOPPING_DISTANCE_PARAM_FES 2U
/**
 * @brief           Number of stopping distance parameters in array.
 * @requirements    SC_REQ-218563
 */
#define LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS 3U

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Physical constraints of LCU
 * @requirements    SC_REQ-218562
 */
typedef struct {
    /** The distance between LCU alignment point and leftmost/highest and rightmost/lowest point */
    area_t tSize;
    /** Horizontal distance between the Economy LCU alignment point and center of bearing between cabin and sledge */
    int16_t sPivotBearingOffsetY;
    /** Vertical distance between the Economy LCU alignment point and center of bearing between cabin and sledge */
    int16_t sPivotBearingOffsetZ;
} lcuPhysicalConstraints_t;

/**
 * @brief           Collective load max
 * @requirements    SC_REQ-218562
 */
typedef struct {
    /** Maximum allowed number of activations of Controlled Automatic Stop
     *  in vertical direction. */
    uint16_t usCASVerticalNrOfEvents;
    /** Maximum allowed number of activations of Controlled Automatic Stop
     * in horizontal direction. */
    uint16_t usCASHorizontalNrOfEvents;
    /** Maximum allowed number of activations of Emergency Controlled Automatic Stop
      *  in vertical direction. */
    uint16_t usECASVerticalNrOfEvents;
   /** Maximum allowed number of activations of Controlled Braking in
     * horizontal direction. */
    uint16_t usCESBrakingVerticalNrOfEvents;
   /** Maximum allowed number of activations of Controlled Braking in
    * horizontal direction. */
    uint16_t usCESBrakingHorizontalNrOfEvents;
   /** Maximum allowed number of activations of Forced Braking in extreme case in vertical
    *  direction. */
    uint16_t usFESBrakingExtremeVerticalNrOfEvents;
    /** Maximum allowed number of activations of Forced Braking in vertical
     *  direction. */
    uint16_t usFESBrakingVerticalNrOfEvents;
   /** Maximum allowed number of activations of Forced Braking in
     * horizontal direction. */
    uint16_t usFESBrakingHorizontalNrOfEvents;
    /** Maximum allowed number of Overloads detected for LCU. */
    uint16_t usOverloadNrOfEvents;
    /** Maximum allowed number of Crushed Loads detected for LCU. */
    uint16_t usCrushloadNrOfEvents;
    /** Maximum allowed number of Jumps. */
    uint16_t usJumpingPassengerNrOfEvents;
    /** Maximum allowed number of times, LCU enteres buffer area with v > 0.5 m/s */
    uint16_t usLcuTooFastInBufferAreaNrOfEvents;
    /** Maximum allowed number of detected load changing cycles */
    uint32_t ulPayloadChangeNrOfEvents;
    /** Maximum allowed number of activations of step 1 of LCUBrake when LCU is stopped */
    uint32_t ulBrk1ActivationWhenStoppedNrOfEvents;
    /** Maximum allowed number of activations of step 2 of LCUBrake when LCU is stopped */
    uint32_t ulBrk2ActivationWhenStoppedNrOfEvents;
    /** Maximum allowed number of activations of step 3 of LCUBrake when LCU is stopped */
    uint32_t ulBrk3ActivationWhenStoppedNrOfEvents;
    /** Maximum allowed number of activations of step 4 of LCUBrake when LCU is stopped */
    uint32_t ulBrk4ActivationWhenStoppedNrOfEvents;

    /** Maximum allowed number of activations of step 1 of LCUBrake when LCU is moving upwards */
    uint16_t usBrk1ActivationWhenMovingUpNrOfEvents;
    /** Maximum allowed number of activations of step 2 of LCUBrake when LCU is moving upwards */
    uint16_t usBrk2ActivationWhenMovingUpNrOfEvents;
    /** Maximum allowed number of activations of step 3 of LCUBrake when LCU is moving upwards */
    uint16_t usBrk3ActivationWhenMovingUpNrOfEvents;
    /** Maximum allowed number of activations of step 4 of LCUBrake when LCU is moving upwards */
    uint16_t usBrk4ActivationWhenMovingUpNrOfEvents;

    /** Maximum allowed number of activations of controlled braking of step 1 of LCUBrake  */
    uint16_t usCESBrakingBrk1NrOfEvents;
    /** Maximum allowed number of activations of controlled braking of step 2 of LCUBrake  */
    uint16_t usCESBrakingBrk2NrOfEvents;
    /** Maximum allowed number of activations of controlled braking of step 3 of LCUBrake  */
    uint16_t usCESBrakingBrk3NrOfEvents;
    /** Maximum allowed number of activations of controlled braking of step 4 of LCUBrake  */
    uint16_t usCESBrakingBrk4NrOfEvents;

    /** Maximum allowed number of activations of forced braking of step 1 of LCUBrake  */
    uint16_t usFESBrakingBrk1NrOfEvents;
    /** Maximum allowed number of activations of forced braking of step 2 of LCUBrake  */
    uint16_t usFESBrakingBrk2NrOfEvents;
    /** Maximum allowed number of activations of forced braking of step 3 of LCUBrake  */
    uint16_t usFESBrakingBrk3NrOfEvents;
    /** Maximum allowed number of activations of forced braking of step 4 of LCUBrake  */
    uint16_t usFESBrakingBrk4NrOfEvents;

} collectiveLoadMax_t;

/**
 * @brief           Parameters for stopping distance.
 * @requirements    SC_REQ-218562
 *
 * @remarks
 * Values are stored as 32 bit integers by upscaling the original floating point values with P6.
 *
 */
typedef struct {
    int32_t lK1;
    int32_t lK2;
    int32_t lOffset;
} stoppingDistanceParam_t;

/**
 * @brief           Stopping distance parameters
 * @requirements    SC_REQ-218562
 *
 * @remarks     see D00003987.pdf V00.02.00, 11.12.2020, Chapter 3.2 Stopping Area
 */
typedef struct {
    /** Lower stopping distance, moving downwards */
    stoppingDistanceParam_t LoSD_upwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS];

    /** Lower stopping distance, moving downwards */
    stoppingDistanceParam_t LoSD_downwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS];

    /** Upper stopping distance, moving downwards */
    stoppingDistanceParam_t USD_downwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS];

    /** Upper stopping distance, moving upwards */
    stoppingDistanceParam_t USD_upwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS];

    /** Left stopping distance, moving rightwards */
    stoppingDistanceParam_t LeSD_rightwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS];

    /** Left stopping distance, moving leftwards */
    stoppingDistanceParam_t LeSD_leftwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS];

    /** Right stopping distance, moving rightwards */
    stoppingDistanceParam_t RSD_rightwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS];

    /** Right stopping distance, moving leftwards */
    stoppingDistanceParam_t RSD_leftwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS];
} stoppingDistanceParamSet_t;

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
CfgLcu_initialize(void);

extern int32_t
CfgLcu_get(enCfgParam_t tParam, uint32_t ulIndex);

extern const lcuPhysicalConstraints_t*
CfgLcu_getPhysicalConstraints(enLcuType_t tLcuType);

extern const collectiveLoadMax_t*
CfgLcu_getCollectiveLoadMax(void);

extern enLcuType_t
CfgLcu_getLcuType(uint32_t ulIndex);

extern const stoppingDistanceParamSet_t*
CfgLcu_getStoppingDistanceParamSet(void);

#endif /* CFGLCU_H_ */

/** @}  End of this module */
