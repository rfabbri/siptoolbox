/*
 * -------------------------------------------------------------------------
 * SIP - Scilab Image Processing toolbox
 * Copyright (C) 2002-2009  Ricardo Fabbri
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
  

#include <stack-c.h>
#include <stdio.h>
#include <string.h>
#include <animal/animal.h>
#include "sip_common.h"


/*--------------------------------------------------------------
 * [skl,dt,lbl]=skel(img [, opt, algorithm])
 *	- opt = "interior" or "full"(default)
 * - algorithm = "exact euclidean" or "fast euclidean"(default)
 *--------------------------------------------------------------*/
SipExport int 
skel_int(char *fname)
{
   /* Interface variables */ 
   int   r1, c1, p1,  // img
         r2, c2, // skl
         r3, c3, // dt
         r4, c4, // lbl
         r_opt, c_opt, l_opt, opt=INTERIOR,
			r_alg, c_alg, l_alg, alg=SKL_IFT,
         i,
         minlhs=1, maxlhs=3, minrhs=1, maxrhs=3;
   Img *im; //*aux;
   annotated_skl *skl;

   double *pt2, *pt3, *pt4;

   CheckRhs(minrhs,maxrhs);
   CheckLhs(minlhs,maxlhs);


   GetRhsVar(1, "d", &r1, &c1, &p1);  // img 
   if (Rhs >= 2) {
      GetRhsVar(2, "c", &r_opt, &c_opt, &l_opt);
      if (strcmp("interior",cstk(l_opt)) == 0)
         opt = INTERIOR;
      else if (strcmp("both",cstk(l_opt)) == 0)
         opt = BOTH;
      else if (strcmp("exterior",cstk(l_opt)) == 0)
         opt = EXTERIOR;
      else
         sip_error("invalid second argument");
		if (Rhs == 3) {
			GetRhsVar(3,"c",&r_alg,&c_alg,&l_alg);
			if (strcmp("exact euclidean",cstk(l_alg)) == 0)
				alg=SKL_COSTA_ESTROZI;
			else if (strcmp("fast euclidean",cstk(l_alg)) == 0)
				alg=SKL_IFT;
			else
				sip_error("invalid third argument");
		}
   }


   // pass transposed image to internal row-wise storage
   im=new_img(c1,r1); 

   sci_2D_double_matrix_to_animal(p1,r1,c1,im,pixval,1);
    
   im->isbinary = true;

   // regularize image
   //aux = im;
   //im = imregularize(im,NULL);
   //imfree(&aux);

   skl = msskl(im,opt,alg);
   if (!skl)
      sip_error("internal msskl routine failed");
   imfree(&im);

   /* @@@ change here for integer data */
   pt2 = (double *)calloc(r1*c1, sizeof(double));
      if (!pt2)
         sip_error("unable to alloc memory");
   pt3 = (double *)calloc(r1*c1, sizeof(double));
      if (!pt3)
         sip_error("unable to alloc memory");
   pt4 = (double *)calloc(r1*c1, sizeof(double));
      if (!pt4)
          sip_error("unable to alloc memory");

   for (i=0; i<r1*c1; i++) {
      pt2[i] = skl->skl->data[i];
      pt3[i] = skl->dt->data[i];
      pt4[i] = skl->lbl->data[i];
   }

   free_ann_skl(&skl);
   r2 = r3 = r4 = r1;
   c2 = c3 = c4 = c1;
   CreateVarFromPtr(2, "d", &r2, &c2, &pt2);
   CreateVarFromPtr(3, "d", &r3, &c3, &pt3);
   CreateVarFromPtr(4, "d", &r4, &c4, &pt4);

   free(pt2);
   free(pt3);
   free(pt4);

   /*  Return variables  */
   LhsVar(1) = 2;
   LhsVar(2) = 3;
   LhsVar(3) = 4;
   return true;
}

/*----------------------------------------------------------------
 * out = thin(img)
 * Simple thinning algorithms, only Zhang-Suen supported for now.
 * The "skel" routine provide better and more complex algorithms.
 * In this funcion only "boundary deletion" algorithms shall be
 * provided.
 *----------------------------------------------------------------*/
SipExport int 
thin_int(char *fname)
{
   int   rim, cim, pim,  // img
         i,s,
         minlhs=1, maxlhs=1, minrhs=1, maxrhs=1;
   double *pt;
   bool stat;
   Img *im;

   CheckRhs(minrhs,maxrhs);
   CheckLhs(minlhs,maxlhs);

   GetRhsVar(1, "d", &rim, &cim, &pim);  // img 

   // pass transposed image to internal row-wise storage
   im=new_img(cim,rim); 
   sci_2D_double_matrix_to_animal(pim,rim,cim,im,pixval,1);
    
   im->isbinary = true;

   s= thinzs_np(im);
      if (!s) sip_error("thin: problem inside thinzs_np C subroutine");



   stat = animal_grayscale_image_to_double_array(fname,im,&pt);
   if (!stat) return false;
   imfree(&im);

   CreateVarFromPtr(2, "d", &rim, &cim, &pt);
   LhsVar(1) = 2;

   free(pt);
   return true;
}

/*----------------------------------------------------------------
 * i=percol(img [, direction])
 *  i is 1 if image is percolated; 0 otherwise. Direction is 1 if
 *  percolation is to be tested horizontally; 0 if it is to be
 *  tested vertically.
 *----------------------------------------------------------------*/

