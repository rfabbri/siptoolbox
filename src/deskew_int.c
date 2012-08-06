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
#include <sip_lept.h>

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

static int check_args(char *fname, int opt_pos);

/*----------------------------------------------------------
 * int_deskew:
 *     interface for deskew function.
 *     should provide    [RGB1]=deskew(RGB)
 *                [Index1,Map1]=deskew(Index,Map) at Scilab level
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
         m2, n2,   /* for index output argument    */
         minlhs=1, maxlhs=2, minrhs=1, maxrhs=2, let,
         rindex, cindex, nopt, iopos;
   double *l2;
   static rhs_opts opts[]= {
         {-1,"depth","d",0,0,0},
         {-1,"quality","d",0,0,0},
		   {-1,NULL,NULL,0,0,0}
   };
   unsigned nv;
   bool stat;

   /* Other variables */
   unsigned long  imgsize;
   short int argtype;

   /* Leptonica variable */
   char        *fileout;
   l_int32      ret;
   l_float32    deg2rad;
   l_float32    angle, conf, score;
   PIX         *pix, *pixs, *pixd, *pixmn;
   l_int32      pix_depth,pix_quality;
   fileout="/tmp/help.png";

   /* ImageMagick variables */
   ExceptionInfo  exception,exception1;
   Image          *image,*image1;
   ImageInfo      *image_info,*image_info1;

   /* -- Deal with the arguments -- */
   nopt = NumOpt();
   CheckRhs(minrhs,maxrhs + nopt);
   CheckLhs(minlhs,maxlhs);

   if ( get_optionals(fname,opts) == 0) return false;
   if ( (argtype = check_args(fname, nopt)) == false) return false;

   /* default values if optional arguments are not given */
   iopos=Rhs ;
   if ( opts[0].position  == -1 ) {
      iopos++; opts[0].position = iopos;
      opts[0].m = opts[0].n = 1; opts[0].type = "d";
      CreateVar(opts[0].position,opts[0].type,&opts[0].m,&opts[0].n,&opts[0].l);
      *stk(opts[0].l) = QuantumDepth;
   } else if ( *stk(opts[0].l) != 8) {
      if  (*stk(opts[0].l) == 16 && QuantumDepth < 16)
         sip_error("depth cannot be 16 in your current setup")
      else if (*stk(opts[0].l) != 16)
         sip_error("depth must be 8 or 16")
   }
   if ( opts[1].position  == -1 ) {
      iopos++; opts[1].position = iopos;
      opts[1].m = opts[1].n = 1; opts[1].type = "d";
      CreateVar(opts[1].position,opts[1].type,&opts[1].m,&opts[1].n,&opts[1].l);
      *stk(opts[1].l)=75.0; /* Default quality/compression for jpeg/png/miff */
   } else if ( *stk(opts[1].l) < 0 || *stk(opts[1].l) > 100)
      sip_error("quality must be in range 0-100")
       /* -- Pass scilab structures to IM -- */

   InitializeMagick(NULL);
   GetExceptionInfo(&exception1);
   image_info1=CloneImageInfo((ImageInfo *) NULL);

   image_info1->colorspace = RGBColorspace; // @@@ maybe to take this off
   image_info1->monochrome = 0;
   image_info1->dither = 0;  // Imagemagick sets this as true by default.
                            // But this changes binary images too much.
   image_info1->depth= (unsigned long) *stk(opts[0].l);
   image_info1->quality= (unsigned long) *stk(opts[1].l);
   image1=AllocateImage(image_info1);
   pix_depth = (unsigned long) *stk(opts[0].l);
   pix_quality = (unsigned long) *stk(opts[1].l);
   nv = 1;
   let=0;
   switch (argtype) {
      case ARG_2D:
            GetRhsVar(nv++, "d", &m1, &n1, &l1);
            pixmn = sci_2D_double_matrix_to_pix(fname, l1, m1, n1);
            let = 1;
            break;

      case ARG_3D:
            pixmn = sci_3D_double_hypermat_to_pix(fname,nv++);
            let = 2;
            break;

      case ARG_INDEX_MAP:
            pixmn = sci_index_map_to_pix(fname, nv);
            nv+=2;
            let = 3;
            break;
      default:
            return false;
   }

   /*Deskew Function in SIP*/
   pixs=NULL;
   pixd = NULL;
   if ((pixs = pixCopy(pixs,pixmn)) == NULL)
      return sciprint("pixs not made");

   rindex=pixGetHeight(pixs);
   cindex=pixGetWidth(pixs);

   pix = pixConvertTo1(pixs, 130);
   pixSipFindSkew(pix, &angle, &conf);

   sciprint("     pixFindSkew()\n\r");
   sciprint("           conf: %5.3f\n\r", conf);
   sciprint("          angle: %7.3f degrees\n\r\n", angle);

   pixSipFindSkewSweepAndSearchScorePivot(pix, &angle, &conf, &score, SWEEP_REDUCTION2, SEARCH_REDUCTION,
                                        0.0, SWEEP_RANGE2, SWEEP_DELTA2,
                                        SEARCH_MIN_DELTA,
                                        L_SHEAR_ABOUT_CORNER);

   sciprint("    pixFind...Pivot(about corner):\n\r");
   sciprint("           conf: %5.3f\n\r", conf);
   sciprint("          angle: %7.3f degrees\n\r", angle);
   sciprint("          score: %f\n\r\n", score);

   pixSipFindSkewSweepAndSearchScorePivot(pix, &angle, &conf, &score,
                                        SWEEP_REDUCTION2, SEARCH_REDUCTION,
                                        0.0, SWEEP_RANGE2, SWEEP_DELTA2,
                                        SEARCH_MIN_DELTA,
                                        L_SHEAR_ABOUT_CENTER);

   sciprint("    pixFind...Pivot(about center)\n\r");
   sciprint("           conf: %5.3f\n\r", conf);
   sciprint("          angle: %7.3f degrees\n\r", angle);
   sciprint("          score: %f\n\r\n", score);

   /* Use top-level */
   pixd = pixSipDeskew(pixs, 0);
  #if 0
   /* Do it piecemeal; fails if outside the range */
   if (pixGetDepth(pixs) == 1) {
        pixd = pixSipDeskew(pix, DESKEW_REDUCTION);
   }
   else {
        ret = pixSipFindSkewSweepAndSearch(pix, &angle, &conf, SWEEP_REDUCTION2,
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
            pixDestroy(&pixd);
        }
   }
   #endif
   pixWrite(fileout, pixd, IFF_PNG);

   /* Initialize the image info structure and read an image.  */
   InitializeMagick(NULL);
   GetExceptionInfo(&exception);
   image_info=CloneImageInfo((ImageInfo *) NULL);
   (void) strncpy(image_info->filename,fileout,MaxTextExtent);

   image=ReadImage(image_info,&exception);

   m2 = (unsigned)pixGetHeight(pixd);
   n2 = (unsigned)pixGetWidth(pixd);
   imgsize = m2 * n2;

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
		 stat = pix_binary_image_to_double_array(fname,pixd,&l2, m2, n2);
         if (!stat) return false;
         CreateVarFromPtr(2, "d",&m2,&n2,&l2);
         free(l2);
      } else {
         stat= pix_truecolor_image_to_double_hypermat(fname,pixd,&Img,m2,n2);
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

int
check_args(char *fname, int nopts)
{
   int nargs;

   nargs = Rhs - nopts;
  /* nargs ==1 */
   if (nargs == 1) {
      switch (VarType(1)) {
         case USUALMATRIX: return ARG_2D;
         case HYPERMATRIX: return ARG_3D;
         default:
            sip_error("argument 1 must be matrix or hypermatrix")
      }
   }
   /* nargs == 2 */
   if((VarType(1) != USUALMATRIX) || (VarType(2) != USUALMATRIX))
      sip_error("the first two arguments must be arrays")

   return ARG_INDEX_MAP;
}
