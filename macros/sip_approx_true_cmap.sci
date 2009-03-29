function cmap = sip_approx_true_cmap(n)
//
// There are n levels for each color channel intensity
// (each intensity being given by an integer I between 0 and n-1)
// To the "color" R,G,B (R,G,B in [0,n-1]) must correspond the
// index k= R n^2 + G n + B + 1 of the table cmap of size n^3 x 3
// and cmap(k,:) =  [R/(n-1) G/(n-1) B/(n-1)]
//
// As the max size of a cmap in scilab is 2^16-2, 
// n = 40 is the max possible (40^3 <= 2^16 - 2 < 41^3).
// 
// This function returns this colormap.
//
// ORIGINAL AUTHOR 
//	   Bruno Pincon <bruno.pincon@free.fr>
//

if argn(2)==0
   n = 40
end
nb_col = n^3
temp = (0:nb_col-1)'
cmap = zeros(nb_col,3)
q = int(temp/n^2)
cmap(:,1) = q/(n-1)
q = modulo(int(temp/n),n)
cmap(:,2) = q/(n-1)
cmap(:,3) = modulo(temp,n)/(n-1)
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
