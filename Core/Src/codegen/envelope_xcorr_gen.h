/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: envelope_xcorr_gen.h
 *
 * MATLAB Coder version            : 23.2
 * C/C++ source code generated on  : 15-Nov-2024 13:26:43
 */

#ifndef ENVELOPE_XCORR_GEN_H
#define ENVELOPE_XCORR_GEN_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern float envelope_xcorr_gen(float ms1[256], float ms2[256], float N,
                                float fs, float channels, float window,
                                float thrshld);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for envelope_xcorr_gen.h
 *
 * [EOF]
 */
