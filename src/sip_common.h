/*
 * -------------------------------------------------------------------------
 * SIP - Scilab Image Processing toolbox
 * Copyright (C) 2002  Ricardo Fabbri
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * -------------------------------------------------------------------------
 */ 
  

#ifndef SIP_COMMON_H
#define SIP_COMMON_H 1

#ifdef HAVE_CONFIG_H
#include "sip_config.h"
#endif

#if defined(WIN32) &&  !defined(__CYGWIN__)
#	if defined (_DLL) && !defined(SIP_DLL)
#		define SIP_DLL
#  endif
#  if defined(SIP_DLL)
#     define SipExport __declspec(dllexport)
#	else
#		define SipExport
#	endif
#else
#  define SipExport
#endif

#ifndef SIP_HAVE_GetLocaleExceptionMessage
#  define GetLocaleExceptionMessage(severity,description) description
#endif

#ifndef SIP_HAVE_QueryColorDatabase_3args
#  define SIP_QueryColorDatabase(arg1,arg2,exception) QueryColorDatabase(arg1,arg2)
#else
#  define SIP_QueryColorDatabase(arg1,arg2,exception) QueryColorDatabase(arg1,arg2,exception)
#endif


#include <stack-c.h>
#include <api_scilab.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <magick/api.h>
#include <allheaders.h>
#ifdef SIP_HAVE_MagickWand
#include <wand/MagickWand.h>
#endif


#include <animal/img.h>
#include <animal/util.h>

#ifdef SIP_HAVE_OPENCV
#include <opencv2/imgproc/imgproc_c.h>

#define SIVP_FLOAT 32
#define SIVP_DOUBLE 64
#endif


// sip for scilab types
#define USUALMATRIX 1
#define STRING 10
#define HYPERMATRIX 17

#define ARG_2D 2 
#define ARG_3D 3
#define ARG_INDEX_MAP 4

#define IndexImgByRow(M, i, j) M[(j) + (i)*(image->columns)]
#define IndexImgByRowInPix(M, i, j) M[(j) + (i)*(pixcolumn)]
#define IndexImgByCol(M, i, j) M[(i) + (j)*(image->rows)]
#define IndexImgByColInPix(M, i, j) M[(i) + (j)*(pixrow)]
#define IndexImg3d(M, i, j, k) M[(i) + (j)*(image->rows) + \
                                 (k) * (image->rows)*(image->columns)]
#define IndexImg3dInPix(M, i, j, k) M[(i) + (j)*(pixrow) + \
                                 (k) * (pixrow)*(pixcolumn)]

/* (Row, Col) indexing of 1D arrays in Pix*/
#define RCbyRInPix(a,i,j,pixcolumn) a[(j) + (i)*pixcolumn]
#define RCbyCInPix(a,i,j,pixrow) a[(i) + (j)*pixrow]
#define RC3DbyCInPix(a,i,j,k,pixrow,pixcolumn) a[(i) + (j)*(pixrow) + \
                                         (k) * (pixrow)*(pixcolumn)]
                                 
// SIP error utility macros
// - It is useful to print the fname so the source of error is
//   easier identifyed inside a big script
// - Scierror does not exit the function!!

#define sip_error(str) {Scierror(999,"%s: %s.\n\r",fname, (str)); fprintf(stderr,"%s: %s.\n",fname, (str)); return false;}
#define sip_warning(str) sciprint("%s (warning): %s.\n\r",fname, (str));

/* It would be best if an error handler would be set and then
 * CatchImageException and CatchException would be used.*/
#define SIP_MAGICK_ERROR do {                                             \
   fprintf(stderr,"%s: ",fname); \
   Scierror(999,""); \
   sciprint("%s: ",fname); \
   if (image != (Image *) NULL) { \
     GetImageException(image,&exception);                                  \
   } \
   if( (exception.reason != NULL)  ) {\
      fprintf(stderr,"%s ", \
         GetLocaleExceptionMessage(exception.severity,exception.reason)); \
      sciprint("%s ", \
         GetLocaleExceptionMessage(exception.severity,exception.reason)); \
      if (exception.description != NULL) { \
         fprintf(stderr,": %s", \
         GetLocaleExceptionMessage(exception.severity,exception.description));\
         sciprint(": %s", \
         GetLocaleExceptionMessage(exception.severity,exception.description));\
         fprintf(stderr,".\n"); \
         sciprint(".\n\r"); \
      } \
   } else { \
      fprintf(stderr,"unknown reason\n"); \
      sciprint("unknown reason.\n\r"); \
   } \
} while (0)

