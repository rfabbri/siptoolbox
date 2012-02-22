// default parameters to be used in our system

SIPVIEWER = 'display';
sip_quiet();
[size_y,size_x] = size(im);

// dimensions and areas for scale-independent parameters
max_dim = max(size_x,size_y);
max_area = max_dim*max_dim;

// scaling ratios for the 'normal' 1024x1024 image
area_ratio_normal = max_area/(1024*1024);
dim_ratio_normal = max_dim/1024;

// minimum area of connected components
min_area = 1200*area_ratio_normal;

// lowest distance that can make nuclei of interest connected
// todo: this could be made smaller.
closing_distance = 10*dim_ratio_normal;

// minimum edge length
min_length = 10*dim_ratio_normal;

// minimum possible tangential radius for a nucleus
min_possible_rt = 1.5*dim_ratio_normal;

// minimum ratio of tangential rt over normal radius rn
min_possible_rt_nt_ratio = 0.5;

// working directory
workdir = '/home/rfabbri/b';
workdir = workdir + '/';

