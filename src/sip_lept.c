#include <math.h>
#include "allheaders.h"
#include "sip_lept.h"
#include "sip_common.h"

static const l_float32  DEFAULT_SWEEP_RANGE = 10.;    /* degrees */
static const l_float32  DEFAULT_SWEEP_DELTA = 1.;    /* degrees */
static const l_float32  DEFAULT_MINBS_DELTA = 0.01;  /* degrees */
static const l_int32  DEFAULT_SWEEP_REDUCTION = 4;  /* sweep part; 4 is good */
static const l_int32  DEFAULT_BS_REDUCTION = 2;  /* binary search part */
static const l_float32  MIN_DESKEW_ANGLE = 0.1;  /* degree */
static const l_float32  MIN_ALLOWED_CONFIDENCE = 3.0;
static const l_int32  MIN_VALID_MAXSCORE = 10000;
static const l_float32  MINSCORE_THRESHOLD_CONSTANT = 0.000002;

    /* Default binarization threshold value */
static const l_int32  DEFAULT_BINARY_THRESHOLD = 130;

#ifndef  NO_CONSOLE_IO
#define  DEBUG_PRINT_SCORES     0
#define  DEBUG_PRINT_SWEEP      0
#define  DEBUG_PRINT_BINARY     0
#define  DEBUG_PRINT_ORTH       0
#define  DEBUG_THRESHOLD        0
#define  DEBUG_PLOT_SCORES      0
#endif  /* ~NO_CONSOLE_IO */

/*****************************************************
 *                pixSipDeskew()
 *****************************************************/

PIX *
pixSipDeskew(PIX     *pixs,
          l_int32  redsearch)
{
    PROCNAME("pixDeskew");

    if (!pixs)
        return (PIX *)ERROR_PTR("pixs not defined", procName, NULL);
    if (redsearch == 0)
        redsearch = DEFAULT_BS_REDUCTION;
    else if (redsearch != 1 && redsearch != 2 && redsearch != 4)
        return (PIX *)ERROR_PTR("redsearch not in {1,2,4}", procName, NULL);

    return pixDeskewGeneral(pixs, 0, 0.0, 0.0, redsearch, 0, NULL, NULL);
}

/*****************************************************
 *                pixSipFindSkewAndDeskew()
 *****************************************************/

PIX *
pixSipFindSkewAndDeskew (PIX        *pixs,
                     l_int32     redsearch,
                     l_float32  *pangle,
                     l_float32  *pconf)
{
    PROCNAME("pixFindSkewAndDeskew");

    if (!pixs)
        return (PIX *)ERROR_PTR("pixs not defined", procName, NULL);
    if (redsearch == 0)
        redsearch = DEFAULT_BS_REDUCTION;
    else if (redsearch != 1 && redsearch != 2 && redsearch != 4)
        return (PIX *)ERROR_PTR("redsearch not in {1,2,4}", procName, NULL);

    return pixSipDeskewGeneral (pixs, 0, 0.0, 0.0, redsearch, 0, pangle, pconf);
}

/*****************************************************
 *                pixSipDeskewGeneral()
 *****************************************************/

