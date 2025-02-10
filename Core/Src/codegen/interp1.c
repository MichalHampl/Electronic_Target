/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: interp1.c
 *
 * MATLAB Coder version            : 23.2
 * C/C++ source code generated on  : 15-Nov-2024 13:26:43
 */

/* Include Files */
#include "interp1.h"
#include "envelope_xcorr_gen_emxutil.h"
#include "envelope_xcorr_gen_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *varargin_1
 *                const float varargin_2[256]
 *                const emxArray_real32_T *varargin_3
 *                emxArray_real32_T *Vq
 * Return Type  : void
 */
void interp1(const emxArray_real32_T *varargin_1, const float varargin_2[256],
             const emxArray_real32_T *varargin_3, emxArray_real32_T *Vq)
{
  emxArray_real32_T *dx;
  emxArray_real32_T *md;
  emxArray_real32_T *x;
  float y[256];
  const float *varargin_1_data;
  const float *varargin_3_data;
  float *Vq_data;
  float *dx_data;
  float *md_data;
  float *x_data;
  int high_i;
  int k;
  int nxi;
  int nxm1;
  bool has_endslopes;
  varargin_3_data = varargin_3->data;
  varargin_1_data = varargin_1->data;
  memcpy(&y[0], &varargin_2[0], 256U * sizeof(float));
  emxInit_real32_T(&x, 2);
  nxi = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = varargin_1->size[1];
  emxEnsureCapacity_real32_T(x, nxi);
  x_data = x->data;
  high_i = varargin_1->size[1];
  for (nxi = 0; nxi < high_i; nxi++) {
    x_data[nxi] = varargin_1_data[nxi];
  }
  nxi = Vq->size[0] * Vq->size[1];
  Vq->size[0] = 1;
  Vq->size[1] = varargin_3->size[1];
  emxEnsureCapacity_real32_T(Vq, nxi);
  Vq_data = Vq->data;
  high_i = varargin_3->size[1];
  for (nxi = 0; nxi < high_i; nxi++) {
    Vq_data[nxi] = 0.0F;
  }
  has_endslopes = (varargin_3->size[1] == 0);
  emxInit_real32_T(&dx, 2);
  emxInit_real32_T(&md, 2);
  if (!has_endslopes) {
    k = 0;
    int exitg1;
    do {
      exitg1 = 0;
      if (k < 256) {
        if (rtIsNaNF(varargin_1_data[k])) {
          exitg1 = 1;
        } else {
          k++;
        }
      } else {
        float pp_coefs_data[1020];
        float r;
        float xtmp;
        int pp_coefs_size_idx_1;
        int yoffset;
        if (varargin_1_data[1] < varargin_1_data[0]) {
          for (nxm1 = 0; nxm1 < 128; nxm1++) {
            xtmp = x_data[nxm1];
            x_data[nxm1] = x_data[255 - nxm1];
            x_data[255 - nxm1] = xtmp;
            xtmp = y[nxm1];
            y[nxm1] = y[255 - nxm1];
            y[255 - nxm1] = xtmp;
          }
        }
        has_endslopes = (x->size[1] + 2 == 256);
        if ((x->size[1] <= 2) || ((x->size[1] <= 3) && (!has_endslopes))) {
          if (x->size[1] <= 2) {
            pp_coefs_size_idx_1 = 2;
          } else {
            pp_coefs_size_idx_1 = 3;
          }
          if (x->size[1] <= 2) {
            pp_coefs_data[0] = (y[1] - y[0]) / (x_data[1] - x_data[0]);
            pp_coefs_data[1] = y[0];
            nxi = dx->size[0] * dx->size[1];
            dx->size[0] = 1;
            dx->size[1] = x->size[1];
            emxEnsureCapacity_real32_T(dx, nxi);
            dx_data = dx->data;
            high_i = x->size[1];
            for (nxi = 0; nxi < high_i; nxi++) {
              dx_data[nxi] = x_data[nxi];
            }
            nxi = x->size[0] * x->size[1];
            x->size[0] = 1;
            x->size[1] = dx->size[1];
            emxEnsureCapacity_real32_T(x, nxi);
            x_data = x->data;
            high_i = dx->size[1];
            for (nxi = 0; nxi < high_i; nxi++) {
              x_data[nxi] = dx_data[nxi];
            }
          } else {
            xtmp = x_data[1] - x_data[0];
            r = (y[1] - y[0]) / xtmp;
            pp_coefs_data[0] = ((y[2] - y[1]) / (x_data[2] - x_data[1]) - r) /
                               (x_data[2] - x_data[0]);
            pp_coefs_data[1] = r - pp_coefs_data[0] * xtmp;
            pp_coefs_data[2] = y[0];
            xtmp = x_data[0];
            r = x_data[2];
            nxi = x->size[0] * x->size[1];
            x->size[0] = 1;
            x->size[1] = 2;
            emxEnsureCapacity_real32_T(x, nxi);
            x_data = x->data;
            x_data[0] = xtmp;
            x_data[1] = r;
          }
        } else {
          float s_data[256];
          float dvdf_data[255];
          float d31;
          float dnnm2;
          short szs_idx_1;
          nxm1 = x->size[1] - 1;
          if (has_endslopes) {
            szs_idx_1 = 254;
            yoffset = 1;
          } else {
            szs_idx_1 = 256;
            yoffset = 0;
          }
          nxi = dx->size[0] * dx->size[1];
          dx->size[0] = 1;
          dx->size[1] = x->size[1] - 1;
          emxEnsureCapacity_real32_T(dx, nxi);
          dx_data = dx->data;
          for (k = 0; k < nxm1; k++) {
            xtmp = x_data[k + 1] - x_data[k];
            dx_data[k] = xtmp;
            high_i = yoffset + k;
            dvdf_data[k] = (y[high_i + 1] - y[high_i]) / xtmp;
          }
          for (k = 2; k <= nxm1; k++) {
            s_data[k - 1] = 3.0F * (dx_data[k - 1] * dvdf_data[k - 2] +
                                    dx_data[k - 2] * dvdf_data[k - 1]);
          }
          if (has_endslopes) {
            d31 = 0.0F;
            dnnm2 = 0.0F;
            s_data[0] = y[0] * dx_data[1];
            s_data[x->size[1] - 1] =
                dx_data[x->size[1] - 3] * y[x->size[1] + 1];
          } else {
            d31 = x_data[2] - x_data[0];
            dnnm2 = x_data[x->size[1] - 1] - x_data[x->size[1] - 3];
            s_data[0] = ((dx_data[0] + 2.0F * d31) * dx_data[1] * dvdf_data[0] +
                         dx_data[0] * dx_data[0] * dvdf_data[1]) /
                        d31;
            xtmp = dx_data[x->size[1] - 2];
            s_data[x->size[1] - 1] =
                ((xtmp + 2.0F * dnnm2) * dx_data[x->size[1] - 3] *
                     dvdf_data[x->size[1] - 2] +
                 xtmp * xtmp * dvdf_data[x->size[1] - 3]) /
                dnnm2;
          }
          nxi = md->size[0] * md->size[1];
          md->size[0] = 1;
          md->size[1] = x->size[1];
          emxEnsureCapacity_real32_T(md, nxi);
          md_data = md->data;
          md_data[0] = dx_data[1];
          xtmp = dx_data[x->size[1] - 3];
          md_data[x->size[1] - 1] = xtmp;
          for (k = 2; k <= nxm1; k++) {
            md_data[k - 1] = 2.0F * (dx_data[k - 1] + dx_data[k - 2]);
          }
          r = dx_data[1] / md_data[0];
          md_data[1] -= r * d31;
          s_data[1] -= r * s_data[0];
          for (k = 3; k <= nxm1; k++) {
            r = dx_data[k - 1] / md_data[k - 2];
            md_data[k - 1] -= r * dx_data[k - 3];
            s_data[k - 1] -= r * s_data[k - 2];
          }
          r = dnnm2 / md_data[x->size[1] - 2];
          md_data[x->size[1] - 1] -= r * xtmp;
          s_data[x->size[1] - 1] -= r * s_data[x->size[1] - 2];
          s_data[x->size[1] - 1] /= md_data[x->size[1] - 1];
          for (k = nxm1; k >= 2; k--) {
            s_data[k - 1] =
                (s_data[k - 1] - dx_data[k - 2] * s_data[k]) / md_data[k - 1];
          }
          s_data[0] = (s_data[0] - d31 * s_data[1]) / md_data[0];
          nxm1 = x->size[1];
          pp_coefs_size_idx_1 = 4;
          for (high_i = 0; high_i <= nxm1 - 2; high_i++) {
            xtmp = dvdf_data[high_i];
            r = s_data[high_i];
            d31 = dx_data[high_i];
            dnnm2 = (xtmp - r) / d31;
            xtmp = (s_data[high_i + 1] - xtmp) / d31;
            pp_coefs_data[high_i] = (xtmp - dnnm2) / d31;
            pp_coefs_data[(szs_idx_1 + high_i) - 1] = 2.0F * dnnm2 - xtmp;
            pp_coefs_data[((szs_idx_1 - 1) << 1) + high_i] = r;
            pp_coefs_data[3 * (szs_idx_1 - 1) + high_i] = y[yoffset + high_i];
          }
        }
        nxi = varargin_3->size[1];
        for (k = 0; k < nxi; k++) {
          if (rtIsNaNF(varargin_3_data[k])) {
            Vq_data[k] = rtNaNF;
          } else {
            int low_i;
            high_i = x->size[1];
            low_i = 1;
            nxm1 = 2;
            while (high_i > nxm1) {
              yoffset = (low_i >> 1) + (high_i >> 1);
              if (((low_i & 1) == 1) && ((high_i & 1) == 1)) {
                yoffset++;
              }
              if (varargin_3_data[k] >= x_data[yoffset - 1]) {
                low_i = yoffset;
                nxm1 = yoffset + 1;
              } else {
                high_i = yoffset;
              }
            }
            r = varargin_3_data[k] - x_data[low_i - 1];
            xtmp = pp_coefs_data[low_i - 1];
            for (nxm1 = 2; nxm1 <= pp_coefs_size_idx_1; nxm1++) {
              xtmp = r * xtmp +
                     pp_coefs_data[(low_i + (nxm1 - 1) * (x->size[1] - 1)) - 1];
            }
            Vq_data[k] = xtmp;
          }
        }
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  emxFree_real32_T(&md);
  emxFree_real32_T(&dx);
  emxFree_real32_T(&x);
}

/*
 * File trailer for interp1.c
 *
 * [EOF]
 */
