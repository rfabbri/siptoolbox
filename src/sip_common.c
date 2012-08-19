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

#include "sip_common.h"
#include <string.h>
#include <MALLOC.h>


/* global variable across all SIP's sources */
sip_verbose_level sip_verbose = SIP_DEFAULT_VERBOSE_LEVEL;


static void myFreeRhsSVar(char **ptrStr);

bool
sci_2D_double_matrix_to_magick(char *fname, int p, int r, int c, Image *image, PixelPacket **ppix)
{

   PixelPacket  *pix;
   ExceptionInfo  exception;
   unsigned i,j;

   
   image->rows=r; image->columns=c;
   pix = SetImagePixels(image, 0, 0, (unsigned long)c, (unsigned long)r);
   if(pix == (PixelPacket *) NULL)
      SIP_MAGICK_ERROR;

   ppix = &pix;

   image->storage_class=DirectClass;
// removed for ImageMagick 6   SetImageType(image,GrayscaleType);

   for (i=0; i < (unsigned)r; i++)
      for (j=0; j < (unsigned)c; j++) {
         IndexImgByRow(pix,i,j).red   = 
            PROUND(Quantum, IndexImgByCol(stk(p),i,j)*MaxRGB);
         IndexImgByRow(pix,i,j).green = 
            PROUND(Quantum, IndexImgByCol(stk(p),i,j)*MaxRGB);
         IndexImgByRow(pix,i,j).blue  = 
            PROUND(Quantum, IndexImgByCol(stk(p),i,j)*MaxRGB);
      }

   if(!SyncImagePixels(image))
      SIP_MAGICK_ERROR;

   return true;
}

bool
sci_3D_double_hypermat_to_magick(char *fname, int nv, Image *image, PixelPacket **ppix)
{
   PixelPacket   *pix;
   ExceptionInfo  exception;
   unsigned i,j;
   HyperMat Img;

   sip_get_rhs_tru_img(nv,&Img,fname);

   assert(Img.sc.n == 3 && IC_INT32(Img.sc.D)[2] == 3);

   image->rows   = IC_INT32(Img.sc.D)[0];
   image->columns= IC_INT32(Img.sc.D)[1];
   image->storage_class=DirectClass;
   
   pix = SetImagePixels(image, 0, 0, image->columns, image->rows);
   if(pix == (PixelPacket *) NULL) 
      SIP_MAGICK_ERROR;

   *ppix = pix;
   for (i=0; i< image->rows; i++)
      for (j=0; j< image->columns; j++) {
         IndexImgByRow(pix,i,j).red   = 
            PROUND(Quantum, IndexImg3d(Img.R, i,j,0)*MaxRGB);
         IndexImgByRow(pix,i,j).green = 
            PROUND(Quantum, IndexImg3d(Img.R, i,j,1)*MaxRGB);
         IndexImgByRow(pix,i,j).blue  = 
            PROUND(Quantum, IndexImg3d(Img.R, i,j,2)*MaxRGB);
      }

   if(!SyncImagePixels(image))
      SIP_MAGICK_ERROR;

   SetImageType(image,TrueColorType);
   return true;
}

bool
sci_index_map_to_magick(char *fname, int nv, Image *image, PixelPacket **ppix)
{
   PixelPacket   *pix;
   IndexPacket   *index;
   ExceptionInfo  exception;
   unsigned i,j,imgsize;
   int rim, cim, pim, rmap, cmap, pmap;

   GetRhsVar(nv, "d", &rim, &cim, &pim);
   GetRhsVar(nv+1, "d", &rmap, &cmap, &pmap);
   if(cmap != 3)
     sip_error("colormap array must have exactly 3 columns");

   image->rows=rim;
   image->columns=cim;
   imgsize=rim*cim;
   image->storage_class=PseudoClass;
   image->colors=rmap;
   image->colormap=NULL;

   pix = SetImagePixels(image, 0, 0, image->columns, image->rows);
   if(pix == (PixelPacket *) NULL)
      SIP_MAGICK_ERROR;

   *ppix = pix;

   if((index = GetIndexes(image)) == (IndexPacket *) NULL)
      SIP_MAGICK_ERROR;

   if(AllocateImageColormap(image,image->colors) == 0)
      SIP_MAGICK_ERROR;
   
   for (i=0; i<image->colors; i++) {
      image->colormap[i].red   = 
         PROUND(Quantum, stk(pmap)[i] * MaxRGB);
      image->colormap[i].green = 
         PROUND(Quantum, stk(pmap)[i+image->colors] * MaxRGB);
      image->colormap[i].blue  = 
         PROUND(Quantum, stk(pmap)[i+2*(image->colors)] * MaxRGB);
   } 
   for (i=0; i< image->rows; i++)
      for (j=0; j< image->columns; j++) 
         IndexImgByRow(index,i,j) = PROUND(Quantum, IndexImgByCol(stk(pim),i,j))-1;
      

   SyncImage(image);

   SetImageType(image,PaletteType);
   return true;
}

/* 
 * Creates new hypermatrix structure to be further passed to scilab 
 * using sip_cre_hmat
 */
