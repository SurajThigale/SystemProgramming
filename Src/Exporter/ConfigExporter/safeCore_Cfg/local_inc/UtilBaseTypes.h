/** \addtogroup UtilBase
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      UtilBaseTypes.h
 *  @brief     Common data types
 *  @author    Marius Groeger
 *  @reference Generic Utilities Fub Functional Block Specification
 *
 */

#ifndef UTILBASETYPES_H_
#define UTILBASETYPES_H_

/*
 * INCLUDEFILES
 */
#include <stdint.h>
#include <stdbool.h>

#include "CfgLimits.h"

/*
 *  MACRO DEFINITIONS
 */

/**
 * @brief           Value denoting the first LCU
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tSubSystemId_t type variables, this constant denotes the first LCU.
 * @remark          It is important that this value is 0.
 *                  UtilSfMgr relies on LCU, XC, MULTI being contiguous and starting at zero
 */
#define SYSID_LCU_FIRST 0U

/**
 * @brief           Value denoting the last LCU
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tSubSystemId_t type variables, this constant denotes the last LCU.
 * @remarks
 * The LINT instruction suppresses the warning about the left operand always evaluating to 0.
 */
#define SYSID_LCU_LAST /*lint --e(835) */ (SYSID_LCU_FIRST + MULTI_NUMBER_OF_LCU_MAX - 1U)

/**
 * @brief           Value denoting the first Exchanger
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tSubSystemId_t type variables, this constant denotes the first XC.
 * @remark          It is important, that this value starts directly after SYSID_LCU_LAST.
 *                  UtilSfMgr relies on LCU, XC, MULTI being contiguous and starting at zero
 */
#define SYSID_XC_FIRST (SYSID_LCU_LAST + 1U)

/**
 * @brief           Value denoting the last Exchanger
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tSubSystemId_t type variables, this constant denotes the last XC.
 */
#define SYSID_XC_LAST (SYSID_XC_FIRST + MULTI_NUMBER_OF_XC_MAX - 1U)

/**
 * @brief           Value denoting a virtual sub-system "MULTI"
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @remark          It is important, that this value starts directly after SYSID_XC_LAST.
 *                  UtilSfMgr relies on LCU, XC, MULTI being contiguous and starting at zero
 */
#define SYSID_MULTI (SYSID_XC_LAST + 1U)

/**
 * @brief           Value denoting the first IO Collector
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tSubSystemId_t type variables, this constant denotes the first IOC.
 */
#define SYSID_IOC_FIRST (SYSID_MULTI + 1U)

/**
 * @brief           Value denoting the last IO Collector
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tSubSystemId_t type variables, this constant denotes the last IOC.
 */
#define SYSID_IOC_LAST (SYSID_IOC_FIRST + MULTI_NUMBER_OF_IOC_MAX - 1U)

/**
 * @brief           Value denoting the first MasterArray SN
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tSubSystemId_t type variables, this constant denotes the first MASN.
 */
#define SYSID_MST_FIRST (SYSID_IOC_LAST + 1U)

/**
 * @brief           Value denoting the last MasterArray SN
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tSubSystemId_t type variables, this constant denotes the last MASN.
 */
#define SYSID_MST_LAST (SYSID_MST_FIRST + MULTI_NUMBER_OF_MA_MAX - 1U)

/**
 * @brief           Value denoting the first Exchanger
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tXcId_t type variables, this constant denotes the first XC.
 */
#define FIRST_XC_INDEX  0U

/**
 * @brief           Value denoting the first LCU
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tLcuId_t type variables, this constant denotes the first LCU.
 */
#define FIRST_LCU_INDEX 0U

/**
 * @brief           Value denoting the first IOC
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tIocId_t type variables, this constant denotes the first IOC.
 */
#define FIRST_IOC_INDEX 0U

/**
 * @brief           Value denoting the first Master
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details         When using tMstId_t type variables, this constant denotes the first Master.
 */
#define FIRST_MST_INDEX 0U

/**
 * @brief           Convert an LCU id to a system-wide subsystem id
 * @param[in]       id - LCU id
 * @return          system wide sub system id corresponding to this LCU
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @remarks
 * The LINT instruction suppresses the warning about the right operand always evaluating to 0.
 */
