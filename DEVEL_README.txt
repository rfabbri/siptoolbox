SIP - The Scilab Image Processing Toolbox
=========================================

The development version is unstable and subject to fast changes
without concerning any compatibility with previous versions. Be
sure to check out the SIP development home page at:

         http://siptoolbox.sourceforge.net/devel

   - See the ChangeLog.txt file to see what's new

   - SIP uses GNU Libtool in order to build shared libraries on a
   variety of systems. While this is very nice for making usable
   binaries, it can be a pain when trying to debug a program. For
   that reason, compilation of shared libraries can be turned off
   by specifying the `--disable-shared' option to `configure'. This
   will make compilation much faster.

Making a Release
================

- update changelog
- bump version in configure.ac
- git tag your release
- push
- make dist in animal to generate .tar.gz with configure in it
- make dist in SIP
- sfupload archive.tar.gz  # for animal and sip
- sshfor # to create a shell
- cd /home/frs/project/animal/animal/
- mkdikr 0.16.0
- cd 0.16.0
- cp ~/uploads/animal-0.16.0.tar.gz .
- chmod a+r *gz
- similarly for SIP and for /home/frs/project/siptoolbox/animal/
- update website: link and news
- publish news on sourceforge
- mailing list announces to all SIP lists
- anounce in Scilab
- download and test the archives
- sfupload ChangeLog.txt  # this is automatically linked on the webpage
- mv ~/uploads/ChangeLog.txt  /home/frs/project/siptoolbox/SIP
