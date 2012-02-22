function imshow(img, arg2, strf, viewer)
//
// Displays image in Scilab graphic window.
//    imshow(I)      // I assumed to be 0-1
//    imshow(I,n)    // I assumed to be 0-1
//    imshow(I,[low high]) 
//    imshow(X,map)  // X assumed to be 1-N
//    imshow(RGB)    // RGB 0-1
//    imshow(RGB, arg2, [, strf, viewer])    // RGB 0-1
//    imshow filename
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// TODO
//
// - incorporate other Matplot parameters 
// - use internal imagemagick API for 'display'-style viewer without 
// writing image to disk
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $

[lhs, rhs] = argn(0);
if rhs == 0 then
   error('Invalid number of arguments.');
end

if ~exists('strf','local')
   strf='040'
else
   rhs=rhs-1
end

if ~exists('viewer','local')
  viewer=SIPVIEWER
elseif getos() == 'Windows'
  error('external viewer not supported on Windows');
else
  tmpname = tempname();
  tmpname = tmpname + '-imshow.png';
  // remove temp file only at the beginning
  unix('rm -f ' + TMPDIR + '/*imshow.png');
end


// TODO
// double-buffering will be default for the next SIP release
// TODO do it the modern way as in sip-intro.dem
//
//prev_pixmap_mode = xget('pixmap');
//
//// double-buffering:
//if ~exists('sip_disable_pixbuffer') 
//   xset('pixmap',1);
//end

select type(img)
   case 1 then    // paletted or grayscale image 
      if ~exists('arg2','local')
        if viewer == 'sci'
          [m,n]=size(img)
          xset('colormap', graycolormap(256))
          xset('wdim',n,m)
          Matplot(img*255 + 1,strf)
        else
          disp ('writing temporary image file ' + tmpname );
          imwrite(img, tmpname);
        end
      else
         [m2,n2] = size(arg2)
         [m,n]=size(img)
         if n2 == 1 then
            if type(arg2) ~= 1 then    // imshow(img,ncolors)
               error("2nd argument must be a scalar.")
            end
            if (arg2 < 3) // workaround to a colormap bug
              mygraycmap = [0 0 0; 1 1 1];
            else
              mygraycmap = graycolormap(arg2);
            end
            img = img*(arg2-1)+1;
            if viewer == 'sci'
              xset('colormap', mygraycmap);
            else
              disp ('writing temporary image file ' + tmpname );
              imwrite(img, mygraycmap, tmpname);
            end
         elseif (n2==0 | n2==2) then   // [] or [n1 n2]
            if n2~=0 then
               img(img<=arg2(1))=arg2(1)
               img(img>=arg2(2))=arg2(2)
            end
            if viewer == 'sci'
              img=round(normal(img,256,1));
              xset('colormap', graycolormap(256))
            else
              img=normal(img);
              disp ('writing temporary image file ' + tmpname );
              imwrite(img, tmpname);
            end
         elseif (n2 == 3) then         // imshow(img,map)
            if viewer == 'sci'
              if m2 == 65536
                 //
                 // Scilab reserves black & white colors, so only max 65534
                 // entries are permited in the colormap :-( 
                 // Black and white are always accessible by m2+1 and m2+2, 
                 // respectively, no matter what is the current colormap.
                 //
                 // I'm ashamed of this code... we must urgently improve scilab's
                 // image display. Please help us if you can :-)
                 //
                 img2 = ind2rgb(img,arg2);
                 arg2=sip_approx_true_cmap(11)  // 11 levels (11^3 colors)
                 img=sip_index_true_cmap(img2,11)
              end
              xset('colormap', arg2)
            else
              disp ('writing temporary image file ' + tmpname );
              imwrite(img, arg2, tmpname);
            end
         else
            error('Invalid size of 2nd argument.')
         end
         if viewer == 'sci'
           xset('wdim',n,m)
           Matplot(img,strf)
         end
      end   
   case 17 then   // truecolor image
      dims=size(img)
      if dims(3) ~= 3 then
         error('RGB image must have 3rd dimension equal to 3.')
      end
      if viewer == 'sci'
        // The following works, at the cost of reduction
        // of the number of colors to 40^3 ~= 2^16
        // Scilab unfortunately doesn't work with 25bit colordepth
        // My thanks to Bruno Pincon
        printf('This may take a while.');
        if argn(2) == 1
           nlevels = 20  // 20^3 colors only, for speed
        else
           nlevels = arg2
        end
        xset('colormap',sip_approx_true_cmap(nlevels))
        xset('wdim',dims(2),dims(1))
        Matplot(sip_index_true_cmap(img,nlevels),strf)
      else
        disp ('writing temporary image file ' + tmpname );
        imwrite(img, tmpname);
      end
   case 10 then   // filename
      if viewer == 'sci'
        map=0;
        [image,map] = imread(img)
        imshow(image,map)
      else
        tmpname = img;
      end
   else
      error('1st argument of invalid type.')
end

// if xget('pixmap') == 1,  xset('wshow'), end

// Part of double-buffer default for next release:
//xset('pixmap',prev_pixmap_mode);
//
if viewer <> 'sci'
  mycmd = viewer + ' ' + tmpname + ' &';
  disp ('running viewer command: ' + mycmd);
  unix_w(mycmd);
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
