function [R]=radon(M,Theta)
//
// Return the radon transform of M (also known as Sinogram) sampled by
// DeltaTheta (degrees).
// 
// AUTHOR
//    Leandro F. Estrozi  <estrozi at vms.cnrs-gif.fr>
//    http://www.hotreference.com/estrozi/
//
// $Revision: 1.2 $  $Date: 2009-03-29 21:34:48 $

if ~exists('Theta','local')
   Theta=0:179
end

maxsize = round(sqrt(size(M,'r')^2+size(M,'c')^2));
R = zeros(maxsize,size(Theta,2));
//Normalization because mogrify.
minM = min(M);
M = M - minM;
maxM = max(M);
M = M / maxM;

prev=sip_quiet();

i = 1
for theta=Theta,
	r = sum(maxM*mogrify(M, ['-background','black','-rotate', string(theta)])+minM,'c');
	sr = size(r,1);
	inidx = round((maxsize-sr)/2)+1;
	R(inidx:inidx+sr-1,i) = r;
   i = i + 1
end

sip_set_verbose(prev);
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
