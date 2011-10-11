label = color_classify(RGB)
//
// Classifies an image window into Red, Green, Blue, and others.
// Should work well under different lighting conditions and is robust to wrong white
// balance. Should be simple enough for real time applications.
//
// AUTHOR 
//  Ricardo Fabbri <rfabbri@gmail.com>
//
// USAGE
//
// Input: RGB
// Dois modos de uso: 
//
// a) descomentar uma das linhas de imread abaixo
// e ler a imagem que voce quer, editando o nome 
//
// b) exec classify-run.sce pra rodar em varias imagens


// median is a little better:

r = median(RGB(:,:,1));
g = median(RGB(:,:,2));
b = median(RGB(:,:,3));

//r = mean(RGB(:,:,1));
//g = mean(RGB(:,:,2));
//b = mean(RGB(:,:,3));

hsv = rgb2hsv([r g b]);

hue = hsv(1);
sat = hsv(2);
val = hsv(3);

hue = hue * 360;

if val < 0.2 & sat < 0.8
  label = 'black';
  exit;
elseif val > 0.8 & sat < 0.1
  label = 'white';
  exit;
end

// At this point we have
// val > 0.2 | (val < 0.8 | sat > 0.1)

if hue < 30 | hue > 330
  label = 'red';
elseif hue > 90 & hue < 150
  label = 'green';
elseif hue > 185 & hue < 270
  label = 'blue';
else // disp 'unreliable color...';
  if sat < 0.5
    warning('unreliable!')
    // in the real system you just discard this estimate and use the previous
    // estimate (e.g. previous frame) at this point
  end
  if hue >= 30 & hue <= 90
    label = 'guessing yellow';
  elseif hue >= 150 & hue <= 185
    if hue >= 180
      label = 'guessing blue'
    else
      label = 'guessing blue-green'
    end
  elseif hue >= 270 & hue <= 330
    label = 'guessing purple-pink'
  end
end

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
