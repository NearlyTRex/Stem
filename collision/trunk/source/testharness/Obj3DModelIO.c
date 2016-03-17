#include "gamemath/Vector3f.h"
#include "gamemath/Vector3i.h"
#include "gamemath/Vector3x.h"
#include "gamemath/VectorConversions.h"
#include "testharness/Obj3DModelIO.h"
#include "utilities/IOUtilities.h"
#include <math.h>
#include <string.h>

enum ObjToken {
	OBJ_TOKEN_UNKNOWN = -1,
	OBJ_TOKEN_COMMENT,       // #
	OBJ_TOKEN_VERTEX,        // v
	OBJ_TOKEN_NORMAL,        // vn
	OBJ_TOKEN_TEXTURE_COORD, // vt
	OBJ_TOKEN_FACE,          // f
	OBJ_TOKEN_MTLLIB,        // mtllib
	OBJ_TOKEN_USEMTL,        // usemtl
	OBJ_TOKEN_OBJECT,        // o
	OBJ_TOKEN_GROUP,         // g
	OBJ_TOKEN_SMOOTH_SHADING // s
};

static void skipWhitespace(const char * fileContents, size_t fileLength, size_t * ioCharIndex) {
	while (*ioCharIndex < fileLength && (fileContents[*ioCharIndex] == ' ' || fileContents[*ioCharIndex] == '\t')) {
		++*ioCharIndex;
	}
}

static void skipToNextLine(const char * fileContents, size_t fileLength, size_t * ioCharIndex) {
	while (*ioCharIndex < fileLength && (fileContents[*ioCharIndex] != '\r' && fileContents[*ioCharIndex] != '\n')) {
		++*ioCharIndex;
	}
	while (*ioCharIndex < fileLength && (fileContents[*ioCharIndex] == '\r' || fileContents[*ioCharIndex] == '\n')) {
		++*ioCharIndex;
	}
}

static enum ObjToken readObjToken(const char * fileContents, size_t fileLength, size_t * ioCharIndex) {
	size_t lastCharIndex = *ioCharIndex, length;
	
	if (fileContents[lastCharIndex] == '#') {
		++*ioCharIndex;
		return OBJ_TOKEN_COMMENT;
	}
	
	while (*ioCharIndex < fileLength && (fileContents[*ioCharIndex] != ' ' && fileContents[*ioCharIndex] != '\t' && fileContents[*ioCharIndex] != '\r' && fileContents[*ioCharIndex] != '\n')) {
		++*ioCharIndex;
	}
	length = *ioCharIndex - lastCharIndex;
	
	if (length == 1 && fileContents[lastCharIndex] == 'v') {
		return OBJ_TOKEN_VERTEX;
	}
	if (length == 2 && !strncmp(fileContents + lastCharIndex, "vn", length)) {
		return OBJ_TOKEN_NORMAL;
	}
	if (length == 2 && !strncmp(fileContents + lastCharIndex, "vt", length)) {
		return OBJ_TOKEN_TEXTURE_COORD;
	}
	if (length == 1 && fileContents[lastCharIndex] == 'f') {
		return OBJ_TOKEN_FACE;
	}
	if (length == 6 && !strncmp(fileContents + lastCharIndex, "mtllib", length)) {
		return OBJ_TOKEN_MTLLIB;
	}
	if (length == 6 && !strncmp(fileContents + lastCharIndex, "usemtl", length)) {
		return OBJ_TOKEN_USEMTL;
	}
	if (length == 1 && fileContents[lastCharIndex] == 'o') {
		return OBJ_TOKEN_OBJECT;
	}
	if (length == 1 && fileContents[lastCharIndex] == 'g') {
		return OBJ_TOKEN_GROUP;
	}
	if (length == 1 && fileContents[lastCharIndex] == 's') {
		return OBJ_TOKEN_SMOOTH_SHADING;
	}
	
	return OBJ_TOKEN_UNKNOWN;
}

