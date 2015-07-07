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

#ifndef __ResourceWiring_H__
#define __ResourceWiring_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "resourcemanager/ResourceManager.h"

void ResourceWiring_addTypeHandlers(ResourceManager * resourceManager);

void * loadPNGFlip(const char * resourceName, void * context);
void * loadTextureJSON(const char * resourceName, void * context);
void * loadTextureAtlasJSON(const char * resourceName, void * context);
void * loadBitmapFont(const char * resourceName, void * context);
void unloadPNG(void * resource, void * context);
void unloadTexture(void * resource, void * context);
void unloadTextureAtlas(void * resource, void * context);
void unloadBitmapFont(void * resource, void * context);

#ifdef __cplusplus
}
#endif
#endif
