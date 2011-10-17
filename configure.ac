# Process this file with autoconf to produce a configure script.
#
# Copyright (C) 2003-2009 Ricardo Fabbri
# Some inspiration came from ImageMagick's configure.ac (thx Bob F.)
# and examples from the Autobook (http://sources.redhat.com/autobook),
# and some changes were re-incorporated from SIVP.
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

AC_PREREQ(2.61)

#AC_INIT([sip], [20040710-2], [ricardofabbri@users.sf.net],[sip-dev])
AC_INIT([sip], [0.5.8-git], [ricardofabbri@users.sourceforge.net])
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


AC_CHECK_PROG([has_scilab],[scilab],[yes],[no], , )
if test x$has_scilab = xno; then
    AC_MSG_ERROR([[Scilab was not found in your PATH], your PATH is $PATH])
fi

SCIEXE="scilab"
# test if SCI is defined. If not, test if scilab is in path,
# then get SCI from scilab -nw
if test -z "$SCI"; then
      cmd='F=mopen("getpath.incl","w");
           mfprintf(F,SCI);
           mclose(F);exit;'
      echo "$cmd" > getpath.sci
      scilab -nwni -nb -nouserstartup -f getpath.sci # >/dev/null
      SCIDIR=`cat getpath.incl`
      rm -f getpath.sci getpath.incl 
else
   SCIDIR="$SCI"
fi

#get scilab version
cmd='F=mopen("version.incl","w");
     myver=getversion();
     myver=part(myver, 8);
     mfprintf(F,myver);
     mclose(F);exit;'
echo "$cmd" > version.sci
scilab -nwni -nb -nouserstartup -f version.sci # >/dev/null
SCIVERSION=`cat version.incl`
rm -f version.sci version.incl 

# scilab-branch-xx (b as in branch)
# TODO detect scilab yasp branch
if test $SCIVERSION = b; then
  SCIVERSION=5
fi
echo "Scilab version is $SCIVERSION"


