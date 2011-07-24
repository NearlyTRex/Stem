#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "alaudio/ALAudioBuffer.h"
#include "alaudio/ALAudioSource.h"
#include "alaudio/ALAudioSystem.h"
//#include "vorbisaudioio/VorbisAudioIO.h"
//#include "vorbisaudioio/VorbisAudioStream.h"

// Need loop, stream, arbitrary vorbis file, multiple formats, 3d audio?

int main() {
	ALAudioSystem * audioSystem;
	ALAudioBuffer * buffer;
	ALAudioSource * source;
	uint16_t audioData[22050];
	size_t audioDataSize = 22050;
	size_t sampleIndex;
	
	for (sampleIndex = 0; sampleIndex < audioDataSize; sampleIndex++) {
		audioData[sampleIndex] = (sin(sampleIndex * M_PI * 2 / 22050 * 261.626f) * 0.5f + 0.5f) * UINT16_MAX / 2;
	}
	
	audioSystem = ALAudioSystem_create();
	buffer = ALAudioBuffer_create();
	buffer->bufferAudioData(buffer, AL_FORMAT_MONO16, audioData, audioDataSize, 22050);
	source = ALAudioSource_create();
	source->load(source, buffer);
	source->play(source);
	sleep(1);
	
	return EXIT_SUCCESS;
}
