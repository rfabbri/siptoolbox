function [labeled_img, cmap, uncertainty_level] = color_segmentation(RGB, method)
// Performs color detection / segmentation in an image
//
// AUTHOR 
//  Ricardo Fabbri <rfabbri@gmail.com>
//
// USAGE
// 
// INPUT
//  RGB - truecolor image (3D array)
//  method - 'hsv_sip' (default), 'distance_to_reference'
//
// OUTPUT
//  labeled_rgb, cmap - labeled_rgb is a 2D array the same size as RGB, where at (i,j) we have a
//  reference color id (working as a segmentation label) to the real color in
//  the colormap cmap. The color code is as follows:
//
//  labeled_img(i,j)     cmap(labeled_img(i,j))
//  1                    [1 0 0]  (red)
//  2                    [0 1 0]  (green)
//  3                    [0 0 1]  (blue)
//  4                    [0 0 0]  (black)
//  5                    [1 1 0]  (yellow)
//  6                    [1 1 1]  (white)
//
//  you can show the segmentation using imshow(labeled_img, cmap);
//   
//  uncertainty_level - a 2D array the same size as RGB, where at (i,j) it is 0
//  for fully certain, 1 is a good guess, 2 is uncertain
//
select argn(2)
  case 0
    error('Invalid number of arguments.')
  case 1
    method = 'hsv_sip';
end

id_red = 1;
id_green = 2;
id_blue = 3;
id_black = 4;
id_yellow = 5;
id_white = 6;

id_certain = 0;
id_good_guess = 1;
id_unreliable = 2;

r = size(RGB,1)
c = size(RGB,2)

labeled_img = zeros(r,c);
uncertainty_level = zeros(r,c);

for i=1:r
  for j=1:c
    [label, certainty_level] = color_classify_single(RGB(i,j,:), 'hsv_sip');

    select label
    case 'red'
      labeled_img(i,j) =  id_red;
    case 'green'
      labeled_img(i,j) =  id_green;
    case 'blue'
      labeled_img(i,j) =  id_blue;
    case 'black'
      labeled_img(i,j) =  id_black;
    case 'yellow'
      labeled_img(i,j) =  id_yellow;
    case 'white'
      labeled_img(i,j) =  id_white;
    else
      error('Invalid color class returned for ' + string(i) + ', ' + string(j));
    end

    select certainty_level
    case 'certain'
      uncertainty_level(i,j) = id_certain;
    case 'good guess'
      uncertainty_level(i,j) = id_good_guess;
    case 'unreliable'
      uncertainty_level(i,j) = id_unreliable;
    else
      error('Unknown certainty label returned for ' + string(i) + ', ' + string(j));
    end
  end
end

cmap = zeros(6,3);
cmap(id_red,:) = [1 0 0];
cmap(id_green,:) = [0 1 0];
cmap(id_blue,:) = [0 0 1];
cmap(id_black,:) = [0 0 0];
cmap(id_yellow,:) = [1 1 0];
cmap(id_white,:) = [1 1 1];

endfunction
