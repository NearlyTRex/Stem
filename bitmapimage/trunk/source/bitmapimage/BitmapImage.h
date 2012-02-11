/*
  Copyright (c) 2012 Alex Diener
  
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

#ifndef __BITMAP_IMAGE_H__
#define __BITMAP_IMAGE_H__

#include "stemobject/StemObject.h"

typedef struct BitmapImage BitmapImage;

enum BitmapPixelFormat {
	BITMAP_PIXEL_FORMAT_RGBA_8888 = 0,
	BITMAP_PIXEL_FORMAT_RGB_888 = 1,
	BITMAP_PIXEL_FORMAT_GRAYALPHA_88 = 2,
	BITMAP_PIXEL_FORMAT_GRAY_8 = 3
};

#define BitmapImage_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	enum BitmapPixelFormat pixelFormat; \
	unsigned int width; \
	unsigned int height; \
	unsigned int bytesPerRow; \
	unsigned char * pixels;

stemobject_struct_definition(BitmapImage)

#define BitmapImage_pixelFormatBytes(pixelFormat) (4 - pixelFormat)

BitmapImage * BitmapImage_create(enum BitmapPixelFormat pixelFormat,
                                 unsigned int width,
                                 unsigned int height,
                                 unsigned int bytesPerRow,
                                 unsigned char * pixels);
void BitmapImage_init(BitmapImage * self,
                      enum BitmapPixelFormat pixelFormat,
                      unsigned int width,
                      unsigned int height,
                      unsigned int bytesPerRow,
                      unsigned char * pixels);
void BitmapImage_dispose(BitmapImage * self);

#endif
