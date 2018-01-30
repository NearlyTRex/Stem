/*
  Copyright (c) 2018 Alex Diener
  
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
  
  Alex Diener alex@ludobloom.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3dmodelio/MeshData.h"
#include "binaryserialization/BinaryDeserializationContext.h"
#include "binaryserialization/BinarySerializationContext.h"
#include "jsonserialization/JSONDeserializationContext.h"
#include "jsonserialization/JSONSerializationContext.h"

static const char * inFileName, * outFileName;
static bool writeBinary;

static void printUsage() {
	fprintf(stderr, "Usage: meshutils -o outfile [--binary | --json] infile\n");
}

static void parseArgs(int argc, char ** argv) {
	int argIndex;
	
	for (argIndex = 1; argIndex < argc; argIndex++) {
		if (!strcmp(argv[argIndex], "--help")) {
			printUsage();
			exit(EXIT_SUCCESS);
			
		} else if (!strcmp(argv[argIndex], "-o")) {
			if (argIndex >= argc - 1) {
				fprintf(stderr, "Error: -o specified at end of argv\n");
				printUsage();
				exit(EXIT_FAILURE);
			}
			argIndex++;
			outFileName = argv[argIndex];
			
		} else if (!strcmp(argv[argIndex], "--binary")) {
			writeBinary = true;
			
		} else if (!strcmp(argv[argIndex], "--json")) {
			writeBinary = false;
			
		} else {
			if (inFileName != NULL) {
				fprintf(stderr, "Couldn't understand argument \"%s\" (infile already read as \"%s\")\n", argv[argIndex], inFileName);
				printUsage();
				exit(EXIT_FAILURE);
			}
			inFileName = argv[argIndex];
		}
	}
}

static DeserializationContext * guessDeserializationType(const char * filePath) {
	FILE * file;
	int firstChar;
	DeserializationContext * context;
	
	file = fopen(filePath, "rb");
	if (file == NULL) {
		fprintf(stderr, "Couldn't open \"%s\"\n", filePath);
		return NULL;
	}
	firstChar = fgetc(file);
	fclose(file);
	
	switch (firstChar) {
		case '{':
			context = (DeserializationContext *) JSONDeserializationContext_createWithFile(filePath);
			if (context->status != SERIALIZATION_ERROR_OK) {
				fprintf(stderr, "File \"%s\" looks like JSON, but JSONDeserializationContext couldn't parse it.\n", filePath);
				context = NULL;
			}
			break;
		case 'S':
		case 'm':
			context = (DeserializationContext *) BinaryDeserializationContext_createWithFile(filePath);
			break;
		default:
			fprintf(stderr, "Couldn't determine serialization type of file \"%s\" (first character was '%c')\n", filePath, firstChar);
			context = NULL;
			break;
	}
	return context;
}

static void * deserializeFile(const char * filePath, void * (* deserializeFunction)(compat_type(DeserializationContext *))) {
	DeserializationContext * context;
	void * result;
	
	context = guessDeserializationType(filePath);
	if (context == NULL) {
		return NULL;
	}
	result = deserializeFunction(context);
	if (result == NULL) {
		fprintf(stderr, "Couldn't deserialize \"%s\" (error %d)", filePath, context->status);
		context->dispose(context);
		return NULL;
	}
	context->dispose(context);
	return result;
}

int main(int argc, char ** argv) {
	MeshData * meshData;
	
	parseArgs(argc, argv);
	if (inFileName == NULL || outFileName == NULL) {
		fprintf(stderr, "Error: Must specify both input and output file names\n");
		printUsage();
		return EXIT_FAILURE;
	}
	
	meshData = deserializeFile(inFileName, (void * (*)(void *)) MeshData_deserialize);
	if (meshData == NULL) {
		fprintf(stderr, "Couldn't load input mesh\n");
	}
	
	if (writeBinary) {
		BinarySerializationContext * context = BinarySerializationContext_create(false);
		MeshData_serialize(meshData, context);
		BinarySerializationContext_writeToFile(context, outFileName);
		BinarySerializationContext_dispose(context);
		
	} else {
		JSONSerializationContext * context = JSONSerializationContext_create();
		MeshData_serialize(meshData, context);
		JSONSerializationContext_writeToFile(context, JSONEmitterFormat_multiLine, outFileName, NULL);
		JSONSerializationContext_dispose(context);
	}
	
	return EXIT_SUCCESS;
}
