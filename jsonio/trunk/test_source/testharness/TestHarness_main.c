#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "jsonio/JSONEmitter.h"
#include "jsonio/JSONIO.h"
#include "jsonio/JSONParser.h"
#include "utilities/IOUtilities.h"

static void printUsage() {
	fprintf(stderr, "Usage: jsonio_testharness [-f {compact | singleLine | multiLine}] [-o outfile] [infile]\n");
	fprintf(stderr, "if infile is unspecified, stdin will be used.\n");
	fprintf(stderr, "if outfile is \"-\", stdout will be used.\n");
	fprintf(stderr, "if outfile is unspecified, infile will only be checked for errors. No JSON will be written out.\n");
}

int main(int argc, char ** argv) {
	int argIndex;
	bool nextArgIsOutfile = false, nextArgIsFormat = false;
	char * infileName = NULL, * outfileName = NULL;
	char * infileData = NULL;
	size_t infileLength = 0;
	struct JSONNode * rootNode;
	struct JSONParseError parseError;
	struct JSONEmissionError emissionError;
	enum JSONEmitterFormat format = JSONEmitterFormat_multiLine;
	
	for (argIndex = 1; argIndex < argc; argIndex++) {
		if (nextArgIsOutfile) {
			outfileName = argv[argIndex];
			nextArgIsOutfile = false;
			
		} else if (nextArgIsFormat) {
			if (!strcmp(argv[argIndex], "compact")) {
				format = JSONEmitterFormat_compact;
				
			} else if (!strcmp(argv[argIndex], "singleLine")) {
				format = JSONEmitterFormat_singleLine;
				
			} else if (!strcmp(argv[argIndex], "multiLine")) {
				format = JSONEmitterFormat_multiLine;
				
			} else {
				fprintf(stderr, "Error: Unrecognized arg \"%s\" following -f\n", argv[argIndex]);
				printUsage();
				return EXIT_FAILURE;
			}
			nextArgIsFormat = false;
			
		} else {
			if (!strcmp(argv[argIndex], "-o")) {
				if (outfileName != NULL) {
					fprintf(stderr, "Error: -o may only be used once\n");
					printUsage();
					return EXIT_FAILURE;
				}
				nextArgIsOutfile = true;
				
			} else if (!strcmp(argv[argIndex], "-f")) {
				nextArgIsFormat = true;
				
			} else {
				if (infileName != NULL) {
					fprintf(stderr, "Error: input file may only be specified once\n");
					printUsage();
					return EXIT_FAILURE;
				}
				infileName = argv[argIndex];
			}
		}
	}
	if (nextArgIsOutfile) {
		fprintf(stderr, "Error: -o specified as last arg\n");
		printUsage();
		return EXIT_FAILURE;
	}
	
	if (infileName == NULL) {
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
		rootNode = JSONParser_loadString(infileData, infileLength, &parseError);
		
	} else {
		rootNode = JSONParser_loadFile(infileName, &parseError);
	}
	
	if (rootNode == NULL) {
		fprintf(stderr, "Failed to load %s as JSON: %s (error code %d)\n", infileName == NULL ? "stdin" : infileName, parseError.description, parseError.code);
		if (parseError.code != JSONParseError_fileNotFound) {
			size_t charIndex;
			size_t lineCount = 1, lineFirstCharIndex = 0, lineEndCharIndex;
			
			if (infileName != NULL) {
				infileData = readFileSimple(infileName, &infileLength);
				if (infileData == NULL) {
					fprintf(stderr, "Double fail! Couldn't load %s to determine line number even though it loaded with JSONParser_loadFile!\n", infileName);
					return EXIT_FAILURE;
				}
			}
			
			lineEndCharIndex = infileLength;
			for (charIndex = 0; charIndex < infileLength; charIndex++) {
				if (infileData[charIndex] == '\n') {
					lineEndCharIndex = charIndex;
					if (lineFirstCharIndex <= parseError.charIndex && charIndex >= parseError.charIndex) {
						break;
					}
					lineCount++;
					lineFirstCharIndex = lineEndCharIndex + 1;
				}
			}
			
			fprintf(stderr, "Line %lu, char %lu (%lu overall)\n", (unsigned long) lineCount + 1, (unsigned long) (parseError.charIndex - lineFirstCharIndex + 1), (unsigned long) parseError.charIndex + 1);
			fprintf(stderr, "%.*s\n", (int) (lineEndCharIndex - lineFirstCharIndex), infileData + lineFirstCharIndex);
			for (charIndex = lineFirstCharIndex; charIndex < parseError.charIndex && charIndex < infileLength; charIndex++) {
				if (isspace(infileData[charIndex])) {
					fputc(infileData[charIndex], stderr);
					
				} else {
					fputc(' ', stderr);
				}
			}
			fputc('^', stderr);
			fputc('\n', stderr);
			
			return EXIT_FAILURE;
		}
	}
	
	free(infileData);
	
	if (outfileName != NULL) {
		bool success = false;
		
		if (!strcmp(outfileName, "-")) {
			char * outfileData;
			size_t outfileLength;
			
			outfileData = JSONEmitter_writeString(rootNode, format, &outfileLength, &emissionError);
			if (outfileData != NULL) {
				fwrite(outfileData, 1, outfileLength, stdout);
				success = true;
			}
			free(outfileData);
			
		} else {
			success = JSONEmitter_writeFile(rootNode, format, outfileName, &emissionError);
		}
		
		if (!success) {
			fprintf(stderr, "Failed to emit loaded node as JSON: %s (error code %d)\n", emissionError.description, emissionError.code);
		}
	}
	
	return EXIT_SUCCESS;
}
