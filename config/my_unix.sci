function stat=my_unix(cmd)

tmp=TMPDIR+'/unix.out';
cmd1='('+cmd+')>'+ tmp +' 2>'+TMPDIR+'/unix.err;';

stat=host(cmd1)
select stat
case -1 // host failed
   error('host does not answer...') 
case 0  // success
   msg=read(tmp,-1,1,'(a)');
   for s=msg
      write(%io(2),s)
   end
else // error
   msg=read(TMPDIR+'/unix.err',-1,1,'(a)');
   for s=msg
      write(%io(2),s)
   end
   error('')
end

host('rm -rf ' + tmp);
      
endfunction 
