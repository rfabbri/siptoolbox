function  K = curvature(arg1, arg2, arg3, arg4)
//
// Curvature of an object border represented by a
// binary image or a parametric contour.
// TODO
//    - accept coordinates as unique complex vector
//    - pad vector for FFT
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
//    "Differential Geometry of Curves and Surfaces", 
//    Manfredo P. do Carmo, Prentice Hall, 1976.
//
// $Revision: 1.2 $  $Date: 2009-03-29 21:34:48 $


rhs = argn(2)
if rhs == 0 then
   error('Invalid number of arguments.')
end
[r,c]=size(arg1)
if (r > 1) & (c > 1)    // input is a binary image
   [x,y] = follow(arg1)
   select rhs
      case 1 
         sigma = 5
         delta = 1
      case 2 
         sigma = arg2
         delta = 1
      case 3
         sigma = arg2
         delta = arg3
      else error('Invalid number of arguments.')
   end
else      // input is a parametric contour
   select rhs
      case 2 
         sigma = 5
         delta = 1
      case 3 
         sigma = arg3
         delta = 1
      case 4
         sigma = arg3
         delta = arg4
      else error('Invalid number of arguments.')
   end
   x = arg1, y = arg2
end

X=fft(x,-1);
delta = 1/(delta*size(x,'*'))  // delta_f = 1/(dt*N)
D1x = fftderiv(X,1,sigma,in='frequency', delta=delta)
D2x = fftderiv(X,2,sigma,in='frequency', delta=delta)

Y=fft(y,-1);
D1y = fftderiv(Y,1,sigma,in='frequency', delta=delta)
D2y = fftderiv(Y,2,sigma,in='frequency', delta=delta)

K = (D1x.*D2y - D1y.*D2x)./ ( (D1x.^2 +D1y.^2).^1.5 ); 
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
