/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: FFTImplementationCallback.c
 *
 * MATLAB Coder version            : 23.2
 * C/C++ source code generated on  : 15-Nov-2024 13:26:43
 */

/* Include Files */
#include "FFTImplementationCallback.h"
#include "envelope_xcorr_gen_emxutil.h"
#include "envelope_xcorr_gen_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static void c_FFTImplementationCallback_doH(const emxArray_real32_T *x,
                                            emxArray_creal32_T *y,
                                            int unsigned_nRows,
                                            const emxArray_real32_T *costab,
                                            const emxArray_real32_T *sintab);

static void d_FFTImplementationCallback_doH(
    const emxArray_real32_T *x, emxArray_creal32_T *y, int nrowsx, int nRows,
    int nfft, const emxArray_creal32_T *wwc, const emxArray_real32_T *costab,
    const emxArray_real32_T *sintab, const emxArray_real32_T *costabinv,
    const emxArray_real32_T *sintabinv);

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *x
 *                emxArray_creal32_T *y
 *                int unsigned_nRows
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 * Return Type  : void
 */
static void c_FFTImplementationCallback_doH(const emxArray_real32_T *x,
                                            emxArray_creal32_T *y,
                                            int unsigned_nRows,
                                            const emxArray_real32_T *costab,
                                            const emxArray_real32_T *sintab)
{
  emxArray_creal32_T *reconVar1;
  emxArray_creal32_T *reconVar2;
  emxArray_int32_T *bitrevIndex;
  emxArray_int32_T *wrapIndex;
  emxArray_real32_T *hcostab;
  emxArray_real32_T *hsintab;
  creal32_T *reconVar1_data;
  creal32_T *reconVar2_data;
  creal32_T *y_data;
  double d;
  double z;
  const float *costab_data;
  const float *sintab_data;
  const float *x_data;
  float b_y_re_tmp;
  float c_y_re_tmp;
  float d_y_re_tmp;
  float im;
  float re;
  float temp2_im;
  float temp2_re;
  float temp_im;
  float temp_im_tmp;
  float temp_re;
  float temp_re_tmp;
  float y_re_tmp;
  float *hcostab_data;
  float *hsintab_data;
  int hszCostab;
  int i;
  int iDelta2;
  int iheight;
  int iterVar;
  int ju;
  int k;
  int nRows;
  int nRowsD2;
  int u0;
  int *bitrevIndex_data;
  int *wrapIndex_data;
  bool tst;
  sintab_data = sintab->data;
  costab_data = costab->data;
  y_data = y->data;
  x_data = x->data;
  nRows = (int)((unsigned int)unsigned_nRows >> 1);
  u0 = y->size[0];
  if (u0 > nRows) {
    u0 = nRows;
  }
  iDelta2 = nRows - 2;
  nRowsD2 = nRows / 2;
  k = nRowsD2 / 2;
  hszCostab = (int)((unsigned int)costab->size[1] >> 1);
  emxInit_real32_T(&hcostab, 2);
  iheight = hcostab->size[0] * hcostab->size[1];
  hcostab->size[0] = 1;
  hcostab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hcostab, iheight);
  hcostab_data = hcostab->data;
  emxInit_real32_T(&hsintab, 2);
  iheight = hsintab->size[0] * hsintab->size[1];
  hsintab->size[0] = 1;
  hsintab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hsintab, iheight);
  hsintab_data = hsintab->data;
  for (i = 0; i < hszCostab; i++) {
    iterVar = ((i + 1) << 1) - 2;
    hcostab_data[i] = costab_data[iterVar];
    hsintab_data[i] = sintab_data[iterVar];
  }
  emxInit_creal32_T(&reconVar1);
  iheight = reconVar1->size[0];
  reconVar1->size[0] = nRows;
  emxEnsureCapacity_creal32_T(reconVar1, iheight);
  reconVar1_data = reconVar1->data;
  emxInit_creal32_T(&reconVar2);
  iheight = reconVar2->size[0];
  reconVar2->size[0] = nRows;
  emxEnsureCapacity_creal32_T(reconVar2, iheight);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  iheight = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = nRows;
  emxEnsureCapacity_int32_T(wrapIndex, iheight);
  wrapIndex_data = wrapIndex->data;
  for (i = 0; i < nRows; i++) {
    temp2_im = sintab_data[i];
    temp2_re = costab_data[i];
    reconVar1_data[i].re = temp2_im + 1.0F;
    reconVar1_data[i].im = -temp2_re;
    reconVar2_data[i].re = 1.0F - temp2_im;
    reconVar2_data[i].im = temp2_re;
    if (i + 1 != 1) {
      wrapIndex_data[i] = (nRows - i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  z = (double)unsigned_nRows / 2.0;
  ju = 0;
  hszCostab = 1;
  emxInit_int32_T(&bitrevIndex, 1);
  iterVar = (int)z;
  iheight = bitrevIndex->size[0];
  bitrevIndex->size[0] = (int)z;
  emxEnsureCapacity_int32_T(bitrevIndex, iheight);
  bitrevIndex_data = bitrevIndex->data;
  for (iheight = 0; iheight < iterVar; iheight++) {
    bitrevIndex_data[iheight] = 0;
  }
  for (iheight = 0; iheight <= u0 - 2; iheight++) {
    bitrevIndex_data[iheight] = hszCostab;
    iterVar = (int)z;
    tst = true;
    while (tst) {
      iterVar >>= 1;
      ju ^= iterVar;
      tst = ((ju & iterVar) == 0);
    }
    hszCostab = ju + 1;
  }
  bitrevIndex_data[u0 - 1] = hszCostab;
  if ((x->size[0] & 1) == 0) {
    tst = true;
    u0 = x->size[0];
  } else if (x->size[0] >= unsigned_nRows) {
    tst = true;
    u0 = unsigned_nRows;
  } else {
    tst = false;
    u0 = x->size[0] - 1;
  }
  if (u0 > unsigned_nRows) {
    u0 = unsigned_nRows;
  }
  d = (double)u0 / 2.0;
  iheight = (int)d;
  for (i = 0; i < iheight; i++) {
    hszCostab = i << 1;
    y_data[bitrevIndex_data[i] - 1].re = x_data[hszCostab];
    y_data[bitrevIndex_data[i] - 1].im = x_data[hszCostab + 1];
  }
  if (!tst) {
    iheight = bitrevIndex_data[(int)d] - 1;
    if ((int)d - 1 < 0) {
      u0 = 0;
    } else {
      u0 = (int)d << 1;
    }
    y_data[iheight].re = x_data[u0];
    y_data[iheight].im = 0.0F;
  }
  emxFree_int32_T(&bitrevIndex);
  if (nRows > 1) {
    for (i = 0; i <= iDelta2; i += 2) {
      temp2_re = y_data[i + 1].re;
      temp2_im = y_data[i + 1].im;
      temp_re = temp2_re;
      temp_im = temp2_im;
      re = y_data[i].re;
      im = y_data[i].im;
      temp2_re = re - temp2_re;
      temp2_im = im - temp2_im;
      y_data[i + 1].re = temp2_re;
      y_data[i + 1].im = temp2_im;
      re += temp_re;
      im += temp_im;
      y_data[i].re = re;
      y_data[i].im = im;
    }
  }
  nRows = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      iterVar = i + nRows;
      temp_re = y_data[iterVar].re;
      temp_im = y_data[iterVar].im;
      y_data[iterVar].re = y_data[i].re - temp_re;
      y_data[iterVar].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }
    iterVar = 1;
    for (hszCostab = k; hszCostab < nRowsD2; hszCostab += k) {
      temp2_re = hcostab_data[hszCostab];
      temp2_im = hsintab_data[hszCostab];
      i = iterVar;
      ju = iterVar + iheight;
      while (i < ju) {
        u0 = i + nRows;
        temp_re_tmp = y_data[u0].im;
        temp_im = y_data[u0].re;
        temp_re = temp2_re * temp_im - temp2_im * temp_re_tmp;
        temp_im = temp2_re * temp_re_tmp + temp2_im * temp_im;
        y_data[u0].re = y_data[i].re - temp_re;
        y_data[u0].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }
      iterVar++;
    }
    k /= 2;
    nRows = iDelta2;
    iDelta2 += iDelta2;
    iheight -= nRows;
  }
  emxFree_real32_T(&hsintab);
  emxFree_real32_T(&hcostab);
  iterVar = (int)((unsigned int)(int)z >> 1);
  temp2_im = y_data[0].re;
  temp_im_tmp = y_data[0].im;
  y_re_tmp = temp2_im * reconVar1_data[0].re;
  temp_im = temp2_im * reconVar1_data[0].im;
  b_y_re_tmp = temp2_im * reconVar2_data[0].re;
  re = temp2_im * reconVar2_data[0].im;
  y_data[0].re = 0.5F * ((y_re_tmp - temp_im_tmp * reconVar1_data[0].im) +
                         (b_y_re_tmp - -temp_im_tmp * reconVar2_data[0].im));
  y_data[0].im = 0.5F * ((temp_im + temp_im_tmp * reconVar1_data[0].re) +
                         (re + -temp_im_tmp * reconVar2_data[0].re));
  y_data[(int)z].re =
      0.5F * ((b_y_re_tmp - temp_im_tmp * reconVar2_data[0].im) +
              (y_re_tmp - -temp_im_tmp * reconVar1_data[0].im));
  y_data[(int)z].im = 0.5F * ((re + temp_im_tmp * reconVar2_data[0].re) +
                              (temp_im + -temp_im_tmp * reconVar1_data[0].re));
  for (i = 2; i <= iterVar; i++) {
    temp_re_tmp = y_data[i - 1].re;
    temp_im_tmp = y_data[i - 1].im;
    iheight = wrapIndex_data[i - 1];
    temp2_im = y_data[iheight - 1].re;
    temp_re = y_data[iheight - 1].im;
    y_re_tmp = reconVar1_data[i - 1].im;
    b_y_re_tmp = reconVar1_data[i - 1].re;
    c_y_re_tmp = reconVar2_data[i - 1].im;
    d_y_re_tmp = reconVar2_data[i - 1].re;
    y_data[i - 1].re =
        0.5F * ((temp_re_tmp * b_y_re_tmp - temp_im_tmp * y_re_tmp) +
                (temp2_im * d_y_re_tmp - -temp_re * c_y_re_tmp));
    y_data[i - 1].im =
        0.5F * ((temp_re_tmp * y_re_tmp + temp_im_tmp * b_y_re_tmp) +
                (temp2_im * c_y_re_tmp + -temp_re * d_y_re_tmp));
    hszCostab = ((int)z + i) - 1;
    y_data[hszCostab].re =
        0.5F * ((temp_re_tmp * d_y_re_tmp - temp_im_tmp * c_y_re_tmp) +
                (temp2_im * b_y_re_tmp - -temp_re * y_re_tmp));
    y_data[hszCostab].im =
        0.5F * ((temp_re_tmp * c_y_re_tmp + temp_im_tmp * d_y_re_tmp) +
                (temp2_im * y_re_tmp + -temp_re * b_y_re_tmp));
    re = reconVar1_data[iheight - 1].im;
    im = reconVar1_data[iheight - 1].re;
    temp_im = reconVar2_data[iheight - 1].im;
    temp2_re = reconVar2_data[iheight - 1].re;
    y_data[iheight - 1].re =
        0.5F * ((temp2_im * im - temp_re * re) +
                (temp_re_tmp * temp2_re - -temp_im_tmp * temp_im));
    y_data[iheight - 1].im =
        0.5F * ((temp2_im * re + temp_re * im) +
                (temp_re_tmp * temp_im + -temp_im_tmp * temp2_re));
    iheight = (iheight + (int)z) - 1;
    y_data[iheight].re = 0.5F * ((temp2_im * temp2_re - temp_re * temp_im) +
                                 (temp_re_tmp * im - -temp_im_tmp * re));
    y_data[iheight].im = 0.5F * ((temp2_im * temp_im + temp_re * temp2_re) +
                                 (temp_re_tmp * re + -temp_im_tmp * im));
  }
  emxFree_int32_T(&wrapIndex);
  if (iterVar != 0) {
    temp_re_tmp = y_data[iterVar].re;
    temp_im_tmp = y_data[iterVar].im;
    y_re_tmp = reconVar1_data[iterVar].im;
    b_y_re_tmp = reconVar1_data[iterVar].re;
    c_y_re_tmp = temp_re_tmp * b_y_re_tmp;
    temp_im = temp_re_tmp * y_re_tmp;
    d_y_re_tmp = reconVar2_data[iterVar].im;
    im = reconVar2_data[iterVar].re;
    temp2_re = temp_re_tmp * im;
    re = temp_re_tmp * d_y_re_tmp;
    y_data[iterVar].re = 0.5F * ((c_y_re_tmp - temp_im_tmp * y_re_tmp) +
                                 (temp2_re - -temp_im_tmp * d_y_re_tmp));
    y_data[iterVar].im = 0.5F * ((temp_im + temp_im_tmp * b_y_re_tmp) +
                                 (re + -temp_im_tmp * im));
    iheight = (int)z + iterVar;
    y_data[iheight].re = 0.5F * ((temp2_re - temp_im_tmp * d_y_re_tmp) +
                                 (c_y_re_tmp - -temp_im_tmp * y_re_tmp));
    y_data[iheight].im = 0.5F * ((re + temp_im_tmp * im) +
                                 (temp_im + -temp_im_tmp * b_y_re_tmp));
  }
  emxFree_creal32_T(&reconVar2);
  emxFree_creal32_T(&reconVar1);
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                emxArray_creal32_T *y
 *                int nrowsx
 *                int nRows
 *                int nfft
 *                const emxArray_creal32_T *wwc
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *costabinv
 *                const emxArray_real32_T *sintabinv
 * Return Type  : void
 */
static void d_FFTImplementationCallback_doH(
    const emxArray_real32_T *x, emxArray_creal32_T *y, int nrowsx, int nRows,
    int nfft, const emxArray_creal32_T *wwc, const emxArray_real32_T *costab,
    const emxArray_real32_T *sintab, const emxArray_real32_T *costabinv,
    const emxArray_real32_T *sintabinv)
{
  emxArray_creal32_T *fv;
  emxArray_creal32_T *fy;
  emxArray_creal32_T *reconVar1;
  emxArray_creal32_T *reconVar2;
  emxArray_creal32_T *ytmp;
  emxArray_int32_T *wrapIndex;
  emxArray_real32_T *b_costab;
  emxArray_real32_T *b_sintab;
  emxArray_real32_T *costab1q;
  emxArray_real32_T *hcostabinv;
  emxArray_real32_T *hsintab;
  emxArray_real32_T *hsintabinv;
  const creal32_T *wwc_data;
  creal32_T *fv_data;
  creal32_T *fy_data;
  creal32_T *reconVar1_data;
  creal32_T *reconVar2_data;
  creal32_T *y_data;
  creal32_T *ytmp_data;
  double z_tmp;
  const float *costab_data;
  const float *costabinv_data;
  const float *sintab_data;
  const float *sintabinv_data;
  const float *x_data;
  float temp_im;
  float temp_re;
  float temp_re_tmp;
  float twid_im;
  float twid_re;
  float *b_costab_data;
  float *b_sintab_data;
  float *costab1q_data;
  float *hcostabinv_data;
  float *hsintab_data;
  float *hsintabinv_data;
  int hnRows;
  int hszCostab;
  int i;
  int iDelta;
  int iDelta2;
  int iheight;
  int ju;
  int k;
  int nRowsD2;
  int nd2;
  int u1;
  int *wrapIndex_data;
  bool tst;
  sintabinv_data = sintabinv->data;
  costabinv_data = costabinv->data;
  sintab_data = sintab->data;
  costab_data = costab->data;
  wwc_data = wwc->data;
  y_data = y->data;
  x_data = x->data;
  hnRows = (int)((unsigned int)nRows >> 1);
  emxInit_creal32_T(&ytmp);
  u1 = ytmp->size[0];
  ytmp->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(ytmp, u1);
  ytmp_data = ytmp->data;
  if (hnRows > nrowsx) {
    u1 = ytmp->size[0];
    ytmp->size[0] = hnRows;
    emxEnsureCapacity_creal32_T(ytmp, u1);
    ytmp_data = ytmp->data;
    for (u1 = 0; u1 < hnRows; u1++) {
      ytmp_data[u1].re = 0.0F;
      ytmp_data[u1].im = 0.0F;
    }
  }
  if ((x->size[0] & 1) == 0) {
    tst = true;
    ju = x->size[0];
  } else if (x->size[0] >= nRows) {
    tst = true;
    ju = nRows;
  } else {
    tst = false;
    ju = x->size[0] - 1;
  }
  if (ju > nRows) {
    ju = nRows;
  }
  nd2 = nRows << 1;
  temp_im = 6.28318548F / (float)nd2;
  iDelta = (int)((unsigned int)nd2 >> 1) >> 1;
  emxInit_real32_T(&costab1q, 2);
  u1 = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = iDelta + 1;
  emxEnsureCapacity_real32_T(costab1q, u1);
  costab1q_data = costab1q->data;
  costab1q_data[0] = 1.0F;
  nd2 = iDelta / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q_data[k + 1] = cosf(temp_im * (float)(k + 1));
  }
  u1 = nd2 + 2;
  nd2 = iDelta - 1;
  for (k = u1; k <= nd2; k++) {
    costab1q_data[k] = sinf(temp_im * (float)(iDelta - k));
  }
  costab1q_data[iDelta] = 0.0F;
  iDelta = costab1q->size[1] - 1;
  nd2 = (costab1q->size[1] - 1) << 1;
  emxInit_real32_T(&b_costab, 2);
  u1 = b_costab->size[0] * b_costab->size[1];
  b_costab->size[0] = 1;
  b_costab->size[1] = nd2 + 1;
  emxEnsureCapacity_real32_T(b_costab, u1);
  b_costab_data = b_costab->data;
  emxInit_real32_T(&b_sintab, 2);
  u1 = b_sintab->size[0] * b_sintab->size[1];
  b_sintab->size[0] = 1;
  b_sintab->size[1] = nd2 + 1;
  emxEnsureCapacity_real32_T(b_sintab, u1);
  b_sintab_data = b_sintab->data;
  b_costab_data[0] = 1.0F;
  b_sintab_data[0] = 0.0F;
  for (k = 0; k < iDelta; k++) {
    b_costab_data[k + 1] = costab1q_data[k + 1];
    b_sintab_data[k + 1] = -costab1q_data[(iDelta - k) - 1];
  }
  u1 = costab1q->size[1];
  for (k = u1; k <= nd2; k++) {
    b_costab_data[k] = -costab1q_data[nd2 - k];
    b_sintab_data[k] = -costab1q_data[k - iDelta];
  }
  hszCostab = (int)((unsigned int)costab->size[1] >> 1);
  u1 = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(costab1q, u1);
  costab1q_data = costab1q->data;
  emxInit_real32_T(&hsintab, 2);
  u1 = hsintab->size[0] * hsintab->size[1];
  hsintab->size[0] = 1;
  hsintab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hsintab, u1);
  hsintab_data = hsintab->data;
  emxInit_real32_T(&hcostabinv, 2);
  u1 = hcostabinv->size[0] * hcostabinv->size[1];
  hcostabinv->size[0] = 1;
  hcostabinv->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hcostabinv, u1);
  hcostabinv_data = hcostabinv->data;
  emxInit_real32_T(&hsintabinv, 2);
  u1 = hsintabinv->size[0] * hsintabinv->size[1];
  hsintabinv->size[0] = 1;
  hsintabinv->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hsintabinv, u1);
  hsintabinv_data = hsintabinv->data;
  for (i = 0; i < hszCostab; i++) {
    nd2 = ((i + 1) << 1) - 2;
    costab1q_data[i] = costab_data[nd2];
    hsintab_data[i] = sintab_data[nd2];
    hcostabinv_data[i] = costabinv_data[nd2];
    hsintabinv_data[i] = sintabinv_data[nd2];
  }
  emxInit_creal32_T(&reconVar1);
  u1 = reconVar1->size[0];
  reconVar1->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar1, u1);
  reconVar1_data = reconVar1->data;
  emxInit_creal32_T(&reconVar2);
  u1 = reconVar2->size[0];
  reconVar2->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar2, u1);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  u1 = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = hnRows;
  emxEnsureCapacity_int32_T(wrapIndex, u1);
  wrapIndex_data = wrapIndex->data;
  for (i = 0; i < hnRows; i++) {
    u1 = i << 1;
    temp_im = b_sintab_data[u1];
    temp_re = b_costab_data[u1];
    reconVar1_data[i].re = temp_im + 1.0F;
    reconVar1_data[i].im = -temp_re;
    reconVar2_data[i].re = 1.0F - temp_im;
    reconVar2_data[i].im = temp_re;
    if (i + 1 != 1) {
      wrapIndex_data[i] = (hnRows - i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  emxFree_real32_T(&b_sintab);
  emxFree_real32_T(&b_costab);
  z_tmp = (double)ju / 2.0;
  u1 = (int)z_tmp;
  for (hszCostab = 0; hszCostab < u1; hszCostab++) {
    temp_re = wwc_data[(hnRows + hszCostab) - 1].re;
    temp_im = wwc_data[(hnRows + hszCostab) - 1].im;
    nd2 = hszCostab << 1;
    twid_re = x_data[nd2];
    twid_im = x_data[nd2 + 1];
    ytmp_data[hszCostab].re = temp_re * twid_re + temp_im * twid_im;
    ytmp_data[hszCostab].im = temp_re * twid_im - temp_im * twid_re;
  }
  if (!tst) {
    temp_re = wwc_data[(hnRows + (int)z_tmp) - 1].re;
    temp_im = wwc_data[(hnRows + (int)z_tmp) - 1].im;
    if ((int)z_tmp - 1 < 0) {
      iDelta = 0;
    } else {
      iDelta = (int)z_tmp << 1;
    }
    twid_re = x_data[iDelta];
    ytmp_data[(int)((double)ju / 2.0)].re = temp_re * twid_re + temp_im * 0.0F;
    ytmp_data[(int)((double)ju / 2.0)].im = temp_re * 0.0F - temp_im * twid_re;
    if ((int)z_tmp + 2 <= hnRows) {
      u1 = (int)z_tmp + 2;
      for (i = u1; i <= hnRows; i++) {
        ytmp_data[i - 1].re = 0.0F;
        ytmp_data[i - 1].im = 0.0F;
      }
    }
  } else if ((int)z_tmp + 1 <= hnRows) {
    u1 = (int)z_tmp + 1;
    for (i = u1; i <= hnRows; i++) {
      ytmp_data[i - 1].re = 0.0F;
      ytmp_data[i - 1].im = 0.0F;
    }
  }
  z_tmp = (double)nfft / 2.0;
  emxInit_creal32_T(&fy);
  nd2 = (int)z_tmp;
  u1 = fy->size[0];
  fy->size[0] = (int)z_tmp;
  emxEnsureCapacity_creal32_T(fy, u1);
  fy_data = fy->data;
  if ((int)z_tmp > ytmp->size[0]) {
    u1 = fy->size[0];
    fy->size[0] = (int)z_tmp;
    emxEnsureCapacity_creal32_T(fy, u1);
    fy_data = fy->data;
    for (u1 = 0; u1 < nd2; u1++) {
      fy_data[u1].re = 0.0F;
      fy_data[u1].im = 0.0F;
    }
  }
  ju = ytmp->size[0];
  u1 = (int)z_tmp;
  if (ju <= u1) {
    u1 = ju;
  }
  hszCostab = (int)z_tmp - 2;
  nRowsD2 = (int)((unsigned int)(int)z_tmp >> 1);
  k = nRowsD2 / 2;
  nd2 = 0;
  ju = 0;
  for (i = 0; i <= u1 - 2; i++) {
    fy_data[nd2] = ytmp_data[i];
    iDelta = (int)z_tmp;
    tst = true;
    while (tst) {
      iDelta >>= 1;
      ju ^= iDelta;
      tst = ((ju & iDelta) == 0);
    }
    nd2 = ju;
  }
  if (u1 - 2 < 0) {
    iDelta = 0;
  } else {
    iDelta = u1 - 1;
  }
  fy_data[nd2] = ytmp_data[iDelta];
  if ((int)z_tmp > 1) {
    for (i = 0; i <= hszCostab; i += 2) {
      temp_re_tmp = fy_data[i + 1].re;
      temp_re = fy_data[i + 1].im;
      twid_im = fy_data[i].re;
      temp_im = fy_data[i].im;
      fy_data[i + 1].re = twid_im - temp_re_tmp;
      fy_data[i + 1].im = temp_im - temp_re;
      fy_data[i].re = twid_im + temp_re_tmp;
      fy_data[i].im = temp_im + temp_re;
    }
  }
  iDelta = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      nd2 = i + iDelta;
      temp_re = fy_data[nd2].re;
      temp_im = fy_data[nd2].im;
      fy_data[nd2].re = fy_data[i].re - temp_re;
      fy_data[nd2].im = fy_data[i].im - temp_im;
      fy_data[i].re += temp_re;
      fy_data[i].im += temp_im;
    }
    nd2 = 1;
    for (hszCostab = k; hszCostab < nRowsD2; hszCostab += k) {
      twid_re = costab1q_data[hszCostab];
      twid_im = hsintab_data[hszCostab];
      i = nd2;
      ju = nd2 + iheight;
      while (i < ju) {
        u1 = i + iDelta;
        temp_re_tmp = fy_data[u1].im;
        temp_im = fy_data[u1].re;
        temp_re = twid_re * temp_im - twid_im * temp_re_tmp;
        temp_im = twid_re * temp_re_tmp + twid_im * temp_im;
        fy_data[u1].re = fy_data[i].re - temp_re;
        fy_data[u1].im = fy_data[i].im - temp_im;
        fy_data[i].re += temp_re;
        fy_data[i].im += temp_im;
        i += iDelta2;
      }
      nd2++;
    }
    k /= 2;
    iDelta = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iDelta;
  }
  emxInit_creal32_T(&fv);
  d_FFTImplementationCallback_r2b(wwc, (int)z_tmp, costab1q, hsintab, fv);
  fv_data = fv->data;
  emxFree_real32_T(&costab1q);
  emxFree_real32_T(&hsintab);
  nd2 = fy->size[0];
  for (u1 = 0; u1 < nd2; u1++) {
    twid_im = fy_data[u1].re;
    temp_im = fv_data[u1].im;
    temp_re = fy_data[u1].im;
    twid_re = fv_data[u1].re;
    fy_data[u1].re = twid_im * twid_re - temp_re * temp_im;
    fy_data[u1].im = twid_im * temp_im + temp_re * twid_re;
  }
  d_FFTImplementationCallback_r2b(fy, (int)z_tmp, hcostabinv, hsintabinv, fv);
  fv_data = fv->data;
  emxFree_creal32_T(&fy);
  emxFree_real32_T(&hsintabinv);
  emxFree_real32_T(&hcostabinv);
  if (fv->size[0] > 1) {
    temp_im = 1.0F / (float)fv->size[0];
    nd2 = fv->size[0];
    for (u1 = 0; u1 < nd2; u1++) {
      fv_data[u1].re *= temp_im;
      fv_data[u1].im *= temp_im;
    }
  }
  u1 = (int)(float)hnRows;
  nd2 = wwc->size[0];
  for (k = u1; k <= nd2; k++) {
    temp_im = wwc_data[k - 1].re;
    temp_re = fv_data[k - 1].im;
    twid_re = wwc_data[k - 1].im;
    twid_im = fv_data[k - 1].re;
    ytmp_data[k - (int)(float)hnRows].re =
        temp_im * twid_im + twid_re * temp_re;
    ytmp_data[k - (int)(float)hnRows].im =
        temp_im * temp_re - twid_re * twid_im;
  }
  emxFree_creal32_T(&fv);
  for (i = 0; i < hnRows; i++) {
    float b_ytmp_re_tmp;
    float ytmp_im_tmp;
    float ytmp_re_tmp;
    u1 = wrapIndex_data[i];
    temp_im = ytmp_data[i].re;
    temp_re = reconVar1_data[i].im;
    twid_re = ytmp_data[i].im;
    twid_im = reconVar1_data[i].re;
    temp_re_tmp = ytmp_data[u1 - 1].re;
    ytmp_im_tmp = -ytmp_data[u1 - 1].im;
    ytmp_re_tmp = reconVar2_data[i].im;
    b_ytmp_re_tmp = reconVar2_data[i].re;
    y_data[i].re =
        0.5F * ((temp_im * twid_im - twid_re * temp_re) +
                (temp_re_tmp * b_ytmp_re_tmp - ytmp_im_tmp * ytmp_re_tmp));
    y_data[i].im =
        0.5F * ((temp_im * temp_re + twid_re * twid_im) +
                (temp_re_tmp * ytmp_re_tmp + ytmp_im_tmp * b_ytmp_re_tmp));
    y_data[hnRows + i].re =
        0.5F * ((temp_im * b_ytmp_re_tmp - twid_re * ytmp_re_tmp) +
                (temp_re_tmp * twid_im - ytmp_im_tmp * temp_re));
    y_data[hnRows + i].im =
        0.5F * ((temp_im * ytmp_re_tmp + twid_re * b_ytmp_re_tmp) +
                (temp_re_tmp * temp_re + ytmp_im_tmp * twid_im));
  }
  emxFree_creal32_T(&reconVar2);
  emxFree_creal32_T(&reconVar1);
  emxFree_int32_T(&wrapIndex);
  emxFree_creal32_T(&ytmp);
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                int n2blue
 *                int nfft
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *sintabinv
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void c_FFTImplementationCallback_dob(const emxArray_real32_T *x, int n2blue,
                                     int nfft, const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *wwc;
  creal32_T *b_fv_data;
  creal32_T *fv_data;
  creal32_T *wwc_data;
  creal32_T *y_data;
  const float *x_data;
  float nt_im;
  int i;
  int k;
  int minNrowsNx;
  int nInt2m1;
  x_data = x->data;
  emxInit_creal32_T(&wwc);
  if ((nfft != 1) && ((nfft & 1) == 0)) {
    int nInt2;
    int nRows;
    int rt;
    nRows = (int)((unsigned int)nfft >> 1);
    nInt2m1 = (nRows + nRows) - 1;
    i = wwc->size[0];
    wwc->size[0] = nInt2m1;
    emxEnsureCapacity_creal32_T(wwc, i);
    wwc_data = wwc->data;
    rt = 0;
    wwc_data[nRows - 1].re = 1.0F;
    wwc_data[nRows - 1].im = 0.0F;
    nInt2 = nRows << 1;
    for (k = 0; k <= nRows - 2; k++) {
      minNrowsNx = ((k + 1) << 1) - 1;
      if (nInt2 - rt <= minNrowsNx) {
        rt += minNrowsNx - nInt2;
      } else {
        rt += minNrowsNx;
      }
      nt_im = -3.14159274F * (float)rt / (float)nRows;
      wwc_data[(nRows - k) - 2].re = cosf(nt_im);
      wwc_data[(nRows - k) - 2].im = -sinf(nt_im);
    }
    i = nInt2m1 - 1;
    for (k = i; k >= nRows; k--) {
      wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
    }
  } else {
    int nInt2;
    int rt;
    nInt2m1 = (nfft + nfft) - 1;
    i = wwc->size[0];
    wwc->size[0] = nInt2m1;
    emxEnsureCapacity_creal32_T(wwc, i);
    wwc_data = wwc->data;
    rt = 0;
    wwc_data[nfft - 1].re = 1.0F;
    wwc_data[nfft - 1].im = 0.0F;
    nInt2 = nfft << 1;
    for (k = 0; k <= nfft - 2; k++) {
      minNrowsNx = ((k + 1) << 1) - 1;
      if (nInt2 - rt <= minNrowsNx) {
        rt += minNrowsNx - nInt2;
      } else {
        rt += minNrowsNx;
      }
      nt_im = -3.14159274F * (float)rt / (float)nfft;
      i = (nfft - k) - 2;
      wwc_data[i].re = cosf(nt_im);
      wwc_data[i].im = -sinf(nt_im);
    }
    i = nInt2m1 - 1;
    for (k = i; k >= nfft; k--) {
      wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
    }
  }
  i = y->size[0];
  y->size[0] = nfft;
  emxEnsureCapacity_creal32_T(y, i);
  y_data = y->data;
  if (nfft > x->size[0]) {
    i = y->size[0];
    y->size[0] = nfft;
    emxEnsureCapacity_creal32_T(y, i);
    y_data = y->data;
    for (i = 0; i < nfft; i++) {
      y_data[i].re = 0.0F;
      y_data[i].im = 0.0F;
    }
  }
  emxInit_creal32_T(&fv);
  emxInit_creal32_T(&b_fv);
  if ((n2blue != 1) && ((nfft & 1) == 0)) {
    d_FFTImplementationCallback_doH(x, y, x->size[0], nfft, n2blue, wwc, costab,
                                    sintab, costab, sintabinv);
  } else {
    float b_re_tmp;
    float c_re_tmp;
    float re_tmp;
    minNrowsNx = x->size[0];
    if (nfft <= minNrowsNx) {
      minNrowsNx = nfft;
    }
    for (k = 0; k < minNrowsNx; k++) {
      nInt2m1 = (nfft + k) - 1;
      y_data[k].re = wwc_data[nInt2m1].re * x_data[k];
      y_data[k].im = wwc_data[nInt2m1].im * -x_data[k];
    }
    i = minNrowsNx + 1;
    for (k = i; k <= nfft; k++) {
      y_data[k - 1].re = 0.0F;
      y_data[k - 1].im = 0.0F;
    }
    d_FFTImplementationCallback_r2b(y, n2blue, costab, sintab, fv);
    fv_data = fv->data;
    d_FFTImplementationCallback_r2b(wwc, n2blue, costab, sintab, b_fv);
    i = b_fv->size[0];
    b_fv->size[0] = fv->size[0];
    emxEnsureCapacity_creal32_T(b_fv, i);
    b_fv_data = b_fv->data;
    minNrowsNx = fv->size[0];
    for (i = 0; i < minNrowsNx; i++) {
      nt_im = fv_data[i].re;
      re_tmp = b_fv_data[i].im;
      b_re_tmp = fv_data[i].im;
      c_re_tmp = b_fv_data[i].re;
      b_fv_data[i].re = nt_im * c_re_tmp - b_re_tmp * re_tmp;
      b_fv_data[i].im = nt_im * re_tmp + b_re_tmp * c_re_tmp;
    }
    d_FFTImplementationCallback_r2b(b_fv, n2blue, costab, sintabinv, fv);
    fv_data = fv->data;
    if (fv->size[0] > 1) {
      nt_im = 1.0F / (float)fv->size[0];
      minNrowsNx = fv->size[0];
      for (i = 0; i < minNrowsNx; i++) {
        fv_data[i].re *= nt_im;
        fv_data[i].im *= nt_im;
      }
    }
    i = (int)(float)nfft;
    minNrowsNx = wwc->size[0];
    for (k = i; k <= minNrowsNx; k++) {
      nt_im = wwc_data[k - 1].re;
      re_tmp = fv_data[k - 1].im;
      b_re_tmp = wwc_data[k - 1].im;
      c_re_tmp = fv_data[k - 1].re;
      nInt2m1 = k - (int)(float)nfft;
      y_data[nInt2m1].re = nt_im * c_re_tmp + b_re_tmp * re_tmp;
      y_data[nInt2m1].im = nt_im * re_tmp - b_re_tmp * c_re_tmp;
    }
  }
  emxFree_creal32_T(&b_fv);
  emxFree_creal32_T(&fv);
  emxFree_creal32_T(&wwc);
}

/*
 * Arguments    : int nRows
 *                bool useRadix2
 *                emxArray_real32_T *costab
 *                emxArray_real32_T *sintab
 *                emxArray_real32_T *sintabinv
 * Return Type  : void
 */
void c_FFTImplementationCallback_gen(int nRows, bool useRadix2,
                                     emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     emxArray_real32_T *sintabinv)
{
  emxArray_real32_T *costab1q;
  float e;
  float *costab1q_data;
  float *costab_data;
  float *sintab_data;
  float *sintabinv_data;
  int i;
  int k;
  int n;
  int nd2;
  e = 6.28318548F / (float)nRows;
  n = (int)((unsigned int)nRows >> 1) >> 1;
  emxInit_real32_T(&costab1q, 2);
  i = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = n + 1;
  emxEnsureCapacity_real32_T(costab1q, i);
  costab1q_data = costab1q->data;
  costab1q_data[0] = 1.0F;
  nd2 = n / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q_data[k + 1] = cosf(e * (float)(k + 1));
  }
  i = nd2 + 2;
  nd2 = n - 1;
  for (k = i; k <= nd2; k++) {
    costab1q_data[k] = sinf(e * (float)(n - k));
  }
  costab1q_data[n] = 0.0F;
  if (!useRadix2) {
    n = costab1q->size[1] - 1;
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
    for (k = 0; k < n; k++) {
      sintabinv_data[k + 1] = costab1q_data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      sintabinv_data[k] = costab1q_data[k - n];
    }
    for (k = 0; k < n; k++) {
      costab_data[k + 1] = costab1q_data[k + 1];
      sintab_data[k + 1] = -costab1q_data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab_data[k] = -costab1q_data[nd2 - k];
      sintab_data[k] = -costab1q_data[k - n];
    }
  } else {
    n = costab1q->size[1] - 1;
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
    for (k = 0; k < n; k++) {
      costab_data[k + 1] = costab1q_data[k + 1];
      sintab_data[k + 1] = -costab1q_data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab_data[k] = -costab1q_data[nd2 - k];
      sintab_data[k] = -costab1q_data[k - n];
    }
    sintabinv->size[0] = 1;
    sintabinv->size[1] = 0;
  }
  emxFree_real32_T(&costab1q);
}

