//function [x,y,line_h,line2_v]=imxprofile(image,option,x_scale,y_scale,z_scale,lum_max)
//
// Improved version of improfile:
// interactively draws profiles of the image.
// option=0 (default) to plot profiles on the same window as the image,
// option=1 pour another window
// option=2 to plot gaussian interpolation on the same window
// (used in laser beam images)
//
// lum_max is the maximal luminance of the image (255 for 8bits).
// It's only a multiplication factor to allow direct measure of the luminance
// on the screen, because SIP normalizes all images to 0-1 range.
// The default is that maximum values are displayed at a quarter of the
// current image.
//
// NOTE: image(1,1) is the left up corner of the image
// and in plot2d, we start in the left down corner - be carefull if
// you want to adapt this function to your own purpose !
// 
// This function has been tested only with gray level images (and not indexed images)
//
//
// In order to be useable in scilab-2.6 (and not only in scilab-2.7)
// the function gaussprofile is not included in the function
// improfile, but is independant.
// It should change in the future as Scilab-2.6 becomes obsolete.
//
//
// ORIGINAL AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
//
// Changelog:
// Adding x_scale,y_scale and z_scale. Warning: incompatible with previous versions !


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
//
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $
//

Inorm=line/sum(line) ;
nb=max(size(line));//nb of elements on a line
R=1:nb;
R0= sum(R.*Inorm)
sigma=sqrt(sum(Inorm.*((R-R0).^2)))
w=2*sigma;
endfunction; //gaussprofile





function [x,yreturn,line_h,line2_v]=imxprofile(image,option,x_scale,y_scale,z_scale,lum_max)

if argn(2)==0 then
  error("imxprofile(image[,option,x_scale,y_scale,z_scale,lum_max])")
end;

if typeof(image) == 'hypermat'
   warning(' this function was created for gray level images.');
     warning(' the image is converted to gray to display profiles.'); 
   image=im2gray(image);
end

if ~exists('option','local') then
  option=0;
end


//initialisation
//test if a graphical window is present
alpha=winsid();
if size(alpha,1)==0 then
  imshow(image,[]);
end;//if no graphical window exists

win_nb_init=xget("window");//takes the window number
[ymax,xmax]=size(image);
rx=1:xmax;ry=1:ymax;



if ~exists('x_scale','local') then
x_scale=1;
end;
if ~exists('y_scale','local') then
y_scale=1;
end;
if ~exists('z_scale','local') then
z_scale=1;
end;


if ~exists('lum_max','local') then
  lum_max=max(ymax,xmax)/(4*max(image));//maximal luminance (default: display is on 1/4 of the image)
end



   
xset("alufunction",6);//animation

if (option==1) then //profiles are displayed in another window
  xset("window",win_nb_init+1);
  xset("alufunction",6);
  xset("window",win_nb_init);
end;//  


//Prepare the box to display coordinates
if (option==2) then
  figure('position',[0 0 340 80]);//display coordinates (list)
else
  figure('position',[0 0 340 60]);//display coordinates (text)
end;//end of the coordinate box

//initial answer for the xgetmouse()
rep=[0 0 -1];


firsttime=%t;

while rep(3)==-1 then
  rep=xgetmouse()
