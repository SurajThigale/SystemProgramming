/** \defgroup GenericCfgXc GenericCfgXc SW Module Specification
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgXc.h
 *  @brief     Provides access to Safety Node specific CfgXc configuration parameter data.
 *  @author    Frank Meier
 *
 */

#ifndef CFGXC_H_
#define CFGXC_H_

/*
 * INCLUDEFILES
 */
#include "UtilBase.h"
#include "CfgTypes.h"

/*
 * MACRO DEFINITIONS
 */
/**
 * @brief           Number of rotational tripping speed pattern infos.
 * @requirements    SC_REQ-218574
 */
#define XC_NUMBER_OF_TSPR_INFOS     4U

/*
 * TYPE DEFINITIONS
 */
/**
 * @brief           XC Info
 * @requirements    SC_REQ-218573
 */
typedef struct {
    point_t tCenter; /**< XC center point */
    area_t tArea; /**< XC area */
} xcInfo_t;

/**
 * @brief           Parameters for stopping distance.
 * @requirements    SC_REQ-245563
 */
typedef struct {
    int32_t lXC_K1;
    int32_t lXC_K2;
    int32_t lXC_Offset;
} xcSD_Param_t;
/**
 * @brief           Stopping distance parameters for XC
 * @requirements    SC_REQ-245563
 */
typedef struct {
    /** XC stopping distance, clockwise */
    xcSD_Param_t tXC_SD_clockwise;
    /** XC stopping distance, counterclockwise */
    xcSD_Param_t tXC_SD_counterclockwise;
} xcSD_ParamSet_t;

/**
 * @brief           Rotational tripping speed pattern info
 * @requirements    SC_REQ-218573
 */
//typedef struct {
//    /** maximum angular velocity at starting angle */
//    int32_t lAngularVelocityMaxAtStart;
//    /** maximum angular velocity at ending angle */
//    int32_t lAngularVelocityMaxAtEnd;
//    uint16_t usStartAngle; /**< y starting angle */
//    uint16_t usEndAngle; /**< y ending angle */
//} tsprInfo_t;

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
CfgXc_initialize(void);

extern int32_t
CfgXc_get(enCfgParam_t tParam, uint32_t ulIndex);

extern const xcInfo_t*
CfgXc_getXcInfo(tXcId_t tXcId);

//extern const tsprInfo_t*
//CfgXc_getTsprInfo(uint32_t ulIndex);


#endif /* CFGXC_H_ */

/** @}  End of this module */
