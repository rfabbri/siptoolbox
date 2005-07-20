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
#include "sip_common.h"


/*----------------------------------------------------------
 * int_imfinfo: 
 *     interface for imfinfo function.
 *     should provide  type=imfinfo(name)  at Scilab level 
 * $Revision: 1.1 $ $Date: 2005-07-20 01:33:34 $
 *----------------------------------------------------------*/
SipExport int 
int_imfinfo(char *fname)
{
   /* Interface variables */ 
   int   mC, nC, lC,  /* for name input  arg */
         mV, nV, lV,  /* for "verbose" optional input arg */
         mL=12, nL=1, lL,  /* for list output arg */
         mS=1, nS=12,
         mFileName, nFileName=1, 
         mFileSize=1, nFileSize=1,
         mFormat, nFormat=1, 
         mWidth=1, nWidth=1, 
         mHeight=1, nHeight=1, 
         mDepth=1, nDepth=1, 
         mStorageType, nStorageType=1, 
         mNumberOfColors=1, nNumberOfColors=1, 
         mResolutionUnit, nResolutionUnit=1, 
         mXResolution=1, nXResolution=1, 
         mYResolution=1, nYResolution=1, 
         pos,
         minlhs=1, maxlhs=1, minrhs=1, maxrhs=2;
   unsigned long int *lWidth, *lHeight, *lDepth, *lNumberOfColors,
                     *lFileSize, fsize;
   char *lFileName, *lStorageType, *lFormat, *lResolutionUnit, *arg2;
   double *lXResolution, *lYResolution;
   static char 
      *Str[]= {   "type", "FileName", "FileSize", "Format", 
                  "Width", "Height", "Depth", "StorageType", 
                  "NumberOfColors", "ResolutionUnit", "XResolution", 
                  "YResolution" }; 

   /* ImageMagick variables */
   ExceptionInfo  exception;
   Image          *image;
   ImageInfo      *image_info;

   CheckRhs(minrhs,maxrhs) ;
   CheckLhs(minlhs,maxlhs) ;


   /* Get name (#1) and "verbose" optional arg (#2)*/
   GetRhsVar(1, "c", &mC, &nC, &lC);  
   if (Rhs == 2) {
      GetRhsVar(2, "c", &mV, &nV, &lV);
      arg2 = (char *)calloc(strlen(cstk(lV))+1, sizeof(char));
      if (!arg2)
         sip_error("unable to alloc memory");
      strcpy(arg2,cstk(lV));
   }

   InitializeMagick(NULL);
   GetExceptionInfo(&exception);
   image_info=CloneImageInfo((ImageInfo *) NULL);
   (void) strcpy(image_info->filename,cstk(lC));

   image = PingImage(image_info, &exception);
   if (image == (Image *) NULL)
      SIP_MAGICK_ERROR;


   pos =1;
   CreateVar(1, "t", &mL, &nL, &lL);
   CreateListVarFromPtr(1,pos++,"S", &mS, &nS, Str);

   lFileName = image->filename;
   mFileName = strlen(image->filename);
   CreateListVarFromPtr(1,pos++,"c", &mFileName, &nFileName, &lFileName);

   fsize = SizeBlob(image);
   lFileSize = &fsize;
   CreateListVarFromPtr(1,pos++,"i", &mFileSize, &nFileSize, &lFileSize);

   lFormat = image->magick;
   mFormat = strlen(image->magick);
   CreateListVarFromPtr(1,pos++,"c", &mFormat, &nFormat, &lFormat);

   lWidth = &(image->columns);
   CreateListVarFromPtr(1,pos++,"i", &mWidth, &nWidth, &lWidth);

   lHeight = &(image->rows);
   CreateListVarFromPtr(1,pos++,"i", &mHeight, &nHeight, &lHeight);

   lDepth = &(image->depth);
   CreateListVarFromPtr(1,pos++,"i", &mDepth, &nDepth, &lDepth);

   lStorageType = (image->storage_class == DirectClass)? "truecolor":"indexed";
   mStorageType = strlen(lStorageType);
   CreateListVarFromPtr(1,pos++,"c", &mStorageType, &nStorageType, &lStorageType);

   lNumberOfColors = &(image->colors);
   CreateListVarFromPtr(1,pos++,"i", &mNumberOfColors, \
                                     &nNumberOfColors, &lNumberOfColors);

   /*
   lComments = image->comments;
   mComments = strlen(image->comments);
   CreateListVarFromPtr(1,pos++,"c", &mComments, &nComments, &lComments);
   */

   lResolutionUnit = (image->units == PixelsPerInchResolution)?"inch":"centimeter";
   mResolutionUnit = strlen(lResolutionUnit);
   CreateListVarFromPtr(1,pos++,"c", &mResolutionUnit, \
                                     &nResolutionUnit, &lResolutionUnit);

   lXResolution = &(image->x_resolution);
   CreateListVarFromPtr(1,pos++,"i", &mXResolution, &nXResolution, &lXResolution);

   lYResolution = &(image->y_resolution);
   CreateListVarFromPtr(1,pos++,"i", &mYResolution, &nYResolution, &lYResolution);

   if (Rhs==2) {
      if (strcmp(arg2,"verbose") != 0)
         sip_error("invalid 2nd argument");
      sciprint("       FileName: %s\n\r", image->filename);
      sciprint("       FileSize: %d\n\r", fsize);
      sciprint("         Format: %s\n\r", image->magick);
      sciprint("          Width: %d\n\r", image->columns);
      sciprint("         Height: %d\n\r", image->rows);
      sciprint("          Depth: %d\n\r", image->depth);
      sciprint("    StorageType: %s\n\r", lStorageType);
      sciprint(" NumberOfColors: %d\n\r", image->colors);
      sciprint(" ResolutionUnit: %s\n\r", lResolutionUnit);
      sciprint("    XResolution: %f\n\r", image->x_resolution);
      sciprint("    YResolution: %f\n\r", image->y_resolution);
      free(arg2);
   }

   /* Terminate Imagemagick */
   DestroyImageInfo(image_info);
   DestroyImage(image);
   DestroyExceptionInfo(&exception);
   DestroyMagick();

   /*  Return variables  */
   LhsVar(1) = 1;
   return true;
}
