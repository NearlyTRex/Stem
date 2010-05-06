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

#include "utilities/Atom.h"

#include <stdlib.h>
#include <string.h>

static size_t numberOfAtoms = 0;
static size_t atomListSize = 1;
static Atom * atoms = NULL;

Atom Atom_fromString(const char * string) {
	unsigned int atomIndex;
	char * newAtom;
	size_t length;
	
	if (atoms == NULL) {
		atoms = malloc(sizeof(Atom) * atomListSize);
	}
	
	for (atomIndex = 0; atomIndex < numberOfAtoms; atomIndex++) {
		if (!strcmp(atoms[atomIndex], string)) {
			return atoms[atomIndex];
		}
	}
	
	if (numberOfAtoms >= atomListSize) {
		atomListSize *= 2;
		atoms = realloc(atoms, sizeof(Atom) * atomListSize);
	}
	length = strlen(string);
	newAtom = malloc(length + 1);
	strcpy(newAtom, string);
	newAtom[length] = '\x00';
	atoms[numberOfAtoms++] = newAtom;
	
	return newAtom;
}
