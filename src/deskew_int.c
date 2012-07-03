/*
 * -------------------------------------------------------------------------
 * SIP - Scilab Image Processing toolbox
 * Copyright (C) 2002-2012  Ricardo Fabbri
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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <math.h>
#include "sip_common.h"
#include <allheaders.h>
    /* deskew */
#define   DESKEW_REDUCTION      2      /* 1, 2 or 4 */

    /* sweep only */
#define   SWEEP_RANGE           7.    /* degrees */
#define   SWEEP_DELTA           0.2    /* degrees */
#define   SWEEP_REDUCTION       2      /* 1, 2, 4 or 8 */

    /* sweep and search */
#define   SWEEP_RANGE2          10.    /* degrees */ //comment change here
#define   SWEEP_DELTA2          1.     /* degrees */
#define   SWEEP_REDUCTION2      2      /* 1, 2, 4 or 8 */
#define   SEARCH_REDUCTION      2      /* 1, 2, 4 or 8 */
#define   SEARCH_MIN_DELTA      0.01   /* degrees */


/*----------------------------------------------------------
 * int_deskew:
 *     interface for deskew function.
 *     should provide   [RGB]=deskew(name) at Scilab level
 *
 * TO-DO
 *    - return on errors, even if exception is NULL
 *
 * $Revision: 1.2 $ $Date: 2012-07-03 21:34:48 $
 *----------------------------------------------------------*/