PIX *
pixSipDeskewGeneral (PIX        *pixs,
                 l_int32     redsweep,
                 l_float32   sweeprange,
                 l_float32   sweepdelta,
                 l_int32     redsearch,
                 l_int32     thresh,
                 l_float32  *pangle,
                 l_float32  *pconf)
{
l_int32    ret, depth;
l_float32  angle, conf, deg2rad;
PIX       *pixb, *pixd;

    PROCNAME("pixDeskewGeneral");

    if (!pixs)
        return (PIX *)ERROR_PTR("pixs not defined", procName, NULL);
    if (redsweep == 0)
        redsweep = DEFAULT_SWEEP_REDUCTION;
    else if (redsweep != 1 && redsweep != 2 && redsweep != 4)
        return (PIX *)ERROR_PTR("redsweep not in {1,2,4}", procName, NULL);
    if (sweeprange == 0.0)
        sweeprange = DEFAULT_SWEEP_RANGE;
    if (sweepdelta == 0.0)
        sweepdelta = DEFAULT_SWEEP_DELTA;
    if (redsearch == 0)
        redsearch = DEFAULT_BS_REDUCTION;
    else if (redsearch != 1 && redsearch != 2 && redsearch != 4)
        return (PIX *)ERROR_PTR("redsearch not in {1,2,4}", procName, NULL);
    if (thresh == 0)
        thresh = DEFAULT_BINARY_THRESHOLD;

    deg2rad = 3.1415926535 / 180.;

        /* Binarize if necessary */
    depth = pixGetDepth(pixs);
    if (depth == 1)
        pixb = pixClone(pixs);
    else
        pixb = pixConvertTo1(pixs, thresh);

        /* Use the 1 bpp image to find the skew */
    ret = pixSipFindSkewSweepAndSearch (pixb, &angle, &conf, redsweep, redsearch,
                                    sweeprange, sweepdelta,
                                    DEFAULT_MINBS_DELTA);
    pixDestroy(&pixb);
    if (pangle)
        *pangle = angle;
    if (pconf)
        *pconf = conf;
    if (ret)
        return pixClone(pixs);

    if (L_ABS(angle) < MIN_DESKEW_ANGLE || conf < MIN_ALLOWED_CONFIDENCE)
        return pixClone(pixs);

    if ((pixd = pixRotate(pixs, deg2rad * angle, L_ROTATE_AREA_MAP,
                          L_BRING_IN_WHITE, 0, 0)) == NULL)
        return pixClone(pixs);
    else
        return pixd;
}

/*****************************************************
 *                pixSipFindSkew()
 *****************************************************/

l_int32
pixSipFindSkew (PIX        *pixs,
            l_float32  *pangle,
            l_float32  *pconf)
{
    PROCNAME("pixFindSkew");

    if (!pixs)
        return sciprint(" %s :pixs not defined\r \n",procName);
    if (pixGetDepth(pixs) != 1)
        return sciprint(" %s :pixs not 1 bpp\r \n",procName);
    if (!pangle)
        return sciprint(" %s :&angle not defined\r \n",procName);
    if (!pconf)
        return sciprint(" %s :&conf not defined\r \n",procName);

    return pixSipFindSkewSweepAndSearch (pixs, pangle, pconf,
                                     DEFAULT_SWEEP_REDUCTION,
                                     DEFAULT_BS_REDUCTION,
                                     DEFAULT_SWEEP_RANGE,
                                     DEFAULT_SWEEP_DELTA,
                                     DEFAULT_MINBS_DELTA);
}

/*****************************************************
 *                pixFindSkewSweep()
 *****************************************************/

