#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "UtilBase_SATs.h"

#include "MasterCfgGeneralTypes.h"
#include "LcuCfgGeneralTypes.h"
#include "XcCfgGeneralTypes.h"
#include "IocCfgGeneralTypes.h"

#define AREA_WIDTH(a) ((a)->tBottomRight.lY - (a)->tTopLeft.lY)
#define AREA_HEIGHT(a) ((a)->tTopLeft.lZ - (a)->tBottomRight.lZ)

uint32_t
CRC32(const void* pvAddr, uint32_t ulSize);

/*
 * See D00003987.pdf V00.02.00, 11.12.2020
 * Chapter 3.2 Stopping Area
 * Figure 14 stopping curve parameters of lower stopping distance
 */

static stoppingDistanceParamSet_t tStoppingDistanceParamSet = {
    /* LoSD_upwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS]; */
    {
        { UPSCALE_K1(-0.1010925190), UPSCALE_K2(0.0588265046), UPSCALE_KOFFSET(-1.8744062733 + -1.3487999703) },
        { UPSCALE_K1(-0.0209477088), UPSCALE_K2(0.6445716628), UPSCALE_KOFFSET(-2.1429677904 + -0.0030000000) },
        { UPSCALE_K1(-0.0224493615), UPSCALE_K2(0.3949904581), UPSCALE_KOFFSET(-1.0335022015 + -0.0030000000) }
    },
    /* LoSD_downwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS]; */
    {
        { UPSCALE_K1(-0.1010925190), UPSCALE_K2(0.0588265046), UPSCALE_KOFFSET(-1.8744062733 + -1.3487999703) },
        { UPSCALE_K1(-0.0209477088), UPSCALE_K2(0.6445716628), UPSCALE_KOFFSET(-2.1429677904 + -0.0030000000) },
        { UPSCALE_K1(-0.0224493615), UPSCALE_K2(0.3949904581), UPSCALE_KOFFSET(-1.0335022015 + -0.0030000000) }
    },

    /* USD_downwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS]; */
    {
        { UPSCALE_K1(0.0000000000), UPSCALE_K2(0.0000000000), UPSCALE_KOFFSET(0.0000022585 + 0.3030000000) },
        { UPSCALE_K1(0.0000000000), UPSCALE_K2(0.0000000000), UPSCALE_KOFFSET(0.0000000000 + 0.3030000000) },
        { UPSCALE_K1(0.0000000000), UPSCALE_K2(0.0000000000), UPSCALE_KOFFSET(0.0000000000 + 0.3030000000) }
    },

    /* USD_upwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS]; */
    {
        { UPSCALE_K1(0.1019367049), UPSCALE_K2(0.3350009584), UPSCALE_KOFFSET(0.0000000000 + 0.3030000000) },
        { UPSCALE_K1(0.0489843376), UPSCALE_K2(0.3319998532), UPSCALE_KOFFSET(0.0000000000 + 0.3030000000) },
        { UPSCALE_K1(0.0489843376), UPSCALE_K2(0.3319998532), UPSCALE_KOFFSET(0.0000000000 + 0.3030000000) }
    },

    /* LeSD_rightwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS]; */
    {
        { UPSCALE_K1( 0.0000000000), UPSCALE_K2(0.0000000000), UPSCALE_KOFFSET(-0.1931457215 +  0.0000000000) },
        { UPSCALE_K1( 0.0000000000), UPSCALE_K2(0.0000000000), UPSCALE_KOFFSET(-0.2300038324 +  0.0000000000) },
        { UPSCALE_K1( 0.0000000000), UPSCALE_K2(0.0000000000), UPSCALE_KOFFSET(-0.1306168673 +  0.0000000000) }
    },

    /* LeSD_leftwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS]; */
    {
        { UPSCALE_K1(-0.3386629726), UPSCALE_K2(0.0538186801), UPSCALE_KOFFSET(-0.1931457215 + -0.1708800032) },
        { UPSCALE_K1(-0.0455790336), UPSCALE_K2(0.4351403080), UPSCALE_KOFFSET(-0.2300038324 + -0.0030000000) },
        { UPSCALE_K1(-0.0470074023), UPSCALE_K2(0.2658220328), UPSCALE_KOFFSET(-0.1306168673 + -0.0030000000) }
    },

    /* RSD_rightwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS]; */
    {
        { UPSCALE_K1( 0.3386629726), UPSCALE_K2(0.0538186801), UPSCALE_KOFFSET( 0.1931457215 +  0.1708800032) },
        { UPSCALE_K1( 0.0455790336), UPSCALE_K2(0.4351403080), UPSCALE_KOFFSET( 0.2300038324 +  0.0030000000) },
        { UPSCALE_K1( 0.0470074023), UPSCALE_K2(0.2658220328), UPSCALE_KOFFSET( 0.1306168673 +  0.0030000000) }
    },

    /* RSD_leftwards_BS[LCU_NUMBER_OF_STOPPING_DISTANCE_PARAMS]; */
    {
        { UPSCALE_K1( 0.0000000000), UPSCALE_K2(0.0000000000), UPSCALE_KOFFSET( 0.1931457215 + 0.0000000000) },
        { UPSCALE_K1( 0.0000000000), UPSCALE_K2(0.0000000000), UPSCALE_KOFFSET( 0.2300038324 + 0.0000000000) },
        { UPSCALE_K1( 0.0000000000), UPSCALE_K2(0.0000000000), UPSCALE_KOFFSET( 0.1306168673 + 0.0000000000) }
    }
};

