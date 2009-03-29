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


/*--------------------------------------------------------
 * unwrapp_c_int: interface for unwrapp_c C-function 
 *     should provide jumps=unwrapp_c(wrapped_phase,merit_function, line_start,col_start, threshold)
 *     at Scilab level 
 * Many thanks to Jean Denaveaut (Lycee Gustave Eiffel)
 * for the original C translation of this function 
 *
 * TODO: line_start and col_start should be passed as integers 
 * (and not doubles)
 *--------------------------------------------------------*/


static void 
   unwrapp_c(double a[], double b[], double *line_start,
             double *col_start, double *threshold, double c[], int m, int n);


SipExport int 
unwrapp_c_int(char *fname)
{
   int r1, c1, p1,              /* a: unwrapped_phase */
    r_merit, c_merit, l_merit,  /* b: merit_function */
    r_line, c_line, l_line,     /* line_start */
    r_col, c_col, l_col,        /* col_start */
    r_thres, c_thres, l_thres,  /* threshold */
    r2, c2, p2,                 /* c: jumps */
    minlhs = 1, maxlhs = 1, minrhs = 5, maxrhs = 5;


   /* Check number of inputs (rhs) and outputs (lhs) */
   CheckRhs(minrhs, maxrhs);
   CheckLhs(minlhs, maxlhs);

   /*Input parameters: */
   /*GetRhsVar(number, type, matrix_size1, matrix_size2, value); */
   GetRhsVar(1, "d", &r1, &c1, &p1);
   GetRhsVar(2, "d", &r_merit, &c_merit, &l_merit);
   GetRhsVar(3, "d", &r_line, &c_line, &l_line);
   GetRhsVar(4, "d", &r_col, &c_col, &l_col);
   GetRhsVar(5, "d", &r_thres, &c_thres, &l_thres);

   /* Ouput creation: */
   /* CreateVar(number, type, matrix_size1, matrix_size2, &l3); */

   CreateVar(6, "d", &r1, &c1, &p2);

   /* Call function */
   unwrapp_c(stk(p1), stk(l_merit), stk(l_line), stk(l_col), stk(l_thres),
             stk(p2), r1, c1);


   /*  The return variable is number (6)  */
   LhsVar(1) = 6;
   return true;
}

/*--------------------------------------------------------
 *  Path following phase unwrapping process 
 *  C function
 *--------------------------------------------------------*/


#define A(i,j) a[i + j*m]
#define B(i,j) b[i + j*m]
#define C(i,j) c[i + j*m]


/* m= number of rows, n=columns*/
/* i and j are row and column */

#define VALVISITE  3000         /* Maximal value in the merit_function matrix:
                                   a visited point has this value (3000) */


void 
unwrapp_c(double a[], double b[], double *line_start,
          double *col_start, double *threshold, double c[], int m, int n)

{
   typedef struct position {
      short l;
      short c;
   } position;
   static position TabPath[442370]; /*memorize path followed */
   double phase, WinPhase;      /* idem */
   int val, min,                /* variables used to search the best integrator point */
       decalPhi,                /* phase offset */
       deltaX, deltaY,
       line, col,
       i, j, k, somme,
       radius = 15;             /* at thebeginning, we search the best point in this radius */
   unsigned nbPoints_toSee,
      nbPointsVisited, /* Number of points already visited */
      num_pt_depart,
      AllSeen = 9 * VALVISITE;
   float thres_val = *threshold; /* if the phase difference is greater than 
                                    this threshold, we consider that there's 
                                    a phase jump. */
   line = *line_start;
   col  = *col_start;


   /* Borders are not treated, so we mark them as visited */
   /* 1st and last rows */
   for (i = 0; i < m; i++) {
      B(i, 0) = VALVISITE;
      B(i, (n - 1)) = VALVISITE;
      C(i, 0) = 0;
      C(i, (n - 1)) = 0;
   }
   /* 1st and columns */
   for (j = 0; j < n; j++) {
      B(0, j) = VALVISITE;
      B((m - 1), j) = VALVISITE;
      C(0, j) = 0;
      C((m - 1), j) = 0;
   }


   /* The starting point */
   /* Search for an optimal starting point */
   val = B(line, col);          /* Merit for the central point */
   deltaX = 0;                  /* to find a position near the actual point */
   deltaY = 0;
   /* Exploration of the zone +- radius in rows and columns in order
      to find a minimum merit value */
   for (j = -radius; j <= radius; j++)
      for (k = -radius; k <= radius; k++) {
         if (B((line + j), (col + k)) < val) {
            val = B((line + j), (col + k));
            deltaX = k;
            deltaY = j;
         }
      }
   col  = col + deltaX;
   line = line + deltaY;
   deltaX = 0;
   deltaY = 0;

   /* Misc. initialisations */
   B(line, col) = VALVISITE;    /* the starting is visited */
   nbPointsVisited = 1;
   C(line, col) = 0;            /* no jump at the position of the starting point */
   TabPath[0].l = (short) line; /* path memorisation */
   TabPath[0].c = (short) col;  /* idem */
   num_pt_depart = 0;           /* idem */
   decalPhi = 0;


   /* How many points do we have to visit ? */
   nbPoints_toSee = 0;
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++) {
         C(i, j) = 0;
         if (B(i, j) < VALVISITE)
            nbPoints_toSee++;
      }


   /* Are there still unvisited points ? */
   while (nbPointsVisited < nbPoints_toSee) {
      /* if all the neighbours have been visited, then we're in a baggins:
         we have to go back till we find a point with unvisited neighbours */
      while (B((line - 1), (col - 1)) + B(line, (col - 1)) +
             B((line + 1), (col - 1)) + B((line - 1), col) + B(line, col) +
             B((line + 1), col) + B((line - 1), (col + 1)) + B(line,
                                                               (col + 1)) +
             B((line + 1), (col + 1)) == AllSeen) {
         num_pt_depart++;
         line = TabPath[num_pt_depart].l;
         col = TabPath[num_pt_depart].c;
      }
      phase = A(line, col);
      decalPhi = C(line, col);

      /* Searching for the new integrator point */
      min = VALVISITE;
      deltaX = 0;
      deltaY = 0;
      for (j = -1; j < 2; j++)
         for (k = -1; k < 2; k++)
            if (B((line + j), (col + k)) < min) {
               min = B((line + j), (col + k));
               deltaX = k;
               deltaY = j;
            }
      line = line + deltaY;
      col = col + deltaX;

      if (line < 1)
         line = 1;
      if (line > (m - 1))
         line = m - 1;
      if (col > (n - 1))
         col = n - 1;
      if (col < 1)
         col = 1;
      WinPhase = A(line, col);
      if ((phase - WinPhase) > thres_val)
         decalPhi++;
      if ((WinPhase - phase) > thres_val)
         decalPhi--;
      C(line, col) = decalPhi;
      B(line, col) = VALVISITE; /* the pixel is marked as seen */
      nbPointsVisited++;
      TabPath[nbPointsVisited].l = (short) line;
      TabPath[nbPointsVisited].c = (short) col;
   }
}