l_int32
pixSipFindSkewSweep (PIX        *pixs,
                 l_float32  *pangle,
                 l_int32     reduction,
                 l_float32   sweeprange,
                 l_float32   sweepdelta)
{
l_int32    ret, bzero, i, nangles;
l_float32  deg2rad, theta;
l_float32  sum, maxscore, maxangle;
NUMA      *natheta, *nascore;
PIX       *pix, *pixt;

    PROCNAME("pixFindSkewSweep");

    if (!pixs)
        return sciprint(" %s :pixs not defined\r \n",procName);
    if (pixGetDepth(pixs) != 1)
        return sciprint(" %s :pixs not 1 bpp\r \n",procName);
    if (!pangle)
        return sciprint(" %s :&angle not defined\r \n",procName);
    if (reduction != 1 && reduction != 2 && reduction != 4 && reduction != 8)
        return sciprint(" %s :reduction must be in {1,2,4,8}\r \n",procName);

    *pangle = 0.0;  /* init */
    deg2rad = 3.1415926535 / 180.;
    ret = 0;

        /* Generate reduced image, if requested */
    if (reduction == 1)
        pix = pixClone(pixs);
    else if (reduction == 2)
        pix = pixReduceRankBinaryCascade(pixs, 1, 0, 0, 0);
    else if (reduction == 4)
        pix = pixReduceRankBinaryCascade(pixs, 1, 1, 0, 0);
    else /* reduction == 8 */
        pix = pixReduceRankBinaryCascade(pixs, 1, 1, 2, 0);

    pixZero(pix, &bzero);
    if (bzero) {
        pixDestroy(&pix);
        return 1;
    }

    nangles = (l_int32)((2. * sweeprange) / sweepdelta + 1);
    natheta = numaCreate(nangles);
    nascore = numaCreate(nangles);
    pixt = pixCreateTemplate(pix);

    if (!pix || !pixt) {
        ret = sciprint(" %s :pix and pixt not both made\r \n",procName);
        goto cleanup;
    }
    if (!natheta || !nascore) {
        ret = sciprint(" %s :natheta and nascore not both made\r \n",procName);
        goto cleanup;
    }

    for (i = 0; i < nangles; i++) {
        theta = -sweeprange + i * sweepdelta;   /* degrees */

            /* Shear pix about the UL corner and put the result in pixt */
        pixVShearCorner(pixt, pix, deg2rad * theta, L_BRING_IN_WHITE);

            /* Get score */
        pixSipFindDifferentialSquareSum (pixt, &sum);

#if  DEBUG_PRINT_SCORES
        sciprint(" %s :sum(%7.2f) = %7.0f \r \n", procName, theta, sum);
#endif  /* DEBUG_PRINT_SCORES */

            /* Save the result in the output arrays */
        numaAddNumber(nascore, sum);
        numaAddNumber(natheta, theta);
    }

        /* Find the location of the maximum (i.e., the skew angle)
         * by fitting the largest data point and its two neighbors
         * to a quadratic, using lagrangian interpolation.  */
    numaFitMax(nascore, &maxscore, natheta, &maxangle);
    *pangle = maxangle;

#if  DEBUG_PRINT_SWEEP
    sciprint(" %s : From sweep: angle = %7.3f, score = %7.3f \r \n", procName,
                  maxangle, maxscore);
#endif  /* DEBUG_PRINT_SWEEP */

#if  DEBUG_PLOT_SCORES
        /* Plot the result -- the scores versus rotation angle --
         * using gnuplot with GPLOT_LINES (lines connecting data points).
         * The GPLOT data structure is first created, with the
         * appropriate data incorporated from the two input NUMAs,
         * and then the function gplotMakeOutput() uses gnuplot to
         * generate the output plot.  This can be either a .png file
         * or a .ps file, depending on whether you use GPLOT_PNG
         * or GPLOT_PS.  */
    {GPLOT  *gplot;
        gplot = gplotCreate("sweep_output", GPLOT_PNG,
                    "Sweep. Variance of difference of ON pixels vs. angle",
                    "angle (deg)", "score");
        gplotAddPlot(gplot, natheta, nascore, GPLOT_LINES, "plot1");
        gplotAddPlot(gplot, natheta, nascore, GPLOT_POINTS, "plot2");
        gplotMakeOutput(gplot);
        gplotDestroy(&gplot);
    }
#endif  /* DEBUG_PLOT_SCORES */

cleanup:
    pixDestroy(&pix);
    pixDestroy(&pixt);
    numaDestroy(&nascore);
    numaDestroy(&natheta);
    return 0;
}

/*****************************************************
 *                pixSipFindSkewSweepAndSearch()
 *****************************************************/

l_int32
pixSipFindSkewSweepAndSearch (PIX        *pixs,
                          l_float32  *pangle,
                          l_float32  *pconf,
                          l_int32     redsweep,
                          l_int32     redsearch,
                          l_float32   sweeprange,
                          l_float32   sweepdelta,
                          l_float32   minbsdelta)
{
    return pixSipFindSkewSweepAndSearchScore (pixs, pangle, pconf, NULL,
                                          redsweep, redsearch, 0.0, sweeprange, 
                                          sweepdelta, minbsdelta);
}

/*****************************************************
 *           pixSipFindSkewSweepAndSearchScore()
 *****************************************************/

l_int32
pixSipFindSkewSweepAndSearchScore (PIX        *pixs,
                               l_float32  *pangle,
                               l_float32  *pconf,
                               l_float32  *pendscore,
                               l_int32     redsweep,
                               l_int32     redsearch,
                               l_float32   sweepcenter,
                               l_float32   sweeprange,
                               l_float32   sweepdelta,
                               l_float32   minbsdelta)
{
    return pixSipFindSkewSweepAndSearchScorePivot (pixs, pangle, pconf, pendscore,
                                               redsweep, redsearch, 0.0,
                                               sweeprange, sweepdelta,
                                               minbsdelta,
                                               L_SHEAR_ABOUT_CORNER);
}

