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

#include "bitmapimage/BitmapImage.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

BitmapImage * BitmapImage_create(enum BitmapPixelFormat pixelFormat,
                                 unsigned int width,
                                 unsigned int height,
                                 unsigned int bytesPerRow,
                                 unsigned char * pixels) {
	stemobject_create_implementation(BitmapImage, init, pixelFormat, width, height, bytesPerRow, pixels)
}

void BitmapImage_init(BitmapImage * self,
                      enum BitmapPixelFormat pixelFormat,
                      unsigned int width,
                      unsigned int height,
                      unsigned int bytesPerRow,
                      unsigned char * pixels) {
	unsigned int bytesPerPixel;
	
	call_super(init, self);
	
	self->pixelFormat = pixelFormat;
	self->width = width;
	self->height = height;
	self->bytesPerRow = bytesPerRow;
	
	bytesPerPixel = BitmapImage_pixelFormatBytes(pixelFormat);
	self->pixels = malloc(bytesPerRow * height * bytesPerPixel);
	memcpy(self->pixels, pixels, bytesPerRow * height);
	
	self->dispose = BitmapImage_dispose;
}

void BitmapImage_dispose(BitmapImage * self) {
	free(self->pixels);
	call_super(dispose, self);
}
