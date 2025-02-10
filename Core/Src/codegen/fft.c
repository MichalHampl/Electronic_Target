/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fft.c
 *
 * MATLAB Coder version            : 23.2
 * C/C++ source code generated on  : 15-Nov-2024 13:26:43
 */

/* Include Files */
#include "fft.h"
#include "FFTImplementationCallback.h"
#include "envelope_xcorr_gen_emxutil.h"
#include "envelope_xcorr_gen_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *x
 *                double varargin_1
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void fft(const emxArray_real32_T *x, double varargin_1, emxArray_creal32_T *y)
{
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  creal32_T *y_data;
  int nRows;
  emxInit_real32_T(&costab, 2);
  emxInit_real32_T(&sintab, 2);
  emxInit_real32_T(&sintabinv, 2);
  if ((x->size[0] == 0) || ((int)varargin_1 == 0)) {
    int N2blue;
    N2blue = (int)varargin_1;
    nRows = y->size[0];
    y->size[0] = (int)varargin_1;
    emxEnsureCapacity_creal32_T(y, nRows);
    y_data = y->data;
    for (nRows = 0; nRows < N2blue; nRows++) {
      y_data[nRows].re = 0.0F;
      y_data[nRows].im = 0.0F;
    }
  } else {
    int N2blue;
    bool useRadix2;
    useRadix2 = (((int)varargin_1 > 0) &&
                 (((int)varargin_1 & ((int)varargin_1 - 1)) == 0));
    N2blue =
        c_FFTImplementationCallback_get((int)varargin_1, useRadix2, &nRows);
    c_FFTImplementationCallback_gen(nRows, useRadix2, costab, sintab,
                                    sintabinv);
    if (useRadix2) {
      c_FFTImplementationCallback_r2b(x, (int)varargin_1, costab, sintab, y);
    } else {
      c_FFTImplementationCallback_dob(x, N2blue, (int)varargin_1, costab,
                                      sintab, sintabinv, y);
    }
  }
  emxFree_real32_T(&sintabinv);
  emxFree_real32_T(&sintab);
  emxFree_real32_T(&costab);
}

/*
 * File trailer for fft.c
 *
 * [EOF]
 */
