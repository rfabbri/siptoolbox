// -----------------------------------------------------------------------------
// use a RANSAC fitting to align and possibly determine a finer spacing
// corresponding to the maximum possible number of inliers

exec ransac_offset.sce;

// -----------------------------------------------------------------------------
// Plot the lengths along the normals to the central curve.
// Perhaps even plot the prototype


n_zc_model = floor(maxlen/rt2);
zc_len_pos = (0:n_zc_model)*rt2 + zc_len(best_offset);
zc_len_pos = modulo(zc_len_pos, maxlen);

// now get points at arclengths zc_len_pos. 

zc_pos = zeros(zc_len_pos);

for i=1:length(zc_pos)
  [m,m_i] = min(abs(len - zc_len_pos(i)));
  zc_pos(i) = m_i;
end

zimg = unfollow(x(zc_pos), y(zc_pos), size(e));

myclf
imshow(5*zimg+e,[]);