#define ID_LCU_TO_SUBSYSTEM(id) /*lint --e(845) */ ((tSubSystemId_t)((uint32_t)(id) + (uint32_t)(SYSID_LCU_FIRST)))

/**
 * @brief           Convert an XC id to a system-wide subsystem id
 * @param[in]       id - XC id
 * @return          system wide sub system id corresponding to this XC
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
#define ID_XC_TO_SUBSYSTEM(id)  ((tSubSystemId_t)((uint32_t)(id) + (uint32_t)(SYSID_XC_FIRST)))

/**
 * @brief           Convert an IOC id to a system-wide subsystem id
 * @param[in]       id - IOC id
 * @return          system wide sub system id corresponding to this IOC
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
#define ID_IOC_TO_SUBSYSTEM(id) ((tSubSystemId_t)((uint32_t)(id) + (uint32_t)(SYSID_IOC_FIRST)))

/**
 * @brief           Convert an MST id to a system-wide subsystem id
 * @param[in]       id - MST id
 * @return          system wide sub system id corresponding to this MST
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
#define ID_MST_TO_SUBSYSTEM(id) ((tSubSystemId_t)((uint32_t)(id) + (uint32_t)(SYSID_MST_FIRST)))

/**
 * @brief           Convert a system-wide subsystem id to an LCU id
 * @param[in]       id - system wide system ID
 * @return          LCU id corresponding to this LCU subsystem id
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @remarks
 * The LINT instruction suppresses the warning about the right operand always evaluating to 0.
 */
#define ID_SUBSYSTEM_TO_LCU(id) /*lint --e(845) */ ((tLcuId_t)((uint8_t)(id) - (uint8_t)(SYSID_LCU_FIRST)))

/**
 * @brief           Convert a system-wide subsystem id to an XC id
 * @param[in]       id - system wide system ID
 * @return          XC id corresponding to this XC subsystem id
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
#define ID_SUBSYSTEM_TO_XC(id)  ((tXcId_t)((uint8_t)(id) - (uint8_t)(SYSID_XC_FIRST)))

/**
 * @brief           Convert a system-wide subsystem id to an IOC id
 * @param[in]       id - system wide system ID
 * @return          IOC id corresponding to this IOC subsystem id
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
/** Convert a system-wide subsystem id to an IOC id */
#define ID_SUBSYSTEM_TO_IOC(id) ((tIocId_t)((uint8_t)(id) - (uint8_t)(SYSID_IOC_FIRST)))

/**
 * @brief           Convert a system-wide subsystem id to an MASN id
 * @param[in]       id - system wide system ID
 * @return          MST id corresponding to this MASN subsystem id
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
/** Convert a system-wide subsystem id to an MST id */
#define ID_SUBSYSTEM_TO_MST(id) ((tIocId_t)((uint8_t)(id) - (uint8_t)(SYSID_MST_FIRST)))

/**
 * @brief        Allowed value of tSubSystemId_t on SNs other than MASN
 * @requirements SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details
 *
 * tSubSystemId_t is only valid to be used on MASN. Other safety nodes MUST use this value when
 * interacting with a variable of type tSubSystemId_t.
 */
#define UNUSED_SUBSYSTEM_ID 0

#if defined(SN_MASTER)
/**
 * @brief         Determine if (system-wide) sub-system id points to a XC
 * @requirements SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 *
 */
#define ID_SUBSYSTEM_IS_XC(id) (((id) >= SYSID_XC_FIRST) && ((id) <= SYSID_XC_LAST))

/**
 * @brief         Determine if (system-wide) sub-system id points to a LCU
 * @requirements SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 *
 */
#define ID_SUBSYSTEM_IS_LCU(id) ((id) <= SYSID_LCU_LAST)

/**
* @brief         Determine if (system-wide) sub-system id points to an IOC
* @requirements SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
*
*/
 #define ID_SUBSYSTEM_IS_IOC(id) (((id) >= SYSID_IOC_FIRST) && ((id) <= SYSID_IOC_LAST))

 /**
  * @brief         Determine if (system-wide) sub-system id points to a Master
  * @requirements  SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
  *
  */
