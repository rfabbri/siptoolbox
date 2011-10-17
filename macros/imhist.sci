function [counts, cells] = imhist(im, varargin)
//
// Compute and plot the histogram of an image
// imhist(im)
// imhist(im, 10)
// imhist(im, 10, 0.8)
// imhist(im, 10, '')
// imhist(im, 10, 'green')
// imhist(im, 10, 0.8, 'green')
//
// TODO
//  - remove the necessity of a second parameter just to get to the third,
//    ie, remove the need for imhist(im,10,'');
//  - enable a better x-axis labeling, perhaps adding a colorbar
//  - accept color images as input
//  - try plot2d3, perhaps output bars with just pure color (e.g. black),
//  without boundaries
//  - perhaps make axis a different color (e.g. gray)
//
  if(size(size(im),2)>2)
    error("The input matrix im should be a 2D matrix.");
  end

  bin=[];
  //if user specified 
  if length(varargin)>=1 then
    if varargin(1)~=floor(varargin(1)) then
      error("The second argument must be an integer.");
    elseif varargin(1)<=0 then
      error("The second argument must be a positive integer.");
    end
    bin = varargin(1);
  //elseif length(varargin)>2 then
  //  error("Too many arguments.");
  end

  imtype = typeof(im(1));
  
  select imtype
   case 'boolean' then
    if (isempty(bin) | bin>2) then, bin=2; end
    cells = linspace(0,1,bin);
   case 'uint8' then
    if isempty(bin) then, bin=2^8; end
    cells = linspace(0, 255, bin);
   case 'int8' then
    if isempty(bin) then, bin=2^8; end
    cells = linspace(-128, 127, bin);
   case 'uint16' then
    if isempty(bin) then, bin=2^16; end
    cells = linspace(0, 2^16-1, bin);
   case 'int16' then
    if isempty(bin) then, bin=2^16; end
    cells = linspace(-2^15, 2^15-1, bin);
   case 'int32' then
    if isempty(bin) then, 
      bin=2^16; 
      printf("The number of bins is set to 2^16 for int32 images."); 
    end
    cells = linspace(-2^31, 2^31-1, bin);
   case 'constant' then
    if isempty(bin) then, bin=10; end
    cells = linspace(0, 1, bin);
  else
    error("Data type " + imtype + " is not supported.");
  end
  
  if(bin > 1) then
    //such as for 256 bins of uint8 image
    //cells=[0,1,2,...,254,255]
    //new_cells=[-0.5, 0.5, 1.5, 2.5, ..., 254.5, 255.5]
    step = cells(2)-cells(1);
    new_cells = [cells, cells(length(cells))+step ] - step/2;
    [tmp, counts, info]=dsearch(double(im), new_cells);
  elseif(bin ==1) then
    counts=length(im);
  end
    
  cells = cells';
  counts = counts';
  
  if length(varargin)>1 then
    bar(cells, counts, varargin(2:length(varargin)));
  end
endfunction

// -------------------------------------------------------------------------
// SIP - Scilab Image Processing toolbox
// Copyright (C) 2002-2009  Ricardo Fabbri
//
// Modified starting October 15, 2011 by merging code back from:
//
// SIVP - Scilab Image and Video Processing toolbox
// Copyright (C) 2006  Shiqi Yu
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
