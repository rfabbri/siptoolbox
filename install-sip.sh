#!/bin/sh
set -x
# 
# DESCRIPTION
#
#   This is a script to help automate massive installation of SIP.
#   It has been tested under Ubuntu 12.04 LTS and a few other distros.
#  
#   Things I'd like to see in the future are: 
#      * automatic downloading, compiling and installing of third party
#   libs that can make a difference, such as OpenCV Subversion (svn) and
#   leptonica. 
#
# PRE-REQUISITES
#
#   Right now the installation comes out of the box if one first perform the
#   (slightly overkill but convenient) steps in:
#  
#   http://wiki.nosdigitais.teia.org.br/Configuring_Ubuntu_for_Programming
#
#   If you choose not to perform the above steps, then you will have to install
#   a number of dependencies on a case-by-case basis, which can be annoying.
#  
# USAGE
#  
#   sh install-sip.sh
#
# AUTHOR
#   Ricardo Fabbri
#

git clone git://siptoolbox.git.sourceforge.net/gitroot/siptoolbox/animal 

git clone git://siptoolbox.git.sourceforge.net/gitroot/siptoolbox/siptoolbox 

cd animal 

./autogen.sh
./configure
make
sudo make install

sudo ldconfig

cd ../siptoolbox
./autogen.sh
./configure
make
sudo make install

make autoload
