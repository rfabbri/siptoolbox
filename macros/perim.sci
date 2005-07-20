function p = perim(img)
// 
// Simple routine to measure the perimeter of a binary shape.
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@if.sc.usp.br>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://cyvision.if.sc.usp.br
//
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $

[x,y] = follow(img);
v = x + y*%i;
n=size(x,'*')
p = abs(v($) - v(1));
for i=1:n-1
   p = p + abs(v(i+1)-v(i))
end

endfunction 
