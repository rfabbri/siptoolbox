function [map,number]=sip_colormap(j,nc)
//
// DESCRIPTION
// Computes a beautiful colormap.
// These colormaps were created by Enrico Segre
// (anyway you should have a look at the Enrico toolbox for
// Scilab if you're interested in nice graphics).
// The names and numbers are also from Enrico.
// I simply put the code in a more practical manner for me.
//
// The default is 8bit (256 colors).
// 
// AUTHOR
//    Original code: Enrico Segre
//
//    Code rearrangement:
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// LICENSE: GNU GPL
//
// CHANGES:
// * argument can be the map number or the name
// * negative colormap numbers are no longer accepted. Give a negative number of colors instead
// to get the complimentary colormap
//


if argn(2)==0 then
  err_string= "[map,colormap_name]=sip_colormap(colormap_name_or_number[,number_of_colors])"
  explain_string=[' Available colormaps are: ';..
	  '1 - blackwhite ';..
	  '2 - fluorescine ';..
	  '3 - caramel ';..
	  '4 - cappuccino ';..
	  '5 - chocolate ';..
	  '6 - hot';..
	  '7 - hot2';..
	  '8 - bluered';..
	  '9 - lightbluered';..
	  '10 - sunrise';..
	  '11 - blueblackred';..
	  '12 - blueblackred2';..
	  '13 - bluegreenred';..
	  '14 - bluegreenred2';..
	  '15 - bluegreenred3';..
	  '16 - prism';..
	  '17 - prism2';..
	  '18 - bands';..
	  '19 - blackbands'];
  disp(explain_string);
  error(err_string);
end;


if ~exists('nc','local') then
  nc=256;
end

i=string(j);

if (nc<0) then //complementary colormap if number_of_colors is < 0
  complement=1;
  nc=abs(nc);
else
  complement=0;
end;

select (i) //select by name
case 'blackwhite'
  i='1';

case 'fluorescine'
  i='2'; 
    
case 'caramel'
  i='3';
  
case 'cappuccino'
  i='4';
  
case 'chocolate'
  i='5';
  
case 'hot'
  i='6';
  
case 'hot2'
  i='7';
    
case 'bluered'
  i='8';
  
case 'lightbluered'
  i='9'
  
case 'sunrise'
  i='10';
  
case 'blueblackred'
  i='11';
  
case 'blueblackred2'
  i='12';
  
case 'bluegreenred'
  i='13';
  
case 'bluegreenred2'
  i='14';
  
case 'bluegreenred3'
  i='15';
  
case 'prism'
  i='16';
  
case 'prism2'
  i='17';
  
case 'bands'
  i='18';
  
case 'blackbands'
  i='19';
  
end;//case (name)




select (i) //select by number
case '1'
  map=linspace(0,1,nc)'*[1,1,1]; 
  
case '2'
  map=[((1:nc).^2)/nc;1:nc;((1:nc).^2)/nc]'/nc; 
  
case '3' 
  map=[(0:nc-1).^0.67*nc^0.33;1:nc;((1:nc).^3)/nc^2]'/nc ;
  
case '4'
  map=[(0:nc-1).^0.8*nc^0.2;(1:nc).^2/nc;((1:nc).^3)/nc^2]'/nc;
  
case '5'
  map=[(0:nc-1).^0.8*nc^0.2;(1:nc).^2/nc;sin(3*%pi*(1:nc)/nc)/2+1]'/nc; 
  
case '6'
  map=  [min(1,max((2*(1:nc)/nc),0));...
	  min(1,max((2.5*(1:nc)/nc)-1,0));...
	  min(1,max((3*(1:nc)/nc)-2,0))]'; 
  