HyperMat *
new_sci_tru_img(unsigned rows, unsigned cols)
{
   char *fname="subroutine new_sci_tru_img";
   HyperMat *h;

   h = (HyperMat *) malloc(sizeof(HyperMat));
   if (!h) 
      sip_error("unable to alloc memory for the return image\n");

   h->size = 3*rows*cols;
   h->it = 0; /* real */

   h->R = (double *)malloc(h->size*sizeof(double));
   if (!h->R)
      sip_error("unable to alloc memory for the return image\n");
   h->I = NULL;

   h->sc.m = 1;
   h->sc.n = 3;
   h->sc.l = -1; /* dimensions matrix not in scilab stack */
   h->sc.it = I_INT32;
   h->sc.D = malloc(3*sizeof(pint32));
   if (!h->sc.D)
      sip_error("unable to alloc memory for the return image\n");

   IC_INT32(h->sc.D)[0] = rows;
   IC_INT32(h->sc.D)[1] = cols;
   IC_INT32(h->sc.D)[2] = 3;

   return h;
}

void
free_sci_tru_img(HyperMat **hh)
{

   HyperMat *h = *hh;
   
   free(h->sc.D);
   free(h->R);
   free (*hh);
   *hh = NULL;
}

/*-----------------------------------------
 * Cre_hmat:                                                 
 * Creates a Scilab hypermatrix (mlist)
 * from a hypermatrix structure 
 *-----------------------------------------*/
bool 
sip_cre_hmat(int pos, HyperMat *H)
{
  static char *Str[]= { "hm","dims","entries"}; int m1=1,n1=3;
  int mL=3,nL=1,lL, un=1;
  CreateVar(pos,"m", &mL, &nL, &lL);
  CreateListVarFromPtr(pos,1,"S", &m1, &n1, Str);
  CreateListVarFromPtr(pos,2,"I",&(H->sc.m), &(H->sc.n), &(H->sc));
  CreateListCVarFromPtr(pos,3,"d", &H->it,&H->size,&un ,&H->R,&H->I);
  return true;
}

/*----------------------------------------------------
 * get_rhs_hmat:                                                
 *    Creates a hypermatrix structure from a Scilab
 *    hypermatrix (mlist)   
 *----------------------------------------------------*/
bool
sip_get_rhs_hmat(int pos, HyperMat *H) 
{
  char **Str2;
  int m,n,l,m1,n1,m2,n2,m3,n3,l3,it3,lc3;
  GetRhsVar(pos,"m",&m,&n,&l);
  CheckLength(pos,m,3);
  GetListRhsVar(pos,1,"S",&m1,&n1,&Str2);
  if ( strcmp(Str2[0],"hm") != 0) {
      Scierror(999,"Argument %d is not an hypermatrix\r\n",pos);
      return false;
  }
  GetListRhsVar(pos,2,"I",&m2,&n2,&H->sc);
  GetListRhsCVar(pos,3,"d",&it3,&m3,&n3,&l3,&lc3);
  H->size = m3*n3;
  H->R = stk(l3);
  H->I = (it3==1) ? stk(lc3): NULL;
  H->it= it3;
  FreeRhsSVar(Str2);
  return true;
}

bool
sip_get_rhs_tru_img(int pos, HyperMat *h, char *fname)
{
   GetRhsHMat(pos,h);
   if (h->sc.n != 3) /* TODO function sip_read_tru_img */
      sip_error("argument 1 is not a 3D hypermatrix")
   else if (IC_INT32(h->sc.D)[2] != 3)
      sip_error("argument 1 is not a M x N x 3 hypermatrix");
   return true;
}

bool
magick_binary_image_to_double_array(char *fname, PixelPacket *pix, double **dbl_array, int rows, int cols)
{
   int i,j;
   double *imptr;

   if (sip_verbose == SIP_WORDY)
      sciprint("Binary Image\n\r");

   imptr = (double *)calloc(rows*cols, sizeof(double));
   if (!imptr)
     sip_error("unable to alloc memory\n");
   for (i=0; i < rows; i++) 
      for (j=0; j < cols; j++) 
         RCbyC(imptr,i,j,rows) = RCbyR(pix,i,j,cols).red / MaxRGB;
   *dbl_array = imptr;
   return true;
}

bool
magick_grayscale_image_to_double_array(char *fname, PixelPacket *pix, double **dbl_array, int rows, int cols)
{
   int i,j;
   double *imptr, factor = 1.0 / MaxRGB;

   if (sip_verbose == SIP_WORDY)
      sciprint("Grayscale Image\n\r");

   imptr = (double *)calloc(rows*cols, sizeof(double));
   if (!imptr)
     sip_error("unable to alloc memory\n");
   for (i=0; i < rows; i++) 
      for (j=0; j < cols; j++) 
         RCbyC(imptr,i,j,rows) = RCbyR(pix,i,j,cols).red * factor;
   *dbl_array = imptr;
   return true;
}

