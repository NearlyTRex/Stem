#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitmapimage/BitmapImage.h"
#include "pngimageio/PNGImageIO.h"
#include "utilities/IOUtilities.h"

static void printUsage() {
	fprintf(stderr, "Usage: pngimageio_testharness [-itype {png | raw}] [-iwidth width_in_pixels] [-iformat {rgba | rgb | grayalpha | gray}] [-iflip] [-o outfile] [-otype {png | raw}] [-oformat {rgba | rgb | grayalpha | gray}] [-oflip] file\n");
	fprintf(stderr, "if file is \"-\", stdin will be used.\n");
	fprintf(stderr, "if outfile is \"-\", stdout will be used.\n");
	fprintf(stderr, "If -itype raw is used, -iwidth and -iformat must be specified. If -itype png is used, -iwidth is ignored.\n");
}

int main(int argc, char ** argv) {
	int argIndex;
	bool inputIsRaw = false;
	unsigned int inputWidth = 0;
	int inputFormat = PNG_PIXEL_FORMAT_AUTOMATIC;
	char * infileName = NULL;
	unsigned char * infileData = NULL;
	size_t infileLength = 0;
	bool flipInput = false;
	bool outputIsRaw = false;
	int outputFormat = PNG_PIXEL_FORMAT_AUTOMATIC;
	char * outfileName = NULL;
	void * outfileData = NULL;
	size_t outfileLength = 0;
	bool flipOutput = false;
	char * lastFlagArg = NULL;
	BitmapImage * inputImage;
	char * pixelFormatStrings[] = {
		"rgba",
		"rgb",
		"grayalpha",
		"gray"
	};
	
	if (argc < 2) {
		printUsage();
		return EXIT_FAILURE;
	}
	
	for (argIndex = 1; argIndex < argc; argIndex++) {
		if (lastFlagArg == NULL) {
			if (!strcmp(argv[argIndex], "-itype") ||
			    !strcmp(argv[argIndex], "-iwidth") ||
			    !strcmp(argv[argIndex], "-iformat") ||
			    !strcmp(argv[argIndex], "-o") ||
			    !strcmp(argv[argIndex], "-otype") ||
			    !strcmp(argv[argIndex], "-oformat")) {
				lastFlagArg = argv[argIndex];
				
			} else if (!strcmp(argv[argIndex], "-iflip")) {
				flipInput = true;
				
			} else if (!strcmp(argv[argIndex], "-oflip")) {
				flipOutput = true;
				
			} else {
				if (infileName != NULL) {
					fprintf(stderr, "Unrecognized option or multiple input files specified; unable to continue\n");
					printUsage();
					return EXIT_FAILURE;
				}
				
				infileName = argv[argIndex];
			}
			
		} else {
			if (!strcmp(lastFlagArg, "-itype")) {
				if (!strcmp(argv[argIndex], "png")) {
					inputIsRaw = false;
				} else if (!strcmp(argv[argIndex], "raw")) {
					inputIsRaw = true;
				} else {
					fprintf(stderr, "Invalid argument following -itype: %s (must be either png or raw)\n", argv[argIndex]);
					return EXIT_FAILURE;
				}
				
			} else if (!strcmp(lastFlagArg, "-iwidth")) {
				if (!sscanf(argv[argIndex], "%u", &inputWidth)) {
					fprintf(stderr, "Invalid argument following -iwidth: %s (must be a positive integer)\n", argv[argIndex]);
					return EXIT_FAILURE;
				}
				
			} else if (!strcmp(lastFlagArg, "-iformat")) {
				if (!strcmp(argv[argIndex], "rgba")) {
					inputFormat = BITMAP_PIXEL_FORMAT_RGBA_8888;
				} else if (!strcmp(argv[argIndex], "rgb")) {
					inputFormat = BITMAP_PIXEL_FORMAT_RGB_888;
				} else if (!strcmp(argv[argIndex], "grayalpha")) {
					inputFormat = BITMAP_PIXEL_FORMAT_GRAYALPHA_88;
				} else if (!strcmp(argv[argIndex], "gray")) {
					inputFormat = BITMAP_PIXEL_FORMAT_GRAY_8;
				} else {
					fprintf(stderr, "Invalid argument following -iformat: %s (must be rgba, rgb, grayalpha, or gray)\n", argv[argIndex]);
					return EXIT_FAILURE;
				}
				
			} else if (!strcmp(lastFlagArg, "-o")) {
				outfileName = argv[argIndex];
				
			} else if (!strcmp(lastFlagArg, "-otype")) {
				if (!strcmp(argv[argIndex], "png")) {
					outputIsRaw = false;
				} else if (!strcmp(argv[argIndex], "raw")) {
					outputIsRaw = true;
				} else {
					fprintf(stderr, "Invalid argument following -otype: %s (must be either png or raw)\n", argv[argIndex]);
					return EXIT_FAILURE;
				}
				
			} else if (!strcmp(lastFlagArg, "-oformat")) {
				if (!strcmp(argv[argIndex], "rgba")) {
					outputFormat = BITMAP_PIXEL_FORMAT_RGBA_8888;
				} else if (!strcmp(argv[argIndex], "rgb")) {
					outputFormat = BITMAP_PIXEL_FORMAT_RGB_888;
				} else if (!strcmp(argv[argIndex], "grayalpha")) {
					outputFormat = BITMAP_PIXEL_FORMAT_GRAYALPHA_88;
				} else if (!strcmp(argv[argIndex], "gray")) {
					outputFormat = BITMAP_PIXEL_FORMAT_GRAY_8;
				} else {
					fprintf(stderr, "Invalid argument following -oformat: %s (must be rgba, rgb, grayalpha, or gray)\n", argv[argIndex]);
					return EXIT_FAILURE;
				}
			}
			lastFlagArg = NULL;
		}
	}
	if (lastFlagArg != NULL) {
		fprintf(stderr, "%s must be followed by an argument specifying its value\n", lastFlagArg);
		printUsage();
		return EXIT_FAILURE;
	}
	
	if (infileName == NULL) {
		fprintf(stderr, "No input file specified!\n");
		printUsage();
		return EXIT_FAILURE;
	}
	if (inputIsRaw && outputIsRaw) {
		fprintf(stderr, "Invalid parameter combination; input and output cannot both be raw\n");
		return EXIT_FAILURE;
	}
	if (inputIsRaw && inputWidth == 0) {
		fprintf(stderr, "-iwidth must be specified if -itype raw is used\n");
		return EXIT_FAILURE;
	}
	if (inputIsRaw && inputFormat == PNG_PIXEL_FORMAT_AUTOMATIC) {
		fprintf(stderr, "-iformat must be specified if -itype raw is used\n");
		return EXIT_FAILURE;
	}
	if (inputIsRaw && infileLength % (inputWidth * BitmapImage_pixelFormatBytes(inputFormat)) != 0) {
		fprintf(stderr, "Raw image data length is not divisible by specified pixel width (%d bytes left at the end of the file when expecting rows of %d bytes)\n", (int) (infileLength % (inputWidth * BitmapImage_pixelFormatBytes(inputFormat))), (int) (inputWidth * BitmapImage_pixelFormatBytes(inputFormat)));
		return EXIT_FAILURE;
	}
	if (inputIsRaw && flipInput) {
		fprintf(stderr, "Warning: -iflip will be ignored when using -itype raw\n");
	}
	if (outputIsRaw && flipOutput) {
		fprintf(stderr, "Warning: -oflip will be ignored when using -otype raw\n");
	}
	
	if (!strcmp(infileName, "-")) {
		int byte;
		size_t allocatedSize = 1;
		
		infileData = malloc(allocatedSize);
		for (;;) {
			byte = getchar();
			if (feof(stdin)) {
				break;
			}
			if (infileLength >= allocatedSize) {
				allocatedSize *= 2;
				infileData = realloc(infileData, allocatedSize);
			}
			infileData[infileLength++] = byte;
		}
		
	} else {
		infileData = readFileSimple(infileName, &infileLength);
		if (infileData == NULL) {
			fprintf(stderr, "Failed to read %s\n", infileName);
			return EXIT_FAILURE;
		}
	}
	
	if (inputIsRaw) {
		inputImage = BitmapImage_createWithPixels(inputFormat, inputWidth, infileLength / (inputWidth * BitmapImage_pixelFormatBytes(inputFormat)), inputWidth * BitmapImage_pixelFormatBytes(inputFormat), infileData);
	} else {
		inputImage = PNGImageIO_loadPNGData(infileData, infileLength, inputFormat, flipInput);
		if (inputImage == NULL) {
			fprintf(stderr, "Failed to load %s as a png\n", !strcmp(infileName, "-") ? "stdin" : infileName);
			return EXIT_FAILURE;
		}
	}
	
	fprintf(stderr, "%u x %u %s\n", inputImage->width, inputImage->height, pixelFormatStrings[inputImage->pixelFormat]);
	
	if (outfileName != NULL) {
		if (outputIsRaw) {
			outfileData = inputImage->pixels;
			outfileLength = inputImage->bytesPerRow * inputImage->height;
			
		} else {
			outfileData = PNGImageIO_writePNGData(inputImage, outputFormat, flipOutput, &outfileLength);
			if (outfileData == NULL) {
				fprintf(stderr, "Failed to write PNG data!\n");
				return EXIT_FAILURE;
			}
		}
		
		if (!strcmp(outfileName, "-")) {
			fwrite(outfileData, 1, outfileLength, stdout);
		} else {
			if (!writeFileSimple(outfileName, outfileData, outfileLength)) {
				fprintf(stderr, "Failed to write to %s (errno = %d)\n", outfileName, errno);
			}
		}
	}
	
	return EXIT_SUCCESS;
}
