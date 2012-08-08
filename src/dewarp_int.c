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
         name_rows, name_columns, name,
         nopt, iopos;
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
   char str1[1000];
   short int argtype;

   /* leptonica variables */
   char        *filein, *fileout;
   l_int32    j, n, ignore;
   l_float32  a, b, c, d, e;
   L_DEWARP  *dew;
   FILE      *fp;
   FPIX      *fpix;
   NUMA      *nax, *nay, *nafit;
   PIX       *pixs, *pixn, *pixg, *pixb, *pixt1, *pixt2, *pixt3, *pixdw;
   PIX       *pixs2, *pixn2, *pixg2, *pixb2, *pixv, *pixd;
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
            pixmn = sci_2D_double_matrix_to_pix(fname, l1, m1, n1);
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

   if((pixs = pixCopy(pixs,pxmn))==NULL){
	   sciprint("pixs not made\r\n");
	   return false;
	   }

   /* Normalize for varying background and binarize */
   pixn = pixBackgroundNormSimple(pixs, NULL, NULL);
   pixg = pixConvertRGBToGray(pixn, 0.5, 0.3, 0.2);
   pixb = pixThresholdToBinary(pixg, 130);

   /* Run the basic functions */
   if((dew = dewarpCreate(pixb, 7, 30, 15, 1))==NULL){
	    sciprint("Unable to create dewarp\r\n");
		return false;
	}

   stat2 =  dewarpBuildModel(dew, 1);
   if(stat2!=0){
	    sciprint("pixs not made\r\n");
		return false;
	}

   stat1=dewarpApplyDisparity(dew, pixg, 1);
	if(stat1!=0){
	    sciprint("pixs not made\r\n");
		return false;
	}

   /* Save the intermediate dewarped images */
   pixv = pixRead("/tmp/pixv.png");
   pixd = pixRead("/tmp/pixd.png");

   /* Normalize another image, that doesn't have enough textlines
         * to build an accurate model */
   pixs2 = pixRead(filein);
   pixn2 = pixBackgroundNormSimple(pixs2, NULL, NULL);
   pixg2 = pixConvertRGBToGray(pixn2, 0.5, 0.3, 0.2);
   pixb2 = pixThresholdToBinary(pixg2, 130);

   /* Apply the previous disparity model to this image */
   dewarpApplyDisparity(dew, pixg2, 1);
   dewarpDestroy(&dew);

   /* Get the textline centers */
   ptaa1 = pixGetTextlineCenters(pixb, 0);
   pixt1 = pixCreateTemplate(pixs);
   pixt2 = pixDisplayPtaa(pixt1, ptaa1);
   pixWrite("/tmp/textline1.png", pixt2, IFF_PNG);
   pixDisplayWithTitle(pixt2, 500, 100, "textline centers", 1);
   pixDestroy(&pixt1);

   /* Remove short lines */
   sciprint("Num all lines = %d\n", ptaaGetCount(ptaa1));
   ptaa2 = ptaaRemoveShortLines(pixb, ptaa1, 0.8, 0);

   /* Fit to curve */
   n = ptaaGetCount(ptaa2);
   sciprint("Num long lines = %d\n", n);
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

   pixDisplayWithTitle(pixt2, 700, 100, "fitted lines superimposed", 1);
   pixWrite("/tmp/textline2.png", pixt2, IFF_PNG);
   ptaaDestroy(&ptaa1);
   ptaaDestroy(&ptaa2);
   pixDestroy(&pixt2);

   /* Write out the files to be imaged */
   lept_mkdir("junkdir");
   pixWrite("/tmp/junkdir/001.jpg", pixs, IFF_JFIF_JPEG);
   pixWrite("/tmp/junkdir/002.jpg", pixn, IFF_JFIF_JPEG);
   pixWrite("/tmp/junkdir/003.jpg", pixg, IFF_JFIF_JPEG);
   pixWrite("/tmp/junkdir/004.png", pixb, IFF_TIFF_G4);
   pixt1 = pixRead("/tmp/textline1.png");
   pixWrite("/tmp/junkdir/005.png", pixt1, IFF_PNG);
   pixDestroy(&pixt1);
   pixt1 = pixRead("/tmp/textline2.png");
   pixWrite("/tmp/junkdir/006.png", pixt1, IFF_PNG);
   pixDestroy(&pixt1);
   pixt1 = pixRead("/tmp/lines1.png");
   pixWrite("/tmp/junkdir/007.png", pixt1, IFF_PNG);
   pixDestroy(&pixt1);
   pixt1 = pixRead("/tmp/lines2.png");
   pixWrite("/tmp/junkdir/008.png", pixt1, IFF_PNG);
   pixDestroy(&pixt1);
   pixt1 = pixRead("/tmp/vert-contours.png");
   pixWrite("/tmp/junkdir/009.png", pixt1, IFF_PNG);
   pixDestroy(&pixt1);
   pixWrite("/tmp/junkdir/010.png", pixv, IFF_PNG);
   pixt1 = pixThresholdToBinary(pixv, 130);
   pixWrite("/tmp/junkdir/011.png", pixt1, IFF_PNG);
   pixDestroy(&pixt1);
   pixt1 = pixRead("/tmp/horiz-contours.png");
   pixWrite("/tmp/junkdir/012.png", pixt1, IFF_PNG);
   pixDestroy(&pixt1);
   pixWrite("/tmp/junkdir/013.png", pixd, IFF_PNG);

   pixt1 = pixThresholdToBinary(pixd, 130);
   pixWrite("/tmp/junkdir/014.png", pixt1, IFF_PNG);
   pixDestroy(&pixt1);
   pixWrite("/tmp/junkdir/015.png", pixb, IFF_TIFF_G4);

   /* (these are for the second image) */
   pixWrite("/tmp/junkdir/016.jpg", pixs2, IFF_JFIF_JPEG);
   pixWrite("/tmp/junkdir/017.png", pixb2, IFF_TIFF_G4);
   pixt1 = pixRead("/tmp/pixv.png");
   pixt2 = pixThresholdToBinary(pixt1, 130);
   pixWrite("/tmp/junkdir/018.png", pixt2, IFF_PNG);
   pixDestroy(&pixt1);
   pixDestroy(&pixt2);
   pixt1 = pixRead("/tmp/pixd.png");
   pixt2 = pixThresholdToBinary(pixt1, 130);
   pixWrite("/tmp/junkdir/019.png", pixt2, IFF_PNG);
   pixDestroy(&pixt1);
   pixDestroy(&pixt2);

   pixdw = pixRead("/tmp/pixv.png");
   pixWrite(fileout, pixdw, IFF_PNG);
   pixWriteImpliedFormat(fileout, pixdw, 0, 0);

   /* Initialize the image info structure and read an image.  */
    m2 = image->rows; n2 = image->columns;

   if (sip_verbose == SIP_WORDY)
      sciprint("    Size:\t%ld rows X %ld columns\n\r", m2, n2);

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

   pixDestroy(&pixs);
   pixDestroy(&pixn);
   pixDestroy(&pixg);
   pixDestroy(&pixb);
   pixDestroy(&pixs2);
   pixDestroy(&pixn2);
   pixDestroy(&pixg2);
   pixDestroy(&pixb2);
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

