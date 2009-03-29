function [A] = sip_index_true_cmap(Im,n)
   //
   // On input Im is a n1 x n2 x 3 hypermat describing a 
   // true color image  Im(i,j,:) giving the R-G-B of the 
   // pixel (i,j)
   //
   // On output A is a n1 x n2 matrix, A(i,j) given the 
   // index on the "true" color map of the (i,j) pixel. 
   // 
   // This new version doesn't use anymore hypermatrices
   // extraction for a gain in speed (as hypermatrix extraction 
   // is not currently too efficient in scilab); this result in
   // a less aesthetic code than before...  Also it uses
   // round in place of floor for a better color reduction
   // (from 0-255 levels to 0-39). 
   //
   // Author : Bruno Pincon
   //
   if argn(2)==1
      n = 40
   end
   dims = size(Im)
   v = round(Im("entries")*(n-1))
   m = dims(1)*dims(2)
   A = v(1:m)*n^2 + v(m+1:2*m)*n + v(2*m+1:$) + 1
   A = matrix(A,dims(1),dims(2))
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
