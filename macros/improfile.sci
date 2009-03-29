//function [line_h,line2_v]=improfile(image,option,lum_max)
//
// Draws profiles of the image
// example: improfile(imread("my_picture.tif"),1);
// option=0 (default) to plot profiles on the same window as the image,
// option=1 pour another window
// option=2 to plot gaussian interpolation on the same window
// (used in laser beam images)
//
// lum_max is the maximal luminance of the image (255 for 8bits).
// It's only a multiplication factor to allow direct measure of the luminance
// on the screen, because SIP normalizes all images to 0-1 range.
// The default is 255 (8bits image).
//
// NOTE: image(1,1) is the left up corner of the image
// and in plot2d, we start in the left down corner - be carefull if
// you want to adapt this function to your own purpose !
// 
// This function has been tested only with gray level images.
//
// Thanks to Ricardo for adding nice colors
//
// In order to be useable in scilab-2.6 (and not only in scilab-2.7)
// the function gaussprofile is not included in the function
// improfile, but is independant.
// I prefer this solution, as scilab-2.7 is rather young.
//
// ORIGINAL AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $
//
// Changelog:
// Adapted to the new conventions in SIP: all images are in 0-1 range
// Error message rewritten
// lum_max parameter added



function [R0,w]=gaussprofile(line)
//
// line is a line (and not column) matrix
//
// Determines the radius  w of a gaussian laser beam profile (or line) 
//(defined by I0/e^2, ie about 13,6% of the maximum).
//
// A laser beam (in mode TEM00) has a gaussian intensity repartition as follow:
// I(R) = I0 * exp ((-2*R^2)/w^2) where "w" is called the radius of the beam
// If the beam is not centered on R=0 but on R=R0, the expression becomes:
// I(R) = I0 * exp((-2(R-R0)^2)/w^2)
// In general, a gaussian distribution is written:
// G(R) = A * exp (-(R-R0)^2/(2*sigma^2)
// where sigma is the standard deviation
// The value of sigma gives the value of "w"
//
// The method is:
// 1) Calculate Inorm (normalised intensity):
// Inorm(R)  =I(R)/Itotal
// 2) Calculate R0 (center of the gaussian)
// R0 = sum (R*Inorm(R))
// 3) Standard deviation is defined by
// sigma^2 = sum (Inorm(R)*(R-R0)^2)
// 4) w = 2*sigma
//
// REFERENCES: John Taylor: "Incertitudes et analyse des erreurs dans les 
// mesures physiques" (DUNOD) 
// translated from
// "An introduction to Error Analysis" (University Science Books)
// 
// AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// TODO / KNOWN BUGS
//    - if I call improfile with no graphical window present, I 
//      have the title "left click to select..."
//      If a graphicalwindow is already present, I see the image, 
//      but I don't have this title (I have no title in fact).
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $
//

Inorm=line/sum(line) ;
nb=max(size(line));//nb of elements on a line
R=1:nb;
R0= sum(R.*Inorm)
sigma=sqrt(sum(Inorm.*((R-R0).^2)))
w=2*sigma;
endfunction; //gaussprofile


function [line_h,line2_v]=improfile(image,option,lum_max)



if argn(2)==0 then
  error("improfile(image[,option,lum_max])")
end;

if typeof(image) == 'hypermat'
   warning(' this function has been tested only with gray level images.')
end

if ~exists('option','local') then
  option=0;
end
if ~exists('lum_max','local') then
  lum_max=255;//maximal luminance (default: the image is 8bits)
end



xbasc();xselect();
imshow(image,strf='041'); //same as imshow but draw axes
xtitle(["Left click to select a point";"Right click to stop"]);
win_nb_init=xget("window");//takes the window number
[r,c]=size(image);

while %t
  position=xclick();
  position_x=round(position(2));
  position_y=round(position(3));
  
  if position(1)==2 then  //right click for stop
    break;
  end; //if (verifying right click)
  
  //verify if we don't click outside the image
  if position_x<1 | position_x>c | position_y<1 | position_y>r
    disp("don''t click outside image");
    xbasc();xselect();imshow(image,strf='041');
    xtitle(["Don''t click outside image";"Right click for stop"]);
    titlepage("Don''t click outside image");
  else
    xbasc();xselect();imshow(image,strf='041');
    
    //Draw a cross on mouse location
    // Horizontal line
    red_id=addcolor([1 0 0]);
    green_id = addcolor([0 1 0]);
    x=1:c;
    plot2d(x,ones(x)*position_y,red_id,"000"); //draw line on mouse location
    // first line of the image is located at last line in the matrix:
    line_h=image(r-position_y,:);

    //Vertical line
    y=1:r;
    plot2d(ones(y)*position_x,y,red_id,"000");//draw line on mouse location
    line_v=image(:,position_x);
    line2_v=line_v(size(line_v,'r'):-1:1);//invert the order of the matrix

    

    // same window or new window ?
    if option==0 then
      plot2d(x,lum_max*line_h,red_id,"000");//horizontal
      plot2d(lum_max*line2_v,y,green_id,"000");//vertical
      xtitle(["Right click to stop";"x = "+string(position_x)+"; y = "+string(position_y)]);
    end;
	
    if option==1 then
      xtitle(["Right click to stop";"x = "+string(position_x)+"; y = "+string(position_y)]);
      xset("window",win_nb_init+1);
      xbasc();
      color=5;//5=red (see xset())
      plot2d(x,lum_max*line_h,color);
      xtitle(["Horizontal Profile";"x = "+string(position_x)]);
      xset("window",win_nb_init+2);
      xbasc();
      plot2d(lum_max*line2_v,y,color);
      xtitle(["Vertical Profile";"y = "+string(position_y)]);
    end;

    
    if option==2 then 
      //first: display profiles
      plot2d(x,lum_max*line_h,red_id,"000");//horizontal
      plot2d(lum_max*line2_v,y,green_id,"000");//vertical

      //gaussian profile horizontal
      [R0h,wh]=gaussprofile(line_h);
      //Now, we adapt scales to have a pleasant view of the result
       gauss=exp(-2*((x-R0h).^2)/wh.^2);//gaussian curve, max=1
       gaussnorm=gauss/sum(gauss);  //normalised gaussian, ie sum(gaussnorm)=1
       plot2d(x,lum_max*gaussnorm*sum(line_h),red_id,"000");//gaussian profile horizontal
       //end gaussian profile horizontal
       //gaussian profile vertical
       [R0v,wv]=gaussprofile(line2_v');//gaussprofile accepts line matrix
       //Now, we adapt scales to have a pleasant view of the result
       gauss=exp(-2*((y-R0v).^2)/wv.^2);//gaussian curve, max=1
       gaussnorm=gauss/sum(gauss);  //normalised gaussian, ie sum(gaussnorm)=1
       plot2d(lum_max*gaussnorm*sum(line2_v),y,green_id,"000");//gaussian profile
       //end gaussian profile vertical
       
       xtitle(["Right click to stop";
	   "x = "+string(position_x)+"; y = "+string(position_y);
	   "wh= "+string(wh)+"; wv= "+string(wv)]);

     end;

    
  end;//verify if not clicking outside the image

  xset("window",win_nb_init);
  
end;//while %t

xset("window",win_nb_init);

endfunction;//improfile

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
