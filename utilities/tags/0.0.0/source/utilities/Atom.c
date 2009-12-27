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
