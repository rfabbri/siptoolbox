function [Dy, DY]=fftderiv(y,n,sigma,delta,in,out)
//
// nth derivative of a sampled periodic function by FFT.
//
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// $Revision: 1.2 $  $Date: 2009-03-29 21:34:48 $

if size(y,'c') < size(y,'r') then
  y = y.' 
end

rhs = argn(2)
if rhs<1 
   error('Invalid number of arguments.');
end

if ~exists('n','local')
   n = 1
end
if ~exists('sigma','local')
   sigma = 5
end   
if ~exists('in','local')
   in = 'time'   
end   
if ~exists('out','local')
   out = 'time' 
end   


N=size(y,'c');
select in 
case 'time'
   if ~exists('delta','local')
      delta = 1
   end   
   Y = fft(y,-1)
   dt  = delta
   aux = 1/dt
   df  = 1/(N*dt)
case 'frequency'
   if ~exists('delta','local')
      delta = 1/N
   end   
   Y = y
   df  = delta
   aux = N*df
   dt  = 1/aux
else
   error('''in'' must be ''time'' or frequency''')
end


f = [0:N-1]*df
f(int(N/2):$) = f(int(N/2):$) - aux

if sigma == 0 then
   G=ones(y)
else
   // create the gaussian in frequency 
   G = exp( -2*(sigma*%pi*f)^2 )
end

DY = Y .* G .* (%i*2*%pi*f)^n

select out
case 'time'
   Dy = real(fft(DY,1)); 
case 'complex'
   Dy = fft(DY,1);
case 'frequency'
   Dy = DY
else
   error('''out'' must be ''time'', ''frequency'' or ''complex''');
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
