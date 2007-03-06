# Process this file with autoconf to produce a configure script.
#
# Copyright (C) 2003 Ricardo Fabbri
# Some inspiration came from ImageMagick's configure.ac (thx Bob F.)
# and examples from the Autobook (http://sources.redhat.com/autobook)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

AC_PREREQ(2.59)

#AC_INIT([sip], [20040710-2], [ricardofabbri@users.sf.net],[sip-dev])
AC_INIT([sip], [0.4.0-rc4-cvs], [ricardofabbri@users.sf.net],[sip])
echo "configuring SIP AC_PACKAGE_VERSION"
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/imread.c])
AC_CONFIG_HEADER([src/config.h])
AX_PREFIX_CONFIG_H([src/sip_config.h],[sip])
AM_INIT_AUTOMAKE([no-define])

# Checks for programs.
AC_PROG_CC

if test x"$ac_cv_prog_cc_stdc" = x'no'; then
   AC_MSG_WARN([[found the $CC compiler but it is not ANSI-C compliant.]])
fi



AC_LIBTOOL_WIN32_DLL
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Check to see if building shared libraries
libtool_build_shared_libs='no'
if ./libtool --config | grep 'build_libtool_libs=yes' > /dev/null
then
  libtool_build_shared_libs='yes'
fi


#
# Tests for windoze
#
native_win32_build='no'
cygwin_build='no'
case $host_os in
  cygwin*) 
      cygwin_build='yes'
      ;;
  mingw* )
      native_win32_build='yes' 
      ;;
esac


