function C = imcorrcoef(img,template)
// 
// Template matching by normalized correlation coefficient
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@if.sc.usp.br>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://cyvision.if.sc.usp.br
//
// TODO
//    - improve imconv speed, e.g. by passing it to C language.
//    - use correlation instead of convolution
//

   nt = size(template,'*');
   F  = imconv(img, (1/nt)*ones(template));
   denom1 = variance(template)*nt;
   denom2 = (img-F).^2
   denom2 = imconv(denom2,ones(template));
   denominator = sqrt(denom1.*denom2)

   T  = mean(template);
   C = imconv(img-F, template-T) ./ (denominator + %eps);
   // The %eps was added to avoid division by zero.
   // Please tell me a better way to avoid this, if it exists
   // <rfabbri@if.sc.usp.br>  @@@
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
