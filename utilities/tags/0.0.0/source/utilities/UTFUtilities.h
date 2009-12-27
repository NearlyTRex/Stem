/*
  Copyright (C) 2009 Alex Diener

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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

  Alex Diener adiener@sacredsoftware.net
*/

// This API provides utility functions for working with and converting between
// Unicode string representations. All UTF-16 and UTF-32 strings are expected to
// be in the host platform's native endianness.
// For more information:
// - http://en.wikipedia.org/wiki/UTF-8
// - http://en.wikipedia.org/wiki/UTF-16
// - http://en.wikipedia.org/wiki/UTF-32

#ifndef __UTF_UTILITIES_H__
#define __UTF_UTILITIES_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Functions for measuring strings (equivalent to strlen).
size_t utf8StringLength(const  uint8_t  * utf8String);
size_t utf16StringLength(const uint16_t * utf16String);
size_t utf32StringLength(const uint32_t * utf32String);

// Functions for checking the well-formedness of Unicode strings.
bool utf8StringIsWellFormed(const  uint8_t  * utf8String,  size_t length);
bool utf16StringIsWellFormed(const uint16_t * utf16String, size_t length);
bool utf32StringIsWellFormed(const uint32_t * utf32String, size_t length);

// Functions for determining how long strings would be in different encodings.
size_t utf8StringUTF16Length(const  uint8_t  * utf8String,  size_t length);
size_t utf8StringUTF32Length(const  uint8_t  * utf8String,  size_t length);
size_t utf16StringUTF8Length(const  uint16_t * utf16String, size_t length);
size_t utf16StringUTF32Length(const uint16_t * utf16String, size_t length);
size_t utf32StringUTF8Length(const  uint32_t * utf32String, size_t length);
size_t utf32StringUTF16Length(const uint32_t * utf32String, size_t length);

// Simple functions for converting between encodings. The returned pointer must be
// freed by the caller. If any malformed characters are encountered, they are
// represented in the converted string as the Unicode Replacement Character (U+FFFD).
uint16_t * utf8StringToUTF16String(const  uint8_t  * utf8String,  size_t length);
uint32_t * utf8StringToUTF32String(const  uint8_t  * utf8String,  size_t length);
uint8_t  * utf16StringToUTF8String(const  uint16_t * utf16String, size_t length);
uint32_t * utf16StringToUTF32String(const uint16_t * utf16String, size_t length);
uint8_t  * utf32StringToUTF8String(const  uint32_t * utf32String, size_t length);
uint16_t * utf32StringToUTF16String(const uint32_t * utf32String, size_t length);

// Extended functions for converting between encodings. The return value is always
// the length of the string in the specified encoding. These can be used for a
// variety of purposes:
// - If the outUTF*String parameter is NULL, nothing is written, but the function
//   still returns the length of the input string in the requested encoding. This
//   can be used to measure the space needed for the conversion to take place.
// - If the outUTF*String parameter is non-NULL, the converted string is written
//   to it. This MUST be large enough to contain the entire transcoded input string,
//   plus a trailing NUL byte; in other words, it must be at least as large as
//   sizeof(uint<target>_t) * (utf*StringToUTF*String(source, length, NULL, NULL) + 1).
// - If the outMalformed parameter is non-NULL, a boolean value indicating the
//   malformedness of the input string is written to it.
size_t utf8StringToUTF32StringExtended(const uint8_t * utf8String, size_t utf8Length, uint32_t * outUTF32String, bool * outMalformed);
size_t utf32StringToUTF8StringExtended(const uint32_t * utf32String, size_t utf32Length, uint8_t * outUTF8String, bool * outMalformed);
size_t utf16StringToUTF32StringExtended(const uint16_t * utf16String, size_t utf16Length, uint32_t * outUTF32String, bool * outMalformed);
size_t utf32StringToUTF16StringExtended(const uint32_t * utf32String, size_t utf32Length, uint16_t * outUTF16String, bool * outMalformed);
size_t utf8StringToUTF16StringExtended(const uint8_t * utf8String, size_t utf8Length, uint16_t * outUTF16String, bool * outMalformed);
size_t utf16StringToUTF8StringExtended(const uint16_t * utf16String, size_t utf16Length, uint8_t * outUTF8String, bool * outMalformed);

#endif
