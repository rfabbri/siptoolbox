function h=mkfftfilter(image,name,rc1,rc2)
//
// Build renowned filters, such as Butterworth, used to filter 
// the spectrum (fft) of an image.
//
// All these filters are low-pass. The corresponding high-pass filters
// are easily obtained by (1-h).
// 
// Many thanks to Ricardo for his advices
//
//
// AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// REFERENCES
//    "Optique: fondements et applications" J-P PEREZ
//     6e edition, Dunod
//     chap34: introduction au traitement numerique des images et a la couleur
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $
//


if (argn(2)==0|argn(2)<3|argn(2)>4) then
  error("Wrong number of arguments: mkfftfilter(image,name,rc1[,rc2])")
end;




[r,c]=size(image);
x=((1:r)'*ones(1:c));
y=((1:c)'*ones(1:r))';
x0=r/2;y0=c/2;

//Passing in cylindrical coordinates:
//each element of the matrix has the value of the radius
r=sqrt((x-x0).^2+(y-y0).^2);


select name
case 'binary' then
  h=zeros(r);
  h(find(r<=rc1))=1;h(find(r>rc1))=0;
  
case 'butterworth1'
  n=1;
  d=1+(r/rc1).^(2*n);
  h=ones(r)./d;
  
case 'butterworth2'
  n=2;
  d=1+(r/rc1).^(2*n);
  h=ones(r)./d;

case 'butterworth3'
  n=3;
  d=1+(r/rc1).^(2*n);
  h=ones(r)./d;

case 'exp'//exponential	
  n=1;
  h=exp(-(r/rc1).^n);
  
case 'gauss'//gaussian curve
  n=2;
  h=exp(-(r/rc1).^n);

case 'trapeze'//cutting frequencies: rc2>rc1	
  if ~exists('rc2','local') then
      error("Wrong number of arguments: mkfftfilter(image,''trapeze'',rc1,rc2)")
    else
      h=-(r-rc2)/(rc2-rc1);
      h(find(r<=rc1))=1;
      h(find(r>=rc2))=0;
    end
        
else
   error("Invalid filter name: ''binary'', ''butterworth1,2,3'', ''exp'', ''gauss'' or ''trapeze''")
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
