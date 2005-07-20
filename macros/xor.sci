// XOR convenience logical function
//
function C=xor(A,B)
  C =  (A & ~B) | (~A & B)
endfunction
