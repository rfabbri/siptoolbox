function bw = im2bw(Img, arg2, arg3, arg4)
//
// Threshold an intensity, indexed or truecolor image, 
// producing a binary image. "maxvalue" indicates
// the maximum value to assume Img can take. 
// By default, maxvalue is max(Img).
// 
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $
//

if argn(2) <=1 then
   error('Invalid number of arguments.");
end


if type(Img) == 1 then // 2D double array
   if size(arg2,'*') == 1 then   // im2bw(Img, level,???)
      level = arg2
      if (argn(2) == 2)          // im2bw(Img,level)
         maxvalue = max(Img);
      else
         maxvalue = arg3
      end
   else    // im2bw(Img, map, ???)
      if argn(2) ==2 then
         error('Missing the threshold level.')
      end
      level = arg3
      Img=im2gray(Img,arg2)
      if (argn(2) == 3)
         maxvalue = max(Img);
      else
         maxvalue = arg4
      end
   end
elseif type(Img) == 17 then  // Hypermatrix
   level = arg2
   if (argn(2) == 2)          // im2bw(RGB,level)
      maxvalue = max(Img);
   else
      maxvalue = arg3
   end
   Img = im2gray(Img)
else
   error('Invalid type of 1st. argument.')
end

if (level < 0) | (level > 1) then
   error('The threshold level must be in range 0-1.')
end

pause
bw = 1*(Img>=level*maxvalue)

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
