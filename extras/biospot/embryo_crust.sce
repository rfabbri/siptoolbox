// This computes the region containing the nucleii, eliminating most noise
// inside the embryo.

// To be called from ini.sce or after execing params.sce

// e=gray_imread('edge.png');
// todo automatic
e = edge(im,'canny',[0 0.04]/dim_ratio_normal,3);

[w,h] = size(e);
en = zeros(w+2*4*closing_distance+1,h+2*4*closing_distance+1);

en((4*closing_distance+1):(w+4*closing_distance),(4*closing_distance+1):(h+4*closing_distance)) = e;
e = en;
clear en;

imn = zeros(w+2*4*closing_distance+1,h+2*4*closing_distance+1);

imn((4*closing_distance+1):(w+4*closing_distance),(4*closing_distance+1):(h+4*closing_distance)) = im;
im = imn;
clear imn;

// morphological closing
// TODO: use fast algorithm here (cuisernaire?)
ed = edilate(e, closing_distance, 'same');
ed = 1-edilate(1-ed, closing_distance);
myclf
imshow(ed);

// connected components 

[L, n] = bwlabel(ed);

// Eliminate regions smaller than min_area pixels (noise)
for i=1:n
  f = find(L==i);      // linear coordinates of i-th region
  reg_size = size(f,'*');
  if reg_size < min_area
     L(f) = 0;         // merge small regions with the background
  end
end

myclf
imshow(L+1, rand(n+1,3));   // note how the small regions are gone

// embryo nucleii crust
ec = 1 * (L > 0.5);

//myclf;
//imshow(e,[]);
