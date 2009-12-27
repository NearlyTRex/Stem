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

#include "utilities/UTFUtilities.h"

#include <string.h>

size_t utf8StringLength(const uint8_t * utf8String) {
	return strlen((const char *) utf8String);
}

size_t utf16StringLength(const uint16_t * utf16String) {
	size_t charIndex;
	
	for (charIndex = 0; utf16String[charIndex] != 0x0000; charIndex++) {
	}
	return charIndex;
}

size_t utf32StringLength(const uint32_t * utf32String) {
	size_t charIndex;
	
	for (charIndex = 0; utf32String[charIndex] != 0x00000000; charIndex++) {
	}
	return charIndex;
}

bool utf8StringIsWellFormed(const uint8_t * utf8String, size_t length) {
	bool malformed;
	
	utf8StringToUTF32StringExtended(utf8String, length, NULL, &malformed);
	return !malformed;
}

bool utf16StringIsWellFormed(const uint16_t * utf16String, size_t length) {
	bool malformed;
	
	utf16StringToUTF32StringExtended(utf16String, length, NULL, &malformed);
	return !malformed;
}

bool utf32StringIsWellFormed(const uint32_t * utf32String, size_t length) {
	bool malformed;
	
	utf32StringToUTF8StringExtended(utf32String, length, NULL, &malformed);
	return !malformed;
}

size_t utf8StringUTF16Length(const uint8_t * utf8String, size_t length) {
	return utf8StringToUTF16StringExtended(utf8String, length, NULL, NULL);
}

size_t utf8StringUTF32Length(const uint8_t * utf8String, size_t length) {
	return utf8StringToUTF32StringExtended(utf8String, length, NULL, NULL);
}

size_t utf16StringUTF8Length(const uint16_t * utf16String, size_t length) {
	return utf16StringToUTF8StringExtended(utf16String, length, NULL, NULL);
}

size_t utf16StringUTF32Length(const uint16_t * utf16String, size_t length) {
	return utf16StringToUTF32StringExtended(utf16String, length, NULL, NULL);
}

size_t utf32StringUTF8Length(const uint32_t * utf32String, size_t length) {
	return utf32StringToUTF8StringExtended(utf32String, length, NULL, NULL);
}

size_t utf32StringUTF16Length(const uint32_t * utf32String, size_t length) {
	return utf32StringToUTF16StringExtended(utf32String, length, NULL, NULL);
}

uint16_t * utf8StringToUTF16String(const uint8_t * utf8String, size_t length) {
	uint16_t * utf16String;
	int utf16Length;
	
	utf16Length = utf8StringUTF16Length(utf8String, length);
	utf16String = malloc(sizeof(uint16_t) * (utf16Length + 1));
	utf8StringToUTF16StringExtended(utf8String, length, utf16String, NULL);
	
	return utf16String;
}

uint32_t * utf8StringToUTF32String(const uint8_t * utf8String, size_t length) {
	uint32_t * utf32String;
	int utf32Length;
	
	utf32Length = utf8StringUTF32Length(utf8String, length);
	utf32String = malloc(sizeof(uint32_t) * (utf32Length + 1));
	utf8StringToUTF32StringExtended(utf8String, length, utf32String, NULL);
	
	return utf32String;
}

uint8_t * utf16StringToUTF8String(const uint16_t * utf16String, size_t length) {
	uint8_t * utf8String;
	int utf8Length;
	
	utf8Length = utf16StringUTF8Length(utf16String, length);
	utf8String = malloc(sizeof(uint8_t) * (utf8Length + 1));
	utf16StringToUTF8StringExtended(utf16String, length, utf8String, NULL);
	
	return utf8String;
}

uint32_t * utf16StringToUTF32String(const uint16_t * utf16String, size_t length) {
	uint32_t * utf32String;
	int utf32Length;
	
	utf32Length = utf16StringUTF32Length(utf16String, length);
	utf32String = malloc(sizeof(uint32_t) * (utf32Length + 1));
	utf16StringToUTF32StringExtended(utf16String, length, utf32String, NULL);
	
	return utf32String;
}