#define ID_SUBSYSTEM_IS_MST(id) (((id) >= SYSID_MST_FIRST) && ((id) <= SYSID_MST_LAST))

#endif

/**
 * @brief           Array size of subsystemSet for LCUs
 * @requirements    SC_REQ-221654
 */
#define ARRAY_SIZE_SUBSYSTEMSET_LCU ((MULTI_NUMBER_OF_LCU_MAX + 7U) / 8U)

/**
 * @brief           Array size of subsystemSet for XCs
 * @requirements    SC_REQ-221654
 */
#define ARRAY_SIZE_SUBSYSTEMSET_XC  ((MULTI_NUMBER_OF_XC_MAX + 7U) / 8U)

/*
 * TYPE DEFINITIONS
 */

/**
 * @brief           Safety function type
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @remarks         SfType_FOC_ROT is currently not yet implemented
 */
typedef enum {
    /** SF leading to LCU translatory safe state*/
    SfType_LCU_TRANS,
    /** SF leading to LCU rotatory safe state */
    SfType_LCU_ROT,
    /** SF leading to XC safe state */
    SfType_XC,
    /** SF leading to shaft equipment safe state */
    SfType_SE,
    /** SF leading to MULTI safe state */
    SfType_MULTI,
} enSfType_t;

/**
 * @brief           The index of a safety function
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
typedef uint8_t tSfIndex_t;

/**
 * @brief           Identifies a safety function by type and index
 * @requirements    SC_REQ-217385 FBRQ - Define datastructures in UtilBase.h
 */
typedef struct {
    tSfIndex_t tSfIndex;
    enSfType_t tSfType;
} safetyFunctionId_t;

typedef uint8_t tSrIndex_t;

/**
 * @brief           LCU type
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Type denoting an economy type LCU */
    LcuType_Econonmy = 0,
    /** Type denoting an premium type LCU */
    LcuType_Premium,
} enLcuType_t;

/**
 * @brief           Safe switch status
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
typedef enum {
    /** Value denoting a safe switch's state is active */
    SafeSwitch_Active,
    /** Value denoting a safe switch's state is inactive */
    SafeSwitch_Inactive,
    /** Value denoting a safe switch's state is undefined */
    SafeSwitch_Undefined,
} enSafeSwitchStatus_t;

/**
 * @brief           Blocking status of a locking device
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
typedef enum {
    /** Value denoting a locking device's blocking state is not blocked */
    LockBlock_NotBlocked,
    /** Value denoting a locking device's blocking state is blocked */
    LockBlock_Blocked,
    /** Value denoting a locking device's blocking state is undefined  */
    LockBlock_BlockUndefined,
} enLockBlockStatus_t;

/**
 * @brief           Locking status of a locking device
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting a locking device is locked  */
    Lock_FullyLocked = 3,
    /** Value denoting a locking device is transitioning between locked and unlocked  */
    Lock_Transitioning = 2,
    /** Value denoting a locking device is unlocked  */
    Lock_FullyUnlocked = 1,
    /** Value denoting an undefined locking device state */
    Lock_Undefined = 0,
} enLockStatus_t;

/**
 * @brief           System-wide safety-node type
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details
 *
 * Using this type safety-node (LCU, XC, IOC, Master SN) can be referenced.
 */
typedef uint32_t tSubSystemId_t;

/**
 * @brief           Exchanger safety-node ID
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details
 *
 * Using this type an XC SN can be referenced. Iterations may use 0..MULTI_NUMBER_OF_XC_MAX
 */
typedef uint8_t tXcId_t;

/**
 * @brief           LCU Safety-Node ID
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details
 *
 * Using this type an LCU SN can be referenced. Iterations may use 0..MULTI_NUMBER_OF_LCU_MAX
 */
typedef uint8_t tLcuId_t;

/**
 * @brief           IO Collector safety-node ID
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details
 *
 * Using this type an IOC SN can be referenced.
 */
typedef uint8_t tIocId_t;

/**
 * @brief           Master safety-node ID
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 * @details
 *
 * Using this type a Master SN can be referenced.
 */
typedef uint8_t tMstId_t;

/**
 * @brief           A point as signed y, z cooridinates in a 2D plane.
 * @requirements    SC_REQ-217385 FBRQ - Define datastructures in UtilBase.h
 */