case '7'
  // this is copied from hotcolormap of scilab 2.4.1
  map=[(1:fix(3/8*nc))'/fix(3/8*nc)  zeros(fix(3/8*nc),1) ...
	  zeros(fix(3/8*nc),1); ...
	  ones(fix(3/8*nc),1)  (1:fix(3/8*nc))'/fix(3/8*nc)  ...
	  zeros(fix(3/8*nc),1); ...
	  ones((nc-2*fix(3/8*nc)),1)  ones((nc-2*fix(3/8*nc)),1)  ...
	  (1:(nc-2*fix(3/8*nc)))'/(nc-2*fix(3/8*nc))];
  
case '8'
  map=[1:nc;0*(1:nc);nc:-1:1]'/nc;
  
case '9'
  map=[1:nc;nc:-1:1;nc:-1:1]'/nc;
  
case '10'
  map=[[zeros(1,nc-floor(nc/4)-floor(nc/2)),...
      linspace(1,nc,floor(nc/4))./nc,ones(1,floor(nc/2))]',..
       [linspace(0,1,ceil(nc/2)),linspace(1,0,floor(nc/2))]',..
       [ones(1,floor(nc/2)),linspace(nc,1,floor(nc/4))./nc,...
         zeros(1,nc-floor(nc/2)-floor(nc/4))]'];

case '11'
  map=[zeros(1,ceil(nc/2)),linspace(1,nc,floor(nc/2));...
  zeros(1,nc);linspace(nc,1,floor(nc/2)),zeros(1,ceil(nc/2))]'/nc;

case '12'
  map11=[zeros(1,ceil(nc/2)),linspace(1,nc,floor(nc/2));...
	  zeros(1,nc);linspace(nc,1,floor(nc/2)),zeros(1,ceil(nc/2))]'/nc;
  map=sqrt(map11);

case '13'
  map=[zeros(1,ceil(nc/2)),linspace(1,nc,floor(nc/2));..
        zeros(1,floor(nc/4)),linspace(1,nc,floor(nc/4)),...
        linspace(nc,0,nc-3*floor(nc/4)),zeros(1,floor(nc/4));...
        linspace(nc,1,floor(nc/2)),zeros(1,ceil(nc/2))]'/nc;

case '14'
  map=[zeros(1,floor(nc/2)),..
	linspace(1,nc,ceil(nc/2));...
      linspace(0,nc,ceil(nc/2)),linspace(nc,0,floor(nc/2));...
      linspace(nc,0,ceil(nc/2)),zeros(1,floor(nc/2))]'/nc;

case '15'
  map=[1:nc;...
        linspace(0,nc,floor(nc/2)),linspace(nc,0,ceil(nc/2))...
        ;nc:-1:1]'/nc;

case '16'
  map=...
  [ sin((linspace(2,2*nc,nc)+2.*nc/3)*%pi/nc);...
    sin((linspace(2,2*nc,nc)-2.*nc/3)*%pi/nc);...
    sin((linspace(2,2*nc,nc))*%pi/nc)]'/2+1./2;

case '17'
  map16=...
  [ sin((linspace(2,2*nc,nc)+2.*nc/3)*%pi/nc);...
    sin((linspace(2,2*nc,nc)-2.*nc/3)*%pi/nc);...
    sin((linspace(2,2*nc,nc))*%pi/nc)]'/2+1./2;
  map=map16.^(1/2)';
  
case '18'
  map=[1:nc;nc:-1:1;nc*((-sin(15*%pi*(1:nc)/nc)+1)/2)]'/nc;
  
case '19'
  map=...
   ([sin((linspace(2,2*nc,nc)+2.*nc/3)*%pi/nc);...
     sin((linspace(2,2*nc,nc)-2.*nc/3)*%pi/nc);...
     sin((linspace(2,2*nc,nc))*%pi/nc)]'/2+1./2)...
    .*modulo([1:nc;1:nc;1:nc]'-1,nc/8)*7/nc;



  

else warning(["Colormap numbers are integers in the range 1-19";..
	"So, the function returns a gray colormap"]);
  i='1';
  map=linspace(0,1,nc)'*[1,1,1]; 
  

end;//case select by number

map=abs(complement-map);//complementary map if colormap_number<0
number=evstr(i);
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


