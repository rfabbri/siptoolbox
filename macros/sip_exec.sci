function sip_exec(myfiletoexec)
// This is just a misc. scilab utility; this functionality shall appear in
// Scilab 6.
//
// Like exec but picks the file from PATH so you don't have to specify the full
// path every time.
//
// AUTHOR
// Allan CORNET http://bugzilla.scilab.org/show_bug.cgi?id=10094

ps = getenv('PATH');
ps = strsplit(ps,':')

for i = 1:size(ps,'*')
  p = fullpath(ps(1), myfiletoexec)
  if isfile(p) then
    exec(p);
    return
  end
end

error('file not found in PATH');
endfunction