static float readFloat(const char * fileContents, size_t fileLength, size_t * ioCharIndex) {
	char * endPtr;
	float value;
	
	value = strtof(fileContents + *ioCharIndex, &endPtr);
	if (endPtr == fileContents + *ioCharIndex) {
		return nanf("");
	}
	
	*ioCharIndex += endPtr - (fileContents + *ioCharIndex);
	return value;
}

static int readInt(const char * fileContents, size_t fileLength, size_t * ioCharIndex) {
	char * endPtr;
	int value;
	
	value = (int) strtol(fileContents + *ioCharIndex, &endPtr, 10);
	if (endPtr == fileContents + *ioCharIndex) {
		return 0;
	}
	
	*ioCharIndex += endPtr - (fileContents + *ioCharIndex);
	return value;
}

static void readFaceVertex(const char * fileContents, size_t fileLength, size_t * ioCharIndex, int * outFaceVertex) {
	outFaceVertex[0] = readInt(fileContents, fileLength, ioCharIndex);
	
	if (fileContents[*ioCharIndex] == '/') {
		++*ioCharIndex;
		
		if (fileContents[*ioCharIndex] == '/') {
			outFaceVertex[1] = 0;
		} else {
			outFaceVertex[1] = readInt(fileContents, fileLength, ioCharIndex);
		}
		if (fileContents[*ioCharIndex] == '/') {
			++*ioCharIndex;
			outFaceVertex[2] = readInt(fileContents, fileLength, ioCharIndex);
		} else {
			outFaceVertex[2] = 0;
		}
		
	} else {
		outFaceVertex[1] = 0;
		outFaceVertex[2] = 0;
	}
}