uint8_t * utf32StringToUTF8String(const uint32_t * utf32String, size_t length) {
	uint8_t * utf8String;
	int utf8Length;
	
	utf8Length = utf32StringUTF8Length(utf32String, length);
	utf8String = malloc(sizeof(uint8_t) * (utf8Length + 1));
	utf32StringToUTF8StringExtended(utf32String, length, utf8String, NULL);
	
	return utf8String;
}

uint16_t * utf32StringToUTF16String(const uint32_t * utf32String, size_t length) {
	uint16_t * utf16String;
	int utf16Length;
	
	utf16Length = utf32StringUTF16Length(utf32String, length);
	utf16String = malloc(sizeof(uint16_t) * (utf16Length + 1));
	utf32StringToUTF16StringExtended(utf32String, length, utf16String, NULL);
	
	return utf16String;
}

size_t utf8StringToUTF32StringExtended(const uint8_t * utf8String, size_t utf8Length, uint32_t * outUTF32String, bool * outMalformed) {
	size_t charIndex;
	size_t utf32Length;
	bool inMultibyteSequence;
	size_t rememberedCharIndex = 0;
	bool malformed = false;
	
	utf32Length = 0;
	inMultibyteSequence = false;
	for (charIndex = 0; charIndex < utf8Length; charIndex++) {
		if (!inMultibyteSequence) {
			if (utf8String[charIndex] < 0x80) {
				if (outUTF32String != NULL) {
					outUTF32String[utf32Length] = utf8String[charIndex];
				}
				utf32Length++;
				
			} else {
				if ((utf8String[charIndex] & 0xE0) == 0xC0 ||
				    (utf8String[charIndex] & 0xF0) == 0xE0 ||
				    (utf8String[charIndex] & 0xF8) == 0xF0) {
					inMultibyteSequence = true;
					rememberedCharIndex = charIndex;
				} else {
					// Malformed due to incorrect multibyte sequence start indicator
					malformed = true;
					if (outUTF32String != NULL) {
						outUTF32String[utf32Length] = 0x0000FFFD;
					}
					utf32Length++;
				}
			}
			
		} else {
			if ((utf8String[rememberedCharIndex] & 0xE0) == 0xC0) {
				if ((utf8String[charIndex] & 0xC0) == 0x80) {
					if (outUTF32String != NULL) {
						outUTF32String[utf32Length] = ((utf8String[charIndex - 1] & 0x1F) << 6) |
						                               (utf8String[charIndex - 0] & 0x3F);
					}
					utf32Length++;
				} else {
					// Malformed due to incorrect second byte of two-byte sequence
					malformed = true;
					if (outUTF32String != NULL) {
						outUTF32String[utf32Length] = 0x0000FFFD;
					}
					utf32Length++;
				}
				inMultibyteSequence = false;
				
			} else if ((utf8String[rememberedCharIndex] & 0xF0) == 0xE0) {
				if ((utf8String[charIndex] & 0xC0) == 0x80) {
					if (charIndex - rememberedCharIndex == 2) {
						if (outUTF32String != NULL) {
							outUTF32String[utf32Length] = ((utf8String[charIndex - 2] & 0x0F) << 12) |
							                              ((utf8String[charIndex - 1] & 0x3F) << 6) |
							                               (utf8String[charIndex - 0] & 0x3F);
						}
						utf32Length++;
						inMultibyteSequence = false;
					}
				} else {
					// Malformed due to incorrect second or third byte of three-byte sequence
					malformed = true;
					inMultibyteSequence = false;
					if (outUTF32String != NULL) {
						outUTF32String[utf32Length] = 0x0000FFFD;
					}
					utf32Length++;
				}
				
			} else if ((utf8String[rememberedCharIndex] & 0xF8) == 0xF0) {
				if ((utf8String[charIndex] & 0xC0) == 0x80) {
					if (charIndex - rememberedCharIndex == 3) {
						if (outUTF32String != NULL) {
							outUTF32String[utf32Length] = ((utf8String[charIndex - 3] & 0x07) << 18) |
							                              ((utf8String[charIndex - 2] & 0x3F) << 12) |
							                              ((utf8String[charIndex - 1] & 0x3F) << 6) |
							                               (utf8String[charIndex - 0] & 0x3F);
						}
						utf32Length++;
						inMultibyteSequence = false;
					}
				} else {
					// Malformed due to incorrect second, third, or fourth byte of four-byte sequence
					malformed = true;
					inMultibyteSequence = false;
					if (outUTF32String != NULL) {
						outUTF32String[utf32Length] = 0x0000FFFD;
					}
					utf32Length++;
				}
			}
		}
	}
	if (inMultibyteSequence) {
		// Malformed due to unterminated multibyte sequence
		malformed = true;
		if (outUTF32String != NULL) {
			outUTF32String[utf32Length] = 0x0000FFFD;
		}
		utf32Length++;
	}
	if (outUTF32String != NULL) {
		outUTF32String[utf32Length] = 0x00000000;
	}
	
	if (outMalformed != NULL) {
		*outMalformed = malformed;
	}
	return utf32Length;
}

