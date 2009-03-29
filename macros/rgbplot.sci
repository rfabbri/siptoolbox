function rgbplot(colormap)

// Description:
// rgbplot plots the 3 columns of a colormap:
// 1st in red, 2nd in green, 3rd in blue.
//
// 
// AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
//
// LICENSE: GNU GPL
//


if (argn(2)==0|argn(2)>1) then   
   error("rgbplot(colormap)"); 
end;


T=eye(3,3);//color table with the 3 fundamental colors only

xbasc();
xset("colormap",T);
plot2d([colormap(:,1) colormap(:,2) colormap(:,3)]);

endfunction;



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
