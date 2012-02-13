// use a RANSAC fitting to align and possibly determine a finer spacing
// corresponding to the maximum possible number of inliers

// from rn above we know a range of tangential radii rt that must be the truth, 
// between 0.5*rn and 1.5*rn for sure


// search up to half the embryo for good starting points for the tangential grid

// inliers are integrated within a +-rt_tol*(tangential nucleus radius) range
rt_tol = 0.3*median_rt;

maxlen = zc_len($);
max_zc_offset = 0.5*maxlen;
rt2 = 2*median_rt;

max_consensus = 0;
best_offset = 0;
ia = 1;
a = zc_len(ia);
goodness_of_fit = 0;

while a < max_zc_offset
  consensus = 0;
  goodness_of_fit = 0;
  b = a + rt2;
  b_ini = b;
  while b + rt_tol < maxlen
    inliers = find(zc_len >= b - rt_tol    &   zc_len <= b + rt_tol);
    ni = length(inliers);
    if ni > 0
      dfit = min(abs(b - zc_len(inliers)))^2;
      consensus = consensus + ni;
    else
      dfit = (rt_tol*2)^2+1;
    end
      
    goodness_of_fit = goodness_of_fit + dfit;
    b = b + rt2;
  end

  b = b_ini - rt2;
  while b - rt_tol > 0
    inliers = find(zc_len >= b - rt_tol    &   zc_len <= b + rt_tol);
    ni = length(inliers);
    if ni > 0
      dfit = min(abs(b - zc_len(inliers)))^2;
      consensus = consensus + ni;
    else
      dfit = (rt_tol*2)^2+1;
    end
      
    goodness_of_fit = goodness_of_fit + dfit;
    b = b - rt2;
  end

  if max_consensus <= consensus
    if max_consensus == consensus
      if goodness_of_fit < best_goodness_of_fit
        max_consensus = consensus;
        best_offset = ia;
        best_goodness_of_fit = goodness_of_fit;
      end
    else
      max_consensus = consensus
      best_offset = ia;
      best_goodness_of_fit = goodness_of_fit;
    end
  end

  ia = ia + 1;
  a = zc_len(ia);
end

max_consensus
best_offset
best_goodness_of_fit = sqrt(best_goodness_of_fit/maxlen)
