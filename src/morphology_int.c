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


/*----------------------------------------------------------
 * [img]=edilate(img [, radius, opt, pad])
 *    radius defaults to 5
 *    opt is "full" if result is not trimmed 
 *    opt is "same" if result is trimmed to original size
 * @@@@@@@@@
 * TODO
 * - use IFT fast euclidean propagation
 * - work with integers
 * - interface must be similar to matlab 
 * - fazer opcao: pad/unpad? como matlab faz isto?
 *----------------------------------------------------------*/
#define MAX_OPT 10
SipExport int 
edilate_int(char *fname)
{
   /* Interface variables */ 
   int   r1, c1, p1,  
         r2, c2, p2,  
         r3, c3, p3,  
         ro, co,
         i, irad,
         nv=1,
         minlhs=1, maxlhs=1, minrhs=1, maxrhs=3;
   Img *im, *result, *tmp;
   double *po, radius=5;
   char opt[MAX_OPT]="same";
   bool stat;

   CheckRhs(minrhs,maxrhs);
   CheckLhs(minlhs,maxlhs);


   GetRhsVar(nv++, "d", &r1, &c1, &p1);  // img 
   if (Rhs == 2) {
      GetRhsVar(nv++, "d", &r2, &c2, &p2);  // radius
      radius = *stk(p2);
   } else if (Rhs == 3) {
      GetRhsVar(nv++, "d", &r2, &c2, &p2);  // radius
      radius = *stk(p2);
      GetRhsVar(nv++, "c", &r3, &c3, &p3);  // opt
      strncpy(opt,cstk(p3),MAX_OPT);
   }

   /* @@@ maybe there's a better way of passing data */
   im=new_img(c1, r1);
   if (!im) sip_error("unable to alloc memory");
   sci_2D_double_matrix_to_animal(p1,r1,c1,im,pixval,1);

   irad   = (int) ceil(radius);
   result = new_img(im->rows+2*irad, im->cols+2*irad);
   if (!result) sip_error("unable to alloc memory");

   edilate_np(result,im,radius);    

   if (strcmp(opt,"same") == 0) {
      tmp = result;
      result = imtrim (result, irad, irad);
      imfree (&tmp);
   }
   imfree(&im);
   im = result;

   ro=im->cols; co=im->rows;
   stat = animal_grayscale_image_to_double_array(fname, im, &po);
   if (!stat) return false;
   imfree(&im);

   CreateVarFromPtr(nv, "d", &ro, &co, &po);

   /*  Return variables  */
   LhsVar(1) = nv;

   free(po);
   return true;
}

/*----------------------------------------------------------------
 * out = watershed(img [, markers, nhood])
 *
 * Description:  Watershed transform for image segmentation
 *    - also known as "Sup Reconstruction".
 *    - it is better to calculate gradient before using it without markers.
 *    - its core is easily extended for chromatic images
 * 
 * INPUT
 *    - im: grayscale image
 *    - nhood: connectivity. Must be 4 or 8.
 *    - markers: image with the markers (seeds). Each mark must have a
 *    unique label from 1 to N, where N is the number of marks.
 *       + if markers is "-1" or is omitted, then the regional minima of 
 *       "im" will be taken as the markers.
 *
 * OUTPUT 
 *    - an image with the watershed regions, each with a unique number.
 *    If minima != NULL, the regions will have the same label as the
 *    corresponding supplied markers.
 *
 * ALGORITHM
 *    - Image Foresting Transform (see ift.h)
 *
 * TODO
 *    - allow arbitrary neighborhood (structuring element)
 *
 * out = watershed(img [, markers, nhood])
 *----------------------------------------------------------------*/
SipExport int
watershed_int(char *fname)
{
   int   rim, cim, pim, 
         r_markers, c_markers, p_markers,
			r_nhood, c_nhood, p_nhood,
         i, nv=1,
         minlhs=1, maxlhs=1, minrhs=1, maxrhs=3;
   double *pt;
   Img *im;
   ImgPUInt32 *markers=NULL, *regions;
   nhood *nh;
   bool stat;

   CheckRhs(minrhs,maxrhs); CheckLhs(minlhs,maxlhs);

   GetRhsVar(nv++, "d", &rim, &cim, &pim);  // img 

   if (Rhs > 1) {
      GetRhsVar(nv++, "d", &r_markers, &c_markers, &p_markers);
      if ( *stk(p_markers) != -1 )  {
         if (r_markers != rim || c_markers != cim)
            sip_error("image and Marker must be the same size");
            
         markers = new_img_puint32(r_markers,c_markers);
         if (!markers) sip_error("not enough memory");

         /* pass transposed image to internal row-wise storage */
         for (i=0; i<r_markers*c_markers; i++)
            DATA(markers)[i] = (puint32) *stk(p_markers + i);
      } 

      if (Rhs == 3) {
         GetRhsVar(nv++, "d", &r_nhood, &c_nhood, &p_nhood);
         if (r_nhood * c_nhood != 1)
            sip_error("nhood must be a scalar");
         switch ( (int)*stk(p_nhood) ) {
            case 4:
               nh = get_4_nhood();
               break;
            case 8:
               nh = get_8_nhood();
               break;
            default:
               sip_error("invalid 3rd parameter (neighborhood)");
         }
      } else
         nh = get_8_nhood();
   } else
      nh = get_8_nhood();

   /* pass transposed image to internal row-wise storage */
   im=new_img(cim,rim); 
      if (!im) sip_error("not enough memory");
   sci_2D_double_matrix_to_animal(pim,rim,cim,im,pixval,PIXVAL_MAX);

   /* call Watershed */
   regions = watershed(im, markers, nh);
      if (!regions) sip_error("problem inside watershed C subroutine");

   imfree(&im);
   if (markers != NULL)
      imfree_puint32(&markers);
   free_nhood(&nh);

   /* return to Scilab */
   stat = animal_grayscale_imgpuint32_to_double_array(fname, regions, &pt);
   if (!stat) return false;

   CreateVarFromPtr(nv, "d", &rim, &cim, &pt);
   LhsVar(1) = nv;
   free(pt);

   return true;
}
