function mask = mkfilter(name, opt1, opt2)
//
// Build renowned 2d filter "masks" (kernels), such as
// sobel, prewitt, mean, etc.
// 
// ORIGINAL AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
//    Laplace, sharp enhancers, low-pass, and mean by
//    Jocelyn DRUEL
//
// REFERENCES
//    "Processamento Digital de Imagens", O. M. Filho
//    and H. V. Neto, Brasport -- Rio de Janeiro, 
//    pp. 47-56.
//    "Algorithms for Image Processing and Computer Vision", 
//    J.R. Parker, Wiley, chapter 1.
//    "Cyclope's user manual" (Digital Vision).
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $
//
// Changelog:
// An error message if the number of arguments is not correct

if (argn(2)<1)|(argn(2)>3) then
  error("Invalid number of arguments: mkfilter(name[,opt1,opt2])")
end;


if 'sobel' == name
   mask = [1  2  1
           0  0  0
          -1 -2 -1]  
elseif 'prewitt' == name
   mask = [1  1  1
           0  0  0
          -1 -1 -1]  
elseif 'circular' == name
   if argn(2) < 2
      radius=1
   else
      radius=opt1
   end
   mask = zeros(2*radius+1,2*radius+1);
   radius_sq = radius*radius
   for x=-radius:radius,
      x_sq = x*x
      for y=-radius:radius,
         if x_sq + y*y <= radius_sq
            mask(x+radius+1,y+radius+1) = 1;
         end
      end;
   end;        
elseif 'circular-mean' == name
   mask=mkfilter('circular',opt1);
   mask=mask/sum(mask);
elseif 'laplace1' == name  | 'laplacian' == name
   mask = [0 -1  0
          -1  4 -1
           0 -1  0]
elseif 'laplace2' == name | 'laplacian2'==name
   mask = [-1 -1 -1
           -1  8 -1
           -1 -1 -1]
elseif 'laplace3' == name | 'laplacian3' == name
   mask = [1 -2  1
          -2  4 -2
           1 -2  1]

//sharp enhancers
elseif 'sh1' == name | 'sharp1' == name
   mask = [0 -1  0
          -1  5 -1
           0 -1  0]
elseif 'sh2' == name | 'sharp2' == name
   mask = [-1 -1 -1
           -1  9 -1
           -1 -1 -1]

//low-pass filters
elseif 'low-pass' == name | 'lowpass' == name
   mask = [1 1 1
           1 4 1
           1 1 1]
   mask=mask/sum(mask);
elseif 'mean' == name
   mask = [1 1 1
           1 1 1
           1 1 1]
   mask=mask/sum(mask);
else
   error('Invalid filter name.')
end



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
