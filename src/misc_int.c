/*
 * =============================================================================
 *
 * $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $
 *
 * SIP - Scilab Image Processing Toolbox
 * Copyright (C) 2002-2009  Ricardo Fabbri <rfabbri@(not this part) gmail d0t com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * =============================================================================
 */ 

#include "sip_common.h"
#include <stdlib.h>
#include <string.h>

SipExport int
sip_setenv_int(char *fname)
{
   int rname,cname,pname,
       rval,cval,pval,
       minlhs=1, maxlhs=1, minrhs=2, maxrhs=2;

   CheckRhs(minrhs,maxrhs);
   CheckLhs(minlhs,maxlhs);

   GetRhsVar(1, "c", &rname, &cname, &pname); 
   GetRhsVar(2, "c", &rval, &cval, &pval);

   if ( setenv(cstk(pname), cstk(pval), 0) == -1 ) {
      sip_error("environment is already full");
      return false;
   }

   LhsVar(1)=0;
   
   return true;
}

/*
 * sets SIP verbosity level
 * TODO
 *    - even in quiet mode, all function should output useful 
 *    info/debug/error codes in a global variable (sip_ans_msg)
 */
SipExport int
sip_set_verbose_int(char *fname)
{
   int rname,cname,pname,
       rout,cout,
       minlhs=1, maxlhs=1, minrhs=0, maxrhs=1;

   char *prev;

   CheckRhs(minrhs,maxrhs);
   CheckLhs(minlhs,maxlhs);

   /* print previous value */
   switch (sip_verbose) {
      case SIP_WORDY:
         prev = "wordy";
         break;
      case SIP_QUIET:
         prev = "quiet";
         break;
      default:
         sip_error("BUG: internal variable has invalid value (please report)");
         break;
   }

   cout = 1;
   rout = strlen(prev);
   CreateVarFromPtr(2, "c", &rout, &cout, &prev);

   
   if (Rhs == 0) 
      /* toggle previous value */
      sip_verbose = (sip_verbose == SIP_QUIET) ? SIP_WORDY : SIP_QUIET;
   else { 
      /* set new value */
      GetRhsVar(1, "c", &rname, &cname, &pname); 
      if (cname == 1 && *cstk(pname) == 'q')
         sip_verbose = SIP_QUIET;
      else
         sip_verbose = SIP_WORDY;
   }

   LhsVar(1)=2;
   
   return true;
}

SipExport int
sip_get_verbose_int(char *fname)
{
   int rout,cout,
       minlhs=1, maxlhs=1, minrhs=0, maxrhs=0;

   char *pout;

   CheckRhs(minrhs,maxrhs);
   CheckLhs(minlhs,maxlhs);

   switch (sip_verbose) {
      case SIP_WORDY:
         pout = "wordy";
         break;
      case SIP_QUIET:
         pout = "quiet";
         break;
      default:
         sip_error("BUG: internal variable has invalid value (please report)");
         break;
   }

   cout = 1;
   rout = strlen(pout);
   CreateVarFromPtr(1, "c", &rout, &cout, &pout);

   LhsVar(1)=1;
   
   return true;
}
