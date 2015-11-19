= Overview =

This project serves as a starting point for all Stem library projects. It is meant to be copied and modified to suit the specific project being created. The centerpiece of this template is a robust, sophisticated makefile suitable for a wide range of build tasks and environments. Currently supported target platforms are Mac OS X, Windows, Linux, iPhone Simulator, and iPhone OS. Cross compilation is not supported except in the case of compiling for iPhone platforms from Mac OS X.


**Warning: File contents below are somewhat incomplete and out of date**


= Makefile Usage =

The makefile's default task will build all configurations of all targets for all available target platforms, run unit tests, and copy public-facing include files to the build/include directory. Make tasks are defined for each target in case you want to build one individually. Other tasks you may want to invoke:

	* clean: Delete all artifacts that were generated by the makefile.

	* analyze: Perform static analysis of source code in the project. You must have splint and clang installed to use this task; see the "Platform Details" section below for installation instructions on each platform. Analyzer output will be stored in build/analyzer-results, as well as printed for each file for which there are results.

		* You can also run each analyzer separately by invoking the analyze_splint and analyze_clang tasks. splint tends to produce significantly more output than clang, so it may be useful to only work with clang output until its analyzer warnings are resolved before moving on to splint.

	* install: Copy built libraries, executables, and public headers to STEM_SHARED_DIR in a format suitable for use by other Stem libraries.

	* install_<application_target>_iphonesimulator: Build <application_target> and deploy it to the iPhone Simulator. If the iPhone Simulator is running when this task is invoked, it will be killed. The simulator is launched when this task completes. Due to limitations of the iPhone SDK, this task is unable to launch the deployed application itself inside the simulator; you must start it manually from the simulator's Springboard.

	* install_<application_target>_iphonesimulator_ipad: Same as above, but launches the iPhone Simulator in iPad simulation mode.

	* codesign_<application_target>_iphoneos: Build <application_target> and sign it for deployment to a provisioned iPhone OS device. Due to limitations of the iPhone SDK, this task is unable to actually deploy the signed application to a connected device; you must deploy it manually by dragging the app bundle (in build/<application_target>/<configuration>-iphoneos) into the Xcode Organizer.



= Platform details =

Building with the stem build system on Mac OS X:

	* Has been tested under Mac OS X 10.6 with Xcode 3.2+ and iPhone SDK 3.1.2+

	* Requires Mac OS X 10.6 or later, Xcode tools 3.2 or later, and iPhone SDK 3.1 or later. If you change the macosx compiler from clang to gcc-4.2, you'll probably be able to build on 10.5.

	* In order to use the "analyze" make target, you must have splint and clang installed.

		* splint: Source code can be downloaded from splint.org and built in the standard UNIX way. Note that as of splint 3.1.2, I needed to change __pid_t to pid_t on line 519 of src/osd.c in order to build successfully on Mac OS X.

		* clang: The version of clang that comes with Mac OS X 10.6 developer tools is adequate.

Building with the stem build system on Windows:

	* Has been tested under Windows XP and Windows 7 with MinGW and MSYS 1.0.11

	* MinGW (http://mingw.org/), MinGW-w64 (http://mingw-w64.sourceforge.net/), and MSYS (http://mingw.org/wiki/MSYS) must be installed. Be sure to add C:\msys\bin and C:\MinGW\bin to your Path environment variable. If you install MinGW at a different location, you'll need to update the MINGW_W32_PATH and MINGW_W64_PATH makefile variables accordingly.

	* Call mingw32-make to use the makefile. MSYS make (as of version 1.0.11) will not work.

	* In order to use the "analyze" make target, you must have splint and clang installed.

		* splint: splint doesn't build from source on Windows without significant effort, but you can download a pre-built binary from http://splint.org/win32.html. Follow the instructions on that page to set up your installation.

		* clang: Checkout and build instructions can be found at http://clang.llvm.org/get_started. The "Unix-like" build instructions didn't work for me out of the box, but I was able to build successfully using MinGW by using the Visual Studio build instructions with some small modifications:

			* When invoking cmake, instead of passing -G "Visual Studio 9 2008", pass -G "MinGW Makefiles". cmake will barf if you do this in the same directory as your llvm checkout, so I created a directory next to it and ran cmake from there, passing the path to the llvm checkout as the last argument to cmake.
			
			* cmake will also barf on creating a MinGW makefile when C:/msys/1.0/bin is in your PATH, so you may need to temporarily remove it from your PATH before running cmake.
			
			* After cmake completes, run mingw32-make, and copy bin/clang.exe to C:/llvm/bin/clang.exe (or change CLANG_windows in the Makefile to the path to your clang build), and you're ready to go.
			
			* Note that clang invocations need extra include paths provided to them, since it doesn't have a standard location like /usr/include to search by default. This is done in the makefile with CLANGFLAGS_windows.

Building with the stem build system on Linux:

	* Has been tested under Ubuntu Linux 9.10+

	* Requires basic development tools (gcc, make, etc.) to be installed. More details hopefully forthcoming in a future version of this template; for now, you're on your own to figure out which packages to install.

	* In order to use the "analyze" make target, you must have splint and clang installed.

		* splint: Source code can be downloaded from splint.org and built in the standard UNIX way. You'll need the flex package installed to configure and build splint correctly. If your build fails to link due to being unable to find 'yywrap', sudo apt-get install flex

		* clang: Checkout and build instructions can be found at http://clang.llvm.org/get_started