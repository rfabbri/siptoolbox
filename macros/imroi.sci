function mask=imroi(image,name,option)
//
// Selects a Region of Interest (ROI) in an image ALREADY DISPLAYED
// and returns the corresponding binary matrix.
//
// The mask is working either with gray or color images,
// but for color images: but image.*mask is not working with Scilab2.6
// one possibility is:
// image(1,1,1)=image(1,1,1);
// then you can do
// image.*mask;
// Scilab-2.7 causes no problem.
//
// name is the name of the selection tool: only 'rect' and 'ellipse' are
// avalaible at the moment
//
// Option:0 (default) for no coordinates
// 1 to display coordinates
//
// AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// The code to display coordinates comes directly from the function xgetpixel
// by Ricardo Fabbri
//
//
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $
// Changelog: 
// rev1.6 ellipse selection added
//



if (argn(2)==0|argn(2)>3) then
  error("Wrong number of arguments: mask=imroi(image[,type,option])")
end

disp('Select the ROI (Region Of Interest) on the Image')


if ~exists('name','local') then name='rect';end//default:rectangular selection

if ~exists('option','local') then option=0;end//default:no coordinates displayed


if (name=='rect')|(name=='ellipse') then
//select name
//case 'rect' then
  xset("alufunction",6);//mode XOR for animation

  
  [ymax,xmax]=size(image);
  [but,x0,y0]=xclick();//starting point

  //Do we click inside the graphical window ?
  if x0<0 then
    x0=1;
  end
  if x0>xmax then
    x0=xmax;
  end
  if y0<0 then
    y0=1;
  end
  if y0>ymax then
    y0=ymax;
  end

  //Initialisation
  x=x0;y=y0;
  xi=x0;yi=y0;
  w=x-xi;h=yi-y;

  stop=%f;

  if option==1 then
    figure('position',[0 0 400 30]);
  end

  while ~stop
    p=xgetmouse();

    if name=='rect' then
      xrects([xi;yi;abs(w);abs(h)],1);//delete the previous rect
    end
    if name=='ellipse' then
      a1=0;a2=360*64;//draw filled ellipses
      xfarcs([xi;yi;abs(w);abs(h);a1;a2],1);//delete the previous ellipse
    end
    x=p(1); y=p(2);

  
    //Don't go outside
    if x<0 then
      x=0;
    end
    if x>xmax then
      x=xmax;
    end
    if y<0 then
      y=0;
    end
    if y>ymax then
      y=ymax;
    end
    
    if option==1 then
      display1='Origin: ( '+string(round(ymax-y0+1))+' row, '+string(round(x0))+' col) - Actual '+'('+string(round(ymax-y+1))+' row , '+string(round(x))+' col)';
      uicontrol(  'style','text',...
	  'string',display1,...
	  'position',[0,0,400,30],...
	  'fontsize',14);
    end
  

  
    //Draw rect
    w=x-x0;h=y0-y;
    if (w<0) then
      xi=x;
    else
      xi=x0;
    end
    if (h<0) then 
      yi=y;
    else
      yi=y0;
    end
    
    if name=='rect' then
      xrects([xi;yi;abs(w);abs(h)],1);
    end
    if name=='ellipse' then
      xfarcs([xi;yi;abs(w);abs(h);a1;a2],1);
    end
    stop=(p(3)>=0);
  end//while

  close();



  //mask
  mask=zeros(image);

  //Define extremities
  if (w<0) then
    xincr=-1;
  else
    xincr=+1;
  end  
  
  if (h<0) then
    yincr=-1;
  else
    yincr=+1;
  end


  //create the binary mask
  //Conditions at limits
  y1=ymax-round(y0);
  y2=ymax-round(y);
  if y1<0.5 then y1=1;end
  if y2<0.5 then y2=1;end
  if x<0.5 then x=1;end

  if name=='rect' then 
    if typeof(image) == 'hypermat' then
      //for color images
      mask(y1:yincr:y2,round(x0):xincr:round(x),1:3)=1;
    else
      //for grayscale images
      mask(y1:yincr:y2,round(x0):xincr:round(x))=1;
    end
  end//rectangular selection

  if name=='ellipse' then
    xc=x0+(x-x0)/2;yc=y1+(y2-y1)/2;//center of the ellipse
    a=abs(w/2);b=abs(h/2);//small and large axes of the ellipse
    //for the mask: calculating in cylindrical coordinates
    xm=((1:xmax)'*ones(1:ymax))';
    ym=((1:ymax)'*ones(1:xmax));
    r=sqrt(((xm-xc)/a).^2+((ym-yc)/b).^2);
    premask=zeros(r);
    premask(find(r<=1))=1;//definition for an ellipse
    if typeof(image) == 'hypermat' then
      //for color images
      mask(:,:,1)=premask;mask(:,:,2)=premask;mask(:,:,3)=premask;
    else
      //for grayscale images
      mask=premask;
    end
  end//ellipse selection
  
  xset("alufunction",3);//back to normal mode

else //if name not known
  printf('Not implemented yet. Sorry...')
end

  
endfunction


//
// -------------------------------------------------------------------------
// SIP - Scilab Image Processing toolbox
// Copyright (C) 2002-2004  Ricardo Fabbri
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
