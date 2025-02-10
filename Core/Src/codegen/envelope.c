/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: envelope.c
 *
 * MATLAB Coder version            : 23.2
 * C/C++ source code generated on  : 15-Nov-2024 13:26:43
 */

/* Include Files */
#include "envelope.h"
#include "FFTImplementationCallback.h"
#include "envelope_xcorr_gen_emxutil.h"
#include "envelope_xcorr_gen_types.h"
#include "ifft.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static float rt_hypotf_snf(float u0, float u1);

/* Function Definitions */
/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
static float rt_hypotf_snf(float u0, float u1)
{
  float a;
  float b;
  float y;
  a = fabsf(u0);
  b = fabsf(u1);
  if (a < b) {
    a /= b;
    y = b * sqrtf(a * a + 1.0F);
  } else if (a > b) {
    b /= a;
    y = a * sqrtf(b * b + 1.0F);
  } else if (rtIsNaNF(b)) {
    y = rtNaNF;
  } else {
    y = a * 1.41421354F;
  }
  return y;
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                emxArray_real32_T *upperEnv
 * Return Type  : void
 */
void envelope(const emxArray_real32_T *x, emxArray_real32_T *upperEnv)
{
  emxArray_creal32_T *c_x;
  emxArray_creal32_T *d_x;
  emxArray_real32_T *b_x;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  creal32_T *c_x_data;
  const float *x_data;
  float *b_x_data;
  float *upperEnv_data;
  int firstBlockLength;
  int hi;
  int ib;
  int k;
  int nblocks;
  x_data = x->data;
  emxInit_real32_T(&b_x, 1);
  nblocks = b_x->size[0];
  b_x->size[0] = x->size[1];
  emxEnsureCapacity_real32_T(b_x, nblocks);
  b_x_data = b_x->data;
  firstBlockLength = x->size[1];
  for (nblocks = 0; nblocks < firstBlockLength; nblocks++) {
    b_x_data[nblocks] = x_data[nblocks];
  }
  if (b_x->size[0] > 0) {
    float xmean;
    int lastBlockLength;
    bool useRadix2;
    if (b_x->size[0] <= 1024) {
      firstBlockLength = b_x->size[0];
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      firstBlockLength = 1024;
      nblocks = (int)((unsigned int)b_x->size[0] >> 10);
      lastBlockLength = b_x->size[0] - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    xmean = b_x_data[0];
    for (k = 2; k <= firstBlockLength; k++) {
      xmean += b_x_data[k - 1];
    }
    for (ib = 2; ib <= nblocks; ib++) {
      float bsum;
      firstBlockLength = (ib - 1) << 10;
      bsum = b_x_data[firstBlockLength];
      if (ib == nblocks) {
        hi = lastBlockLength;
      } else {
        hi = 1024;
      }
      for (k = 2; k <= hi; k++) {
        bsum += b_x_data[(firstBlockLength + k) - 1];
      }
      xmean += bsum;
    }
    xmean /= (float)b_x->size[0];
    firstBlockLength = b_x->size[0];
    for (nblocks = 0; nblocks < firstBlockLength; nblocks++) {
      b_x_data[nblocks] -= xmean;
    }
    emxInit_creal32_T(&c_x);
    useRadix2 = ((b_x->size[0] & (b_x->size[0] - 1)) == 0);
    firstBlockLength =
        c_FFTImplementationCallback_get(b_x->size[0], useRadix2, &hi);
    emxInit_real32_T(&costab, 2);
    emxInit_real32_T(&sintab, 2);
    emxInit_real32_T(&sintabinv, 2);
    c_FFTImplementationCallback_gen(hi, useRadix2, costab, sintab, sintabinv);
    emxInit_creal32_T(&d_x);
    if (useRadix2) {
      c_FFTImplementationCallback_r2b(b_x, b_x->size[0], costab, sintab, d_x);
      c_x_data = d_x->data;
    } else {
      c_FFTImplementationCallback_dob(b_x, firstBlockLength, b_x->size[0],
                                      costab, sintab, sintabinv, d_x);
      c_x_data = d_x->data;
    }
    emxFree_real32_T(&sintabinv);
    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
    nblocks = b_x->size[0];
    firstBlockLength = b_x->size[0] >> 1;
    if ((b_x->size[0] & 1) == 0) {
      lastBlockLength = firstBlockLength;
    } else {
      lastBlockLength = firstBlockLength + 1;
    }
    hi = firstBlockLength + 2;
    for (firstBlockLength = 2; firstBlockLength <= lastBlockLength;
         firstBlockLength++) {
      c_x_data[firstBlockLength - 1].re *= 2.0F;
      c_x_data[firstBlockLength - 1].im *= 2.0F;
    }
    for (firstBlockLength = hi; firstBlockLength <= nblocks;
         firstBlockLength++) {
      c_x_data[firstBlockLength - 1].re = 0.0F;
      c_x_data[firstBlockLength - 1].im = 0.0F;
    }
    ifft(d_x, c_x);
    c_x_data = c_x->data;
    emxFree_creal32_T(&d_x);
    hi = c_x->size[0];
    nblocks = b_x->size[0];
    b_x->size[0] = c_x->size[0];
    emxEnsureCapacity_real32_T(b_x, nblocks);
    b_x_data = b_x->data;
    for (k = 0; k < hi; k++) {
      b_x_data[k] = rt_hypotf_snf(c_x_data[k].re, c_x_data[k].im);
    }
    emxFree_creal32_T(&c_x);
    firstBlockLength = b_x->size[0];
    for (nblocks = 0; nblocks < firstBlockLength; nblocks++) {
      b_x_data[nblocks] += xmean;
    }
  }
  nblocks = upperEnv->size[0] * upperEnv->size[1];
  upperEnv->size[0] = 1;
  upperEnv->size[1] = x->size[1];
  emxEnsureCapacity_real32_T(upperEnv, nblocks);
  upperEnv_data = upperEnv->data;
  firstBlockLength = x->size[1];
  for (nblocks = 0; nblocks < firstBlockLength; nblocks++) {
    upperEnv_data[nblocks] = b_x_data[nblocks];
  }
  emxFree_real32_T(&b_x);
}

/*
 * File trailer for envelope.c
 *
 * [EOF]
 */
