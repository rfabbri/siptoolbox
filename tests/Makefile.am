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

# Todo: 
# - need to set LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/scilab/ in order to
#   run sip_link_test. Perhaps a wrapper script to do this; scilab does the
#   same, see:
#   		http://wiki.scilab.org/Debugging%20and%20Profiling%20Scilab%205
LDADD = $(top_builddir)/src/libsip.la @SCILIBDIR@/libscilab.so

noinst_PROGRAMS = sip_link_test
sip_link_test_SOURCES = sip_link_test.c
