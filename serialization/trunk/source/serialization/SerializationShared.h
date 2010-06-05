/*
  Copyright (c) 2010 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __SERIALIZATION_SHARED_H__
#define __SERIALIZATION_SHARED_H__

#define enumKV(value) #value, value

// No error
#define SERIALIZATION_ERROR_OK 0

// Incorrect API usage, such as mismatched begin/end structure/dictionary/array calls
#define SERIALIZATION_ERROR_INVALID_OPERATION 1

// The requested key is not present in the current dictionary or structure
#define SERIALIZATION_ERROR_KEY_NOT_FOUND 2

// Other errors defined by concrete serialization modules

#endif
