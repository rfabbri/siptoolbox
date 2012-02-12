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

/* global variable across all SIP's sources */
sip_verbose_level sip_verbose = SIP_DEFAULT_VERBOSE_LEVEL;

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

/************************************************************
 * convert IplImage to SCI matrix
************************************************************/
IplImage * Mat2IplImg(int nPos)
{
  IplImage * pImg;

  int mR1, nR1, lR1;
  SciIntMat IntMat;
  int iplType;

  switch(VarType(nPos)){
  case 1: /*real or complex constant matrix.*/
    GetRhsVar(nPos, "d", &mR1, &nR1, &lR1);
    pImg = cvCreateImage(cvSize(nR1, mR1), IPL_DEPTH_64F, 1);
    /*if can not create the IplImage*/
    if(pImg == NULL)
      {
	sciprint("Create IplImage for %d'th argument failed.\r\n", nPos);
	return NULL;
      }
    MatData2ImgData(pImg, stk(lR1) );
    return pImg;
    break;

    /*integer matrix*/
  case 8: 

    GetRhsVar(nPos, "I", &mR1, &nR1, &IntMat);
    iplType = SciType2IplType(IntMat.it);
    if(iplType==0)
      {
	sciprint("This integer data type is not supported by SIVP. Integer type number: %d. \r\n", IntMat.it);
	return NULL;
      }
    pImg = cvCreateImage(cvSize(nR1, mR1),iplType , 1);
    if(pImg == NULL)
      {
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

