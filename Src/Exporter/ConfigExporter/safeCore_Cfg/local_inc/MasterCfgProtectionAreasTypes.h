/** \addtogroup MasterCfgProtectionAreas
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgProtectionAreasTypes.h
 *  @brief     Data type that holds all the Master's CfgProtectionArea module configuration data.
 *  @author    Frank Meier
 *
 */

#ifndef MASTERCFGPROTECTIONAREASTYPES_H_
#define MASTERCFGPROTECTIONAREASTYPES_H_

/*
 * INCLUDEFILES
 */

#include "CfgProtectionAreas.h"
#include "CfgGeneralFubInternalTypes.h"

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Configuration data of Master CfgProtectionAreas module.
 * @requirements    SC_REQ-218644
 */
typedef struct { /*lint !e9045 */
    protectionArea_t tFireCompartments[MULTI_NUMBER_OF_FIRECOMPARTMENTS_MAX];
    protectionArea_t tGarageAreas[MULTI_NUMBER_OF_GARAGEAREAS_MAX];
    protectionArea_t tIocAffectedArea[MULTI_NUMBER_OF_IOC_MAX];
    protectionArea_t tMaintenanceAreas[MULTI_NUMBER_OF_MAINTENANCEAREAS_MAX];
    /** Configured propulsion sections. The sections are found track after track in this array,
     * while they are sorted in each track (by coordinates of top left corner, first by z-coordinate
     * descending, then by y-coordinate ascending). As exchanger areas lie on two tracks, the
     * corresponding sections are duplicated in the configuration. This leads to an increase of the
     * maximum sections by addition of MULTI_NUMBER_OF_XC_MAX possible duplicates. */
    protectionArea_t tPropulsionSections[MULTI_NUMBER_OF_PROPULSIONSECTIONS_MAX +
                                         MULTI_NUMBER_OF_XC_MAX];
    /**
     * Start indices of the propulsion sections for a track in tPropulsionSections. A track track_id
     * includes the propulsion sections from lPropulsionSectionTrackStartIndex[track_id] (included)
     * to lPropulsionSectionTrackStartIndex[track_id + 1] (excluded).
     */
    int32_t lPropulsionSectionTrackStartIndex[MULTI_NUMBER_OF_TRACKS_MAX + 1U];
    protectionArea_t tSwivelPlatforms[MULTI_NUMBER_OF_XC_MAX];
    protectionArea_t tTranslationalMovementAreas[MULTI_NUMBER_OF_TRACKS_MAX];
    protectionArea_t tWalkwayAreas[MULTI_NUMBER_OF_HORIZONTAL_TRACKS_MAX];
    protectionArea_t tWallPanelAreas[MULTI_NUMBER_OF_WALLPANELAREAS_MAX];
    protectionArea_t tWellSegmentAreas[MULTI_NUMBER_OF_WELLSEGMENTS_MAX];

    /* beware: MULTI_NUMBER_OF_WELLDOORS_MAX is the number WellDoors + WellAccessPorts together.
     * tWellAreas contains WellDoors first, then WellAccessPorts */
    protectionArea_t tWellDoorAreas[MULTI_NUMBER_OF_WELLDOORS_MAX];

    protectionArea_t tWellLocks[MULTI_NUMBER_OF_XC_MAX];
    protectionArea_t tWellWaterAreas[MULTI_NUMBER_OF_WELLWATERAREAS_MAX];

    int32_t lWorkingArea_ZPoint2Top[MULTI_NUMBER_OF_WORKINGAREATYPES_MAX];
    int32_t lWorkingArea_ZPoint2Bottom[MULTI_NUMBER_OF_WORKINGAREATYPES_MAX];
    int32_t lWorkingArea_YPoint2Right[MULTI_NUMBER_OF_WORKINGAREATYPES_MAX];
    int32_t lWorkingArea_YPoint2Left[MULTI_NUMBER_OF_WORKINGAREATYPES_MAX];
    int32_t lWorkingArea_ExtensionTop;
    int32_t lWorkingArea_ExtensionBottom;
    int32_t lWorkingArea_ExtensionRight;
    int32_t lWorkingArea_ExtensionLeft;

} masterCfgProtectionAreas_t;

#endif /* MASTERCFGPROTECTIONAREASTYPES_H_ */

/** @}  End of this module */
