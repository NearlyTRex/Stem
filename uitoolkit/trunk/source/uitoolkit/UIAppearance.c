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

#include "uitoolkit/UIAppearance.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

UIAppearance * UIAppearance_create(TextureAtlas * atlas, GLBitmapFont * font, struct UIAppearance_metrics metrics) {
	stemobject_create_implementation(UIAppearance, init, atlas, font, metrics)
}

bool UIAppearance_init(UIAppearance * self, TextureAtlas * atlas, GLBitmapFont * font, struct UIAppearance_metrics metrics) {
	call_super(init, self);
	self->dispose = UIAppearance_dispose;
	self->atlas = atlas;
	self->font = font;
	self->metrics = metrics;
	return true;
}

void UIAppearance_dispose(UIAppearance * self) {
	call_super(dispose, self);
}

struct UIAppearance_metrics UIAppearance_defaultMetrics(void) {
	struct UIAppearance_metrics metrics;
	
	metrics.buttonLabelHeight = 12.0f;
	metrics.buttonLabelPadding = 4.0f;
	metrics.buttonLabelColor = COLOR4f(1.0f, 1.0f, 1.0f, 1.0f);
	metrics.buttonSlices.leftColumn = 2.0f;
	metrics.buttonSlices.centerColumn = 1.0f;
	metrics.buttonSlices.rightColumn = 2.0f;
	metrics.buttonSlices.bottomRow = 2.0f;
	metrics.buttonSlices.centerRow = 1.0f;
	metrics.buttonSlices.topRow = 2.0f;
	
	return metrics;
}
