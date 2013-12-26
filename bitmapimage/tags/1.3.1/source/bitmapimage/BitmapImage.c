/*
  Copyright (c) 2013 Alex Diener
  
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
                                 unsigned int bytesPerRow) {
	stemobject_create_implementation(BitmapImage, init, pixelFormat, width, height, bytesPerRow)
}

BitmapImage * BitmapImage_createWithPixels(enum BitmapPixelFormat pixelFormat,
                                           unsigned int width,
                                           unsigned int height,
                                           unsigned int bytesPerRow,
                                           const unsigned char * pixels) {
	stemobject_create_implementation(BitmapImage, initWithPixels, pixelFormat, width, height, bytesPerRow, pixels)
}

BitmapImage * BitmapImage_createWithPixelsNoCopy(enum BitmapPixelFormat pixelFormat,
                                                 unsigned int width,
                                                 unsigned int height,
                                                 unsigned int bytesPerRow,
                                                 unsigned char * pixels,
                                                 bool takeOwnership) {
	stemobject_create_implementation(BitmapImage, initWithPixelsNoCopy, pixelFormat, width, height, bytesPerRow, pixels, takeOwnership)
}

static void sharedInit(BitmapImage * self, enum BitmapPixelFormat pixelFormat, unsigned int width, unsigned int height, unsigned int bytesPerRow) {
	call_super(init, self);
	
	self->pixelFormat = pixelFormat;
	self->width = width;
	self->height = height;
	self->bytesPerRow = bytesPerRow;
	
	self->dispose = BitmapImage_dispose;
}

bool BitmapImage_init(BitmapImage * self,
                      enum BitmapPixelFormat pixelFormat,
                      unsigned int width,
                      unsigned int height,
                      unsigned int bytesPerRow) {
	sharedInit(self, pixelFormat, width, height, bytesPerRow);
	
	self->pixels = malloc(width * height * BitmapImage_pixelFormatBytes(pixelFormat));
	self->private_ivar(freePixelsOnDispose) = false;
	return true;
}

bool BitmapImage_initWithPixels(BitmapImage * self,
                                enum BitmapPixelFormat pixelFormat,
                                unsigned int width,
                                unsigned int height,
                                unsigned int bytesPerRow,
                                const unsigned char * pixels) {
	unsigned int bytesPerPixel;
	
	sharedInit(self, pixelFormat, width, height, bytesPerRow);
	bytesPerPixel = BitmapImage_pixelFormatBytes(pixelFormat);
	self->pixels = malloc(bytesPerRow * height * bytesPerPixel);
	memcpy(self->pixels, pixels, bytesPerRow * height);
	self->private_ivar(freePixelsOnDispose) = true;
	return true;
}

bool BitmapImage_initWithPixelsNoCopy(BitmapImage * self,
                                      enum BitmapPixelFormat pixelFormat,
                                      unsigned int width,
                                      unsigned int height,
                                      unsigned int bytesPerRow,
                                      unsigned char * pixels,
                                      bool takeOwnership) {
	sharedInit(self, pixelFormat, width, height, bytesPerRow);
	
	self->pixels = pixels;
	self->private_ivar(freePixelsOnDispose) = takeOwnership;
	return true;
}

void BitmapImage_dispose(BitmapImage * self) {
	if (self->private_ivar(freePixelsOnDispose)) {
		free(self->pixels);
	}
	call_super(dispose, self);
}