size_t utf32StringToUTF8StringExtended(const uint32_t * utf32String, size_t utf32Length, uint8_t * outUTF8String, bool * outMalformed) {
	size_t charIndex;
	size_t utf8Length;
	bool malformed = false;
	
	utf8Length = 0;
	for (charIndex = 0; charIndex < utf32Length; charIndex++) {
		if (utf32String[charIndex] <= 0x0000007F) {
			if (outUTF8String != NULL) {
				outUTF8String[utf8Length] = utf32String[charIndex];
			}
			utf8Length++;
			
		} else if (utf32String[charIndex] <= 0x000007FF) {
			if (outUTF8String != NULL) {
				outUTF8String[utf8Length + 0] = 0xC0 | ((utf32String[charIndex] >> 6) & 0x1F);
				outUTF8String[utf8Length + 1] = 0x80 |  (utf32String[charIndex]       & 0x3F);
			}
			utf8Length += 2;
			
		} else if (utf32String[charIndex] <= 0x0000D7FF || (utf32String[charIndex] >= 0x0000E000 && utf32String[charIndex] <= 0x0000FFFF)) {
			if (outUTF8String != NULL) {
				outUTF8String[utf8Length + 0] = 0xE0 | ((utf32String[charIndex] >> 12) & 0x0F);
				outUTF8String[utf8Length + 1] = 0x80 | ((utf32String[charIndex] >> 6)  & 0x3F);
				outUTF8String[utf8Length + 2] = 0x80 |  (utf32String[charIndex]        & 0x3F);
			}
			utf8Length += 3;
			
		} else if (utf32String[charIndex] < 0x0000E000) {
			// Malformed due to code point in the reserved U+D800-U+DFFF range
			malformed = true;
			if (outUTF8String != NULL) {
				outUTF8String[utf8Length + 0] = 0xEF;
				outUTF8String[utf8Length + 1] = 0xBF;
				outUTF8String[utf8Length + 2] = 0xBD;
			}
			utf8Length += 3;
			
		} else if (utf32String[charIndex] <= 0x0010FFFF) {
			if (outUTF8String != NULL) {
				outUTF8String[utf8Length + 0] = 0xF0 | ((utf32String[charIndex] >> 18) & 0x07);
				outUTF8String[utf8Length + 1] = 0x80 | ((utf32String[charIndex] >> 12) & 0x3F);
				outUTF8String[utf8Length + 2] = 0x80 | ((utf32String[charIndex] >> 6)  & 0x3F);
				outUTF8String[utf8Length + 3] = 0x80 |  (utf32String[charIndex]        & 0x3F);
			}
			utf8Length += 4;
			
		} else {
			// Malformed due to code point beyond the upper limit of Unicode
			malformed = true;
			if (outUTF8String != NULL) {
				outUTF8String[utf8Length + 0] = 0xEF;
				outUTF8String[utf8Length + 1] = 0xBF;
				outUTF8String[utf8Length + 2] = 0xBD;
			}
			utf8Length += 3;
		}
	}
	if (outUTF8String != NULL) {
		outUTF8String[utf8Length] = 0x00;
	}
	
	if (outMalformed != NULL) {
		*outMalformed = malformed;
	}
	return utf8Length;
}

