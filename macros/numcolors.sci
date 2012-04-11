function nc = numcolors(rgb)
//
// Counts the number of unique colors used in an rgb image
//
  rgb = round(255*matrix(rgb,size(rgb,1)*size(rgb,2),size(rgb,3)));
  rgb = dec2hex(rgb);
  rgb = rgb(:,1) + rgb(:,2) + rgb(:,3);
  nc = size(unique(hex2dec(rgb)),'*');
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
