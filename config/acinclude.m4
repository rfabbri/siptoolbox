dnl=============================================================================
dnl @synopsis AX_PREFIX_CONFIG_H [(OUTPUT-HEADER [,PREFIX [,ORIG-HEADER]])]
dnl
dnl * this is a new variant from ac_prefix_config_
dnl this one will use a lowercase-prefix if
dnl the config-define was starting with a lowercase-char, e.g. 
dnl   #define const or #define restrict or #define off_t
dnl (and this one can live in another directory, e.g. testpkg/config.h
dnl therefore I decided to move the output-header to be the first arg)
dnl
dnl takes the usual config.h generated header file; looks for each of
dnl the generated "#define SOMEDEF" lines, and prefixes the defined name
dnl (ie. makes it "#define PREFIX_SOMEDEF". The result is written to
dnl the output config.header file. The PREFIX is converted to uppercase 
dnl for the conversions. 
dnl
dnl - default OUTPUT-HEADER = $PACKAGE-config.h
dnl - default PREFIX = $PACKAGE
dnl - default ORIG-HEADER, from AM_CONFIG_HEADER(config.h)
dnl
dnl Your configure.ac script should contain both macros in this order,
dnl and unlike the earlier variations of this prefix-macro it is okay to
dnl place the AX_PREFIX_CONFIG_H call before the AC_OUTPUT invokation.
dnl
dnl example:
dnl   AC_INIT(config.h.in)        # config.h.in as created by "autoheader"
dnl   AM_INIT_AUTOMAKE(testpkg, 0.1.1)    # makes #undef VERSION and PACKAGE
dnl   AM_CONFIG_HEADER(config.h)          # prep config.h from config.h.in
dnl   AX_PREFIX_CONFIG_H(mylib/_config.h) # prep mylib/_config.h from it..
dnl   AC_MEMORY_H                         # makes "#undef NEED_MEMORY_H"
dnl   AC_C_CONST_H                        # makes "#undef const"
dnl   AC_OUTPUT(Makefile)                 # creates the "config.h" now
dnl                                       # and also mylib/_config.h
dnl
dnl if the argument to AX_PREFIX_CONFIG_H would have been omitted then the
dnl default outputfile would have been called simply "testpkg-config.h", but
dnl even under the name "mylib/_config.h" it contains prefix-defines like
dnl   #ifndef TESTPKG_VERSION 
dnl   #define TESTPKG_VERSION "0.1.1"
dnl   #endif
dnl   #ifndef TESTPKG_NEED_MEMORY_H 
dnl   #define TESTPKG_NEED_MEMORY_H 1
dnl   #endif
dnl   #ifndef _testpkg_const 
dnl   #define _testpkg_const _const
dnl   #endif
dnl
dnl   and this "mylib/_config.h" can be installed along with other
dnl   header-files, which is most convenient when creating a shared
dnl   library (that has some headers) where some functionality is
dnl   dependent on the OS-features detected at compile-time. No
dnl   need to invent some "mylib-confdefs.h.in" manually. :-)
dnl
dnl Note that some AC_DEFINEs that end up in the config.h file are 
dnl actually self-referential - e.g. AC_C_INLINE, AC_C_CONST, and the
dnl AC_TYPE_OFF_T say that they "will define inline|const|off_t if the
dnl system does not do it by itself". You might want to clean up about
dnl these - consider an extra mylib/conf.h that reads something like:
dnl
dnl    #include <mylib/_config.h>
dnl    #ifndef _testpkg_const
dnl    #define _testpkg_const const
dnl    #endif
dnl
dnl and then start using _testpkg_const in the header files. That is
dnl also a good thing to differentiate whether some library-user has
dnl starting to take up with a different compiler, so perhaps it could
dnl read something like this:
dnl
dnl   #ifdef _MSC_VER
dnl   #include <mylib/_msvc.h>
dnl   #else
dnl   #include <mylib/_config.h>
dnl   #endif
dnl   #ifndef _testpkg_const
dnl   #define _testpkg_const const
dnl   #endif
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005-07-20 01:33:34 aerringt Exp $
dnl @author  Guiodo Draheim <guidod@gmx.de>

