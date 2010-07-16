#include <stdlib.h>
#include "glbitmapfont/GLBitmapFont.h"
#include "jsonserialization/JSONDeserializationContext.h"
#include "jsonserialization/JSONSerializationContext.h"
#include "utilities/IOUtilities.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printUsage() {
	fprintf(stderr, "Usage: convertfontunits [-o outfile] [-fromEmHeight height -toEmHeight height] [-fromTexWidth width -toTexWidth width] [-fromTexHeight height -toTexHeight height] infile\n");
	fprintf(stderr, "If outfile is omitted or specified as -, stdout will be used.\n");
	fprintf(stderr, "If infile is omitted or specified as -, stdin will be used.\n");
	fprintf(stderr, "If unspecified, all from/to conversions default to 1.0.\n");
}

int main(int argc, char ** argv) {
	int argIndex;
	char * infile = NULL, * outfile = NULL;
	double fromEmHeight = 1.0, toEmHeight = 1.0;
	double fromTexWidth = 1.0, toTexWidth = 1.0;
	double fromTexHeight = 1.0, toTexHeight = 1.0;
	char * infileData = NULL;
	size_t infileLength = 0;
	GLBitmapFont * font;
	JSONDeserializationContext * deserializationContext;
	unsigned int charIndex, kernCharIndex;
	JSONSerializationContext * serializationContext;
	char * outfileData = NULL;
	size_t outfileLength = 0;
	struct JSONEmissionError error;
	bool success;
	
	for (argIndex = 1; argIndex < argc; argIndex++) {
		if (!strcmp(argv[argIndex], "-o")) {
			if (argIndex >= argc - 1 || outfile != NULL) {
				printUsage();
				return EXIT_FAILURE;
			}
			outfile = argv[++argIndex];
			
		} else if (!strcmp(argv[argIndex], "-fromEmHeight")) {
			if (argIndex >= argc - 1 || !sscanf(argv[++argIndex], "%lf", &fromEmHeight)) {
				printUsage();
				return EXIT_FAILURE;
			}
			
		} else if (!strcmp(argv[argIndex], "-toEmHeight")) {
			if (argIndex >= argc - 1 || !sscanf(argv[++argIndex], "%lf", &toEmHeight)) {
				printUsage();
				return EXIT_FAILURE;
			}
			
		} else if (!strcmp(argv[argIndex], "-fromTexWidth")) {
			if (argIndex >= argc - 1 || !sscanf(argv[++argIndex], "%lf", &fromTexWidth)) {
				printUsage();
				return EXIT_FAILURE;
			}
			
		} else if (!strcmp(argv[argIndex], "-toTexWidth")) {
			if (argIndex >= argc - 1 || !sscanf(argv[++argIndex], "%lf", &toTexWidth)) {
				printUsage();
				return EXIT_FAILURE;
			}
			
		} else if (!strcmp(argv[argIndex], "-fromTexHeight")) {
			if (argIndex >= argc - 1 || !sscanf(argv[++argIndex], "%lf", &fromTexHeight)) {
				printUsage();
				return EXIT_FAILURE;
			}
			
		} else if (!strcmp(argv[argIndex], "-toTexHeight")) {
			if (argIndex >= argc - 1 || !sscanf(argv[++argIndex], "%lf", &toTexHeight)) {
				printUsage();
				return EXIT_FAILURE;
			}
			
		} else {
			if (infile == NULL) {
				infile = argv[argIndex];
			}
		}
	}
	
	if (infile == NULL || !strcmp(infile, "-")) {
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
		infileData = readFileSimple(infile, &infileLength);
	}
	
	deserializationContext = JSONDeserializationContext_createWithString(infileData, infileLength);
	free(infileData);
	if (deserializationContext->status != SERIALIZATION_ERROR_OK) {
		fprintf(stderr, "Error: Failed to parse %s as json (JSONDeserializationContext status %d)\n", (infile == NULL || !strcmp(infile, "-")) ? "stdin" : infile, deserializationContext->status);
		return EXIT_FAILURE;
	}
	
	font = GLBitmapFont_deserialize((DeserializationContext *) deserializationContext);
	if (font == NULL) {
		fprintf(stderr, "Error: Failed to deserialize %s (status %d)\n", (infile == NULL || !strcmp(infile, "-")) ? "stdin" : infile, deserializationContext->status);
		return EXIT_FAILURE;
	}
	deserializationContext->dispose(deserializationContext);
	
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		font->characters[charIndex].glyphOffset = font->characters[charIndex].glyphOffset * toEmHeight / fromEmHeight;
		font->characters[charIndex].glyphWidth = font->characters[charIndex].glyphWidth * toEmHeight / fromEmHeight;
		font->characters[charIndex].advance = font->characters[charIndex].advance * toEmHeight / fromEmHeight;
		font->characters[charIndex].textureLeft = font->characters[charIndex].textureLeft * toTexWidth / fromTexWidth;
		font->characters[charIndex].textureRight = font->characters[charIndex].textureRight * toTexWidth / fromTexWidth;
		font->characters[charIndex].textureBottom = font->characters[charIndex].textureBottom * toTexHeight / fromTexHeight;
		font->characters[charIndex].textureTop = font->characters[charIndex].textureTop * toTexHeight / fromTexHeight;
		for (kernCharIndex = 0; kernCharIndex < font->characters[charIndex].kernCharCount; kernCharIndex++) {
			font->characters[charIndex].kernChars[kernCharIndex].offset = font->characters[charIndex].kernChars[kernCharIndex].offset * toEmHeight / fromEmHeight;
		}
	}
	
	serializationContext = JSONSerializationContext_create();
	GLBitmapFont_serialize(font, (SerializationContext *) serializationContext);
	outfileData = serializationContext->writeToString(serializationContext, JSONEmitterFormat_multiLine, &outfileLength, &error);
	serializationContext->dispose(serializationContext);
	if (outfileData == NULL) {
		fprintf(stderr, "Error: Failed to serialize font: %s (%d)\n", error.description, error.code);
		return EXIT_FAILURE;
	}
	
	if (outfile == NULL || !strcmp(outfile, "-")) {
		success = fwrite(outfileData, 1, outfileLength, stdout) == outfileLength;
	} else {
		success = writeFileSimple(outfile, outfileData, outfileLength);
	}
	if (!success) {
		fprintf(stderr, "Error: Couldn't write %s: %d\n", (outfile == NULL || !strcmp(outfile, "-")) ? "stdout" : outfile, errno);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
