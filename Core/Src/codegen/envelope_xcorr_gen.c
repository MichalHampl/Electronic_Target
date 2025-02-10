/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: envelope_xcorr_gen.c
 *
 * MATLAB Coder version            : 23.2
 * C/C++ source code generated on  : 15-Nov-2024 13:26:43
 */

/* Include Files */
#include "envelope_xcorr_gen.h"
#include "colon.h"
#include "envelope.h"
#include "envelope_xcorr_gen_emxutil.h"
#include "envelope_xcorr_gen_types.h"
#include "fft.h"
#include "find.h"
#include "ifft.h"
#include "interp1.h"
#include "mean.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }
  return y;
}

/*
 * Arguments    : float ms1[256]
 *                float ms2[256]
 *                float N
 *                float fs
 *                float channels
 *                float window
 *                float thrshld
 * Return Type  : float
 */
float envelope_xcorr_gen(float ms1[256], float ms2[256], float N, float fs,
                         float channels, float window, float thrshld)
{
  emxArray_boolean_T *b_x;
  emxArray_creal32_T *X;
  emxArray_creal32_T *Y;
  emxArray_int32_T *r1;
  emxArray_real32_T c_ms1;
  emxArray_real32_T *b_c1;
  emxArray_real32_T *b_ms1;
  emxArray_real32_T *c1;
  emxArray_real32_T *r;
  emxArray_real32_T *x;
  emxArray_real32_T *xq;
  creal32_T *X_data;
  creal32_T *Y_data;
  double c0;
  double f_tmp;
  double m2;
  double tdops;
  float Ts;
  float t1_data;
  float t3_data;
  float t4_data;
  float *c1_data;
  float *ms1_data;
  float *x_data;
  float *xq_data;
  int tmp_size[2];
  int ceilLog2;
  int d_ms1;
  int e_ms1;
  int i;
  int k;
  int m;
  int maxval_tmp_tmp;
  int mxl;
  int n;
  int nx;
  int *r2;
  bool *b_x_data;
  (void)channels;
  Ts = 1.0F / fs;
  t4_data = ms1[0];
  for (k = 0; k < 255; k++) {
    t4_data += ms1[k + 1];
  }
  t4_data /= 256.0F;
  for (i = 0; i < 256; i++) {
    ms1[i] -= t4_data;
  }
  t4_data = ms2[0];
  for (k = 0; k < 255; k++) {
    t4_data += ms2[k + 1];
  }
  t4_data /= 256.0F;
  for (i = 0; i < 256; i++) {
    ms2[i] -= t4_data;
  }
  /* interpolace */
  t4_data = (N - 1.0F) * Ts;
  emxInit_real32_T(&x, 2);
  if (rtIsNaNF(Ts) || rtIsNaNF(t4_data)) {
    i = x->size[0] * x->size[1];
    x->size[0] = 1;
    x->size[1] = 1;
    emxEnsureCapacity_real32_T(x, i);
    x_data = x->data;
    x_data[0] = rtNaNF;
  } else if ((Ts == 0.0F) || ((t4_data > 0.0F) && (Ts < 0.0F)) ||
             ((t4_data < 0.0F) && (Ts > 0.0F))) {
    x->size[0] = 1;
    x->size[1] = 0;
  } else if (rtIsInfF(t4_data) && rtIsInfF(Ts)) {
    i = x->size[0] * x->size[1];
    x->size[0] = 1;
    x->size[1] = 1;
    emxEnsureCapacity_real32_T(x, i);
    x_data = x->data;
    x_data[0] = rtNaNF;
  } else if (rtIsInfF(Ts)) {
    i = x->size[0] * x->size[1];
    x->size[0] = 1;
    x->size[1] = 1;
    emxEnsureCapacity_real32_T(x, i);
    x_data = x->data;
    x_data[0] = 0.0F;
  } else if (floorf(Ts) == Ts) {
    i = x->size[0] * x->size[1];
    x->size[0] = 1;
    m = (int)((double)t4_data / Ts);
    x->size[1] = m + 1;
    emxEnsureCapacity_real32_T(x, i);
    x_data = x->data;
    for (i = 0; i <= m; i++) {
      x_data[i] = (float)(Ts * (double)i);
    }
  } else {
    eml_float_colon(Ts, t4_data, x);
  }
  t1_data = Ts / 4.0F;
  t4_data = N * Ts - Ts / 4.0F;
  emxInit_real32_T(&xq, 2);
  if (rtIsNaNF(t1_data) || rtIsNaNF(t4_data)) {
    i = xq->size[0] * xq->size[1];
    xq->size[0] = 1;
    xq->size[1] = 1;
    emxEnsureCapacity_real32_T(xq, i);
    xq_data = xq->data;
    xq_data[0] = rtNaNF;
  } else if ((t1_data == 0.0F) || ((t4_data > 0.0F) && (t1_data < 0.0F)) ||
             ((t4_data < 0.0F) && (t1_data > 0.0F))) {
    xq->size[0] = 1;
    xq->size[1] = 0;
  } else if (rtIsInfF(t4_data) && rtIsInfF(t1_data)) {
    i = xq->size[0] * xq->size[1];
    xq->size[0] = 1;
    xq->size[1] = 1;
    emxEnsureCapacity_real32_T(xq, i);
    xq_data = xq->data;
    xq_data[0] = rtNaNF;
  } else if (rtIsInfF(t1_data)) {
    i = xq->size[0] * xq->size[1];
    xq->size[0] = 1;
    xq->size[1] = 1;
    emxEnsureCapacity_real32_T(xq, i);
    xq_data = xq->data;
    xq_data[0] = 0.0F;
  } else if (floorf(t1_data) == t1_data) {
    i = xq->size[0] * xq->size[1];
    xq->size[0] = 1;
    m = (int)((double)t4_data / t1_data);
    xq->size[1] = m + 1;
    emxEnsureCapacity_real32_T(xq, i);
    xq_data = xq->data;
    for (i = 0; i <= m; i++) {
      xq_data[i] = (float)(t1_data * (double)i);
    }
  } else {
    eml_float_colon(t1_data, t4_data, xq);
  }
  /* filtrace  */
  emxInit_real32_T(&r, 2);
  interp1(x, ms1, xq, r);
  emxInit_real32_T(&b_ms1, 2);
  envelope(r, b_ms1);
  interp1(x, ms2, xq, r);
  envelope(r, xq);
  emxFree_real32_T(&r);
  t3_data = mean(b_ms1);
  m = b_ms1->size[1] - 1;
  i = b_ms1->size[0] * b_ms1->size[1];
  b_ms1->size[0] = 1;
  emxEnsureCapacity_real32_T(b_ms1, i);
  ms1_data = b_ms1->data;
  for (i = 0; i <= m; i++) {
    ms1_data[i] -= t3_data;
  }
  t3_data = mean(xq);
  m = xq->size[1] - 1;
  i = xq->size[0] * xq->size[1];
  xq->size[0] = 1;
  emxEnsureCapacity_real32_T(xq, i);
  xq_data = xq->data;
  for (i = 0; i <= m; i++) {
    xq_data[i] -= t3_data;
  }
  /* osekání signálu */
  /* thrshld = 400; */
  /* window = 200; */
  nx = b_ms1->size[1];
  i = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = b_ms1->size[1];
  emxEnsureCapacity_real32_T(x, i);
  x_data = x->data;
  for (k = 0; k < nx; k++) {
    x_data[k] = fabsf(ms1_data[k]);
  }
  emxInit_boolean_T(&b_x);
  i = b_x->size[0] * b_x->size[1];
  b_x->size[0] = 1;
  b_x->size[1] = x->size[1];
  emxEnsureCapacity_boolean_T(b_x, i);
  b_x_data = b_x->data;
  m = x->size[1];
  for (i = 0; i < m; i++) {
    b_x_data[i] = (x_data[i] > thrshld);
  }
  eml_find(b_x, (int *)&nx, tmp_size);
  if (tmp_size[1] - 1 >= 0) {
    t1_data = (float)nx + window;
  }
  t4_data = window / 4.0F;
  nx = b_ms1->size[1];
  i = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = b_ms1->size[1];
  emxEnsureCapacity_real32_T(x, i);
  x_data = x->data;
  for (k = 0; k < nx; k++) {
    x_data[k] = fabsf(ms1_data[k]);
  }
  i = b_x->size[0] * b_x->size[1];
  b_x->size[0] = 1;
  b_x->size[1] = x->size[1];
  emxEnsureCapacity_boolean_T(b_x, i);
  b_x_data = b_x->data;
  m = x->size[1];
  for (i = 0; i < m; i++) {
    b_x_data[i] = (x_data[i] > thrshld);
  }
  eml_find(b_x, (int *)&nx, tmp_size);
  if (tmp_size[1] - 1 >= 0) {
    Ts = (float)nx - t4_data;
  }
  nx = xq->size[1];
  i = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = xq->size[1];
  emxEnsureCapacity_real32_T(x, i);
  x_data = x->data;
  for (k = 0; k < nx; k++) {
    x_data[k] = fabsf(xq_data[k]);
  }
  i = b_x->size[0] * b_x->size[1];
  b_x->size[0] = 1;
  b_x->size[1] = x->size[1];
  emxEnsureCapacity_boolean_T(b_x, i);
  b_x_data = b_x->data;
  m = x->size[1];
  for (i = 0; i < m; i++) {
    b_x_data[i] = (x_data[i] > thrshld);
  }
  eml_find(b_x, (int *)&nx, tmp_size);
  if (tmp_size[1] - 1 >= 0) {
    t3_data = (float)nx + window;
  }
  nx = xq->size[1];
  i = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = xq->size[1];
  emxEnsureCapacity_real32_T(x, i);
  x_data = x->data;
  for (k = 0; k < nx; k++) {
    x_data[k] = fabsf(xq_data[k]);
  }
  i = b_x->size[0] * b_x->size[1];
  b_x->size[0] = 1;
  b_x->size[1] = x->size[1];
  emxEnsureCapacity_boolean_T(b_x, i);
  b_x_data = b_x->data;
  m = x->size[1];
  for (i = 0; i < m; i++) {
    b_x_data[i] = (x_data[i] > thrshld);
  }
  emxFree_real32_T(&x);
  eml_find(b_x, (int *)&nx, tmp_size);
  emxFree_boolean_T(&b_x);
  if (tmp_size[1] - 1 >= 0) {
    t4_data = (float)nx - t4_data;
  }
  if (t1_data > b_ms1->size[1]) {
    i = 0;
    nx = 0;
  } else {
    i = (int)t1_data - 1;
    nx = (int)(float)b_ms1->size[1];
  }
  m = nx - i;
  for (nx = 0; nx < m; nx++) {
    ms1_data[i + nx] = 0.0F;
  }
  if (Ts < 1.0F) {
    m = 0;
  } else {
    m = (int)Ts;
  }
  for (i = 0; i < m; i++) {
    ms1_data[i] = 0.0F;
  }
  if (t3_data > xq->size[1]) {
    i = 0;
    nx = 0;
  } else {
    i = (int)t3_data - 1;
    nx = (int)(float)xq->size[1];
  }
  m = nx - i;
  for (nx = 0; nx < m; nx++) {
    xq_data[i + nx] = 0.0F;
  }
  if (t4_data < 1.0F) {
    m = 0;
  } else {
    m = (int)t4_data;
  }
  for (i = 0; i < m; i++) {
    xq_data[i] = 0.0F;
  }
  maxval_tmp_tmp = (int)fmax(b_ms1->size[1], xq->size[1]);
  mxl = (int)fmin((double)maxval_tmp_tmp - 1.0, (double)maxval_tmp_tmp - 1.0);
  f_tmp = 2.0 * (double)maxval_tmp_tmp;
  tdops = frexp(fabs(f_tmp - 1.0), &ceilLog2);
  if (tdops == 0.5) {
    ceilLog2--;
  }
  m2 = rt_powd_snf(2.0, ceilLog2);
  n = (int)fmin(b_ms1->size[1], xq->size[1]);
  c0 = 2.0 * (double)n - 1.0;
  if (mxl <= n - 1) {
    tdops = (double)mxl * ((c0 - (double)mxl) - 1.0);
    nx = maxval_tmp_tmp - n;
    if (mxl <= nx) {
      tdops += c0 + (double)mxl * c0;
    } else {
      tdops += (c0 + (double)nx * c0) +
               ((double)mxl - (double)nx) *
                   ((((double)maxval_tmp_tmp - (double)mxl) + (double)n) - 2.0);
    }
  } else if (mxl <= maxval_tmp_tmp - 1) {
    tdops = ((double)n - 1.0) * ((double)n - 1.0);
    nx = maxval_tmp_tmp - n;
    if (mxl <= nx) {
      tdops += c0 + (double)mxl * c0;
    } else {
      tdops += (c0 + (double)nx * c0) +
               ((double)mxl - (double)nx) *
                   (((double)(maxval_tmp_tmp - mxl) + (double)n) - 2.0);
    }
  } else {
    tdops = f_tmp * (double)n -
            ((double)((unsigned int)maxval_tmp_tmp + (unsigned int)n) - 1.0);
  }
  emxInit_real32_T(&c1, 1);
  emxInit_real32_T(&b_c1, 1);
  if (tdops < m2 * (15.0 * (double)ceilLog2 + 6.0)) {
    m = b_ms1->size[1];
    n = xq->size[1];
    mxl = (int)fmin(mxl, (double)maxval_tmp_tmp - 1.0);
    nx = (int)(2.0 * (double)mxl + 1.0);
    i = c1->size[0];
    c1->size[0] = nx;
    emxEnsureCapacity_real32_T(c1, i);
    c1_data = c1->data;
    for (i = 0; i < nx; i++) {
      c1_data[i] = 0.0F;
    }
    for (k = 0; k <= mxl; k++) {
      t4_data = 0.0F;
      i = (int)fmin(m - k, n);
      for (nx = 0; nx < i; nx++) {
        t4_data += xq_data[nx] * ms1_data[k + nx];
      }
      c1_data[mxl + k] = t4_data;
    }
    for (k = 0; k < mxl; k++) {
      t4_data = 0.0F;
      i = (int)fmin(m, (n - k) - 1);
      for (nx = 0; nx < i; nx++) {
        t4_data += xq_data[(k + nx) + 1] * ms1_data[nx];
      }
      c1_data[(mxl - k) - 1] = t4_data;
    }
  } else {
    nx = b_ms1->size[1];
    c_ms1 = *b_ms1;
    d_ms1 = nx;
    c_ms1.size = &d_ms1;
    c_ms1.numDimensions = 1;
    emxInit_creal32_T(&X);
    fft(&c_ms1, m2, X);
    X_data = X->data;
    nx = xq->size[1];
    c_ms1 = *xq;
    e_ms1 = nx;
    c_ms1.size = &e_ms1;
    c_ms1.numDimensions = 1;
    emxInit_creal32_T(&Y);
    fft(&c_ms1, m2, Y);
    i = Y->size[0];
    Y->size[0] = X->size[0];
    emxEnsureCapacity_creal32_T(Y, i);
    Y_data = Y->data;
    m = X->size[0];
    for (i = 0; i < m; i++) {
      t1_data = Y_data[i].re;
      t4_data = -Y_data[i].im;
      t3_data = X_data[i].re;
      Ts = X_data[i].im;
      Y_data[i].re = t3_data * t1_data - Ts * t4_data;
      Y_data[i].im = t3_data * t4_data + Ts * t1_data;
    }
    ifft(Y, X);
    X_data = X->data;
    emxFree_creal32_T(&Y);
    i = b_c1->size[0];
    b_c1->size[0] = X->size[0];
    emxEnsureCapacity_real32_T(b_c1, i);
    x_data = b_c1->data;
    m = X->size[0];
    for (i = 0; i < m; i++) {
      x_data[i] = X_data[i].re;
    }
    emxFree_creal32_T(&X);
    emxInit_int32_T(&r1, 2);
    r2 = r1->data;
    if (mxl < 1) {
      r1->size[0] = 1;
      r1->size[1] = 0;
    } else {
      i = r1->size[0] * r1->size[1];
      r1->size[0] = 1;
      r1->size[1] = mxl;
      emxEnsureCapacity_int32_T(r1, i);
      r2 = r1->data;
      for (i = 0; i < mxl; i++) {
        r2[i] = i + 1;
      }
    }
    if (mxl + 1 < 1) {
      m = -1;
    } else {
      m = mxl;
    }
    m2 -= (double)mxl;
    i = c1->size[0];
    c1->size[0] = (r1->size[1] + m) + 1;
    emxEnsureCapacity_real32_T(c1, i);
    c1_data = c1->data;
    nx = r1->size[1];
    for (i = 0; i < nx; i++) {
      c1_data[i] = x_data[(int)(m2 + (double)r2[i]) - 1];
    }
    for (i = 0; i <= m; i++) {
      c1_data[i + r1->size[1]] = x_data[i];
    }
    emxFree_int32_T(&r1);
  }
  emxFree_real32_T(&b_ms1);
  emxFree_real32_T(&xq);
  m = (int)(2.0 * ((double)maxval_tmp_tmp - 1.0) + 1.0);
  i = b_c1->size[0];
  b_c1->size[0] = m;
  emxEnsureCapacity_real32_T(b_c1, i);
  x_data = b_c1->data;
  for (i = 0; i < m; i++) {
    x_data[i] = 0.0F;
  }
  m = c1->size[0];
  for (i = 0; i < m; i++) {
    x_data[i] = c1_data[i];
  }
  emxFree_real32_T(&c1);
  nx = b_c1->size[0];
  if (b_c1->size[0] <= 2) {
    if (b_c1->size[0] == 1) {
      m = 1;
    } else {
      t3_data = x_data[b_c1->size[0] - 1];
      if ((x_data[0] < t3_data) ||
          (rtIsNaNF(x_data[0]) && (!rtIsNaNF(t3_data)))) {
        m = b_c1->size[0];
      } else {
        m = 1;
      }
    }
  } else {
    if (!rtIsNaNF(x_data[0])) {
      m = 1;
    } else {
      bool exitg1;
      m = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= nx)) {
        if (!rtIsNaNF(x_data[k - 1])) {
          m = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (m == 0) {
      m = 1;
    } else {
      t4_data = x_data[m - 1];
      i = m + 1;
      for (k = i; k <= nx; k++) {
        t3_data = x_data[k - 1];
        if (t4_data < t3_data) {
          t4_data = t3_data;
          m = k;
        }
      }
    }
  }
  emxFree_real32_T(&b_c1);
  return ((float)m - N * 4.0F) / (fs * 4.0F);
}

/*
 * File trailer for envelope_xcorr_gen.c
 *
 * [EOF]
 */
