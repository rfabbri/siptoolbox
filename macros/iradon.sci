function [B]=iradon(R,Theta,filter)
//
// Performs the well knwon "Back Projection" algorithm to reconstruct an
// image from its  radon transform. 
//
// The parameter filter is optional and it can be used to disable the
// 'Ram-Lak' (ramp) filtering.
// 
// PRIMARY AUTHOR
//    Leandro F. Estrozi  <estrozi at vms.cnrs-gif.fr>
//    http://www.hotreference.com/estrozi/
//
// $Revision: 1.1 $  $Date: 2005-07-20 01:33:35 $

if ~exists('filter','local')
   filter = 'Ram-Lak';
end

if ~exists('theta','local') | Theta == []
   DeltaTheta = 180/size(R,'c');
   Theta = 0:DeltaTheta:180-DeltaTheta
else
   DeltaTheta = Theta(2) - Theta(1)
end

sR = size(R,'r');
B = zeros(sR,sR);

prev=sip_quiet();
for theta=Theta
	minB = min(B);
	B = B - minB;
	maxB = max(B);
	if maxB~=0 then
		B = B / maxB;
	end
	B = mogrify(B, ['-background','black','-rotate', string(DeltaTheta)]);
	B = B * maxB + minB;
	inidx = ((size(B,'r')-sR)/2)+1;
	if filter=='Ram-Lak' then
		r = sip_ramp(R(:,round(theta/DeltaTheta)+1));
	else
		r = R(:,round(theta/DeltaTheta)+1);
	end
	B = B(inidx:inidx+sR-1,inidx:inidx+sR-1) + r*ones(1,size(inidx:inidx+sR-1,'c'));
end
sip_set_verbose(prev);

endfunction 

function [fr]=sip_ramp(r)
        N=size(r,'r');
        Y = fft(r',-1);
        df  = 1/N;
        f = [0:N-1]*df;
        f(int(N/2):$) = f(int(N/2):$) - 1;
        DY = Y .* abs(f);
        fr = real(fft(DY,1));
	fr = fr';
endfunction 

//
// -------------------------------------------------------------------------
// SIP - Scilab Image Processing toolbox
// Copyright (C) 2002-2004  Ricardo Fabbri
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
