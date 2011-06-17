#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "vorbisaudioio/VorbisAudioIO.h"
#include "vorbisaudioio/VorbisAudioStream.h"

static void printUsage() {
	fprintf(stderr, "Usage: alaudio_testharness [-o outFile] [--stream] [--loop] inFile\n");
}

#define STREAM_READ_SIZE 16384

int main(int argc, char ** argv) {
	int argIndex;
	char * inFileName = NULL;
	char * outFileName = NULL;
	bool stream = false;
	bool loop = false;
	
	for (argIndex = 1; argIndex < argc; argIndex++) {
		if (!strcmp(argv[argIndex], "-o")) {
			if (argIndex >= argc - 1) {
				printUsage();
				return EXIT_FAILURE;
			}
			outFileName = argv[++argIndex];
			
		} else if (!strcmp(argv[argIndex], "--stream")) {
			stream = true;
			
		} else if (!strcmp(argv[argIndex], "--loop")) {
			loop = true;
			
		} else if (!strcmp(argv[argIndex], "--help")) {
			printUsage();
			return EXIT_SUCCESS;
			
		} else {
			if (inFileName != NULL) {
				printUsage();
				return EXIT_FAILURE;
			}
			inFileName = argv[argIndex];
		}
	}
	
	if (inFileName == NULL) {
		printUsage();
		return EXIT_FAILURE;
	}
	
	if (stream) {
		VorbisAudioStream * stream;
		
		stream = VorbisAudioStream_createWithFile(inFileName);
		printf("Stream has %zu samples\n%u bytes per sample\n%u channels\n%u samples per second\n", stream->sampleCount, stream->bytesPerSample, stream->channelCount, stream->sampleRate);
		
		if (outFileName != NULL) {
			FILE * outFile;
			size_t bytesToRead, bytesRead;
			char buffer[STREAM_READ_SIZE];
			
			bytesToRead = stream->sampleCount * stream->channelCount * stream->bytesPerSample;
			if (loop) {
				bytesToRead *= 2;
			}
			
			outFile = fopen(outFileName, "wb");
			while (bytesToRead > 0) {
				bytesRead = stream->read(stream, STREAM_READ_SIZE > bytesToRead ? bytesToRead : STREAM_READ_SIZE, buffer, loop);
				if (bytesRead == 0) {
					fprintf(stderr, "Unexpectedly couldn't read %zu more bytes\n", bytesToRead);
					break;
				}
				printf("Read %zu bytes\n", bytesRead);
				bytesToRead -= bytesRead;
				fwrite(buffer, 1, bytesRead, outFile);
			}
			fclose(outFile);
		}
		
		stream->dispose(stream);
		
	} else {
		PCMAudio * audio;
		
		audio = VorbisAudioIO_loadOggVorbisFile(inFileName);
		if (audio == NULL) {
			return EXIT_FAILURE;
		}
		
		printf("Read %zu samples\n%u bytes per sample\n%u channels\n%u samples per second\n", audio->sampleCount, audio->bytesPerSample, audio->channelCount, audio->sampleRate);
		
		if (outFileName != NULL) {
			FILE * outFile;
			
			outFile = fopen(outFileName, "wb");
			fwrite(audio->samples, 1, audio->sampleCount * audio->channelCount * audio->bytesPerSample, outFile);
			fclose(outFile);
		}
		
		audio->dispose(audio);
	}
	
	return EXIT_SUCCESS;
}
