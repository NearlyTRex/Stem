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

#ifndef __PNG_IMAGE_IO_H__
#define __PNG_IMAGE_IO_H__

#include "bitmapimage/BitmapImage.h"
#include <stdbool.h>
#include <stdlib.h>

#define PNG_PIXEL_FORMAT_AUTOMATIC -1

BitmapImage * PNGImageIO_loadPNGFile(const char * filePath, int pixelFormat, bool flipVertical);
BitmapImage * PNGImageIO_loadPNGData(const void * data, size_t length, int pixelFormat, bool flipVertical);
bool PNGImageIO_writePNGFile(BitmapImage * image, const char * filePath, int pixelFormat, bool flipVertical);
void * PNGImageIO_writePNGData(BitmapImage * image, int pixelFormat, bool flipVertical, size_t * outLength);

#endif
