#include "unittest/framework/TestSuite.h"
#include "bitmapimage/BitmapImage.h"
#include <string.h>

static void testInit() {
	BitmapImage * bitmapImagePtr, bitmapImage;
	unsigned char pixelsGray2x2r3[] = {
		0xFF, 0xBF, 0x00,
		0x7F, 0x3F, 0x00
	};
	unsigned char pixelsGrayAlpha2x2r4[] = {
		0xFF,0xFF, 0xBF,0x7F,
		0x7F,0x00, 0x3F,0xFF
	};
	unsigned char pixelsRGB3x2r9[] = {
		0xFF,0x00,0x00, 0x00,0xFF,0x00, 0x00,0x00,0xFF,
		0xFF,0xFF,0x00, 0xFF,0x00,0xFF, 0x00,0xFF,0xFF
	};
	unsigned char pixelsRGBA2x3r8[] = {
		0xFF,0x00,0x00,0x7F, 0x00,0xFF,0x00,0xBF,
		0x00,0x00,0x00,0xFF, 0xFF,0xFF,0xFF,0x00,
		0xFF,0xFF,0x00,0xFF, 0x00,0x00,0xFF,0x3F
	};
	
	BitmapImage_init(&bitmapImage, BITMAP_PIXEL_FORMAT_GRAY_8, 2, 2, 3, pixelsGray2x2r3);
	TestCase_assert(bitmapImage.pixelFormat == BITMAP_PIXEL_FORMAT_GRAY_8, "Expected %d but got %d", BITMAP_PIXEL_FORMAT_GRAY_8, bitmapImage.pixelFormat);
	TestCase_assert(bitmapImage.width == 2, "Expected 2 but got %u", bitmapImage.width);
	TestCase_assert(bitmapImage.height == 2, "Expected 2 but got %u", bitmapImage.height);
	TestCase_assert(bitmapImage.bytesPerRow == 3, "Expected 3 but got %u", bitmapImage.bytesPerRow);
	TestCase_assert(bitmapImage.pixels != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!memcmp(pixelsGray2x2r3, bitmapImage.pixels, 6), "Expected image:\n  %02X %02X %02X\n  %02X %02X %02X\nActual image:\n  %02X %02X %02X\n  %02X %02X %02X", pixelsGray2x2r3[0], pixelsGray2x2r3[1], pixelsGray2x2r3[2], pixelsGray2x2r3[3], pixelsGray2x2r3[4], pixelsGray2x2r3[5], bitmapImage.pixels[0], bitmapImage.pixels[1], bitmapImage.pixels[2], bitmapImage.pixels[3], bitmapImage.pixels[4], bitmapImage.pixels[5]);
	bitmapImage.dispose(&bitmapImage);
	
	BitmapImage_init(&bitmapImage, BITMAP_PIXEL_FORMAT_GRAYALPHA_88, 2, 2, 4, pixelsGrayAlpha2x2r4);
	TestCase_assert(bitmapImage.pixelFormat == BITMAP_PIXEL_FORMAT_GRAYALPHA_88, "Expected %d but got %d", BITMAP_PIXEL_FORMAT_GRAYALPHA_88, bitmapImage.pixelFormat);
	TestCase_assert(bitmapImage.width == 2, "Expected 2 but got %u", bitmapImage.width);
	TestCase_assert(bitmapImage.height == 2, "Expected 2 but got %u", bitmapImage.height);
	TestCase_assert(bitmapImage.bytesPerRow == 4, "Expected 3 but got %u", bitmapImage.bytesPerRow);
	TestCase_assert(bitmapImage.pixels != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!memcmp(pixelsGrayAlpha2x2r4, bitmapImage.pixels, 8), "Expected image:\n  %02X%02X %02X%02X\n  %02X%02X %02X%02X\nActual image:\n  %02X%02X %02X%02X\n  %02X%02X %02X%02X", pixelsGrayAlpha2x2r4[0], pixelsGrayAlpha2x2r4[1], pixelsGrayAlpha2x2r4[2], pixelsGrayAlpha2x2r4[3], pixelsGrayAlpha2x2r4[4], pixelsGrayAlpha2x2r4[5], pixelsGrayAlpha2x2r4[6], pixelsGrayAlpha2x2r4[7], bitmapImage.pixels[0], bitmapImage.pixels[1], bitmapImage.pixels[2], bitmapImage.pixels[3], bitmapImage.pixels[4], bitmapImage.pixels[5], bitmapImage.pixels[6], bitmapImage.pixels[7]);
	bitmapImage.dispose(&bitmapImage);
	
	BitmapImage_init(&bitmapImage, BITMAP_PIXEL_FORMAT_RGB_888, 3, 2, 9, pixelsRGB3x2r9);
	TestCase_assert(bitmapImage.pixelFormat == BITMAP_PIXEL_FORMAT_RGB_888, "Expected %d but got %d", BITMAP_PIXEL_FORMAT_RGB_888, bitmapImage.pixelFormat);
	TestCase_assert(bitmapImage.width == 3, "Expected 3 but got %u", bitmapImage.width);
	TestCase_assert(bitmapImage.height == 2, "Expected 2 but got %u", bitmapImage.height);
	TestCase_assert(bitmapImage.bytesPerRow == 9, "Expected 9 but got %u", bitmapImage.bytesPerRow);
	TestCase_assert(bitmapImage.pixels != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!memcmp(pixelsRGB3x2r9, bitmapImage.pixels, 18), "Expected image:\n  %02X%02X%02X %02X%02X%02X %02X%02X%02X\n  %02X%02X%02X %02X%02X%02X %02X%02X%02X\nActual image:\n  %02X%02X%02X %02X%02X%02X %02X%02X%02X\n  %02X%02X%02X %02X%02X%02X %02X%02X%02X", pixelsRGB3x2r9[0], pixelsRGB3x2r9[1], pixelsRGB3x2r9[2], pixelsRGB3x2r9[3], pixelsRGB3x2r9[4], pixelsRGB3x2r9[5], pixelsRGB3x2r9[6], pixelsRGB3x2r9[7], pixelsRGB3x2r9[8], pixelsRGB3x2r9[9], pixelsRGB3x2r9[10], pixelsRGB3x2r9[11], pixelsRGB3x2r9[12], pixelsRGB3x2r9[13], pixelsRGB3x2r9[14], pixelsRGB3x2r9[15], pixelsRGB3x2r9[16], pixelsRGB3x2r9[17], bitmapImage.pixels[0], bitmapImage.pixels[1], bitmapImage.pixels[2], bitmapImage.pixels[3], bitmapImage.pixels[4], bitmapImage.pixels[5], bitmapImage.pixels[6], bitmapImage.pixels[7], bitmapImage.pixels[8], bitmapImage.pixels[9], bitmapImage.pixels[10], bitmapImage.pixels[11], bitmapImage.pixels[12], bitmapImage.pixels[13], bitmapImage.pixels[14], bitmapImage.pixels[15], bitmapImage.pixels[16], bitmapImage.pixels[17]);
	bitmapImage.dispose(&bitmapImage);
	
	BitmapImage_init(&bitmapImage, BITMAP_PIXEL_FORMAT_RGBA_8888, 2, 3, 8, pixelsRGBA2x3r8);
	TestCase_assert(bitmapImage.pixelFormat == BITMAP_PIXEL_FORMAT_RGBA_8888, "Expected %d but got %d", BITMAP_PIXEL_FORMAT_RGBA_8888, bitmapImage.pixelFormat);
	TestCase_assert(bitmapImage.width == 2, "Expected 2 but got %u", bitmapImage.width);
	TestCase_assert(bitmapImage.height == 3, "Expected 3 but got %u", bitmapImage.height);
	TestCase_assert(bitmapImage.bytesPerRow == 8, "Expected 8 but got %u", bitmapImage.bytesPerRow);
	TestCase_assert(bitmapImage.pixels != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!memcmp(pixelsRGBA2x3r8, bitmapImage.pixels, 24), "Expected image:\n  %02X%02X%02X%02X %02X%02X%02X%02X\n  %02X%02X%02X%02X %02X%02X%02X%02X\n  %02X%02X%02X%02X %02X%02X%02X%02X\nActual image:\n  %02X%02X%02X%02X %02X%02X%02X%02X\n  %02X%02X%02X%02X %02X%02X%02X%02X\n  %02X%02X%02X%02X %02X%02X%02X%02X", pixelsRGBA2x3r8[0], pixelsRGBA2x3r8[1], pixelsRGBA2x3r8[2], pixelsRGBA2x3r8[3], pixelsRGBA2x3r8[4], pixelsRGBA2x3r8[5], pixelsRGBA2x3r8[6], pixelsRGBA2x3r8[7], pixelsRGBA2x3r8[8], pixelsRGBA2x3r8[9], pixelsRGBA2x3r8[10], pixelsRGBA2x3r8[11], pixelsRGBA2x3r8[12], pixelsRGBA2x3r8[13], pixelsRGBA2x3r8[14], pixelsRGBA2x3r8[15], pixelsRGBA2x3r8[16], pixelsRGBA2x3r8[17], pixelsRGBA2x3r8[18], pixelsRGBA2x3r8[19], pixelsRGBA2x3r8[20], pixelsRGBA2x3r8[21], pixelsRGBA2x3r8[22], pixelsRGBA2x3r8[23], bitmapImage.pixels[0], bitmapImage.pixels[1], bitmapImage.pixels[2], bitmapImage.pixels[3], bitmapImage.pixels[4], bitmapImage.pixels[5], bitmapImage.pixels[6], bitmapImage.pixels[7], bitmapImage.pixels[8], bitmapImage.pixels[9], bitmapImage.pixels[10], bitmapImage.pixels[11], bitmapImage.pixels[12], bitmapImage.pixels[13], bitmapImage.pixels[14], bitmapImage.pixels[15], bitmapImage.pixels[16], bitmapImage.pixels[17], bitmapImage.pixels[18], bitmapImage.pixels[19], bitmapImage.pixels[20], bitmapImage.pixels[21], bitmapImage.pixels[22], bitmapImage.pixels[23]);
	bitmapImage.dispose(&bitmapImage);
	
	bitmapImagePtr = BitmapImage_create(BITMAP_PIXEL_FORMAT_GRAY_8, 2, 2, 3, pixelsGray2x2r3);
	TestCase_assert(bitmapImagePtr != NULL, "BitmapImage_create unexpectedly returned NULL");
	if (bitmapImagePtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(bitmapImagePtr->pixelFormat == BITMAP_PIXEL_FORMAT_GRAY_8, "Expected %d but got %d", BITMAP_PIXEL_FORMAT_GRAY_8, bitmapImagePtr->pixelFormat);
	TestCase_assert(bitmapImagePtr->width == 2, "Expected 2 but got %u", bitmapImagePtr->width);
	TestCase_assert(bitmapImagePtr->height == 2, "Expected 2 but got %u", bitmapImagePtr->height);
	TestCase_assert(bitmapImagePtr->bytesPerRow == 3, "Expected 3 but got %u", bitmapImagePtr->bytesPerRow);
	TestCase_assert(bitmapImagePtr->pixels != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!memcmp(pixelsGray2x2r3, bitmapImagePtr->pixels, 6), "Expected image:\n  %02X %02X %02X\n  %02X %02X %02X\nActual image:\n  %02X %02X %02X\n  %02X %02X %02X", pixelsGray2x2r3[0], pixelsGray2x2r3[1], pixelsGray2x2r3[2], pixelsGray2x2r3[3], pixelsGray2x2r3[4], pixelsGray2x2r3[5], bitmapImagePtr->pixels[0], bitmapImagePtr->pixels[1], bitmapImagePtr->pixels[2], bitmapImagePtr->pixels[3], bitmapImagePtr->pixels[4], bitmapImagePtr->pixels[5]);
	bitmapImagePtr->dispose(bitmapImagePtr);
	
	bitmapImagePtr = BitmapImage_create(BITMAP_PIXEL_FORMAT_GRAYALPHA_88, 2, 2, 4, pixelsGrayAlpha2x2r4);
	TestCase_assert(bitmapImagePtr != NULL, "BitmapImage_create unexpectedly returned NULL");
	if (bitmapImagePtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(bitmapImagePtr->pixelFormat == BITMAP_PIXEL_FORMAT_GRAYALPHA_88, "Expected %d but got %d", BITMAP_PIXEL_FORMAT_GRAYALPHA_88, bitmapImagePtr->pixelFormat);
	TestCase_assert(bitmapImagePtr->width == 2, "Expected 2 but got %u", bitmapImagePtr->width);
	TestCase_assert(bitmapImagePtr->height == 2, "Expected 2 but got %u", bitmapImagePtr->height);
	TestCase_assert(bitmapImagePtr->bytesPerRow == 4, "Expected 3 but got %u", bitmapImagePtr->bytesPerRow);
	TestCase_assert(bitmapImagePtr->pixels != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!memcmp(pixelsGrayAlpha2x2r4, bitmapImagePtr->pixels, 8), "Expected image:\n  %02X%02X %02X%02X\n  %02X%02X %02X%02X\nActual image:\n  %02X%02X %02X%02X\n  %02X%02X %02X%02X", pixelsGrayAlpha2x2r4[0], pixelsGrayAlpha2x2r4[1], pixelsGrayAlpha2x2r4[2], pixelsGrayAlpha2x2r4[3], pixelsGrayAlpha2x2r4[4], pixelsGrayAlpha2x2r4[5], pixelsGrayAlpha2x2r4[6], pixelsGrayAlpha2x2r4[7], bitmapImagePtr->pixels[0], bitmapImagePtr->pixels[1], bitmapImagePtr->pixels[2], bitmapImagePtr->pixels[3], bitmapImagePtr->pixels[4], bitmapImagePtr->pixels[5], bitmapImagePtr->pixels[6], bitmapImagePtr->pixels[7]);
	bitmapImagePtr->dispose(bitmapImagePtr);
	
	bitmapImagePtr = BitmapImage_create(BITMAP_PIXEL_FORMAT_RGB_888, 3, 2, 9, pixelsRGB3x2r9);
	TestCase_assert(bitmapImagePtr != NULL, "BitmapImage_create unexpectedly returned NULL");
	if (bitmapImagePtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(bitmapImagePtr->pixelFormat == BITMAP_PIXEL_FORMAT_RGB_888, "Expected %d but got %d", BITMAP_PIXEL_FORMAT_RGB_888, bitmapImagePtr->pixelFormat);
	TestCase_assert(bitmapImagePtr->width == 3, "Expected 3 but got %u", bitmapImagePtr->width);
	TestCase_assert(bitmapImagePtr->height == 2, "Expected 2 but got %u", bitmapImagePtr->height);
	TestCase_assert(bitmapImagePtr->bytesPerRow == 9, "Expected 9 but got %u", bitmapImagePtr->bytesPerRow);
	TestCase_assert(bitmapImagePtr->pixels != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!memcmp(pixelsRGB3x2r9, bitmapImagePtr->pixels, 18), "Expected image:\n  %02X%02X%02X %02X%02X%02X %02X%02X%02X\n  %02X%02X%02X %02X%02X%02X %02X%02X%02X\nActual image:\n  %02X%02X%02X %02X%02X%02X %02X%02X%02X\n  %02X%02X%02X %02X%02X%02X %02X%02X%02X", pixelsRGB3x2r9[0], pixelsRGB3x2r9[1], pixelsRGB3x2r9[2], pixelsRGB3x2r9[3], pixelsRGB3x2r9[4], pixelsRGB3x2r9[5], pixelsRGB3x2r9[6], pixelsRGB3x2r9[7], pixelsRGB3x2r9[8], pixelsRGB3x2r9[9], pixelsRGB3x2r9[10], pixelsRGB3x2r9[11], pixelsRGB3x2r9[12], pixelsRGB3x2r9[13], pixelsRGB3x2r9[14], pixelsRGB3x2r9[15], pixelsRGB3x2r9[16], pixelsRGB3x2r9[17], bitmapImagePtr->pixels[0], bitmapImagePtr->pixels[1], bitmapImagePtr->pixels[2], bitmapImagePtr->pixels[3], bitmapImagePtr->pixels[4], bitmapImagePtr->pixels[5], bitmapImagePtr->pixels[6], bitmapImagePtr->pixels[7], bitmapImagePtr->pixels[8], bitmapImagePtr->pixels[9], bitmapImagePtr->pixels[10], bitmapImagePtr->pixels[11], bitmapImagePtr->pixels[12], bitmapImagePtr->pixels[13], bitmapImagePtr->pixels[14], bitmapImagePtr->pixels[15], bitmapImagePtr->pixels[16], bitmapImagePtr->pixels[17]);
	bitmapImagePtr->dispose(bitmapImagePtr);
	
	bitmapImagePtr = BitmapImage_create(BITMAP_PIXEL_FORMAT_RGBA_8888, 2, 3, 8, pixelsRGBA2x3r8);
	TestCase_assert(bitmapImagePtr != NULL, "BitmapImage_create unexpectedly returned NULL");
	if (bitmapImagePtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(bitmapImagePtr->pixelFormat == BITMAP_PIXEL_FORMAT_RGBA_8888, "Expected %d but got %d", BITMAP_PIXEL_FORMAT_RGBA_8888, bitmapImagePtr->pixelFormat);
	TestCase_assert(bitmapImagePtr->width == 2, "Expected 2 but got %u", bitmapImagePtr->width);
	TestCase_assert(bitmapImagePtr->height == 3, "Expected 3 but got %u", bitmapImagePtr->height);
	TestCase_assert(bitmapImagePtr->bytesPerRow == 8, "Expected 8 but got %u", bitmapImagePtr->bytesPerRow);
	TestCase_assert(bitmapImagePtr->pixels != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!memcmp(pixelsRGBA2x3r8, bitmapImagePtr->pixels, 24), "Expected image:\n  %02X%02X%02X%02X %02X%02X%02X%02X\n  %02X%02X%02X%02X %02X%02X%02X%02X\n  %02X%02X%02X%02X %02X%02X%02X%02X\nActual image:\n  %02X%02X%02X%02X %02X%02X%02X%02X\n  %02X%02X%02X%02X %02X%02X%02X%02X\n  %02X%02X%02X%02X %02X%02X%02X%02X", pixelsRGBA2x3r8[0], pixelsRGBA2x3r8[1], pixelsRGBA2x3r8[2], pixelsRGBA2x3r8[3], pixelsRGBA2x3r8[4], pixelsRGBA2x3r8[5], pixelsRGBA2x3r8[6], pixelsRGBA2x3r8[7], pixelsRGBA2x3r8[8], pixelsRGBA2x3r8[9], pixelsRGBA2x3r8[10], pixelsRGBA2x3r8[11], pixelsRGBA2x3r8[12], pixelsRGBA2x3r8[13], pixelsRGBA2x3r8[14], pixelsRGBA2x3r8[15], pixelsRGBA2x3r8[16], pixelsRGBA2x3r8[17], pixelsRGBA2x3r8[18], pixelsRGBA2x3r8[19], pixelsRGBA2x3r8[20], pixelsRGBA2x3r8[21], pixelsRGBA2x3r8[22], pixelsRGBA2x3r8[23], bitmapImagePtr->pixels[0], bitmapImagePtr->pixels[1], bitmapImagePtr->pixels[2], bitmapImagePtr->pixels[3], bitmapImagePtr->pixels[4], bitmapImagePtr->pixels[5], bitmapImagePtr->pixels[6], bitmapImagePtr->pixels[7], bitmapImagePtr->pixels[8], bitmapImagePtr->pixels[9], bitmapImagePtr->pixels[10], bitmapImagePtr->pixels[11], bitmapImagePtr->pixels[12], bitmapImagePtr->pixels[13], bitmapImagePtr->pixels[14], bitmapImagePtr->pixels[15], bitmapImagePtr->pixels[16], bitmapImagePtr->pixels[17], bitmapImagePtr->pixels[18], bitmapImagePtr->pixels[19], bitmapImagePtr->pixels[20], bitmapImagePtr->pixels[21], bitmapImagePtr->pixels[22], bitmapImagePtr->pixels[23]);
	bitmapImagePtr->dispose(bitmapImagePtr);
}

TEST_SUITE(BitmapImageTest, testInit)
