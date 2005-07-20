function [MG]=gsm2d(M,sigma)
//
// Performs 2D gaussian smoothing of image M, with 
// standard deviation sigma. 
//
// TODO
//   - currently, this is circular convolution. We should offer an
//   optional parameter that will pad the original image with zeros.
//   - maybe introduce pading so nlogn FFT is forced
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@if.sc.usp.br>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://cyvision.if.sc.usp.br
//
// $Revision: 1.1 $  $Date: 2005-07-20 01:33:35 $

select argn(2)
case 1
   sigma = 5;
case 0
   error('Invalid number of arguments.');
end

MG=zeros(M);
[r,c]=size(M);

fu = [0:c-1]*(1/c)
fv = [0:r-1]*(1/r)
fu(int(c/2):c) = fu(int(c/2):c) - 1;
fv(int(r/2):r) = fv(int(r/2):r) - 1;

gf = ones(r,1) * exp(-(sigma*%pi*fu)^2)
gv = exp(-(sigma*%pi*fv')^2) * ones(1,c)
gf = gf .* gv

MGf = fft(M,-1) .* gf;

MG = real(fft(MGf,1));
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