AC_DEFUN([AX_PREFIX_CONFIG_H],[AC_REQUIRE([AC_CONFIG_HEADER])
AC_CONFIG_COMMANDS([ifelse($1,,$PACKAGE-config.h,$1)],[dnl

ac_prefix_conf_OUT=`echo ifelse($1, , $PACKAGE-config.h, $1)`
ac_prefix_conf_DEF=`echo _$ac_prefix_conf_OUT | sed -e 'y:abcdefghijklmnopqrstuvwxyz./,-:ABCDEFGHIJKLMNOPQRSTUVWXYZ____:'`
ac_prefix_conf_PKG=`echo ifelse($2, , $PACKAGE, $2)`
ac_prefix_conf_LOW=`echo _$ac_prefix_conf_PKG | sed -e 'y:ABCDEFGHIJKLMNOPQRSTUVWXYZ-:abcdefghijklmnopqrstuvwxyz_:'`
ac_prefix_conf_UPP=`echo $ac_prefix_conf_PKG | sed -e 'y:abcdefghijklmnopqrstuvwxyz-:ABCDEFGHIJKLMNOPQRSTUVWXYZ_:'  -e '/^[0-9]/s/^/_/'`
ac_prefix_conf_INP=`echo ifelse($3, , _, $3)`
if test "$ac_prefix_conf_INP" = "_"; then
   for ac_file in : $CONFIG_HEADERS; do test "_$ac_file" = _: && continue
     test -f "$ac_prefix_conf_INP" && continue
     case $ac_file in
        *.h) test -f $ac_file && ac_prefix_conf_INP=$ac_file ;;
        *)
     esac
   done
fi
if test "$ac_prefix_conf_INP" = "_"; then
   case $ac_prefix_conf_OUT in
      */*) ac_prefix_conf_INP=`basename $ac_prefix_conf_OUT` 
      ;;
      *-*) ac_prefix_conf_INP=`echo $ac_prefix_conf_OUT | sed -e 's/[a-zA-Z0-9_]*-//'`
      ;;
      *) ac_prefix_conf_INP=config.h
      ;;
   esac
fi
if test -z "$ac_prefix_conf_PKG" ; then
   AC_MSG_ERROR([no prefix for _PREFIX_PKG_CONFIG_H])
else
  AC_MSG_NOTICE(creating $ac_prefix_conf_OUT - prefix $ac_prefix_conf_UPP for $ac_prefix_conf_INP defines)
  if test -f $ac_prefix_conf_INP ; then
    echo '#ifndef '$ac_prefix_conf_DEF      >$tmp/pconfig.h
    echo '#define '$ac_prefix_conf_DEF' 1' >>$tmp/pconfig.h
    echo ' ' >>$tmp/pconfig.h
    echo /'*' $ac_prefix_conf_OUT. Generated automatically at end of configure. '*'/ >>$tmp/pconfig.h

    echo 's/#undef  *\([A-Z_]\)/#undef '$ac_prefix_conf_UPP'_\1/' >conftest.prefix
    echo 's/#undef  *\([a-z]\)/#undef '$ac_prefix_conf_LOW'_\1/' >>conftest.prefix
    echo 's/#define  *\([A-Z_][A-Za-z0-9_]*\)\(.*\)/#ifndef '$ac_prefix_conf_UPP"_\\1 \\" >>conftest.prefix
    echo '#define '$ac_prefix_conf_UPP"_\\1 \\2 \\" >>conftest.prefix
    echo '#endif/' >>conftest.prefix
    echo 's/#define  *\([a-z][A-Za-z0-9_]*\)\(.*\)/#ifndef '$ac_prefix_conf_LOW"_\\1 \\" >>conftest.prefix
    echo '#define '$ac_prefix_conf_LOW"_\\1 \\2 \\" >>conftest.prefix
    echo '#endif/' >>conftest.prefix
    sed -f conftest.prefix $ac_prefix_conf_INP >>$tmp/pconfig.h
    echo ' ' >>$tmp/pconfig.h
    echo '/* once:' $ac_prefix_conf_DEF '*/' >>$tmp/pconfig.h
    echo '#endif' >>$tmp/pconfig.h
    if cmp -s $ac_prefix_conf_OUT $tmp/pconfig.h 2>/dev/null; then
      AC_MSG_NOTICE([$ac_prefix_conf_OUT is unchanged])
    else
      ac_dir=`AS_DIRNAME(["$ac_prefix_conf_OUT"])`
      AS_MKDIR_P(["$ac_dir"])
      rm -f $ac_prefix_conf_OUT
      mv $tmp/pconfig.h $ac_prefix_conf_OUT
    fi
  else
    AC_MSG_ERROR([input file $ac_prefix_conf_IN does not exist - dnl
    skip generating $ac_prefix_conf_OUT])
  fi
  rm -f conftest.* 
fi
],[PACKAGE="$PACKAGE"])])

dnl=============================================================================
dnl Generic library check macro
dnl
dnl by Roger Leigh <roger@whinlatter.uklinux.net>
dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_path_lib.html
dnl 
dnl Hacked by Ricardo Fabbri to take into account CFLAG_CONFIG_ARGS
dnl and LIBS_CONFIG_ARGS, which tells AC_PATH_LIB what are the parameters
dnl to CONFIG-SCRIPT which gives the correct information.
dnl
## Table of Contents:
## 1. The main macro
## 2. Core macros
## 3. Integrity checks
## 4. Error reporting
## 5. Feature macros


## ------------------ ##
## 1. The main macro. ##
## ------------------ ##


