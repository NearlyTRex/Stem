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

// If you want to safely use Atom in a multithreaded environment, ensure that
// Atom_initMutex() is called before any other Atom function
void Atom_initMutex(MUTEX_TYPE (* createMutexFunc)(void), void (* lockMutexFunc)(MUTEX_TYPE), void (* unlockMutexFunc)(MUTEX_TYPE));

Atom Atom_fromString(const char * string);

#endif