bool
magick_truecolor_image_to_double_hypermat(char *fname,PixelPacket *pix,HyperMat **H, int rows, int cols)
{
   int i,j;
   double factor = 1.0 / MaxRGB;
   HyperMat *h;

   if (sip_verbose == SIP_WORDY)
      sciprint("Truecolor Image\n\r");

   h = *H = new_sci_tru_img(rows,cols);
   if (!h)
      sip_error("unable to alloc memory for the return image\n");

   for (i=0; i < rows; i++)
      for (j=0; j < cols; j++) {
         RC3DbyC(h->R,i,j,0,rows,cols) = RCbyR(pix,i,j,cols).red   * factor;
         RC3DbyC(h->R,i,j,1,rows,cols) = RCbyR(pix,i,j,cols).green * factor;
         RC3DbyC(h->R,i,j,2,rows,cols) = RCbyR(pix,i,j,cols).blue  * factor;
   }

   return true;
}

bool
magick_index_map_to_sci_dbl(char *fname, Image *image, int nv)
{
   IndexPacket    *index;
   ExceptionInfo  exception;

   double *img_index, *map;
   unsigned i,j;
   double factor = 1.0 / MaxRGB;
   int rmap, cmap, rindex,cindex;

   if (sip_verbose == SIP_WORDY)
      sciprint("Indexed Image\n\r");

   if((index = GetIndexes(image)) == (IndexPacket *) NULL)
      SIP_MAGICK_ERROR;
   
   rindex = image->rows;
   cindex = image->columns;
   img_index = (double *)calloc(rindex*cindex, sizeof(double));
   if (!img_index)
      sip_error("unable to alloc memory for output image indexes");
   for (i=0; i< (unsigned)rindex; i++)
      for (j=0; j< (unsigned)cindex; j++)
         IndexImgByCol(img_index,i,j) = IndexImgByRow(index,i,j) + 1;
     
   CreateVarFromPtr(nv++, "d",&rindex,&cindex,&img_index);  
   free(img_index);
   
   rmap = image->colors; cmap = 3;
   map = (double *)calloc(rmap*3, sizeof(double));
   if (!map)
      sip_error("unable to alloc memory for output colormap");
   for (i=0; i < (unsigned) rmap; i++) {
      map[i]        = image->colormap[i].red   * factor;
      map[i+rmap]   = image->colormap[i].green * factor;
      map[i+2*rmap] = image->colormap[i].blue  * factor;
   }
   CreateVarFromPtr(nv, "d",&rmap,&cmap,&map);  /* colormap */
   
   free(map);
   return true;
}

bool
animal_grayscale_image_to_double_array(char *fname,Img *img, double **ptr) 
{
   unsigned i,npts;
   double *pt;


   npts = img->rows*img->cols;
   pt = (double *)calloc(npts, sizeof(double));
      if (!pt) sip_error("unable to alloc memory");

   for (i=0; i<npts; ++i)
      pt[i] = DATA(img)[i];

   *ptr = pt;

   return true;
}

bool
animal_grayscale_imgpuint32_to_double_array(char *fname, ImgPUInt32 *img, double **ptr) 
{
   unsigned i,npts;
   double *pt;


   npts = img->rows*img->cols;
   pt = (double *)calloc(npts, sizeof(double));
      if (!pt) sip_error("unable to alloc memory");

   for (i=0; i<npts; ++i)
      pt[i] = DATA(img)[i];

   *ptr = pt;

   return true;
}

/* ---------------------------Interface functions for SIP and Leptonica------------------------------------*/

