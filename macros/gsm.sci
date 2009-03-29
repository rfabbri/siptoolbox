function [xsm,Xsm]=gsm(x,sigma,delta,in,out)
//
// Performs gaussian smoothig of the vector x, 
// with standard deviation sigma, using FFT. 
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// $Revision: 1.2 $  $Date: 2009-03-29 21:34:48 $

if size(x,'c') < size(x,'r') then
  x = x.'
end

rhs = argn(2);
if rhs <1 then
   error('Invalid number of arguments.');
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

//  SIGMA == 0
if sigma == 0 then  // return x unaltered
   select in
   case 'time'
      select out
      case 'time'
         xsm = x
      case 'frequency'
         xsm = fft(x,-1);
      else
         error('''out'' must be ''time'', ''frequency'' or ''complex''');
      end
   case 'frequency'
      select out
      case 'time'
         xsm = real(fft(x,1));
      case 'complex'
         xsm = fft(x,1);
      case 'frequency'
         xsm = x
      else
         error('''out'' must be ''time'', ''frequency'' or ''complex''');
      end
   else
      error('''in'' must be ''time'' or ''frequency''');
   end
   return
end

//  SIGMA <> 0
N=size(x,'c')
select in 
case 'time'
   if ~exists('delta','local')
      delta = 1
   end   
   X   = fft(x,-1)
   dt  = delta
   aux = 1/dt
   df  = 1/(N*dt)
case 'frequency'
   if ~exists('delta','local')
      delta = 1/N
   end   
   X   = x
   df  = delta
   aux = N*df
   dt  = 1/aux
else
   error('''in'' must be ''time'' or frequency''')
end


// create the gaussian in frequency 
f = [0:N-1]*df
f(int(N/2):$) = f(int(N/2):$) - aux
G = aux * exp( -(sigma*%pi*f)^2 )

Xsm=X.*G;

select out
case 'time'
   xsm=real(fft(Xsm,1));
case 'complex'
   xsm=fft(Xsm,1);
case 'frequency'
   xsm = Xsm;
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
