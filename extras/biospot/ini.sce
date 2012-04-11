//
// TODO: option to subst imshow's to imwrites to a results dir
//


// im: pixels used to generate masks, segmentations, etc
im = gray_imread('normal.tif');

// im_signal: pixels that will be integrated
im_signal = im;


//im = normal(im);

// default parameters
exec params.sce;

xset('auto clear', 'on')
chdir(workdir);

exec myclf.sci;

// -----------------------------------------------------------------------------
// compute the region containing the nucleii, eliminating most noise inside the
// embryo.
exec embryo_crust.sce;

// -----------------------------------------------------------------------------
// estimate thickness through the DT
//
// Idea (partially as implemented)
//
// 1 - compute DT of the inverse ec
// 2 - compute the skeleton
// 3 - median distance along the skeleton is one of your nucleus radius rn along
//     the normal
//   - perhaps have to increase the scale of the skeleton slightly
// 4 - The central line could be obtained as the levelset of the DT at rn, or as
// the skeleton at maximum scale which preserves loops (this is what we did)!!


[skl,dt,vor] = skel(ec);

// Smallest ammount of detail as possible - loop preserving
sklt = (skl >= max(skl));

// max(skl) is an indicator of the maximum thickness, or we could read this from
// DT.

myclf
imshow(ec+sklt,[]);

myclf
imshow(e+2*sklt,[]);

myclf
imshow(e.*sklt,[]);

sk_dt = bwdist(1-1*sklt);
myclf
imshow(e.*(sk_dt<=4),[]);

myclf
imshow(im.*(sk_dt<=4),[]);

// TODO: improve error detection if no complete loop was found. Attempt:
if (size(find(sklt),'*') / size(find(ec==1),'*') < 0.01)
  error('embryo crust does not seem to be a closed loop');
end

// -----------------------------------------------------------------------------
// filter out very small connected components in the edge map

[L, n] = bwlabel(e);

// eliminate edge segments smaller than min_length pixels (noise)
for i=1:n
  f = find(L==i);      // linear coordinates of i-th region
  reg_size = size(f,'*');
  if reg_size < min_length
     L(f) = 0;         // merge small regions with the background
  end
end

myclf
imshow(L+1, rand(n+1,3));   // note how the small regions are gone

// filtered embryo
ef = 1 * (L > 0.5);

// -----------------------------------------------------------------------------
// follow the border and read pixel values to generate a 1D signal

[x,y] = follow(sklt*1);

dims = size(sklt);
s = zeros(x); // signal from edgemap
ds = s; // distance signal (from edgemap's dt)
os = s; // original signal

for i=1:size(x,'*') do
   ij = [dims(1) - y(i), x(i)+1];
   s(i) = ef(ij(1),ij(2));
   ds(i) = dt(ij(1),ij(2));
   os(i)    = im(ij(1),ij(2));
end
figure
clf

disp('median nucleii thickness along normal direction:')
rn = sqrt(median(ds))

len=arclength([x y], %t);
plot(len,[s;s(1)]);

plot(len,[os;os(1)]);
ylabel('original signal');

// -----------------------------------------------------------------------------
// let's first give an estimate through the median spacing between edges along
// the medial line

zc = find(s<>0);

zc_len = len(zc);

n_zc = length(zc);

widths = zc_len(2:n_zc) - zc_len(1:(n_zc-1));


disp('median nucleii thickness along tangential direction:')
median_rt = median(widths)/2;


disp('The nucleus prototype has diameters: ' + string(median_rt*2) + ' by ' + string(rn*2) + ' pixels');

if median_rt < min_possible_rt
  warning('this edge image must have excessive ammounts of noise or has a weak signal, giving too small of an estimate for the tangential radius.');
end

if median_rt/rn < min_possible_rt_nt_ratio
  warning('tangential radius seems too low.');
end

// -----------------------------------------------------------------------------
// segment (hard) along tangential direction

exec tangential_align_using_edges.sce;

//exec tangential_align_using_original_image.sce;

// -----------------------------------------------------------------------------
// sliding ellipses along contour

// compute tangent directions along contours so we can place the minor axes of
// the nuclei prototypes (ellipses)

npts = size(x,'*');
crv = [x,y];
dt = crv(1:npts-1,:)-crv(2:npts,:);
dt = sqrt(dt(:,1).^2 + dt(:,2).^2);

tgt=tangent(x,y, 1, mean(dt));

fig;
clf;
plot(x,y);
set(gca(), "isoview","on");
xset("auto clear","off");

f=gcf();f.pixmap='on';
for i=1:npts
  champ(x(i),y(i),tgt(i,1),tgt(i,2),0.5);
end
show_pixmap();

// -----------------------------------------------------------------------------
// integration of the nearest pixels to each medial axis pixel
//
// 0- compute the filtered/reconstructed crust
//    TODO: implement medial axis reconstruction

// 1- compute the DT/voronoi diagram of the medial axis constrained to this shell

[dt2,vor2] = bwdist(1-sklt*1);
clear dt2;
vor2 = vor2.*ec;


nearest_colormap = rand(max(vor2)+1,3);

// shows the nearest pixels to each pixel of the medial line
myclf;
imshow(vor2+1, nearest_colormap); 

// shows the color associated to each pixel of the medial line
myclf;
imshow(vor2.*(sklt*1)+1, nearest_colormap); 

// 2- add up all pixels having the same label


// Implementation 1: Too slow!
// TODO: this could have been faster if performed during label propagation
//is = zeros(max(vor2),1);
//for i=1:max(vor2)
//  is(i) = sum(im(find(vor2==i)));
//end


// Implementation 2: Single pass over the image

im_acc = zeros(im);
num_pix = zeros(im);
// reimplementation
for i=1:size(im,'*')
  im_acc(vor2(i)+1) = im_acc(vor2(i)+1) + im_signal(i); 
  num_pix(vor2(i)+1) = num_pix(vor2(i)+1) + 1;
end

// signal is now obtained by tracing x,y
is = zeros(size(x,'*'), 1);
for i=1:npts
  ij = [dims(1) - y(i), x(i)+1];
  is(i) = im_acc(ij(1),ij(2))/num_pix(ij(1),ij(2));
end

fig;
clf;
plot(len,[is;is(1)]);


[minx, id_minx] = min(x);
[maxx, id_maxx] = max(x);

// left tip of the embryo:  [minx y(id_minx)]
// right tip of the embryo: [maxx y(id_maxx)]

// todo: break the curve from up and bottom


// -----------------------------------------------------------------------------
// integration by sliding ellipses

// XXX under construction

//exec sliding_ellipse.sce;

// -----------------------------------------------------------------------------
// integration by sliding normal line segments

exec sliding_normal_lines.sce;