size_t utf16StringToUTF32StringExtended(const uint16_t * utf16String, size_t utf16Length, uint32_t * outUTF32String, bool * outMalformed) {
	size_t charIndex;
	size_t utf32Length;
	bool inSurrogatePair;
	bool malformed = false;
	
	utf32Length = 0;
	inSurrogatePair = false;
	for (charIndex = 0; charIndex < utf16Length; charIndex++) {
		if (inSurrogatePair) {
			if (utf16String[charIndex] >= 0xDC00 && utf16String[charIndex] <= 0xDFFF) {
				if (outUTF32String != NULL) {
					outUTF32String[utf32Length] = (((utf16String[charIndex - 1] & 0x03FF) << 10) |
					                                (utf16String[charIndex - 0] & 0x03FF)) +
					                              0x00010000;
				}
				utf32Length++;
				
			} else {
				// Malformed due to mismatched first character of surrogate pair
				malformed = true;
				if (outUTF32String != NULL) {
					outUTF32String[utf32Length] = 0x0000FFFD;
				}
				utf32Length++;
			}
			inSurrogatePair = false;
			
		} else {
			if (utf16String[charIndex] >= 0xD800 && utf16String[charIndex] <= 0xDBFF) {
				inSurrogatePair = true;
				
			} else if (utf16String[charIndex] >= 0xDC00 && utf16String[charIndex] <= 0xDFFF) {
				// Malformed due to mismatched second character of surrogate pair
				malformed = true;
				if (outUTF32String != NULL) {
					outUTF32String[utf32Length] = 0x0000FFFD;
				}
				utf32Length++;
				
			} else {
				if (outUTF32String != NULL) {
					outUTF32String[utf32Length] = utf16String[charIndex];
				}
				utf32Length++;
			}
		}
	}
	if (inSurrogatePair) {
		// Malformed due to unterminated surrogate pair
		malformed = true;
		if (outUTF32String != NULL) {
			outUTF32String[utf32Length] = 0x0000FFFD;
		}
		utf32Length++;
	}
	if (outUTF32String != NULL) {
		outUTF32String[utf32Length] = 0x00000000;
	}
	
	if (outMalformed != NULL) {
		*outMalformed = malformed;
	}
	return utf32Length;
}

size_t utf32StringToUTF16StringExtended(const uint32_t * utf32String, size_t utf32Length, uint16_t * outUTF16String, bool * outMalformed) {
	size_t charIndex;
	size_t utf16Length;
	bool malformed = false;
	
	utf16Length = 0;
	for (charIndex = 0; charIndex < utf32Length; charIndex++) {
		if (utf32String[charIndex] <= 0x0000D7FF || (utf32String[charIndex] >= 0x0000E000 && utf32String[charIndex] <= 0x0000FFFF)) {
			if (outUTF16String != NULL) {
				outUTF16String[utf16Length] = utf32String[charIndex];
			}
			utf16Length++;
			
		} else if (utf32String[charIndex] < 0x0000E000) {
			// Malformed due to code point in the reserved U+D800-U+DFFF range
			malformed = true;
			if (outUTF16String != NULL) {
				outUTF16String[utf16Length] = 0xFFFD;
			}
			utf16Length++;
			
		} else if (utf32String[charIndex] <= 0x0010FFFF) {
			if (outUTF16String != NULL) {
				outUTF16String[utf16Length + 0] = 0xD800 | ((utf32String[charIndex] - 0x00010000) >> 10);
				outUTF16String[utf16Length + 1] = 0xDC00 |  (utf32String[charIndex] & 0x03FF);
			}
			utf16Length += 2;
			
		} else {
			// Malformed due to code point beyond the upper limit of Unicode
			malformed = true;
			if (outUTF16String != NULL) {
				outUTF16String[utf16Length] = 0xFFFD;
			}
			utf16Length++;
		}
	}
	if (outUTF16String != NULL) {
		outUTF16String[utf16Length] = 0x0000;
	}
	
	if (outMalformed != NULL) {
		*outMalformed = malformed;
	}
	return utf16Length;
}

