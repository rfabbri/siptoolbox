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

manpages_xml  = angle.xml bwborder.xml bwdist.xml bwlabel.xml curvature.xml curvature2d.xml dilate.xml drawline.xml edge.xml edilate.xml erode.xml fftderiv.xml follow.xml fractal.xml getangle.xml gray_imread.xml gsm.xml gsm2d.xml hello_sip.xml hough.xml hsv2rgb.xml ihough.xml im2bw.xml im2gray.xml imconv.xml imcorrcoef.xml imfinfo.xml imnoise.xml imphase.xml improfile.xml imread.xml imroi.xml imshow.xml imvariance.xml imwrite.xml imxprofile.xml ind2rgb.xml interferometry_gui.xml iradon.xml minmax.xml mkfftfilter.xml mkfilter.xml mogrify.xml normal.xml ntsc2rgb.xml percol.xml perim.xml pifilter.xml poledetection.xml radon.xml rgb2hsv.xml rgb2ntsc.xml rgbplot.xml sip_colormap.xml sip_fftshift.xml sip_set_verbose.xml sip_setenv.xml skel.xml thin.xml unfollow.xml unwrapl.xml unwrapp.xml watershed.xml xgetpixel.xml xor.xml
manpages_cat  = angle.cat bwborder.cat bwdist.cat bwlabel.cat curvature.cat curvature2d.cat dilate.cat drawline.cat edge.cat edilate.cat erode.cat fftderiv.cat follow.cat fractal.cat getangle.cat gray_imread.cat gsm.cat gsm2d.cat hello_sip.cat hough.cat hsv2rgb.cat ihough.cat im2bw.cat im2gray.cat imconv.cat imcorrcoef.cat imfinfo.cat imnoise.cat imphase.cat improfile.cat imread.cat imroi.cat imshow.cat imvariance.cat imwrite.cat imxprofile.cat ind2rgb.cat interferometry_gui.cat iradon.cat minmax.cat mkfftfilter.cat mkfilter.cat mogrify.cat normal.cat ntsc2rgb.cat percol.cat perim.cat pifilter.cat poledetection.cat radon.cat rgb2hsv.cat rgb2ntsc.cat rgbplot.cat sip_colormap.cat sip_fftshift.cat sip_set_verbose.cat sip_setenv.cat skel.cat thin.cat unfollow.cat unwrapl.cat unwrapp.cat watershed.cat xgetpixel.cat xor.cat
manpages_html = angle.htm bwborder.htm bwdist.htm bwlabel.htm curvature.htm curvature2d.htm dilate.htm drawline.htm edge.htm edilate.htm erode.htm fftderiv.htm follow.htm fractal.htm getangle.htm gray_imread.htm gsm.htm gsm2d.htm hello_sip.htm hough.htm hsv2rgb.htm ihough.htm im2bw.htm im2gray.htm imconv.htm imcorrcoef.htm imfinfo.htm imnoise.htm imphase.htm improfile.htm imread.htm imroi.htm imshow.htm imvariance.htm imwrite.htm imxprofile.htm ind2rgb.htm interferometry_gui.htm iradon.htm minmax.htm mkfftfilter.htm mkfilter.htm mogrify.htm normal.htm ntsc2rgb.htm percol.htm perim.htm pifilter.htm poledetection.htm radon.htm rgb2hsv.htm rgb2ntsc.htm rgbplot.htm sip_colormap.htm sip_fftshift.htm sip_set_verbose.htm sip_setenv.htm skel.htm thin.htm unfollow.htm unwrapl.htm unwrapp.htm watershed.htm xgetpixel.htm xor.htm
SCIDIR=@SCIDIR@

helpdir=@TOOLBOXDIR@/man
help_DATA = $(manpages_html) whatis.htm 

EXTRA_DIST = $(manpages_xml) html-sip.xsl mansip.dtd Makefile.am.input

.xml.htm: $(manpages_xml)
	@echo "Creating html help files"
	@$(SCIDIR)/bin/scilab -nwni -nb -e "exec('$(top_srcdir)/config/sip_xmltohtml.sci');%helps=[PWD,'SIP';%helps];sip_xmltohtml(PWD,'SIP','html-sip.xsl');quit"

Makefile.am: Makefile.am.input
	$(top_srcdir)/config/subst_input_files.sh

clean-local:
	rm -f *.htm
