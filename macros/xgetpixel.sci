function p=xgetpixel(arg,winno)
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// TODO @@@
// - test under windows and other machines/resolutions
// 
// DESCRIPTION
// arg might be the # of rows in the considered image,
// or it might be the image itself. 
// The coordinate returned is the standard (row,col) scilab
// coordinate, starting at (1,1).
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $
//
   if argn(2) == 0
      error('Invalid number of arguments.');
   elseif argn(2)==2
      xset('window',winno);
   end
   if size(arg,1)<>1 & size(arg,2)<>1
      arg=size(arg,1); 
   end
   printf('Press any mouse button to finish...\n');
   figure('position',[0 0 200 30])
   ended=%f
   while ~ended
      p=xgetmouse()
      m=round(p(1:2))
      m(2)=arg-m(2)+1
      uicontrol(  'style','text',...
         'string','('+string(m(2))+' row , '+string(m(1))+' col)',...
         'position',[0,0,200,30],...
         'fontsize',14);
      ended = ( p(3)>=0 );
   end
   p=m(2:-1:1)
   close();
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
