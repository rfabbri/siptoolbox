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
#include "allheaders.h"

#define   DO_QUAD     1
#define   DO_CUBIC    0
#define   DO_QUARTIC  0

static int check_args(char *fname, int opt_pos);

/*----------------------------------------------------------
 * int_dewarp:
 *     interface for dewarp function.
 *     should provide   [RGB]=dewarp(RGB)
 *              [Index1,Map1]=dewarp(Index,Map) at Scilab level
 *
 * TODO-Use scilab matrix to do pix operation
 * $Revision: 1.2 $ $Date: 2012-07-08 21:34:48 $
 *----------------------------------------------------------*/
SipExport int
int_dewarp(char *fname)
{
	/* Interface variables */
   HyperMat *Img;
   int   m1, n1,l1, /* for name input argument      */
         m2, n2,    /* for index output argument    */
         minlhs=1, maxlhs=2, minrhs=1, maxrhs=2, i,
         let, nopt, iopos;
   double *l2;
   static rhs_opts opts[]= {
         {-1,"depth","d",0,0,0},
         {-1,"quality","d",0,0,0},
		   {-1,NULL,NULL,0,0,0}
   };
   unsigned nv;
   bool stat;

   /* Other variables */
   short int argtype;

   /* leptonica variables */
   l_int32    j, n;
   l_float32  a, b, c;
   L_DEWARP  *dew;
   NUMA      *nax, *nafit;
   PIX       *pixs, *pixn, *pixg, *pixb, *pixt1, *pixt2, *pixt3, *pixdw;
   PIX       *pixv, *pixd, *pixmn;
   PTA       *pta, *ptad;
   PTAA      *ptaa1, *ptaa2;

   int stat2=0,stat1=0;

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

    /* -- Pass scilab structures to Leptonica -- */
   let=0;
   nv = 1;
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
            let = 3;
            break;
      default:
            return false;
   }
   pixs=NULL;

   if((pixs = pixCopy(pixs,pixmn))==NULL){
	   sip_error("pixs not made\r\n");
	   return false;
	   }

   /* Normalize for varying background and binarize */
   pixn = pixBackgroundNormSimple(pixs, NULL, NULL);
   pixg = pixConvertRGBToGray(pixn, 0.5, 0.3, 0.2);
   pixb = pixThresholdToBinary(pixg, 130);

   /* Run the basic functions */
   if((dew = dewarpCreate(pixb, 7, 30, 15, 1))==NULL){
	    sip("Unable to create dewarp\r\n");
		return false;
	}

   stat1 =  dewarpBuildModel(dew, 1);
   if(stat1!=0){
	    sip_error("Unable to build dewarp Model\r\n");
		return false;
	}

   stat2 = dewarpApplyDisparity(dew, pixg, 1);
	if(stat2!=0){
	    sip_error("Unable to apply disparity on pixs\r\n");
		return false;
	}

   /* Save the intermediate dewarped images */
   pixv = pixRead("/tmp/pixv.png");
   pixd = pixRead("/tmp/pixd.png");

   dewarpDestroy(&dew);

   /* Get the textline centers */
   ptaa1 = pixGetTextlineCenters(pixb, 0);
   pixt1 = pixCreateTemplate(pixs);
   pixt2 = pixDisplayPtaa(pixt1, ptaa1);
   pixDestroy(&pixt1);

   /* Remove short lines */
   ptaa2 = ptaaRemoveShortLines(pixb, ptaa1, 0.8, 0);

   /* Fit to curve */
   n = ptaaGetCount(ptaa2);
   for (j = 0; j < n; j++) {
       pta = ptaaGetPta(ptaa2, j, L_CLONE);
       ptaGetArrays(pta, &nax, NULL);
#if DO_QUAD
        ptaGetQuadraticLSF(pta, &a, &b, &c, &nafit);
#elif  DO_CUBIC
        ptaGetCubicLSF(pta, &a, &b, &c, &d, &nafit);
#elif DO_QUARTIC
        ptaGetQuarticLSF(pta, &a, &b, &c, &d, &e, &nafit);
#endif
        ptad = ptaCreateFromNuma(nax, nafit);
        pixDisplayPta(pixt2, pixt2, ptad);
        ptaDestroy(&pta);
        ptaDestroy(&ptad);
        numaDestroy(&nax);
        numaDestroy(&nafit);
    }

   /* Converting 8bpp to 32bpp pixs */
   pixdw=NULL;
   pixd=NULL;
   pixdw=pixConvert8To32(pixv);

   /* Initialize the image */
   if ((pixd=pixCopy(pixd,pixdw))  == NULL)
   {
	  sip_error("pixs not made");
      return false;
   }

   m2=pixGetHeight(pixd);
   n2=pixGetWidth(pixd);

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

   ptaaDestroy(&ptaa1);
   ptaaDestroy(&ptaa2);
   pixDestroy(&pixt1);
   pixDestroy(&pixt2);
   pixDestroy(&pixs);
   pixDestroy(&pixn);
   pixDestroy(&pixg);
   pixDestroy(&pixb);
   pixDestroy(&pixv);
   pixDestroy(&pixd);
   pixDestroy(&pixdw);
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