SipExport int 
percol_int(char *fname)
{
   int   r1, c1, p1,  
         r2, c2, p2,  
         ro, co, po, nv,
         i, direction, stat,
         minlhs=1, maxlhs=1, minrhs=1, maxrhs=2;
   Img *im; 

   CheckRhs(minrhs,maxrhs);
   CheckLhs(minlhs,maxlhs);


   nv = 1;
   GetRhsVar(nv++, "d", &r1, &c1, &p1);  // img 
   if (Rhs == 1)
      direction = 0;
   else {
      GetRhsVar(nv++, "d", &r2, &c2, &p2);  // direction
      direction = ((int)*stk(p2) != 1);
   }

   im=new_img(c1, r1);
   sci_2D_double_matrix_to_animal(p1,r1,c1,im,pixval,1);

   stat = percol(im,direction);
   imfree(&im);

   ro=co=1;
   CreateVar(nv, "d", &ro, &co, &po);
   *stk(po) = stat;

   LhsVar(1) = nv;
   return true;
}

/*----------------------------------------------------------------
 * out = bwdist(img [,method, max_dist])
 * Distance transforms. 
 *
 * "method" may be: 
 *    - "euclidean" : default euclidean method (Lotufo-Zampirolli)
 *    - "lotufo-zampirolli" (fast exact euclidean)
 *    - "costa-estrozi" (exact euclidean)
 *    - "IFT" - Image Foresting Transform (fast very accurate euclidean)
 *
 * In the future:
 *    - "chessboard"
 *    - "chamfer"
 *    - (...)
 *
 * TODO
 *    - add an output Label parameter (discrete Voronoi diagram)
 *----------------------------------------------------------------*/

SipExport int 
bwdist_int(char *fname)
{
   int   rim, cim, pim,  // img
			r_alg, c_alg, l_alg,
			r_d, c_d, l_d,
         i, nv=1,
         minlhs=1, maxlhs=1, minrhs=1, maxrhs=4;

   dt_algorithm alg=DT_MEIJSTER_2000;
   double *pt, max_dist = (double)((puint32) -1);
   char *str;
   bool noexec=false, stat, is1const;
   Img *im;
   ImgPUInt32 *dt;

   CheckRhs(minrhs,maxrhs); CheckLhs(minlhs,maxlhs);

   GetRhsVar(nv++, "d", &rim, &cim, &pim);  // img 
   if (Rhs >= 2) {
      GetRhsVar(nv++, "c", &r_alg, &c_alg, &l_alg);
      str=cstk(l_alg);
      if (strcasecmp("lotufo-zampirolli",str) == 0)
         alg=DT_LOTUFO_ZAMPIROLLI;
      else if ( strncasecmp("exact dilations",str,8) == 0 || 
              /* backward-compat:*/ strcmp("costa-estrozi",str) == 0)
         alg=DT_EXACT_DILATIONS;
      else if ( strcasecmp("IFT",str) == 0 || 
                strncasecmp("IFT 8",str,5) == 0)
         alg=DT_IFT;
      else if ( strncasecmp("IFT 4",str,5) == 0)
         alg=DT_IFT_4;
      else if ( strncasecmp("meijster",str,3) == 0)
         alg=DT_MEIJSTER_2000;
      else if ( strncasecmp("maurer",str,3) == 0)
         alg=DT_MAURER2003;
      else if ( strncasecmp("euclidean",str,6) == 0 || 
                strcasecmp("cuisenaire pmn",str) == 0)
         alg=DT_CUISENAIRE_PMN_1999;
      else if ( strncasecmp("cuisenaire pmon",str,15) == 0)
         alg=DT_CUISENAIRE_PMON_1999;
      else if ( strncasecmp("cuisenaire psn4",str,15) == 0)
         alg=DT_CUISENAIRE_PSN4_1999;
      else if ( strncasecmp("cuisenaire psn8",str,15) == 0)
         alg=DT_CUISENAIRE_PSN8_1999;
      else if ( strncasecmp("noexec",str,5) == 0) 
         noexec = true;
         /* undocumented option used to see how much overhead does
          * this interface function imposes for a particular image */
      else
			  sip_error("invalid second argument -- unknown method");
      if (Rhs >= 3) {
        GetRhsVar(nv++, "d", &r_d, &c_d, &l_d);
        max_dist = *stk(l_d);
      }
   }
   // pass transposed image to internal row-wise storage
   im=new_img(cim,rim); 

   sci_2D_double_matrix_to_animal(pim,rim,cim,im,pixval,1);

   is1const=true;
   for (i=0; i<rim*cim; ++i)
      if (DATA(im)[i]==0) {
         is1const=false;
         break;
      }
   if (is1const) {
      sip_warning("the input image is constant and different than 0");
      sip_warning("the distance transform is undefined for this case");
   }

   im->isbinary = true;

   if (noexec)
      dt = new_img_puint32(im->rows, im->cols);
   else
      dt = distance_transform_max_dist(im, alg, max_dist*max_dist);

   if (!dt) sip_error("problem inside distance_transform C subroutine");
   imfree(&im);  /* FIXME: use better err treatment */

   stat = animal_grayscale_imgpuint32_to_double_array(fname,dt,&pt);
   if (!stat) return false;

   imfree_puint32(&dt);

   CreateVarFromPtr(nv, "d", &rim, &cim, &pt);
   LhsVar(1) = nv;

   free(pt);
   return true;
}
