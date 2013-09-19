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
#include <stdlib.h>
#include "sip_common.h"
#include <allheaders.h>

    /* for pixGenerateSelWithRuns() */
static const l_int32  NumHorLines = 11;
static const l_int32  NumVertLines = 8;
static const l_int32  MinRunlength = 1;

    /* for pixDisplayHitMissSel() */
static const l_uint32  HitColor = 0xff880000;
static const l_uint32  MissColor = 0x00ff8800;

//--------------------------hmt--------------------

static int check_args(char *fname, int opt_pos);

/*----------------------------------------------------------
 * int_hmt:
 *     interface for deskew function.
 *     should provide
 *                [img]=pixhmt(img1,img2) at Scilab level
 * TO-DO
 *    - return on errors, even if exception is NULL
 *
 * $Revision: 1.2 $ $Date: 2012-08-31 21:34:48 $
 *----------------------------------------------------------*/
SipExport int
int_hmt(char *fname)
{
   /* Sip variables */
   HyperMat *Img;
   int   m1, n1,l1, /* for name input argument      */
         m2, n2,   /* for index output argument    */
         minlhs=1, maxlhs=2, minrhs=1, maxrhs=2, let,
         rindex, cindex, nopt, iopos, stat1, stat2, stat3,l3,m3,n3;
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
   PIX         *pix, *pixs, *pixd, *pixmn, *pixmn1;
   l_int32      pix_depth,pix_quality;
//-------------------------------------------------------
char        *filein, *fileout, *patternfile;
l_int32      w, h, i, n;
BOX         *box, *boxe;
BOXA        *boxa1, *boxa2;
PIX         *pixp, *pixpe;
PIX         *pixt1, *pixt2, *pixhmt;
SEL         *sel_2h, *sel;
//------------------------------------------------------
   /* -- Deal with the arguments -- */
   nopt = NumOpt();
   CheckRhs(minrhs,maxrhs + nopt);
   CheckLhs(minlhs,maxlhs);

   if ( get_optionals(fname,opts) == 0) return false;
   if ( (argtype = check_args(fname, nopt)) == false) return false;

   /* default values if optional arguments are not given */
   iopos=Rhs ;

   pix_depth = (unsigned long) *stk(opts[0].l);
   pix_quality = (unsigned long) *stk(opts[1].l);
   nv = 1;
   let=0;
   switch (argtype) {
      case ARG_2D:
            GetRhsVar(nv++, "d", &m1, &n1, &l1);
            pixmn = sci_2D_gray_double_matrix_to_pix(fname, l1, m1, n1);
            pixWrite("/tmp/junkdebug.png", pixmn, IFF_PNG);

            GetRhsVar(nv++, "d", &m3, &n3, &l3);
            pixmn1 = sci_2D_gray_double_matrix_to_pix(fname, l3, m3, n3);
            pixWrite("/tmp/junkdebug.png", pixmn1, IFF_PNG);

            let = 1;
            break;

      case ARG_3D:
            pixmn = sci_3D_double_hypermat_to_pix(fname,nv++);
            let = 2;
            break;

      case ARG_INDEX_MAP:
            GetRhsVar(nv++, "d", &m1, &n1, &l1);
            pixmn = sci_2D_gray_double_matrix_to_pix(fname, l1, m1, n1);
            pixWrite("/tmp/junkdebug1.png", pixmn, IFF_PNG);

            GetRhsVar(nv++, "d", &m3, &n3, &l3);
            pixmn1 = sci_2D_gray_double_matrix_to_pix(fname, l3, m3, n3);
            pixWrite("/tmp/junkdebug2.png", pixmn1, IFF_PNG);

            let = 3;
            break;
      default:
            sip_error("Unknown argument type");
            return false;
   }

   /*Deskew Function in SIP*/
   pixs = NULL;
   pixd = NULL;
   if ((pixs = pixCopy(pixs,pixmn)) == NULL){
      sip_error("Unable to create pixs");
      return false;
   }

   if ((pixd = pixCopy(pixd,pixmn1)) == NULL){
      sip_error("Unable to create pixs");
      return false;
   }
    w = pixGetWidth(pixd);
    h = pixGetHeight(pixd);
    pixs = pixConvertTo1(pixs, 130);
    pixd = pixConvertTo1(pixd, 130);
        /* generate the hit-miss Sel with runs */
    sel = pixGenerateSelWithRuns(pixd, NumHorLines, NumVertLines, 0,
                                MinRunlength, 7, 7, 0, 0, &pixpe);

        /* display the Sel two ways */
    selWriteStream(stderr, sel);
    pixt1 = pixDisplayHitMissSel(pixpe, sel, 9, HitColor, MissColor);
    pixDisplay(pixt1, 200, 200);
    pixWrite("/tmp/junkpixt", pixt1, IFF_PNG);

        /* use the Sel to find all instances in the page */
    startTimer();
    pixhmt = pixHMT(NULL, pixs, sel);
    pixWrite("/tmp/temp.png", pixhmt, IFF_PNG);
    fprintf(stderr, "Time to find patterns = %7.3f\n", stopTimer());
   //-------------------------------------------------------------
       /* small erosion to remove noise; typically not necessary if
	 * there are enough elements in the Sel */
    sel_2h = selCreateBrick(1, 2, 0, 0, SEL_HIT);
    pixt2 = pixErode(NULL, pixhmt, sel_2h);

        /* display the result visually by placing the Sel at each
	 * location found */
    pixd = pixDilate(NULL, pixt2, sel);
    pixWrite(fileout, pixd, IFF_TIFF_G4);

        /* display outut with an outline around each located pattern */
    boxa1 = pixConnCompBB(pixt2, 8);
    n = boxaGetCount(boxa1);
    boxa2 = boxaCreate(n);
    for (i = 0; i < n; i++) {
        box = boxaGetBox(boxa1, i, L_COPY);
        boxe = boxCreate(box->x - w / 2, box->y - h / 2, w + 4, h + 4);
        boxaAddBox(boxa2, boxe, L_INSERT);
        pixRenderBox(pixs, boxe, 4, L_FLIP_PIXELS);
        boxDestroy(&box);
    }
    pixWrite("/tmp/junkoutline", pixs, IFF_TIFF_G4);
    boxaWriteStream(stderr, boxa2);
   //-----------------------------------------------------------

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
   pixDestroy(&pixpe);
   pixDestroy(&pixhmt);
   selDestroy(&sel);
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
         case USUALMATRIX:
			sip_error("there should be two arguments and must be arrays");
         case HYPERMATRIX:
			sip_error("there should be two arguments and must be arrays");
         default:
            sip_error("there should be two arguments and must be arrays");
      }
   }
   /* nargs == 2 */
   if((VarType(1) == USUALMATRIX) || (VarType(2) == USUALMATRIX))
		return ARG_INDEX_MAP;
   else{
      sip_error("the first two arguments must be arrays");
	}
}