static xcSD_ParamSet_t tStoppingDistanceParamSetXc[2] = {
    {
        /* tXC_SD_clockwise */
        {
            { UPSCALE_K1(-0.1010925190), UPSCALE_K2(0.0588265046), UPSCALE_KOFFSET(-1.8744062733 + -1.3487999703) }
        },
        /* tXC_SD_counterclockwise */
        {
            { UPSCALE_K1(-0.1010925190), UPSCALE_K2(0.0588265046), UPSCALE_KOFFSET(-1.8744062733 + -1.3487999703) }
        }
    },
    {
        /* tXC_SD_clockwise */
        {
            { UPSCALE_K1(0.123456789), UPSCALE_K2(0.987654321), UPSCALE_KOFFSET(2.22) }
        },
        /* tXC_SD_counterclockwise */
        {
            { UPSCALE_K1(0.111111111), UPSCALE_K2(0.999999999), UPSCALE_KOFFSET(3.33) }
        }
    }
};

/* calculate tLcu.cPositionTranslation_TrackIsVertical[] and
 * tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack[] from track definition
 */
static void
fixupTrackInfo(masterCfg_t* mstCfg) {
    int32_t numTracks = mstCfg->tConstants.lMULTI_NumberOfHorizontalTracksInstalled +
        mstCfg->tConstants.lMULTI_NumberOfVerticalTracksInstalled;
    for (int32_t i = 0; i < numTracks; ++i) {
        const area_t* ptArea = &mstCfg->tProtectionAreas.tTranslationalMovementAreas[i].tArea;
        int32_t lWidth = AREA_WIDTH(ptArea);
        int32_t lHeight = AREA_HEIGHT(ptArea);
        if (lHeight > lWidth) {
            /* vertical track: relPosOnTrack from LCU is Z, Y is just in the center of the track */
            mstCfg->tLcu.cPositionTranslation_TrackIsVertical[i] = true;
            mstCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack[i] =
                ptArea->tTopLeft.lY + lWidth / 2;
        }
        else {
            /* horizontal track: relPosOnTrack from LCU is Y, Z is just in the center of the track */
            mstCfg->tLcu.cPositionTranslation_TrackIsVertical[i] = false;
            mstCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack[i] =
                ptArea->tBottomRight.lZ + lHeight / 2;
        }
    }
}

