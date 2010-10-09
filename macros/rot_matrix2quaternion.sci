function q = rot_matrix2quaternion(R)
// Generates quaternion from rotation matrix
//
// REFERENCE
//    Oxford's Visual geometry group multiview matlab toolbox
// 
// $Revision: 1.1 $  $Date: 2010-10-09 23:39:24 $

q = [	(1 + R(1,1) + R(2,2) + R(3,3))
	(1 + R(1,1) - R(2,2) - R(3,3))
	(1 - R(1,1) + R(2,2) - R(3,3))
	(1 - R(1,1) - R(2,2) + R(3,3)) ];

// Pivot to avoid division by small numbers
[b I] = max(abs(q));

q(I) = sqrt(q(I)) / 2 ;

if I == 1 
	q(2) = (R(3,2) - R(2,3)) / (4*q(I));
	q(3) = (R(1,3) - R(3,1)) / (4*q(I));
	q(4) = (R(2,1) - R(1,2)) / (4*q(I));
elseif I==2
	q(1) = (R(3,2) - R(2,3)) / (4*q(I));
	q(3) = (R(2,1) + R(1,2)) / (4*q(I));
	q(4) = (R(1,3) + R(3,1)) / (4*q(I));
elseif I==3
	q(1) = (R(1,3) - R(3,1)) / (4*q(I));
	q(2) = (R(2,1) + R(1,2)) / (4*q(I));
	q(4) = (R(3,2) + R(2,3)) / (4*q(I));
elseif I==4
	q(1) = (R(2,1) - R(1,2)) / (4*q(I));
	q(2) = (R(1,3) + R(3,1)) / (4*q(I));
	q(3) = (R(3,2) + R(2,3)) / (4*q(I));
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
