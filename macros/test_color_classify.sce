method = 'hsv_sip';
run_image=%f;
mydir='/tmp/'
//method='distance_to_reference'


// Fundamentals =======================================

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0 0 1], method);
label == 'blue'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0 1 0], method);
label == 'green'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([1 0 0], method);
label == 'red'
certainty == 'certain'


[label, certainty, confidence, secondary_label] = ..
  color_classify_single([1 1 0], method);
label == 'yellow'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0 0 0], method);
label == 'black'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([1 1 1], method);
label == 'white'
certainty == 'certain'

// Perturbs =======================================

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0.2 0.2 0.9], method);
label == 'blue'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0.2 0.9 0.2], method);
label == 'green'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0.9 0.2 0.2], method);
label == 'red'
certainty == 'certain'


[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0.9 0.9 0.2], method);
label == 'yellow'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0.2 0.15 0.2], method);
label == 'black' | secondary_label == 'black'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0.85 0.9 0.9], method);
label == 'white'
certainty == 'certain'

// More ambiguous =======================================

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([254 121 164]/255, method);
label == 'red'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([169 36 74]/255, method);
label == 'red'
certainty == 'certain'


[label, certainty, confidence, secondary_label] = ..
  color_classify_single([254 121 164]/255, method);
label == 'red'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([1 85 66]/255, method);
label == 'green'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([65 153 122]/255, method);
label == 'green'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([59 136 213]/255, method);
label == 'blue'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0 42 102]/255, method);
label == 'blue'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([254 221 172]/255, method);
label == 'yellow'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([254 209 168]/255, method);
label == 'yellow'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0 0 2]/255, method);
label == 'black'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([0 1 0]/255, method);
label == 'black'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([171 115 119]/255, method);
label == 'red'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([187 184 177]/255, method);
label == 'white'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([90 156 100]/255, method);
label == 'green'
certainty == 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([169 160 161]/255, method);
(label == 'gray' | label == 'white') &  (secondary_label == 'gray' | secondary_label == 'white')

certainty ~= 'certain'

[label, certainty, confidence, secondary_label] = ..
  color_classify_single([121 180 2]/255, method);
label == 'green'
certainty == 'certain'

// best so far: 100% precision at coverage of 81 cases base hard

// Taxa de acerto: 0.7852349 (117/149)   
 
// Taxa de acerto com certeza: 0.9753086 (79/81) coverage: 0.5436242 (81/149)
// (os 2 errados aqui sao OK certain abaixo, logo contam como certos)

//certainty == 'certain'

//OK !blue    black                        certain     blue-halogen-32   20   !
//!                                                                        !
//SHiNY!green   red     purple-pink-magenta  good guess  green-natural-06  34   !
//!                                                                        !
//OK GOOD EXAMPLE !green   yellow  gray                 unreliable  green-natural-09  68   !
//!                                                                        !
//!OK yellow  green   gray                 good guess  yellow-natural-5  81   !
//!                                                                        !
//OK !green   black                        certain     green-halogen-16  107  !
//!                                                                        !
//OK!blue    green   blue                 uncertain   blue-natural-6    110  !
//!                                                                        !
//OK!green   blue    black                good guess  green-halogen-11  112  !



//!blue    green   gray                 unreliable  blue-halogen-27    11   !
//!                                                                         !
//!blue    green   gray                 unreliable  blue-halogen-2     14   !
//!                                                                         !
//HARD!red     green   gray                 unreliable  red-halogen-10     23   !
//!                                                                         !
//!blue    black   gray                 good guess  blue-halogen-25    30   !
//!                                                                         !
//!black   blue    gray                 good guess  black-halogen-4    35   !
//!                                                                         !
//!yellow  green   gray                 unreliable  yellow-halogen-7   46   !
//!                                                                         !
//!yellow  green   gray                 unreliable  yellow-halogen-3   64   !
//!                                                                         !
//!yellow  green   gray                 unreliable  yellow-halogen-11  76   !
//!                                                                         !
//!yellow  green   gray                 unreliable  yellow-natural-2   80   !
//!                                                                         !
//HARD!red     blue    gray                 good guess  red-natural-0      83   !
//!                                                                         !
//HARD!red     blue    gray                 good guess  red-natural-6      90   !
//!                                                                         !
//HARD!red     green   gray                 unreliable  red-halogen-6      98   !
//!                                                                         !
//HARD!blue    green   gray                 unreliable  blue-halogen-6     100  !
//!                                                                         !
//!blue    green                        good guess  blue-natural-18    110  !
//!                                                                         !
//!HARD red     green   gray                 unreliable  red-halogen-14     114  !
//!                                                                         !
//!HARD red     blue    gray                 good guess  red-natural-4      115  !
//!                                                                         !
//!HARD red     yellow  gray                 unreliable  red-halogen-2      116  !
//!                                                                         !
//!HARD red     green   blue                 uncertain   red-natural-8      117  !
//!                                                                         !
//VERY HARD!red     green   gray                 unreliable  red-halogen-17     124  !
//!                                                                         !
//HARD!blue    green   blue                 uncertain   blue-halogen-29    126  !


if ~run_image
  return;
end


imgu=imgo(1:16:1536,1:16:2048,:);
[img, cmap, uncertainty_level] = color_segmentation(imgu);

imwrite(img,cmap, mydir + 'result.png');
imwrite(normal(uncertainty_level), mydir + 'uncertainty.png');

img(uncertainty_level > 0) = 4;
imwrite(img,cmap, mydir + 'only-certain.png');
