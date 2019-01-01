/*
  Copyright (c) 2019 Alex Diener
  
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

#ifndef __UIToolkitDrawing_H__
#define __UIToolkitDrawing_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gamemath/Rect4f.h"
#include "glgraphics/VertexTypes.h"
#include "uitoolkit/UIAppearance.h"

void UIToolkit_getFrameVerticesWithSlices(Rect4f drawBounds,
                                          Rect4f textureBounds,
                                          UIAppearance_sliceGrid slices,
                                          Color4f color,
                                          struct vertex_p2f_t2f_c4f * outVertices,
                                          GLuint * outIndexes,
                                          unsigned int * ioVertexCount,
                                          unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
