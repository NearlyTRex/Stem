/*
  Copyright (c) 2015 Alex Diener
  
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

#include "glbitmapfont/GLBitmapFont.h"
#include "gltexture/GLTexture.h"
#include "gltexture/GLTextureAtlas.h"
#include "jsonserialization/JSONDeserializationContext.h"
#include "testharness/ResourceWiring.h"
#include "pngimageio/PNGImageIO.h"
#include "utilities/IOUtilities.h"
#include <limits.h>
#include <stdio.h>

void ResourceWiring_addTypeHandlers(ResourceManager * resourceManager) {
	ResourceManager_addTypeHandler(resourceManager, "png_flip", loadPNGFlip, unloadPNG, resourceManager);
	ResourceManager_addTypeHandler(resourceManager, "texture_json", loadTextureJSON, unloadTexture, resourceManager);
	ResourceManager_addTypeHandler(resourceManager, "texture_atlas_json", loadTextureAtlasJSON, unloadTextureAtlas, resourceManager);
	ResourceManager_addTypeHandler(resourceManager, "bitmap_font", loadBitmapFont, unloadBitmapFont, resourceManager);
}

void * loadPNGFlip(const char * resourceName, void * context) {
	return PNGImageIO_loadPNGFile(resourceName, PNG_PIXEL_FORMAT_AUTOMATIC, true);
}

void * loadTextureJSON(const char * resourceName, void * context) {
	ResourceManager * resourceManager = context;
	JSONDeserializationContext * deserializationContext;
	GLTexture * texture;
	BitmapImage * image;
	
	deserializationContext = JSONDeserializationContext_createWithFile(resourceName);
	if (deserializationContext->status != SERIALIZATION_ERROR_OK) {
		fprintf(stderr, "Error: Couldn't load %s as a json file (error %d)\n", resourceName, deserializationContext->status);
		return NULL;
	}
	texture = GLTexture_deserialize(deserializationContext);
	if (texture == NULL) {
		fprintf(stderr, "Error: Couldn't load %s as a texture metadata file (error %d)\n", resourceName, deserializationContext->status);
		deserializationContext->dispose(deserializationContext);
		return NULL;
	}
	deserializationContext->dispose(deserializationContext);
	
	image = resourceManager->referenceResource(resourceManager, "png_flip", texture->imageName);
	if (image == NULL) {
		fprintf(stderr, "Error: Couldn't load %s as a png file\n", texture->imageName);
		texture->dispose(texture);
		return NULL;
	}
	texture->setImage(texture, 0, image->width, image->height, image->bytesPerRow, image->pixels);
	resourceManager->releaseResource(resourceManager, "png_flip", texture->imageName);
	
	return texture;
}

void * loadTextureAtlasJSON(const char * resourceName, void * context) {
	ResourceManager * resourceManager = context;
	JSONDeserializationContext * deserializationContext;
	GLTextureAtlas * textureAtlas;
	GLTexture * texture;
	
	deserializationContext = JSONDeserializationContext_createWithFile(resourceName);
	if (deserializationContext->status != SERIALIZATION_ERROR_OK) {
		fprintf(stderr, "Error: Couldn't load %s as a json file (error %d)\n", resourceName, deserializationContext->status);
		return NULL;
	}
	textureAtlas = GLTextureAtlas_deserialize(deserializationContext);
	if (textureAtlas == NULL) {
		fprintf(stderr, "Error: Couldn't load %s as a texture atlas file (error %d)\n", resourceName, deserializationContext->status);
		deserializationContext->dispose(deserializationContext);
		return NULL;
	}
	deserializationContext->dispose(deserializationContext);
	
	texture = resourceManager->referenceResource(resourceManager, "texture_json", textureAtlas->textureName);
	if (texture == NULL) {
		textureAtlas->dispose(textureAtlas);
		return NULL;
	}
	GLTextureAtlas_setTexture(textureAtlas, texture, false);
	
	return textureAtlas;
}

void * loadBitmapFont(const char * resourceName, void * context) {
	ResourceManager * resourceManager = context;
	JSONDeserializationContext * deserializationContext;
	GLBitmapFont * font;
	GLTextureAtlas * atlas;
	
	deserializationContext = JSONDeserializationContext_createWithFile(resourceName);
	if (deserializationContext->status != SERIALIZATION_ERROR_OK) {
		fprintf(stderr, "Error: Couldn't load %s as a json file (error %d)\n", resourceName, deserializationContext->status);
		return NULL;
	}
	font = GLBitmapFont_deserialize(deserializationContext);
	if (font == NULL) {
		fprintf(stderr, "Error: Couldn't load %s as a font metadata file (error %d)\n", resourceName, deserializationContext->status);
		deserializationContext->dispose(deserializationContext);
		return NULL;
	}
	deserializationContext->dispose(deserializationContext);
	
	atlas = resourceManager->referenceResource(resourceManager, "texture_atlas_json", font->atlasName);
	if (atlas == NULL) {
		font->dispose(font);
		return NULL;
	}
	GLBitmapFont_setTextureAtlas(font, atlas, false);
	
	return font;
}

void unloadPNG(void * resource, void * context) {
	BitmapImage * image = resource;
	
	image->dispose(image);
}

void unloadTexture(void * resource, void * context) {
	GLTexture * texture = resource;
	
	texture->dispose(texture);
}

void unloadTextureAtlas(void * resource, void * context) {
	GLTextureAtlas * textureAtlas = resource;
	ResourceManager * resourceManager = context;
	
	resourceManager->releaseResource(resourceManager, "texture_json", textureAtlas->textureName);
	textureAtlas->dispose(textureAtlas);
}

void unloadBitmapFont(void * resource, void * context) {
	GLBitmapFont * font = resource;
	ResourceManager * resourceManager = context;
	
	resourceManager->releaseResource(resourceManager, "texture_atlas_json", font->atlasName);
	font->dispose(font);
}
