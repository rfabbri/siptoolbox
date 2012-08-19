chdir(SIPDIR + 'images');
//grayscale image
Img  = gray_imread('pde.jpg');
Img1 = deskew(Img);

//True Color image
img  = imread('pde.jpg');
img1 = deskew(img);

//Indexed image
[im,map]   = imread('pde.png');
[im1,map1] = deskew(im,map);

//The above functions works for all the
//three types of image and has been tested.
chdir(SIPDIR + 'macros');
