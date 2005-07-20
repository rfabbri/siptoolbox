/*
 * -------------------------------------------------------------------------
 * SIP - Scilab Image Processing toolbox
 * Copyright (C) 2002-2004  Ricardo Fabbri
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
#include <animal/animal.h>
#include "sip_common.h"


/*----------------------------------------------------------
 * [img]=drawline(img,xy)
 * - xy is a nx2 vector of n 2D (row,col) coordinates 
 *
 * TODO
 * - work with integers
 * - interface must be similar to matlab 
 *----------------------------------------------------------*/
SipExport int 
drawline_int(char *fname)
{
   /* Interface variables */ 
   int   r1, c1, p1,  
         r2, c2, p2,  
         ro, co,
         i,
         minlhs=1, maxlhs=1, minrhs=2, maxrhs=3;
   Img *im;
   double *po;
   bool stat;

   CheckRhs(minrhs,maxrhs);
   CheckLhs(minlhs,maxlhs);


   GetRhsVar(1, "d", &r1, &c1, &p1);  // img 
   GetRhsVar(2, "d", &r2, &c2, &p2);  // xy
   if (c2 != 2)
      sip_error("drawline: second arg. must have 2 cols");
   if (r2 < 2)
      sip_error("drawline: second arg. must have 2 or more rows");

   im=new_img(c1, r1);
   sci_2D_double_matrix_to_animal(p1,r1,c1,im,pixval,1);
    
   for (i=p2; i < p2+r2-1; ++i)
      imdrawline(im, PROUND(unsigned,*stk(i)-1), 
                     PROUND(unsigned,c1 - *stk(i+r2)),
                     PROUND(unsigned,*stk(i+1)-1), 
                     PROUND(unsigned,c1 - *stk(i+1+r2)));

   stat = animal_grayscale_image_to_double_array(fname, im, &po);
   if (!stat) return false;
   imfree(&im);

   ro=r1; co=c1;
   CreateVarFromPtr(3, "d", &ro, &co, &po);
   /*  Return variables  */
   LhsVar(1) = 3;
   free(po);
   return true;
}


SipExport int 
rgb2hsv_int(char *fname)
{
   int 
      rmap,cmap,pmap, 
      rout,cout,pout, 
      minlhs=1, maxlhs=1, minrhs=1, maxrhs=1;
   unsigned i,size2d;
   float r,g,b,h,s,v;

   HyperMat Im,*ImOut;


   CheckRhs(minrhs,maxrhs); CheckLhs(minlhs,maxlhs);

   switch (VarType(1)) {
   /*-------------*/
   case USUALMATRIX:
   /*-------------*/
      GetRhsVar(1, "d", &rmap, &cmap, &pmap);
      if (cmap != 3)
         sip_error("argument 1 must have 3 columns or be an rgb image");

      rout = rmap; cout = cmap;
      CreateVar(2, "d", &rout, &cout, &pout);

      for (i=0; i<(unsigned)rmap; ++i) {
         r = RCbyC(stk(pmap),i,0,rmap);
         g = RCbyC(stk(pmap),i,1,rmap);
         b = RCbyC(stk(pmap),i,2,rmap);
         rgb2hsv(r,g,b,&h,&s,&v);
         RCbyC(stk(pout),i,0,rmap) = h;
         RCbyC(stk(pout),i,1,rmap) = s;
         RCbyC(stk(pout),i,2,rmap) = v;
      }

      break;
   /*-------------*/
   case HYPERMATRIX:
   /*-------------*/

      sip_get_rhs_tru_img(1,&Im,fname);

      ImOut = new_sci_tru_img(IC_INT32(Im.sc.D)[0],IC_INT32(Im.sc.D)[1]);
      if (!ImOut)
         sip_error("unable to alloc memory for the return image\n");

      size2d = IC_INT32(Im.sc.D)[0]*IC_INT32(Im.sc.D)[1];

      for (i=0; i<size2d; ++i) {
         r = Im.R[i];
         g = Im.R[i +   size2d];
         b = Im.R[i + 2*size2d];
         rgb2hsv(r,g,b,&h,&s,&v);
         ImOut->R[i]            = h;
         ImOut->R[i +   size2d] = s;
         ImOut->R[i + 2*size2d] = v;
      }

      CreateHMat(2,ImOut);
      free_sci_tru_img(&ImOut);
      break;
   default:
      sip_error("argument 1 must be a colormap or rgb image.");
   }


   LhsVar(1) = 2;
   return true;
}

SipExport int 
hsv2rgb_int(char *fname)
{
   int 
      rmap,cmap,pmap, 
      rout,cout,pout, 
      minlhs=1, maxlhs=1, minrhs=1, maxrhs=1;
   unsigned i,size2d;
   float r,g,b,h,s,v;

   HyperMat Im, *ImOut;


   CheckRhs(minrhs,maxrhs); CheckLhs(minlhs,maxlhs);

   switch (VarType(1)) {
   /*-------------*/
   case USUALMATRIX:
   /*-------------*/
      GetRhsVar(1, "d", &rmap, &cmap, &pmap);
      if (cmap != 3)
         sip_error("argument 1 must have 3 columns or be a truecolorimage");

      rout = rmap; cout = cmap;
      CreateVar(2, "d", &rout, &cout, &pout);

      for (i=0; i<(unsigned)rmap; ++i) {
         h = RCbyC(stk(pmap),i,0,rmap);
         s = RCbyC(stk(pmap),i,1,rmap);
         v = RCbyC(stk(pmap),i,2,rmap);
         hsv2rgb(h,s,v,&r,&g,&b);
         RCbyC(stk(pout),i,0,rmap) = r;
         RCbyC(stk(pout),i,1,rmap) = g;
         RCbyC(stk(pout),i,2,rmap) = b;
      }

      break;
   /*-------------*/
   case HYPERMATRIX:
   /*-------------*/

      sip_get_rhs_tru_img(1,&Im,fname);

      ImOut = new_sci_tru_img(IC_INT32(Im.sc.D)[0],IC_INT32(Im.sc.D)[1]);
      if (!ImOut)
         sip_error("unable to alloc memory for the return image\n");

      size2d = IC_INT32(Im.sc.D)[0]*IC_INT32(Im.sc.D)[1];

      for (i=0; i<size2d; ++i) {
         h = Im.R[i];
         s = Im.R[i +   size2d];
         v = Im.R[i + 2*size2d];
         hsv2rgb(h,s,v,&r,&g,&b);
         ImOut->R[i]            = r;
         ImOut->R[i +   size2d] = g;
         ImOut->R[i + 2*size2d] = b;
      }

      CreateHMat(2,ImOut);
      free_sci_tru_img(&ImOut);
      break;
   default:
      sip_error("argument 1 must be a colormap or truecolor image.");
   }


   LhsVar(1) = 2;
   return true;
}
