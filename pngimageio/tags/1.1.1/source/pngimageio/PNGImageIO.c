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
	png_bytep * rows = NULL;
	unsigned char * pixels = NULL;
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
		free(rows);
		free(pixels);
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
		colorType = PNG_COLOR_TYPE_RGB;
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
			png_read_update_info(pngReadStruct, pngInfoStruct);
			break;
			
		case BITMAP_PIXEL_FORMAT_RGBA_8888:
			if (!(colorType & PNG_COLOR_MASK_ALPHA)) {
				png_set_add_alpha(pngReadStruct, 0xFF, PNG_FILLER_AFTER);
			}
			png_read_update_info(pngReadStruct, pngInfoStruct);
			if (!(colorType & PNG_COLOR_MASK_COLOR)) {
				png_set_gray_to_rgb(pngReadStruct);
			}
			chosenPixelFormat = BITMAP_PIXEL_FORMAT_RGBA_8888;
			break;
			
		case BITMAP_PIXEL_FORMAT_RGB_888:
			if (colorType & PNG_COLOR_MASK_ALPHA) {
				png_set_strip_alpha(pngReadStruct);
			}
			png_read_update_info(pngReadStruct, pngInfoStruct);
			if (!(colorType & PNG_COLOR_MASK_COLOR)) {
				png_set_gray_to_rgb(pngReadStruct);
			}
			chosenPixelFormat = BITMAP_PIXEL_FORMAT_RGB_888;
			break;
			
		case BITMAP_PIXEL_FORMAT_GRAYALPHA_88:
			if (!(colorType & PNG_COLOR_MASK_ALPHA)) {
				png_set_add_alpha(pngReadStruct, 0xFF, PNG_FILLER_AFTER);
			}
			png_read_update_info(pngReadStruct, pngInfoStruct);
			if (colorType & PNG_COLOR_MASK_COLOR) {
				png_set_rgb_to_gray(pngReadStruct, 1, (float) 0x55 / 0xFF, (float) 0x55 / 0xFF);
			}
			chosenPixelFormat = BITMAP_PIXEL_FORMAT_GRAYALPHA_88;
			break;
			
		case BITMAP_PIXEL_FORMAT_GRAY_8:
			if (colorType & PNG_COLOR_MASK_ALPHA) {
				png_set_strip_alpha(pngReadStruct);
			}
			png_read_update_info(pngReadStruct, pngInfoStruct);
			if (colorType & PNG_COLOR_MASK_COLOR) {
				png_set_rgb_to_gray(pngReadStruct, 1, (float) 0x55 / 0xFF, (float) 0x55 / 0xFF);
			}
			chosenPixelFormat = BITMAP_PIXEL_FORMAT_GRAY_8;
			break;
	}
	
	pixels = calloc(width * height, BitmapImage_pixelFormatBytes(chosenPixelFormat));
	rows = malloc(sizeof(png_bytep) * height);
	for (rowIndex = 0; rowIndex < height; rowIndex++) {
		rows[rowIndex] = pixels + ((flipVertical ? height - rowIndex - 1 : rowIndex) * width * BitmapImage_pixelFormatBytes(chosenPixelFormat));
	}
	
	png_read_rows(pngReadStruct, rows, NULL, height);
	png_read_end(pngReadStruct, NULL);
	
	png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, NULL);
	free(rows);
	
	image = BitmapImage_create(chosenPixelFormat, width, height, width * BitmapImage_pixelFormatBytes(chosenPixelFormat), pixels);
	free(pixels);
	
	return image;
}

bool PNGImageIO_writePNGFile(BitmapImage * image, const char * filePath, int pixelFormat, bool flipVertical) {
	void * data;
	size_t dataLength;
	bool success;
	
	data = PNGImageIO_writePNGData(image, pixelFormat, flipVertical, &dataLength);
	if (data == NULL) {
		return false;
	}
	
	success = writeFileSimple(filePath, data, dataLength);
	free(data);
	
	return success;
}

