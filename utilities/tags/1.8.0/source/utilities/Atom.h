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

#ifndef __ATOM_H__
#define __ATOM_H__

typedef const char * Atom;

#define ATOM(string) Atom_fromString(string)

#ifndef MUTEX_TYPE
#ifdef __SHELL_THREADS_H__
#define MUTEX_TYPE ShellMutex;
#else
#define MUTEX_TYPE void *
#endif
#endif

// If you want to safely use Atom in a multithreaded environment, call Atom_initMutex()
// prior to calling any Atom functions from a secondary thread
void Atom_initMutex(MUTEX_TYPE (* createMutexFunc)(void), void (* lockMutexFunc)(MUTEX_TYPE), void (* unlockMutexFunc)(MUTEX_TYPE));

// Returns a unique pointer for each string value, which may or may not be equal to the
// argument. If the string has not previously been seen by Atom_fromString() or
// Atom_registerStaticAddress(), a new entry is added to the global Atom table for it.
// New entries will not return a value equal to the argument.
Atom Atom_fromString(const char * string);

// Adds staticAtom to the global Atom table as the authoritative address of its string
// value. The purpose of this function is to allow the use of known pointer values
// without having to look them up each time with Atom_fromString(). It's recommended
// to use a function marked with __attribute__((constructor)) to call this at the
// earliest possible time in your program's execution.
// 
// Note that it is an error to call Atom_registerStaticAddress with a string value that
// already exists in the global Atom table. If this happens, the duplicate string value
// will be printed to stderr, and Atom_registerStaticAddress calls abort().
// 
// This function is not thread safe. Call it before spawning any secondary threads.
void Atom_registerStaticAddress(const char * staticAtom);

#endif
