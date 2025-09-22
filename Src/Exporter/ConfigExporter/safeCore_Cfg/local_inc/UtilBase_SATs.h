/*
 *  @copyright (c) 2021: infoteam SET GmbH, all rights reserved
 *
 *  @file      UtilBase_SATs.h
 *  @brief     Common data definitions and data types for SAT calculation
 *  @author    Marius Groeger
 *  @reference FubUtils Module Design Specification
 */

#ifndef UTILBASE_SATS_H_
#define UTILBASE_SATS_H_

/*
 * MACRO DEFINITIONS
 */

/* Scaling factors for k1, k2 and koffset.
 *
 * Scalings are chosen to speed up (1) calculation of the quadratic term and (2) the downscaling
 * operation for each term. (1) is accomplished by using the same scaling for K1 and K2, so the
 * Horner scheme [ k1 * v * v + k2 * v = ((k1 * v) + k2) * v ] can be applied. (2) is accomplished
 * by using powers of two, so the downscaling is a logical shift operation.
 */
#define SCALE_K1_K2          (0x80000000U)
#define SCALE_KOFFSET        (0x10000U)

/*
 *  Upscaling macros for k1, k2 and koffset to generate configuration values.
 *
 *  Upscaling includes (1) velocity conversion from m/s to mm/s and (2) result conversion from
 *  m to mm. Note that the velocity conversion factor needs to match the polynomial order.
 */
#define UPSCALE_K1(v)      ((int32_t)((v) * SCALE_K1_K2 / 1000000 * 1000))
#define UPSCALE_K2(v)      ((int32_t)((v) * SCALE_K1_K2 / 1000 * 1000))
#define UPSCALE_KOFFSET(v) ((int32_t)((v) * SCALE_KOFFSET * 1000))

#endif /* UTILBASE_SATS_H_ */
