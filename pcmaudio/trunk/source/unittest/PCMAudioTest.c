#include "unittest/TestSuite.h"
#include "pcmaudio/PCMAudio.h"
#include "pcmaudio/PCMAudioStream.h"
#include <string.h>

static void testInit() {
	PCMAudio audio;
	PCMAudio * audioPtr;
	char * samples3;
	char samples4[4] = {3, 4, 5, 6};
	
	samples3 = malloc(3);
	PCMAudio_init(&audio, 1, 2, 22050, 3, samples3, false);
	TestCase_assert(audio.dispose == PCMAudio_dispose, "Expected %p but got %p", PCMAudio_dispose, audio.dispose);
	TestCase_assert(audio.bytesPerSample == 1, "Expected 1 but got %u", audio.bytesPerSample);
	TestCase_assert(audio.channelCount == 2, "Expected 2 but got %u", audio.channelCount);
	TestCase_assert(audio.sampleRate == 22050, "Expected 22050 but got %u", audio.sampleRate);
	TestCase_assert(audio.sampleCount == 3, "Expected 3 but got %u", (unsigned int) audio.sampleCount);
	TestCase_assert(audio.samples == samples3, "Expected %p but got %p", samples3, audio.samples);
	audio.dispose(&audio);
	
	samples3 = malloc(3);
	audioPtr = PCMAudio_create(1, 2, 22050, 3, samples3, false);
	TestCase_assert(audioPtr != NULL, "Expected non-NULL but got NULL");
	if (audioPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(audioPtr->dispose == PCMAudio_dispose, "Expected %p but got %p", PCMAudio_dispose, audioPtr->dispose);
	TestCase_assert(audioPtr->bytesPerSample == 1, "Expected 1 but got %u", audioPtr->bytesPerSample);
	TestCase_assert(audioPtr->channelCount == 2, "Expected 2 but got %u", audioPtr->channelCount);
	TestCase_assert(audioPtr->sampleRate == 22050, "Expected 22050 but got %u", audioPtr->sampleRate);
	TestCase_assert(audioPtr->sampleCount == 3, "Expected 3 but got %u", (unsigned int) audioPtr->sampleCount);
	TestCase_assert(audioPtr->samples == samples3, "Expected %p but got %p", samples3, audioPtr->samples);
	audioPtr->dispose(audioPtr);
	
	PCMAudio_init(&audio, 2, 3, 44100, 4, samples4, true);
	TestCase_assert(audio.dispose == PCMAudio_dispose, "Expected %p but got %p", PCMAudio_dispose, audio.dispose);
	TestCase_assert(audio.bytesPerSample == 2, "Expected 2 but got %u", audio.bytesPerSample);
	TestCase_assert(audio.channelCount == 3, "Expected 3 but got %u", audio.channelCount);
	TestCase_assert(audio.sampleRate == 44100, "Expected 44100 but got %u", audio.sampleRate);
	TestCase_assert(audio.sampleCount == 4, "Expected 4 but got %u", (unsigned int) audio.sampleCount);
	TestCase_assert(audio.samples != samples4, "Pointers expected to differ, but didn't");
	TestCase_assert(!memcmp(samples4, audio.samples, 4), "Expected {0x%02X, 0x%02X, 0x%02X, 0x%02X} but got {0x%02X, 0x%02X, 0x%02X, 0x%02X}", samples4[0], samples4[1], samples4[2], samples4[3], ((char *) audio.samples)[0], ((char *) audio.samples)[1], ((char *) audio.samples)[2], ((char *) audio.samples)[3]);
	audio.dispose(&audio);
	
	audioPtr = PCMAudio_create(2, 3, 44100, 4, samples4, true);
	TestCase_assert(audioPtr != NULL, "Expected non-NULL but got NULL");
	if (audioPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(audioPtr->dispose == PCMAudio_dispose, "Expected %p but got %p", PCMAudio_dispose, audioPtr->dispose);
	TestCase_assert(audioPtr->bytesPerSample == 2, "Expected 2 but got %u", audioPtr->bytesPerSample);
	TestCase_assert(audioPtr->channelCount == 3, "Expected 3 but got %u", audioPtr->channelCount);
	TestCase_assert(audioPtr->sampleRate == 44100, "Expected 44100 but got %u", audioPtr->sampleRate);
	TestCase_assert(audioPtr->sampleCount == 4, "Expected 4 but got %u", (unsigned int) audioPtr->sampleCount);
	TestCase_assert(audioPtr->samples != samples4, "Pointers expected to differ, but didn't");
	TestCase_assert(!memcmp(samples4, audioPtr->samples, 4), "Expected {0x%02X, 0x%02X, 0x%02X, 0x%02X} but got {0x%02X, 0x%02X, 0x%02X, 0x%02X}", samples4[0], samples4[1], samples4[2], samples4[3], ((char *) audioPtr->samples)[0], ((char *) audioPtr->samples)[1], ((char *) audioPtr->samples)[2], ((char *) audioPtr->samples)[3]);
	audioPtr->dispose(audioPtr);
}

static void testInitStream() {
	PCMAudioStream stream;
	
	PCMAudioStream_init(&stream);
	TestCase_assert(stream.dispose == PCMAudioStream_dispose, "Expected %p but got %p", PCMAudioStream_dispose, stream.dispose);
	TestCase_assert(stream.read == NULL, "Expected NULL but got %p", stream.read);
	TestCase_assert(stream.seek == NULL, "Expected NULL but got %p", stream.seek);
	TestCase_assert(stream.bytesPerSample == 0, "Expected 0 but got %u", stream.bytesPerSample);
	TestCase_assert(stream.channelCount == 0, "Expected 0 but got %u", stream.channelCount);
	TestCase_assert(stream.sampleRate == 0, "Expected 0 but got %u", stream.sampleRate);
	TestCase_assert(stream.sampleCount == 0, "Expected 0 but got %u", (unsigned int) stream.sampleCount);
	stream.dispose(&stream);
}

TEST_SUITE(PCMAudioTest, testInit, testInitStream)
