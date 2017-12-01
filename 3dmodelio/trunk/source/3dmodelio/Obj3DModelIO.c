/*
  Copyright (c) 2017 Alex Diener
  
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

#include "3dmodelio/Obj3DModelIO.h"
#include "gamemath/Vector3f.h"
#include "gamemath/Vector3i.h"
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

static void skipWhitespace(const char * data, size_t length, size_t * ioCharIndex) {
	while (*ioCharIndex < length && (data[*ioCharIndex] == ' ' || data[*ioCharIndex] == '\t')) {
		++*ioCharIndex;
	}
}

static void skipToNextLine(const char * data, size_t length, size_t * ioCharIndex) {
	while (*ioCharIndex < length && (data[*ioCharIndex] != '\r' && data[*ioCharIndex] != '\n')) {
		++*ioCharIndex;
	}
	while (*ioCharIndex < length && (data[*ioCharIndex] == '\r' || data[*ioCharIndex] == '\n')) {
		++*ioCharIndex;
	}
}

static enum ObjToken readObjToken(const char * data, size_t length, size_t * ioCharIndex) {
	size_t lastCharIndex = *ioCharIndex, tokenLength;
	
	if (data[lastCharIndex] == '#') {
		++*ioCharIndex;
		return OBJ_TOKEN_COMMENT;
	}
	
	while (*ioCharIndex < length && (data[*ioCharIndex] != ' ' && data[*ioCharIndex] != '\t' && data[*ioCharIndex] != '\r' && data[*ioCharIndex] != '\n')) {
		++*ioCharIndex;
	}
	tokenLength = *ioCharIndex - lastCharIndex;
	
	if (tokenLength == 1 && data[lastCharIndex] == 'v') {
		return OBJ_TOKEN_VERTEX;
	}
	if (tokenLength == 2 && !strncmp(data + lastCharIndex, "vn", tokenLength)) {
		return OBJ_TOKEN_NORMAL;
	}
	if (tokenLength == 2 && !strncmp(data + lastCharIndex, "vt", tokenLength)) {
		return OBJ_TOKEN_TEXTURE_COORD;
	}
	if (tokenLength == 1 && data[lastCharIndex] == 'f') {
		return OBJ_TOKEN_FACE;
	}
	if (tokenLength == 6 && !strncmp(data + lastCharIndex, "mtllib", tokenLength)) {
		return OBJ_TOKEN_MTLLIB;
	}
	if (tokenLength == 6 && !strncmp(data + lastCharIndex, "usemtl", tokenLength)) {
		return OBJ_TOKEN_USEMTL;
	}
	if (tokenLength == 1 && data[lastCharIndex] == 'o') {
		return OBJ_TOKEN_OBJECT;
	}
	if (tokenLength == 1 && data[lastCharIndex] == 'g') {
		return OBJ_TOKEN_GROUP;
	}
	if (tokenLength == 1 && data[lastCharIndex] == 's') {
		return OBJ_TOKEN_SMOOTH_SHADING;
	}
	
	return OBJ_TOKEN_UNKNOWN;
}

// TODO: Possibly unsafe for nonterminated strings
static float readFloat(const char * data, size_t length, size_t * ioCharIndex) {
	char * endPtr;
	float value;
	
	value = strtof(data + *ioCharIndex, &endPtr);
	if (endPtr == data + *ioCharIndex) {
		return nanf("");
	}
	
	*ioCharIndex += endPtr - (data + *ioCharIndex);
	return value;
}

static int readInt(const char * data, size_t length, size_t * ioCharIndex) {
	char * endPtr;
	int value;
	
	value = (int) strtol(data + *ioCharIndex, &endPtr, 10);
	if (endPtr == data + *ioCharIndex) {
		return 0;
	}
	
	*ioCharIndex += endPtr - (data + *ioCharIndex);
	return value;
}

static void readFaceVertex(const char * data, size_t length, size_t * ioCharIndex, int * outFaceVertex) {
	outFaceVertex[0] = readInt(data, length, ioCharIndex);
	
	if (data[*ioCharIndex] == '/') {
		++*ioCharIndex;
		
		if (data[*ioCharIndex] == '/') {
			outFaceVertex[1] = 0;
		} else {
			outFaceVertex[1] = readInt(data, length, ioCharIndex);
		}
		if (data[*ioCharIndex] == '/') {
			++*ioCharIndex;
			outFaceVertex[2] = readInt(data, length, ioCharIndex);
		} else {
			outFaceVertex[2] = 0;
		}
		
	} else {
		outFaceVertex[1] = 0;
		outFaceVertex[2] = 0;
	}
}

MeshRenderable * Obj3DModelIO_loadFile(const char * filePath) {
	char * fileContents;
	size_t fileLength = 0;
	MeshRenderable * result;
	
	fileContents = readFileSimple(filePath, &fileLength);
	if (fileContents == NULL) {
		return NULL;
	}
	result = Obj3DModelIO_loadData(fileContents, fileLength);
	free(fileContents);
	return result;
}

MeshRenderable * Obj3DModelIO_loadData(const char * data, size_t length) {
	size_t charIndex = 0;
	size_t positionCount = 0, positionAllocatedCount = 1024;
	Vector3f * positions = malloc(sizeof(Vector3f) * positionAllocatedCount);
	size_t normalCount = 0, normalAllocatedCount = 1024;
	Vector3f * normals = malloc(sizeof(Vector3f) * normalAllocatedCount);
	size_t faceCount = 0, faceAllocatedCount = 1024;
	int * faces = malloc(sizeof(int) * faceAllocatedCount * 9);
	int face[9];
	struct vertex_p3f_t2f_n3f_c4f * vertices, vertex;
	GLuint * indexes;
	MeshRenderable * result;
	
	// TODO: This code is unsafe for malformed obj files
	for (;;) {
		enum ObjToken token;
		Vector3f position, normal;
		
		skipWhitespace(data, length, &charIndex);
		if (charIndex >= length) {
			break;
		}
		
		token = readObjToken(data, length, &charIndex);
		switch (token) {
			case OBJ_TOKEN_COMMENT:
			case OBJ_TOKEN_MTLLIB:
			case OBJ_TOKEN_USEMTL:
			case OBJ_TOKEN_OBJECT:
			case OBJ_TOKEN_GROUP:
			case OBJ_TOKEN_SMOOTH_SHADING:
			case OBJ_TOKEN_TEXTURE_COORD: // TODO: Load texture coords
			case OBJ_TOKEN_UNKNOWN:
				skipToNextLine(data, length, &charIndex);
				break;
				
			case OBJ_TOKEN_VERTEX:
				skipWhitespace(data, length, &charIndex);
				position.x = readFloat(data, length, &charIndex);
				skipWhitespace(data, length, &charIndex);
				position.y = readFloat(data, length, &charIndex);
				skipWhitespace(data, length, &charIndex);
				position.z = readFloat(data, length, &charIndex);
				
				if (positionCount >= positionAllocatedCount) {
					positionAllocatedCount *= 2;
					positions = realloc(positions, sizeof(Vector3f) * positionAllocatedCount);
				}
				positions[positionCount++] = position;
				
				skipToNextLine(data, length, &charIndex);
				break;
				
			case OBJ_TOKEN_NORMAL:
				skipWhitespace(data, length, &charIndex);
				normal.x = readFloat(data, length, &charIndex);
				skipWhitespace(data, length, &charIndex);
				normal.y = readFloat(data, length, &charIndex);
				skipWhitespace(data, length, &charIndex);
				normal.z = readFloat(data, length, &charIndex);
				
				if (normalCount >= normalAllocatedCount) {
					normalAllocatedCount *= 2;
					normals = realloc(normals, sizeof(Vector3f) * normalAllocatedCount);
				}
				normals[normalCount++] = normal;
				
				skipToNextLine(data, length, &charIndex);
				break;
				
			case OBJ_TOKEN_FACE:
				skipWhitespace(data, length, &charIndex);
				readFaceVertex(data, length, &charIndex, face);
				skipWhitespace(data, length, &charIndex);
				readFaceVertex(data, length, &charIndex, face + 3);
				skipWhitespace(data, length, &charIndex);
				readFaceVertex(data, length, &charIndex, face + 6);
				
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
				
				skipWhitespace(data, length, &charIndex);
				while (data[charIndex] != '\n' && data[charIndex] != '\r' && data[charIndex] != '#') {
					face[3] = face[6];
					face[4] = face[7];
					face[5] = face[8];
					readFaceVertex(data, length, &charIndex, face + 6);
					
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
					
					skipWhitespace(data, length, &charIndex);
				}
				skipToNextLine(data, length, &charIndex);
				
				break;
		}
	}
	
	// TODO: uniq vertex/normal/texCoord triples
	vertices = malloc(sizeof(*vertices) * faceCount * 3);
	indexes = malloc(sizeof(GLuint) * faceCount * 3);
	vertex.texCoords[0] = 0.0f;
	vertex.texCoords[1] = 0.0f;
	vertex.color[0] = 1.0f;
	vertex.color[1] = 1.0f;
	vertex.color[2] = 1.0f;
	vertex.color[3] = 1.0f;
	for (size_t faceIndex = 0; faceIndex < faceCount; faceIndex++) {
		for (size_t vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
			vertex.position[0] = positions[faces[faceIndex * 9 + vertexIndex * 3 + 0] - 1].x;
			vertex.position[1] = positions[faces[faceIndex * 9 + vertexIndex * 3 + 0] - 1].y;
			vertex.position[2] = positions[faces[faceIndex * 9 + vertexIndex * 3 + 0] - 1].z;
			
			if (faces[faceIndex * 9 + vertexIndex * 3 + 2] > 0 && faces[faceIndex * 9 + vertexIndex * 3 + 2] <= (int) normalCount) {
				vertex.normal[0] = normals[faces[faceIndex * 9 + vertexIndex * 3 + 2] - 1].x;
				vertex.normal[1] = normals[faces[faceIndex * 9 + vertexIndex * 3 + 2] - 1].y;
				vertex.normal[2] = normals[faces[faceIndex * 9 + vertexIndex * 3 + 2] - 1].z;
				
			} else {
				vertex.normal[0] = 0.0f;
				vertex.normal[1] = 0.0f;
				vertex.normal[2] = 0.0f;
			}
			vertices[faceIndex * 3 + vertexIndex] = vertex;
			indexes[faceIndex * 3 + vertexIndex] = faceIndex * 3 + vertexIndex;
		}
	}
	
	free(positions);
	free(normals);
	free(faces);
	
	result = MeshRenderable_createStatic(vertices, faceCount * 3, indexes, faceCount * 3, NULL);
	free(vertices);
	free(indexes);
	return result;
}
