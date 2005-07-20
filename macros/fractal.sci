//
// $Revision: 1.1 $ $Date: 2005-07-20 01:33:35 $
//
// TODO
// - tornar as rotinas k e q como subrotinas
// - manter imagem "padded" durante dilatacoes
// - usar dilatacao incremental pra cada raio, e NAO 
//   a dilatacao "do zero" como aqui
// - fazer a rotina em C
// - rodar profiling
//



// Fractal Dimension Curve by Minkowski Sausages
//
// PARAMETERS
// -     im:  binary image
// - rstart: radius where the dilation starts
// -   rend: radius where the dilation stops
// -   pace: the increment of each dilation step
//
// NOTA
//   o usuario pode entrar os parametros 2 a 4 em qquer ordem
//
function [log_radius, log_area] = fractal(im,rstart,rend,pace)
   nlc=lines();
   lines(0);
   select argn(2)
   case 1
      rstart=1
      rend=30
      pace=1
   case 2
      rend=30
      pace=1
   case 3
      pace=1
   end

   log_area   = zeros((rend-rstart)/pace,1);
   log_radius = log_area;
   i=1;
   for rad=rstart:pace:rend do
      disp(rad);
      img=edilate(im,rad,'full');
      log_area(i) = sum(img);
      i=i+1
   end
   log_radius = log(rstart:pace:rend); 
   log_area= log(log_area);

   lines(nlc(2));
endfunction

//
// Calcula curva de koch
//
// PARAMETERS
//      - vetor inicial:
//
//                  l = ||vetor||
//             .-------------------->
//            p     
//   
//      - n = numero de niveis de recursao
//
//
function v = k(p,l,n)
   if n == 0 then
      v = [p;  p+l]
   else
      a=l/3
      v1 = k(p,a,n-1)
      v2 = k(p+a, a*rot60, n-1)
      v3 = k(p+a+a*rot60, a*rot300, n-1)
      v4 = k(p+2*a, a, n-1)
      v = [v1;v2;v3;v4]
   end
endfunction


//
// calcula curva quadrada
//
// PARAMETERS
//    - the same as the "k" routine
//
function v = q(p,l,n)
   if n == 0 then
      v = [p;  p+l]
   else
      a=l/3
      v1 = q(p,a,n-1)
      v2 = q(p+a, a*rot90, n-1)
      v3 = q(p+a+a*rot90, a, n-1)
      v4 = q(p+a+a*rot90+a, a*rot270, n-1)
      v5 = q(p+2*a, a, n-1)
      v = [v1;v2;v3;v4;v5]
   end
endfunction

//
// Desenha a curva de Koch
//
//
function koch(p,l,n)
   v=k(p,l,n)
   plot2d(v(:,1),v(:,2),2,strf='040')
endfunction

//
// Desenha a curva quadrada
//
function quad(p,l,n)
   v=q(p,l,n)
   plot2d(v(:,1),v(:,2),2,strf='040')
endfunction

//
// Desenha a o floco de neve de Koch
//
//  - n = grau de recursividade
//
function koch_snowflake(n)
   v1 = k([0 0],[0.5 0.8660254],n)
   v2 = k([0.5 0.8660254],[0.5 -0.8660254],n)
   v3 = k([1 0],[-1 0],n)
   v = [v1;v2;v3]
   plot2d(v(:,1),v(:,2),2,strf='040')
endfunction