# AC_PATH_LIB(LIBRARY, MINIMUM-VERSION, CONFIG-SCRIPT,
#              MODULES, ACTION-IF-FOUND, ACTION-IF-NOT-FOUND,
#              CFLAGS_CONFIG_ARGS, LIBS_CONFIG_ARGS)
# -----------------------------------------------------------
# Check for the presence of libLIBRARY, with a minumum version
# MINIMUM-VERSION.
#
# If needed, use the libconfig script CONFIG-SCRIPT.  If the script
# needs extra modules specifying, pass them as MODULES.
#
# Run ACTION-IF-FOUND if the library is present and all tests pass, or
# ACTION-IF-NOT-FOUND if it is not present or any tests fail.
AC_DEFUN([AC_PATH_LIB],[# check for presence of lib$1
dnl We're going to need uppercase, lowercase and user-friendly
dnl versions of the string `LIBRARY', and long and cache variants.
m4_pushdef([UP], m4_translit([$1], [a-z], [A-Z]))dnl
m4_pushdef([DOWN], m4_translit([$1], [A-Z], [a-z]))dnl
m4_pushdef([USERFRIENDLY],[$1])dnl
m4_pushdef([LDOWN], ac_path_lib_[]DOWN)dnl
m4_pushdef([CACHE], ac_cv_path_lib_[]DOWN)dnl
m4_pushdef([ERRORLOG], error.[]DOWN[]test)dnl
_AC_PATH_LIB_INIT([$1], [$3])
_AC_PATH_LIB_CHECK([$1], [$2], [$4], [$7], [$8])
_AC_PATH_LIB_CHECK_TESTS([$2])
_AC_PATH_LIB_ERROR_DUMP
_AC_PATH_LIB_FINI([$5], [$6])
dnl Pop the macros we defined earlier.
m4_popdef([UP])dnl
m4_popdef([DOWN])dnl
m4_popdef([USERFRIENDLY])dnl
m4_popdef([LDOWN])dnl
m4_popdef([CACHE])dnl
m4_popdef([ERRORLOG])dnl
])# AC_PATH_LIB




## --------------- ##
## 2. Core macros. ##
## --------------- ##


# _AC_PATH_LIB_INIT(LIBRARY, CONFIG-SCRIPT)
# -----------------------------------------
# Initialisation of defaults and options.
# Remove error log from previous runs.
AC_DEFUN([_AC_PATH_LIB_INIT],
[_AC_PATH_LIB_DEFAULTS([$1], [$2])
_AC_PATH_LIB_OPTIONS
rm -f ERRORLOG
# Save variables in case check fails.
ac_save_[]UP[]_CFLAGS="$UP[]_CFLAGS"
ac_save_[]UP[]_LIBS="$UP[]_LIBS"
ac_save_[]UP[]_VERSION="$UP[]_VERSION"
])


# _AC_PATH_LIB_DEFAULTS(LIBRARY, HEADER, CONFIG-SCRIPT)
# -----------------------------------------------------
# Set up default behaviour.
AC_DEFUN([_AC_PATH_LIB_DEFAULTS],
[dnl Set up pkgconfig as default config script.
m4_ifdef([AC_PATH_LIB_USEPKGCONFIG],, [AC_PATH_LIB_PKGCONFIG])
dnl Set default header and config script names.
LDOWN[]_header="m4_default([_PATH_LIB_HEADER], [$1/$1.h])"
LDOWN[]_config="m4_default([$2], [$1-config])"
ifdef([_AC_PATH_LIB_VERSION_PREFIX],,
      [m4_define([_AC_PATH_LIB_VERSION_PREFIX],
                 DOWN[_])
      ])
ifdef([_AC_PATH_LIB_VERSION_MAJOR],,
      [m4_define([_AC_PATH_LIB_VERSION_MAJOR],
                 [major])
      ])
ifdef([_AC_PATH_LIB_VERSION_MINOR],,
      [m4_define([_AC_PATH_LIB_VERSION_MINOR],
                 [minor])
      ])
ifdef([_AC_PATH_LIB_VERSION_MICRO],,
      [m4_define([_AC_PATH_LIB_VERSION_MICRO],
                 [micro])
      ])
ifdef([_AC_PATH_LIB_VERSION_SUFFIX],,
      [m4_define([_AC_PATH_LIB_VERSION_SUFFIX],
                 [_version])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_PREFIX],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_PREFIX],
                 UP[_])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_MAJOR],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_MAJOR],
                 [MAJOR])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_MINOR],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_MINOR],
                 [MINOR])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_MICRO],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_MICRO],
                 [MICRO])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_SUFFIX],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_SUFFIX],
                 [_VERSION])
      ])
])# _AC_PATH_LIB_DEFAULTS


# _AC_PATH_LIB_OPTIONS
# --------------------
# configure options.
AC_DEFUN([_AC_PATH_LIB_OPTIONS],
[m4_if(AC_PATH_LIB_USEPKGCONFIG, [true], ,
[AC_ARG_WITH(DOWN-prefix,
            AC_HELP_STRING([--with-DOWN-prefix=PFX],
                           [prefix where USERFRIENDLY is installed (optional)]),
            [LDOWN[]_config_prefix="$withval"],
            [LDOWN[]_config_prefix=""])dnl
AC_ARG_WITH(DOWN-exec-prefix,
            AC_HELP_STRING([--with-DOWN-exec-prefix=PFX],
                           [exec-prefix where USERFRIENDLY is installed (optional)]),
            [LDOWN[]_config_exec_prefix="$withval"],
            [LDOWN[]_config_exec_prefix=""])dnl
])
AC_ARG_ENABLE(DOWN[]test,
              AC_HELP_STRING([--disable-DOWN[]test],
                             [do not try to compile and run a test USERFRIENDLY program]),
              [LDOWN[]_test_enabled="no"],
              [LDOWN[]_test_enabled="yes"])dnl
])# _AC_PATH_LIB_OPTIONS


# _AC_PATH_LIB_CHECK(LIBRARY, MINIMUM-VERSION, MODULES, [CFLAG_CONFIG_ARGS,
# LIBS_CONFIG_ARGS])
# -----------------------------------------------------
# Obtain library CFLAGS, LIBS and VERSION information.  Cache results,
# but set avoidcache to no if config program is not available.  Break
# up available and minumum version into major, minor and micro version
# numbers.
AC_DEFUN([_AC_PATH_LIB_CHECK],
[
# Set up LIBRARY-config script parameters
m4_if([$3], , ,
      [LDOWN[]_config_args="$LDOWN[]_config_args $3"])
LDOWN[]_min_version=`echo "$2" | sed -e 's/ -nocheck//'`
m4_if([$2], , ,[if test "$LDOWN[]_min_version" = "$2" ; then
                  LDOWN[]_version_test_enabled="yes"
                fi])
m4_if(AC_PATH_LIB_USEPKGCONFIG, [true],
[LDOWN[]_config_args="$1 $LDOWN[]_config_args"
],
[  if test x$LDOWN[]_config_exec_prefix != x ; then
    LDOWN[]_config_args="$LDOWN[]_config_args --exec-prefix=$LDOWN[]_config_exec_prefix"
  fi
  if test x$LDOWN[]_config_prefix != x ; then
    LDOWN[]_config_args="$LDOWN[]_config_args --prefix=$LDOWN[]_config_prefix"
  fi
])
dnl set --version for libconfig or --modversion for pkgconfig
m4_if(AC_PATH_LIB_USEPKGCONFIG, [true],
      [AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
       if test x$PKG_CONFIG != xno ; then
         if pkg-config --atleast-pkgconfig-version 0.7 ; then
           :
         else
           AC_PATH_LIB_ERROR([A new enough version of pkg-config was not found:])
           AC_PATH_LIB_ERROR([version 0.7 or better required.])
           AC_PATH_LIB_ERROR([See http://pkgconfig.sourceforge.net])
           PKG_CONFIG=no
         fi
       fi
       UP[]_CONFIG="$PKG_CONFIG"
       LDOWN[]_config="pkg-config"
       m4_pushdef([LIBCONFIG_CFLAGS], [--cflags])
       m4_pushdef([LIBCONFIG_LIBS], [--libs])
       m4_pushdef([LIBCONFIG_VERSION], [--modversion])
      ],
      [AC_PATH_PROG(UP[]_CONFIG, $LDOWN[]_config, no)
       dnl m4_pushdef([LIBCONFIG_CFLAGS], [--cflags])
       dnl m4_pushdef([LIBCONFIG_LIBS], [--libs])
       m4_pushdef([LIBCONFIG_CFLAGS], [$4])
       m4_pushdef([LIBCONFIG_LIBS], [$5])
       m4_pushdef([LIBCONFIG_VERSION], [--version])
       if test x$UP[]_CONFIG = xno ; then
         AC_PATH_LIB_ERROR([The $LDOWN[]_config script installed by USERFRIENDLY could not be found.])
         AC_PATH_LIB_ERROR([If USERFRIENDLY was installed in PREFIX, make sure PREFIX/bin is in])
         AC_PATH_LIB_ERROR([your path, or set the UP[]_CONFIG environment variable to the])
         AC_PATH_LIB_ERROR([full path to $LDOWN[]_config.])
       fi
      ])

if test x$UP[]_CONFIG = xno ; then
  LDOWN[]_present_avoidcache="no"
else
  LDOWN[]_present_avoidcache="yes"

  AC_CACHE_CHECK([for USERFRIENDLY CFLAGS],
                 [CACHE[]_cflags], [
     CACHE[]_cflags=`$UP[]_CONFIG $LDOWN[]_config_args LIBCONFIG_CFLAGS \
     | tr '\012' \ `
  ])
  AC_CACHE_CHECK([for USERFRIENDLY LIBS],
                 [CACHE[]_libs], [
     CACHE[]_libs=`$UP[]_CONFIG $LDOWN[]_config_args LIBCONFIG_LIBS \
     | tr '\012' \ `
  ])
  AC_CACHE_CHECK([for USERFRIENDLY VERSION],
                 [CACHE[]_version], [
     CACHE[]_version=`$UP[]_CONFIG $LDOWN[]_config_args LIBCONFIG_VERSION \
     | tr '\012' \ `
  ])
  UP[]_CFLAGS="$CACHE[]_cflags"
  UP[]_LIBS="$CACHE[]_libs"
  UP[]_VERSION="$CACHE[]_version"
  LDOWN[]_config_major_version=`echo "$CACHE[]_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
  LDOWN[]_config_minor_version=`echo "$CACHE[]_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
  LDOWN[]_config_micro_version=`echo "$CACHE[]_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
  LDOWN[]_min_major_version=`echo "$LDOWN[]_min_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
  LDOWN[]_min_minor_version=`echo "$LDOWN[]_min_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
  LDOWN[]_min_micro_version=`echo "$LDOWN[]_min_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
fi
m4_popdef([LIBCONFIG_CFLAGS])dnl
m4_popdef([LIBCONFIG_LIBS])dnl
m4_popdef([LIBCONFIG_VERSION])dnl
])# _AC_PATH_LIB_CHECK


# _AC_PATH_LIB_FINI(ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
# -------------------------------------------------------
# Finish: report errors and define/substitute variables.  Run any
# user-supplied code for success or failure.  Restore defaults.
AC_DEFUN([_AC_PATH_LIB_FINI],
[dnl define variables and run extra code
UP[]_CFLAGS="$CACHE[]_cflags"
UP[]_LIBS="$CACHE[]_libs"
UP[]_VERSION="$CACHE[]_version"
AC_SUBST(UP[]_CFLAGS)dnl
AC_SUBST(UP[]_LIBS)dnl
AC_SUBST(UP[]_VERSION)dnl
# Run code which depends upon the test result.
if test x$CACHE[]_present = xyes ; then
  m4_if([$1], , :, [$1])
else
# Restore saved variables if test fails
  UP[]_CFLAGS="$ac_save_[]UP[]_CFLAGS"
  UP[]_LIBS="$ac_save_[]UP[]_LIBS"
  UP[]_VERSION="$ac_save_[]UP[]_VERSION"
  m4_if([$2], , :, [$2])
fi
dnl Restore defaults
AC_PATH_LIB_CHECK_REGISTER_DEFAULTS
AC_PATH_LIB_PKGCONFIG
])# _AC_PATH_LIB_FINI




## -------------------- ##
## 3. Integrity checks. ##
## -------------------- ##


# _AC_PATH_LIB_CHECK_TESTS(MINIMUM-VERSION)
# -----------------------------------------
# Now check if the installed USERFRIENDLY is sufficiently new. (Also sanity
# checks the results of DOWN-config to some extent
AC_DEFUN([_AC_PATH_LIB_CHECK_TESTS],
[AC_CACHE_CHECK([for USERFRIENDLY - m4_if([$1], ,
                               [any version],
                               [version >= $LDOWN[]_min_version])],
               [CACHE[]_present],
[CACHE[]_present="$LDOWN[]_present_avoidcache"
if test x$CACHE[]_present = xyes -a x$LDOWN[]_test_enabled = xyes -a \
    x$LDOWN[]_version_test_enabled = xyes ; then
  m4_default([_AC_PATH_LIB_CHECK_TEST_COMPILE],
             [_AC_PATH_LIB_CHECK_TEST_COMPILE],
             [_AC_PATH_LIB_CHECK_TEST_COMPILE_DEFAULT])
else
  m4_default([_AC_PATH_LIB_CHECK_VERSION],
             [_AC_PATH_LIB_CHECK_VERSION],
             [_AC_PATH_LIB_CHECK_VERSION_DEFAULT])

  _AC_PATH_LIB_CHECK_VERSION([],$LDOWN[]_min_version)
# If the user allowed it, try linking with the library
  if test x$LDOWN[]_test_enabled = xyes ; then
    _AC_PATH_LIB_CHECK_LINK(, [
      CACHE[]_present="no"
      if test x$LDOWN[]_version_test_error = xyes ; then
        AC_PATH_LIB_ERROR
      fi
      AC_PATH_LIB_ERROR([The test program failed to compile or link.  See the file])
      AC_PATH_LIB_ERROR([config.log for the exact error that occured.  This usually])
      AC_PATH_LIB_ERROR([means USERFRIENDLY was not installed, was incorrectly installed])
      AC_PATH_LIB_ERROR([or that you have moved USERFRIENDLY since it was installed.  In])
      AC_PATH_LIB_ERROR([the latter case, you may want to edit the $LDOWN[]_config])
      AC_PATH_LIB_ERROR([script: $UP[]_CONFIG])
    ])
  fi
fi])
dnl end tests
])# _AC_PATH_LIB_CHECK_TESTS


# _AC_PATH_LIB_CHECK_TEST_COMPILE_DEFAULT
# ---------------------------------------
# Check if the installed USERFRIENDLY is sufficiently new. (Also sanity checks
# the results of DOWN-config to some extent.  The test program must
# compile, link and run sucessfully
AC_DEFUN([_AC_PATH_LIB_CHECK_TEST_COMPILE],
[m4_pushdef([RUNLOG], run.[]DOWN[]test)dnl
m4_pushdef([MAJOR], _AC_PATH_LIB_VERSION_PREFIX[]_AC_PATH_LIB_VERSION_MAJOR[]_AC_PATH_LIB_VERSION_SUFFIX)dnl
m4_pushdef([MINOR], _AC_PATH_LIB_VERSION_PREFIX[]_AC_PATH_LIB_VERSION_MINOR[]_AC_PATH_LIB_VERSION_SUFFIX)dnl
m4_pushdef([MICRO], _AC_PATH_LIB_VERSION_PREFIX[]_AC_PATH_LIB_VERSION_MICRO[]_AC_PATH_LIB_VERSION_SUFFIX)dnl
m4_pushdef([DEFMAJOR], _AC_PATH_LIB_DEFVERSION_PREFIX[]_AC_PATH_LIB_DEFVERSION_MAJOR[]_AC_PATH_LIB_DEFVERSION_SUFFIX)dnl
m4_pushdef([DEFMINOR], _AC_PATH_LIB_DEFVERSION_PREFIX[]_AC_PATH_LIB_DEFVERSION_MINOR[]_AC_PATH_LIB_DEFVERSION_SUFFIX)dnl
m4_pushdef([DEFMICRO], _AC_PATH_LIB_DEFVERSION_PREFIX[]_AC_PATH_LIB_DEFVERSION_MICRO[]_AC_PATH_LIB_DEFVERSION_SUFFIX)dnl
  ac_save_CFLAGS="$CFLAGS"
  ac_save_LIBS="$LIBS"
  CFLAGS="$CFLAGS $UP[]_CFLAGS"
  LIBS="$UP[]_LIBS $LIBS"
  rm -f RUNLOG
  AC_TRY_RUN([
#include <$]LDOWN[_header>
#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  int major, minor, micro;
  char *tmp_version;
  FILE *errorlog;

  if ((errorlog = fopen("]ERRORLOG[", "w")) == NULL) {
     exit(1);
   }

  system ("touch ]RUNLOG[");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = strdup("$]LDOWN[_min_version");
  if (!tmp_version) {
     exit(1);
   }
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     fprintf(errorlog, "*** %s: bad version string\n", "$]LDOWN[_min_version");
     exit(1);
   }

  if ((]MAJOR[ != $]LDOWN[_config_major_version) ||
      (]MINOR[ != $]LDOWN[_config_minor_version) ||
      (]MICRO[ != $]LDOWN[_config_micro_version))
    {
      fprintf(errorlog, "*** '$]UP[_CONFIG ]LIBCONFIG_VERSION[' returned %d.%d.%d, but \n", \
        $]LDOWN[_config_major_version, $]LDOWN[_config_minor_version, \
        $]LDOWN[_config_micro_version);
      fprintf(errorlog, "*** ]USERFRIENDLY[ (%d.%d.%d) was found!\n", \
        ]MAJOR[, ]MINOR[, ]MICRO[);
      fprintf(errorlog, "***\n");
      fprintf(errorlog, "*** If $]LDOWN[_config was correct, then it is best to remove\n");
      fprintf(errorlog, "*** the old version of ]USERFRIENDLY[.  You may also be able to\n");
      fprintf(errorlog, "*** fix the error by modifying your LD_LIBRARY_PATH enviroment\n");
      fprintf(errorlog, "*** variable, or by editing /etc/ld.so.conf.  Make sure you have\n");
      fprintf(errorlog, "*** run ldconfig if that is required on your system.\n");
      fprintf(errorlog, "*** If $]LDOWN[_config was wrong, set the environment\n");
      fprintf(errorlog, "*** variable ]UP[_CONFIG to point to the correct copy of\n");
      fprintf(errorlog, "*** $]LDOWN[_config, and remove the file config.cache\n");
      fprintf(errorlog, "*** before re-running configure.\n");
    }
#if defined (]DEFMAJOR[) && defined (]DEFMINOR[) && defined (]DEFMICRO[)
  else if ((]MAJOR[ != ]DEFMAJOR[) ||
           (]MINOR[ != ]DEFMINOR[) ||
           (]MICRO[ != ]DEFMICRO[))
    {
      fprintf(errorlog, "*** ]USERFRIENDLY[ header files (version %d.%d.%d) do not match\n",
             ]DEFMAJOR[, ]DEFMINOR[, ]DEFMICRO[);
      fprintf(errorlog, "*** library (version %d.%d.%d)\n",
             ]MAJOR[, ]MINOR[, ]MICRO[);
    }
#endif /* defined (]DEFMAJOR[) ... */
  else
    {
      if ((]MAJOR[ > major) ||
        ((]MAJOR[ == major) && (]MINOR[  > minor)) ||
        ((]MAJOR[ == major) && (]MINOR[ == minor) && (]MICRO[ >= micro)))
      {
        return 0;
       }
     else
      {
        fprintf(errorlog, "*** An old version of ]USERFRIENDLY[ (%d.%d.%d) was found.\n",
               ]MAJOR[, ]MINOR[, ]MICRO[);
        fprintf(errorlog, "*** You need a version of ]USERFRIENDLY[ newer than %d.%d.%d.\n",
               major, minor, micro);
        /*fprintf(errorlog, "*** The latest version of ]USERFRIENDLY[ is always available from ftp://ftp.my.site\n");*/
        fprintf(errorlog, "***\n");
        fprintf(errorlog, "*** If you have already installed a sufficiently new version, this\n");
        fprintf(errorlog, "*** error probably means that the wrong copy of the $]LDOWN[_config\n");
        fprintf(errorlog, "*** shell script is being found.  The easiest way to fix this is to\n");
        fprintf(errorlog, "*** remove the old version of ]USERFRIENDLY[, but you can also set the\n");
        fprintf(errorlog, "*** ]UP[_CONFIG environment variable to point to the correct\n");
        fprintf(errorlog, "*** copy of $]LDOWN[_config.  (In this case, you will have to\n");
        fprintf(errorlog, "*** modify your LD_LIBRARY_PATH environment variable, or edit\n");
        fprintf(errorlog, "*** /etc/ld.so.conf so that the correct libraries are found at\n");
        fprintf(errorlog, "*** run-time.)\n");
      }
    }
  return 1;
}
],, [CACHE[]_present="no"],
    [AC_PATH_LIB_ERROR([cross compiling; assumed OK.])])
  CFLAGS="$ac_save_CFLAGS"
  LIBS="$ac_save_LIBS"

if test -f RUNLOG ; then
  :
elif test x$LDOWN[]_version_test_enabled = xyes ; then
  AC_PATH_LIB_ERROR([Could not run USERFRIENDLY test program, checking why...])
  AC_PATH_LIB_ERROR
  _AC_PATH_LIB_CHECK_LINK(dnl
    [AC_PATH_LIB_ERROR([The test program compiled, but did not run.  This usually])
     AC_PATH_LIB_ERROR([means that the run-time linker is not finding USERFRIENDLY or finding])
     AC_PATH_LIB_ERROR([finding the wrong version of USERFRIENDLY.  If it is not finding])
     AC_PATH_LIB_ERROR([USERFRIENDLY, you will need to set your LD_LIBRARY_PATH environment])
     AC_PATH_LIB_ERROR([variable, or edit /etc/ld.so.conf to point to the installed])
     AC_PATH_LIB_ERROR([location.  Also, make sure you have run ldconfig if that is])
     AC_PATH_LIB_ERROR([required on your system.])
     AC_PATH_LIB_ERROR
     AC_PATH_LIB_ERROR([If you have an old version installed, it is best to remove])
     AC_PATH_LIB_ERROR([it, although you may also be able to get things to work by])
     AC_PATH_LIB_ERROR([modifying LD_LIBRARY_PATH])
    ],
    [AC_PATH_LIB_ERROR([The test program failed to compile or link.  See the file])
     AC_PATH_LIB_ERROR([config.log for the exact error that occured.  This usually])
     AC_PATH_LIB_ERROR([means USERFRIENDLY was incorrectly installed or that you have])
     AC_PATH_LIB_ERROR([moved USERFRIENDLY since it was installed.  In the latter case,])
     AC_PATH_LIB_ERROR([you may want to edit the $LDOWN[]_config script:])
     AC_PATH_LIB_ERROR([$UP[]_CONFIG])
    ])
fi
rm -f RUNLOG
m4_popdef([RUNLOG])dnl
m4_popdef([MAJOR])dnl
m4_popdef([MINOR])dnl
m4_popdef([MICRO])dnl
m4_popdef([DEFMAJOR])dnl
m4_popdef([DEFMINOR])dnl
m4_popdef([DEFMICRO])dnl
])# _AC_PATH_LIB_CHECK_TEST_COMPILE_DEFAULT


# _AC_PATH_LIB_CHECK_VERSION_DEFAULT
# ----------------------------------
# Check that the library version (config) is greater than or equal to
# the requested (minimum) version.
AC_DEFUN([_AC_PATH_LIB_CHECK_VERSION],
[m4_if([$2], , ,
       [if test x$LDOWN[]_present_avoidcache = xyes ; then
         if test \
             "$LDOWN[]_config_major_version" -lt "$LDOWN[]_min_major_version" -o \
             "$LDOWN[]_config_major_version" -eq "$LDOWN[]_min_major_version" -a \
             "$LDOWN[]_config_minor_version" -lt "$LDOWN[]_min_minor_version" -o \
             "$LDOWN[]_config_major_version" -eq "$LDOWN[]_min_major_version" -a \
             "$LDOWN[]_config_minor_version" -eq "$LDOWN[]_min_minor_version" -a \
             "$LDOWN[]_config_micro_version" -lt "$LDOWN[]_min_micro_version" ; then
           CACHE[]_present="no"
           LDOWN[]_version_test_error="yes"
         else
           CACHE[]_present="yes"
         fi
       fi])
])# _AC_PATH_LIB_CHECK_VERSION_DEFAULT


# _AC_PATH_LIB_CHECK_LINK_DEFAULT(SUCCESS, FAIL)
# ----------------------------------------------
# Check if the library will link successfully.  If specified, run
# SUCCESS or FAIL on success or failure
AC_DEFUN([_AC_PATH_LIB_CHECK_LINK],
[ac_save_CFLAGS="$CFLAGS"
ac_save_LIBS="$LIBS"
CFLAGS="$CFLAGS $UP[]_CFLAGS"
LIBS="$LIBS $UP[]_LIBS"
AC_TRY_LINK([ #include <stdio.h> ], ,
            [m4_if([$1], , :, [$1])],
            [m4_if([$2], , :, [$2])])
CFLAGS="$ac_save_CFLAGS"
LIBS="$ac_save_LIBS"
])# _AC_PATH_LIB_CHECK_LINK_DEFAULT




## ------------------- ##
## 4. Error reporting. ##
## ------------------- ##


# AC_PATH_LIB_ERROR(MESSAGE)
# --------------------------
# Print an error message, MESSAGE, to the error log.
AC_DEFUN([AC_PATH_LIB_ERROR],
[echo '*** m4_if([$1], , , [$1])' >>ERRORLOG])


# _AC_PATH_LIB_ERROR_DUMP
# -----------------------
# Print the error log (after main AC_CACHE_CHECK completes).
AC_DEFUN([_AC_PATH_LIB_ERROR_DUMP],
[if test -f ERRORLOG ; then
  cat ERRORLOG
  rm -f ERRORLOG
fi])




## ------------------ ##
## 5. Feature macros. ##
## ------------------ ##


# AC_PATH_LIB_PKGCONFIG
# ---------------------
# Enable pkgconfig support in libconfig script (default).
AC_DEFUN([AC_PATH_LIB_PKGCONFIG],
[m4_define([AC_PATH_LIB_USEPKGCONFIG], [true])
])dnl


# AC_PATH_LIB_LIBCONFIG
# ---------------------
# Disable pkgconfig support in libconfig script.
AC_DEFUN([AC_PATH_LIB_LIBCONFIG],
[m4_define([AC_PATH_LIB_USEPKGCONFIG], [false])
])dnl


# AC_PATH_LIB_REGISTER (MACRO, REPLACEMENT)
# -----------------------------------------
# Register a macro to replace the default checks.  Use the REPLACEMENT
# macro for the check macro MACRO.
#
# Possible MACROs are:
#   _AC_PATH_LIB_CHECK_COMPILE and
#   _AC_PATH_LIP_CHECK_VERSION
# You should make sure that replacement macros use the same arguments
# (if any), and do error logging in the same manner and behave in the
# same way as the original.

# Non-default header names may be specified, as well as version
# variable names in the library itself (needed for
# _AC_PATH_LIB_CHECK_COMPILE):
#   _PATH_LIB_HEADER
#   _AC_PATH_LIB_VERSION_PREFIX (default library_)
#   _AC_PATH_LIB_VERSION_MAJOR (default major)
#   _AC_PATH_LIB_VERSION_MINOR (default minor)
#   _AC_PATH_LIB_VERSION_MICRO (default micro)
#   _AC_PATH_LIB_VERSION_SUFFIX (default _version)
#   _AC_PATH_LIB_DEFVERSION_PREFIX (default LIBRARY_)
#   _AC_PATH_LIB_DEFVERSION_MAJOR (default MAJOR)
#   _AC_PATH_LIB_DEFVERSION_MINOR (default MINOR)
#   _AC_PATH_LIB_DEFVERSION_MICRO (default MICRO)
#   _AC_PATH_LIB_DEFVERSION_SUFFIX (default _VERSION)
# For example, library_major_version.
AC_DEFUN([AC_PATH_LIB_REGISTER],
[m4_define([$1], [$2])])


# AC_PATH_LIB_CHECK_REGISTER_DEFAULTS
# -----------------------------------
# Restore the default check macros.
AC_DEFUN([AC_PATH_LIB_CHECK_REGISTER_DEFAULTS],
[_AC_PATH_LIB_CHECK_REGISTER_DEFAULTS([_AC_PATH_LIB_CHECK_COMPILE],
                                       [_AC_PATH_LIP_CHECK_VERSION],
                                       [_PATH_LIB_HEADER],
                                       [_AC_PATH_LIB_VERSION_PREFIX],
                                       [_AC_PATH_LIB_VERSION_MAJOR],
                                       [_AC_PATH_LIB_VERSION_MINOR],
                                       [_AC_PATH_LIB_VERSION_MICRO],
                                       [_AC_PATH_LIB_VERSION_SUFFIX],
                                       [_AC_PATH_LIB_DEFVERSION_PREFIX],
                                       [_AC_PATH_LIB_DEFVERSION_MAJOR],
                                       [_AC_PATH_LIB_DEFVERSION_MINOR],
                                       [_AC_PATH_LIB_DEFVERSION_MICRO],
                                       [_AC_PATH_LIB_DEFVERSION_SUFFIX])
])# AC_PATH_LIB_CHECK_REGISTER_DEFAULTS


# _AC_PATH_LIB_CHECK_REGISTER_DEFAULTS(MACROs ...)
# ------------------------------------------------
# Undefine MACROs.
AC_DEFUN([AC_PATH_LIB_CHECK_REGISTER_DEFAULTS],
[m4_if([$1], , ,
       [m4_ifdef([$1],
                 [m4_undefine([$1])])
        AC_PATH_LIB_CHECK_REGISTER_DEFAULTS(m4_shift($@))
       ])
])

