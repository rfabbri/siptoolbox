function [border, thresh]=edge(Img, method, thresh, direction, sigma)
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// REFERENCE
//    "Algorithms for Image Processing and Computer Vision", 
//    J.R. Parker, Wiley, chapter 1.
//
// TODO
//    - "edge" routine should have parameters "same" and "valid", just
//    like imconv.
// 
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $

if argn(2)==0 then
   error('Invalid number of arguments.')
else
   if ~exists('method','local') then
      method='sobel'
   end
   if ~exists('thresh','local') then
      thresh=0.5;
   end
   if ~exists('direction','local') then
      direction='both'
   end
end

select method
case 'sobel' then
   mask = mkfilter('sobel')
case 'prewitt' then
   mask = mkfilter('prewitt')
case 'fftderiv' // fourier gradient
   if ~exists('sigma','local') then
      sigma=1
   end
   [r,c] = size(Img)
   fu = [0:c-1]*(1/c)
   fv = [0:r-1]*(1/r)
   fu(int(c/2):c) = fu(int(c/2):c) - 1;
   fv(int(r/2):r) = fv(int(r/2):r) - 1;
   
   if sigma == 0
      gf = ones(r,c)
   else
      gf = ones(r,1) * exp(-(sigma*%pi*fu)^2)
      gv = exp(-(sigma*%pi*fv')^2) * ones(1,c)
      gf = gf .* gv
   end

   select direction
   case 'horizontal'
      fvp = %i*2*%pi*fv' * ones(1,c)
      Dyf = fft(Img,-1) .* gf .* fvp
      border = abs(fft(Dyf,1))
   case 'vertical'
      fup = ones(r,1) * %i*2*%pi*fu
      Dxf = fft(Img,-1) .* gf .* fup
      border = abs(fft(Dxf,1))
   case 'both'
      fup = ones(r,1) * %i*2*%pi*fu
      fvp = %i*2*%pi*fv' * ones(1,c)
      Dxf = fft(Img,-1) .* gf .* fup
      Dyf = fft(Img,-1) .* gf .* fvp
   
      Dx = abs(fft(Dxf,1))
      Dy = abs(fft(Dyf,1))
   
      border = sqrt(Dx.^2 + Dy.^2)
   else
      error('Invalid direction.');
   end
   if thresh >=0 then
      border=im2bw(border,thresh,max(border))
   end
   // END
   return
else
   error('Invalid edge detection method.')
end

select direction
case 'horizontal'
   mx=imconv(Img,mask);
   border=abs(mx)
case 'vertical'
   my=imconv(Img,-mask');
   border=abs(my)
case 'both'
   mx=imconv(Img,mask);
   my=imconv(Img,-mask');
   border=sqrt(mx.*mx + my.*my);
else
   error('Invalid direction.');
end

if thresh >=0 then
   border=im2bw(border,thresh,max(border))
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