/*****************************************************
 *       pixSipFindSkewSweepAndSearchScorePivot()
 *****************************************************/

l_int32
pixSipFindSkewSweepAndSearchScorePivot (PIX        *pixs,
                                    l_float32  *pangle,
                                    l_float32  *pconf,
                                    l_float32  *pendscore,
                                    l_int32     redsweep,
                                    l_int32     redsearch,
                                    l_float32   sweepcenter,
                                    l_float32   sweeprange,
                                    l_float32   sweepdelta,
                                    l_float32   minbsdelta,
                                    l_int32     pivot)
{
l_int32    ret, bzero, i, nangles, n, ratio, maxindex, minloc;
l_int32    width, height;
l_float32  deg2rad, theta, delta;
l_float32  sum, maxscore, maxangle;
l_float32  centerangle, leftcenterangle, rightcenterangle;
l_float32  lefttemp, righttemp;
l_float32  bsearchscore[5];
l_float32  minscore, minthresh;
l_float32  rangeleft;
NUMA      *natheta, *nascore;
PIX       *pixsw, *pixsch, *pixt1, *pixt2;

    PROCNAME("pixFindSkewSweepAndSearchScorePivot");

    if (!pixs)
        return sciprint(" %s :pixs not defined\r \n",procName);
    if (pixGetDepth(pixs) != 1)
        return sciprint(" %s :pixs not 1 bpp\r \n",procName);
    if (!pangle)
        return sciprint(" %s :&angle not defined\r \n",procName);
    if (!pconf)
        return sciprint(" %s :&conf not defined\r \n",procName);
    if (redsweep != 1 && redsweep != 2 && redsweep != 4 && redsweep != 8)
        return sciprint(" %s :redsweep must be in {1,2,4,8}\r \n",procName);
    if (redsearch != 1 && redsearch != 2 && redsearch != 4 && redsearch != 8)
        return sciprint(" %s :redsearch must be in {1,2,4,8}\r \n",procName);
    if (redsearch > redsweep)
        return sciprint(" %s :redsearch must not exceed redsweep\r \n",procName);
    if (pivot != L_SHEAR_ABOUT_CORNER && pivot != L_SHEAR_ABOUT_CENTER)
        return sciprint(" %s :invalid pivot\r \n",procName);

    *pangle = 0.0;
    *pconf = 0.0;
    deg2rad = 3.1415926535 / 180.;
    ret = 0;

        /* Generate reduced image for binary search, if requested */
    if (redsearch == 1)
        pixsch = pixClone(pixs);
    else if (redsearch == 2)
        pixsch = pixReduceRankBinaryCascade(pixs, 1, 0, 0, 0);
    else if (redsearch == 4)
        pixsch = pixReduceRankBinaryCascade(pixs, 1, 1, 0, 0);
    else  /* redsearch == 8 */
        pixsch = pixReduceRankBinaryCascade(pixs, 1, 1, 2, 0);

    pixZero(pixsch, &bzero);
    if (bzero) {
        pixDestroy(&pixsch);
        return 1;
    }

        /* Generate reduced image for sweep, if requested */
    ratio = redsweep / redsearch;
    if (ratio == 1)
        pixsw = pixClone(pixsch);
    else {  /* ratio > 1 */
        if (ratio == 2)
            pixsw = pixReduceRankBinaryCascade(pixsch, 1, 0, 0, 0);
        else if (ratio == 4)
            pixsw = pixReduceRankBinaryCascade(pixsch, 1, 2, 0, 0);
        else  /* ratio == 8 */
            pixsw = pixReduceRankBinaryCascade(pixsch, 1, 2, 2, 0);
    }

    pixt1 = pixCreateTemplate(pixsw);
    if (ratio == 1)
        pixt2 = pixClone(pixt1);
    else
        pixt2 = pixCreateTemplate(pixsch);

    nangles = (l_int32)((2. * sweeprange) / sweepdelta + 1);
    natheta = numaCreate(nangles);
    nascore = numaCreate(nangles);

    if (!pixsch || !pixsw) {
        ret = sciprint(" %s :pixsch and pixsw not both made\r \n",procName);
        goto cleanup;
    }
    if (!pixt1 || !pixt2) {
        ret = sciprint(" %s :pixt1 and pixt2 not both made\r \n",procName);
        goto cleanup;
    }
    if (!natheta || !nascore) {
        ret = sciprint(" %s :natheta and nascore not both made\r \n",procName);
        goto cleanup;
    }

        /* Do sweep */
    rangeleft = sweepcenter - sweeprange;
    for (i = 0; i < nangles; i++) {
        theta = rangeleft + i * sweepdelta;   /* degrees */

            /* Shear pix and put the result in pixt1 */
        if (pivot == L_SHEAR_ABOUT_CORNER)
            pixVShearCorner(pixt1, pixsw, deg2rad * theta, L_BRING_IN_WHITE);
        else
            pixVShearCenter(pixt1, pixsw, deg2rad * theta, L_BRING_IN_WHITE);

            /* Get score */
        pixSipFindDifferentialSquareSum (pixt1, &sum);

#if  DEBUG_PRINT_SCORES
        sciprint(" %s :sum(%7.2f) = %7.0f \r \n", procName, theta, sum);
#endif  /* DEBUG_PRINT_SCORES */

            /* Save the result in the output arrays */
        numaAddNumber(nascore, sum);
        numaAddNumber(natheta, theta);
    }

        /* Find the largest of the set (maxscore at maxangle) */
    numaGetMax(nascore, &maxscore, &maxindex);
    numaGetFValue(natheta, maxindex, &maxangle);

#if  DEBUG_PRINT_SWEEP
    sciprint(" %s : From sweep: angle = %7.3f, score = %7.3f \r \n", procName,
                  maxangle, maxscore);
#endif  /* DEBUG_PRINT_SWEEP */

#if  DEBUG_PLOT_SCORES
        /* Plot the sweep result -- the scores versus rotation angle --
         * using gnuplot with GPLOT_LINES (lines connecting data points). */
    {GPLOT  *gplot;
        gplot = gplotCreate("sweep_output", GPLOT_PNG,
                    "Sweep. Variance of difference of ON pixels vs. angle",
                    "angle (deg)", "score");
        gplotAddPlot(gplot, natheta, nascore, GPLOT_LINES, "plot1");
        gplotAddPlot(gplot, natheta, nascore, GPLOT_POINTS, "plot2");
        gplotMakeOutput(gplot);
        gplotDestroy(&gplot);
    }
#endif  /* DEBUG_PLOT_SCORES */

        /* Check if the max is at the end of the sweep. */
    n = numaGetCount(natheta);
    if (maxindex == 0 || maxindex == n - 1) {
        sciprint("  Warning %s :max found at sweep edge\r \n", procName);
        return 0 ;
        goto cleanup;
    }

        /* Empty the numas for re-use */
    numaEmpty(nascore);
    numaEmpty(natheta);

        /* Do binary search to find skew angle.
         * First, set up initial three points. */
    centerangle = maxangle;
    if (pivot == L_SHEAR_ABOUT_CORNER) {
        pixVShearCorner(pixt2, pixsch, deg2rad * centerangle, L_BRING_IN_WHITE);
        pixSipFindDifferentialSquareSum (pixt2, &bsearchscore[2]);
        pixVShearCorner(pixt2, pixsch, deg2rad * (centerangle - sweepdelta),
                        L_BRING_IN_WHITE);
        pixSipFindDifferentialSquareSum (pixt2, &bsearchscore[0]);
        pixVShearCorner(pixt2, pixsch, deg2rad * (centerangle + sweepdelta),
                        L_BRING_IN_WHITE);
        pixSipFindDifferentialSquareSum (pixt2, &bsearchscore[4]);
    }
    else {
        pixVShearCenter(pixt2, pixsch, deg2rad * centerangle, L_BRING_IN_WHITE);
        pixSipFindDifferentialSquareSum (pixt2, &bsearchscore[2]);
        pixVShearCenter(pixt2, pixsch, deg2rad * (centerangle - sweepdelta),
                        L_BRING_IN_WHITE);
        pixSipFindDifferentialSquareSum (pixt2, &bsearchscore[0]);
        pixVShearCenter(pixt2, pixsch, deg2rad * (centerangle + sweepdelta),
                        L_BRING_IN_WHITE);
        pixSipFindDifferentialSquareSum (pixt2, &bsearchscore[4]);
    }

    numaAddNumber(nascore, bsearchscore[2]);
    numaAddNumber(natheta, centerangle);
    numaAddNumber(nascore, bsearchscore[0]);
    numaAddNumber(natheta, centerangle - sweepdelta);
    numaAddNumber(nascore, bsearchscore[4]);
    numaAddNumber(natheta, centerangle + sweepdelta);

        /* Start the search */
    delta = 0.5 * sweepdelta;
    while (delta >= minbsdelta)
    {
            /* Get the left intermediate score */
        leftcenterangle = centerangle - delta;
        if (pivot == L_SHEAR_ABOUT_CORNER)
            pixVShearCorner(pixt2, pixsch, deg2rad * leftcenterangle,
                            L_BRING_IN_WHITE);
        else
            pixVShearCenter(pixt2, pixsch, deg2rad * leftcenterangle,
                            L_BRING_IN_WHITE);
        pixSipFindDifferentialSquareSum (pixt2, &bsearchscore[1]);
        numaAddNumber(nascore, bsearchscore[1]);
        numaAddNumber(natheta, leftcenterangle);
        
            /* Get the right intermediate score */
        rightcenterangle = centerangle + delta;
        if (pivot == L_SHEAR_ABOUT_CORNER)
            pixVShearCorner(pixt2, pixsch, deg2rad * rightcenterangle,
                            L_BRING_IN_WHITE);
        else
            pixVShearCenter(pixt2, pixsch, deg2rad * rightcenterangle,
                            L_BRING_IN_WHITE);
        pixSipFindDifferentialSquareSum(pixt2, &bsearchscore[3]);
        numaAddNumber(nascore, bsearchscore[3]);
        numaAddNumber(natheta, rightcenterangle);
        
            /* Find the maximum of the five scores and its location.
             * Note that the maximum must be in the center
             * three values, not in the end two. */
        maxscore = bsearchscore[1];
        maxindex = 1;
        for (i = 2; i < 4; i++) {
            if (bsearchscore[i] > maxscore) {
                maxscore = bsearchscore[i];
                maxindex = i;
            }
        }

            /* Set up score array to interpolate for the next iteration */
        lefttemp = bsearchscore[maxindex - 1];
        righttemp = bsearchscore[maxindex + 1];
        bsearchscore[2] = maxscore;
        bsearchscore[0] = lefttemp;
        bsearchscore[4] = righttemp;

            /* Get new center angle and delta for next iteration */
        centerangle = centerangle + delta * (maxindex - 2);
        delta = 0.5 * delta;
    }
    *pangle = centerangle;

#if  DEBUG_PRINT_SCORES
    L_INFO_FLOAT(" Binary search score = %7.3f", procName, bsearchscore[2]);
#endif  /* DEBUG_PRINT_SCORES */

    if (pendscore)  /* save if requested */
        *pendscore = bsearchscore[2];

        /* Return the ratio of Max score over Min score
         * as a confidence value.  Don't trust if the Min score
         * is too small, which can happen if the image is all black
         * with only a few white pixels interspersed.  In that case,
         * we get a contribution from the top and bottom edges when
         * vertically sheared, but this contribution becomes zero when
         * the shear angle is zero.  For zero shear angle, the only
         * contribution will be from the white pixels.  We expect that
         * the signal goes as the product of the (height * width^2),
         * so we compute a (hopefully) normalized minimum threshold as
         * a function of these dimensions.  */
    numaGetMin(nascore, &minscore, &minloc);
    width = pixGetWidth(pixsch);
    height = pixGetHeight(pixsch);
    minthresh = MINSCORE_THRESHOLD_CONSTANT * width * width * height;

#if  DEBUG_THRESHOLD
    sciprint(" %s : minthresh = %10.2f, minscore = %10.2f \r \n", procName,
            minthresh, minscore);
    L_INFO_FLOAT(" maxscore = %10.2f", procName, maxscore);
#endif  /* DEBUG_THRESHOLD */

    if (minscore > minthresh)
        *pconf = maxscore / minscore;
    else
        *pconf = 0.0;

        /* Don't trust it if too close to the edge of the sweep
         * range or if maxscore is small */
    if ((centerangle > rangeleft + 2 * sweeprange - sweepdelta) ||
        (centerangle < rangeleft + sweepdelta) ||
        (maxscore < MIN_VALID_MAXSCORE))
        *pconf = 0.0;

#if  DEBUG_PRINT_BINARY
    fprintf(stderr, "Binary search: angle = %7.3f, score ratio = %6.2f\n",
            *pangle, *pconf);
    fprintf(stderr, "               max score = %8.0f\n", maxscore);
#endif  /* DEBUG_PRINT_BINARY */

#if  DEBUG_PLOT_SCORES
        /* Plot the result -- the scores versus rotation angle --
         * using gnuplot with GPLOT_POINTS.  Because the data
         * points are not ordered by theta (increasing or decreasing),
         * using GPLOT_LINES would be confusing! */
    {GPLOT  *gplot;
        gplot = gplotCreate("search_output", GPLOT_PNG,
                "Binary search.  Variance of difference of ON pixels vs. angle",
                "angle (deg)", "score");
        gplotAddPlot(gplot, natheta, nascore, GPLOT_POINTS, "plot1");
        gplotMakeOutput(gplot);
        gplotDestroy(&gplot);
    }
#endif  /* DEBUG_PLOT_SCORES */

cleanup:
    pixDestroy(&pixsw);
    pixDestroy(&pixsch);
    pixDestroy(&pixt1);
    pixDestroy(&pixt2);
    numaDestroy(&nascore);
    numaDestroy(&natheta);
    return ret;
}