/************************************************************
 * convert SCI 2D double matrix to PIX
************************************************************/
PIX
*sci_2D_double_matrix_to_pix(char *fname, int p, int r, int c)
{
   PIX *pix2d;
   unsigned i, j, pr,
			 pixrow,pixcolumn;
   pixrow=r;
   pixcolumn=c;
   pix2d=pixCreate(pixcolumn,pixrow,32);
   for (i=0; i < pixrow; i++)
      for (j=0; j < pixcolumn; j++) {
         pr= PROUND(Quantum, (IndexImgByColInPix(stk(p),i,j)-1)*255);
         pixSetRGBPixel(pix2d,j,i,pr,pr,pr);
       }
   return pix2d;
}
/************************************************************
 * convert SCI 3D double hypermatrix to PIX
************************************************************/
PIX
*sci_3D_double_hypermat_to_pix(char *fname, int nv)
{
   PIX * pixme;
   unsigned i,j;
   unsigned pixrow,pixcolumn;
   HyperMat Img;
   unsigned r,g,b;
   sip_get_rhs_tru_img(nv,&Img,fname);
   assert(Img.sc.n == 3 && IC_INT32(Img.sc.D)[2] == 3);
   pixrow = IC_INT32(Img.sc.D)[0];
   pixcolumn = IC_INT32(Img.sc.D)[1];
   pixme=pixCreate(pixcolumn,pixrow,32);
   for (i=0; i< pixrow; i++)
      for (j=0; j< pixcolumn; j++) {
         r=PROUND(Quantum, IndexImg3dInPix(Img.R, i,j,0)*(255));
         g=PROUND(Quantum, IndexImg3dInPix(Img.R, i,j,1)*(255));
         b=PROUND(Quantum, IndexImg3dInPix(Img.R, i,j,2)*(255));
         pixSetRGBPixel(pixme,j,i,r,g,b);
      }
   return pixme;
}
/************************************************************
 * convert SCI index map to PIX
************************************************************/
PIX
*sci_index_map_to_pix(char *fname, int nv)
{
   PIX *pixs3; //Pix defined
   unsigned i,j,imgsize,
             pixrow,pixcolumn,pixcolors,
             prval,pgval,pbval,a;
   int rim, cim, pim, rmap, cmap, pmap;

   GetRhsVar(nv, "d", &rim, &cim, &pim);
   GetRhsVar(nv+1, "d", &rmap, &cmap, &pmap);
   if(cmap != 3)
     sip_error("colormap array must have exactly 3 columns");
   pixrow=rim;
   pixcolumn=cim;
   pixcolors=rmap;
   pixs3=pixCreate(pixcolumn,pixrow,32);
   unsigned p[pixcolors][3];

   for (i=0; i<pixcolors; i++) {
     p[i][0] = PROUND(Quantum, stk(pmap)[i] * 255);
     p[i][1] = PROUND(Quantum, stk(pmap)[i+pixcolors] * 255);
     p[i][2] = PROUND(Quantum, stk(pmap)[i+2*(pixcolors)] * 255);
    }


   for (i=0; i< pixrow; i++)
      for (j=0; j< pixcolumn; j++)
         {
			 a = PROUND(Quantum, IndexImgByColInPix(stk(pim),i,j))-1;
             prval=p[a][0];
             pgval=p[a][1];
             pbval=p[a][2];
             pixSetRGBPixel(pixs3,j,i,prval,pgval,pbval);
         }

   return pixs3;
}
/************************************************************
 * convert SCI 2D gray double matrix to PIX
************************************************************/
PIX
*sci_2D_gray_double_matrix_to_pix(char *fname, int p, int r, int c)
{
   PIX *pix2d;
   unsigned i, j, pr,
			 pixrow,pixcolumn;
   pixrow=r;
   pixcolumn=c;
   pix2d=pixCreate(pixcolumn,pixrow,32);
   for (i=0; i < pixrow; i++)
      for (j=0; j < pixcolumn; j++) {
         pr= PROUND(Quantum, (IndexImgByColInPix(stk(p),i,j))*255);
         pixSetRGBPixel(pix2d,j,i,pr,pr,pr);
       }
   return pix2d;
}
/************************************************************
 * convert PIX to SCI 2D double matrix
************************************************************/
bool
pix_binary_image_to_double_array(char *fname, PIX *pixme, double **dbl_array, int rows, int cols)
{
   int i,j;
   double *imptr;
   l_int32 pr,pg,pb;

   if (sip_verbose == SIP_WORDY)
      sciprint("Binary Image\n\r");

   imptr = (double *)calloc(rows*cols, sizeof(double));
   if (!imptr)
     sip_error("unable to alloc memory\n");
   for (i=0; i < rows; i++)
      for (j=0; j < cols; j++)
      {
		 pixGetPixel(pixme,j,i,&pr);
	     RCbyCInPix(imptr,i,j,rows) = ((pr+1)/255)*(-1);
	 }
   *dbl_array = imptr;
   return true;
}
/************************************************************
 * convert PIX truecolor image to SCI 3D double hypermatrix
************************************************************/
bool
pix_truecolor_image_to_double_hypermat(char *fname, PIX *pixme, HyperMat **H, int rows, int cols)
{
   int i,j;
   double factor = 1.0 / MaxRGB;
   HyperMat *h;
   l_int32 pr,pg,pb;
   double prval,pgval,pbval;

   if (sip_verbose == SIP_WORDY)
      sciprint("Truecolor Image\n\r");

   h = *H = new_sci_tru_img(rows,cols);
   if (!h)
      sip_error("unable to alloc memory for the return image\n");

   for (i=0; i < rows; i++)
      for (j=0; j < cols; j++) {
		pixGetRGBPixel(pixme,j,i,&pr,&pg,&pb);
		prval= (unsigned)pr;
        pgval= (unsigned)pg;
        pbval= (unsigned)pb;

        RC3DbyCInPix(h->R,i,j,0,rows,cols) = prval/255;
        RC3DbyCInPix(h->R,i,j,1,rows,cols) = pgval/255;
        RC3DbyCInPix(h->R,i,j,2,rows,cols) = pbval/255;
   }
   return true;
}
/************************************************************
 * convert PIX index map to SCI
************************************************************/
bool
pix_index_map_to_sci_dbl(char *fname, PIX *pixme, int nv)
{
   PIXCMAP *cmapl;
   int pixrow,pixcolumn,
       rmap, cmap, rindex,cindex;
   unsigned i,j,prval,
       pgval,pbval,count;
   double ppr,ppg,ppb,*img_index, *map;
   l_int32 pindex, ival, rval,
           gval, bval,pr,pg,pb;

   if (sip_verbose == SIP_WORDY)
      sciprint("Indexed Image\n\r");

   if (pixme  == NULL)
	  return sciprint("pixs not made");

   pixrow = pixGetHeight(pixme);
   pixcolumn  = pixGetWidth(pixme);
   rindex =(unsigned)pixrow;
   cindex =(unsigned)pixcolumn;
   cmapl=pixGetColormap(pixme);
   count = pixcmapGetCount(cmapl);
   img_index = (double *)calloc(rindex*cindex, sizeof(double));

   if (!img_index)
      sip_error("unable to alloc memory for output image indexes");
   for (i=0; i< (unsigned)rindex; i++)
      for (j=0; j< (unsigned)cindex; j++)
      {
	    pixGetPixel(pixme,j,i,&pindex);
        IndexImgByColInPix(img_index,i,j) = (double)pindex +1;
	  }

   CreateVarFromPtr(nv++, "d",&rindex,&cindex,&img_index);
   free(img_index);

   rmap = count; cmap = 3;
   map = (double *)calloc(rmap*3, sizeof(double));
   if (!map)
      sip_error("unable to alloc memory for output colormap");
   for (i=0; i < (unsigned) rmap; i++) {
	  pixcmapGetColor(cmapl, i, &rval, &gval, &bval);
      ppr=(double)rval;
      ppg=(double)gval;
      ppb=(double)bval;
      map[i]        = ppr/255;
      map[i+rmap]   = ppg/255;
      map[i+2*rmap] = ppb/255;
   }
   CreateVarFromPtr(nv, "d",&rmap,&cmap,&map);  /* colormap */

   free(map);
   return true;
}

