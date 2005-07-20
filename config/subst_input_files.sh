# Script called by libsimple_build.sce
# DONT RUN IT DIRECTLY UNLESS YOU KNOW WHAT YOU ARE DOING!!!

topbuildir='..'
ilib_name=$1
shift

case `basename $PWD` in
src)
	sed "s/%LIBNAME%/$ilib_name.la/g
	     s/%SRC_FILES%/$*/g
		  /^#%.*$/d
	     s/#%.*//g" Makefile.am.input >Makefile.am;;

macros)
	macros_sci=`(cd $topbuildir/macros; ls *.sci)`
	macros_sci=`echo $macros_sci` # put in jus 1 line
	macros_bin=`echo $macros_sci | sed 's/\.sci/\.bin/g'`
	
	sed "s/%LIBNAME%/$ilib_name.la/g
		  s/%MACROS_SCI%/$macros_sci/g
		  s/%MACROS_BIN%/$macros_bin/g
		  /^#%.*$/d
	     s/#%.*//g" Makefile.am.input >Makefile.am;;

images)
	images=`(cd $topbuildir/images; ls *.jpeg *.jpg *.png *.bmp *.tif* *.jpg *.gif *.xpm) 2>/dev/null`
	images=`echo $images`
	sed "s@%IMAGES%@$images@g
		  /^#%.*$/d
	     s/#%.*//g" Makefile.am.input >Makefile.am;;

photonics)
	images=`(cd $topbuildir/images/photonics; ls *.jpeg *.jpg *.png *.bmp *.tif* *.jpg *.gif *.xpm) 2>/dev/null`
	images=`echo $images`
	sed "s@%PHOTONIC_IMAGES%@$images@g
		  /^#%.*$/d
	     s/#%.*//g" Makefile.am.input >Makefile.am;;

man)
	manpages=`(cd $topbuildir/man; ls *.xml)`
   manpages_xml=`echo $manpages`
	manpages_cat=`echo $manpages | sed 's/\.xml/\.cat/g'`
	manpages_man=`echo $manpages | sed 's/\.xml/\.man/g'`
	manpages_html=`echo $manpages | sed 's/\.xml/\.htm/g'`
	sed "s/%LIBNAME%/$ilib_name.la/g
		  s/%MANPAGES_XML%/$manpages_xml/g
		  s/%MANPAGES_CAT%/$manpages_cat/g
		  s/%MANPAGES_MAN%/$manpages_man/g
		  s/%MANPAGES_HTML%/$manpages_html/g
		  /^#%.*$/d
	     s/#%.*//g" Makefile.am.input >Makefile.am;;
esac
