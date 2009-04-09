mode(-1);
//lines(0);

macropath=get_absolute_file_path('buildmacros.sce');
disp('Building macros  in ' + macropath);

scilab_version=getversion();
if (str2code( part(scilab_version, 8)) <= 4) then //version <= 4.x.x
   genlib('siplib', macropath,%t);
else
   tbx_build_macros("sip", macropath);
end

exit;
