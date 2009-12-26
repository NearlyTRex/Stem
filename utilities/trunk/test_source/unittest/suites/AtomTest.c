#include "unittest/framework/TestSuite.h"
#include "utilities/Atom.h"

static void testAtomFromString() {
	char * atom1String1 = "I am an atom", atom1String2[] = "I am an atom", * atom2String = "I am also an atom";
	Atom atom1_1, atom1_2, atom2;
	
	atom1_1 = Atom_fromString(atom1String1);
	TestCase_assert(!strcmp(atom1_1, atom1String1), "Atom \"%s\" didn't match expected string \"%s\"", atom1_1, atom1String1);
	
	atom1_2 = Atom_fromString(atom1String2);
	TestCase_assert(!strcmp(atom1_2, atom1String2), "Atom \"%s\" didn't match expected string \"%s\"", atom1_2, atom1String2);
	TestCase_assert(atom1_1 == atom1_2, "Pointers to the same atom differ: %p != %p", atom1_1, atom1_2);
	
	atom2 = Atom_fromString(atom2String);
	TestCase_assert(!strcmp(atom2, atom2String), "Atom \"%s\" didn't match expected string \"%s\"", atom2, atom2String);
}

TestSuite * AtomTest_suite() {
	return testSuite("AtomTest", testAtomFromString, NULL);
}
