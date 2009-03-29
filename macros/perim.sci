function p = perim(img)
// 
// Simple routine to measure the perimeter of a binary shape.
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
// $Revision: 1.2 $ $Date: 2009-03-29 21:34:48 $

[x,y] = follow(img);
v = x + y*%i;
n=size(x,'*')
p = abs(v($) - v(1));
for i=1:n-1
   p = p + abs(v(i+1)-v(i))
end

endfunction 
