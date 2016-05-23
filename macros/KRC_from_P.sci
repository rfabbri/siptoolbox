function [K, R, C] = KRC_from_P(P, noscale)
// 
// Decompose a 3x4 camera matrix P into intrinsic parameters K, rotation R, and
// center of projection C.
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//
// REFERENCE
//    Oxford's Visual geometry group matlab toolbox
//
// $Revision: 1.1 $ $Date: 2010-10-09 14:46:33 $
//

N = size(P,1);
H = P(:,1:N);

[K,R] = sip_rq(H);
  
if argn(2) < 2
  K = K / K(N,N);
//  if K(1,1) < 0
//    D = diag([-1 -1 ones(1,N-2)]);
//    K = K * D;
//    R = D * R;
    
//    test = K*R; 
//    vgg_assert0(test/test(1,1) - H/H(1,1), 1e-07)
//  end
  // from http://ksimek.github.io/2012/08/14/decompose/
  // make diagonal of K positive
  sg = diag(sign(diag(K)));

  K = K * sg;
  R = sg * R; 
  // det(R) negative, just invert - the proj equation remains same:
  R = -R
end



if argn(1) > 2
  C = -P(:,1:N)\P(:,$);
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
