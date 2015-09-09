// Make fig a shortcut to scf
fig=scf;

// Make doc a shortcut to help

doc=help;

function hold_on()
  // old: xset('auto clear', 'on'); 
  set(gca(),"auto_clear","off")
endfunction

function hold_off()
  // old: xset('auto clear', 'off'); 
  set(gca(),"auto_clear","on")
endfunction

function axis_equal()
  a = gca()
  a.isoview = 'on'
endfunction

// assert=assert_checktrue;
