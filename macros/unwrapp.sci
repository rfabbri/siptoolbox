function [unwrapped_phase,phase_jumps]=unwrapp(wrapped_phase,merit_function,line_start,col_start,threshold);

// Description:
// Unwraps a phased image (gray levels) by path following:
// 1) the quality of each pixel is estimated: the result
// is stored in a "merit function" (mf).
// Some merit functions can be: laplacien, variance...
// A good quality pixel has a low merit function.
// 2) We begin from a point of good quality. It is called "integrator point".
// 3) we rely it to its best quality neighbour, which becomes the new
// "integrator point".
// 4) If the difference between phases of the 2 points is higher (or lower)
// than a threshold, then we consider there's a phase jump.
// 5) we continue until all points are treated.
//
// (pw means "phase wrapped" and puw means "phase unwrapped")
//
// NB: we store the path in a matrix ("path"). If we go in a baggin
// (all neighbours have been treated), then we restart from the beginning until
// we find a pixel with an unvisited neighbour.
//
// ORIGINAL AUTHOR
//    Jocelyn DRUEL  <jocelyn.druel1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// The C coding of the function was done with the great help of Jean Denaveaut
// (Lycee Gustave Eiffel). Many Thanks.
//
// REFERENCES
// David VENET:
// "techniques automatiques de raccordement de phase"
// Memoire de l'universite de Lausanne:
//
// LICENSE: GNU GPL
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $
//
// Changelog:
// 1.5: C coding of unwrapp which improves speed.
// A threshold parameter can be passed.
//
// 1.4: Function adapted to the new SIP convention of representing all 
// images in 0-1 range.
//
// 1.3: points with a merit function (mf) >= vval (default: vval=3000)
// will not be demodulated (they are considered as too bad to be demodulated.
// The "shift" variable was changed to "shift1" to avoid conflicts
// with the homonym Scilab function
//
// TODO: improve the interface function to pass line_start and col_start
// as integers instead of doubles.
//



if argn(2)==0 then
  error("[unwrapped_phase,phase_jumps]=unwrapp(wrapped_phase,merit_function[,line_start,col_start,threshold]");
end;

if ~exists('line_start','local') then
  line_start=round(size(wrapped_phase,"r")/2);
end
if ~exists('col_start','local') then
  col_start=round(size(wrapped_phase,"c")/2);
end
if ~exists('threshold','local') then
  threshold=0.5;
end

phase_jumps=sip_unwrapp_c(wrapped_phase,merit_function, line_start, col_start, threshold');
unwrapped_phase=wrapped_phase+phase_jumps;

endfunction;