static void pngWriteFnMemwrite(png_structp pngWriteStruct, png_bytep data, png_size_t length) {
	memwrite((struct memwriteContext *) png_get_io_ptr(pngWriteStruct), length, data);
}

static void pngFlushFnNoOp(png_structp pngWriteStruct) {
}

static void copyAlpha(unsigned char * targetPixels, unsigned char * sourcePixels, unsigned int width, unsigned int height, unsigned int targetBytesPerPixel, unsigned int sourceBytesPerPixel) {
	unsigned int rowIndex, columnIndex;
	
	for (rowIndex = 0; rowIndex < height; rowIndex++) {
		for (columnIndex = 0; columnIndex < width; columnIndex++) {
			targetPixels[rowIndex * width * targetBytesPerPixel + columnIndex * targetBytesPerPixel + targetBytesPerPixel - 1] = sourcePixels[rowIndex * width * sourceBytesPerPixel + columnIndex * sourceBytesPerPixel + sourceBytesPerPixel - 1];
		}
	}
}

static void addAlphaByte(unsigned char * pixels, unsigned int width, unsigned int height, unsigned int bytesPerPixel) {
	unsigned int rowIndex, columnIndex;
	
	for (rowIndex = 0; rowIndex < height; rowIndex++) {
		for (columnIndex = 0; columnIndex < width; columnIndex++) {
			pixels[rowIndex * width * bytesPerPixel + columnIndex * bytesPerPixel + bytesPerPixel - 1] = 0xFF;
		}
	}
}

static void copyGrayToRGB(unsigned char * targetPixels, unsigned char * sourcePixels, unsigned int width, unsigned int height, unsigned int targetBytesPerPixel, unsigned int sourceBytesPerPixel) {
	unsigned int rowIndex, columnIndex;
	unsigned char gray;
	
	for (rowIndex = 0; rowIndex < height; rowIndex++) {
		for (columnIndex = 0; columnIndex < width; columnIndex++) {
			gray = sourcePixels[rowIndex * width * sourceBytesPerPixel + columnIndex * sourceBytesPerPixel];
			targetPixels[rowIndex * width * targetBytesPerPixel + columnIndex * targetBytesPerPixel + 0] = gray;
			targetPixels[rowIndex * width * targetBytesPerPixel + columnIndex * targetBytesPerPixel + 1] = gray;
			targetPixels[rowIndex * width * targetBytesPerPixel + columnIndex * targetBytesPerPixel + 2] = gray;
		}
	}
}

static void copyRGB(unsigned char * targetPixels, unsigned char * sourcePixels, unsigned int width, unsigned int height, unsigned int targetBytesPerPixel, unsigned int sourceBytesPerPixel) {
	unsigned int rowIndex, columnIndex;
	
	for (rowIndex = 0; rowIndex < height; rowIndex++) {
		for (columnIndex = 0; columnIndex < width; columnIndex++) {
			targetPixels[rowIndex * width * targetBytesPerPixel + columnIndex * targetBytesPerPixel + 0] = sourcePixels[rowIndex * width * sourceBytesPerPixel + columnIndex * sourceBytesPerPixel + 0];
			targetPixels[rowIndex * width * targetBytesPerPixel + columnIndex * targetBytesPerPixel + 1] = sourcePixels[rowIndex * width * sourceBytesPerPixel + columnIndex * sourceBytesPerPixel + 1];
			targetPixels[rowIndex * width * targetBytesPerPixel + columnIndex * targetBytesPerPixel + 2] = sourcePixels[rowIndex * width * sourceBytesPerPixel + columnIndex * sourceBytesPerPixel + 2];
		}
	}
}