# checks for xsltproc (to buld HTML help pages)
AC_CHECK_PROG([has_xsltproc],[xsltproc],[yes],[no], , )
if test x$has_xsltproc = xno; then
   # checks for sabcmd as an alternative
   AC_CHECK_PROG([has_sablotron],[sabcmd],[yes],[no], , )
   if test x$has_sablotron = xno; then
      # Only warn in Windows.
      # On unix I will halt.
      if test "${native_win32_build}" = 'yes'; then
         AC_MSG_WARN([[manpages are not going to be built: xsltproc or
sabcmd programs were not found and at least one of them is required to 
build the SIP manual pages.
Download xslproc (from libxml) at:
   http://http://www.xmlsoft.org 
or sabcmd (from sablotron) at:
   http://www.gingerall.com/charlie/ga/xml/p_sab.xml]])
      else
      AC_MSG_ERROR([[xsltproc or sabcmd programs were not found but \
at least one of them is required to build the SIP manual pages.
Download xslproc (from libxml) at:
   http://http://www.xmlsoft.org 
or sabcmd (from sablotron) at:
   http://www.gingerall.com/charlie/ga/xml/p_sab.xml]])
      fi
   fi
fi

# test if SCI is defined. If not,
# test if scilab is in path, then get SCI from scilab -nw.
if test -z "$SCI"; then
   if test $native_win32_build = yes; then
      dnl TODO take SCIDIR from the registry ??
      AC_MSG_WARN([[hardcoding SCI to /c/scilab]])
      SCIDIR="/c/scilab"
   else
      AC_CHECK_PROG([has_scilab],[scilab],[yes],[no], , )
      if test x$has_scilab = xno; then
         AC_MSG_ERROR([[neither Scilab was found in your PATH nor a SCI variable has been defined]])
      else
         cmd='F=mopen("path.incl","w");
              mfprintf(F,SCI);
              mclose(F);'
         echo "$cmd" | scilab -nw >/dev/null
         SCIDIR=`cat path.incl`
      fi
   fi
else
   SCIDIR="$SCI"
fi
CPPFLAGS="$CPPFLAGS -I${SCIDIR}/routines"

AC_PREFIX_DEFAULT([SCI/contrib/sip])
if test "x$prefix"  = xNONE; then 
   prefix="${SCIDIR}/contrib/sip"
fi
TOOLBOXDIR=$prefix
AC_SUBST(TOOLBOXDIR)dnl
AC_SUBST(SCIDIR)dnl 



#
# Set defines required to build DLLs and modules using MinGW
# thx to Bob F.
#
# These options are set for DLL build
#  libanimal:           _DLL ANIMAL_LIB
# TODO:
#   - update from SIMPLE toolbox
#
LIBRARY_EXTRA_CPPFLAGS=''
LIBRARY_EXTRA_LIBS=''
if test "${native_win32_build}" = 'yes'
then
   if test "${libtool_build_shared_libs}" = 'yes'
   then
      SHARED_LIB_SUFFIX=dll
      CPPFLAGS="$CPPFLAGS -DFORDLL -D_DLL"
      AC_MSG_WARN([[Don't forget to rename SCI/bin/LibScilab* to lowercase]])
      LDFLAGS="$LDFLAGS -L${SCIDIR}/bin"
      LIBS="-lscilab"
   else
      CPPFLAGS="$CPPFLAGS -D_LIB"
   fi
else
      SHARED_LIB_SUFFIX=dll
fi
AC_SUBST(SHARED_LIB_SUFFIX)
AC_SUBST(LIBRARY_EXTRA_CPPFLAGS)
AC_SUBST(LIBRARY_EXTRA_LIBS)


#
# Checks for libraries.
#
#AC_CHECK_PROG([has_pkgconfig],[pkg-config],[yes],[no], , )
#if test x$has_pkgconfig = xyes; then
   # pkg-config method
#   AC_PATH_LIB([animal],[0.3.0 -nocheck], [],[],[
#      #if found:
#      CFLAGS="$CFLAGS -W -Wall $ANIMAL_CFLAGS"
#      LIBS="$LIBS `Magick-config --libs` $ANIMAL_LIBS"
#   ], [AC_MSG_ERROR([[AnImaL version >= 0.3.0 was not found.]])])
#else

# animal-config method
AC_PATH_LIB_LIBCONFIG
AC_PATH_LIB([animal], [0.14.0 -nocheck], [animal-config], ,[
   # if found:
   if test "$GCC" = "yes";  then
      CFLAGS="$CFLAGS -W -Wall -Wno-implicit-int"
   fi
   CFLAGS="$CFLAGS $ANIMAL_CFLAGS"
   LIBS="$LIBS $ANIMAL_LIBS"
], [AC_MSG_ERROR([[Either AnImaL version >= 0.13.0 was not found or a compile test failed.]])],
[--cppflags], [--ldflags --libs])


##################################
#  I M A G E   M A G I C K
##################################

if test "${native_win32_build}" = 'yes'
then
   # under windows, set some stuff by hand
   AC_MSG_CHECKING([ImageMagick location])
   if test -z "$MAGICK_DIR"; then
      MAGICK_DIR="/c/imagemagick"
   fi
   AC_MSG_RESULT(["assuming $MAGICK_DIR"])

   IMAGEMAGICK_VERSION="6.0.3"
   LIBS="$LIBS -lCORE_RL_magick_"
   LDFLAGS="$LDFLAGS -L$MAGICK_DIR"
   CPPFLAGS="$CPPFLAGS -W -Wall -I$MAGICK_DIR/include"
   # CPPFLAGS="$CPPFLAGS -D_MAGICKLIB_"
   MAGICK_LIBNAME="CORE_RL_magick_"
else
   MAGICK_LIBNAME="Magick"
#
# ImageMagick tests to assure compatibility across MANY ImageMagick
# versions, from 5.4.x to 6.x.x
#
AC_PATH_LIB_LIBCONFIG
AC_PATH_LIB([ImageMagick], [[5.4.2 -nocheck]], [Magick-config], ,[
   # if found:
   if test x$IMAGEMAGICK_VERSION = "x5.5.5"; then
      AC_MSG_ERROR([[ImageMagick 5.5.5 has a bug which make it unusable with
AnImaL. Please download and install any other version from www.imagemagick.org]])
   fi
   if test x$IMAGEMAGICK_VERSION = "x6.0.1" -o x$IMAGEMAGICK_VERSION = "x6.0.0" ; then
      AC_MSG_WARN([[
****************
ImageMagick 6.0.0 and 6.0.1-3 have a bug which can \
make some images not to be read. To have a fully-functional \
system please download and install another version from \
www.imagemagick.org
****************]])
   fi
], [AC_MSG_ERROR([[ImageMagick version >= 5.4.2 was not found.]])],
[--cppflags --cflags], [--ldflags --libs])
fi

AC_MSG_CHECKING([if  ImageMagick links])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                    #include <time.h>
                                    #include <sys/types.h>
                                    #include <magick/api.h>]],
                  [[if (1)]]),
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no])
AC_MSG_ERROR([could not link to ImageMagick. Check the file 
   config.log and/or send it to the SIP developers.]))

# ------------------------------------
AC_MSG_CHECKING([for HWBColorspace in ImageMagick])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                    #include <time.h>
                                    #include <sys/types.h>
                                    #include <magick/api.h>]],
                  [[if (HWBColorspace == GRAYColorspace);]]),
   AC_MSG_RESULT([yes])
   AC_DEFINE([HAVE_HWBCOLORSPACE],[],
             [Defines if ImageMagick has HWBColorspace]),
   AC_MSG_RESULT([no]))

# ------------------------------------
AC_MSG_CHECKING([for HSLColorspace in ImageMagick])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                    #include <time.h>
                                    #include <sys/types.h>
                                    #include <magick/api.h>]],
                  [[if (HSLColorspace == GRAYColorspace);]]),
   AC_MSG_RESULT([yes])
   AC_DEFINE([HAVE_HSLCOLORSPACE],[],
             [Defines if ImageMagick has HSLColorspace]),
   AC_MSG_RESULT([no]))

# ------------------------------------
AC_MSG_CHECKING([for Virtual Pixel in ImageMagick])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                    #include <time.h>
                                    #include <sys/types.h>
                                    #include <magick/api.h>]],
                  [[VirtualPixelMethod vpm;]]),
   AC_MSG_RESULT([yes])
   AC_DEFINE([HAVE_VIRTUAL_PIXEL],[],
             [Defines if ImageMagick has Virtual Pixel specifications]),
   AC_MSG_RESULT([no]))