typedef struct {
    /** horizontal component of coordinate */
    int32_t lY;
    /** vertical component of coordinate */
    int32_t lZ;
} point_t;

/**
 * @brief           A rectangular area defined by two points
 * @requirements    SC_REQ-217385 FBRQ - Define datastructures in UtilBase.h
 */
typedef struct {
    /** Upper-left point of area */
    point_t tTopLeft;
    /** Bottom-right point of area */
    point_t tBottomRight;
} area_t;

/**
 * @brief           Protection area type
 * @requirements    SC_REQ-217385 FBRQ - Define datastructures in UtilBase.h
 */
typedef enum {
    /** Value denoting a protection area defined by a wall panel */
    ProtectionArea_WallPanel,
    /** Value denoting a protection area defined by a well door */
    ProtectionArea_WellDoor,
    /** Value denoting a protection area defined by a well access port */
    ProtectionArea_WellAccessPort,
    /** Value denoting a protection area defined by detection water in a well */
    ProtectionArea_WellWater,
    /** Value denoting a protection area defined by a walkway area */
    ProtectionArea_Walkway,
    /** Value denoting a protection area defined by a horizontal or vertical well */
    ProtectionArea_TranslationalMovement,
    /** Value denoting a protection area defined by a garage */
    ProtectionArea_Garage,
    /** Value denoting a protection area defined due to a failing IOC */
    ProtectionArea_IocAffectedArea,
    /** Value denoting a protection area defined by a maintenance zone */
    ProtectionArea_Maintenance,
    /** Value denoting a protection area defined by a working area */
    ProtectionArea_WorkingArea,
    /** Value denoting a protection area defined by a fire compartment */
    ProtectionArea_FireCompartment,
    /** Value denoting a protection area defined by a propulsion section */
    ProtectionArea_PropulsionSection,
    /** Value denoting a protection area defined by a swivel platform */
    ProtectionArea_SwivelPlatform,
    /** Value denoting a protection area defined by a well lock */
    ProtectionArea_WellLock,
    /** Value denoting a protection area defined by a well segment triggered by open well access
     * ports */
    ProtectionArea_WellAccessPortSegment,
    /** Value denoting a protection area defined by a well door segment */
    ProtectionArea_WellDoorSegment,
    /** Value denoting a protection area defined by a well segment triggered by emergency stop */
    ProtectionArea_EmergencyWellSegment,
    /** Value denoting a well segment protection area, used in the configuration */
    ProtectionArea_WellSegment,
    /** Value denoting a protection area around LCU */
    ProtectionArea_LCU,
    /** Value to indicate number of protection area type */
    ProtectionArea_LastEntry,
    /**
     * Value to enforce container storage size to 32 bit. This way they can be used in
     * configuration data structures that are shared with other compiler environments.
     */
    ProtectionArea_Datatype_Storage_Size = INT32_MAX,
} enProtectionAreaType_t;

/**
 * @brief           Protection area
 * @requirements    SC_REQ-217384 FBRQ - Define datastructures in UtilBase.h
 */
typedef struct {
    /** Rectengular dimensions of protection area */
    area_t tArea;
    /** Type of protection area */
    enProtectionAreaType_t tType;
} protectionArea_t;

/**
 * @brief           Stopping areas for LCU translational movement
 * @requirements    SC_REQ-217385 FBRQ - Define datastructures in UtilBase.h
 */
typedef struct {
    /** SAT needed for controlled autonomous stopping */
    area_t tAutonomousSat;
    /** SAT needed for controlled emergency stopping */
    area_t tControlledSat;
    /** SAT needed for forced emergency stopping */
    area_t tForcedSat;
} lcuSats_t;

/**
 * @brief           LCU movement stopping scenarios
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting no stopping scenerario */
    StoppingScenario_NONE = 0,
    /** Value denoting controlled autonomous stopping */
    StoppingScenario_CAS = 1,
    /** Value denoting controlled emergency stopping */
    StoppingScenario_CES = 2,
    /** Value denoting forced emergency stopping */
    StoppingScenario_FES = 3,
} enStoppingScenario_t;