static void copyGray(unsigned char * targetPixels, unsigned char * sourcePixels, unsigned int width, unsigned int height, unsigned int targetBytesPerPixel, unsigned int sourceBytesPerPixel) {
	unsigned int rowIndex, columnIndex;
	
	for (rowIndex = 0; rowIndex < height; rowIndex++) {
		for (columnIndex = 0; columnIndex < width; columnIndex++) {
			targetPixels[rowIndex * width * targetBytesPerPixel + columnIndex * targetBytesPerPixel] = sourcePixels[rowIndex * width * sourceBytesPerPixel + columnIndex * sourceBytesPerPixel];
		}
	}
}

static void copyRGBToGray(unsigned char * targetPixels, unsigned char * sourcePixels, unsigned int width, unsigned int height, unsigned int targetBytesPerPixel, unsigned int sourceBytesPerPixel) {
	unsigned int rowIndex, columnIndex;
	int gray;
	
	for (rowIndex = 0; rowIndex < height; rowIndex++) {
		for (columnIndex = 0; columnIndex < width; columnIndex++) {
			gray = sourcePixels[rowIndex * width * sourceBytesPerPixel + columnIndex * sourceBytesPerPixel + 0] +
			       sourcePixels[rowIndex * width * sourceBytesPerPixel + columnIndex * sourceBytesPerPixel + 1] +
			       sourcePixels[rowIndex * width * sourceBytesPerPixel + columnIndex * sourceBytesPerPixel + 2];
			targetPixels[rowIndex * width * targetBytesPerPixel + columnIndex * targetBytesPerPixel] = gray / 3;
		}
	}
}

