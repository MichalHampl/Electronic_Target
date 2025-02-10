/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ifft.c
 *
 * MATLAB Coder version            : 23.2
 * C/C++ source code generated on  : 15-Nov-2024 13:26:43
 */

/* Include Files */
#include "ifft.h"
#include "FFTImplementationCallback.h"
#include "envelope_xcorr_gen_emxutil.h"
#include "envelope_xcorr_gen_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal32_T *x
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void ifft(const emxArray_creal32_T *x, emxArray_creal32_T *y)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *wwc;
  emxArray_real32_T *costab;
  emxArray_real32_T *costab1q;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  const creal32_T *x_data;
  creal32_T *b_fv_data;
  creal32_T *fv_data;
  creal32_T *wwc_data;
  creal32_T *y_data;
  float *costab1q_data;
  float *costab_data;
  float *sintab_data;
  float *sintabinv_data;
  int i;
  int k;
  int nd2;
  int nfft;
  x_data = x->data;
  nfft = x->size[0];
  if (x->size[0] == 0) {
    y->size[0] = 0;
  } else {
    float nt_im;
    int N2blue;
    int nInt2;
    int rt;
    bool useRadix2;
    useRadix2 = ((x->size[0] & (x->size[0] - 1)) == 0);
    N2blue = c_FFTImplementationCallback_get(x->size[0], useRadix2, &nd2);
    nt_im = 6.28318548F / (float)nd2;
    nInt2 = (int)((unsigned int)nd2 >> 1) >> 1;
    emxInit_real32_T(&costab1q, 2);
    i = costab1q->size[0] * costab1q->size[1];
    costab1q->size[0] = 1;
    costab1q->size[1] = nInt2 + 1;
    emxEnsureCapacity_real32_T(costab1q, i);
    costab1q_data = costab1q->data;
    costab1q_data[0] = 1.0F;
    nd2 = nInt2 / 2 - 1;
    for (k = 0; k <= nd2; k++) {
      costab1q_data[k + 1] = cosf(nt_im * (float)(k + 1));
    }
    i = nd2 + 2;
    rt = nInt2 - 1;
    for (k = i; k <= rt; k++) {
      costab1q_data[k] = sinf(nt_im * (float)(nInt2 - k));
    }
    costab1q_data[nInt2] = 0.0F;
    emxInit_real32_T(&costab, 2);
    emxInit_real32_T(&sintab, 2);
    emxInit_real32_T(&sintabinv, 2);
    if (!useRadix2) {
      nInt2 = costab1q->size[1] - 1;
      nd2 = (costab1q->size[1] - 1) << 1;
      i = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(costab, i);
      costab_data = costab->data;
      i = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(sintab, i);
      sintab_data = sintab->data;
      costab_data[0] = 1.0F;
      sintab_data[0] = 0.0F;
      i = sintabinv->size[0] * sintabinv->size[1];
      sintabinv->size[0] = 1;
      sintabinv->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(sintabinv, i);
      sintabinv_data = sintabinv->data;
      for (k = 0; k < nInt2; k++) {
        sintabinv_data[k + 1] = costab1q_data[(nInt2 - k) - 1];
      }
      i = costab1q->size[1];
      for (k = i; k <= nd2; k++) {
        sintabinv_data[k] = costab1q_data[k - nInt2];
      }
      for (k = 0; k < nInt2; k++) {
        costab_data[k + 1] = costab1q_data[k + 1];
        sintab_data[k + 1] = -costab1q_data[(nInt2 - k) - 1];
      }
      i = costab1q->size[1];
      for (k = i; k <= nd2; k++) {
        costab_data[k] = -costab1q_data[nd2 - k];
        sintab_data[k] = -costab1q_data[k - nInt2];
      }
    } else {
      nInt2 = costab1q->size[1] - 1;
      nd2 = (costab1q->size[1] - 1) << 1;
      i = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(costab, i);
      costab_data = costab->data;
      i = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(sintab, i);
      sintab_data = sintab->data;
      costab_data[0] = 1.0F;
      sintab_data[0] = 0.0F;
      for (k = 0; k < nInt2; k++) {
        costab_data[k + 1] = costab1q_data[k + 1];
        sintab_data[k + 1] = costab1q_data[(nInt2 - k) - 1];
      }
      i = costab1q->size[1];
      for (k = i; k <= nd2; k++) {
        costab_data[k] = -costab1q_data[nd2 - k];
        sintab_data[k] = costab1q_data[k - nInt2];
      }
      sintabinv->size[0] = 1;
      sintabinv->size[1] = 0;
    }
    emxFree_real32_T(&costab1q);
    if (useRadix2) {
      d_FFTImplementationCallback_r2b(x, x->size[0], costab, sintab, y);
      y_data = y->data;
      if (y->size[0] > 1) {
        nt_im = 1.0F / (float)y->size[0];
        nd2 = y->size[0];
        for (i = 0; i < nd2; i++) {
          y_data[i].re *= nt_im;
          y_data[i].im *= nt_im;
        }
      }
    } else {
      float b_re_tmp;
      float c_re_tmp;
      float nt_re;
      float re_tmp;
      nd2 = (x->size[0] + x->size[0]) - 1;
      emxInit_creal32_T(&wwc);
      i = wwc->size[0];
      wwc->size[0] = nd2;
      emxEnsureCapacity_creal32_T(wwc, i);
      wwc_data = wwc->data;
      rt = 0;
      wwc_data[x->size[0] - 1].re = 1.0F;
      wwc_data[x->size[0] - 1].im = 0.0F;
      nInt2 = x->size[0] << 1;
      i = x->size[0];
      for (k = 0; k <= i - 2; k++) {
        int b_y;
        b_y = ((k + 1) << 1) - 1;
        if (nInt2 - rt <= b_y) {
          rt += b_y - nInt2;
        } else {
          rt += b_y;
        }
        nt_im = 3.14159274F * (float)rt / (float)x->size[0];
        wwc_data[(x->size[0] - k) - 2].re = cosf(nt_im);
        wwc_data[(x->size[0] - k) - 2].im = -sinf(nt_im);
      }
      i = nd2 - 1;
      for (k = i; k >= nfft; k--) {
        wwc_data[k] = wwc_data[(nd2 - k) - 1];
      }
      i = y->size[0];
      y->size[0] = x->size[0];
      emxEnsureCapacity_creal32_T(y, i);
      y_data = y->data;
      nd2 = x->size[0];
      for (k = 0; k < nd2; k++) {
        rt = (nfft + k) - 1;
        nt_re = wwc_data[rt].re;
        nt_im = wwc_data[rt].im;
        re_tmp = x_data[k].im;
        b_re_tmp = x_data[k].re;
        y_data[k].re = nt_re * b_re_tmp + nt_im * re_tmp;
        y_data[k].im = nt_re * re_tmp - nt_im * b_re_tmp;
      }
      i = x->size[0] + 1;
      for (k = i; k <= nfft; k++) {
        y_data[k - 1].re = 0.0F;
        y_data[k - 1].im = 0.0F;
      }
      emxInit_creal32_T(&fv);
      d_FFTImplementationCallback_r2b(y, N2blue, costab, sintab, fv);
      fv_data = fv->data;
      emxInit_creal32_T(&b_fv);
      d_FFTImplementationCallback_r2b(wwc, N2blue, costab, sintab, b_fv);
      i = b_fv->size[0];
      b_fv->size[0] = fv->size[0];
      emxEnsureCapacity_creal32_T(b_fv, i);
      b_fv_data = b_fv->data;
      nd2 = fv->size[0];
      for (i = 0; i < nd2; i++) {
        nt_im = fv_data[i].re;
        re_tmp = b_fv_data[i].im;
        b_re_tmp = fv_data[i].im;
        c_re_tmp = b_fv_data[i].re;
        b_fv_data[i].re = nt_im * c_re_tmp - b_re_tmp * re_tmp;
        b_fv_data[i].im = nt_im * re_tmp + b_re_tmp * c_re_tmp;
      }
      d_FFTImplementationCallback_r2b(b_fv, N2blue, costab, sintabinv, fv);
      fv_data = fv->data;
      emxFree_creal32_T(&b_fv);
      if (fv->size[0] > 1) {
        nt_im = 1.0F / (float)fv->size[0];
        nd2 = fv->size[0];
        for (i = 0; i < nd2; i++) {
          fv_data[i].re *= nt_im;
          fv_data[i].im *= nt_im;
        }
      }
      nt_re = (float)x->size[0];
      i = (int)(float)x->size[0];
      rt = wwc->size[0];
      for (k = i; k <= rt; k++) {
        float ar;
        nt_im = wwc_data[k - 1].re;
        re_tmp = fv_data[k - 1].im;
        b_re_tmp = wwc_data[k - 1].im;
        c_re_tmp = fv_data[k - 1].re;
        ar = nt_im * c_re_tmp + b_re_tmp * re_tmp;
        nt_im = nt_im * re_tmp - b_re_tmp * c_re_tmp;
        if (nt_im == 0.0F) {
          nd2 = k - i;
          y_data[nd2].re = ar / nt_re;
          y_data[nd2].im = 0.0F;
        } else if (ar == 0.0F) {
          nd2 = k - i;
          y_data[nd2].re = 0.0F;
          y_data[nd2].im = nt_im / nt_re;
        } else {
          nd2 = k - i;
          y_data[nd2].re = ar / nt_re;
          y_data[nd2].im = nt_im / nt_re;
        }
      }
      emxFree_creal32_T(&fv);
      emxFree_creal32_T(&wwc);
    }
    emxFree_real32_T(&sintabinv);
    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
  }
}

/*
 * File trailer for ifft.c
 *
 * [EOF]
 */
