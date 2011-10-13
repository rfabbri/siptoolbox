function [label, certainty_level, confidence, secondary_label] = color_classify(RGB, method, aggregation)
//
// Classifies an image window into a single color: Red, Green, Blue, Yellow,
// Black, or others.
// Should work well under different lighting conditions and is robust to wrong white
// balance. Should be simple enough for real time applications.
//
// AUTHOR 
//  Ricardo Fabbri <rfabbri@gmail.com>
//
// USAGE
//
// INPUT 
//  RGB - truecolor image
//  method - 'hsv_sip' (default), 'distance_to_reference'
//  aggregation - 'median' (default), 'mean', 'histogram', 'ransac'
//
// OUTPUT 
//
//  label - 'red', 'green', 'blue', 'yellow', 'black', 'white' or something else
//
//  certainty_level - 'certain', 'good guess', 'unreliable'.
//
//  confidence - a scalar from 0 to 1 indicating the confidence in the guess,
//  for those algorithms that enable it; for other algorithms this is just -1.
//   
//  secondary_label - a secondary class label for the color when ambiguity
//  occurs. This will only be set if certainty_level <> 'certain'. Multiple
//  labels can be present here and are separated by '-'.
//
//  TODO
//   perhaps a mask option.

select argn(2)
  case 0
    error('Invalid number of arguments.')
  case 1
    method = 'hsv_sip';
    aggregation = 'median'
  case 2
    aggregation = 'median'
end

select aggregation
case 'median'
  r = median(RGB(:,:,1));
  g = median(RGB(:,:,2));
  b = median(RGB(:,:,3));
case 'mean'
  r = mean(RGB(:,:,1));
  g = mean(RGB(:,:,2));
  b = mean(RGB(:,:,3));
case 'histogram'
case 'ransac'
  error('not yet implemented');
else
  error('invalid method');
end

// TODO instead of median can use a histogram-style result,
// or, if too slow, a RANSAC-style approximation to it

[label, certainty_level, confidence, secondary_label] = ...
  color_classify_single([r g b], method);


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
