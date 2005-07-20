function outm=pifilter(inm,name)
//
// Filters a phased image before unwrapping it.
// The purpose is to keep the phase jumps sharp:
// a convolution by a low-pass kernel would be catastrophic,
// because it would smooth these jumps, causing the demodulation
// algorithm to fail.
//
// Principle:
// 1) We calculate the sinus and cosinus of the phase
// 2) We apply a convolution filtering on each component
// 3) We get the phase again with an atan
//
// This function uses the mkfilter and imconv functions.
// Reasonable kernels are: 'low-pass' and 'mean': a high-pass filter
// would make no sense as it would burst noise !
//
// AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// REFERENCES:
// "Techniques automatiques de raccordement de phase" by David Venet
// Ecole Federale de Lausanne
//
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $
//
//

if (argn(2)==0|argn(2)>2) then
  error("Wrong number of arguments: filtered_image=pifilter(image[,filter_name])")
end;

if typeof(inm)=='hypermat'
  warning(' this function has been coded for gray level images: convert with im2gray first.')
  end

if ~exists('name','local') then name='mean';end;//default:mean filter

a=normal(inm,-%pi,%pi);//atan(x,y) returns a value between -%pi and +%pi
n1=sin(a);
d1=cos(a);

kernel1=mkfilter(name);
n2=imconv(n1,kernel1);
d2=imconv(d1,kernel1);

r1=atan(n2,d2);
outm=normal(r1,1);//phased images use the full range 0-1
outm=1-outm;//invert
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