void * PNGImageIO_writePNGData(BitmapImage * image, int pixelFormat, bool flipVertical, size_t * outLength) {
	png_structp pngWriteStruct;
	png_infop pngInfoStruct;
	png_byte ** rows = NULL;
	struct memwriteContext writeContext;
	int colorType;
	enum BitmapPixelFormat outputPixelFormat;
	unsigned char * transformedPixels = NULL, * pixels;
	unsigned int rowIndex;
	
	pngWriteStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	pngInfoStruct = png_create_info_struct(pngWriteStruct);
	
	if (setjmp(png_jmpbuf(pngWriteStruct))) {
		png_destroy_write_struct(&pngWriteStruct, &pngInfoStruct);
		free(rows);
		return NULL;
	}
	
	switch (image->pixelFormat) {
		case BITMAP_PIXEL_FORMAT_RGBA_8888:
			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
			break;
			
		case BITMAP_PIXEL_FORMAT_RGB_888:
			colorType = PNG_COLOR_TYPE_RGB;
			break;
			
		case BITMAP_PIXEL_FORMAT_GRAYALPHA_88:
			colorType = PNG_COLOR_TYPE_GRAY_ALPHA;
			break;
			
		case BITMAP_PIXEL_FORMAT_GRAY_8:
			colorType = PNG_COLOR_TYPE_GRAY;
			break;
			
		default:
			png_destroy_write_struct(&pngWriteStruct, &pngInfoStruct);
			return NULL;
	}
	
	switch (pixelFormat) {
		case PNG_PIXEL_FORMAT_AUTOMATIC:
			outputPixelFormat = image->pixelFormat;
			break;
			
		case BITMAP_PIXEL_FORMAT_RGBA_8888:
			if ((int) image->pixelFormat != pixelFormat) {
				transformedPixels = malloc(image->width * image->height * 4);
				if (colorType & PNG_COLOR_MASK_ALPHA) {
					copyAlpha(transformedPixels, image->pixels, image->width, image->height, 4, BitmapImage_pixelFormatBytes(image->pixelFormat));
				} else {
					addAlphaByte(transformedPixels, image->width, image->height, 4);
				}
				if (colorType & PNG_COLOR_MASK_COLOR) {
					copyRGB(transformedPixels, image->pixels, image->width, image->height, 4, BitmapImage_pixelFormatBytes(image->pixelFormat));
				} else {
					copyGrayToRGB(transformedPixels, image->pixels, image->width, image->height, 4, BitmapImage_pixelFormatBytes(image->pixelFormat));
				}
			}
			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
			outputPixelFormat = BITMAP_PIXEL_FORMAT_RGBA_8888;
			break;
			
		case BITMAP_PIXEL_FORMAT_RGB_888:
			if ((int) image->pixelFormat != pixelFormat) {
				transformedPixels = malloc(image->width * image->height * 3);
				if (colorType & PNG_COLOR_MASK_COLOR) {
					copyRGB(transformedPixels, image->pixels, image->width, image->height, 3, BitmapImage_pixelFormatBytes(image->pixelFormat));
				} else {
					copyGrayToRGB(transformedPixels, image->pixels, image->width, image->height, 3, BitmapImage_pixelFormatBytes(image->pixelFormat));
				}
			}
			colorType = PNG_COLOR_TYPE_RGB;
			outputPixelFormat = BITMAP_PIXEL_FORMAT_RGB_888;
			break;
			
		case BITMAP_PIXEL_FORMAT_GRAYALPHA_88:
			if ((int) image->pixelFormat != pixelFormat) {
				transformedPixels = malloc(image->width * image->height * 2);
				if (colorType & PNG_COLOR_MASK_ALPHA) {
					copyAlpha(transformedPixels, image->pixels, image->width, image->height, 2, BitmapImage_pixelFormatBytes(image->pixelFormat));
				} else {
					addAlphaByte(transformedPixels, image->width, image->height, 2);
				}
				if (colorType & PNG_COLOR_MASK_COLOR) {
					copyRGBToGray(transformedPixels, image->pixels, image->width, image->height, 2, BitmapImage_pixelFormatBytes(image->pixelFormat));
				} else {
					copyGray(transformedPixels, image->pixels, image->width, image->height, 2, BitmapImage_pixelFormatBytes(image->pixelFormat));
				}
			}
			colorType = PNG_COLOR_TYPE_GRAY_ALPHA;
			outputPixelFormat = BITMAP_PIXEL_FORMAT_GRAYALPHA_88;
			break;
			
		case BITMAP_PIXEL_FORMAT_GRAY_8:
			if ((int) image->pixelFormat != pixelFormat) {
				transformedPixels = malloc(image->width * image->height);
				if (colorType & PNG_COLOR_MASK_COLOR) {
					copyRGBToGray(transformedPixels, image->pixels, image->width, image->height, 1, BitmapImage_pixelFormatBytes(image->pixelFormat));
				} else {
					copyGray(transformedPixels, image->pixels, image->width, image->height, 1, BitmapImage_pixelFormatBytes(image->pixelFormat));
				}
			}
			colorType = PNG_COLOR_TYPE_GRAY;
			outputPixelFormat = BITMAP_PIXEL_FORMAT_GRAY_8;
			break;
			
		default:
			png_destroy_write_struct(&pngWriteStruct, &pngInfoStruct);
			return NULL;
	}
	
	writeContext = memwriteContextInit(malloc(1), 0, 1, true);
	png_set_write_fn(pngWriteStruct, &writeContext, pngWriteFnMemwrite, pngFlushFnNoOp);
	
	png_set_IHDR(pngWriteStruct, pngInfoStruct, image->width, image->height, 8, colorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	
	rows = malloc(sizeof(png_byte *) * image->height);
	pixels = transformedPixels == NULL ? image->pixels : transformedPixels;
	for (rowIndex = 0; rowIndex < image->height; rowIndex++) {
		rows[rowIndex] = pixels + ((flipVertical ? image->height - rowIndex - 1 : rowIndex) * image->width * BitmapImage_pixelFormatBytes(outputPixelFormat));
	}
	
	png_set_rows(pngWriteStruct, pngInfoStruct, rows);
	png_write_png(pngWriteStruct, pngInfoStruct, PNG_TRANSFORM_IDENTITY, NULL);
	
	free(rows);
	free(transformedPixels);
	png_destroy_write_struct(&pngWriteStruct, &pngInfoStruct);
	
	*outLength = writeContext.position;
	return writeContext.data;
}