/* Hypermatrix I/O */                                 
#define GetRhsHMat(pos,H) if (!sip_get_rhs_hmat(pos,H)) { return false;}
#define CreateHMat(pos,H) if (!sip_cre_hmat(pos,H)) { return false;}

/*
 *  Data Types
 */

typedef struct HyperMat {
  SciIntMat sc;   /* coding informations */
  int    it,      /* it=1 for complex; it=0 for real */
         size;    /* size of data array              */
  double *R,      /* real part                       */
         *I;      /* imaginary part (NULL for real hypermatrices) */
} HyperMat;

/* verbose levels */
typedef enum {
   SIP_QUIET,
   SIP_WORDY,
} sip_verbose_level;

#define SIP_DEFAULT_VERBOSE_LEVEL SIP_WORDY

/* global variables */
extern sip_verbose_level sip_verbose;


/* 
 *  Useful functions
 */

/* ???'static' keyword should be used because these functions are
seen only in SIP and wont conflict with function names available
to Scilab when the toolbox is loaded */

void
   free_sci_tru_img(HyperMat **hh);

HyperMat
   *new_sci_tru_img(unsigned rows, unsigned cols);

bool 
/* Scilab <--> Imagemagick I/O */
   sci_2D_double_matrix_to_magick(char *fname, int p, int r, int c, 
         Image *image,PixelPacket **ppix), 
   sci_3D_double_hypermat_to_magick(char *fname, int nv, Image *image, PixelPacket **ppix),
   sci_index_map_to_magick(char *fname, int nv, Image *image, 
         PixelPacket **ppix),
   magick_grayscale_image_to_double_array(char *fname, PixelPacket *pix, 
         double **dbl_array, int rows, int cols),
   magick_truecolor_image_to_double_hypermat(char *fname,PixelPacket *pix,
         HyperMat **H, int rows, int cols),
   magick_binary_image_to_double_array(char *fname, PixelPacket *pix, 
         double **dbl_array, int rows, int cols),
   magick_index_map_to_sci_dbl(char *fname, Image *image, int nv),

/* Scilab <--> C hypermatrix I/O */
   sip_get_rhs_hmat(int pos, HyperMat *H),
   sip_get_rhs_tru_img(int pos, HyperMat *h, char *fname),
   sip_cre_hmat(int pos, HyperMat *H),

/* Scilab <--> Animal I/O */
   animal_grayscale_image_to_double_array(char *fname, Img *img, double **ptr),
   animal_grayscale_imgpuint32_to_double_array(char *fname, ImgPUInt32 *img, double **ptr),

/* PIX <--> Scilab */
   pix_truecolor_image_to_double_hypermat(char *fname, PIX *pixme, HyperMat **H, int rows, int cols),
   pix_binary_image_to_double_array(char *fname, PIX *pixme, double **dbl_array, int rows, int cols),
   pix_index_map_to_sci_dbl(char *fname, PIX *pixme, int nv),
   pix_gray_image_to_double_array(char *fname, PIX *pixme, double **dbl_array, int rows, int cols);

PIX
/* Scilab <--> PIX */
   *sci_2D_double_matrix_to_pix(char *fname, int p, int r, int c),
   *sci_3D_double_hypermat_to_pix(char *fname, int nv),
   *sci_index_map_to_pix(char *fname, int nv),
   *sci_2D_gray_double_matrix_to_pix(char *fname, int p, int r, int c);

#define sci_2D_double_matrix_to_animal(ptr,r,c,img,pixtype,maxval) { \
   for (i=0; i<(r)*(c); ++i) \
      DATA(img)[i] = PROUND(pixtype,*stk(ptr+i) * maxval); \
}

/* Scilab <--> OpenCV I/O */
#ifdef SIP_HAVE_OPENCV
  IplImage * Mat2IplImg(int nPos);
  int IplImg2Mat(IplImage * pImage, int nPos);
  int Create2DIntMat(int nPos, int nRow, int nCol, void * pData, int nType);
  int Create2DFloatMat(int nPos, int nRow, int nCol, float * pData);
  int Create2DDoubleMat(int nPos, int nRow, int nCol, double * pData);
  int Create3DIntMat(int nPos, int nRow, int nCol, int nCh, void * pData, int nType);
  int Create3DFloatMat(int nPos, int nRow, int nCol, int nCh, float* pData);
  int Create3DDoubleMat(int nPos, int nRow, int nCol, int nCh, double* pData);

  IplImage * CreateIplImgFromHm(int nPos);

  int MatData2ImgData(IplImage * pImage, void * pMatData);

  int IplType2SciType(int IplType);
  int SciType2IplType(int SciType);

  int MatData2ImgData(IplImage * pImage, void * pMatData);
  int ImgData2MatData(IplImage * pImage, void * pMatData);


#endif


#endif
