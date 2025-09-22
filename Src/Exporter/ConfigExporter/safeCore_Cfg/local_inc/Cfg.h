/** \addtogroup GenericCfgGeneral
 *  @{
 *
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      Cfg.h
 *  @brief     FUB level interface of Cfg FUB.
 *  @author    Frank Meier
 */

#ifndef CFG_H_
#define CFG_H_

/*
 * INCLUDE FILES
 */
#include <stdbool.h>

/*
 * EXTERN FUNCTION DECLARATIONS
 */

extern bool
Cfg_earlyInitialize(void);

extern bool
Cfg_initialize(void);

#endif /* CFG_H_ */

/** @}  End of this module */
