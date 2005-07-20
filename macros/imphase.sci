function phase_value = imphase(formula,threshold,i1,i2,i3,i4,i5)
//
// DESCRIPTION:
// image phase calculation.
// This function is used in profilometry (or in interferometry) to calculate
// the phase on any point of an image:
// Fringes are projected on an object.
// A 1st image is taken with a CCD camera.
// fringes are deplaced from a quarter of an interfringe 
// (in this case, phase shift = %pi/2)
// A 2nd image is taken. And so on...
// The phased image is calculated from these images.
// Several formulas exist depending on
// a) the phase shift (often (2*%pi/3) or (%pi/2)
// b) the number of images available 
// using five images (when it is possible) should lead
// to better phase calculation.
// 
// Note about Object mask: 
// If the value of a pixel doesn't vary more than the threshold
// we consider that this pixel is in the background
// and we give it the phase = 0 radian.
//
// REFERENCES:
// Software FRINGE ANALYSIS by HOLO3 (St Louis, FRANCE)
// Software INTELLIWAVE by engsynthesis
// Optics 505 - James C.Wyant
// "Modélisation de forme 3D par méthode de moiré de projection
// et analyse par décalage de phases", 
// Cyril Breque & Fabrice Bremand
// "Metrologie optique par decalage de phase",
// Yves Surrel,
// conservatoire national des arts et metiers
// 
// A google search with keywords: phase shifting interferometry or 
// moire or phase unwrapping will found good introductory documents
// on the subject
// 
//
// AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// LICENSE: GNU GPL
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $



if argn(2)==0 then
  error("imphase(formula_name,threshold,im1,im2,im3[,im4,im5])")
end;

select formula

  // Bucket 3A
  // phase shift is 90 degres
  // usage: general purpose (vibration environnements)
  case 'bucket3a' then
    if argn(2)<>5 then
  error("wrong number of arguments: imphase(''bucket3a'',threshold,im1,im2,im3)")
end;

n=(i3-i2);//numerator
d=(i1-i2);//denuminator
  
  
  // Bucket 3B
  // phase shift is 120 degres
  // usage: general purpose (vibration environnements)
  case 'bucket3b' then
    if argn(2)<>5 then
  error("wrong number of arguments: imphase(''bucket3b'',threshold,im1,im2,im3)")
end;

//phase between images
p1=2*(%pi/3);
p2=2*p1;

n=(i3-i2)+(i1-i3)*cos(p1)+(i2-i1)*cos(p2);//numerator
d=(i1-i3)*sin(p1)+(i2-i1)*sin(p2);//denuminator


  
  // Bucket 3C
  // phase shift is 120 degres
  // usage: general purpose 
  case 'bucket3c' then
    if argn(2)<>5 then
  error("wrong number of arguments: imphase(''bucket3c'',threshold,im1,im2,im3)")
end;

n=sqrt(3)*(i3-i2);//numerator
d=(2*i1-i2-i3);//denuminator
  
  
  // Bucket 4A
  // phase shift is 90 degres
  // usage: general purpose
  case 'bucket4a' then
    if argn(2)<>6 then
  error("wrong number of arguments: imphase(''bucket4a'',threshold,im1,im2,im3,im4)")
end;

n=(i4-i2);//numerator
d=(i1-i3);//denuminator



  // Bucket 5A (hariharan)
  // phase shift is 90 degres
  // usage: phase shift correction
  case 'bucket5a' then
    if argn(2)<>7 then
  error("wrong number of arguments: imphase(''bucket5a'',threshold,im1,im2,im3,im4,im5)")
end;

n=2*(i4-i2);//numerator
d=(i1-2*i3+i5);//denuminator


  // Bucket 5B (Creath/Schmit)
  // phase shift is 90 degres
  // usage: enhanced phase shift error correction
  case 'bucket5b' then
    if argn(2)<>7 then
  error("wrong number of arguments: imphase(''bucket5b'',threshold,im1,im2,im3,im4,im5)")
end;

n=(i1-4*i2+4*i4-i5);//numerator
d=(i1+2*i2-6*i3+2*i4+i5);//denuminator




else
  error('Invalid formula name.')
end;



//phase calculation:
d(find(d==0))=1e-8; //to avoid division by zero

//object mask
n(find((abs(i1-i2)<threshold)&(abs(i1-i3)<threshold)&(abs(i2-i3)<threshold)))=0;//end of the mask
//phase calculation:
phase_value=atan(n,d);//in general we use arctan2 to determine the phase
// of a point. Arctan2 gives a value between (-%pi) and (+%pi)
// whereas this formulation: phase=atan(n./d) would return
// a result between (-%pi/2) and (+%pi/2)



endfunction;
//
// -------------------------------------------------------------------------
// SIP - Scilab Image Processing toolbox
// Copyright (C) 2002-2004  Ricardo Fabbri
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// -------------------------------------------------------------------------
//
