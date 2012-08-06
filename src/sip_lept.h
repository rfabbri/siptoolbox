#include <stdio.h>
#include <allheaders.h>
#include <math.h>
#include <stdlib.h>
#include <sip_common.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
extern PIX * pixSipDeskew ( PIX *pixs, l_int32 redsearch );
extern PIX * pixSipFindSkewAndDeskew ( PIX *pixs, l_int32 redsearch, l_float32 *pangle, l_float32 *pconf );
extern PIX * pixSipDeskewGeneral ( PIX *pixs, l_int32 redsweep, l_float32 sweeprange, l_float32 sweepdelta, l_int32 redsearch, l_int32 thresh, l_float32 *pangle, l_float32 *pconf );
extern l_int32 pixSipFindSkew ( PIX *pixs, l_float32 *pangle, l_float32 *pconf );
extern l_int32 pixSipFindSkewSweep ( PIX *pixs, l_float32 *pangle, l_int32 reduction, l_float32 sweeprange, l_float32 sweepdelta );
extern l_int32 pixSipFindSkewSweepAndSearch ( PIX *pixs, l_float32 *pangle, l_float32 *pconf, l_int32 redsweep, l_int32 redsearch, l_float32 sweeprange, l_float32 sweepdelta, l_float32 minbsdelta );
extern l_int32 pixSipFindSkewSweepAndSearchScore ( PIX *pixs, l_float32 *pangle, l_float32 *pconf, l_float32 *pendscore, l_int32 redsweep, l_int32 redsearch, l_float32 sweepcenter, l_float32 sweeprange, l_float32 sweepdelta, l_float32 minbsdelta );
extern l_int32 pixSipFindSkewSweepAndSearchScorePivot ( PIX *pixs, l_float32 *pangle, l_float32 *pconf, l_float32 *pendscore, l_int32 redsweep, l_int32 redsearch, l_float32 sweepcenter, l_float32 sweeprange, l_float32 sweepdelta, l_float32 minbsdelta, l_int32 pivot );
extern l_int32 pixSipFindSkewOrthogonalRange ( PIX *pixs, l_float32 *pangle, l_float32 *pconf, l_int32 redsweep, l_int32 redsearch, l_float32 sweeprange, l_float32 sweepdelta, l_float32 minbsdelta, l_float32 confprior );
extern l_int32 pixSipFindDifferentialSquareSum ( PIX *pixs, l_float32 *psum );
extern l_int32 pixSipFindNormalizedSquareSum ( PIX *pixs, l_float32 *phratio, l_float32 *pvratio, l_float32 *pfract );

#ifdef __cplusplus
}
#endif  /* __cplusplus */
