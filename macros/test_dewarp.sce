chdir(SIPDIR + 'images');
//grayscale image
Img  = gray_imread('chapter.jpg');
Img1 = dewarp(Img);

//True Color image
img  = imread('chapter.jpg');
img1 = dewarp(img);

//Indexed image
[im,map]   = imread('chapter.png');
[im1,map1] = dewarp(im,map);

//The above functions works for all the
//three types of image and has been tested.
chdir(SIPDIR + 'macros');
