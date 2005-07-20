function [unwrapped_phase,phase_jumps]=unwrapl(wrapped_phase,threshold,step,direction);
//
// Description:
// Unwraps a phased image (gray levels) of the simpliest manner: linearly
// 1) computes a linear matrix: 1st line is read from left to right,
// 2nd line is read from right to left, etc...
// 2) compares the gradient to a threshold (0.5 is the default):
// add or substract 1 to ensure phase continuity.
// 3) rebuilds a image matrix from the linear matrix
// 4) step: don't authorize 2 phase jumps which are too close from one another
// 5) direction: h (horizontal: default) or v (vertical)
// 6) phase_jumps is the map containing the number of phase jumps
// 
// Very simple and fast algorithm. But very noise sensitive.
// Images to treat should be of excellent quality.
// 
// Changelog:
// This function now calls the C-coded function unwrapl_c.
// Speed was greatly improved by this mean.
// To preserve homogeneity with the unwrapp function, unwrapl now returns also
// the phase_jumps map (and not only the unwrapped_phase).
//
// AUTHOR
//    Jocelyn DRUEL  <jocelyn.drual1@libertysurf.fr>
//    Labo Photonique, Lycee Gustave Eiffel
//    59280 ARMENTIERES (FRANCE)
//
// LICENSE: GNU GPL
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $

if argn(2)==0 then
  error("[unwrapped_phase,phase_jumps]=unwrapl(wrapped_phase[,threshold,step,direction])")
end;


if ~exists('threshold','local') then
  threshold=0.5
end
if ~exists('step','local') then
  step=0;
end
if ~exists('direction','local') then
  direction='h'
end

phase_jumps=sip_unwrapl_c(wrapped_phase,threshold,step,direction');
unwrapped_phase=wrapped_phase+phase_jumps;

endfunction;