size_t utf8StringToUTF16StringExtended(const uint8_t * utf8String, size_t utf8Length, uint16_t * outUTF16String, bool * outMalformed) {
	size_t charIndex;
	size_t utf16Length;
	bool inMultibyteSequence;
	size_t rememberedCharIndex = 0;
	bool malformed = false;
	
	utf16Length = 0;
	inMultibyteSequence = false;
	for (charIndex = 0; charIndex < utf8Length; charIndex++) {
		if (!inMultibyteSequence) {
			if (utf8String[charIndex] < 0x80) {
				if (outUTF16String != NULL) {
					outUTF16String[utf16Length] = utf8String[charIndex];
				}
				utf16Length++;
				
			} else {
				if ((utf8String[charIndex] & 0xE0) == 0xC0 ||
				    (utf8String[charIndex] & 0xF0) == 0xE0 ||
				    (utf8String[charIndex] & 0xF8) == 0xF0) {
					inMultibyteSequence = true;
					rememberedCharIndex = charIndex;
				} else {
					// Malformed due to incorrect multibyte sequence start indicator
					malformed = true;
					if (outUTF16String != NULL) {
						outUTF16String[utf16Length] = 0xFFFD;
					}
					utf16Length++;
				}
			}
			
		} else {
			if ((utf8String[rememberedCharIndex] & 0xE0) == 0xC0) {
				if ((utf8String[charIndex] & 0xC0) == 0x80) {
					if (outUTF16String != NULL) {
						outUTF16String[utf16Length] = ((utf8String[charIndex - 1] & 0x1F) << 6) |
						                               (utf8String[charIndex - 0] & 0x3F);
					}
					utf16Length++;
				} else {
					// Malformed due to incorrect second byte of two-byte sequence
					malformed = true;
					if (outUTF16String != NULL) {
						outUTF16String[utf16Length] = 0xFFFD;
					}
					utf16Length++;
				}
				inMultibyteSequence = false;
				
			} else if ((utf8String[rememberedCharIndex] & 0xF0) == 0xE0) {
				if ((utf8String[charIndex] & 0xC0) == 0x80) {
					if (charIndex - rememberedCharIndex == 2) {
						if (outUTF16String != NULL) {
							outUTF16String[utf16Length] = ((utf8String[charIndex - 2] & 0x0F) << 12) |
							                              ((utf8String[charIndex - 1] & 0x3F) << 6) |
							                               (utf8String[charIndex - 0] & 0x3F);
						}
						utf16Length++;
						inMultibyteSequence = false;
					}
				} else {
					// Malformed due to incorrect second or third byte of three-byte sequence
					malformed = true;
					inMultibyteSequence = false;
					if (outUTF16String != NULL) {
						outUTF16String[utf16Length] = 0xFFFD;
					}
					utf16Length++;
				}
				
			} else if ((utf8String[rememberedCharIndex] & 0xF8) == 0xF0) {
				if ((utf8String[charIndex] & 0xC0) == 0x80) {
					if (charIndex - rememberedCharIndex == 3) {
						if (outUTF16String != NULL) {
							uint32_t utf32Char;
							
							utf32Char = ((utf8String[charIndex - 3] & 0x07) << 18) |
							            ((utf8String[charIndex - 2] & 0x3F) << 12) |
							            ((utf8String[charIndex - 1] & 0x3F) << 6) |
							             (utf8String[charIndex - 0] & 0x3F);
							
							outUTF16String[utf16Length + 0] = 0xD800 | ((utf32Char - 0x00010000) >> 10);
							outUTF16String[utf16Length + 1] = 0xDC00 |  (utf32Char & 0x03FF);
						}
						utf16Length += 2;
						inMultibyteSequence = false;
					}
				} else {
					// Malformed due to incorrect second, third, or fourth byte of four-byte sequence
					malformed = true;
					inMultibyteSequence = false;
					if (outUTF16String != NULL) {
						outUTF16String[utf16Length] = 0xFFFD;
					}
					utf16Length++;
				}
			}
		}
	}
	if (inMultibyteSequence) {
		// Malformed due to unterminated multibyte sequence
		malformed = true;
		if (outUTF16String != NULL) {
			outUTF16String[utf16Length] = 0xFFFD;
		}
		utf16Length++;
	}
	if (outUTF16String != NULL) {
		outUTF16String[utf16Length] = 0x0000;
	}
	
	if (outMalformed != NULL) {
		*outMalformed = malformed;
	}
	return utf16Length;
}

