function R = exp2rot_matrix(r)
// Convert from exponential to matrix rotation parameterization.
//
// REFERENCE
//    Oxford's Visual geometry group multiview matlab toolbox
// 
// $Revision: 1.1 $  $Date: 2010-10-09 23:39:24 $


H = [0, -r(3), r(2);  r(3), 0, -r(1); -r(2), r(1), 0];

//  R = expm(H);

angle = norm(r);
if (angle < %eps)
  R=eye(3,3);
else
  ef = sin(angle)/angle;
  gee = (1.0 - cos(angle))/ (angle*angle);
  R = (H*H)*gee + H*ef + eye(3,3);
end

endfunction
//
// -------------------------------------------------------------------------
// SIP - Scilab Image Processing toolbox
// Copyright (C) 2002-2010  Ricardo Fabbri
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
