/*
  Copyright (c) 2018 Alex Diener
  
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

#ifndef __FileCatalog_H__
#define __FileCatalog_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct FileCatalog FileCatalog;

#include "dynamictypes/HashTable.h"
#include "stemobject/StemObject.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include "utilities/Atom.h"

#define FILECATALOG_FORMAT_VERSION 0
#define FILECATALOG_FORMAT_TYPE "catalog"

#define FileCatalog_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	char * private_ivar(basePath); \
	HashTable * private_ivar(catalog);

stemobject_struct_definition(FileCatalog)

FileCatalog * FileCatalog_create();
bool FileCatalog_init(FileCatalog * self);
void FileCatalog_dispose(FileCatalog * self);

// Sets the base path for file paths returned from FileCatalog_getFilePath(). This is usually the directory that contains a
// file catalog that has been deserialized. The base path will be prepended to all non-absolute paths returned from
// FileCatalog_getFilePath().
void FileCatalog_setBasePath(FileCatalog * self, const char * basePath);

// Adds or updates a path with the specified type/name key
void FileCatalog_setFilePath(FileCatalog * self, Atom type, Atom name, const char * path);

// Returns the path for the specified type/name key. The returned pointer is not owned by the caller and should not be freed.
// If FileCatalog_setBasePath() has been called and the path being referenced is local, the base path will be prepended to
// the path returned from this function. Absolute paths are not affected.
const char * FileCatalog_getFilePath(FileCatalog * self, Atom type, Atom name);

// The pointer returned from the following two functions must be freed by the caller.
// NOTE: Although types and names are Atoms in FileCatalog's API, the returned strings in this list are not. If you want
// to pass them to FileCatalog functions, you must use Atom_fromString() on them first.
const char ** FileCatalog_listTypes(FileCatalog * self, size_t * outCount);

// See notes for FileCatalog_listTypes().
// Returns an empty list if the specified type doesn't exist in this catalog.
const char ** FileCatalog_listNamesForType(FileCatalog * self, Atom type, size_t * outCount);

FileCatalog * FileCatalog_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool FileCatalog_loadSerializedData(FileCatalog * self, compat_type(DeserializationContext *) deserializationContext);
void FileCatalog_serialize(FileCatalog * self, compat_type(SerializationContext *) serializationContext);

#ifdef __cplusplus
}
#endif
#endif
