help_lang_dir = get_absolute_file_path('build_help.sce');
// the following doesn't let you specify a language, thus you endup with
// inconsistencies when forcing en_US on other locales, see SIP bug #3393248
// tbx_build_help("SIP - Scilab Image Processing Toolbox", help_lang_dir, );
xmltojar(help_lang_dir,"SIP - Scilab Image Processing Toolbox", ...
         "en_US", "en_US");
clear help_lang_dir;
