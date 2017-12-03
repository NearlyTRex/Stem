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

#include "glgraphics/Renderable.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

bool Renderable_init(Renderable * self, RenderableType type) {
	call_super(init, self);
	self->type = type;
	self->visible = true;
	self->dispose = Renderable_dispose;
	self->getBoundingBox = Renderable_getBoundingBox;
	return true;
}

void Renderable_dispose(Renderable * self) {
	call_super(dispose, self);
}

Box6f Renderable_getBoundingBox(Renderable * self) {
	return BOX6f_EMPTY;
}