/************************************************************
 * convert PIX Gray to SCI 2D double matrix
************************************************************/
bool
pix_gray_image_to_double_array(char *fname, PIX *pixme, double **dbl_array, int rows, int cols)
{
   int i,j;
   double *imptr,prval;
   l_int32 pr,pg,pb;
   if (sip_verbose == SIP_WORDY)
      sciprint("Gray scale image\n\r");

   imptr = (double *)calloc(rows*cols, sizeof(double));
   if (!imptr)
     sip_error("unable to alloc memory\n");
   for (i=0; i < rows; i++){
      for (j=0; j < cols; j++)
      {
		 pixGetRGBPixel(pixme,j,i,&pr,&pg,&pb);
		 prval= (unsigned)pr;
		 RCbyCInPix(imptr,i,j,rows) = (double)prval/255;
	 }
	}
   *dbl_array = imptr;
   return true;
}

/* ---------------------------End of Interface functions for SIP and Leptonica----------------------------------*/

#ifdef SIP_HAVE_OPENCV  /* ----------------------------------------------------------------------*/

/************************************************************
 * convert IplImage to SCI matrix
************************************************************/
IplImage * 
Mat2IplImg(int nPos)
{
  IplImage * pImg;

  int mR1, nR1, lR1;
  SciIntMat IntMat;
  int iplType;

  switch(VarType(nPos)){
  case 1: /*real or complex constant matrix.*/
    GetRhsVar(nPos, "d", &mR1, &nR1, &lR1);
    pImg = cvCreateImage(cvSize(nR1, mR1), IPL_DEPTH_64F, 1);
    /* if cant create the IplImage */
    if(pImg == NULL) {
      sciprint("Create IplImage for %d'th argument failed.\r\n", nPos);
      return NULL;
    }
    MatData2ImgData(pImg, stk(lR1) );
    return pImg;
    break;

  case 8: /*integer matrix*/

    GetRhsVar(nPos, "I", &mR1, &nR1, &IntMat);
    iplType = SciType2IplType(IntMat.it);
    if(iplType==0) {
      sciprint("This integer data type is not supported by SIP. Integer type number: %d. \r\n", IntMat.it);
      return NULL;
    }
    pImg = cvCreateImage(cvSize(nR1, mR1),iplType , 1);
    if(pImg == NULL) {
      sciprint("Create IplImage for %d'th argument failed.\r\n", nPos);
      return NULL;
    }
    MatData2ImgData(pImg, IntMat.D );
    return pImg;
    break;

  case 17:
    return CreateIplImgFromHm( nPos);
    break;
  default:
    sciprint("This data type can't be converted to an image.\r\n");
    return NULL;
  }
}

/************************************************************
 * change the data order from column-wise to row-wise
 ************************************************************/
int 
MatData2ImgData(IplImage * pImage, void * pMatData)
{
  //  IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U,
  //IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F 
  int row, col, ch;
  long nCount = 0;
  int nBytes;

  char * pDst = NULL;
  char * pSrc = NULL;

  if (pImage == NULL || pMatData == NULL)
    return FALSE;

  pDst = (char*)(pImage->imageData);
  pSrc = (char*)pMatData;

  /*how many bytes per pixel per channel*/
  nBytes = pImage->depth;
  if (nBytes > IPL_DEPTH_SIGN)
    nBytes -= IPL_DEPTH_SIGN;
  nBytes = nBytes >> 3;

  for(ch = 0; ch < pImage->nChannels ; ch++) //the order of IplImage is BGR
    for(col =0; col < pImage->width; col++)
      for(row = 0; row < pImage->height; row++)
	{
	  memcpy(pDst + pImage->widthStep*row + (col*pImage->nChannels + (pImage->nChannels-ch-1))*nBytes, pSrc+nCount, nBytes );
	  nCount += nBytes;
	}


  return TRUE;
}

/************************************************************
 * change the data order from row-wise to column-wise
 ************************************************************/