/**
 * @brief           Safe state type
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting LCU transalatory safe state */
    SafeStateType_Translatory,
    /** Value denoting LCU rotatory safe state */
    SafeStateType_Rotatory,
    /** Value denoting shaft equipment safe state */
    SafeStateType_ShaftEquipment,
    /** Value denoting XC swivel platform safe state */
    SafeStateType_Exchanger,
    /** Value denoting MULTI safe state */
    SafeStateType_MULTI,
} enSafeStateType_t;

/**
 * @brief           Structure holding sets of sub-systems
 * @requirements    SC_REQ-221654 FBRQ - type subSystemsSet_t
 */
typedef struct {
    uint8_t ucLcus[ARRAY_SIZE_SUBSYSTEMSET_LCU];
    uint8_t ucXcs[ARRAY_SIZE_SUBSYSTEMSET_XC];
} subSystemsSet_t;

/**
 * @brief           State info for MULTI SF monitor result
 * @requirements    SC_REQ-221655 FBRQ - type multiMonitorStateInfo_t
 */
typedef struct {
    /** flag indicating whether MULTI safe state is requested */
    bool bMultiSafeStateRequested;
    /** sub-systems which shall be part of MULTI safe state */
    subSystemsSet_t tAffectedSubSystems;
} multiMonitorStateInfo_t;

/**
 * @brief           Safety function monitor result
 * @requirements    SC_REQ-221656 FBRQ - type runMonitorResult_t
 */
typedef struct {
    /** when true, the SF monitor has detected a hazard */
    bool bHazardDetected;

    /**
     * Extra data for safe state.
     *
     * Which data is applicable depends on safetyFunctionId_t.enSfType_t
     * of the safety function.
     */
    union {
        enStoppingScenario_t tStoppingScenario;
        bool bRotatorySafeStateRequested;
        bool bSwivelPlatformSafeStateRequested;
        bool bShaftEquipmentSafeStateRequested;
        multiMonitorStateInfo_t tMultiSafeStateInfo;
    } uSafeStateInfo;

    /** Movement prevention requested by safety function monitor */
    bool bPreventMovement;
} runMonitorResult_t;

/**
 * @brief           Safe state stage
 * @requirements    SC_REQ-217386 - FBRQ - Define external datatypes, visible to safeCORE in UtilBase.
 */
typedef enum {
    /** Value denoting no safe state */
    SafeState_NotIn = 0,
    /** Value denoting transitioning into safe state */
    SafeState_TransitioningInto = 1,
    /** Value denoting transitioning out of safe state */
    SafeState_TransitioningOutOf = 2,
    /** Value denoting being in safe state */
    SafeState_In = 3,
} enSafeState_t;

/**
 * @brief           XC rotation state
 * @requirements    SC_REQ-217386 - FBRQ - Define external datatypes, visible to safeCORE in UtilBase.
 */
typedef enum {
    /** Value denoting XC is rotating */
    enXcRotating = 0,
    /** Value denoting XC is not rotating and in horizontal position */
    enXcNotRotatingHorizontal = 1,
    /** Value denoting XC is not rotating and in vertical position */
    enXcNotRotatingVertical = 2,
    /** Value denoting XC is not rotating and neither in horizontal or vertical position */
    enXcNotRotatingOther = 3,
} enXCRotatingStatus_t;

/**
 * @brief           A generic request
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting not request is not made */
    Request_NotRequested = 0,
    /** Value denoting not request is made */
    Request_Requested = 1,
} enRequest_t;

/**
 * @brief           Status for a (new) transmitted configuration
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting that configuration is not yet received */
    ConfigReceivedNotYet = 0,
    /** Value denoting that configuration is received with an error */
    ConfigReceivedErroneously = 1,
    /** Value denoting that configuration is correctly received */
    ConfigReceivedCorrectly = 2,
} enConfigReceivedStatus_t;

/**
 * @brief           Request to prepare bypass of safety functions
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting bypass preparation is not requested */
    BypassPrepare_NotRequested = 0,
    /** Value denoting bypass preparation is requested */
    BypassPrepare_Requested = 1,
} enBypassPrepare_t;

/**
 * @brief           Request to bypass or un-bypass a safety function
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting bypass is not requested */
    BypassActivate_NotRequested = 0,
    /** Value denoting bypass is requested */
    BypassActivate_Requested = 1,
} enBypassActivate_t;

