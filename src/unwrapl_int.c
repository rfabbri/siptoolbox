/*
 * Changes from Jocelyn's original
 *    - function unwrapl_c didn't free malloc'd arrays.
 *    - SIP code style (Kernighan & Ritchie with 3-space indentation)
 *
 * -------------------------------------------------------------------------
 * SIP - Scilab Image Processing toolbox
 * Copyright (C) 2002-2004  Ricardo Fabbri
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

/*--------------------------------------------------------
 * unwrapl_c_int: interface for unwrapl_c C-function 
 *     should provide jumps=unwrapl_c(wrapped_phase,threshold,step,direction)
 *     at Scilab level 
 *--------------------------------------------------------*/

static bool unwrapl_c(double a[], double *threshold, int *step,
                      char *direction, double c[], int n, int m);

SipExport int 
unwrapl_c_int(char *fname)
{
   int r1, c1, p1,                /* a */
    r_thresh, c_thresh, l_thresh, /* threshold */
    r_step, c_step, l_step,       /* step */
    r_direction, c_direction, l_direction,   /* direction */
    p2,    /* c */
    minlhs = 1, maxlhs = 1, minrhs = 4, maxrhs = 4;
   bool stat;


   /* Check number of inputs (rhs) and outputs (lhs) */
   CheckRhs(minrhs, maxrhs);
   CheckLhs(minlhs, maxlhs);

   /* Input parameters: */
   /* GetRhsVar(number, type, matrix_size1, matrix_size2, value); */
   GetRhsVar(1, "d", &r1, &c1, &p1);
   GetRhsVar(2, "d", &r_thresh, &c_thresh, &l_thresh);
   GetRhsVar(3, "i", &r_step, &c_step, &l_step);
   GetRhsVar(4, "c", &r_direction, &c_direction, &l_direction);

   /* Ouput creation: */
   /* CreateVar(number, type, matrix_size1, matrix_size2, &l3); */

   CreateVar(5, "d", &r1, &c1, &p2);

   /* Call function */
   stat = unwrapl_c(stk(p1), stk(l_thresh), istk(l_step), cstk(l_direction),
                    stk(p2), r1, c1);

   if (!stat) sip_error("unable to alloc memory");

   /*  The return variable is number (5)  */
   LhsVar(1) = 5;
   return 0;
}

/*--------------------------------------------------------
 *  Linear phase unwrapping process 
 *  C function
 *--------------------------------------------------------*/


#define A(i,j) a[i + j*m]
#define C(i,j) c[i + j*m]


/* m= number of rows, n=columns*/
/* i and j are row and column */
bool
unwrapl_c(double a[], double *threshold, int *step, char *direction,
          double c[], int m, int n)
{
   int i, j, k;
   unsigned long s = 0;

   /* linear matrix */
   double *lm, *myshift;
   int jumps, distance;

   /*  s=0;*/
   /*  threshold=0.5;*/
   jumps = distance = 0;

   /* memory allocation */
   /* Essential !! */
   lm = (double *) calloc(n * m, sizeof(double));
   if (!lm)
      return false;
   myshift = (double *) calloc(n * m, sizeof(double));
   if (!myshift)
      return false;


   /*create a linear matrix (vertical direction) */
   if (strcmp(direction, "v") == 0)
      for (j = 0; j < n; j = j + 2) {
         for (i = 0; i < m; i++) {
            lm[s] = A(i, j);
   /*	printf("lm = %f\n",A(i,j));*/
            s++;
         }
         for (i = 0; i < m; i++)
            if ((j + 1) < n) {
               lm[s] = A((m - i - 1), (j + 1));
   /*	  printf("lm = %f\n",A((m-i-1),(j+1))); */
               s++;
            }
      }
   /*create a linear matrix (horizontal direction) */
   else
      for (i = 0; i < m; i = i + 2) {
         for (j = 0; j < n; j++) {
            lm[s] = A(i, j);
            s++;
         }
         for (j = 0; j < n; j++)
            if ((i + 1) < m) {
               lm[s] = A((i + 1), (n - j - 1));
               s++;
            }
      }

   /*threshold condition: if the difference between 2 consecutive
   pixels is greater than the threshold, then there's a phase jump*/
   /*distance condition: to avoid noise, 2 jumps must be separated from
   at least a certain amount of pixels*/
   myshift[0] = 0;
   for (k = 1; k < (m * n); k++) {
      if (((lm[k] - lm[k - 1]) > (*threshold)) && (distance >= (*step))) {
         jumps--;
         distance = 0;
      }
      if (((lm[k] - lm[k - 1]) < (-(*threshold)))
          && (distance >= (*step))) {
         jumps++;
         distance = 0;
      }
      myshift[k] = jumps;
      distance++;
   }


   /* Recreate 2D matrix: vertical and horizontal cases */
   if (strcmp(direction, "v") == 0) {
      s = 0;
      for (j = 0; j < n; j = j + 2) {
         for (i = 0; i < m; i++) {
            C(i, j) = myshift[s];
            s++;
         }
         for (i = 0; i < m; i++) {
            if ((j + 1) < n) {
               C((m - i - 1), (j + 1)) = myshift[s];
               s++;
            }
         }
      }
   } else {
      s = 0;
      for (i = 0; i < m; i = i + 2) {
         for (j = 0; j < n; j++) {
            C(i, j) = myshift[s];
            s++;
         }
         for (j = 0; j < n; j++) {
            if ((i + 1) < m) {
               C((i + 1), (n - j - 1)) = myshift[s];
               s++;
            }
         }
      }
   }

   free(lm);
   free(myshift);

   return true;
}
