/** \addtogroup UtilBase
 *  @{
 *
 *  @copyright (c) 2022: infoteam SET GmbH, all rights reserved
 *
 *  @file      UtilOpMode.h
 *  @brief     File UtilOpMode.h contains the global API of module General Operation Mode Manager.
 *  @author    Juergen Scherg, Daniel Mueller
 *  @reference Generic Utilities Fub Functional Block Specification
 *
 */

#ifndef UTIL_OPMODE_H
#define UTIL_OPMODE_H

/*
 * INCLUDE FILES
 */

#include <stdbool.h>

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief
 * Defines all valid values for the requested and current operation mode.
 *
 * @requirements
 * SC_REQ-218628 - FBRQ - UtilOpMode Supported operation mode
 *
 */
typedef enum {
    OPMODE_NOT_OPER = 0,
    /** Used to perform hardware startup test phase. */
    OPMODE_INIT_HW = 1,
    /** Used to perform software startup test phase. */
    OPMODE_INIT_SW = 2,
    /** Used to perform startup tests when SW is operational. */
    OPMODE_INIT_TEST_SW = 3,
    /** Used to perform configuration validation steps.*/
    OPMODE_VALIDATION = 4,
    /** Used to establish a job specific configuration. */
    OPMODE_CONFIGURE = 5,
    /** Normal operation of system. */
    OPMODE_NORMAL = 6,
    /** Used to perform periodic tests. */
    OPMODE_TEST_PERIODIC = 7,
    /** Used to perform inpection operations, aka manual control. */
    OPMODE_INSPECTION = 8,
    /** Operation mode when fire is detected. Only applicable to Master and Lcu*/
    OPMODE_FIRE = 9,
    /** Operation mode when Safety Node enters fail safe state. */
    OPMODE_FAIL_SAFE = 10,
    /** Used for movement by sliding brake of Lcu. Only applicable to Lcu */
    OPMODE_SLIDING_BRAKE = 11,
} enOpMode_t;

/**
 * @brief
 * Enum enUtilOpMode_CompleteStatus_t contains all valid status values for the transition from
 * current operation mode to the new requested operation mode.
 *
 * @requirements
 * SC_REQ-218633 - FBRQ - UtilOpMode Set operation mode completion status
 */
typedef enum {
    UtilOpMode_NotComplete = 0, /**< Transition from current operation mode to new requested
     * operation mode is not yet completed. */
    UtilOpMode_Complete /**< Transition from current operation mode to new requested
     * operation is not yet completed. */
} enUtilOpMode_CompleteStatus_t;

/**
 * @brief
 * Enum for describing status of init SW tests
 *
 * @requirements
 * SC_REQ-222090 - FBRQ - UtilOpMode Provide datatype enInitSwTestStatus_t
 * */
typedef enum {
    INIT_SW_TESTS_ONGOING = 0,
    INIT_SW_TESTS_FAILED,
    INIT_SW_TESTS_SUCCESS,
} enInitSwTestStatus_t;

/**
 * @brief
 * Enum for describing status of periodic tests
 *
 * @requirements
 * SC_REQ-222091 - FBRQ - UtilOpMode Provide datatype enPeriodicTestStatus_t
 */
typedef enum {
    PERIODIC_TEST_ONGOING = 0,/**< PERIODIC_TEST_ONGOING */
    PERIODIC_TEST_FAILED,     /**< PERIODIC_TEST_FAILED */
    PERIODIC_TEST_SUCCESS,    /**< PERIODIC_TEST_SUCCESS */
} enPeriodicTestStatus_t;


/**
 * @brief
 * Enum for describing status of validation tests
 *
 * @requirements
 * SC_REQ-222092 - FBRQ - UtilOpMode Provide datatype enValidationTestStatus_t
 * */
typedef enum {
    VALIDATION_TESTS_ONGOING = 0,
    VALIDATION_TESTS_FAILED,
    VALIDATION_TESTS_SUCCESS,
} enValidationTestStatus_t;
/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
UtilOpMode_init(void);
extern void
UtilOpMode_setRequestedOpMode(enOpMode_t tOpMode);
extern enOpMode_t
UtilOpMode_getRequestedOpMode(void);
extern enOpMode_t
UtilOpMode_getOpMode(void);
extern enUtilOpMode_CompleteStatus_t
UtilOpMode_getOpModeComplete(void);
extern void
UtilOpMode_setOpModeComplete(void);
extern void
UtilOpMode_establishRequestedOperationMode(void);

#endif /* UTIL_OPMODE_H */

/** @}  End of this module */