int 
ImgData2MatData(IplImage * pImage, void * pMatData)
{
  //  IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U,
  //IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F 
  int row, col, ch;
  long nCount = 0;
  int nBytes;

  char * pSrc = NULL;
  char * pDst = NULL;

  if (pImage == NULL || pMatData == NULL)
    return FALSE;

  pSrc = (char*)(pImage->imageData);
  pDst = (char*)pMatData;

  /*how many bytes per pixel per channel*/
  nBytes = pImage->depth;
  if (nBytes > IPL_DEPTH_SIGN)
    nBytes -= IPL_DEPTH_SIGN;
  nBytes = nBytes >> 3;


  for(ch = 0; ch < pImage->nChannels ; ch++) //the order of IplImage is BGR
    for(col =0; col < pImage->width; col++)
      for(row = 0; row < pImage->height; row++)
	{
	  memcpy(pDst+nCount, pSrc + pImage->widthStep*row + (col*pImage->nChannels + (pImage->nChannels-ch-1))*nBytes, nBytes );
	  nCount += nBytes;
	}


  return TRUE;
}



/************************************************************
 * get image dimension information and the data address
 * input: nPos(the nPos'th  argument, the  argument shouble be a mlist)
 * 
************************************************************/
IplImage * 
CreateIplImgFromHm(int nPos)
{
  char ** pStr;
  int * pListHeader;
  int * pDataHeader;
  void * pData;
  int m1, n1, m2, n2, m3, n3, l3;
  SciIntMat Dims;
  SciIntMat IntMat;
  
  int nWidth, nHeight, nCh=1;
  int iplType;
  IplImage * pImg = NULL;

  GetListRhsVar(nPos, 1 ,"S",&m1,&n1,&pStr);
  /*check whether the the argument is a hypermatrix*/
  if( m1 !=1 || n1 !=3)
    goto NOT_HM;
  if( strcmp(pStr[0], "hm") != 0 || strcmp(pStr[1], "dims") != 0 || strcmp(pStr[2], "entries") != 0)
    goto NOT_HM;
  
  /*get the dimension information, it's stored in the second element of the mlist*/

  GetListRhsVar(nPos,2,"I", &m2, &n2,&Dims);
  if( m2*n2 !=2 && m2*n2 !=3)
    goto NOT_HM;

  nHeight = IC_INT32(Dims.D)[0];
  nWidth  = IC_INT32(Dims.D)[1];
  if(m2*n2 == 3)
    nCh = IC_INT32(Dims.D)[2];
  else
    nCh = 1;


  /*get mlist data structure*/
  pListHeader = (int*)GetData(nPos);
  /*data is stored in the third element*/
  /*data struct is get first*/
  pDataHeader = (int*)( ((char*)pListHeader) + 24 + (pListHeader[4]-1)*8 );
  
  /*the next step is to get the data type and the data*/
  /*the data type is store in the first 4 bytes as a integer*/
  switch( pDataHeader[0]  ){
  case 1:  /*if the data is real*/
    iplType = IPL_DEPTH_64F;
    GetListRhsVar(nPos, 3, "d", &m3, &n3, &l3);
    pData = stk(l3);
    break;
  case 8:  /*integer*/
    GetListRhsVar(nPos, 3, "I", &m3, &n3, &IntMat);
    m3 = IntMat.m;
    n3 = IntMat.n;
    iplType = SciType2IplType(IntMat.it);
    if(iplType==0)
      {
	sciprint("This integer data type is not supported by SIVP. Integer type number: %d. \r\n", IntMat.it);
	goto EXIT_TAG;
      }
    pData = IntMat.D;
    break;
  default:
    sciprint("The data type of %d'th argument is %d. It can't be converted to an image.\r\n",nPos, pDataHeader[0] );
    goto EXIT_TAG;
  }

  /*check dimension */
  if(m3*n3 != nWidth * nHeight * nCh)
    {
      sciprint("Broken hypermatrix: The hypermatrix declares %d X %d X %d, but actually %d elements.\r\n", nHeight, nWidth, nCh, m3*n3);
      goto EXIT_TAG;
    }

  /*create a IplImage to receiving the data*/
  pImg = cvCreateImage(cvSize(nWidth, nHeight), iplType, nCh);
  if(pImg == NULL)
  {
    sciprint("Create IplImage for %d'th argument failed.\r\n", nPos);
    goto EXIT_TAG;
  }

  /*change data order and copy data*/
  MatData2ImgData(pImg, pData );

  
  myFreeRhsSVar(pStr);
  return pImg;

 NOT_HM:
  sciprint("The %d'th argument is not a hypermatrix.\r\n", nPos);
  myFreeRhsSVar(pStr);
  return NULL;
 EXIT_TAG:
  myFreeRhsSVar(pStr);
  return NULL;
  
}


