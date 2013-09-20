// libsio_build.sce:  defines sources and scilab function names
// to build libsip C library. This file is to be called from 
// the "bootstrap" script. It is NOT ran by configure.
//
mode(-1);

chdir(get_absolute_file_path('libsip_build.sce') + '/../');
getd 'config';

chdir 'src';

ilib_name  = 'libsip' 		      // interface library name 
//sources = 'imread.c imwrite.c imfinfo.c mogrify_int.c ...
//           analysis_int.c morphology_int.c ...
//           unwrapp_int.c ...
//           unwrapl_int.c canny_int.c deskew_int.c dewarp_int.c';

sources = 'hello_int.c sip_common.c misc_int.c imvariance_int.c ...
           segment_int.c img_int.c analysis_int.c morphology_int.c ...
           unwrapp_int.c unwrapl_int.c ...
           imfinfo.c imread.c mogrify_int.c';

// table of (scilab_name, interface-name) 
table = [ ...
         'imread',     'int_imread';
//         'imwrite',    'int_imwrite';
          'imfinfo',    'int_imfinfo';
//          'deskew',      'int_deskew';
//          'dewarp',      'int_dewarp';
          'imvariance', 'imvariance_int';
          'sip_unwrapl_c', 'unwrapl_c_int';
          'sip_unwrapp_c', 'unwrapp_c_int';
          'mogrify',    'mogrify_int';
          'edilate',    'edilate_int';
          'percol',     'percol_int';
//          'canny_c',      'canny_int';
          'drawline',   'drawline_int';
          'skel',       'skel_int';
          'thin',       'thin_int';
          'bwdist',     'bwdist_int';
          'watershed',  'watershed_int';
          'bwlabel',    'bwlabel_int';
          'rgb2hsv',    'rgb2hsv_int';
          'hsv2rgb',    'hsv2rgb_int';
          'sip_setenv', 'sip_setenv_int';
          'sip_set_verbose', 'sip_set_verbose_int';
          'sip_get_verbose', 'sip_get_verbose_int';
          'hello_sip',  'hello_int'];

my_ilib_gen_gateway(ilib_name,table)

unix_g('../config/subst_input_files.sh ' + ilib_name + ' ' + sources);
chdir '../macros';
unix_g('../config/subst_input_files.sh');
chdir '../images';
unix_g('../config/subst_input_files.sh');
chdir '../images/photonics';
unix_g('../../config/subst_input_files.sh');
chdir '../../man';
unix_g('../config/subst_input_files.sh');

chdir '../config';
my_ilib_gen_loader(ilib_name,table,[])