CollisionStaticTrimesh * loadObjStaticModelFile(const char * filePath) {
	char * fileContents;
	size_t fileLength = 0;
	size_t charIndex = 0;
	size_t positionCount = 0, positionAllocatedCount = 1024;
	Vector3f * positions = malloc(sizeof(Vector3f) * positionAllocatedCount);
	size_t faceCount = 0, faceAllocatedCount = 1024;
	int * faces = malloc(sizeof(int) * faceAllocatedCount * 9);
	int face[9];
	Vector3x * vertices;
	unsigned int * indexes;
	CollisionStaticTrimesh * trimesh;
	
	fileContents = readFileSimple(filePath, &fileLength);
	if (fileContents == NULL) {
		return NULL;
	}
	
	// TODO: This code is unsafe for malformed obj files
	for (;;) {
		enum ObjToken token;
		Vector3f position;
		
		skipWhitespace(fileContents, fileLength, &charIndex);
		if (charIndex >= fileLength) {
			break;
		}
		
		token = readObjToken(fileContents, fileLength, &charIndex);
		switch (token) {
			case OBJ_TOKEN_COMMENT:
			case OBJ_TOKEN_MTLLIB:
			case OBJ_TOKEN_USEMTL:
			case OBJ_TOKEN_OBJECT:
			case OBJ_TOKEN_GROUP:
			case OBJ_TOKEN_SMOOTH_SHADING:
			case OBJ_TOKEN_TEXTURE_COORD: // TODO: Load texture coords
			case OBJ_TOKEN_UNKNOWN:
				skipToNextLine(fileContents, fileLength, &charIndex);
				break;
				
			case OBJ_TOKEN_VERTEX:
				skipWhitespace(fileContents, fileLength, &charIndex);
				position.x = readFloat(fileContents, fileLength, &charIndex);
				skipWhitespace(fileContents, fileLength, &charIndex);
				position.y = readFloat(fileContents, fileLength, &charIndex);
				skipWhitespace(fileContents, fileLength, &charIndex);
				position.z = readFloat(fileContents, fileLength, &charIndex);
				
				if (positionCount >= positionAllocatedCount) {
					positionAllocatedCount *= 2;
					positions = realloc(positions, sizeof(Vector3f) * positionAllocatedCount);
				}
				positions[positionCount++] = position;
				
				skipToNextLine(fileContents, fileLength, &charIndex);
				break;
				
			case OBJ_TOKEN_NORMAL:/*
				skipWhitespace(fileContents, fileLength, &charIndex);
				normal.x = readFloat(fileContents, fileLength, &charIndex);
				skipWhitespace(fileContents, fileLength, &charIndex);
				normal.y = readFloat(fileContents, fileLength, &charIndex);
				skipWhitespace(fileContents, fileLength, &charIndex);
				normal.z = readFloat(fileContents, fileLength, &charIndex);
				
				if (normalCount >= normalAllocatedCount) {
					normalAllocatedCount *= 2;
					normals = realloc(normals, sizeof(Vector3f) * normalAllocatedCount);
				}
				normals[normalCount++] = normal;
				*/
				skipToNextLine(fileContents, fileLength, &charIndex);
				break;
				
			case OBJ_TOKEN_FACE:
				skipWhitespace(fileContents, fileLength, &charIndex);
				readFaceVertex(fileContents, fileLength, &charIndex, face);
				skipWhitespace(fileContents, fileLength, &charIndex);
				readFaceVertex(fileContents, fileLength, &charIndex, face + 3);
				skipWhitespace(fileContents, fileLength, &charIndex);
				readFaceVertex(fileContents, fileLength, &charIndex, face + 6);
				
				if (faceCount >= faceAllocatedCount) {
					faceAllocatedCount *= 2;
					faces = realloc(faces, sizeof(int) * faceAllocatedCount * 9);
				}
				faces[faceCount * 9 + 0] = face[0];
				faces[faceCount * 9 + 1] = face[1];
				faces[faceCount * 9 + 2] = face[2];
				faces[faceCount * 9 + 3] = face[3];
				faces[faceCount * 9 + 4] = face[4];
				faces[faceCount * 9 + 5] = face[5];
				faces[faceCount * 9 + 6] = face[6];
				faces[faceCount * 9 + 7] = face[7];
				faces[faceCount * 9 + 8] = face[8];
				faceCount++;
				
				skipWhitespace(fileContents, fileLength, &charIndex);
				while (fileContents[charIndex] != '\n' && fileContents[charIndex] != '\r' && fileContents[charIndex] != '#') {
					face[3] = face[6];
					face[4] = face[7];
					face[5] = face[8];
					readFaceVertex(fileContents, fileLength, &charIndex, face + 6);
					
					if (faceCount >= faceAllocatedCount) {
						faceAllocatedCount *= 2;
						faces = realloc(faces, sizeof(int) * faceAllocatedCount * 9);
					}
					faces[faceCount * 9 + 0] = face[0];
					faces[faceCount * 9 + 1] = face[1];
					faces[faceCount * 9 + 2] = face[2];
					faces[faceCount * 9 + 3] = face[3];
					faces[faceCount * 9 + 4] = face[4];
					faces[faceCount * 9 + 5] = face[5];
					faces[faceCount * 9 + 6] = face[6];
					faces[faceCount * 9 + 7] = face[7];
					faces[faceCount * 9 + 8] = face[8];
					faceCount++;
					
					skipWhitespace(fileContents, fileLength, &charIndex);
				}
				skipToNextLine(fileContents, fileLength, &charIndex);
				
				break;
		}
	}
	
	// TODO: uniq vertex/normal/texCoord triples
	vertices = malloc(sizeof(Vector3x) * faceCount * 3);
	indexes = malloc(sizeof(unsigned int) * faceCount * 3);
	for (size_t faceIndex = 0; faceIndex < faceCount; faceIndex++) {
		for (size_t vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
			vertices[faceIndex * 3 + vertexIndex] = Vector3f_toVector3x(positions[faces[faceIndex * 9 + vertexIndex * 3 + 0] - 1]);
			indexes[faceIndex * 3 + vertexIndex] = faceIndex * 3 + vertexIndex;
		}
	}
	
	free(fileContents);
	free(positions);
	//free(normals);
	free(faces);
	
	trimesh = CollisionStaticTrimesh_createWithIndexes(NULL, NULL, vertices, indexes, faceCount * 3);
	free(vertices);
	free(indexes);
	return trimesh;
}
