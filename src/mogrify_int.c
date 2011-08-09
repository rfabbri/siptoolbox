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
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include "sip_common.h"


static bool
check_argv_validity(ImageInfo *image_info, char **argv, int *argc, ExceptionInfo *exception);

/*-----------------------------------------------------------------
 * [imout] = mogrify(im, argv);
 * [imout] = mogrify(imRGB, argv);
 * [imout,map] = mogrify(im, I, argv);
 *
 * Interface between imagemagick's MogrifyImage and scilab.
 * TODO
 *    - procedurize 2nd part (return Imagemagick -> Scilab)
 *    - upon return of an indexed image, will the user know how 
 *    many output variables there are? ImageMagick always returns 
 *    DirectClass after its operations??
 *    
 * $Revision: 1.4 $ $Date: 2009-03-29 21:34:48 $
 *-----------------------------------------------------------------*/


SipExport int 
mogrify_int(char *fname)
{
   int   mogrify_check_args(char *fname),
         rim, cim, pim,
         rimout, cimout, 
         rargv, cargv,
         nv, argc, argtype,
         minlhs=1, maxlhs=2, minrhs=1, maxrhs=3;
   char **argv;
   bool stat; 
   double *pimout;
   HyperMat *Im;

   /* ImageMagick variables */
   ExceptionInfo  exception;
   Image          *image;
   ImageInfo      *image_info;
   PixelPacket    *pix;


   CheckRhs(minrhs,maxrhs); CheckLhs(minlhs,maxlhs);
   argtype = mogrify_check_args(fname);
   if (!argtype) // error messages were already issued
      return false; 

   /* --- Pass scilab structures to IM --- */
   
   InitializeMagick(NULL);
   GetExceptionInfo(&exception);
   image_info=CloneImageInfo((ImageInfo *) NULL);
   image=AllocateImage(image_info);
  
   nv = 1;
   switch (argtype) {
      case ARG_2D:
            GetRhsVar(nv++, "d", &rim, &cim, &pim);
            stat = sci_2D_double_matrix_to_magick(fname, pim, rim, cim, image, &pix);
            if (!stat) 
               return false;
            break;
      case ARG_3D:
            stat = sci_3D_double_hypermat_to_magick(fname,nv++,image,&pix); 
            if (!stat) 
               return false;
            break;

      case ARG_INDEX_MAP:
            stat = sci_index_map_to_magick(fname, nv, image, &pix);
            if (!stat) 
               return false;
            nv+=2;
            break;

      default:
            sip_error("invalid argument type");
   }

   /* --- Call ImageMagick's Mogrify --- */

   GetRhsVar(nv++, "S", &rargv, &cargv, &argv);  
   argc = rargv*cargv;

   stat = check_argv_validity(image_info, argv, &argc, &exception);
   if(!stat) {
      fprintf(stderr,"%s: ",fname);
      Scierror(999,"");
      sciprint("%s: ",fname);
      GetImageException(image,&exception);                                  
      if( (exception.reason != NULL)  ) {
         if (strcmp(exception.reason,"UnrecognizedOption") == 0) {
            sciprint("argv parameter %d unrecognized",argc);
            fprintf(stderr,"argv parameter %d unrecognized",argc);
         } else {
            fprintf(stderr,"%s (near argv parameter %d)",
               GetLocaleExceptionMessage(exception.severity,exception.reason),argc);
            sciprint("%s (near argv parameter %d)",
               GetLocaleExceptionMessage(exception.severity,exception.reason),argc);
         }
         if (exception.description != NULL)
            fprintf(stderr,": %s",
               GetLocaleExceptionMessage(exception.severity,exception.description));
            sciprint(": %s",
               GetLocaleExceptionMessage(exception.severity,exception.description));
         fprintf(stderr,".\n");
         sciprint(".\n\r");
      } else {
         fprintf(stderr,"unknown reason (near argv parameter %d).\n",argc);
         sciprint("unknown reason (near argv parameter %d).\n\r",argc);
      }

      /* clean up */
      DestroyImageInfo(image_info);
      DestroyImage(image);
      DestroyExceptionInfo(&exception);
      DestroyMagick();
      return false;
   }

#ifdef SIP_HAVE_MogrifyImage_5args
   stat = MogrifyImage(image_info, argc, argv, &image,&exception);
#else
   stat = MogrifyImage(image_info, argc, argv, &image);
   GetImageException(image,&exception);
#endif
   if(!stat || exception.severity != UndefinedException) 
      SIP_MAGICK_ERROR;

   FreeRhsSVar(argv);

   /* --- Return image to SciLab --- */


   rimout  = image->rows; cimout = image->columns;

   pix = GetImagePixels(image,0,0,cimout,rimout);
   if(!pix)
      SIP_MAGICK_ERROR;

   switch(image->storage_class) {
      case DirectClass:
         if ( IsMonochromeImage(image, &exception) ) {
            stat = magick_binary_image_to_double_array(fname,pix,&pimout, rimout, cimout);
            if (!stat) return false;
            CreateVarFromPtr(nv, "d",&rimout,&cimout,&pimout);  
            LhsVar(1) = nv++;
            free(pimout);
         } else if ( IsGrayImage(image,&exception) ) {
            stat = magick_grayscale_image_to_double_array(fname,pix,&pimout, rimout, cimout);
            if (!stat) return false;
            CreateVarFromPtr(nv, "d",&rimout,&cimout,&pimout);  
            LhsVar(1) = nv++;
            free(pimout);
         } else {
            stat= magick_truecolor_image_to_double_hypermat(fname,pix,&Im,rimout,cimout);
            if (!stat) return false;
            CreateHMat(nv,Im);
            LhsVar(1) = nv++;
            free_sci_tru_img(&Im);
         }
         break;
     case PseudoClass: 
         stat= magick_index_map_to_sci_dbl(fname,image,nv);
         if (!stat) return false;
         LhsVar(1) = nv;
         LhsVar(2) = nv+1;
         break;
     default: 
        sip_error("unknown color class");
        break;
   }


   /* Terminate Imagemagick */
   DestroyImageInfo(image_info);
   DestroyImage(image);
   DestroyExceptionInfo(&exception);
   DestroyMagick();
   return true;
}

