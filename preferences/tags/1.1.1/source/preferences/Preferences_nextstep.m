/*
  Copyright (c) 2010 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener adiener@sacredsoftware.net
*/

#include "preferences/Preferences_private.h"
#import <Foundation/Foundation.h>

void Preferences_getFilePathPrivate(const char * fileName, char * outFilePath) {
	NSAutoreleasePool * pool;
	NSArray * paths;
	const char * libraryDir;
	
	pool = [[NSAutoreleasePool alloc] init];
	paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
	libraryDir = [[paths objectAtIndex: 0] UTF8String];
	snprintf(outFilePath, PATH_MAX, "%s/Preferences/%s", libraryDir, fileName);
	[pool release];
}
