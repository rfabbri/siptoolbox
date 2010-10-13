function [mnew]=normal(m,upper,lower)
//
// Normalizes a matrix to occupy lower-upper range.
// The intent of this macro is to be a shortcut to
// a simple but often used operation.
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// $Revision: 1.3 $ $Date: 2010-10-13 23:50:46 $

select argn(2)
   case 0 then error('Invalid number of arguments.')
   case 1 then lower = 0, upper=1
   case 2 then lower = 0
end

mmax = maxi(m); mmin = mini(m);
if ( mmax-mmin == 0) then
   // Constant images are not modified.
   mnew = m;
   return;
end

mnew=lower + (upper-lower)*(m-mmin)./(mmax-mmin)
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