# ------------------------------------
AC_MSG_CHECKING([whether QueryColorDatabase has 3rd parameter in IM])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                    #include <time.h>
                                    #include <sys/types.h>
                                    #include <magick/api.h>]],
                  [[QueryColorDatabase("black",(PixelPacket *)NULL,\
                  (ExceptionInfo *)NULL);]]),
   AC_MSG_RESULT([yes])
   AC_DEFINE([HAVE_QueryColorDatabase_3args],[],
      [Defines if ImageMagick has QueryColorDatabase with a 3rd parameter]),
   AC_MSG_RESULT([no]))
# ------------------------------------
AC_MSG_CHECKING([whether MagickWand.h in IM])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                    #include <time.h>
                                    #include <sys/types.h>
                                    #include <wand/MagickWand.h>]],
                  [[ ]]),
   AC_MSG_RESULT([yes])
   AC_DEFINE([HAVE_MagickWand],[],
      [Defines if ImageMagick has MagickWand API]),
   AC_MSG_RESULT([no]))
# ------------------------------------
AC_MSG_CHECKING([whether MogrifyImage has 5th parameter in IM])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                    #include <time.h>
                                    #include <sys/types.h>
                                    #include <wand/MagickWand.h>]],
                  [[ ImageInfo *image_info; Image *image; char **argv;\
                  ExceptionInfo exception;
                  MogrifyImage(image_info, 1, argv, &image, &exception);]]),
   AC_MSG_RESULT([yes])
   AC_DEFINE([HAVE_MogrifyImage_5args],[],
      [Defines if ImageMagick has MogrifyImage with a 5th parameter]),
   AC_MSG_RESULT([no]))

# ------------------------------------
AC_CHECK_LIB([$MAGICK_LIBNAME],[GetLocaleExceptionMessage],[
   AC_DEFINE([HAVE_GetLocaleExceptionMessage],[], [Define if \
ImageMagick has locale error messages.])],
[])

# ------------------------------------
AC_MSG_CHECKING([name of the default compression quality in IM])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                    #include <time.h>
                                    #include <sys/types.h>
                                    #include <magick/api.h>]],
                  [[unsigned a=UndefinedCompressionQuality;]]),
   AC_MSG_RESULT([UndefinedCompressionQuality])
   AC_DEFINE([MAGICK_DEFAULT_COMPRESSION_QUALITY],
             [UndefinedCompressionQuality],
             [The name of the default compression quality in this \
particular ImageMagick setup]),
   #----- Not UndefinedCompression. Test for UndefinedCompressionQuality
    AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                        #include <time.h>
                                        #include <sys/types.h>
                                        #include <magick/api.h>]],
                      [[unsigned a=UndefinedCompression;]]),
       AC_MSG_RESULT([UndefinedCompression])
       AC_DEFINE([MAGICK_DEFAULT_COMPRESSION_QUALITY],
                 [UndefinedCompression],
                 [The name of the default compression quality in this \
    particular ImageMagick setup]),
   #----- Not UndefinedCompression. Test for UndefinedCompressionQuality
        AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[#include <stdio.h>
                                            #include <time.h>
                                            #include <sys/types.h>
                                            #include <magick/api.h>]],
                          [[unsigned a=DefaultCompressionQuality;]]),
           AC_MSG_RESULT([DefaultCompressionQuality])
           AC_DEFINE([MAGICK_DEFAULT_COMPRESSION_QUALITY],
                     [DefaultCompressionQuality],
                     [The name of the default compression quality in this \
        particular ImageMagick setup]),
       AC_MSG_ERROR([No compression quality name detected. Please report your ImageMagick version in the bugtracker at http://siptoolbox.sf.net
         ])
          )
     )
   
   )


# END OF IMAGEMAGICK TESTS
##############################################

# Checks for header files.
AC_HEADER_STDC
if test x"$ac_cv_header_stdc" = x'no'; then
   AC_MSG_WARN([[this package uses ANSI-C headers but none were found.]])
fi
AC_CHECK_HEADERS([string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_HEADER_STDBOOL


# Checks for library functions.

AC_CHECK_FUNC([strcasecmp],[],[AC_MSG_ERROR([[this package needs the strcasecmp function.]])])

AC_CONFIG_FILES([Makefile src/Makefile macros/Makefile
                 images/Makefile images/photonics/Makefile
                 man/Makefile config/loader.sce config/sip_xmltohtml.sci
                 config/loader_include.sce])
AC_OUTPUT

echo 
echo "Final Compilation Flags:"
echo "  CC       = $CC"
echo "  CFLAGS   = $CFLAGS"
echo "  CPPFLAGS = $CPPFLAGS"
echo "  LDFLAGS  = $LDFLAGS"
echo "  LIBS     = $LIBS"
echo 
echo ===========================================================================
echo "This toolbox will be installed in $TOOLBOXDIR."
echo 'Now type "make" then "make install".'
echo "After that, if you want the toolbox to be automatically loaded, type"
echo '"make autoload" (as normal user). This will append the proper command'
echo "to your .scilab file."
echo ===========================================================================
echo 
