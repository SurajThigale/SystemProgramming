/** \addtogroup UtilBase
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      UtilBase.h
 *  @brief     Common data definitions and data types
 *  @author    Marius Groeger
 *  @reference FubUtils Module Design Specification
 *
 */

#ifndef SRC_FUBS_COMMON_INCLUDE_UTILBASE_H_
#define SRC_FUBS_COMMON_INCLUDE_UTILBASE_H_

/*
 * INCLUDEFILES
 */
#include <stdint.h>
#include <stdbool.h>

#include "CfgLimits.h"
#include "UtilBaseTypes.h"
#include "UtilOpMode.h"
#include "CfgLcu.h"

/*
 * MACRO DEFINITIONS
 */

/**
 * @brief        Absolute value function
 *
 * @return       Absolute value of input parameter
 *
 * @requirements
 *
 * SC_REQ-217387 FBRQ - Commonly required functionality of UtilBase module
 */
#define MATH_ABS(X) (((X) >= 0) ? (X) : -(X))

/**
 * @brief        Sign function
 *
 * @return       1 for positive arguments, -1 for negative arguments, 0 for argument 0
 *
 * @requirements
 *
 * SC_REQ-217387 FBRQ - Commonly required functionality of UtilBase module
 */
#define MATH_SGN(X) (((X) > 0) ? 1 : (((X) < 0) ? -1 : 0))

/**
 * @brief        Calculate the arithmetic mean out of two (signed) values
 *
 * @return       The mean value
 *
 * @requirements
 *
 * SC_REQ-217387 FBRQ - Commonly required functionality of UtilBase module
 */
#define MATH_MEAN(A,B) (((A) + (B)) / 2)

/**
 * @brief        Calculate the arithmetic mean out of two (unsigned) values
 *
 * @return       The mean value
 *
 * @requirements
 *
 * SC_REQ-217387 FBRQ - Commonly required functionality of UtilBase module
 */
#define MATH_MEAN_UNSIGNED(A,B) (((A) + (B)) / 2U)

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
UtilBase_initialize(void);

extern void
UtilBase_calculateSATs(
    const point_t* ptLcuPosition,
    int16_t lLcuVelocityMmPerS,
    enTravelDirection_t tLcuTravelDirection,
    const lcuPhysicalConstraints_t* ptLcuConstraints,
    lcuSats_t* ptLcuSats);

extern enStoppingScenario_t
UtilBase_getStoppingScenarioForAreaOverlapping(
    const lcuSats_t* ptLcuSats,
    const area_t* ptOther);

extern bool
UtilBase_checkPointInsideArea(
    const area_t* ptArea,
    const point_t* ptPoint);

extern bool
UtilBase_checkLcuIsCoincidentWithXc(
    const point_t* ptLcuCenterAlignment,
    const point_t* ptXcRotation,
    enLcuType_t tLcuType);

extern bool
UtilBase_checkAreaOverlap(
    const area_t* ptA1,
    const area_t* ptA2);

extern void
UtilBase_progressTime(timeDiffMs_t tTimePassed);

extern void
UtilBase_startTimer(timerObject_t* ptTimer);

extern void
UtilBase_startTimerIfNotRunning(timerObject_t* ptTimer);

extern void
UtilBase_stopTimer(timerObject_t* ptTimer);

extern bool
UtilBase_checkTimerExceedsThreshold(
    const timerObject_t* ptTimer,
    timeDiffMs_t tSpan);

extern timeMs_t
UtilBase_getElapsedTime(const timerObject_t* ptTimer);

extern bool
UtilBase_InitializeLcuPositionTranslation(void);

extern void
UtilBase_translateLcuPositionToCartesian(
    uint32_t ulRelPosOnTrack,
    uint8_t ucTrackId,
    point_t* ptResult);

bool
UtilBase_isLcuInSet(
    const subSystemsSet_t* ptSubSystems,
    tLcuId_t tLcuId);

bool
UtilBase_isXcInSet(
    const subSystemsSet_t* ptSubSystems,
    tXcId_t tXcId);

void
UtilBase_addLcuToSet(
    subSystemsSet_t* ptSubSystems,
    tLcuId_t tLcuId);

void
UtilBase_removeLcuFromSet(
    subSystemsSet_t* ptSubSystems,
    tLcuId_t tLcuId);

void
UtilBase_addXcToSet(
    subSystemsSet_t* ptSubSystems,
    tXcId_t tXcId);

void
UtilBase_removeXcFromSet(
    subSystemsSet_t* ptSubSystems,
    tXcId_t tXcId);

void
UtilBase_clearAllInSet(subSystemsSet_t* ptSubSystems);

void
UtilBase_addAllLcusToSet(subSystemsSet_t* ptSubSystems);

void
UtilBase_addAllXcsToSet(subSystemsSet_t* ptSubSystems);

#endif /* SRC_FUBS_COMMON_INCLUDE_UTILBASE_H_ */

/** @}  End of this module */