/*****************************************************
 *       pixSipFindSkewOrthogonalRange
 *****************************************************/

l_int32
pixSipFindSkewOrthogonalRange(PIX        *pixs,
                           l_float32  *pangle,
                           l_float32  *pconf,
                           l_int32     redsweep,
                           l_int32     redsearch,
                           l_float32   sweeprange,
                           l_float32   sweepdelta,
                           l_float32   minbsdelta,
                           l_float32   confprior)
{
l_float32  angle1, conf1, score1, angle2, conf2, score2;
PIX       *pixr;

    PROCNAME("pixFindSkewOrthogonalRange");

    if (!pangle || !pconf)
        return sciprint(" %s :&angle and &conf not both defined\r \n",procName);
    *pangle = *pconf = 0.0;
    if (!pixs || pixGetDepth(pixs) != 1)
        return sciprint(" %s :pixs not defined or not 1 bpp\r \n",procName);

    pixSipFindSkewSweepAndSearchScorePivot(pixs, &angle1, &conf1, &score1,
                                        redsweep, redsearch, 0.0,
                                        sweeprange, sweepdelta, minbsdelta,
                                        L_SHEAR_ABOUT_CORNER);
    pixr = pixRotateOrth(pixs, 1);
    pixSipFindSkewSweepAndSearchScorePivot(pixr, &angle2, &conf2, &score2,
                                        redsweep, redsearch, 0.0,
                                        sweeprange, sweepdelta, minbsdelta,
                                        L_SHEAR_ABOUT_CORNER);
    pixDestroy(&pixr);

    if (conf1 > conf2 - confprior) {
        *pangle = angle1;
        *pconf = conf1;
    } else {
        *pangle = -90.0 + angle2;
        *pconf = conf2;
    }

#if  DEBUG_PRINT_ORTH
    fprintf(stderr, " About 0:  angle1 = %7.3f, conf1 = %7.3f, score1 = %f\n",
            angle1, conf1, score1);
    fprintf(stderr, " About 90: angle2 = %7.3f, conf2 = %7.3f, score2 = %f\n",
            angle2, conf2, score2);
    fprintf(stderr, " Final:    angle = %7.3f, conf = %7.3f\n", *pangle, *pconf);
#endif  /* DEBUG_PRINT_ORTH */

    return 0;
}

