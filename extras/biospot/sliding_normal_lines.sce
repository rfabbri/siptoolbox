// hack:
//rn = rn*5;
rt = 0.3*rn;

rt_rn_s = string(rt) + ',' + string(rn);

is_line = zeros(size(x,'*'), 1);

im_el = zeros(im);

// argv = [];
for i=1:npts
  angle_ellipse = string(atan(tgt(i,2),tgt(i,1))*180/%pi);
  angle_ellipse = '0';
  x_ellipse = string(x(i));
  y_ellipse = string(y(i));
  x_ellipse = '0';
  y_ellipse = '0';
  nrm = [-tgt(i,2), tgt(i,1)];
  xy_min = [x(i) y(i)] + rn*nrm;
  xy_max = [x(i) y(i)] - rn*nrm;

  xy_min = [dims(1) - xy_min(2), xy_min(1)+1];
  xy_max = [dims(1) - xy_max(2), xy_max(1)+1];
  
//   argv = [argv; '-stroke'     ; '#fff'
//           '-fill'       ; 'red' 
//           '+antialias'
// //          '-draw'       ; ' rotate ' + angle_ellipse ;
// //          '-draw'       ; 'ellipse ' + x_ellipse + ',' + y_ellipse +  ' ' + rt_rn_s + ' 0,360' 
//           '-draw'       ; 'polyline ' + string(xy_min(1)) + ',' + string(xy_min(2)) + ' ' + string(xy_max(1)) + ',' + string(xy_max(2));
//           ];
// 
//   im_el = zeros(im);
//   im_el = mogrify(im_el, argv);
//   im_el = im_el(:,:,1);

  
  im_el = drawline(im_el,[xy_min; xy_max]);
//xx  im_el_neq_0 = find(im_el <> 0);

// xx is_line(i) = mean(im(im_el_neq_0));
// xx im_el(im_el_neq_0) = 0;
end

fig
clf;
plot(len, [is_line; is_line(i)]);
imshow(im_el);
