method = 'hsv_sip';
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
