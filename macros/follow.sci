function [x,y]=follow(Im, nhood)
//
// Extracts parametric contours of binary objects.
// "nhood" is an optional parameter, being 4 or 8
// depending if the border following is to be made 
// in 4- or 8-neighborhood sequence, yielding a 4-
// or 8-connected contour.
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// REFERENCE
//    "Shape Analysis and Classification", L. da
//    F. Costa and R. M Cesar Jr., CRC Press,
//    pp. 335-347.
//
// $Revision: 1.2 $  $Date: 2009-03-29 21:34:48 $

if argn(2)==1
   nhood = 8
end

select nhood
case 8 then
   path = [0 1; -1 1; -1 0; -1 -1; 0 -1; 1 -1; 1 0; 1 1];
   cane = 5;
   next = [2 3 4 5 6 7 8 1]; 
   invert = [5 6 7 8 1 2 3 4]
case 4 then
   path = [-1 0; 0 1; 1 0; 0 -1];
   cane = 4;
   next = [2 3 4 1]; 
   invert = [3 4 1 2]
else     
   error('Invalid neighborhood number.');
end

// converts linear index n to indices (row,col)
deff('[row,col]=index2(n,r)', ..
'col=ceil(n/r); ..
 a=n-(r*int(n/r)); ..
 row= r*(a==0)+ a*(a>0); ')

// Assume 0 outside image
[r,c]=size(Im);
aux=zeros(r+2,c+2);
aux(2:r+1,2:c+1)=Im;
Im = aux;
clear aux
i=1;
while( Im(i)<>1)
  i=i+1;
  end;
prv=i-1;
[i,j]=index2(i,r+2);


t=0; 
neighbours = zeros(path)
neighbours(:,1)=path(:,1)+i; neighbours(:,2)=path(:,2)+j;
finished=%F;
while ( ~finished ) 
  t=t+1;
  x(t)=j-2; y(t)=r-i+1;

  cini=cane; continue = %T
  while(continue) 
      cane=next(cane);
      continue = Im(neighbours(cane,1),neighbours(cane,2))==0 & cane<>cini
  end;
  if (Im(neighbours(cane,1),neighbours(cane,2))==1) then
    i = i + path(cane,1)
    j = j + path(cane,2)
    neighbours(:,1) = neighbours(:,1) + path(cane,1)
    neighbours(:,2) = neighbours(:,2) + path(cane,2)
    cane=invert(cane);
    finished=( j==x(1)+2 & i==r-y(1)+1 ); 
  else 
    finished=%T;
  end;
end;
endfunction

//
// -------------------------------------------------------------------------
// SIP - Scilab Image Processing toolbox
// Copyright (C) 2002-2009  Ricardo Fabbri
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
