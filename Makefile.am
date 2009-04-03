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

# Don't require all the GNU mandated files
AUTOMAKE_OPTIONS = 1.9.6 foreign
ACLOCAL_AMFLAGS = -I config
EXTRA_DIST = autogen.sh src/Makefile.am.input config/subst_input_files.sh \
             config/my_ilib_gen_gateway.sci config/my_ilib_gen_loader.sci \
             config/my_unix.sci config/libsip_build.sce  README.txt \
             config/loader.sce.in config/loader_include.sce \
				     config/builder.sce \
             config/my_unix.sci config/sip_xmltohtml.sci COPYING.txt \
				     TODO.txt THANKS.txt NEWS.txt AUTHORS.txt ChangeLog.txt \
				     DEVEL_README.txt FAQ.txt INSTALL.txt INSTALL_UNIX_DETAILED.txt \
				     INSTALL-MinGW.txt

SUBDIRS = src macros images @HELPDIR@

toolboxdir=@TOOLBOXDIR@
toolbox_DATA = config/loader.sce config/loader_include.sce config/builder.sce