SipExport int
int_deskew(char *fname)
{
   /* Sip variables */
   HyperMat *Img;
   int   m1, n1,l1, /* for name input argument      */
         m2, n2,    /* for index output argument    */
         minlhs=1, maxlhs=2, minrhs=1, maxrhs=1, i;
   double *l2;

   /* Other variables */
   unsigned long  imgsize;
   char str1[1000];

   /* Leptonica variable */
   char        *filein, *fileout;
   l_int32      ret;
   l_float32    deg2rad;
   l_float32    angle, conf, score;
   PIX         *pix, *pixs, *pixd;

   /* ImageMagick variables */
   ExceptionInfo  exception;
   Image          *image;
   ImageInfo      *image_info;
   PixelPacket    *pix1;
   ImageType      imgtype;

   bool stat;

   CheckRhs(minrhs,maxrhs) ;
   CheckLhs(minlhs,maxlhs) ;

   /* Get name (#1) */
   GetRhsVar(1, "c", &m1, &n1, &l1);
   filein = cstk(l1);
   fileout = filein;
   pixd = NULL;
   if ((pixs = pixRead(filein)) == NULL)
	return sciprint("pixs not made");

   pix = pixConvertTo1(pixs, 130);
   pixWrite("/tmp/binarized.tif", pix, IFF_TIFF_G4);
   pixFindSkew(pix, &angle, &conf);

   sciprint("     pixFindSkew()\n\r");
   sciprint("           conf: %5.3f\n\r", conf);
   sciprint("          angle: %7.3f degrees\n\r\n", angle);

   pixFindSkewSweepAndSearchScorePivot(pix, &angle, &conf, &score, SWEEP_REDUCTION2, SEARCH_REDUCTION,
                                        0.0, SWEEP_RANGE2, SWEEP_DELTA2,
                                        SEARCH_MIN_DELTA,
                                        L_SHEAR_ABOUT_CORNER);

   sciprint("    pixFind...Pivot(about corner):\n\r");
   sciprint("           conf: %5.3f\n\r", conf);
   sciprint("          angle: %7.3f degrees\n\r", angle);
   sciprint("          score: %f\n\r\n", score);

   pixFindSkewSweepAndSearchScorePivot(pix, &angle, &conf, &score,
                                        SWEEP_REDUCTION2, SEARCH_REDUCTION,
                                        0.0, SWEEP_RANGE2, SWEEP_DELTA2,
                                        SEARCH_MIN_DELTA,
                                        L_SHEAR_ABOUT_CENTER);

   sciprint("    pixFind...Pivot(about center)\n\r");
   sciprint("           conf: %5.3f\n\r", conf);
   sciprint("          angle: %7.3f degrees\n\r", angle);
   sciprint("          score: %f\n\r\n", score);

   /* Use top-level */
   pixd = pixDeskew(pixs, 0);
   pixWriteImpliedFormat(fileout, pixd, 0, 0);

   #if 0
   /* Do it piecemeal; fails if outside the range */
   if (pixGetDepth(pixs) == 1) {
        pixd = pixDeskew(pix, DESKEW_REDUCTION);
        pixWrite(fileout, pixd, IFF_PNG);
   }
   else {
        ret = pixFindSkewSweepAndSearch(pix, &angle, &conf, SWEEP_REDUCTION2,
                                        SEARCH_REDUCTION, SWEEP_RANGE2,
                                        SWEEP_DELTA2, SEARCH_MIN_DELTA);
   if (ret)
            sciprint("skew angle not valid");
       else {
            sciprint("           conf: %5.3f\n\r", conf);
            sciprint("          angle: %7.3f degrees\n\r\n", angle);
            if (conf > 2.5)
                pixd = pixRotate(pixs, angle * deg2rad, L_ROTATE_AREA_MAP,
                                 L_BRING_IN_WHITE, 0, 0);
            else
            pixd = pixClone(pixs);
            pixWrite(fileout, pixd, IFF_PNG);
            pixDestroy(&pixd);
        }
   }
   #endif

   /* Initialize the image info structure and read an image.  */
   InitializeMagick(NULL);
   GetExceptionInfo(&exception);
   image_info=CloneImageInfo((ImageInfo *) NULL);
   (void) strncpy(image_info->filename,fileout,MaxTextExtent);

   image=ReadImage(image_info,&exception);

   if (image == (Image *) NULL) {
      /* clean up */
      if(exception.reason != NULL) {
         char errmsg[50];
         for (i=0; i<49; i++)
            errmsg[i]=' ';
         errmsg[49]='\0';
         strncpy(errmsg,GetLocaleExceptionMessage(exception.severity,exception.reason),50);
         DestroyImageInfo(image_info);
         DestroyExceptionInfo(&exception);
         DestroyMagick();
         sip_error(errmsg);
      }
      DestroyImageInfo(image_info);
      DestroyExceptionInfo(&exception);
      DestroyMagick();
      sip_error("unknown reason");
   }

   m2 = image->rows; n2 = image->columns;
   imgsize = m2 * n2;
   pix1=GetImagePixels(image, 0, 0, n2, m2);
   if(pix1 == (PixelPacket *) NULL)
      SIP_MAGICK_ERROR;

   switch(image->storage_class) {
   case DirectClass: {
      imgtype = GetImageType(image, &exception);
      if(imgtype == BilevelType) {
         stat = magick_binary_image_to_double_array(fname,pix1,&l2, m2, n2);
         if (!stat) return false;
         CreateVarFromPtr(2, "d",&m2,&n2,&l2);
         free(l2);
      } else {
         stat= magick_truecolor_image_to_double_hypermat(fname,pix1,&Img,m2,n2);
         if (!stat) return false;
         CreateHMat(2,Img);
         free_sci_tru_img(&Img);
      }
      m1 = n1 = 0;
      CreateVar(3,"d",&m1,&n1,&l1);
      break;
   }
   case PseudoClass:   {
      stat= magick_index_map_to_sci_dbl(fname,image,2);
      if (!stat) return false;
      break;
   }
   default:
      sip_error("unknown color class");
      break;
   }
   LhsVar(1) = 2;
   LhsVar(2) = 3;

   DestroyImageInfo(image_info);
   DestroyImage(image);
   DestroyExceptionInfo(&exception);
   DestroyMagick();
   pixDestroy(&pixs);
   pixDestroy(&pix);
   pixDestroy(&pixd);

   return true;
}
