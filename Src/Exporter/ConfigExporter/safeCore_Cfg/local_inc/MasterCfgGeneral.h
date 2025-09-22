/** \addtogroup MasterCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      MasterCfgGeneral.h
 *  @brief     Public functions of Master CfgGeneral module.
 *  @author    Frank Meier
 *
 */

#ifndef MASTERCFGGENERAL_H_
#define MASTERCFGGENERAL_H_

/*
 * INCLUDEFILES
 */
#include "UtilOpMode.h"

/*
 * EXTERN FUNCTION DECLARATIONS
 */
extern bool
MasterCfgGeneral_handleConfigureOpMode(enOpMode_t tPreviousOpMode);

#endif /* MASTERCFGGENERAL_H_ */

/** @}  End of this module */