size_t utf16StringToUTF8StringExtended(const uint16_t * utf16String, size_t utf16Length, uint8_t * outUTF8String, bool * outMalformed) {
	size_t charIndex;
	size_t utf8Length;
	bool inSurrogatePair;
	bool malformed = false;
	
	utf8Length = 0;
	inSurrogatePair = false;
	for (charIndex = 0; charIndex < utf16Length; charIndex++) {
		if (inSurrogatePair) {
			if (utf16String[charIndex] >= 0xDC00 && utf16String[charIndex] <= 0xDFFF) {
				if (outUTF8String != NULL) {
					uint32_t utf32Char;
					
					utf32Char = (((utf16String[charIndex - 1] & 0x03FF) << 10) |
					              (utf16String[charIndex - 0] & 0x03FF)) +
					            0x00010000;
					
					outUTF8String[utf8Length + 0] = 0xF0 | ((utf32Char >> 18) & 0x07);
					outUTF8String[utf8Length + 1] = 0x80 | ((utf32Char >> 12) & 0x3F);
					outUTF8String[utf8Length + 2] = 0x80 | ((utf32Char >> 6)  & 0x3F);
					outUTF8String[utf8Length + 3] = 0x80 |  (utf32Char        & 0x3F);
				}
				utf8Length += 4;
				
			} else {
				// Malformed due to mismatched first character of surrogate pair
				malformed = true;
				if (outUTF8String != NULL) {
					outUTF8String[utf8Length + 0] = 0xEF;
					outUTF8String[utf8Length + 1] = 0xBF;
					outUTF8String[utf8Length + 2] = 0xBD;
				}
				utf8Length += 3;
			}
			inSurrogatePair = false;
			
		} else {
			if (utf16String[charIndex] <= 0x007F) {
				if (outUTF8String != NULL) {
					outUTF8String[utf8Length] = utf16String[charIndex];
				}
				utf8Length++;
				
			} else if (utf16String[charIndex] <= 0x07FF) {
				if (outUTF8String != NULL) {
					outUTF8String[utf8Length + 0] = 0xC0 | ((utf16String[charIndex] >> 6) & 0x1F);
					outUTF8String[utf8Length + 1] = 0x80 |  (utf16String[charIndex]       & 0x3F);
				}
				utf8Length += 2;
				
			} else if (utf16String[charIndex] <= 0xD7FF || utf16String[charIndex] >= 0xE000) {
				if (outUTF8String != NULL) {
					outUTF8String[utf8Length + 0] = 0xE0 | ((utf16String[charIndex] >> 12) & 0x0F);
					outUTF8String[utf8Length + 1] = 0x80 | ((utf16String[charIndex] >> 6)  & 0x3F);
					outUTF8String[utf8Length + 2] = 0x80 |  (utf16String[charIndex]        & 0x3F);
				}
				utf8Length += 3;
				
			} else if (utf16String[charIndex] <= 0xDBFF) {
				inSurrogatePair = true;
				
			} else if (utf16String[charIndex] < 0xE000) {
				// Malformed due to mismatched second character of surrogate pair
				malformed = true;
				if (outUTF8String != NULL) {
					outUTF8String[utf8Length + 0] = 0xEF;
					outUTF8String[utf8Length + 1] = 0xBF;
					outUTF8String[utf8Length + 2] = 0xBD;
				}
				utf8Length += 3;
			}
		}
	}
	if (outUTF8String != NULL) {
		outUTF8String[utf8Length] = 0x00;
	}
	if (inSurrogatePair) {
		// Malformed due to unterminated surrogate pair
		malformed = true;
		if (outUTF8String != NULL) {
			outUTF8String[utf8Length + 0] = 0xEF;
			outUTF8String[utf8Length + 1] = 0xBF;
			outUTF8String[utf8Length + 2] = 0xBD;
		}
		utf8Length += 3;
	}
	
	if (outMalformed != NULL) {
		*outMalformed = malformed;
	}
	return utf8Length;
}
