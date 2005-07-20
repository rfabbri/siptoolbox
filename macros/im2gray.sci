function Gray = im2gray(M, map)
//
// Converts RGB images, indexed images or RGB colormaps
// to grayscale.
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@if.sc.usp.br>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://cyvision.if.sc.usp.br
//
// TODO
//    - just set  r=g=b = 0.299*r + 0.587*g + 0.114*b
//      no need to use rgb2ntsc and ntsc2rgb
//
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $

select argn(2)
case 1     // rgb image or colormap 
   dims = size(M)
   select size(dims,'*')
   case 2  // nx3 colormap
      if dims(2)<>3 then  // grayscale image: nothing to do
         Gray = M
         return
      end
      M=rgb2ntsc(M)      
      M(:,[2 3]) = 0;   // remove chromaticity
      Gray = ntsc2rgb(M)
   case 3  // mxnx3 rgb hypermatrix
      if dims(3)<>3 then
         error('RGB image must have dimentions m x n x 3.')
      end
      M=rgb2ntsc(M)      
      M(:,:,[2 3]) = 0;   // remove chromaticity
      M = ntsc2rgb(M)
      Gray = M(:,:,1)
   else 
      error('Incorrect dimentions of 1st. argument.')
   end
case 2     // indexed image
   if size(map,'c')<>3 then
      error('Colormap matrix must have 3 columns.')
   end
   [r,c]=size(M);
   map=im2gray(map)
   Gray = matrix(map(M),r,c);
else
   error('Invalid number of arguments.')
end
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
