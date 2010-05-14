This project serves as a starting point for all Stem library projects. It is meant to be copied and modified to suit the specific project being created.

Building with the stem build system on Mac OS X:
 * Requires Mac OS X 10.6 or later, Xcode tools 3.2 or later, and iPhone SDK 3.1 or later. If you change the macosx compiler from clang to gcc-4.2, you'll probably be able to build on 10.5.
 * Has only been tested under Mac OS X 10.6 with Xcode 3.2+ and iPhone SDK 3.1.2+

Building with the stem build system on Windows:
 * MinGW (http://mingw.org/), TDM-GCC (http://www.tdragon.net/recentgcc/), and MSYS (http://mingw.org/wiki/MSYS) must be installed. Be sure to add C:\msys\bin and C:\MinGW\bin to your Path environment variable. If you install MinGW at a different location, you'll need to update the CC_windows_i686, AR_windows, and RANLIB_windows makefile variables.
 * Call mingw32-make to use the makefile. MSYS make (as of version 1.0.11, at least) will not work.
 * Has only been tested under Windows XP with MinGW and MSYS 1.0.11

Building with the stem build system on Linux
 * Requires basic development tools (gcc, make, etc.) to be installed.
 * Has only been tested under Ubuntu Linux 9.10+