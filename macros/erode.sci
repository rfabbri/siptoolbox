function [imO]=erode(imI, se, center1)
// 
// Morphological erosion of a binary image 'imI'
// using 'se' as the structuring element.
// se:      Arbitrary structuring element represented 
//          as a binary array.
//          Defaults to: [0 1 0
//                        1 1 1 
//                        0 1 0]
// center1:  origin of structuring element.
//          Shold be within image dimensions.
//          Defaults to the center of the SE array. 
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@if.sc.usp.br>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://cyvision.if.sc.usp.br
//
// REMARKS
//    The algorithm is fully functional, but there
//    exists many better ones. The present implementation 
//    will certainly change, but the interface
//    shall remain unaltered.
//
// REFERENCE
//    "Morphological Algorithms", Luc Vincent, in
//    "Mathematical Morphology in Image Processing", 
//    Ed. Marcel Dekker, 1993.
//
// TODO
//    - confusion is made in the code about x/y r/c
//
//
// $Revision: 1.1 $  $Date: 2005-07-20 01:33:35 $
   

select argn(2)
   case 0 then
      error('Invalid number of arguments.')
   case 1 then 
      se = [0 1 0 
            1 1 1 
            0 1 0]
      center1 = [2, 2]
   case 2 then
      [rse,cse] = size(se)
      center1 = [ceil(rse/2),ceil(cse/2)];
end   

// the origin of the SE does not matter 
se(center1(1),center1(2)) = 0; 

// determine the SE coordinates.
[xse, yse] = find(se==1)
xse = xse - center1(1)
yse = yse - center1(2)
nse = size(xse,'c')   

// pad image
[rse,cse] = size(se)
[r,c]=size(imI)
row=r+rse-1
col=c+cse-1
aux = zeros(row, col)
aux(center1(1):center1(1)+r-1 , center1(2):center1(2)+c-1) = imI
imI = aux
clear aux

imO=zeros(row,col);

for i=center1(1):center1(1)+r-1, for j=center1(2):center1(2)+c-1
   if imI(i,j) == 1 then 
      aux = 1
      for k=1:nse
         aux = imI(i+xse(k), j+yse(k)) * aux
      end
      imO(i,j) = aux
   end
end,end

imO = imO(center1(1):center1(1)+r-1 , center1(2):center1(2)+c-1)

endfunction

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
