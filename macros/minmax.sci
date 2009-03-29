function Output = minmax(Input,NSteps,StepSize,Adapt,NonAdaptThreshold,IntMaskSize,ExtMaskSize)
//
// Min/Max algorithm for Noise Removal from Images.
// 
// ORIGINAL AUTHOR
//    Leandro Estrozi  <lfestrozi@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    http://siptoolbox.sf.net/~lfestroz/
// 
// REFERENCES
//    For more theoretical infomation and demostrative applets, see:
//    http://math.berkeley.edu/~sethian/Movies/Movienoiseremoval.html
//
// $Revision: 1.2 $  $Date: 2009-03-29 21:34:48 $

rhs = argn(2)
if rhs<1
	error('Invalid number of arguments.');
end

if ~exists('NSteps','local')
	NSteps = 10
end

if ~exists('StepSize','local')
	StepSize = 0.05
end

if ~exists('Adapt','local')
	Adapt = %F
end

if ~exists('NonAdaptThreshold','local')
	NonAdaptThreshold = 0.5
end

if ~exists('IntMaskSize','local')
	IntMaskSize = 1
end

if ~exists('ExtMaskSize','local')
	ExtMaskSize = 0
end


ExtMask   = mkfilter('circular-mean',ExtMaskSize);
Output = Input;
if Adapt, IntMask = mkfilter('circular-mean',IntMaskSize), end;
for Step=0:NSteps-1 
	k_2D = curvature2d(Output);
	ExtMean = imconv(Output,ExtMask);
	if Adapt then
		IntMean = imconv(Output,IntMask);
		mins = find(ExtMean <  IntMean);
		maxs = find(ExtMean >=  IntMean);
	else
		mins = find(ExtMean <  NonAdaptThreshold);
		maxs = find(ExtMean >=  NonAdaptThreshold);
	end;
	k_2D(mins) = min(k_2D(mins),0);
	k_2D(maxs) = max(k_2D(maxs),0);
	k_2D = k_2D * StepSize;
	Output = Output + k_2D;
end
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
