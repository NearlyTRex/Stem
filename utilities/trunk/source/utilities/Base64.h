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

#ifndef __Base64_H__
#define __Base64_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

// If outEncodedString/outDecodedData is NULL, these functions return the number of bytes required.
// If maxLength is less than the number of bytes required and outEncodedString is not NULL, encodeBase64
// will ensure that it is null-terminated. However, decodeBase64 does not, since data is not assumed to be a string.
size_t encodeBase64(const void * data, size_t dataLength, char * outEncodedString, size_t maxLength);
size_t decodeBase64(const char * encodedString, size_t stringLength, void * outDecodedData, size_t maxLength);

#ifdef __cplusplus
}
#endif
#endif
