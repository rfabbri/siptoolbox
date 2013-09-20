#include "sip_common.h"
#include <mex.h> 
#ifdef SIP_HAVE_SCI_GATEWAY_H
  #include <sci_gateway.h> 
#endif
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
SipExport extern Gatefunc imvariance_int;
SipExport extern Gatefunc bwlabel_int;
SipExport extern Gatefunc sip_setenv_int;
SipExport extern Gatefunc sip_set_verbose_int;
SipExport extern Gatefunc sip_get_verbose_int;
SipExport extern Gatefunc hello_int;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,imvariance_int,"imvariance"},
  {(Myinterfun)sci_gateway,bwlabel_int,"bwlabel"},
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
