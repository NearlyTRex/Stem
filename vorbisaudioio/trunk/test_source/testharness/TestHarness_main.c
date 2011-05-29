#include <stdlib.h>
#include <stdio.h>

#include "vorbisaudioio/VorbisAudioIO.h"

int main(int argc, char ** argv) {
	PCMAudio * audio;
	
	if (argc < 2) {
		fprintf(stderr, "Specify file as argv[1]\n");
		return EXIT_FAILURE;
	}
	
	audio = VorbisAudioIO_loadOggVorbisFile(argv[1]);
	if (audio == NULL) {
		return EXIT_FAILURE;
	}
	
	printf("Read %u samples\n%u bytes per sample\n%u channels\n%u samples per second\n", audio->sampleCount, audio->bytesPerSample, audio->channelCount, audio->sampleRate);
	
	if (argc > 2) {
		FILE * outFile;
		
		outFile = fopen(argv[2], "wb");
		fwrite(audio->samples, 1, audio->sampleCount * audio->channelCount * audio->bytesPerSample, outFile);
		fclose(outFile);
	}
	
	audio->dispose(audio);
	
	return EXIT_SUCCESS;
}
