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
#include <string.h>
#include "sip_common.h"

/*
 * This code was contributed by Jocelyn Druel and
 * was slightly improved by R. Fabbri
 */

static void 
imvariance(double a[], double c[],int m, int n);

SipExport int 
imvariance_int(char *fname) 
{
  static int l1, m1, n1, l2;
  static int minlhs=1, maxlhs=1, minrhs=1, maxrhs=1;

  /* Check number of inputs and outputs */
  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  /*Input parameters: */
  GetRhsVar(1, "d", &m1, &n1, &l1); 

  CreateVar(2, "d", &m1, &n1, &l2);

  imvariance(stk(l1), stk(l2), m1, n1);

  LhsVar(1) = 2;
  return 0;
}

#define A(i,j) a[i + j*m]
#define C(i,j) c[i + j*m]

/* m= number of rows, n=columns*/
/* i and j are row and column (Scilab style, ie 1-max)*/
void 
imvariance(double a[], double c[],int m, int n)
{
  int i,j;
  double s;
  for( i=0 ; i < m; i++){

   C(i,0)=fabs(A(i,1)-A(i,0));
    C(i,(n-1))=fabs(A(i,(n-2))-A(i,(n-1)));
  }
    for( j=0; j < n; j++){
      C(0,j)=0;
      C((m-1),j)=0;
    }


  for( i=1 ; i < (m-1); i++){
    for( j=1; j < (n-1); j++){
      s=0;
      s+=fabs(A((i-1),(j-1))-A(i,j));
      s+=fabs(A((i),(j-1))-A(i,j));
      s+=fabs(A((i+1),(j-1))-A(i,j));
      s+=fabs(A((i-1),(j))-A(i,j));
      s+=fabs(A((i),(j))-A(i,j));
      s+=fabs(A((i+1),(j))-A(i,j));
      s+=fabs(A((i-1),(j+1))-A(i,j));
      s+=fabs(A((i),(j+1))-A(i,j));
      s+=fabs(A((i+1),(j+1))-A(i,j));

      C(i,j)=s;
    }
  }
}
