#include "unittest/framework/TestSuite.h"
#include "pngimageio/PNGImageIO.h"
#include "utilities/IOUtilities.h"
#include <stdio.h>

#define assertPixelMatch(expectedPixels, actualPixels, width, height, bytesPerPixel, bytesPerRow, rgbTolerance, testIndex) { \
	bool different = false; \
	\
	if ((rgbTolerance) > 0) { \
		unsigned int indexX, indexY, componentIndex; \
		\
		for (indexY = 0; indexY < (height); indexY++) { \
			for (indexX = 0; indexX < (width); indexX++) { \
				for (componentIndex = 0; componentIndex < ((bytesPerPixel) > 2 ? 3 : 1); componentIndex++) { \
					if (abs((expectedPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex] - (actualPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex]) > (rgbTolerance)) { \
						different = true; \
						break; \
					} \
				} \
				if (different) { \
					break; \
				} \
				if (componentIndex < (bytesPerPixel) && (expectedPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex] != (actualPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex]) { \
					different = true; \
					break; \
				} \
			} \
			if (different) { \
				break; \
			} \
		} \
	} else { \
		different = !!memcmp((expectedPixels), (actualPixels), (bytesPerRow) * (height)); \
	} \
	if (different) { \
		unsigned int indexX, indexY, componentIndex; \
		\
		fprintf(stderr, "  Expected image: {\n"); \
		for (indexY = 0; indexY < (height); indexY++) { \
			fprintf(stderr, "    "); \
			for (indexX = 0; indexX < (width); indexX++) { \
				fprintf(stderr, "%02X", (expectedPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel)]); \
				for (componentIndex = 1; componentIndex < (bytesPerPixel); componentIndex++) { \
					fprintf(stderr, "%02X", (expectedPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex]); \
				} \
				fputc(' ', stderr); \
			} \
			fputc('\n', stderr); \
		} \
		fprintf(stderr, "  }\nActual image: {\n"); \
		for (indexY = 0; indexY < (height); indexY++) { \
			fprintf(stderr, "    "); \
			for (indexX = 0; indexX < (width); indexX++) { \
				fprintf(stderr, "%02X", (actualPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel)]); \
				for (componentIndex = 1; componentIndex < (bytesPerPixel); componentIndex++) { \
					fprintf(stderr, "%02X", (actualPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex]); \
				} \
				fputc(' ', stderr); \
			} \
			fputc('\n', stderr); \
		} \
		fprintf(stderr, "  }\n"); \
		TestCase_assert(false, "Pixels didn't match; see above (test %u)", (unsigned int) (testIndex)); \
	} \
}

#define assertPixelMatchFlipped(expectedPixels, actualPixels, width, height, bytesPerPixel, bytesPerRow, rgbTolerance, testIndex) { \
	bool different = false; \
	\
	if ((rgbTolerance) > 0) { \
		unsigned int indexX, indexY, componentIndex; \
		\
		for (indexY = 0; indexY < (height); indexY++) { \
			for (indexX = 0; indexX < (width); indexX++) { \
				for (componentIndex = 0; componentIndex < ((bytesPerPixel) > 2 ? 3 : 1); componentIndex++) { \
					if (abs((expectedPixels)[((height) - 1 - indexY) * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex] - (actualPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex]) > (rgbTolerance)) { \
						different = true; \
						break; \
					} \
				} \
				if (different) { \
					break; \
				} \
				if (componentIndex < (bytesPerPixel) && (expectedPixels)[((height) - 1 - indexY) * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex] != (actualPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex]) { \
					different = true; \
					break; \
				} \
			} \
			if (different) { \
				break; \
			} \
		} \
	} else { \
		unsigned int rowIndex; \
		\
		for (rowIndex = 0; rowIndex < (height); rowIndex++) { \
			if (memcmp((expectedPixels) + ((height) - 1 - rowIndex) * (bytesPerRow), (actualPixels) + rowIndex * (bytesPerRow), (bytesPerRow))) { \
				different = true; \
				break; \
			} \
		} \
	} \
	if (different) { \
		unsigned int indexX, indexY, componentIndex; \
		\
		fprintf(stderr, "  Expected image: {\n"); \
		for (indexY = 0; indexY < (height); indexY++) { \
			fprintf(stderr, "    "); \
			for (indexX = 0; indexX < (width); indexX++) { \
				fprintf(stderr, "%02X", (expectedPixels)[((height) - 1 - indexY) * (bytesPerRow) + indexX * (bytesPerPixel)]); \
				for (componentIndex = 1; componentIndex < (bytesPerPixel); componentIndex++) { \
					fprintf(stderr, "%02X", (expectedPixels)[((height) - 1 - indexY) * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex]); \
				} \
				fputc(' ', stderr); \
			} \
			fputc('\n', stderr); \
		} \
		fprintf(stderr, "  }\nActual image: {\n"); \
		for (indexY = 0; indexY < (height); indexY++) { \
			fprintf(stderr, "    "); \
			for (indexX = 0; indexX < (width); indexX++) { \
				fprintf(stderr, "%02X", (actualPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel)]); \
				for (componentIndex = 1; componentIndex < (bytesPerPixel); componentIndex++) { \
					fprintf(stderr, "%02X", (actualPixels)[indexY * (bytesPerRow) + indexX * (bytesPerPixel) + componentIndex]); \
				} \
				fputc(' ', stderr); \
			} \
			fputc('\n', stderr); \
		} \
		fprintf(stderr, "  }\n"); \
		TestCase_assert(false, "Pixels didn't match; see above (test %u)", (unsigned int) (testIndex)); \
	} \
}