/*****************************************************
 *       pixSipFindDifferentialSquareSum()
 *****************************************************/

l_int32
pixSipFindDifferentialSquareSum(PIX        *pixs,
                             l_float32  *psum)
{
l_int32    i, n;
l_int32    w, h, skiph, skip, nskip;
l_float32  val1, val2, diff, sum;
NUMA      *na;

    PROCNAME("pixFindDifferentialSquareSum");

    if (!pixs)
        return sciprint(" %s :pixs not defined\r \n",procName);

        /* Generate a number array consisting of the sum
         * of pixels in each row of pixs */
    if ((na = pixCountPixelsByRow(pixs, NULL)) == NULL)
        return sciprint(" %s :na not made\r \n",procName);

        /* Compute the number of rows at top and bottom to omit.
         * We omit these to avoid getting a spurious signal from
         * the top and bottom of a (nearly) all black image. */
    w = pixGetWidth(pixs);
    h = pixGetHeight(pixs);
    skiph = (l_int32)(0.05 * w);  /* skip for max shear of 0.025 radians */
    skip = L_MIN(h / 10, skiph);  /* don't remove more than 10% of image */
    nskip = L_MAX(skip / 2, 1);  /* at top & bot; skip at least one line */

        /* Sum the squares of differential row sums, on the
         * allowed rows.  Note that nskip must be >= 1. */
    n = numaGetCount(na);
    sum = 0.0;
    for (i = nskip; i < n - nskip; i++) {
        numaGetFValue(na, i - 1, &val1);
        numaGetFValue(na, i, &val2);
        diff = val2 - val1;
        sum += diff * diff;
    }
    numaDestroy(&na);
    *psum = sum;
    return 0;
}

