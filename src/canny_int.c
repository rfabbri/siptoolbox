/*
 * -------------------------------------------------------------------------
 * SIP - Scilab Image Processing toolbox
 * Copyright (C) 2002-2009  Ricardo Fabbri
 *
 * Code merged back from SIVP
 * Copyright (C) 2005  Shiqi Yu
 * Copyright (C) 2005  Vincent Etienne
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

#include <opencv2/imgproc/imgproc_c.h>

int canny_int(char *fname)
{
  static int l2, m2, n2, l3, m3, n3, l4, m4, n4;

  //create variable for parameters
  double * param1=NULL;
  double * param2=NULL;
  int * param3=NULL;

  IplImage * img1 = NULL;
  IplImage * img2 = NULL;
  IplImage * imgTmp = NULL;

  //check the number of in/out parametres
  Rhs=Max(Lhs,Rhs);

  CheckRhs(4,4);
  CheckLhs(1,1);

  //check the nature of parametres
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  GetRhsVar(4, "i", &m4, &n4, &l4);


  // check if arg are scalaire
  if (m2*n2 != 1 || m3*n3 != 1 || m4*n4 != 1) 
      {
        sciprint("%s Error: arguments must be scalars\r\n", fname);
        return 0;
      }

  //receive the data
  param1 =  stk(l2);
  param2 =  stk(l3);
  param3 =  istk(l4);

  //load the input image for cvcanny
  img1=Mat2IplImg(1);

  // check if input image is correctly loaded
  if(img1==NULL)
    {
     sciprint("%s Error: can't read the input image\r\n", fname);
     return 0;
    }

   img2 = cvCreateImage(cvGetSize(img1),  IPL_DEPTH_8U, 1);
   if(img2==NULL)
     {
       cvReleaseImage( &img1 );
       sciprint("%s Error: can't create the output matrix\r\n", fname);
       return 0;
     }


   //convert the input to UING8 Gray image
   if(img1->depth != IPL_DEPTH_8U)
     {
       imgTmp = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, img1->nChannels);
       if(imgTmp==NULL)
         {
     cvReleaseImage( &img1 );
     cvReleaseImage( &img2 );
     sciprint("%s Error: can't create the output matrix\r\n", fname);
     return 0;
         }

       cvConvert(img1, imgTmp);
       cvReleaseImage(&img1);
       img1 = imgTmp;
       imgTmp = NULL;
     }

   //onvert the image to 1 channel image
   if(img1->nChannels != 1)
     {
       imgTmp = cvCreateImage(cvGetSize(img1), IPL_DEPTH_8U, 1);
       if(imgTmp==NULL)
         {
     cvReleaseImage( &img1 );
     cvReleaseImage( &img2 );
     sciprint("%s Error: can't create the output matrix\r\n", fname);
     return 0;
         }

       cvCvtColor(img1, imgTmp, CV_BGR2GRAY);
       cvReleaseImage(&img1);
       img1 = imgTmp;
       imgTmp = NULL;
     }


   cvCanny( img1, img2 ,param1[0],param2[0],param3[0]);

   //transform the result of opencv canny in a matrice
   IplImg2Mat(img2,5);

   //send the result
   LhsVar(1)=5;


  /*
  //create the output image for cv canny 
  IplImage* img2 = NULL;
  img2=cvCreateImage(cvGetSize(img1),img1->depth,img1->nChannels);

  // check if the output image is correctly loaded
  if(img2==NULL)
    {
      cvReleaseImage( &img1 );

     sciprint("%s Error: can't create the output matrix\r\n", fname);
     return 0;
    }

   if((img1->depth==IPL_DEPTH_8U) && (img1->nChannels==1))
    {


     //use the opencv function
     cvCanny( img1, img2 ,param1[0],param2[0],param3[0]);

     //transform the result of opencv canny in a matrice
     IplImg2Mat(img2,5);

     //send the result
     LhsVar(1)=5;



    }
  else
   {
    sciprint("%s Error: wrong input matrice\r\n", fname);
    return 0;
   }  
  */

  //let's free the memory
   cvReleaseImage( &img1 );
   cvReleaseImage( &img2 );

  return 0;
}
