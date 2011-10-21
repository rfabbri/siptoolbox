method = 'hsv_sip';


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
//BAD!white   green   blue                 uncertain   white-halogen-8   47   !
//!                                                                        !
//BAD!white   green   gray                 good guess  white-natural-1   67   !
//!                                                                        !
//OK GOOD EXAMPLE !green   yellow  gray                 unreliable  green-natural-09  68   !
//!                                                                        !
//!OK yellow  green   gray                 good guess  yellow-natural-5  81   !
//!                                                                        !
//SOSO !white   green   gray                 good guess  white-halogen-0   103  !
//!                                                                        !
//OK !green   black                        certain     green-halogen-16  107  !
//!                                                                        !
//OK!blue    green   blue                 uncertain   blue-natural-6    110  !
//!                                                                        !
//OK!green   blue    black                good guess  green-halogen-11  112  !


//!SHINEY green   yellow  gray                 unreliable  green-natural-09  68   !
//!                                                                        !
//BAD !green   white   gray                 good guess  green-halogen-12  77   !
//!                                                                        !
//BAD !green   white   gray                 good guess  green-halogen-13  101  !
//!                                                                        !
//!white   green   gray                 unreliable  white-halogen-0   103  !
