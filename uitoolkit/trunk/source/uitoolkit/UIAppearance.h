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

#ifndef __UIAppearance_H__
#define __UIAppearance_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct UIAppearance UIAppearance;

#include "glbitmapfont/GLBitmapFont.h"
#include "glgraphics/TextureAtlas.h"
#include "stemobject/StemObject.h"

#define UIAPPEARANCE_KEY_BUTTON_UP "button_up"
#define UIAPPEARANCE_KEY_BUTTON_DOWN "button_down"
#define UIAPPEARANCE_KEY_CHECKBOX_UP "checkbox_up"
#define UIAPPEARANCE_KEY_CHECKBOX_DOWN "checkbox_down"
#define UIAPPEARANCE_KEY_CHECKBOX_UP_CHECKED "checkbox_up_checked"
#define UIAPPEARANCE_KEY_CHECKBOX_DOWN_CHECKED "checkbox_down_checked"

// Measured in pixels
struct UIAppearance_sliceGrid {
	float leftColumn;
	float centerColumn;
	float rightColumn;
	float bottomRow;
	float centerRow;
	float topRow;
};

struct UIAppearance_metrics {
	float fontHeight;
	
	float buttonTextPadding;
	Color4f buttonTextColor;
	struct UIAppearance_sliceGrid buttonSlices;
	
	float checkboxGraphicWidth;
	float checkboxGraphicHeight;
	float checkboxTextPadding;
	Color4f checkboxTextColor;
};

#define UIAppearance_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	TextureAtlas * atlas; \
	GLBitmapFont * font; \
	struct UIAppearance_metrics metrics;

stemobject_struct_definition(UIAppearance)

// Copies entries; retains references to atlas and font
UIAppearance * UIAppearance_create(TextureAtlas * atlas, GLBitmapFont * font, struct UIAppearance_metrics metrics);
bool UIAppearance_init(UIAppearance * self, TextureAtlas * atlas, GLBitmapFont * font, struct UIAppearance_metrics metrics);
void UIAppearance_dispose(UIAppearance * self);

struct UIAppearance_metrics UIAppearance_defaultMetrics(void);

#ifdef __cplusplus
}
#endif
#endif