//Calculates the cross:
//xgetmouse returns a value in pixels (0-size), but 
//we need a row and column number
//verify that the mouse is inside the graphic window
  x=rep(1);y=rep(2);
        if x<1 then
	x=1;
      end;
      if x>xmax then
	x=xmax;
      end;
      if y<1 then
	y=1;
      end;
      if y>ymax then
	y=ymax;
      end;

    yreturn=ymax-y+1;//returns the real coordinate in the image matrix
    
    //Calculate profiles
    line_h=image(ymax-y+1,:);
    line_v=image(:,x);
    line2_v=line_v(ymax:-1:1);//invert the order of the matrix

    
    //Draw: if it's not the 1st time, we have to delete the previous plot
    if ~firsttime then
      xsegs(xv_old,yv_old);
      plot2d(1:xmax,lum_max*line_h_old,1,"000");//horizontal
      plot2d(lum_max*line2_v_old,1:ymax,1,"000");//vertical
    end;
    
    xv=[1 xmax x x];yv=[y y 1 ymax];
    xsegs(xv,yv);
    plot2d(1:xmax,lum_max*line_h,1,"000");//horizontal
    plot2d(lum_max*line2_v,1:ymax,1,"000");//vertical
    //End of draw
    xset('wshow');
    
    
    //Display the coordinates in a box:
    if (option==0)|(option==1) then
    display1="position (col,row) = ( "+string(round(x*x_scale))+" , "+string(round(yreturn*y_scale))+" ) ; value = "+string(0.01*round(z_scale*100*image(yreturn,x)));
      uicontrol(  'style','text',...
	  'string',display1,...
	  'position',[0,0,340,60],...
	  'fontsize',14);
    end;//if option=0 or 1

    
    //if option==1, draw profiles in a separate window
    if option==1 then
      xset("window",win_nb_init+1);
      
      //if it's not the 1st time, we have to delete the previous plot
      if ~firsttime then
	subplot(2,1,2);//2 lines, 1 column, 2nd subwindow
	plot2d(1:xmax,lum_max*line_h_old,style=1,strf="011",rect=[0,0,xmax,lum_max]);//horizontal
	subplot(2,1,1);//2 lines, 1 column, 1st subwindow
	plot2d(lum_max*line2_v_old,1:ymax,style=1,strf="011",rect=[0,0,lum_max,ymax]);//vertical
      end;//firsttime
      
      subplot(2,1,2);
      plot2d(1:xmax,lum_max*line_h,style=1,strf="011",rect=[0,0,xmax,lum_max]);//horizontal
      subplot(2,1,1);
      plot2d(lum_max*line2_v,1:ymax,style=1,strf="011",rect=[0,0,lum_max,ymax]);//vertical
      xset("window",win_nb_init);
    end;//if option==1
        
    //if option==2, add gaussian profiles
    if (option==2) then
        //gaussian profile horizontal
      [R0h,wh]=gaussprofile(line_h);
      //Now, we adapt scales to have a pleasant view of the result
       gauss=exp(-2*((rx-R0h).^2)/wh.^2);//gaussian curve, max=1
       gaussnorm_x=gauss/sum(gauss);  //normalised gaussian, ie sum(gaussnorm)=1
     //gaussian profile vertical
       [R0v,wv]=gaussprofile(line2_v');//gaussprofile accepts line matrix
       //Now, we adapt scales to have a pleasant view of the result
       gauss=exp(-2*((ry-R0v).^2)/wv.^2);//gaussian curve, max=1
       gaussnorm_y=gauss/sum(gauss);  //normalised gaussian, ie sum(gaussnorm)=1
       
       //Delete the previous plot if it's not the 1st time
       if ~firsttime then 
	 plot2d(1:xmax,lum_max*gaussnorm_x_old*sum(line_h_old),1,"000");//gaussian profile horizontal
	 plot2d(lum_max*gaussnorm_y_old*sum(line2_v_old),1:ymax,1,"000");//gaussian profile
       end;//firsttime
       
       //draw gaussian profiles
       plot2d(1:xmax,lum_max*gaussnorm_x*sum(line_h),1,"000");//gaussian profile horizontal
       plot2d(lum_max*gaussnorm_y*sum(line2_v),1:ymax,1,"000");//gaussian profile
       
       
       //Display coordinates and wh and wv values
           display1="position (col,row) = ( "+string(round(x*x_scale))+" , "+string(round(yreturn*y_scale))+" ) ; value = "+string(0.01*round(z_scale*100*image(yreturn,x)));

//       display1="position (col,row) = ( "+string(round(x))+" , "+string(round(yreturn))+" ) ; value = "+string(0.01*round(100*image(yreturn,x)));
       display2="wh = "+string(0.01*round(x_scale*100*wh))+" ; wv = "+string(0.01*round(y_scale*wv*100));
       h=uicontrol(  'style','listbox',...
	   'position',[0,0,340,80],...
	   'fontsize',14);
       set(h,'string',display1+"|"+display2);
     
       //Memorize the value to be able to delete the previous plot
       gaussnorm_x_old=gaussnorm_x;
       gaussnorm_y_old=gaussnorm_y;

     end;//option==2


       
  
    //memorize values to delete the previous plot
    xv_old=xv;yv_old=yv;
    line_h_old=line_h;  
    line2_v_old=line2_v;

    firsttime=%f;


    
  end;//while

  close();
  xset("alufunction",3);


endfunction
