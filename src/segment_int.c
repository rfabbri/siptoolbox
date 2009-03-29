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



/*----------------------------------------------------------------
 * [L,num] = bwlabel(img, nhood)
 *
 * Description:  Label connected components in a binary image
 * 
 * INPUT
 *    - img:   binary image
 *    - nhood: connectivity. Must be 4 or 8.
 *
 * OUTPUT 
 *    - L: grayscale image with regions numbered from 1 to N
 *    - num: number of components
 *    
 * ALGORITHM
 *    - Simple stack-based region growing/flooding
 *
 * TODO
 *    - allow arbitrary neighborhood (structuring element)
 *
 * [L,num] = bwlabel(img, nhood)
 *----------------------------------------------------------------*/
SipExport int
bwlabel_int(char *fname)
{
   int   rim, cim, pim, 
			r_nhood, c_nhood, p_nhood,
			rnum, cnum, pnum,
         i, j, nv=1, nregs,
         minlhs=1, maxlhs=2, minrhs=1, maxrhs=2;
   unsigned long npixels;
   double *pt;
   ImgPUInt32 *im;
   nhood *nh;

   CheckRhs(minrhs,maxrhs); CheckLhs(minlhs,maxlhs);

   animal_err_set_trace(ANIMAL_ERR_TRACE_ON);

   GetRhsVar(nv++, "d", &rim, &cim, &pim);  // img 
   npixels = rim*cim;
   if (npixels > PUINT32_MAX)
      sip_error("Image too big.");

   if (Rhs == 2) {
         GetRhsVar(nv++, "d", &r_nhood, &c_nhood, &p_nhood);
         if (r_nhood * c_nhood != 1)
            sip_error("nhood must be a scalar");
         switch ( (int)*stk(p_nhood) ) {
            case 4:
               nh = get_4_nhood();
                  if (!nh) { animal_err_report(); }
               break;
            case 8:
               nh = get_8_nhood();
                  if (!nh) { animal_err_report(); }
               break;
            default:
               sip_error("invalid 2nd parameter (neighborhood)");
         }
   } else
      nh = get_8_nhood();

   im=new_img_puint32(rim+2,cim+2); 
      if (!im) {
         animal_err_report();
         sip_error("not enough memory");
      }

   for(i=0; i<rim; i++)  
      for(j=0; j<cim; j++)
         RC(im,i+1,j+1) = PROUND(puint32,RCbyC(stk(pim),i,j,rim));
   
   nregs = label_components_np(im, nh);
      if (nregs < 0) {
         animal_err_report();
         sip_error("problem inside watershed C subroutine");
      }

   free_nhood(&nh);

   pt = (double *)calloc(rim*cim, sizeof(double));
      if (!pt) sip_error("unable to alloc memory");

   for(i=0; i<rim; i++)  
      for(j=0; j<cim; j++)  
         RCbyC(pt,i,j,rim) = RC(im,i+1,j+1);

   imfree_puint32(&im);

   CreateVarFromPtr(nv, "d", &rim, &cim, &pt);
   LhsVar(1) = nv++;
   free(pt);

   rnum = cnum = 1;
   CreateVar(nv, "d", &rnum, &cnum, &pnum);
   *stk(pnum)  = nregs;
   LhsVar(2) = nv;

   animal_heap_report(NULL);
   return true;
}
