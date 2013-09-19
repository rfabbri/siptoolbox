## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright (C) 2003 Ricardo Fabbri
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

libdir=@TOOLBOXDIR@/lib
lib_LTLIBRARIES = libsip.la

AM_CPPFLAGS = @LIBRARY_EXTRA_CPPFLAGS@

#libsip_la_SOURCES = libsip.c sip_common.h imread.c imwrite.c imfinfo.c mogrify_int.c img_int.c analysis_int.c morphology_int.c hello_int.c segment_int.c sip_common.c misc_int.c imvariance_int.c unwrapp_int.c unwrapl_int.c canny_int.c deskew_int.c dewarp_int.c
libsip_la_SOURCES = libsip.c sip_common.h hello_int.c sip_common.c

libsip_la_LDFLAGS = -no-undefined -version-info 0:0:0 -release @PACKAGE_VERSION@

libsip_la_LIBADD = @LIBRARY_EXTRA_LIBS@
