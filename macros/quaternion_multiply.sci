function r_times_q = quaternion_multiply(r,q)
// Quaternion multiplication
//
// REFERENCE
//    Oxford's Visual geometry group multiview matlab toolbox
// 
// $Revision: 1.1 $  $Date: 2010-10-09 23:39:24 $

r_times_q = ...
     [(r(1)*q(1) - r(2)*q(2) - r(3)*q(3) - r(4)*q(4))  
      (r(1)*q(2) + r(2)*q(1) + r(3)*q(4) - r(4)*q(3))  
      (q(3)*r(1) - q(4)*r(2) + q(1)*r(3) + q(2)*r(4))  
      (q(4)*r(1) + q(3)*r(2) - q(2)*r(3) + q(1)*r(4)) ];
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