/************************************************************
 * convert IplImage to SCI matrix
************************************************************/
int 
IplImg2Mat(IplImage * pImage, int nPos)
{
  void * pMatData;
  int nBytes;
  int nType;
  
  if(pImage == NULL)
    return FALSE;

  //if bottom-left origin
  if(pImage->origin==1)
    {
      cvFlip(pImage, NULL, 0);
      pImage->origin=0;
    }

  /*how many bytes per pixel per channel*/
  nBytes = pImage->depth;
  if (nBytes > IPL_DEPTH_SIGN)
    nBytes -= IPL_DEPTH_SIGN;
  nBytes = nBytes >> 3;
  
  /*alloc memory for matrix data*/
  pMatData = malloc(pImage->width * pImage->height * pImage->nChannels * nBytes);
  if(pMatData == NULL)
    return FALSE;

  ImgData2MatData(pImage, pMatData);

  /*convert IplImage data type to scilab data type*/
  nType = IplType2SciType(pImage->depth);
  if (nType <= 0)
    {
      free(pMatData);
      return FALSE;
    }
  /*create matrix for scilab*/
  if(pImage->nChannels == 1)
    {
      switch(nType){
      case I_CHAR:
      case I_UCHAR:
      case I_INT16:
      case I_UINT16:
      case I_INT32:
        Create2DIntMat(nPos, pImage->height, pImage->width, pMatData, nType);
        break;
      case SIVP_FLOAT:
        Create2DFloatMat(nPos,pImage->height, pImage->width, (float*)pMatData);
        break;
      case SIVP_DOUBLE:
        Create2DDoubleMat(nPos,pImage->height, pImage->width, (double*)pMatData);
        break;
      }
    }
  else
    {
      switch(nType){
      case I_CHAR:
      case I_UCHAR:
      case I_INT16:
      case I_UINT16:
      case I_INT32:
        Create3DIntMat(nPos, pImage->height, pImage->width, pImage->nChannels, pMatData, nType);
        break;
      case SIVP_FLOAT:
        Create3DFloatMat(nPos,pImage->height, pImage->width, pImage->nChannels, (float*)pMatData);
        break;
      case SIVP_DOUBLE:
        Create3DDoubleMat(nPos,pImage->height, pImage->width, pImage->nChannels, (double*)pMatData);
        break;
      }
    }

  /*free matrix data*/
  free(pMatData);
  return TRUE;
}

/************************************************************
 * unsigned int32 is not supported (no this type in OpenCV)
 ************************************************************/
int 
IplType2SciType(int IplType)
{
  switch(IplType)    {
  case IPL_DEPTH_8U:  return I_UCHAR;
  case IPL_DEPTH_8S:  return I_CHAR;
  case IPL_DEPTH_16U: return I_UINT16;
  case IPL_DEPTH_16S: return I_INT16;
  case IPL_DEPTH_32S: return I_INT32;
  case IPL_DEPTH_32F: return SIVP_FLOAT;
  case IPL_DEPTH_64F: return SIVP_DOUBLE;
  default: return 0;
  }
}


/************************************************************
 * unsigned int32 is not supported (no this type in OpenCV)
 ************************************************************/
int 
SciType2IplType(int SciType)
{
  switch(SciType)    {
  case I_UCHAR:  return IPL_DEPTH_8U;
  case I_CHAR:   return IPL_DEPTH_8S;
  case I_UINT16: return IPL_DEPTH_16U;
  case I_INT16:  return IPL_DEPTH_16S;
  case I_INT32:  return IPL_DEPTH_32S;
  case SIVP_FLOAT:  return IPL_DEPTH_32F;
  case SIVP_DOUBLE: return IPL_DEPTH_64F;
  default: return 0;
  }
}

/**************************************************
  In Scilab 5.2.0, I can't find function FreeRhsSVar(),
  so I use this function to replace it.
  The function is from modules/intersci/src/lib/libinter.c
 **************************************************/
void myFreeRhsSVar(char **ptrStr)
{
  int i=0;

  if (ptrStr)
  {
    while ( ptrStr[i] != NULL)
    {
      FREE(ptrStr[i]);
      i++;
    };
    FREE(ptrStr);
    ptrStr = NULL;
  }
}

/**************************************************
 * nRow: the first dim
 * nCol: the second dim
 * pData: pData is the data for matrix, and it can be freed after this function is called.
 * nType: I_CHAR, I_INT16, I_INT32, I_UCHAR,  I_UINT16,  I_UINT32
 **************************************************/
int
Create2DIntMat(int nPos, int nRow, int nCol, void * pData, int nType)
{
  SciIntMat IntMat;

  //create SciIntMat for the image data
  IntMat.m = nRow;
  IntMat.n = nCol;
  IntMat.l = -1; /* dimensions matrix not in scilab stack */
  IntMat.it = nType;
  IntMat.D = pData;

  CreateVarFromPtr(nPos,"I",&(IntMat.m), &(IntMat.n), &IntMat ); 
  return TRUE;
}


/**************************************************
 * Create a 2D nRow * nCol float matrix(single precision, 4bytes)
 * nRow: the first dim
 * nCol: the second dim
 * pData: pData is the data for matrix, and it can be freed after this function is called.
 **************************************************/
int
Create2DFloatMat(int nPos, int nRow, int nCol, float * pData)
{
  CreateVarFromPtr(nPos,"r",&nRow, &nCol, &pData); 
  return TRUE;
}

/**************************************************
 * Create a 2D nRow * nCol double matrix(double precision, 8bytes)
 * nRow: the first dim
 * nCol: the second dim
 * pData: pData is the data for matrix, and it can be freed after this function is called.
 **************************************************/
int
Create2DDoubleMat(int nPos, int nRow, int nCol, double * pData)
{
  CreateVarFromPtr(nPos,"d",&nRow, &nCol, &pData); 
  return TRUE;
}