/*-----------------------------------------------------------
 * check_args:                                                
 *    Certify the correctness of the number and type of
 *    arguments. 
 *    The returning code indicates the sematic type of the
 *    argument (non-zero), or invalid argument (zero)
 *    - @@@ I guess this function could be less pedantic
 *-----------------------------------------------------------*/
int 
mogrify_check_args(char *fname)
{

   if (Rhs == 2) {  // mogrify(image, string)
      if (VarType(2) != STRING)
         sip_error("argument 2 must be a string");
      switch (VarType(1)) {
         case USUALMATRIX: return ARG_2D; 
         case HYPERMATRIX: return ARG_3D; 
         default: 
            sip_error("argument 1 must be matrix or RGB hypermatrix.");
      }
   } 

   /* Rhs == 3  --> mogrify(image,map,string) */
   if(VarType(3) != STRING) 
      sip_error("argument 3 must be a string");
   if((VarType(1) != USUALMATRIX) || (VarType(2) != USUALMATRIX))
      sip_error("first two arguments must be arrays");

   return ARG_INDEX_MAP;
}

bool
check_argv_validity(ImageInfo *image_info, char **argv, int *pargc, ExceptionInfo *exception)
{
#define ThrowMogrifyException(code,reason,description) \
{ \
  (void) ThrowException(exception,code,reason,description); \
  *pargc = i+1; \
  return false; \
}
   int i, argc=*pargc;
   unsigned global_colormap;
   long x;
   char *option, *format;

   for (i=0; i < argc; ++i) {
      option = argv[i];
      switch (option[1])
      {
        case 'a':
        {
          if (LocaleCompare("affine",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingMatrix",option);
                }
              break;
            }
          if (LocaleCompare("antialias",option+1) == 0)
            {
              image_info->antialias=(*option == '-');
              break;
            }
          /*
          if (LocaleCompare("authenticate",option+1) == 0)
            {
              (void) CloneString(&image_info->authenticate,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingPassword",option);
                  (void) CloneString(&image_info->authenticate,argv[i]);
                }
              break;
            }
            */
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'b':
        {
          if (LocaleCompare("background",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingBackgroundColor",
                      option);
                  (void) SIP_QueryColorDatabase(argv[i],&image_info->background_color,
                    exception);
                }
              break;
            }
          if (LocaleCompare("black-threshold",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingThreshold",option);
                }
              break;
            }
          if (LocaleCompare("blue-primary",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingPoint",option);
                }
              break;
            }
          if (LocaleCompare("blur",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingGeometry",option);
              break;
            }
          if (LocaleCompare("border",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("bordercolor",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingBorderColor",
                      option);
                  (void) SIP_QueryColorDatabase(argv[i],&image_info->border_color,
                    exception);
                }
              break;
            }
          if (LocaleCompare("box",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingColors",option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'c':
        {
           /*
          if (LocaleCompare("cache",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingThreshold",option);
                  SetMagickResourceLimit(MemoryResource,atol(argv[i]));
                  SetMagickResourceLimit(MapResource,2*atol(argv[i]));
                }
              break;
            }
            */
          if (LocaleCompare("channel",option+1) == 0)
            {
              if (*option == '-')
                {
                  ChannelType
                    channel;

                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  option=argv[i];
                  channel=UndefinedChannel;
                 if (LocaleCompare("Red",option) == 0)
                   channel=RedChannel;
                 if (LocaleCompare("Cyan",option) == 0)
                   channel=CyanChannel;
                 if (LocaleCompare("Green",option) == 0)
                   channel=GreenChannel;
                 if (LocaleCompare("Magenta",option) == 0)
                   channel=MagentaChannel;
                 if (LocaleCompare("Blue",option) == 0)
                   channel=BlueChannel;
                 if (LocaleCompare("Yellow",option) == 0)
                   channel=YellowChannel;
                 if (LocaleCompare("Opacity",option) == 0)
                   channel=OpacityChannel;
                 if (LocaleCompare("Black",option) == 0)
                   channel=BlackChannel;
                 if (LocaleCompare("Matte",option) == 0)
                   channel=MatteChannel;
                  if (channel == UndefinedChannel)
                    ThrowMogrifyException(OptionError,"UnrecognizedChannelType",
                      option);
                }
              break;
            }
          if (LocaleCompare("charcoal",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingPaintRadius",
                      option);
                }
              break;
            }
          if (LocaleCompare("chop",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("colorize",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingColorizeValue",
                      option);
                }
              break;
            }
          if (LocaleCompare("colors",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingColors",option);
                }
              break;
            }
          if (LocaleCompare("colorspace",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  option=argv[i];
                  image_info->colorspace=UndefinedColorspace;
                  if (LocaleCompare("cmyk",option) == 0)
                    image_info->colorspace=CMYKColorspace;
                  if (LocaleCompare("gray",option) == 0)
                    image_info->colorspace=GRAYColorspace;
#ifdef SIP_HAVE_HSLCOLORSPACE
                  if (LocaleCompare("hsl",option) == 0)
                    image_info->colorspace=HSLColorspace;
#endif
#ifdef SIP_HAVE_HWBCOLORSPACE
                  if (LocaleCompare("hwb",option) == 0)
                    image_info->colorspace=HWBColorspace;
#endif
                  if (LocaleCompare("ohta",option) == 0)
                    image_info->colorspace=OHTAColorspace;
                  if (LocaleCompare("rgb",option) == 0)
                    image_info->colorspace=RGBColorspace;
                  if (LocaleCompare("srgb",option) == 0)
                    image_info->colorspace=sRGBColorspace;
                  if (LocaleCompare("transparent",option) == 0)
                    image_info->colorspace=TransparentColorspace;
                  if (LocaleCompare("xyz",option) == 0)
                    image_info->colorspace=XYZColorspace;
                  if (LocaleCompare("ycbcr",option) == 0)
                    image_info->colorspace=YCbCrColorspace;
                  if (LocaleCompare("ycc",option) == 0)
                    image_info->colorspace=YCCColorspace;
                  if (LocaleCompare("yiq",option) == 0)
                    image_info->colorspace=YIQColorspace;
                  if (LocaleCompare("ypbpr",option) == 0)
                    image_info->colorspace=YPbPrColorspace;
                  if (LocaleCompare("yuv",option) == 0)
                    image_info->colorspace=YUVColorspace;
                  if (image_info->colorspace == UndefinedColorspace)
                    ThrowMogrifyException(OptionError,"UnrecognizedColorspace",option);
                }
              break;
            }
          if (LocaleCompare("comment",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingComment",option);
                }
              break;
            }
          if (LocaleCompare("contrast",option+1) == 0)
            break;
          if (LocaleCompare("convolve",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == (argc-1)) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingCoefficients",
                      option);
                }
              break;
            }
          if (LocaleCompare("crop",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("cycle",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingCycleAmount",
                      option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'd':
        {
           /*
          if (LocaleCompare("debug",option+1) == 0)
            {
              (void) SetLogEventMask("None");
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingEventMask",option);
                  (void) SetLogEventMask(argv[i]);
                }
              break;
            }
            */
          if (LocaleCompare("delay",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingDelay",option);
                }
              break;
            }
          if (LocaleCompare("density",option+1) == 0)
            {
              (void) CloneString(&image_info->density,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                  (void) CloneString(&image_info->density,argv[i]);
                }
              break;
            }
          if (LocaleCompare("depth",option+1) == 0)
            {
              image_info->depth=QuantumDepth;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingImageDepth",option);
                  image_info->depth=atol(argv[i]);
                }
              break;
            }
          if (LocaleCompare("despeckle",option+1) == 0)
            break;
          if (LocaleCompare("display",option+1) == 0)
            {
              (void) CloneString(&image_info->server_name,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingServerName",option);
                  (void) CloneString(&image_info->server_name,argv[i]);
                }
              break;
            }
          if (LocaleCompare("dispose",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingDisposeMethod",
                      option);
                  if ((LocaleCompare("0",option) != 0) &&
                      (LocaleCompare("1",option) != 0) &&
                      (LocaleCompare("2",option) != 0) &&
                      (LocaleCompare("3",option) != 0) &&
                      (LocaleCompare("Undefined",option) != 0) &&
                      (LocaleCompare("None",option) != 0) &&
                      (LocaleCompare("Background",option) != 0) &&
                      (LocaleCompare("Previous",option) != 0))
                    ThrowMogrifyException(OptionError,"UnrecognizedDisposeMethod",
                      option);
                }
              break;
            }
          if (LocaleCompare("dither",option+1) == 0)
            {
              image_info->dither=(*option == '-');
              break;
            }
          if (LocaleCompare("draw",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingDrawingPrimitive",
                      option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'e':
        {
          if (LocaleCompare("edge",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingPaintRadius",
                      option);
                }
              break;
            }
          if (LocaleCompare("emboss",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingEmbossRadius",
                      option);
                }
              break;
            }
          if (LocaleCompare("encoding",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingEncodingType",
                      option);
                }
              break;
            }
          if (LocaleCompare("enhance",option+1) == 0)
            break;
          if (LocaleCompare("equalize",option+1) == 0)
            break;
          /* Removed to remain backward-compatible to 5.5.1
          if (LocaleCompare("extract",option+1) == 0)
            {
              (void) CloneString(&image_info->extract,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                  (void) CloneString(&image_info->extract,argv[i]);
                }
              break;
            }
         */
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'f':
        {
          if (LocaleCompare("fill",option+1) == 0)
            {
              (void) SIP_QueryColorDatabase("none",&image_info->pen,exception);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingFillColor",option);
                  (void) SIP_QueryColorDatabase(argv[i],&image_info->pen,exception);
                }
              break;
            }
          if (LocaleCompare("filter",option+1) == 0)
            {
              if (*option == '-')
                {
                  FilterTypes
                    filter;

                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  option=argv[i];
                  filter=UndefinedFilter;
                  if (LocaleCompare("Point",option) == 0)
                    filter=PointFilter;
                  if (LocaleCompare("Box",option) == 0)
                    filter=BoxFilter;
                  if (LocaleCompare("Triangle",option) == 0)
                    filter=TriangleFilter;
                  if (LocaleCompare("Hermite",option) == 0)
                    filter=HermiteFilter;
                  if (LocaleCompare("Hanning",option) == 0)
                    filter=HanningFilter;
                  if (LocaleCompare("Hamming",option) == 0)
                    filter=HammingFilter;
                  if (LocaleCompare("Blackman",option) == 0)
                    filter=BlackmanFilter;
                  if (LocaleCompare("Gaussian",option) == 0)
                    filter=GaussianFilter;
                  if (LocaleCompare("Quadratic",option) == 0)
                    filter=QuadraticFilter;
                  if (LocaleCompare("Cubic",option) == 0)
                    filter=CubicFilter;
                  if (LocaleCompare("Catrom",option) == 0)
                    filter=CatromFilter;
                  if (LocaleCompare("Mitchell",option) == 0)
                    filter=MitchellFilter;
                  if (LocaleCompare("Lanczos",option) == 0)
                    filter=LanczosFilter;
                  if (LocaleCompare("Bessel",option) == 0)
                    filter=BesselFilter;
                  if (LocaleCompare("Sinc",option) == 0)
                    filter=SincFilter;
                  if (filter == UndefinedFilter)
                    ThrowMogrifyException(OptionError,"UnrecognizedImageFilter",
                      option);
                }
              break;
            }
          if (LocaleCompare("flip",option+1) == 0)
            break;
          if (LocaleCompare("flop",option+1) == 0)
            break;
          if (LocaleCompare("font",option+1) == 0)
            {
              (void) CloneString(&image_info->font,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingFontName",
                      option);
                  (void) CloneString(&image_info->font,argv[i]);
                }
              break;
            }
          if (LocaleCompare("format",option+1) == 0)
            {
              (void) CloneString(&format,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingImageType",option);
                  (void) CloneString(&format,argv[i]);
                  (void) strncpy(image_info->filename,format,MaxTextExtent-1);
                  (void) strcat(image_info->filename,":");
                  (void) SetImageInfo(image_info,false,exception);
                  if (*image_info->magick == '\0')
                    ThrowMogrifyException(OptionError,
                      "UnrecognizedImageFormat",format);
                }
              break;
            }
          if (LocaleCompare("frame",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",
                      option);
                }
              break;
            }
          if (LocaleCompare("fuzz",option+1) == 0)
            {
              image_info->fuzz=0.0;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingDistance",option);
                  // To make it link against Ubuntu 11.04's seemingly broken 
                  // ImageMagick:
                  // image_info->fuzz=StringToDouble(argv[i],MaxRGB);
                  //image_info->fuzz=(double)((x < MaxRGB) ? x : MaxRGB);
                  fprintf(stderr,"Option -fuzz %s temporarily hardcoded to 0.0\n", argv[i]);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'g':
        {
          if (LocaleCompare("gamma",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingImageGamma",option);
              break;
            }
          if (LocaleCompare("gaussian",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingGeometry",option);
              break;
            }
          if (LocaleCompare("geometry",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("gravity",option+1) == 0)
            {
              GravityType
                gravity;

              gravity=(GravityType) ForgetGravity;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  option=argv[i];
                  if (LocaleCompare("NorthWest",option) == 0)
                    gravity=(GravityType) NorthWestGravity;
                  if (LocaleCompare("North",option) == 0)
                    gravity=(GravityType) NorthGravity;
                  if (LocaleCompare("NorthEast",option) == 0)
                    gravity=(GravityType) NorthEastGravity;
                  if (LocaleCompare("West",option) == 0)
                    gravity=(GravityType) WestGravity;
                  if (LocaleCompare("Center",option) == 0)
                    gravity=(GravityType) CenterGravity;
                  if (LocaleCompare("East",option) == 0)
                    gravity=(GravityType) EastGravity;
                  if (LocaleCompare("SouthWest",option) == 0)
                    gravity=(GravityType) SouthWestGravity;
                  if (LocaleCompare("South",option) == 0)
                    gravity=(GravityType) SouthGravity;
                  if (LocaleCompare("SouthEast",option) == 0)
                    gravity=(GravityType) SouthEastGravity;
                  if (gravity == (GravityType) ForgetGravity)
                    ThrowMogrifyException(OptionError,"UnrecognizedGravityType",
                      option);
                }
              break;
            }
          if (LocaleCompare("green-primary",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingPoint",option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'h':
        {
          if (LocaleCompare("help",option+1) == 0)
            break;
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'i':
        {
          if (LocaleCompare("implode",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingImplodeAmount",
                      option);
                }
              break;
            }
          if (LocaleCompare("interlace",option+1) == 0)
            {
              image_info->interlace=NoInterlace;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  option=argv[i];
                  image_info->interlace=UndefinedInterlace;
                  if (LocaleCompare("None",option) == 0)
                    image_info->interlace=NoInterlace;
                  if (LocaleCompare("Line",option) == 0)
                    image_info->interlace=LineInterlace;
                  if (LocaleCompare("Plane",option) == 0)
                    image_info->interlace=PlaneInterlace;
                  if (LocaleCompare("Partition",option) == 0)
                    image_info->interlace=PartitionInterlace;
                  if (image_info->interlace == UndefinedInterlace)
                    ThrowMogrifyException(OptionError,"UnrecognizedInterlaceType",
                      option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'l':
        {
          if (LocaleCompare("label",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingImageLabel",
                      option);
                }
              break;
            }
          if (LocaleCompare("lat",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("level",option+1) == 0)
            {
              i++;
              if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                ThrowMogrifyException(OptionError,"MissingImageLevel",option);
              break;
            }
          if (LocaleCompare("linewidth",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingPointsize",option);
                }
              break;
            }
          /*
          if (LocaleCompare("limit",option+1) == 0)
            {
              if (*option == '-')
                {
                  char
                    *type;

                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingResourceType",
                      option);
                  type=argv[i];
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingResourceLimit",
                      option);
                  if (LocaleCompare("disk",type) == 0)
                    SetMagickResourceLimit(DiskResource,atol(argv[i]));
                  else
                    if (LocaleCompare("map",type) == 0)
                      SetMagickResourceLimit(MapResource,atol(argv[i]));
                    else
                      if (LocaleCompare("memory",type) == 0)
                        SetMagickResourceLimit(MemoryResource,atol(argv[i]));
                      else
                        ThrowMogrifyException(OptionError,
                          "UnrecognizedResourceType",type);
                }
              break;
            }
            */
          if (LocaleCompare("list",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingListName",
                      option);
                  option=argv[i];
                  switch (*option)
                  {
                    case 'C':
                    case 'c':
                    {
                      if (LocaleCompare("Color",option) == 0)
                        {
                          (void) ListColorInfo((FILE *) NULL,exception);
                          break;
                        }
                      ThrowMogrifyException(OptionError,"UnrecognizedListType",
                        option)
                    }
                    case 'D':
                    case 'd':
                    {
                      if (LocaleCompare("Delegate",option) == 0)
                        {
                          (void) ListDelegateInfo((FILE *) NULL,exception);
                          break;
                        }
                      ThrowMogrifyException(OptionError,"UnrecognizedListType",
                        option)
                    }
                    case 'F':
                    case 'f':
                    {
                      if (LocaleCompare("Format",option) == 0)
                        {
                          (void) ListMagickInfo((FILE *) NULL,exception);
                          break;
                        }
                      ThrowMogrifyException(OptionError,"UnrecognizedListType",
                        option)
                    }
                    case 'M':
                    case 'm':
                    {
                      if (LocaleCompare("Magic",option) == 0)
                        {
                          (void) ListMagicInfo((FILE *) NULL,exception);
                          break;
                        }
                      ThrowMogrifyException(OptionError,"UnrecognizedListType",
                        option)
                    }
                    /* Removed for compatibility with 5.5.1
                    case 'R':
                    case 'r':
                    {
                      if (LocaleCompare("Resource",option) == 0)
                        {
                          (void) ListMagickResourceInfo((FILE *) NULL,exception);
                          break;
                        }
                      ThrowMogrifyException(OptionError,"UnrecognizedListType",
                        option)
                    }
                    */
                    case 'T':
                    case 't':
                    {
                      if (LocaleCompare("Type",option) == 0)
                        {
                          (void) ListTypeInfo((FILE *) NULL,exception);
                          break;
                        }
                      ThrowMogrifyException(OptionError,"UnrecognizedListType",
                        option)
                    }
                    default:
                      ThrowMogrifyException(OptionError,"UnrecognizedListType",
                        option)
                  }
                  return true;
                }
              break;
            }
          /* Removed for compat. with 5.5.1
          if (LocaleCompare("log",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingLogFormat",option);
                  (void) SetLogFormat(argv[i]);
                }
              break;
            }
            */
          if (LocaleCompare("loop",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingIterations",option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'm':
        {
          if (LocaleCompare("map",option+1) == 0)
            {
              global_colormap=(*option == '+');
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingFilename",option);
                }
              break;
            }
          if (LocaleCompare("mask",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingFilename",option);
                }
              break;
            }
          if (LocaleCompare("matte",option+1) == 0)
            break;
          if (LocaleCompare("mattecolor",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingMatteColor",option);
                  (void) SIP_QueryColorDatabase(argv[i],&image_info->matte_color,
                    exception);
                }
              break;
            }
          if (LocaleCompare("modulate",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingImageGamma",option);
                }
              break;
            }
          if (LocaleCompare("median",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingPaintRadius",
                      option);
                }
              break;
            }
          if (LocaleCompare("monochrome",option+1) == 0)
            {
              image_info->monochrome=(*option == '-');
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'n':
        {
          if (LocaleCompare("negate",option+1) == 0)
            break;
          if (LocaleCompare("noise",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingImageGamma",option);
                }
              if (*option == '+')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  option=argv[i];
                  if ((LocaleCompare("Uniform",option) != 0) &&
                      (LocaleCompare("Gaussian",option) != 0) &&
                      (LocaleCompare("Multiplicative",option) != 0) &&
                      (LocaleCompare("Impulse",option) != 0) &&
                      (LocaleCompare("Laplacian",option) != 0) &&
                      (LocaleCompare("Poisson",option) != 0))
                    ThrowMogrifyException(OptionError,"UnrecognizedNoiseType",
                      option);
                }
              break;
            }
          if (LocaleCompare("noop",option+1) == 0)
            break;
          if (LocaleCompare("normalize",option+1) == 0)
            break;
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'o':
        {
          if (LocaleCompare("opaque",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingOpaqueColor",
                      option);
                }
              break;
            }
          if (LocaleCompare("ordered-dither",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingOrder",option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'p':
        {
          if (LocaleCompare("paint",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingPaintRadius",
                      option);
                }
              break;
            }
          if (LocaleCompare("pointsize",option+1) == 0)
            {
              image_info->pointsize=12;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingPointsize",option);
                  image_info->pointsize=atof(argv[i]);
                }
              break;
            }
          if (LocaleCompare("profile",option+1) == 0)
            {
              i++;
              if (i == argc)
                ThrowMogrifyException(OptionError,"MissingProfile",option);
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'q':
        {
          if (LocaleCompare("quality",option+1) == 0)
            {
              image_info->quality=SIP_MAGICK_DEFAULT_COMPRESSION_QUALITY;
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingCompressionQuality",
                      option);
                  image_info->quality=atol(argv[i]);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'r':
        {
          if (LocaleCompare("raise",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingBevelWidth",option);
              break;
            }
          if (LocaleCompare("random-threshold",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingThreshold",option);
                }
              break;
            }
          if (LocaleCompare("red-primary",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingPoint",option);
                }
              break;
            }
          if (LocaleCompare("region",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("render",option+1) == 0)
            break;
          if (LocaleCompare("resample",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == (argc-1)) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("resize",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("roll",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("rotate",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingImageRotation",option);
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 's':
        {
          if (LocaleCompare("sample",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          /*
          if (LocaleCompare("sampling-factor",option+1) == 0)
            {
              (void) CloneString(&image_info->sampling_factor,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                  (void) CloneString(&image_info->sampling_factor,argv[i]);
                }
              break;
            }
            */
          if (LocaleCompare("scale",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("scene",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingImageScene",
                      option);
                }
              break;
            }
          /* Removed for backward compat. with 5.5.1
          if (LocaleCompare("seed",option+1) == 0)
            {
              unsigned long
                seed;

              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingImageGamma",option);
                }
              seed=atol(argv[i]);
              DistillRandomEvent((unsigned char *) &seed,sizeof(unsigned long));
              break;
            }
            */
          if (LocaleCompare("segment",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingThreshold",option);
                }
              break;
            }
          if (LocaleCompare("shade",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingAzimuth",option);
              break;
            }
          if (LocaleCompare("sharpen",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingGeometry",option);
              break;
            }
          if (LocaleCompare("shave",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("shear",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingGeometry",option);
              break;
            }
          if (LocaleCompare("size",option+1) == 0)
            {
              (void) CloneString(&image_info->size,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                  (void) CloneString(&image_info->size,argv[i]);
                }
              break;
            }
          if (LocaleCompare("solarize",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingThreshold",
                      option);
                }
              break;
            }
          if (LocaleCompare("spread",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingAmount",option);
                }
              break;
            }
          if (LocaleCompare("stroke",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingColors",option);
                }
              break;
            }
          if (LocaleCompare("strokewidth",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingPointsize",option);
                }
              break;
            }
          if (LocaleCompare("swirl",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingImageRotation",
                      option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 't':
        {
          if (LocaleCompare("texture",option+1) == 0)
            {
              (void) CloneString(&image_info->texture,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingFilename",
                      option);
                  (void) CloneString(&image_info->texture,argv[i]);
                }
              break;
            }
          if (LocaleCompare("threshold",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingThreshold",option);
                }
              break;
            }
          if (LocaleCompare("thumbnail",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingGeometry",option);
                }
              break;
            }
          if (LocaleCompare("transparent",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingTransparentColor",
                      option);
                }
              break;
            }
          if (LocaleCompare("treedepth",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingTreeDepth",
                      option);
                }
              break;
            }
          if (LocaleCompare("trim",option+1) == 0)
            break;
          if (LocaleCompare("type",option+1) == 0)
            {
              image_info->type=UndefinedType;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  option=argv[i];
                  if (LocaleCompare("Bilevel",option) == 0)
                    image_info->type=BilevelType;
                  if (LocaleCompare("Grayscale",option) == 0)
                    image_info->type=GrayscaleType;
                  if (LocaleCompare("GrayscaleMatte",option) == 0)
                    image_info->type=GrayscaleMatteType;
                  if (LocaleCompare("Palette",option) == 0)
                    image_info->type=PaletteType;
                  if (LocaleCompare("PaletteMatte",option) == 0)
                    image_info->type=PaletteMatteType;
                  if (LocaleCompare("TrueColor",option) == 0)
                    image_info->type=TrueColorType;
                  if (LocaleCompare("TrueColorMatte",option) == 0)
                    image_info->type=TrueColorMatteType;
                  if (LocaleCompare("ColorSeparation",option) == 0)
                    image_info->type=ColorSeparationType;
                  if (LocaleCompare("ColorSeparationMatte",option) == 0)
                    image_info->type=ColorSeparationMatteType;
                  if (LocaleCompare("Optimize",option) == 0)
                    image_info->type=OptimizeType;
                  if (image_info->type == UndefinedType)
                    ThrowMogrifyException(OptionError,"UnrecognizedImageType",
                      option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'u':
        {
          if (LocaleCompare("undercolor",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingColors",option);
                }
              break;
            }
          if (LocaleCompare("units",option+1) == 0)
            {
              image_info->units=UndefinedResolution;
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingType",option);
                  option=argv[i];
                  image_info->units=UndefinedResolution;
                  if (LocaleCompare("PixelsPerInch",option) == 0)
                    image_info->units=PixelsPerInchResolution;
                  if (LocaleCompare("PixelsPerCentimeter",option) == 0)
                    image_info->units=PixelsPerCentimeterResolution;
                }
              break;
            }
          if (LocaleCompare("unsharp",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingGeometry",option);
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'v':
        {
          if (LocaleCompare("verbose",option+1) == 0)
            {
              image_info->verbose=(*option == '-');
              break;
            }
          if (LocaleCompare("verbose",option+1) == 0)
            break;
          if (LocaleCompare("view",option+1) == 0)
            {
              (void) CloneString(&image_info->view,(char *) NULL);
              if (*option == '-')
                {
                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,"MissingViewTransform",
                      option);
                  (void) CloneString(&image_info->view,argv[i]);
                }
              break;
            }
#ifdef SIP_HAVE_VIRTUAL_PIXEL
          if (LocaleCompare("virtual-pixel",option+1) == 0)
            {
              if (*option == '-')
                {
                  VirtualPixelMethod
                    virtual_pixel_method;

                  i++;
                  if (i == argc)
                    ThrowMogrifyException(OptionError,
                      "MissingVirtualPixelMethod",option);
                  option=argv[i];
                  virtual_pixel_method=UndefinedVirtualPixelMethod;
                  if (LocaleCompare("Constant",option) == 0)
                    virtual_pixel_method=ConstantVirtualPixelMethod;
                  if (LocaleCompare("Edge",option) == 0)
                    virtual_pixel_method=EdgeVirtualPixelMethod;
                  if (LocaleCompare("Mirror",option) == 0)
                    virtual_pixel_method=MirrorVirtualPixelMethod;
                  if (LocaleCompare("Tile",option) == 0)
                    virtual_pixel_method=TileVirtualPixelMethod;
                  if (virtual_pixel_method == UndefinedVirtualPixelMethod)
                    ThrowMogrifyException(OptionError,
                      "UnrecognizedVirtualPixelMethod",option);
                }
              break;
            }
#endif
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case 'w':
        {
          if (LocaleCompare("wave",option+1) == 0)
            {
              i++;
              if ((i == argc) || !IsGeometry(argv[i]))
                ThrowMogrifyException(OptionError,"MissingAmplitude",option);
              break;
            }
          if (LocaleCompare("white-point",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !IsGeometry(argv[i]))
                    ThrowMogrifyException(OptionError,"MissingPoint",option);
                }
              break;
            }
          if (LocaleCompare("white-threshold",option+1) == 0)
            {
              if (*option == '-')
                {
                  i++;
                  if ((i == argc) || !sscanf(argv[i],"%ld",&x))
                    ThrowMogrifyException(OptionError,"MissingThreshold",option);
                }
              break;
            }
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
        }
        case '?':
          break;
        default:
          ThrowMogrifyException(OptionError,"UnrecognizedOption",option)
      }
   }

   return true;
}
