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

#include "pngimageio/PNGImageIO.h"
#include "utilities/IOUtilities.h"
#include <png.h>

BitmapImage * PNGImageIO_loadPNGFile(const char * filePath, int pixelFormat, bool flipVertical) {
	BitmapImage * image;
	void * fileContents;
	size_t length = 0;
	
	fileContents = readFileSimple(filePath, &length);
	image = PNGImageIO_loadPNGData(fileContents, length, pixelFormat, flipVertical);
	free(fileContents);
	
	return image;
}

static void pngReadFnMemread(png_structp pngReadStruct, png_bytep data, png_size_t length) {
	if (!memread((struct memreadContext *) png_get_io_ptr(pngReadStruct), length, data)) {
		png_error(pngReadStruct, "Failed to read bytes in pngReadFnMemread.");
	}
}

#define PNG_HEADER_SIZE 8

BitmapImage * PNGImageIO_loadPNGData(const void * data, size_t length, int pixelFormat, bool flipVertical) {
	png_byte headerBytes[PNG_HEADER_SIZE];
	png_structp pngReadStruct;
	png_infop pngInfoStruct;
	struct memreadContext readContext;
	unsigned int width, height;
	png_int_32 bitDepth, colorType;
	png_bytep * rows;
	unsigned char * pixels;
	unsigned int rowIndex;
	enum BitmapPixelFormat chosenPixelFormat = BITMAP_PIXEL_FORMAT_RGBA_8888;
	BitmapImage * image;
	
	readContext = memreadContextInit(data, length);
	if (!memread(&readContext, PNG_HEADER_SIZE, headerBytes) || png_sig_cmp(headerBytes, 0, PNG_HEADER_SIZE)) {
		return NULL;
	}
	
	pngReadStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	pngInfoStruct = png_create_info_struct(pngReadStruct);
	
	if (setjmp(png_jmpbuf(pngReadStruct))) {
		png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, NULL);
		return NULL;
	}
	
	png_set_read_fn(pngReadStruct, &readContext, pngReadFnMemread);
	png_set_sig_bytes(pngReadStruct, PNG_HEADER_SIZE);
	png_read_info(pngReadStruct, pngInfoStruct);
	
	width = png_get_image_width(pngReadStruct, pngInfoStruct);
	height = png_get_image_height(pngReadStruct, pngInfoStruct);
	bitDepth = png_get_bit_depth(pngReadStruct, pngInfoStruct);
	colorType = png_get_color_type(pngReadStruct, pngInfoStruct);
	
	if (colorType == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(pngReadStruct);
	}
	if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
		png_set_expand_gray_1_2_4_to_8(pngReadStruct);
	}
	if (png_get_valid(pngReadStruct, pngInfoStruct, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(pngReadStruct);
	}
	if (bitDepth == 16) {
		png_set_strip_16(pngReadStruct);
	}
	
	png_read_update_info(pngReadStruct, pngInfoStruct);
	colorType = png_get_color_type(pngReadStruct, pngInfoStruct);
	
	switch (pixelFormat) {
		case PNG_PIXEL_FORMAT_AUTOMATIC:
			switch (colorType) {
				case PNG_COLOR_TYPE_GRAY:
					chosenPixelFormat = BITMAP_PIXEL_FORMAT_GRAY_8;
					break;
					
				case PNG_COLOR_TYPE_GRAY_ALPHA:
					chosenPixelFormat = BITMAP_PIXEL_FORMAT_GRAYALPHA_88;
					break;
					
				case PNG_COLOR_TYPE_RGB:
					chosenPixelFormat = BITMAP_PIXEL_FORMAT_RGB_888;
					break;
					
				case PNG_COLOR_TYPE_RGB_ALPHA:
					chosenPixelFormat = BITMAP_PIXEL_FORMAT_RGBA_8888;
					break;
			}
			break;
			
		case BITMAP_PIXEL_FORMAT_RGBA_8888:
			if (!(colorType & PNG_COLOR_MASK_ALPHA)) {
				png_set_add_alpha(pngReadStruct, 0xFF, PNG_FILLER_AFTER);
			}
			if (!(colorType & PNG_COLOR_MASK_COLOR)) {
				png_set_gray_to_rgb(pngReadStruct);
			}
			chosenPixelFormat = BITMAP_PIXEL_FORMAT_RGBA_8888;
			break;
			
		case BITMAP_PIXEL_FORMAT_RGB_888:
			if (colorType & PNG_COLOR_MASK_ALPHA) {
				png_set_strip_alpha(pngReadStruct);
			}
			if (!(colorType & PNG_COLOR_MASK_COLOR)) {
				png_set_gray_to_rgb(pngReadStruct);
			}
			chosenPixelFormat = BITMAP_PIXEL_FORMAT_RGB_888;
			break;
			
		case BITMAP_PIXEL_FORMAT_GRAYALPHA_88:
			if (!(colorType & PNG_COLOR_MASK_ALPHA)) {
				png_set_add_alpha(pngReadStruct, 0xFF, PNG_FILLER_AFTER);
			}
			if (colorType & PNG_COLOR_MASK_COLOR) {
				png_set_rgb_to_gray(pngReadStruct, 1, (float) 0x55 / 0xFF, (float) 0x55 / 0xFF);
			}
			chosenPixelFormat = BITMAP_PIXEL_FORMAT_GRAYALPHA_88;
			break;
			
		case BITMAP_PIXEL_FORMAT_GRAY_8:
			if (colorType & PNG_COLOR_MASK_ALPHA) {
				png_set_strip_alpha(pngReadStruct);
			}
			if (colorType & PNG_COLOR_MASK_COLOR) {
				png_set_rgb_to_gray(pngReadStruct, 1, (float) 0x55 / 0xFF, (float) 0x55 / 0xFF);
			}
			chosenPixelFormat = BITMAP_PIXEL_FORMAT_GRAY_8;
			break;
	}
	
	pixels = malloc(width * height * BitmapImage_pixelFormatBytes(chosenPixelFormat));
	rows = malloc(height * sizeof(png_bytep));
	for (rowIndex = 0; rowIndex < height; rowIndex++) {
		rows[rowIndex] = pixels + ((flipVertical ? height - rowIndex - 1 : rowIndex) * width * BitmapImage_pixelFormatBytes(chosenPixelFormat));
	}
	
	png_read_image(pngReadStruct, rows);
	png_read_end(pngReadStruct, NULL);
	
	png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, NULL);
	free(rows);
	
	image = BitmapImage_create(chosenPixelFormat, width, height, width * BitmapImage_pixelFormatBytes(chosenPixelFormat), pixels);
	free(pixels);
	
	return image;
}

bool PNGImageIO_writePNGFile(BitmapImage * image, const char * filePath, int pixelFormat, bool flipVertical) {
	return false;
}

void * PNGImageIO_writePNGData(BitmapImage * image, int pixelFormat, bool flipVertical, size_t * outLength) {
	return NULL;
}
