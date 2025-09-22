/** \addtogroup GenericCfgConstants
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      CfgLimits.h
 *  @brief     General system limit definitions.
 *  @author    Frank Meier
 *
 */

#ifndef CFGLIMITS_H_
#define CFGLIMITS_H_

/*
 *  MACRO DEFINITIONS
 */

/**
 * @brief           Maximum length of vertical travel distance that can be covered by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_TRAVELDISTANCE_VERTICAL_MAX 1600U

/**
 * @brief           Maximum number of LCUs inside one horizontal track.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_HORIZONTAL_TRACKS_MAX 11U

/**
 * @brief           Maximum number of Master SNs that can be covered by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_MA_MAX 1u

/**
 * @brief           Maximum number of XChangers that can be covered by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_XC_MAX 8U

/**
 * @brief           Maximum number of IO collectors that can be covered by safeCore
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_IOC_MAX 16U

/**
 * @brief           Maximum number of vertical tracks that can be covered by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_VERTICAL_TRACKS_MAX 4U

/**
 * @brief           Maximum length of end segments of horizontal tracks which are not
 * between vertical tracks.
 * @requirements    SC_REQ-218560
 */
#define MULTI_LENGTH_END_HORIZONTAL_MAX 16U

/**
 * @brief           Maximum distance between vertical tracks connected with horizontal track.
 * @requirements    SC_REQ-218560
 */
#define MULTI_DISTANCE_VERTICAL_TRACKS_MAX 16U

/**
 * @brief           Maximum length of horizontal travel distance that can be covered by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_TRAVELDISTANCE_HORIZONTAL_MAX 80U

/**
 * @brief           Maximum total number of LCUs that can be covered by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_LCU_MAX 8U

/**
 * @brief           Maximum number of LCUs inside one vertical track.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_LCU_VERTICAL_TRACK_MAX 20U

/**
 * @brief           Maximum number of LCUs inside one horizontal track.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_LCU_HORIZONTAL_TRACK_MAX 20U

/**
 * @brief           Maximum number of Fire Compartments supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_FIRECOMPARTMENTS_MAX 4U

/**
 * @brief           Maximum number of Garage Areas supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_GARAGEAREAS_MAX 2U

/**
 * @brief           Maximum number of Maintenance Areas supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_MAINTENANCEAREAS_MAX 1U

/**
 * @brief           Maximum number of Propulsion Sections supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_PROPULSIONSECTIONS_MAX 1600U

/**
 * @brief           Maximum number of Wall Panel Areas supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_WALLPANELAREAS_MAX 8U

/**
 * @brief           Maximum number of Working Areas supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_WORKINGAREAS_MAX 2U

/**
 * @brief           Maximum number of Working Areas supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_WORKINGAREATYPES_MAX 4U

/**
 * @brief           Maximum number of Well Water Areas supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_WELLWATERAREAS_MAX 4U

/**
 * @brief           Maximum number of Well Doors supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_WELLDOORS_MAX 512U

/**
 * @brief           Maximum number of Well Segments supported by safeCore.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_WELLSEGMENTS_MAX  15U

/**
 * @brief           Maximum number of Walkways supported by safeCore
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_WALKWAYS_MAX    11U

/**
 * @brief           Maximum number of Underpower sensors supported by safeCore
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_UNDERPOWERSENSOR_MAX 1U

/**
 * @brief           Maximum number of tracks in vertical and horizontal direction.
 * @requirements    SC_REQ-218560
 */
#define MULTI_NUMBER_OF_TRACKS_MAX  (MULTI_NUMBER_OF_HORIZONTAL_TRACKS_MAX + MULTI_NUMBER_OF_VERTICAL_TRACKS_MAX)

/**
 * @brief           Maximum number of well temperature sensors supported by safeCore
 * @requirements
 *
   SC_REQ-218560
 * SC_REQ-221156 - FBRQ - IORemoteIn_Ioc - Provide datastructure for IOC sensor values
 *
 * @details
 *
 * The maximum number of well temperature sensors is specified by tkE to be identical to the number
 * of tracks supported by safeCore.
 */
#define MULTI_NUMBER_OF_WELLTEMPERATURESENSOR_MAX 2U

/**
 * @brief        Number of brake steps of a LCU
 *
 * @requirements SC_REQ-218560
 */
#define MULTI_NUMBER_OF_BRAKE_STEPS_PER_LCU 4U

/**
 * @brief        Number of brake calipers per brake step
 *
 * @requirements SC_REQ-218560
 */
#define MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_STEP 2U

/**
 * @brief        Number of brake calipers of a LCU
 * 
 * @requirements SC_REQ-218560
 */
#define MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU \
    (MULTI_NUMBER_OF_BRAKE_STEPS_PER_LCU * MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_STEP)

/**
 * @brief           Current version of this configuration
 * @requirements    SC_REQ-218560
 */
#define MULTI_CONFIG_VERSION 0u

#endif /* CFGLIMITS_H_ */

/** @}  End of this module */
