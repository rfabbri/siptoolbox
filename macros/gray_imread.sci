function im=gray_imread(fname)
// 
// Reads any image into scilab as a grayscale image
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@if.sc.usp.br>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://cyvision.if.sc.usp.br
//
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $
   [im,map]=imread(fname)
   info = imfinfo(fname)
   if info.StorageType == 'indexed'
      im = im2gray(im,map)
   else
      im = im2gray(im)
   end
endfunction
