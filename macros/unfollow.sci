function [Image]=unfollow(x,y,dims);
//
// Plots a contour from its parametric representation
// into a binary image. This is the reverse of the
// "follow" function.
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@if.sc.usp.br>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://cyvision.if.sc.usp.br
//
//
// $Revision: 1.1 $  $Date: 2005-07-20 01:33:35 $

if argn(2) < 3 then
   error('Invalid number of arguments.')
end

// truncation permits one to plot contours
// that have continuous coordinates
y = int(y) 
x = int(x)
Image=zeros(dims(1),dims(2));
for i=1:size(x,'*') do
   Image(dims(1) - y(i), x(i)+1)=1
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
