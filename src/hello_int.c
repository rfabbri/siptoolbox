/*
 * -------------------------------------------------------------------------
 * SIP - Scilab Image Processing toolbox
 * Copyright (C) 2002-2009  Ricardo Fabbri
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
 * -------------------------------------------------------------------------
 */ 
  

#include <animal/animal.h>
#include "sip_common.h"
#include <string.h>


/*-----------------------------------------------------------------
 * sum = hello_sip(a,b)
 *    This is an example of how to interface a C function to 
 * Scilab. It uses the sample "hello" routine from the AnImaL
 * library to calculate the sum of two numbers. For more examples,
 * please take a look at the directories "interface-tour-so"
 * and also "interface-tutorial-so", both under the "examples" 
 * directory of the scilab source tree. The file "builder.sce" has
 * the necessary information about the name of the scilab function to
 * be created, and the necessary object file.
 *-----------------------------------------------------------------*/
SipExport int
hello_int(char *fname)
{
  SciErr sciErr;

  // Interface Variables
  int   ra, ca, pa,
        rb, cb, pb,
        rsum, csum, psum,
        minlhs=1, maxlhs=1, minrhs=2, maxrhs=2;  // specify # of args
  double sum;

  // check if # of args is correct
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1, "d", &ra, &ca, &pa);  // parameter a
  GetRhsVar(2, "d", &rb, &cb, &pb);  // parameter b

  // access the variables in the scilab stack
  // and pass them to "hello"
  sum = animal_hello(*stk(pa), *stk(pb));

  // return the result back to Scilab
  rsum=csum=1;
  CreateVar(3, "d", &rsum, &csum, &psum);
  *stk(psum) = sum;

  LhsVar(1) = 3;
  return true;
}
