// hack:
//rn = rn*5;
rt = 0.3*rn;

rt_rn_s = string(rt) + ',' + string(rn);

im_el = zeros(im);
argv = [];
for i=1:3
  angle_ellipse = string(atan(tgt(i,2),tgt(i,1))*180/%pi);
  angle_ellipse = '0';
  x_ellipse = string(x(i));
  y_ellipse = string(y(i));
  x_ellipse = '0';
  y_ellipse = '0';
  nrm = [-tgt(i,2), tgt(i,1)];
  xy_min = [x(i) y(i)] + rn*nrm;
  xy_max = [x(i) y(i)] - rn*nrm;
  colorstr = dec2hex(i);
  select length(colorstr)
  case 1
    colorstr = '00' + colorstr;
  case 2
    colorstr = '0' + colorstr;
  case 4
    colorstr = '00' + colorstr;
  case 5
    colorstr = '0' + colorstr;
  end
  
  argv = [argv; '-stroke'     ; '#' + colorstr
          '-fill'       ; 'red' 
          '+antialias'
//          '-draw'       ; ' rotate ' + angle_ellipse ;
//          '-draw'       ; 'ellipse ' + x_ellipse + ',' + y_ellipse +  ' ' + rt_rn_s + ' 0,360' 
          '-draw'       ; 'polyline ' + string(xy_min(1)) + ',' + string(xy_min(2)) + ' ' + string(xy_max(1)) + ',' + string(xy_max(2));
          ];

end
im_el = mogrify(im_el, argv);

imshow(im_el);
