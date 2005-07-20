function [poles1]=poledetection(image,threshold)

// Description:
// Poles detection (sometimes named "residues") in a wrapped phased image.
//
// Ideally, phase unwrapping should not depend on the path followed to unwrapp.
// So we consider a small loop of 4 adjacents pixels.
// We decide that there's a phase jump if the difference between
// 2 neighbours is superior to a threshold (=0.5 by default)
// We count the number of jumps when we follow the loop clockwise
// and anticlockwise.
// Because of noise, we observe that these numbers are differents in
// certain locations of the image.
// The phase is called "inconsistent".
// 
// These poles (or residues) can be marked so that the unwrapping process
// ignores them, or can be used to create branches in residue-cut tree algorithms.
//
// 
// AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// REFERENCES:
// Phase unwrapping algorithms for radar interferometry: residue-cut,
// least-squares, and synthesis algorithms
// by Zebker and Lu, Journal of Optical Society Am. A, vol 15, N°3, March 1998
//
// Satellite radar interferometry: two-dimensional phase unwrapping
// by Goldstein, Zebker, Werner
// in Radio Science, vol 23, number 4, pages 713-720, july-august 1988
//
// LICENSE: GNU GPL
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $
//


if (argn(2)==0|argn(2)>2) then   
   error("poledetection(image[,threshold])"); 
end;


if typeof(image) == 'hypermat'
   warning(' this function works only with gray level images. ...
 Use im2gray for example.')
end


if ~exists('threshold','local') then
  threshold=0.5;
end;//if exist


//init
lrsl=zeros(image);
udrc=zeros(image);
rlbl=zeros(image);
dusc=zeros(image);
poles1=zeros(image);


//left>right, same line
lrsl(1:$-1,1:$-1)=image(1:$-1,2:$)-image(1:$-1,1:$-1);
poles1(find(lrsl<-threshold))=poles1(find(lrsl<-threshold))+1;
poles1(find(lrsl>+threshold))=poles1(find(lrsl>+threshold))-1;

//up>down, right cell:
udrc(1:$-1,1:$-1)=image(2:$,2:$)-image(1:$-1,2:$);
poles1(find(udrc<-threshold))=poles1(find(udrc<-threshold))+1;
poles1(find(udrc>+threshold))=poles1(find(udrc>+threshold))-1;

//right>left, bottom line
rlbl(1:$-1,1:$-1)=image(2:$,1:$-1)-image(2:$,2:$);
poles1(find(rlbl<-threshold))=poles1(find(rlbl<-threshold))+1;
poles1(find(rlbl>+threshold))=poles1(find(rlbl>+threshold))-1;

//down>up, same cell
dusc(1:$-1,1:$-1)=image(1:$-1,1:$-1)-image(2:$,1:$-1);
poles1(find(dusc<-threshold))=poles1(find(dusc<-threshold))+1;
poles1(find(dusc>+threshold))=poles1(find(dusc>+threshold))-1;


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
