/*
  Copyright (c) 2014 Alex Diener
  
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

#include "utilities/Base64.h"

static const unsigned char index[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static const unsigned char reverseIndex[256] =
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x3E\xFF\xFF\xFF\x3F"
	"\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E"
	"\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\xFF\xFF\xFF\xFF\xFF"
	"\xFF\x1A\x1B\x1C\x1D\x1E\x1F\x20\x21\x22\x23\x24\x25\x26\x27\x28"
	"\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";

size_t encodeBase64(const void * data, size_t dataLength, char * outEncodedString, size_t maxLength) {
	unsigned char byte, encodedChars[4];
	size_t byteIndex, charIndex = 0;
	const char * encodedData = data;
	
	for (byteIndex = 0; byteIndex < dataLength; byteIndex += 3) {
		byte = encodedData[byteIndex];
		encodedChars[0] = byte >> 2;
		encodedChars[1] = (byte & 0x03) << 4;
		if (byteIndex + 1 < dataLength) {
			byte = encodedData[byteIndex + 1];
			encodedChars[1] |= byte >> 4;
			encodedChars[2] = (byte & 0x0F) << 2;
		} else {
			encodedChars[2] = 64;
		}
		if (byteIndex + 2 < dataLength) {
			byte = encodedData[byteIndex + 2];
			encodedChars[2] |= byte >> 6;
			encodedChars[3] = byte & 0x3F;
		} else {
			encodedChars[3] = 64;
		}
		
		if (outEncodedString != NULL) {
			if (charIndex < maxLength) {
				outEncodedString[charIndex] = index[encodedChars[0]];
			}
			if (charIndex + 1 < maxLength) {
				outEncodedString[charIndex + 1] = index[encodedChars[1]];
			}
			if (charIndex + 2 < maxLength) {
				outEncodedString[charIndex + 2] = index[encodedChars[2]];
			}
			if (charIndex + 3 < maxLength) {
				outEncodedString[charIndex + 3] = index[encodedChars[3]];
			}
		}
		charIndex += 4;
	}
	
	if (outEncodedString != NULL && maxLength > 0) {
		if (charIndex >= maxLength) {
			outEncodedString[maxLength - 1] = 0;
		} else {
			outEncodedString[charIndex] = 0;
		}
	}
	return charIndex;
}

size_t decodeBase64(const char * encodedString, size_t stringLength, void * outDecodedData, size_t maxLength) {
	unsigned char encodedBytes[4], decodedBytes[3];
	size_t charIndex = 0, byteIndex = 0, encodedByteIndex = 0;
	char * decodedData = outDecodedData;
	
	for (charIndex = 0; charIndex < stringLength; charIndex++) {
		encodedBytes[encodedByteIndex] = reverseIndex[(unsigned char) encodedString[charIndex]];
		if (encodedBytes[encodedByteIndex] == 0xFF) {
			continue;
		}
		encodedByteIndex++;
		if (encodedByteIndex >= 4) {
			encodedByteIndex = 0;
			decodedBytes[0] = encodedBytes[0] << 2 | encodedBytes[1] >> 4;
			decodedBytes[1] = encodedBytes[1] << 4 | encodedBytes[2] >> 2;
			decodedBytes[2] = encodedBytes[2] << 6 | encodedBytes[3];
			if (decodedData != NULL) {
				if (byteIndex < maxLength) {
					decodedData[byteIndex] = decodedBytes[0];
				}
				if (byteIndex + 1 < maxLength) {
					decodedData[byteIndex + 1] = decodedBytes[1];
				}
				if (byteIndex + 2 < maxLength) {
					decodedData[byteIndex + 2] = decodedBytes[2];
				}
			}
			byteIndex += 3;
		}
	}
	if (encodedByteIndex > 0) {
		decodedBytes[0] = encodedBytes[0] << 2;
		if (encodedByteIndex > 1) {
			decodedBytes[0] |= encodedBytes[1] >> 4;
			decodedBytes[1] = encodedBytes[1] << 4;
		}
		if (encodedByteIndex > 2) {
			decodedBytes[1] |= encodedBytes[2] >> 2;
		}
		if (decodedData != NULL) {
			if (byteIndex < maxLength) {
				decodedData[byteIndex] = decodedBytes[0];
			}
			if (byteIndex + 1 < maxLength && encodedByteIndex > 2) {
				decodedData[byteIndex + 1] = decodedBytes[1];
			}
		}
		byteIndex++;
		if (encodedByteIndex > 2) {
			byteIndex++;
		}
	}
	
	if (decodedData != NULL && byteIndex < maxLength) {
		decodedData[byteIndex] = 0;
	}
	return byteIndex;
}
