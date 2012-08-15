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
   l_int32      ret;
   l_float32    deg2rad;
   l_float32    angle, conf, score;
   PIX         *pix, *pixs, *pixd, *pixmn;
   l_int32      pix_depth,pix_quality;

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

   pix_depth = (unsigned long) *stk(opts[0].l);
   pix_quality = (unsigned long) *stk(opts[1].l);
   nv = 1;
   let=0;
   switch (argtype) {
      case ARG_2D:
            GetRhsVar(nv++, "d", &m1, &n1, &l1);
            pixmn = sci_2D_gray_double_matrix_to_pix(fname, l1, m1, n1);
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
            sip_error("Unknown argument type");
            return false;
   }

   /*Deskew Function in SIP*/
   pixs=NULL;
   pixd = NULL;
   if ((pixs = pixCopy(pixs,pixmn)) == NULL){
      sip_error("Unable to create pixs");
      return false;
   }

   rindex=pixGetHeight(pixs);
   cindex=pixGetWidth(pixs);

   /*binarizes pixs to pix*/
   pix = pixConvertTo1(pixs, 130);

   /*Finds the skew angle for pix*/
   pixFindSkew(pix, &angle, &conf);

   /*Finds skew sweep and Score pivot about corner*/
   pixFindSkewSweepAndSearchScorePivot(pix, &angle, &conf, &score, SWEEP_REDUCTION2, SEARCH_REDUCTION,
                                        0.0, SWEEP_RANGE2, SWEEP_DELTA2,
                                        SEARCH_MIN_DELTA,
                                        L_SHEAR_ABOUT_CORNER);

   /*Finds skew sweep and Score pivot about center*/
   pixFindSkewSweepAndSearchScorePivot(pix, &angle, &conf, &score,
                                        SWEEP_REDUCTION2, SEARCH_REDUCTION,
                                        0.0, SWEEP_RANGE2, SWEEP_DELTA2,
                                        SEARCH_MIN_DELTA,
                                        L_SHEAR_ABOUT_CENTER);

   /* Use top-level */
   pixd = pixDeskew(pixs, 0);
  #if 0
   /* Do it piecemeal; fails if outside the range */
   if (pixGetDepth(pixs) == 1) {
        pixd = pixDeskew(pix, DESKEW_REDUCTION);
   }
   else {
        ret = pixFindSkewSweepAndSearch(pix, &angle, &conf, SWEEP_REDUCTION2,
                                           SEARCH_REDUCTION, SWEEP_RANGE2,
                                           SWEEP_DELTA2, SEARCH_MIN_DELTA);
   if (ret)
            sip_error("skew angle not valid");
       else {
            if (conf > 2.5)
                pixd = pixRotate(pixs, angle * deg2rad, L_ROTATE_AREA_MAP,
                                 L_BRING_IN_WHITE, 0, 0);
            else
            pixd = pixClone(pixs);
            pixDestroy(&pixd);
        }
   }
   #endif

   m2 = (unsigned)pixGetHeight(pixd);
   n2 = (unsigned)pixGetWidth(pixd);
   imgsize = m2 * n2;

   switch(let) {
   case 1: {
	     stat = pix_gray_image_to_double_array(fname,pixd,&l2, m2, n2);
         if (!stat) return false;
         CreateVarFromPtr(2, "d",&m2,&n2,&l2);
         free(l2);
         m1 = n1 = 0;
         CreateVar(3,"d",&m1,&n1,&l1);
         break;
   }

   case 2: {
         stat= pix_truecolor_image_to_double_hypermat(fname,pixd,&Img,m2,n2);
         if (!stat) return false;
         CreateHMat(2,Img);
         free_sci_tru_img(&Img);
         m1 = n1 = 0;
         CreateVar(3,"d",&m1,&n1,&l1);
         break;
   }

   case 3: {
	  pixd = pixConvertRGBToColormap(pixd,1);
      stat= pix_index_map_to_sci_dbl(fname,pixd,2);
      if (!stat) return false;
      break;
   }

   default:
      sip_error("unknown color class");
      break;
   }

   LhsVar(1) = 2;
   LhsVar(2) = 3;

   pixDestroy(&pixmn);
   pixDestroy(&pixd);
   pixDestroy(&pixs);
   pixDestroy(&pix);
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
