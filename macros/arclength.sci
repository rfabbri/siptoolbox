function  s = arclength(crv, circular)
// 
// Computes the arclength parameter of a curve
//
// 
// Input: 
//   - nx2 curve vector [x y]
// Output: 
//   - arc length at each point, with s[1] = 0;
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $

n = size(crv,1);
s = zeros(n,1);

idx = 2:n;
dx = crv(idx,1) - crv(idx-1,1);
dy = crv(idx,2) - crv(idx-1,2);
s(idx) = sqrt(dx.*dx + dy.*dy);

for i=idx
  s(i) = s(i-1) + s(i);
end


[%nargout,%nargin] = argn(0)

if %nargin == 2
  s(n+1)  = s(n) + norm([crv(n,:) - crv(1,:)]);
end

endfunction 
