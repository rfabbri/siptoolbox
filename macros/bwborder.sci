function [outm]=bwborder(inm, conn)
//
// Extracts contours from binary
// images, by detecting which pixel valued 1 has
// at least one neighbor valued 0.
// conn is 4 or 8 dependig if the detection is to
// be made in 4 or 8-neighborhood. The final border is
// 8-connected if 4-neighbors are used, and 4-connected if
// 8-neighbors are used.
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $

if argn(2)==1, conn = 8, end

select conn
case 8 then
   M=[1 1 1; 1 0 1; 1 1 1];
   result=8;
case 4 then
   M=[0 1 0; 1 0 1; 0 1 0];
   result=4;
else
   error('Invalid neighborhood number.');
end;

if type(inm) == 1
   inm(inm<>0)=1
end

// convert to uint8
//inm = uint8(inm); @@@ parei aqui

// Assume 0 outside image
[r,c]=size(inm)
r = r+2; c = c+2;
//aux = uint8(zeros(r, c));
aux = zeros(r,c)
aux(2:r-1,2:c-1) = inm;
inm = aux;
clear aux
outm = zeros(r,c)
//outm = uint8(zeros(r,c))

for i=2:r-1, for j=2:c-1
   if inm(i,j)==1 then 
    if sum( M .* inm(i-1:i+1,j-1:j+1) ) <> result then
       outm(i,j)=1;
     end;
   end;
end; end;

outm = outm(2:r-1,2:c-1);
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