/*
 * Arguments    : int nfft
 *                bool useRadix2
 *                int *nRows
 * Return Type  : int
 */
int c_FFTImplementationCallback_get(int nfft, bool useRadix2, int *nRows)
{
  int n2blue;
  n2blue = 1;
  if (useRadix2) {
    *nRows = nfft;
  } else {
    if (nfft > 0) {
      int n;
      int pmax;
      n = (nfft + nfft) - 1;
      pmax = 31;
      if (n <= 1) {
        pmax = 0;
      } else {
        int pmin;
        bool exitg1;
        pmin = 0;
        exitg1 = false;
        while ((!exitg1) && (pmax - pmin > 1)) {
          int k;
          int pow2p;
          k = (pmin + pmax) >> 1;
          pow2p = 1 << k;
          if (pow2p == n) {
            pmax = k;
            exitg1 = true;
          } else if (pow2p > n) {
            pmax = k;
          } else {
            pmin = k;
          }
        }
      }
      n2blue = 1 << pmax;
    }
    *nRows = n2blue;
  }
  return n2blue;
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                int n1_unsigned
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void c_FFTImplementationCallback_r2b(const emxArray_real32_T *x,
                                     int n1_unsigned,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y)
{
  creal32_T *y_data;
  const float *x_data;
  int i;
  x_data = x->data;
  i = y->size[0];
  y->size[0] = n1_unsigned;
  emxEnsureCapacity_creal32_T(y, i);
  y_data = y->data;
  if (n1_unsigned > x->size[0]) {
    i = y->size[0];
    y->size[0] = n1_unsigned;
    emxEnsureCapacity_creal32_T(y, i);
    y_data = y->data;
    for (i = 0; i < n1_unsigned; i++) {
      y_data[i].re = 0.0F;
      y_data[i].im = 0.0F;
    }
  }
  if (n1_unsigned != 1) {
    c_FFTImplementationCallback_doH(x, y, n1_unsigned, costab, sintab);
  } else {
    y_data[0].re = x_data[0];
    y_data[0].im = 0.0F;
  }
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                int unsigned_nRows
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void d_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y)
{
  const creal32_T *x_data;
  creal32_T *y_data;
  const float *costab_data;
  const float *sintab_data;
  float temp_im;
  float temp_re;
  float temp_re_tmp;
  float twid_re;
  int i;
  int iDelta;
  int iDelta2;
  int iheight;
  int ihi;
  int iy;
  int j;
  int ju;
  int k;
  int nRowsD2;
  sintab_data = sintab->data;
  costab_data = costab->data;
  x_data = x->data;
  iy = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity_creal32_T(y, iy);
  y_data = y->data;
  if (unsigned_nRows > x->size[0]) {
    iy = y->size[0];
    y->size[0] = unsigned_nRows;
    emxEnsureCapacity_creal32_T(y, iy);
    y_data = y->data;
    for (iy = 0; iy < unsigned_nRows; iy++) {
      y_data[iy].re = 0.0F;
      y_data[iy].im = 0.0F;
    }
  }
  j = x->size[0];
  if (j > unsigned_nRows) {
    j = unsigned_nRows;
  }
  ihi = unsigned_nRows - 2;
  nRowsD2 = (int)((unsigned int)unsigned_nRows >> 1);
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (i = 0; i <= j - 2; i++) {
    bool tst;
    y_data[iy] = x_data[i];
    iy = unsigned_nRows;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju;
  }
  if (j - 2 < 0) {
    j = 0;
  } else {
    j--;
  }
  y_data[iy] = x_data[j];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= ihi; i += 2) {
      temp_re_tmp = y_data[i + 1].re;
      temp_im = y_data[i + 1].im;
      temp_re = y_data[i].re;
      twid_re = y_data[i].im;
      y_data[i + 1].re = temp_re - temp_re_tmp;
      y_data[i + 1].im = twid_re - temp_im;
      y_data[i].re = temp_re + temp_re_tmp;
      y_data[i].im = twid_re + temp_im;
    }
  }
  iDelta = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      iy = i + iDelta;
      temp_re = y_data[iy].re;
      temp_im = y_data[iy].im;
      y_data[iy].re = y_data[i].re - temp_re;
      y_data[iy].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }
    iy = 1;
    for (j = k; j < nRowsD2; j += k) {
      float twid_im;
      twid_re = costab_data[j];
      twid_im = sintab_data[j];
      i = iy;
      ihi = iy + iheight;
      while (i < ihi) {
        ju = i + iDelta;
        temp_re_tmp = y_data[ju].im;
        temp_im = y_data[ju].re;
        temp_re = twid_re * temp_im - twid_im * temp_re_tmp;
        temp_im = twid_re * temp_re_tmp + twid_im * temp_im;
        y_data[ju].re = y_data[i].re - temp_re;
        y_data[ju].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }
      iy++;
    }
    k /= 2;
    iDelta = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iDelta;
  }
}

/*
 * File trailer for FFTImplementationCallback.c
 *
 * [EOF]
 */