/*****************************************************
 *       pixSipFindNormalizedSquareSum()
 *****************************************************/

l_int32
pixSipFindNormalizedSquareSum(PIX        *pixs,
                           l_float32  *phratio,
                           l_float32  *pvratio,
                           l_float32  *pfract)
{
l_int32    i, w, h, empty;
l_float32  sum, sumsq, uniform, val;
NUMA      *na;
PIX       *pixt;

    PROCNAME("pixFindNormalizedSquareSum");

    if (!pixs || pixGetDepth(pixs) != 1)
        return sciprint(" %s :pixs not defined or not 1 bpp\r \n",procName);
    pixGetDimensions(pixs, &w, &h, NULL);

    if (!phratio && !pvratio)
        return sciprint(" %s :nothing to do\r \n",procName);
    if (phratio) *phratio = 0.0;
    if (pvratio) *pvratio = 0.0;

    empty = 0;
    if (phratio) {
        na = pixCountPixelsByRow(pixs, NULL);
        numaGetSum(na, &sum);  /* fg pixels */
        if (pfract) *pfract = sum / (l_float32)(w * h);
        if (sum != 0.0) {
            uniform = sum * sum / h;   /*  h*(sum / h)^2  */
            sumsq = 0.0;
            for (i = 0; i < h; i++) {
                numaGetFValue(na, i, &val);
                sumsq += val * val;
            }
            *phratio = sumsq / uniform;
        }
        else
            empty = 1;
        numaDestroy(&na);
    }

    if (pvratio) {
        if (empty == 1) return 1;
        pixt = pixRotateOrth(pixs, 1);
        na = pixCountPixelsByRow(pixt, NULL);
        numaGetSum(na, &sum);
        if (pfract) *pfract = sum / (l_float32)(w * h);
        if (sum != 0.0) {
            uniform = sum * sum / w;
            sumsq = 0.0;
            for (i = 0; i < w; i++) {
                numaGetFValue(na, i, &val);
                sumsq += val * val;
            }
            *pvratio = sumsq / uniform;
        }
        else
            empty = 1;
        pixDestroy(&pixt);
        numaDestroy(&na);
    }

    return empty;
}
