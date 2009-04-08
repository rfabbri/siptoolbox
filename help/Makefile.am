##
## -------------------------------------------------------------------------
## SIP - Scilab Image Processing toolbox
## Copyright (C) 2009  Ricardo Fabbri
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
## -------------------------------------------------------------------------
##

helppages_xml = en_US/hsv2rgb.xml en_US/im2bw.xml en_US/imfinfo.xml en_US/imnoise.xml en_US/imread.xml en_US/imshow.xml en_US/imwrite.xml en_US/ind2rgb.xml en_US/mogrify.xml en_US/ntsc2rgb.xml en_US/rgb2hsv.xml en_US/follow.xml en_US/unfollow.xml en_US/gsm.xml


helpdir=@TOOLBOXDIR@/jar
help_DATA = ../jar/scilab_en_US_help.jar

EXTRA_DIST = $(helppages_xml) en_US/build_help.sce buildhelp.sce 

all: $(help_DATA)

$(help_DATA): buildhelp.sce $(helppages_xml)
	$(SCIEXE) -nw -nb -f buildhelp.sce

clean:
	rm -rf en_US/scilab_en_US_help en_US/master_help.xml ../jar/scilab_en_US_help.jar
