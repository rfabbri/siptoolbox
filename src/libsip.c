#include "sip_common.h"
#include <mex.h> 
#ifdef SIP_HAVE_SCI_GATEWAY_H
  #include <sci_gateway.h> 
#endif
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
SipExport extern Gatefunc int_imread;
SipExport extern Gatefunc int_imwrite;
SipExport extern Gatefunc int_imfinfo;
SipExport extern Gatefunc imvariance_int;
SipExport extern Gatefunc unwrapl_c_int;
SipExport extern Gatefunc unwrapp_c_int;
SipExport extern Gatefunc mogrify_int;
SipExport extern Gatefunc edilate_int;
SipExport extern Gatefunc percol_int;
SipExport extern Gatefunc drawline_int;
SipExport extern Gatefunc skel_int;
SipExport extern Gatefunc thin_int;
SipExport extern Gatefunc bwdist_int;
SipExport extern Gatefunc watershed_int;
SipExport extern Gatefunc bwlabel_int;
SipExport extern Gatefunc rgb2hsv_int;
SipExport extern Gatefunc hsv2rgb_int;
SipExport extern Gatefunc sip_setenv_int;
SipExport extern Gatefunc sip_set_verbose_int;
SipExport extern Gatefunc sip_get_verbose_int;
SipExport extern Gatefunc hello_int;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,int_imread,"imread"},
  {(Myinterfun)sci_gateway,int_imwrite,"imwrite"},
  {(Myinterfun)sci_gateway,int_imfinfo,"imfinfo"},
  {(Myinterfun)sci_gateway,imvariance_int,"imvariance"},
  {(Myinterfun)sci_gateway,unwrapl_c_int,"sip_unwrapl_c"},
  {(Myinterfun)sci_gateway,unwrapp_c_int,"sip_unwrapp_c"},
  {(Myinterfun)sci_gateway,mogrify_int,"mogrify"},
  {(Myinterfun)sci_gateway,edilate_int,"edilate"},
  {(Myinterfun)sci_gateway,percol_int,"percol"},
  {(Myinterfun)sci_gateway,drawline_int,"drawline"},
  {(Myinterfun)sci_gateway,skel_int,"skel"},
  {(Myinterfun)sci_gateway,thin_int,"thin"},
  {(Myinterfun)sci_gateway,bwdist_int,"bwdist"},
  {(Myinterfun)sci_gateway,watershed_int,"watershed"},
  {(Myinterfun)sci_gateway,bwlabel_int,"bwlabel"},
  {(Myinterfun)sci_gateway,rgb2hsv_int,"rgb2hsv"},
  {(Myinterfun)sci_gateway,hsv2rgb_int,"hsv2rgb"},
  {(Myinterfun)sci_gateway,sip_setenv_int,"sip_setenv"},
  {(Myinterfun)sci_gateway,sip_set_verbose_int,"sip_set_verbose"},
  {(Myinterfun)sci_gateway,sip_get_verbose_int,"sip_get_verbose"},
  {(Myinterfun)sci_gateway,hello_int,"hello_sip"},
};
 
SipExport int C2F(libsip)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
