function R = quaternion2rot_matrix(q)
// Generates rotation matrix from quaternion
//
// REFERENCE
//    Oxford's Visual geometry group multiview matlab toolbox
// 
// $Revision: 1.1 $  $Date: 2010-10-09 23:39:24 $

q00 = q(1)*q(1); 
q0x = q(1)*q(2); 
q0y = q(1)*q(3); 
q0z = q(1)*q(4);
qxx = q(2)*q(2);
qxy = q(2)*q(3);
qxz = q(2)*q(4);
qyy = q(3)*q(3); 
qyz = q(3)*q(4);
qzz = q(4)*q(4);

R = [ q00 + qxx - qyy - qzz       2*(qxy - q0z)	          2*(qxz + q0y)   
          2*(qxy + q0z)	      q00 - qxx + qyy - qzz       2*(qyz - q0x)
          2*(qxz - q0y)	          2*(qyz + q0x)	      q00 - qxx - qyy + qzz ];

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
