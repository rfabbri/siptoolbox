//
// salt & pepper (drop-out noise): 
//    d: density, from 0 to 1
//  val: value of salt (default=maximum of image)
//       If "val" equals to 0, then pixels are replaced by random
//       gray values
//
// gaussian
//    m: mean (from 0 to 1)
//    v: variance (from 0 to 1)
//    val: max value of noise
//
// imnoise(img, 'localvar',V);
//    Additive gaussian noise where the variance at Img(i,j) is
//    V(i,j).
//    
//
// speckle (multiplicative noise)
//    Image = Image + prob*Image
//    where "prob" is a gaussian(0-1) distributed noise
//    v: variance
//

function im = imnoise(im, noise_type, param1, param2, param3)

if noise_type == 'gaussian' | noise_type == 'Gaussian'
   if ~exists('param1','local')
      m=0
   else
      m=param1
   end
   if ~exists('param2','local')
      v=0.01
   else
      v=param2
   end
   if ~exists('param3','local')
      val=maxi(im);
   else
      val=param3
   end

   v  = v*val;
   m  = m*val;
   im = im + sqrt(v)*rand(im,'normal') + m;
elseif noise_type == 'localvar'
   if argn(2) < 3
      error('Too few arguments.');
   end
   im = im + sqrt(maxi(im)*param1).*rand(im,'normal');
elseif noise_type == 'salt & pepper' | noise_type == 'salt and pepper'
   if ~exists('param1','local')
      d=0.05 
   else
      d=param1
   end
   if ~exists('param2','local')
      val=maxi(im);
   else
      val=param2
   end

   // logic:
   //
   // If prob <= d, then modify pix value like this:
   //    if prob <= d/2
   //       put salt
   //    else
   //       put pepper.
   //
   prob=rand(im);
   if val > 0  
      // normal salt-and-pepper
      salt_and_pepper = val*(prob <= d/2);
   else        
      // random gray drop-out noise
      val = maxi(im);
      salt_and_pepper = val*rand(im);
   end

   im(prob <= d) = salt_and_pepper(prob <= d);

elseif noise_type=='speckle'
   if ~exists('param1','local')
      v=0.04
   else
      v=param1
   end
   if ~exists('param2','local')
      val=maxi(im);
   else
      val=param2
   end
   im = im + im .* (sqrt(v)*rand(im,'normal'));   
elseif noise_type == 'poisson'
   error('Not yet implemented');
else
   error('Invalid noise type.');
end

endfunction
