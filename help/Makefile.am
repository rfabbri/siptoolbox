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

helppages_xml = en_US/imshow.xml en_US/bwborder.xml en_US/curvature.xml en_US/edge.xml en_US/follow.xml en_US/getangle.xml en_US/gsm.xml en_US/hsv2rgb.xml en_US/im2bw.xml en_US/im2gray.xml en_US/imfinfo.xml en_US/imnoise.xml en_US/imread.xml en_US/imwrite.xml en_US/ind2rgb.xml en_US/mogrify.xml en_US/ntsc2rgb.xml en_US/rgb2hsv.xml en_US/rgb2ntsc.xml en_US/unfollow.xml en_US/xgetpixel.xml en_US/perim.xml en_US/bwdist.xml en_US/bwlabel.xml en_US/curvature2d.xml en_US/dilate.xml en_US/drawline.xml en_US/edilate.xml en_US/erode.xml en_US/fftderiv.xml en_US/fractal.xml en_US/gsm2d.xml en_US/hello_sip.xml en_US/hough.xml en_US/ihough.xml en_US/imconv.xml en_US/imcorrcoef.xml en_US/imphase.xml en_US/improfile.xml en_US/imroi.xml en_US/imvariance.xml en_US/imxprofile.xml en_US/interferometry_gui.xml en_US/iradon.xml en_US/minmax.xml en_US/mkfftfilter.xml en_US/mkfilter.xml en_US/normal.xml en_US/percol.xml en_US/pifilter.xml en_US/poledetection.xml en_US/radon.xml en_US/rgbplot.xml en_US/sip_colormap.xml en_US/sip_fftshift.xml en_US/sip_setenv.xml en_US/sip_set_verbose.xml en_US/skel.xml en_US/thin.xml en_US/unwrapl.xml en_US/unwrapp.xml en_US/watershed.xml en_US/xor.xml en_US/gray_imread.xml en_US/sip_rq.xml en_US/KRC_from_P.xml en_US/quaternion2rot_matrix.xml en_US/rot_matrix2quaternion.xml en_US/exp2rot_matrix.xml en_US/quaternion_matrix.xml en_US/quaternion_multiply.xml


helpdir=@TOOLBOXDIR@/jar
help_DATA = ../jar/scilab_en_US_help.jar

EXTRA_DIST = $(helppages_xml) en_US/build_help.sce buildhelp.sce 

all: $(help_DATA)

$(help_DATA): buildhelp.sce $(helppages_xml)
	$(SCIEXE) -nw -nb -f buildhelp.sce

clean:
	rm -rf en_US/scilab_en_US_help en_US/master_help.xml ../jar/scilab_en_US_help.jar