/**
 * @brief           Request to bypass or un-bypass a safety function
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting the execution of startup tests is not requested */
    ExecStartupTests_NotRequested = 0,
    /** Value denoting the execution of startup tests is requested */
    ExecStartupTests_Requested = 1,
} enExecStartupTests_t;

/**
 * @brief           Request to bypass or un-bypass a safety function
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting the execution of safety diag tests is not requested */
    ExecSafetyDiagTests_NotRequested = 0,
    /** Value denoting the execution of safety diag tests is requested */
    ExecSafetyDiagTests_Requested = 1,
} enExecSafetyDiagTests_t;

/**
 * @brief           Safe state release confirmation
 * @requirements    SC_REQ-217386 - FBRQ - Define external datatypes, visible to safeCORE in UtilBase.
 */
typedef enum {
    /** Value denoting safe state is not released */
    SafeState_NotReleased = 0,
    /** Value denoting safe state is released */
    SafeState_Released = 1,
} enSafeStateReleaseConfirmation_t;

/**
 * @brief           Command to permit movement (translatory or rotatory)
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting that PermitMovement is not requested */
    PermitMovement_NotRequested = 0,
    /** Value denoting that PermitMovement is requested */
    PermitMovement_Requested = 1,
} enPermitMovement_t;

/**
 * @brief           Command to release brake
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting that release of brake is not requested */
    PermitReleaseOfBrake_NotRequested = 0,
    /** Value denoting that release of brake is requested */
    PermitReleaseOfBrake_Requested = 1
} enPermitReleaseOfBrake_t;

/**
 * @brief           Command to activate new confirmation
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting that activation of new confirmation is not requested*/
    ActivateNewConfirmation_NotRequested = 0,
    /** Value denoting that activation of new confirmation is requested*/
    ActivateNewConfirmation = 1,
} enActivateNewConfirmation_t;

/**
 * @brief           Command to mark new configuration as valid
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting that new configuration is not requested to be marked as valid */
    MarkNewConfiguration_NotRequested = 0,
    /** Value denoting that new configuration is requested to be marked as valid */
    MarkNewConfigurationValid = 1,
} enMarkNewConfigurationValid_t;

/**
 * @brief           Command to lock swivel platform or not
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting that swivel platform is not locked by other means */
    SwivelPlatformNotLockedByOtherMeans = 0,
    /** Value denoting that swivel platform is locked by other means */
    SwivelPlatformLockedByOtherMeans = 1,
} enConfirmSwivelPlatformLockedByOtherMeans_t;


/**
 * @brief           LCU travel direction
 * @requirements    SC_REQ-217386 FBRQ - Define external datatypes, visible to safeCORE in UtilBase.h
 */
typedef enum {
    /** Value denoting an invalid travel direction */
    TravelDirection_Invalid = 0,
    /** Value denoting horizontal travel direction */
    TravelDirection_Horizontal = 1,
    /** Value denoting vertical travel direction */
    TravelDirection_Vertical = 2,
    /** Value denoting travel direction is currently changing */
    TravelDirection_Transitioning = 3,
} enTravelDirection_t;

/**
 * @brief           LCU payload
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
typedef uint32_t LCUPayload_t;

/**
 * @brief           The status of a safety component temperature
 * @requirements    SC_REQ-217384 - FBRQ - Define internal datatypes in UtilBase.h
 */
typedef enum {
    enSctUndefinded,
    enSctTrue,
} enSCTStatus_t;

/**
 * @brief           Time difference in milliseconds
 * @requirements    SC_REQ-216753 FBRQ - Functionality to support timers
 */
typedef uint32_t timeDiffMs_t;

/**
 * @brief           Absolute time in milliseconds
 * @requirements    SC_REQ-216753 FBRQ - Functionality to support timers
 */
typedef uint64_t timeMs_t;

/**
 * @brief           Timer object
 * @requirements    SC_REQ-216753 FBRQ - Functionality to support timers
 */
typedef struct {
    /** Absolute start time for this timer */
    timeMs_t startTime;
} timerObject_t;

#endif /* UTILBASETYPES_H_ */

/** @}  End of this module */
