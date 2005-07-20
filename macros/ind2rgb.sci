function RGB = ind2rgb(idx,map)
   RGB = [map(idx,1); map(idx,2); map(idx,3)];
   [r,c]=size(idx);
   RGB = hypermat([r c 3], RGB);
endfunction
