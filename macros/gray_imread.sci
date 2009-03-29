function im=gray_imread(fname)
// 
// Reads any image into scilab as a grayscale image
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $
   [im,map]=imread(fname)
   info = imfinfo(fname)
   if info.StorageType == 'indexed'
      im = im2gray(im,map)
   else
      im = im2gray(im)
   end
endfunction
