function im=ihough(ht,dims)
//
// Inverse Hough Transform
//
// PARAMETERS
// dims: dimension vector (output of size function)
//
// TODO
//    - more testing 
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// REFERENCES 
//    "Shape Analysis and Classification", L. da
//    F. Costa and R. M Cesar Jr., CRC Press. 
//
//    "Digital Image Processing Algorithms", I. 
//    Pitas, Prentice Hall.
//
// $Revision: 1.2 $  $Date: 2009-03-29 21:34:48 $
//

r = dims(1)
c = dims(2)
conv = %pi/180
[rh,ch] = size(ht)
im = zeros(r,c)
r2=int(r/2); c2=int(c/2)
rmax = ceil(sqrt(r2*r2 + c2*c2))
ctr_x = ceil(c/2),  ctr_y = ceil(r/2)

sarr = zeros(ch,1)
carr = sarr
for itheta=1:ch,  
   sarr(itheta) = sin((itheta-1)*conv)
   carr(itheta) = cos((itheta-1)*conv)
   for irho=1:rh
      if ht(irho,itheta) == 1
         y=0
         rho = irho - rmax-1;

         // line generating procedure as in Costa's book p. 388
         if 45 <= itheta-1 & itheta-1 <= 135
            for j=1:c
               x = j-ctr_x;
               y = (rho - x*carr(itheta)) / sarr(itheta)
               y = floor(y + 0.5)
               i = y + ctr_y
               if i<=r & i>=1
                  im(i,j) = 1
               end
            end
         else
            for i=1:r
               y = i-ctr_y
               x = (rho - y*sarr(itheta)) / carr(itheta)
               x = floor(x + 0.5)
               j = x + ctr_x
               if j<=c & j>=1
                  im(i,j) = 1
               end
            end
         end
      end
   end
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
