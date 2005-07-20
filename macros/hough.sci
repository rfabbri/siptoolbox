function [ht,rho_range] = hough(im)
//
// Hough Transform of a binary image
//
// TODO
//    - parameters for setting the resolution
//    - peak detection
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@if.sc.usp.br>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://cyvision.if.sc.usp.br
//
// REFERENCES 
//    "Shape Analysis and Classification", L. da
//    F. Costa and R. M. Cesar Jr., CRC Press. 
//
//    "Practical Computer Vision using C", J. R. 
//    Parker, Wiley.
//
// $Revision: 1.1 $  $Date: 2005-07-20 01:33:35 $
//

conv  = %pi/180
[r,c] = size(im)
ctr_x = ceil(c/2),  ctr_y = ceil(r/2)

r2=int(r/2); c2=int(c/2)
rmax = ceil(sqrt(r2*r2 + c2*c2))

// sin and cos lookup tables
sarr = zeros(180,1)
carr = sarr
for theta=0:179
   sarr(theta+1) = sin(theta*conv)
   carr(theta+1) = cos(theta*conv)
end

ht = zeros(2*rmax+1,180)
for j=1:c,  for i=1:r
   if im(i,j) == 1
      x = j - ctr_x
      y = i - ctr_y
      for theta=1:180
         rho = x*carr(theta) + y*sarr(theta)
         ht(rmax+rho+1,theta) = ht(rmax+rho+1,theta) + 1
      end
   end
end,end
rho_range = -rmax:rmax;
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
