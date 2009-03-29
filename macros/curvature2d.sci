function Output = curvature2d(Input)
//
// Curvature of a matrix (discrete surface) by finite
// differences
//
// AUTHOR
//    Leandro Estrozi  <lfestrozi@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    http://siptoolbox.sf.net/~lfestroz/
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $
//
	[nr,nc] = size(Input);
   Z = zeros(nr,nc);
	Output = Z
	dx  = Z, dy  = Z
	ddx = Z, ddy = Z, dxy = Z
	dx(:,2:nc-1)  = (Input(:,3:nc) - Input(:,1:nc-2)) / 2;
	dy(2:nr-1,:)  = (Input(3:nr,:) - Input(1:nr-2,:)) / 2;
	ddx(:,2:nc-1) = Input(:,3:nc) - 2 * Input(:,2:nc-1) + Input(:,1:nc-2);
	ddy(2:nr-1,:) = Input(3:nr,:) - 2 * Input(2:nr-1,:) + Input(1:nr-2,:);
	dxy(2:nr-1,2:nc-1) = ((Input(3:nr,3:nc) - Input(3:nr,1:nc-2)) - ...
                         (Input(1:nr-2,3:nc) - Input(1:nr-2,1:nc-2))) / 4;
	Output = (ddx.*dy.^2 - 2*dx.*dy.*dxy + ddy.*dx.^2) ./ ...
            (dx.^2 + dy.^2 + 0.00001);
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