static void
clearIOCConfig(masterCfg_t* mstCfg) {
    for (tIocId_t tIocIndex = 0; tIocIndex < MULTI_NUMBER_OF_IOC_MAX; tIocIndex++) {
        /* Initialize all IOC Sensors to IOC_ENTRY_NOT_POPULATED_XXX */
        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WDSS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellDoorLockedClosed[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT16;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WAPSS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellAccessPortClosed[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT16;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WWLSS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tWaterInWell[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_PIMASS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tPersonInMaintenanceArea[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_PIGASS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tPersonInGarage[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_PIWSS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tPersonInWalkway[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WPSS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tWallPanelClosed[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WSES_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellSegmentEmergencyStopTriggered[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT16;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_FCSS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tFireCompartmentRetracted[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WTSS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellTemperature[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_UDSS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].ucUnderpowerDetectionUsed[ucSensorIndex] = false;
        }

        for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WSSS_AVAILABLE; ucSensorIndex++) {
            mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellSegmentAccessPortNotClosed[ucSensorIndex] = 0;
        }
    }
}

/*
 * MCA Configuration on HIL
 * Reference document: MULTI_G1.0_MCA_2022_07_07
 *
 * 1 LCU
 * 0 XC
 * 2 IOC
 * 1 vertical Track
 *
 *
 * +-----------+ ----- 300000 mm
 * |           |
 * |           |
 * |           |
 * |           |
 * +-----------+ -----  23000 mm
 * |  DOOR#2   |
 * +-----------+ -----  20000 mm
 * |           |
 * |  PROP#2   |
 * |           |
 * +-----------+ -----   3000 mm
 * |  DOOR#1   |
 * +-----------+ -----      0 mm
 * |           |
 * |  PROP#1   |
 * |           |
 * +-----------+ -----  16500 mm
 * |  DOOR#0   |
 * +-----------+ ----- -19500 mm
 * |           |
 * |  PROP#0   |
 * |           |
 * +-----------+ ----- -26500 mm
 * |  RESCUE   |
 * |   DOOR    |
 * +-----------+ ----- -29500 mm
 */
static void
populateMasterConfig_stat_hil(masterCfg_t* mstCfg) {
    mstCfg->tValidity = CfgValidity_Valid;
    mstCfg->tCfgVersion = MULTI_CONFIG_VERSION;

    mstCfg->tConstants.lMULTI_NumberOfLCUInstalled = 1;
    mstCfg->tConstants.lMULTI_NumberOfXCInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfIOCInstalled = 2;
    mstCfg->tConstants.lMULTI_NumberOfVerticalTracksInstalled = 1;
    mstCfg->tConstants.lMULTI_NumberOfHorizontalTracksInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfFireCompartmentsInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfGarageAreasInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfMaintenanceAreasInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfPropulsionSectionsInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfWallPanelAreasInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfWalkwayAreasInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfWellWaterAreasInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfWellDoorsInstalled = 3;
    mstCfg->tConstants.lMULTI_NumberOfWellAccessPortsInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfWellLocksInstalled = 0;
    mstCfg->tConstants.lMULTI_NumberOfWellSegmentsInstalled = 2;

    /* stopping distances */
    mstCfg->tLcu.tStoppingDistanceParamSet = tStoppingDistanceParamSet;

    area_t lcuSize;
    /* LCU size as per firstExample.xml, including equipment */
    lcuSize.tTopLeft.lY   = -(3000/2) - 100;
    lcuSize.tTopLeft.lZ   = (3000/2) + 300;
    lcuSize.tBottomRight.lY = (3000/2) + 100;
    lcuSize.tBottomRight.lZ = -(3000/2) - 200;
    mstCfg->tLcu.tPhysicalConstraints[LcuType_Econonmy].tSize = lcuSize;
    mstCfg->tLcu.tPhysicalConstraints[LcuType_Econonmy].sPivotBearingOffsetY = 0;
    mstCfg->tLcu.tPhysicalConstraints[LcuType_Econonmy].sPivotBearingOffsetZ = 0;
    mstCfg->tLcu.tPhysicalConstraints[LcuType_Premium].tSize = lcuSize;
    mstCfg->tLcu.tPhysicalConstraints[LcuType_Premium].sPivotBearingOffsetY = 0;
    mstCfg->tLcu.tPhysicalConstraints[LcuType_Premium].sPivotBearingOffsetZ = 0;

    /* left and right coordinate of track #0 used in many of the following definitions */
    const int32_t YL = 3000 - 200;
    const int32_t YR = 3000 + 3000 + 200;

    /* Track #0 */
    mstCfg->tConstants.lMULTI_NumberOfVerticalTracksInstalled++;
    mstCfg->tProtectionAreas.tTranslationalMovementAreas[0].tType = ProtectionArea_TranslationalMovement;
    mstCfg->tProtectionAreas.tTranslationalMovementAreas[0].tArea.tBottomRight.lY = YR;
    mstCfg->tProtectionAreas.tTranslationalMovementAreas[0].tArea.tBottomRight.lZ = -29500;
    mstCfg->tProtectionAreas.tTranslationalMovementAreas[0].tArea.tTopLeft.lY     = YL;
    mstCfg->tProtectionAreas.tTranslationalMovementAreas[0].tArea.tTopLeft.lZ     = 30000;

    /* Propulsion section #0 for track #0 */
    mstCfg->tConstants.lMULTI_NumberOfPropulsionSectionsInstalled++;
    mstCfg->tProtectionAreas.tPropulsionSections[0].tType = ProtectionArea_PropulsionSection;
    mstCfg->tProtectionAreas.tPropulsionSections[0].tArea.tBottomRight.lY = YR;
    mstCfg->tProtectionAreas.tPropulsionSections[0].tArea.tBottomRight.lZ = -26500;
    mstCfg->tProtectionAreas.tPropulsionSections[0].tArea.tTopLeft.lY = YL;
    mstCfg->tProtectionAreas.tPropulsionSections[0].tArea.tTopLeft.lZ = -19500;

    /* Propulsion section #1 for track #0 */
    mstCfg->tConstants.lMULTI_NumberOfPropulsionSectionsInstalled++;
    mstCfg->tProtectionAreas.tPropulsionSections[1].tType = ProtectionArea_PropulsionSection;
    mstCfg->tProtectionAreas.tPropulsionSections[1].tArea.tBottomRight.lY = YR;
    mstCfg->tProtectionAreas.tPropulsionSections[1].tArea.tBottomRight.lZ = 16500;
    mstCfg->tProtectionAreas.tPropulsionSections[1].tArea.tTopLeft.lY = YL;
    mstCfg->tProtectionAreas.tPropulsionSections[1].tArea.tTopLeft.lZ = 50000 + 3000 + 500;

    /* Propulsion section #2 for track #0 */
    mstCfg->tConstants.lMULTI_NumberOfPropulsionSectionsInstalled++;
    mstCfg->tProtectionAreas.tPropulsionSections[2].tType = ProtectionArea_PropulsionSection;
    mstCfg->tProtectionAreas.tPropulsionSections[2].tArea.tBottomRight.lY = YR;
    mstCfg->tProtectionAreas.tPropulsionSections[2].tArea.tBottomRight.lZ = 3000;
    mstCfg->tProtectionAreas.tPropulsionSections[2].tArea.tTopLeft.lY = YL;
    mstCfg->tProtectionAreas.tPropulsionSections[2].tArea.tTopLeft.lZ = 20000;

    /* Start indices for track propulsion sections in tPropulsionSections */
    mstCfg->tProtectionAreas.lPropulsionSectionTrackStartIndex[0] = 0;
    mstCfg->tProtectionAreas.lPropulsionSectionTrackStartIndex[1] = 4;

    /* well segment #0 */
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tType = ProtectionArea_WellSegment;
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tArea.tBottomRight.lZ = -29500;
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tArea.tBottomRight.lY = YL;
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tArea.tTopLeft.lZ = 0;
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tArea.tTopLeft.lY = YR;
    mstCfg->tConstants.lMULTI_NumberOfWellSegmentsInstalled++;

    /* well segment #1 */
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tType = ProtectionArea_WellSegment;
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tArea.tBottomRight.lZ = 0;
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tArea.tBottomRight.lY = YL;
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tArea.tTopLeft.lZ = 30000;
    mstCfg->tProtectionAreas.tWellSegmentAreas[0].tArea.tTopLeft.lY = YR;
    mstCfg->tConstants.lMULTI_NumberOfWellSegmentsInstalled++;

    clearIOCConfig(mstCfg);

    tIocId_t tIocIndex;

    tIocIndex = 0;
    /* IOC#0 covers lower half */
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tType = ProtectionArea_IocAffectedArea;
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tBottomRight.lZ = -29500;
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tBottomRight.lY = YL;
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tTopLeft.lZ = 0;
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tTopLeft.lY = YR;

    /* handle bottom WTSS */
    mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellTemperature[0] = WTSS_GLOBAL_IDX_BOTTOM;

    /* Well Door #0 */
    mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellDoorLockedClosed[0] = 0;

    /* Well Well Segment Emergency Stop #0 */
    mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellSegmentEmergencyStopTriggered[0] = 0x0001; /* Segments #0 */

    tIocIndex = 0;
    /* IOC#0 covers upper half */
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tType = ProtectionArea_IocAffectedArea;
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tBottomRight.lZ = 0;
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tBottomRight.lY = YL;
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tTopLeft.lZ = 30000;
    mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tTopLeft.lY = YR;

    /* handle top WTSS */
    mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellTemperature[0] = WTSS_GLOBAL_IDX_TOP;

    /* Well Door #1, #2 */
    mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellDoorLockedClosed[0] = 1;
    mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellDoorLockedClosed[1] = 2;

    /* Well Well Segment Emergency Stop #1 */
    mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellSegmentEmergencyStopTriggered[0] = 0x0002; /* Segments #1 */

    fixupTrackInfo(mstCfg);
}

void
buildMasterConfig(masterCfg_t* mstCfg) {
    memset(mstCfg, 0, sizeof(masterCfg_t));
    populateMasterConfig_stat_hil(mstCfg);
    mstCfg->ulCrc = CRC32((const void *) &((uint8_t *)mstCfg)[sizeof(uint32_t)],
		sizeof(masterCfg_t) - sizeof(uint32_t));
}

void
buildLcuConfig(lcuCfg_t* lcuCfg, tLcuId_t tLcuId) {

    /* build a master config to clone from */
    masterCfg_t* mstCfg = malloc(sizeof(masterCfg_t));
    buildMasterConfig(mstCfg);

    memset(lcuCfg, 0, sizeof(lcuCfg_t));

    lcuCfg->tValidity = CfgValidity_Valid;
    lcuCfg->tCfgVersion = MULTI_CONFIG_VERSION;

    lcuCfg->tLcuId = tLcuId;

    /* populate lcuCfg using values from mstCfg */

    lcuCfg->tConstants.lMULTI_NumberOfGarageAreasInstalled = mstCfg->tConstants.lMULTI_NumberOfGarageAreasInstalled;
    lcuCfg->tConstants.lMULTI_NumberOfVerticalTracksInstalled = mstCfg->tConstants.lMULTI_NumberOfVerticalTracksInstalled;
    lcuCfg->tConstants.lMULTI_NumberOfHorizontalTracksInstalled = mstCfg->tConstants.lMULTI_NumberOfHorizontalTracksInstalled;
    lcuCfg->tLcu.cType = mstCfg->tLcu.cType[lcuCfg->tLcuId];
    memcpy(lcuCfg->tLcu.cPositionTranslation_TrackIsVertical, mstCfg->tLcu.cPositionTranslation_TrackIsVertical,
        sizeof(lcuCfg->tLcu.cPositionTranslation_TrackIsVertical));
    memcpy(lcuCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack,
        mstCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack,
        sizeof(lcuCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack));
    memcpy(lcuCfg->tLcu.tPhysicalConstraints, mstCfg->tLcu.tPhysicalConstraints,
        sizeof(lcuCfg->tLcu.tPhysicalConstraints));
    lcuCfg->tLcu.tStoppingDistanceParamSet = tStoppingDistanceParamSet;

    /* just copy fix protection areas from master */
    memcpy(lcuCfg->tProtectionAreas.tTranslationalMovementAreas,
        mstCfg->tProtectionAreas.tTranslationalMovementAreas,
        sizeof(lcuCfg->tProtectionAreas.tTranslationalMovementAreas));
    /*...*/

    lcuCfg->tLcu.lPrimaryBrakeStepFullActivationThreshold = 1000;

    /* ... for testing ... */
    lcuCfg->tLcu.lAps_maxAccelDeviation = 50;
    lcuCfg->tLcu.lAps_gravityOffset_1A = 9810;
    lcuCfg->tLcu.lAps_gravityOffset_2A = 9810;
    lcuCfg->tLcu.lAps_gravityOffset_1B = 9810;
    lcuCfg->tLcu.lAps_gravityOffset_2B = 9810;

    lcuCfg->tLcu.lSpeedHorizMin = 2;
    lcuCfg->tLcu.lSpeedVertMin = 2;

    /* brake escalation thresholds */
    lcuCfg->tLcu.lControlledDownDecelMin = 2;
    lcuCfg->tLcu.lControlledDownDecelModerate = 10;

    lcuCfg->tLcu.lNormalAutonomDownDecelMax = 2;
    lcuCfg->tLcu.lNormalAutonomDownDecelMin = 2;
    lcuCfg->tLcu.lNormalAutonomHorizDecelMax = 2;
    lcuCfg->tLcu.lNormalAutonomHorizDecelMin = 2;
    lcuCfg->tLcu.lNormalAutonomUpDecelMax = 2;
    lcuCfg->tLcu.lNormalAutonomUpDecelMin = 2;


    /*
     *  horizontal movement --> Target deceleration: 0.2g
     *  vertical movement --> Target deceleration: 0.7g
     */
    lcuCfg->tLcu.lTargetDeceleration_Horizontal = 200; /* unit is mm/s/s */
    lcuCfg->tLcu.lTargetDeceleration_Down = 700; /* unit is mm/s/s */

    /* brake actuation times (in milliseconds) */
    lcuCfg->tLcu.lNormalCasActuationTime = 20;
    lcuCfg->tLcu.lEmergencyCasActuationTime = 200;
    lcuCfg->tLcu.lControlledPrimaryBrakeActuationTime = 200;
    lcuCfg->tLcu.lForcedPrimaryBrakeActuationTime = 200;
    lcuCfg->tLcu.lForcedRedundantBrakeActuationTime = 200;
    lcuCfg->tLcu.lSfoActuationTime = 200;

    /* copy XC positions and matching tolerance */
    memcpy(lcuCfg->tXc.tXcInfo, mstCfg->tXc.tXcInfo, sizeof(lcuCfg->tXc.tXcInfo));
    lcuCfg->tXc.lXcMatchToleranceY = mstCfg->tXc.lXcMatchToleranceY;
    lcuCfg->tXc.lXcMatchToleranceZ = mstCfg->tXc.lXcMatchToleranceZ;

    /* config values for Speed limits (see SF_LCU_036)
     * Just set a dummy value of 50 to be above speed values from Lcu*/
    lcuCfg->tLcu.lSpeedLoadedHorizMax = 50;
    lcuCfg->tLcu.lSpeedUnloadedHorizMax = 50;
    lcuCfg->tLcu.lSpeedVertMax = 50;
    lcuCfg->tLcu.lSpeedInManualOpHorizMax = 50;
    lcuCfg->tLcu.lSpeedInManualOpVertMax = 50;
    lcuCfg->tLcu.lSlidingBrakeSpeedMax = 50;

    /* APS Config */
    /* minimum config to keep simple system running: */
    for (uint16_t usIndex; usIndex < APS_NUMBER_OF_OFFSETS_MAX; usIndex++) {
        lcuCfg->tLcu.tApsTrackOffsets[usIndex].ucTrackNumber = (uint8_t) ((usIndex%15));   // Let the tracks start with 1
        lcuCfg->tLcu.tApsTrackOffsets[usIndex].ulAlternativeOffsetForVerticalXcTrack = 0xFFFFU;
        lcuCfg->tLcu.tApsTrackOffsets[usIndex].ulNumberOfTrackEntriesLeft = 1;
        lcuCfg->tLcu.tApsTrackOffsets[usIndex].ulNumberOfTrackEntriesRight = 1;
        lcuCfg->tLcu.tApsTrackOffsets[usIndex].ulStartIndexLeft = 0;
        lcuCfg->tLcu.tApsTrackOffsets[usIndex].ulStartIndexRight = 0;
    }

    for (uint16_t usIndex; usIndex < LCU_APS_NUMBER_OF_XC_OFFSETS_MAX; usIndex++) {
        lcuCfg->tLcu.tApsXcTrackOffsets[usIndex].ucTrackNumber = (uint8_t) ((usIndex%15));   // Let the tracks start with 1
        lcuCfg->tLcu.tApsXcTrackOffsets[usIndex].ulNumberOfTrackEntriesLeft = 1;
        lcuCfg->tLcu.tApsXcTrackOffsets[usIndex].ulNumberOfTrackEntriesRight = 1;
        lcuCfg->tLcu.tApsXcTrackOffsets[usIndex].ulStartIndexLeft = 0;
        lcuCfg->tLcu.tApsXcTrackOffsets[usIndex].ulStartIndexRight = 0;
    }

    /* Only dummy-values to enable local operation */
    lcuCfg->tSr.lLCUAPS_MaxTravelDistanceWithCalculatedPosition = 10;   /* max distance in mm */
    lcuCfg->tSr.lLCUAPS_MaxTravelTimeWithCalculatedPosition = 5;        /* max time in s */
    lcuCfg->tSr.lLCUAPS_MaxTravelTimeWithCalculatedVelocity = 10;       /* max time in s */

    lcuCfg->ulCrc = CRC32((const void *) &((uint8_t *)lcuCfg)[sizeof(uint32_t)],
		sizeof(lcuCfg_t) - sizeof(uint32_t));

    free(mstCfg);
}

void
buildXcConfig(xcCfg_t* xcCfg, tXcId_t tXcId) {
    /* build a master config to clone from
     * Note: currently Master and XC share no config items so we do not
     * actually use mstCfg. This may change as config items are added
     * so we leave this code here for now.
     */
    masterCfg_t* mstCfg = malloc(sizeof(masterCfg_t));
    buildMasterConfig(mstCfg);

    memset(xcCfg, 0, sizeof(xcCfg_t));
    /* populate xcCfg using values from mstCfg */
    xcCfg->tValidity = CfgValidity_Valid;
    xcCfg->tCfgVersion = MULTI_CONFIG_VERSION;

    xcCfg->tXcId = tXcId;

    /* fill SR related constants
     * See also Main Architecture Design Specification, section Cfg SR
     *
     */
    xcCfg->tSr.lTCBSS_MaxUnblockActuationTime = 5000; /* unit: milliseconds */
    xcCfg->tSr.lTCBSS_MaxBlockActuationTime = 5000; /* unit: milliseconds */
    xcCfg->tSr.lXCSARE_MaxFailureTime = 5000; /* unit: milliseconds */
    xcCfg->tSr.lXCGBSS_MaxAllowedActivationTime = 1000; /* unit: milliseconds */
    xcCfg->tSr.lXCGBSS_MaxAllowedReleaseTime = 1000; /* unit: milliseconds */
    xcCfg->tSr.lXCGBSS_MaxMaintenanceTime = 10; /* unit: seconds */

    xcCfg->tSr.lXCISTO_ActuationTimeDuration = 50; /* unit: milliseconds */
    xcCfg->tSr.lXCISTO_DeActuationTimeDuration = 50; /* unit: milliseconds */

    xcCfg->tXc.lXcSPRotationThreshold = 1;
    xcCfg->tXc.sLockingAngleTolerance = 1;
    xcCfg->tXc.lXcSAREVelocityIntegrationTime = 1;

    xcCfg->tXc.tXC_SD_ParamSet = tStoppingDistanceParamSetXc;

    /*xcCfg->tXc.tTsprInfos[0].usStartAngle = 4096;
    xcCfg->tXc.tTsprInfos[0].usEndAngle = 8191;
    xcCfg->tXc.tTsprInfos[0].lAngularVelocityMaxAtEnd = 100;
    xcCfg->tXc.tTsprInfos[0].lAngularVelocityMaxAtStart = 100;

    xcCfg->tXc.tTsprInfos[1].usStartAngle = 4096;
    xcCfg->tXc.tTsprInfos[1].usEndAngle = 8191;
    xcCfg->tXc.tTsprInfos[1].lAngularVelocityMaxAtEnd = 100;
    xcCfg->tXc.tTsprInfos[1].lAngularVelocityMaxAtStart = 100;

    xcCfg->tXc.tTsprInfos[2].usStartAngle = 4096;
    xcCfg->tXc.tTsprInfos[2].usEndAngle = 8191;
    xcCfg->tXc.tTsprInfos[2].lAngularVelocityMaxAtEnd = 100;
    xcCfg->tXc.tTsprInfos[2].lAngularVelocityMaxAtStart = 100;

    xcCfg->tXc.tTsprInfos[3].usStartAngle = 4096;
    xcCfg->tXc.tTsprInfos[3].usEndAngle = 8191;
    xcCfg->tXc.tTsprInfos[3].lAngularVelocityMaxAtEnd = 100;
    xcCfg->tXc.tTsprInfos[3].lAngularVelocityMaxAtStart = 100;*/

    xcCfg->ulCrc = CRC32((const void *) &((uint8_t *)xcCfg)[sizeof(uint32_t)],
		sizeof(xcCfg_t) - sizeof(uint32_t));

    free(mstCfg);
}

void
buildIocConfig(iocCfg_t* iocCfg, tIocId_t tId) {
    /* build a master config to clone from */
    masterCfg_t* mstCfg = malloc(sizeof(masterCfg_t));
    buildMasterConfig(mstCfg);

    memset(iocCfg, 0, sizeof(iocCfg_t));
    /* populate iocCfg using values from mstCfg */
    iocCfg->tValidity = CfgValidity_Valid;
    iocCfg->tCfgVersion = MULTI_CONFIG_VERSION;

    iocCfg->tIocId = tId;

    /* copy complete device mapping table */
    memcpy(&iocCfg->tIoc.tDeviceMappingTable, &mstCfg->tIoc.tDeviceMappings[tId],
        sizeof(iocCfg->tIoc.tDeviceMappingTable));

    /* TODO add configuration values */

    iocCfg->ulCrc = CRC32((const void *) &((uint8_t *)iocCfg)[sizeof(uint32_t)],
		sizeof(iocCfg_t) - sizeof(uint32_t));

    free(mstCfg);
}

