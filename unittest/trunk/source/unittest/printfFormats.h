/*
  Copyright (c) 2013 Alex Diener
  
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

#ifndef __printfFormats_H__
#define __printfFormats_H__

#ifdef WIN32
#define SIZE_T_FORMAT "%Iu"
#define INT64_FORMAT "%I64d"
#define UINT64_FORMAT "%I64u"
#define XINT64_FORMAT "%I64X"
#else
#define SIZE_T_FORMAT "%zu"
#if defined(linux) && defined(_LP64)
#define INT64_FORMAT "%ld"
#define UINT64_FORMAT "%lu"
#define XINT64_FORMAT "%lX"
#else
#define INT64_FORMAT "%lld"
#define UINT64_FORMAT "%llu"
#define XINT64_FORMAT "%llX"
#endif
#endif

#endif
