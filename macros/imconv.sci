function [outm]=imconv(inm, mask, form);
//
// This function space-domain-convolves 2D matrix "inm" with nxn "mask",
// n odd, yielding "outm".
// Output isn't normalized.
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@if.sc.usp.br>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://cyvision.if.sc.usp.br
//
// REMARKS
//    The implementation incorporates suggestions from Scilab group.
//
// TODO
//    - eliminate zero-valued mask elements in the process
//    - mirror the mask
//    
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $
//
// Changelog:
// Tests if inm is a gray image + error messages remembers the correct syntax
// of the function

if argn(2)==2 then 
   form = 'same'
elseif argn(2) <> 3 then 
   error('Invalid number of arguments: outm=imconv(gray_image,mask[,form])')
end

if typeof(inm) == 'hypermat'
   warning(' this function has been tested only with gray level images.')
end


[mm,nm]=size(mask)
d=(mm-1); 
if mm<>nm | mm==2*int(mm/2) then 
  error('mask must be a square nxn matrix, n odd.')
end


[m,n]=size(inm)
select form
case 'same' then    // pad image and return the original(same) size
   m = m+d, n = n+d
   aux = zeros(m,n) 
   aux((1+d/2):(m-d/2), (1+d/2):(n-d/2)) = inm
   inm = aux
   clear aux
case 'full' then    // pad image and outm has same size as padded image
   m = m+2*d, n = n+2*d
   aux = zeros(m ,n) 
   aux(d+1:m-d,d+1:n-d) = inm
   inm = aux
   clear aux
case 'valid' then    // don't padd and return normally 
else
   error('Invalid format specification.')
end

inm  = double(inm)
outm = zeros(inm)      //final image gets a black border

M=matrix(mask,1,-1)  // reshapes mask into a vector
nelem_mask=size(M,'*')

// J --> column numbers of sliding window
J = ones(1,n-d).*.(0:d)  +  (1:n-d).*.ones(1,mm)

for i=(1+d/2):(m-d/2)
  outm(i,(1+d/2):(n-d/2))=M*matrix( inm(i-d/2:i+d/2,J), nelem_mask, n-d)
end

outm = outm((1+d/2):(m-d/2), (1+d/2):(n-d/2))

endfunction 
//
// Documentation of the above procedure:
//
// inm(i-d/2:i+d/2, J) contains all the neighborhoods of
// elements in row i of inm, side-by-side, so this is a 
// mm -by- (n-d)*mm  matrix. 
//
// matrix( inm(i-d/2:i+d/2,J), nelem, n-d) is the same
// matrix, but now it is nelem -by- n-d, that is, each
// neighborhood is an nelem -by- 1 column vector, and they are side-by-side.
//
// By multiplying the original mask and that matrix,
// we have row i of the convolved matrix. 
//

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