HELPDIR=""
if test $SCIVERSION -le 4; then
   # checks for xsltproc (to buld HTML help pages)
   AC_CHECK_PROG([has_xsltproc],[xsltproc],[yes],[no], , )
   if test x$has_xsltproc = xno; then
      # checks for sabcmd as an alternative
      AC_CHECK_PROG([has_sablotron],[sabcmd],[yes],[no], , )
      if test x$has_sablotron = xno; then
        AC_MSG_ERROR([[xsltproc or sabcmd programs were not found but \
                   at least one of them is required to build the SIP manual pages.
                   Download xslproc (from libxml) at:
                     http://www.xmlsoft.org 
                   or sabcmd (from sablotron) at:
                     http://www.gingerall.com/charlie/ga/xml/p_sab.xml]])
         fi
    fi
    HELPDIR="man"
else
    HELPDIR="help"  
fi



# Determine the include path for the scilab headers.
# 
# The following tests were partly based off ilib_compile from scilab v. 5.1 in
# modules/dynamic_link/macros

# This works for the binary version: SCIDIR=/usr/local/scilab-5.1

SCILIBDIR="";
if test $SCIVERSION -le 4; then
  CPPFLAGS="$CPPFLAGS -I$SCIDIR/routines"
else
  if test -d "$SCIDIR/../../include/scilab/core"; then
    # Binary version
    SCI_INCL="$SCIDIR/../../";
    CPPFLAGS="$CPPFLAGS  -I$SCI_INCL/include/scilab/routines/ -I$SCI_INCL/include/scilab/core/ -I$SCI_INCL/include/scilab/mexlib/"
    if test -d "$SCI_INCL/lib/scilab"; then
      SCILIBDIR="$SCI_INCL/lib/scilab";
    fi
  else 
    if test -d "$SCIDIR/modules/core/includes"; then
      # Source tree version
      SCI_INCL="$SCIDIR/modules";
      CPPFLAGS="$CPPFLAGS -I$SCI_INCL/routines/includes/ -I$SCI_INCL/core/includes/ -I$SCI_INCL/mexlib/includes/"
      if test -d "$SCI_INCL/core/lib"; then
        SCILIBDIR="$SCI_INCL/core/lib";
      fi
    else
      if test -d "/usr/include/scilab"; then
        # System version
        SCI_INCL="/usr/include/scilab"
        CPPFLAGS="$CPPFLAGS  -I$SCI_INCL/routines/ -I$SCI_INCL/core/ -I$SCI_INCL/mexlib/ -I$SCI_INCL/"
        if test -e "$SCI_INCL/sci_gateway.h" -o -e "$SCI_INCL/routines/sci_gateway.h" -o -e "$SCI_INCL/core/sci_gateway.h" -o -e "$SCI_INCL/mexlib/sci_gateway.h"; then
          AC_DEFINE([HAVE_SCI_GATEWAY_H],[],
                    [Defines if Scilab has a sci_gateway.h file separate from mex.h])
        fi
        if test -d "/usr/lib/scilab"; then
          SCILIBDIR="/usr/lib/scilab";
        fi
      else
        if test -d "$SCIDIR/../../include/scilab"; then
          # Scilab binary version around 5.2.2
          SCI_INCL="$SCIDIR/../../";
          CPPFLAGS="$CPPFLAGS  -I$SCI_INCL/include/scilab/";
          AC_DEFINE([HAVE_SCI_GATEWAY_H],[],
                    [Defines if Scilab has a sci_gateway.h file separate from mex.h])
          if test -d "$SCI_INCL/lib/scilab"; then
            SCILIBDIR="$SCI_INCL/lib/scilab";
          fi
        else
          AC_MSG_ERROR([[Cannot locate the include files from Scilab. Please report this.]])
        fi
      fi
    fi
  fi
fi

# determine location of libscilab for building test subdir
if test -z $SCILIBDIR; then
  if test -d "/usr/lib/scilab"; then
    SCILIBDIR="/usr/lib/scilab"
  else 
    if test -d "/usr/local/lib/scilab"; then
      SCILIBDIR="/usr/local/lib/scilab"
    else
      AC_MSG_WARN([[Cannot find location of Scilab libraries needed for tests subdir]])
    fi
  fi
fi

if test "x$prefix"  = xNONE; then 
   prefix="${SCIDIR}/contrib/${PACKAGE_NAME}-${PACKAGE_VERSION}"
else
   prefix="${prefix}/${PACKAGE_NAME}-${PACKAGE_VERSION}"
fi

TOOLBOXDIR=$prefix

AC_PREFIX_DEFAULT([$TOOLBOXDIR])
AC_SUBST(TOOLBOXDIR)
AC_SUBST(SCIDIR)
AC_SUBST(SCILIBDIR)
AC_SUBST(SCIEXE)
AC_SUBST(HELPDIR)

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
      SHARED_LIB_SUFFIX=so
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
   # TODO change this is not going to be true for newer versions of imagemagick,
   # so we must figure this out by testing if the version is greater than
   # 6.3.8-5. Right now, I repeat some tests for both Magick and MagickCore
#
# ImageMagick tests to assure compatibility across MANY ImageMagick
# versions, from 5.4.x to 6.x.x
#
AC_PATH_LIB_LIBCONFIG


AC_CHECK_PROG([has_magickwand_config],[MagickWand-config],[yes],[no], , )

MAGICK_CONFIG="Magick-config"
if test x$has_magickwand_config = xyes; then
  # This is necessary because MogrifyImage is in MagickWand
  # TODO: this can be reconciled with a test for version 6.3.8-5 (when unix
  # refactoring occurred)
  MAGICK_CONFIG="MagickWand-config"
fi




AC_PATH_LIB([ImageMagick], [[5.4.2 -nocheck]], [$MAGICK_CONFIG], ,[
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

   AC_SUBST(IMAGEMAGICK_LIBS)
   AC_SUBST(IMAGEMAGICK_LDFLAGS)
   AC_SUBST(IMAGEMAGICK_CPPFLAGS)

   LIBS="$IMAGEMAGICK_LIBS $LIBS"
   LDFLAGS="$IMAGEMAGICK_LDFLAGS $LDFLAGS"
   CPPFLAGS="$IMAGEMAGICK_CPPFLAGS $CPPFLAGS"
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
                                    #include <magick/api.h>]],
                  [[ ImageInfo *image_info; Image *image; char **argv;\
                  ExceptionInfo exception;
                  MogrifyImage(image_info, 1, argv, &image, &exception);]]),
   AC_MSG_RESULT([yes])
   AC_DEFINE([HAVE_MogrifyImage_5args],[],
      [Defines if ImageMagick has MogrifyImage with a 5th parameter]),
   AC_MSG_RESULT([no]))

# ------------------------------------
# TODO: set MAGICK_LIBNAME appropriately and change this test to use
# MAGICK_LIBNAME. 
AC_CHECK_LIB([Magick],[GetLocaleExceptionMessage],[
  # if found
  AC_DEFINE([HAVE_GetLocaleExceptionMessage],[], [Define if \
ImageMagick has locale error messages.])
   ]
, [
  # if not found, try MagickCore
  AC_CHECK_LIB([MagickCore],[GetLocaleExceptionMessage],[
    AC_DEFINE([HAVE_GetLocaleExceptionMessage],[], [Define if \
  ImageMagick has locale error messages.])] , [])
])

# ------------------------------------
# Some distros (Ubuntu 11.04) fail to have this symbol in the shared libs
AC_CHECK_LIB([Magick],[StringToDouble],[
  # if found
  AC_DEFINE([HAVE_StringToDouble],[], [Define if \
ImageMagick has StringToDouble.])
   ]
, [
  # if not found, try MagickCore
  AC_CHECK_LIB([MagickCore],[StringToDouble],[
    AC_DEFINE([HAVE_StringToDouble],[], [Define if \
  ImageMagick has StringToDouble.])] , [
   AC_MSG_WARN([[sip mogrify -fuzz for segmentation will not work in this system.]])
    ])
])

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

# OpenCV tests (just for the future.)
#have_opencv=no
#PKG_CHECK_MODULES(OPENCV, 
#    [opencv >= 0.9.6],
#    [CFLAGS="$CFLAGS $OPENCV_CFLAGS"
#     LDFLAGS="$LDFLAGS $OPENCV_LIBS"
#     have_opencv=yes
#     AC_DEFINE(HAVE_OPENCV,[], [define HAVE_OPENCV if you have opencv])],
#    [AC_MSG_WARN([[ OpenCV version >= 0.9.6 was not found. ]])])


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
AC_CHECK_FUNCS([malloc])

AC_CONFIG_FILES([Makefile 
  src/Makefile 
  macros/Makefile
  images/Makefile 
  images/photonics/Makefile
  man/Makefile 
  help/Makefile 
  config/loader.sce 
  config/sip_xmltohtml.sci
  config/loader_include.sce
  tests/Makefile])

AC_OUTPUT

AC_MSG_RESULT([
=====================================================
Configuration:
  CC       = $CC
  CFLAGS   = $CFLAGS
  CPPFLAGS = $CPPFLAGS
  LDFLAGS  = $LDFLAGS
  LIBS     = $LIBS
=====================================================
This toolbox will be installed in $TOOLBOXDIR
Please run 'make' and 'make install' to compile and install the toolbox. 
]) 

