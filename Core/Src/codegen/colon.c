/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: colon.c
 *
 * MATLAB Coder version            : 23.2
 * C/C++ source code generated on  : 15-Nov-2024 13:26:43
 */

/* Include Files */
#include "colon.h"
#include "envelope_xcorr_gen_emxutil.h"
#include "envelope_xcorr_gen_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : float d
 *                float b
 *                emxArray_real32_T *y
 * Return Type  : void
 */
void eml_float_colon(float d, float b, emxArray_real32_T *y)
{
  double apnd;
  double cdiff;
  double ndbl;
  float b1;
  float *y_data;
  int k;
  int n;
  int nm1d2;
  ndbl = floor((double)b / d + 0.5);
  apnd = ndbl * d;
  if (d > 0.0F) {
    cdiff = apnd - b;
  } else {
    cdiff = b - apnd;
  }
  if (fabs(cdiff) < 2.384185791015625E-7 * fmax(0.0, fabs(b))) {
    ndbl++;
    b1 = b;
  } else if (cdiff > 0.0) {
    b1 = (float)((ndbl - 1.0) * d);
  } else {
    ndbl++;
    b1 = (float)apnd;
  }
  if (ndbl >= 0.0) {
    n = (int)ndbl;
  } else {
    n = 0;
  }
  nm1d2 = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = n;
  emxEnsureCapacity_real32_T(y, nm1d2);
  y_data = y->data;
  if (n > 0) {
    y_data[0] = 0.0F;
    if (n > 1) {
      float kd;
      y_data[n - 1] = b1;
      nm1d2 = (n - 1) / 2;
      for (k = 0; k <= nm1d2 - 2; k++) {
        kd = (float)(k + 1) * d;
        y_data[k + 1] = kd;
        y_data[(n - k) - 2] = b1 - kd;
      }
      if (nm1d2 << 1 == n - 1) {
        y_data[nm1d2] = b1 / 2.0F;
      } else {
        kd = (float)nm1d2 * d;
        y_data[nm1d2] = kd;
        y_data[nm1d2 + 1] = b1 - kd;
      }
    }
  }
}

/*
 * File trailer for colon.c
 *
 * [EOF]
 */
