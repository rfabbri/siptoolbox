function yiqm = rgb2ntsc (rgbm)
//
// Converts an RGB image or colormap from RGB
// to YIQ colorspace. The YIQ model is used in NTSC
// and European TV's. It is useful for b&w and color
// compatibility, because the cromaticity (I and Q)
// and luminance (Y) are conveniently isolated.
// If rgbm is colormap, it is assumed to have 3 cols.
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

rgb2yiqm= [ 0.299 0.587    0.114
            0.596 -0.274   -0.322 
            0.212 -0.523   0.311]
dims = size(rgbm)
select size(dims,'*')
case 2   // nx3 colormap
   if dims(2)<>3 then
      error('Colormap matrix must have 3 columns.')
   end
   yiqm = rgb2yiqm*rgbm'
   yiqm = yiqm'
case 3  // mxnx3 rgb hypermatrix
   if dims(3)<>3 then
      error('RGB image must have dimentions m x n x 3.')
   end
   rgbm = matrix(rgbm,dims(1)*dims(2),3);
   yiqm = rgb2yiqm*rgbm'
   yiqm = matrix(yiqm',dims)
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