/**************************************************
 * Create a 3D nRow * nCol * nCh integer matrix
 * nRow: the first dim
 * nCol: the second dim
 * nCh:  the third dim
 * pData: pData is the data for matrix, and it can be freed after this function is called.
 * nType: I_CHAR, I_INT16, I_INT32, I_UCHAR,  I_UINT16,  I_UINT32
 **************************************************/
int 
Create3DIntMat(int nPos, int nRow, int nCol, int nCh, void* pData, int nType)
{
  static char *Str[]= { "hm","dims","entries"}; 
  int m1=1,n1=3;
  int mL=3,nL=1,lL, un=1;

  SciIntMat Dims;
  SciIntMat IntData;

  //create SciIntMat for dimentional information
  Dims.m = 1;
  Dims.n = 3;
  Dims.l = -1; /* dimensions matrix not in scilab stack */
  Dims.it = I_INT32;
  Dims.D = malloc(Dims.n * sizeof(int)); //dim data
  if (!Dims.D)
  {
    Scierror(999,"Unable to alloc memory for the image\n");
    return FALSE;
  }

  IC_INT32(Dims.D)[0] = nRow;
  IC_INT32(Dims.D)[1] = nCol;
  IC_INT32(Dims.D)[2] = nCh;

  //create SciIntMat for the image data
  IntData.m = nRow ;
  IntData.n = nCol * nCh;
  IntData.l = -1; /* dimensions matrix not in scilab stack */
  IntData.it = nType;
  IntData.D = pData;

  CreateVar(nPos,"m", &mL, &nL, &lL);
  CreateListVarFromPtr(nPos,1,"S", &m1, &n1, Str);
  CreateListVarFromPtr(nPos,2,"I",&(Dims.m), &(Dims.n), &Dims); 
  CreateListVarFromPtr(nPos,3,"I",&(IntData.m), &(IntData.n), &IntData ); 

/*   free memory */
  free(Dims.D);

  return TRUE;
}

/**************************************************
 * Create a 3D nRow * nCol * nCh float matrix (single precision, 4 bytes)
 * nRow: the first dim
 * nCol: the second dim
 * nCh:  the third dim
 * pData: pData is the data for matrix, and it can be freed after this function is called.
 **************************************************/
int 
Create3DFloatMat(int nPos, int nRow, int nCol, int nCh, float* pData)
{
  static char *Str[]= { "hm","dims","entries"}; 
  int m1=1,n1=3;
  int mL=3,nL=1,lL;
  int un=1;
  int nSize = nRow * nCol * nCh;

  SciIntMat Dims;

  //create SciIntMat for dimentional information
  Dims.m = 1;
  Dims.n = 3;
  Dims.l = -1; /* dimensions matrix not in scilab stack */
  Dims.it = I_INT32;
  Dims.D = malloc(Dims.n * sizeof(int)); //dim data
  if (!Dims.D)
  {
    Scierror(999,"Unable to alloc memory for the image\n");
    return FALSE;
  }

  IC_INT32(Dims.D)[0] = nRow;
  IC_INT32(Dims.D)[1] = nCol;
  IC_INT32(Dims.D)[2] = nCh;


  CreateVar(nPos,"m", &mL, &nL, &lL);
  CreateListVarFromPtr(nPos,1,"S", &m1, &n1, Str);
  CreateListVarFromPtr(nPos,2,"I",&(Dims.m), &(Dims.n), &Dims); 
  CreateListVarFromPtr(nPos,3,"r", &nSize, &un, &pData );

/*   free memory */
  free(Dims.D);

  return TRUE;
}

/**************************************************
 * Create a 3D nRow * nCol * nCh double matrix (double precision, 8 bytes)
 * nRow: the first dim
 * nCol: the second dim
 * nCh:  the third dim
 * pData: pData is the data for matrix, and it can be freed after this function is called.
 **************************************************/
int 
Create3DDoubleMat(int nPos, int nRow, int nCol, int nCh, double* pData)
{
  static char *Str[]= { "hm","dims","entries"}; 
  int m1=1,n1=3;
  int mL=3,nL=1,lL;
  int un=1;
  int nSize = nRow * nCol * nCh;

  SciIntMat Dims;

  //create SciIntMat for dimentional information
  Dims.m = 1;
  Dims.n = 3;
  Dims.l = -1; /* dimensions matrix not in scilab stack */
  Dims.it = I_INT32;
  Dims.D = malloc(Dims.n * sizeof(int)); //dim data
  if (!Dims.D)
  {
    Scierror(999,"Unable to alloc memory for the image\n");
    return FALSE;
  }

  IC_INT32(Dims.D)[0] = nRow;
  IC_INT32(Dims.D)[1] = nCol;
  IC_INT32(Dims.D)[2] = nCh;


  CreateVar(nPos,"m", &mL, &nL, &lL);
  CreateListVarFromPtr(nPos,1,"S", &m1, &n1, Str);
  CreateListVarFromPtr(nPos,2,"I",&(Dims.m), &(Dims.n), &Dims); 
  CreateListVarFromPtr(nPos,3,"d", &nSize, &un, &pData );

/*   free memory */
  free(Dims.D);

  return TRUE;
}





#endif /* SIP_HAVE_OPENCV ----------------------------------------------------------------------*/

