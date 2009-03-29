function rgbm = ntsc2rgb (yiqm)
//
// Converts an YIQ image or colormap from YIQ 
// to RGB colorspace. The YIQ model is used in NTSC
// and European TV's. It is useful for b&w and color
// compatibility, because the cromaticity (I and Q)
// and luminance (Y) are conveniently isolated.
// If yiqm is colormap, it is assumed to have 3 cols.
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $

if argn(2)~=1 then
   error('Invalid number of arguments.')
end

yiq2rgbm= [ 1  0.956    0.621
            1  -0.272   -0.647
            1  -1.105   1.702]
           
          
dims = size(yiqm)
select size(dims,'*')
case 2   // nx3 colormap
   if dims(2)<>3 then
      error('Colormap matrix must have 3 columns.')
   end
   rgbm = yiq2rgbm*yiqm'
   rgbm = rgbm'
case 3  // mxnx3 YIQ hypermatrix
   if dims(3)<>3 then
      error('RGB image must have dimentions m x n x 3.')
   end
   yiqm = matrix(yiqm,dims(1)*dims(2),3);
   rgbm = yiq2rgbm*yiqm'
   rgbm = matrix(rgbm',dims)
else
   error('Incorrect dimentions of 1st. argument.')
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