#define assertImageWellFormed(image, expectedPixelFormat, expectedWidth, expectedHeight, expectedBytesPerRow, expectedPixels, pixelCompareMacro, rgbTolerance, testIndex, imageLoadFunction) \
	TestCase_assert(image != NULL, "%s unexpectedly returned NULL (test %u)", #imageLoadFunction, (unsigned int) (testIndex)); \
	TestCase_assert(image->pixelFormat == expectedPixelFormat, "Expected pixelFormat %d but got %d (test %u)", expectedPixelFormat, image->pixelFormat, (unsigned int) (testIndex)); \
	TestCase_assert(image->width == expectedWidth, "Expected width %u but got %u (test %u)", expectedWidth, image->width, (unsigned int) (testIndex)); \
	TestCase_assert(image->height == expectedHeight, "Expected height %u but got %u (test %u)", expectedHeight, image->height, (unsigned int) (testIndex)); \
	TestCase_assert(image->bytesPerRow == expectedBytesPerRow, "Expected bytesPerRow %u but got %u (test %u)", expectedBytesPerRow, image->bytesPerRow, (unsigned int) (testIndex)); \
	TestCase_assert(image->pixels != NULL, "Expected non-NULL but got NULL (test %u)", (unsigned int) (testIndex)); \
	pixelCompareMacro(expectedPixels, image->pixels, expectedWidth, expectedHeight, BitmapImage_pixelFormatBytes(expectedPixelFormat), expectedBytesPerRow, rgbTolerance, testIndex);

static void testReadPNG() {
	BitmapImage * image;
	unsigned char expectedPixels2x2rgba[] = {
		0x00,0x00,0x00,0xFF, 0xFF,0xFF,0xFF,0xFF,
		0xFF,0x00,0x00,0xFF, 0x00,0x00,0xFF,0x7F
	};
	unsigned char expectedPixels2x2rgb[] = {
		0x00,0x00,0x00, 0xFF,0xFF,0xFF,
		0xFF,0x00,0x00, 0x00,0x00,0xFF
	};
	unsigned char expectedPixels2x2grayalpha[] = {
		0x00,0xFF, 0xFF,0xFF,
		0x55,0xFF, 0x55,0x7F
	};
	unsigned char expectedPixels2x2gray[] = {
		0x00, 0xFF,
		0x55, 0x55
	};
	unsigned char expectedPixels3x3rgba[] = {
		0xFF,0x00,0x00,0xFF, 0x00,0xFF,0x00,0xFF, 0x00,0x00,0xFF,0xFF,
		0xFF,0xFF,0x00,0xFF, 0xFF,0x00,0xFF,0xFF, 0x00,0xFF,0xFF,0xFF,
		0x00,0x00,0x00,0xFF, 0xFF,0xFF,0xFF,0xFF, 0x7F,0x7F,0x7F,0xFF
	};
	unsigned char expectedPixels3x3rgb[] = {
		0xFF,0x00,0x00, 0x00,0xFF,0x00, 0x00,0x00,0xFF,
		0xFF,0xFF,0x00, 0xFF,0x00,0xFF, 0x00,0xFF,0xFF,
		0x00,0x00,0x00, 0xFF,0xFF,0xFF, 0x7F,0x7F,0x7F
	};
	unsigned char expectedPixels3x3grayalpha[] = {
		0x55,0xFF, 0x55,0xFF, 0x55,0xFF,
		0xAA,0xFF, 0xAA,0xFF, 0xAA,0xFF,
		0x00,0xFF, 0xFF,0xFF, 0x7F,0xFF
	};
	unsigned char expectedPixels3x3gray[] = {
		0x55, 0x55, 0x55,
		0xAA, 0xAA, 0xAA,
		0x00, 0xFF, 0x7F
	};
	unsigned char expectedPixels2x3rgba[] = {
		0x00,0x00,0x00,0xFF, 0xFF,0xFF,0xFF,0xFF,
		0x00,0x00,0x00,0x7F, 0xFF,0xFF,0xFF,0x7F,
		0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0x00
	};
	unsigned char expectedPixels2x3rgb[] = {
		0x00,0x00,0x00, 0xFF,0xFF,0xFF,
		0x00,0x00,0x00, 0xFF,0xFF,0xFF,
		0x00,0x00,0x00, 0xFF,0xFF,0xFF
	};
	unsigned char expectedPixels2x3grayalpha[] = {
		0x00,0xFF, 0xFF,0xFF,
		0x00,0x7F, 0xFF,0x7F,
		0x00,0x00, 0xFF,0x00
	};
	unsigned char expectedPixels2x3gray[] = {
		0x00, 0xFF,
		0x00, 0xFF,
		0x00, 0xFF
	};
	unsigned char expectedPixels3x2rgba[] = {
		0x00,0x00,0x00,0xFF, 0x3F,0x3F,0x3F,0xFF, 0x7F,0x7F,0x7F,0xFF,
		0xBF,0xBF,0xBF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0xFF
	};
	unsigned char expectedPixels3x2rgb[] = {
		0x00,0x00,0x00, 0x3F,0x3F,0x3F, 0x7F,0x7F,0x7F,
		0xBF,0xBF,0xBF, 0xFF,0xFF,0xFF, 0x00,0x00,0x00
	};
	unsigned char expectedPixels3x2grayalpha[] = {
		0x00,0xFF, 0x3F,0xFF, 0x7F,0xFF,
		0xBF,0xFF, 0xFF,0xFF, 0x00,0xFF
	};
	unsigned char expectedPixels3x2gray[] = {
		0x00, 0x3F, 0x7F,
		0xBF, 0xFF, 0x00
	};
	unsigned char * expectedPixels[] = {
		expectedPixels2x2rgba,
		expectedPixels2x2rgb,
		expectedPixels2x2grayalpha,
		expectedPixels2x2gray,
		expectedPixels3x3rgba,
		expectedPixels3x3rgb,
		expectedPixels3x3grayalpha,
		expectedPixels3x3gray,
		expectedPixels2x3rgba,
		expectedPixels2x3rgb,
		expectedPixels2x3grayalpha,
		expectedPixels2x3gray,
		expectedPixels3x2rgba,
		expectedPixels3x2rgb,
		expectedPixels3x2grayalpha,
		expectedPixels3x2gray
	};
	char * referenceImageFiles[] = {
		"reference2x2rgba.png",
		"reference3x3rgb.png",
		"reference2x3grayalpha.png",
		"reference3x2gray.png"
	};
	unsigned int imageDimensions[][2] = {
		{2, 2},
		{3, 3},
		{2, 3},
		{3, 2}
	};
	int pixelFormats[] = {
		PNG_PIXEL_FORMAT_AUTOMATIC,
		BITMAP_PIXEL_FORMAT_RGBA_8888,
		BITMAP_PIXEL_FORMAT_RGB_888,
		BITMAP_PIXEL_FORMAT_GRAYALPHA_88,
		BITMAP_PIXEL_FORMAT_GRAY_8
	};
	unsigned int imageFileIndex, pixelFormatIndex;
	enum BitmapPixelFormat expectedPixelFormat;
	unsigned int expectedBytesPerRow;
	void * fileContents;
	size_t fileLength;
	int rgbTolerance;
	
	for (imageFileIndex = 0; imageFileIndex < sizeof(referenceImageFiles) / sizeof(char *); imageFileIndex++) {
		for (pixelFormatIndex = 0; pixelFormatIndex < sizeof(pixelFormats) / sizeof(int); pixelFormatIndex++) {
			expectedPixelFormat = pixelFormatIndex == 0 ? pixelFormats[imageFileIndex + 1] : pixelFormats[pixelFormatIndex];
			expectedBytesPerRow = imageDimensions[imageFileIndex][0] * BitmapImage_pixelFormatBytes(expectedPixelFormat);
			rgbTolerance = ((expectedPixelFormat == BITMAP_PIXEL_FORMAT_GRAYALPHA_88 || expectedPixelFormat == BITMAP_PIXEL_FORMAT_GRAY_8) && imageFileIndex < 2);
			
			image = PNGImageIO_loadPNGFile(referenceImageFiles[imageFileIndex], pixelFormats[pixelFormatIndex], false);
			assertImageWellFormed(image,
			                      expectedPixelFormat,
			                      imageDimensions[imageFileIndex][0],
			                      imageDimensions[imageFileIndex][1],
			                      expectedBytesPerRow,
			                      expectedPixels[imageFileIndex * 4 + expectedPixelFormat],
			                      assertPixelMatch,
			                      rgbTolerance,
			                      imageFileIndex * sizeof(pixelFormats) / sizeof(int) * 4 + pixelFormatIndex * 4,
			                      PNGImageIO_loadPNGFile);
			image->dispose(image);
			
			image = PNGImageIO_loadPNGFile(referenceImageFiles[imageFileIndex], pixelFormats[pixelFormatIndex], true);
			assertImageWellFormed(image,
			                      expectedPixelFormat,
			                      imageDimensions[imageFileIndex][0],
			                      imageDimensions[imageFileIndex][1],
			                      expectedBytesPerRow,
			                      expectedPixels[imageFileIndex * 4 + expectedPixelFormat],
			                      assertPixelMatchFlipped,
			                      rgbTolerance,
			                      imageFileIndex * sizeof(pixelFormats) / sizeof(int) * 4 + pixelFormatIndex * 4 + 1,
			                      PNGImageIO_loadPNGFile);
			image->dispose(image);
			
			fileContents = readFileSimple(referenceImageFiles[imageFileIndex], &fileLength);
			
			image = PNGImageIO_loadPNGData(fileContents, fileLength, pixelFormats[pixelFormatIndex], false);
			assertImageWellFormed(image,
			                      expectedPixelFormat,
			                      imageDimensions[imageFileIndex][0],
			                      imageDimensions[imageFileIndex][1],
			                      expectedBytesPerRow,
			                      expectedPixels[imageFileIndex * 4 + expectedPixelFormat],
			                      assertPixelMatch,
			                      rgbTolerance,
			                      imageFileIndex * sizeof(pixelFormats) / sizeof(int) * 4 + pixelFormatIndex * 4 + 2,
			                      PNGImageIO_loadPNGData);
			image->dispose(image);
			
			image = PNGImageIO_loadPNGData(fileContents, fileLength, pixelFormats[pixelFormatIndex], true);
			assertImageWellFormed(image,
			                      expectedPixelFormat,
			                      imageDimensions[imageFileIndex][0],
			                      imageDimensions[imageFileIndex][1],
			                      expectedBytesPerRow,
			                      expectedPixels[imageFileIndex * 4 + expectedPixelFormat],
			                      assertPixelMatchFlipped,
			                      rgbTolerance,
			                      imageFileIndex * sizeof(pixelFormats) / sizeof(int) * 4 + pixelFormatIndex * 4 + 3,
			                      PNGImageIO_loadPNGData);
			image->dispose(image);
			
			free(fileContents);
		}
	}
	
	image = PNGImageIO_loadPNGFile("This file does not exist.png", PNG_PIXEL_FORMAT_AUTOMATIC, false);
	TestCase_assert(image == NULL, "PNGImageIO_loadPNGFile unexpectedly returned non-NULL (%p)", image);
}

static void testWritePNG() {
	
}

TEST_SUITE(PNGImageIOTest, testReadPNG, testWritePNG)
