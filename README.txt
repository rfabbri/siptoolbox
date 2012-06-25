                  SIP - Scilab Image Processing toolbox 
                  =====================================

SIP development version
=======================

This toolbox reads and writes image files in almost ANY format,
including BMP, JPEG, GIF, PNG, TIFF, XPM, PCX, and more. There
are also various image processing routines for edge detection,
gaussian blurring, digital curvature estimation, etc.

WHAT's NEW? See the file ChangeLog.txt

INSTALLATION?  see the file INSTALL.txt. If you have any
problems with the installation process, feel free to subscribe to
the siptoolbox-users mailing list, at:
   http://sourceforge.net/mail/?group_id=68055

Further information about SIP can be found at
   http://siptoolbox.sourceforge.net

The SIP development source code and information are found at
   http://siptoolbox.sourceforge.net/devel

SIP is FREE software, and you are welcome to redistribute it under
the terms of the GPL, which can be found in the file COPYING.txt.


DEVELOPMENT
===========

Advanced users might want to read the DEVEL_README.txt file, if this
is the devel version, and also to take a look at the new AnImaL C
library at animal.sourceforge.net. We are open for any constructive
criticism, suggestions, and collaborations.


AUTHORS
=======

Principal author: Ricardo Fabbri <rfabbri@(not this part) gmail d0t com>    
Supervisor: Luciano da Fontoura Costa <luciano@if.sc.usp.br>

Cybernetic Vision Research Group <http://cyvision.if.sc.usp.br>

Contact Information:
                    IFSC/USP
                    Cybernetic Vision Research Group
                    Caixa Postal 369
                    13560-970 - Sao Carlos, SP
                    Brazil
                    
Phone: 273.9882  Area Code: 16  International Code: 55

Contents
========
README.txt                : this file
INSTALL.txt               : installation instructions
configure                 : configuration script (see INSTALL.txt)
install-sip.sh            : script to automate the steps in INSTALL.txt
Makefile                  : low-level build instructions (used by 'make')
Makefile.am               : high level build instructions (for automake)
*.txt                     : information about SIP
config                    : aux. config files and scripts for compiling/installing
    install-sip.sh        : 
src                       : folder for C routines
macros                    : folder for Scilab functions
     *.sci                : source versions
     *.bin                : compiled binary versions (generated)
     names                : table of functions (generated)
     lib                  : scilab library binary save (generated)
help                      : directory for help files
     Makefile             : for generating help files (html)
     en_US                : help files in US English (xml sources)
        scilab_en_US_help : html help files (open in any browser)
        images            : images/illustration for the help files
man                       : obsolete directory for help.
     Makefile             : for help formatting 
     *.man                : nroff help file
     *.cat                : formatted files 
     whatis               : short function description 
images                    : contains sample images used by examples and demos
extras                    : folder for


Please send any comments and suggestions to rfabbri@(not this part) gmail d0t com
kkkkkkkkkkkkkkkkkkkkkkk
