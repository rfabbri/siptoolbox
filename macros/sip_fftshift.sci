function x = sip_fftshift(x,job,fun)
// 
// Swap arrays (used with fft).
// Does the same thing as M4t1ab fftshift.
// Fun argument may be used to change the way the shuffling is done
// with deff('sel=fun(sk)','c=floor(sk/2);sel=[c+1:sk,1:c]')
//
// AUTHOR
//    Ricardo Fabbri, with help from Scilab Group.
//
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $

  if argn(2)<2 then job='all',end
  if argn(2)<3 then 
    deff('sel=fun(sk)','c=ceil(sk/2);sel=[c+1:sk,1:c]')
  end
  if job=='r' then job=1,elseif job=='c' then job='2',end
  ind=list()
  if job=='all' then
    for sk=size(x),ind($+1)=fun(sk),end
  else
    for sk=size(x),ind($+1)=:,end;ind(job)=fun(sz(job))
  end
  x=x(ind(:))
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
