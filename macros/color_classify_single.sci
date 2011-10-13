function [label, certainty_level, confidence, secondary_label] = color_classify_single(RGB, method)
//
// Classifies a single RGB triplet into Red, Green, Blue, Yellow, Black, and others.
// Should work well under different lighting conditions and is robust to wrong white
// balance. Should be simple enough for real time applications.
//
// AUTHOR 
//  Ricardo Fabbri <rfabbri@gmail.com>
//
// USAGE
//
// INPUT 
//  RGB - [r g b] triplet
//  method - 'hsv_sip', 'distance_to_reference'
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

select argn(2)
  case 0
    error('Invalid number of arguments.')
  case 1
    method = 'hsv_sip';
end

certainty_level = 'certain';
confidence = -1.0;
secondary_label = '';


select method
// -------------------------------------------------------------------------
case 'hsv_sip'
  // median is better when the image has patches of other colors.
  // otherwise the performance is similar.

  r = RGB(1);
  g = RGB(2);
  b = RGB(3);

  hsv = rgb2hsv([r g b]);

  hue = hsv(1);
  sat = hsv(2);
  val = hsv(3);

  hue = hue * 360;

  label = '';
  if val < 0.3 & sat < 0.8
    label = 'black';
    if val > 0.2
      certainty_level = 'good guess'; 
      //label = label + '-';
      // will proceed below to guessing colors
      label = '';
    else
      return;
    end
  elseif val > 0.8 & sat < 0.2
    label = 'white';
    if sat > 0.1
      // light baby colors; could be white with offset colorbalance.
      certainty_level = 'unreliable'; 
      // will proceed below to guessing colors
      label = '';
      secondary_label = 'white';
    else
      return;
    end
  elseif val < 0.65 & sat < 0.15 // troublesome; looks like above
    if val < 0.50
      label = 'black'
      certainty_level = 'good guess';
      if val > 0.40
        secondary_label = 'gray';
      end
      return;
    else
      certainty_level = 'unreliable';
      secondary_label = 'gray';
    end
  end

  if hue < 30 | hue > 330
    label = label + 'red';
  elseif hue > 90 & hue < 160
    label = label + 'green';
    if sat < 0.2
      certainty_level = 'good guess';
    end
  elseif hue > 185 & hue < 270
    label = label + 'blue';
  else // disp 'unreliable color...';
    if sat < 0.5
      certainty_level = 'unreliable';
      if sip_get_verbose() == 'wordy'
        warning('unreliable!')
      end
      // in the real system you just discard this estimate and use the previous
      // estimate (e.g. previous frame) at this point
    end
    if hue >= 30 & hue <= 90
      label = label + 'yellow';
    else
      certainty_level = 'good guess';
      if hue >= 150 & hue <= 185
        // hard test near cyan and put a secondary label.
        if hue >= 180
          label = label + 'blue'
        elseif hue < 160
          label = label + 'green'
          secondary_label = 'blue';
        else
          label = label + 'blue'
          secondary_label = 'green';
        end
      elseif hue >= 270 & hue <= 330
        // hard test near magenta and put a secondary label.
        if hue < 280
          label = label + 'blue'
          secondary_label = 'purple-pink-lavender';
        else
          label = label + 'red'
          secondary_label = 'purple-pink-magenta';
        end
      end
    end
  end

// -------------------------------------------------------------------------
case 'distance_to_reference'
  error('not yet implemented');
end

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
