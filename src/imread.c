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
#include <time.h>
#include <sys/types.h>
#include <math.h>
#include "sip_common.h"




/*----------------------------------------------------------
 * int_imread: 
 *     interface for imread function.
 *     should provide   [RGB]=imread(name) 
 *                [Index,Map]=imread(name) at Scilab level 
 *  
 * TO-DO
 *    - return on errors, even if exeption is NULL
 *
 * $Revision: 1.1 $ $Date: 2005-07-20 01:33:34 $
 *----------------------------------------------------------*/
SipExport int 
int_imread(char *fname) 
{
   /* Interface variables */ 
   HyperMat *Img;
   int   m1, n1,l1, /* for name input argument      */
         m2, n2,    /* for index output argument    */
         minlhs=1, maxlhs=2, minrhs=1, maxrhs=1, i;
   double *l2;

   /* Other variables */
   unsigned long  imgsize;
 
   /* ImageMagick variables */
   ExceptionInfo  exception;
   Image          *image;
   ImageInfo      *image_info;
   PixelPacket    *pix;
   ImageType      imgtype;

   bool stat;


   CheckRhs(minrhs,maxrhs) ;
   CheckLhs(minlhs,maxlhs) ;
 
   /* Get name (#1) */
   GetRhsVar(1, "c", &m1, &n1, &l1);  
 
   /* Initialize the image info structure and read an image.  */
   InitializeMagick(NULL);
   GetExceptionInfo(&exception);
   image_info=CloneImageInfo((ImageInfo *) NULL);
   (void) strncpy(image_info->filename,cstk(l1),MaxTextExtent);
    
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

   if (sip_verbose == SIP_WORDY)
      sciprint("Size:\t%ld rows X %ld columns\n\r", m2, n2);

   imgsize = m2 * n2;
   
 
   pix=GetImagePixels(image, 0, 0, n2, m2);
   if(pix == (PixelPacket *) NULL)
      SIP_MAGICK_ERROR;
   
   switch(image->storage_class) {
   case DirectClass: {
      imgtype = GetImageType(image, &exception);
      if(imgtype == BilevelType) {
         stat = magick_binary_image_to_double_array(fname,pix,&l2, m2, n2);
         if (!stat) return false;
         CreateVarFromPtr(2, "d",&m2,&n2,&l2);  
         free(l2);
      } else {
         stat= magick_truecolor_image_to_double_hypermat(fname,pix,&Img,m2,n2);
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

   /* Terminate Imagemagick */
   DestroyImageInfo(image_info);
   DestroyImage(image);
   DestroyExceptionInfo(&exception);
   DestroyMagick();
   return true;
}
