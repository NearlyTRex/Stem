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

#ifndef __UIRadioGroup_H__
#define __UIRadioGroup_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct UIRadioGroup UIRadioGroup;

#include "stemobject/StemObject.h"
#include "uitoolkit/UIRadioButton.h"

#define UIRadioGroup_structContents(self_type) \
	StemObject_structContents(self_type) \
	UIRadioButton * checkedRadioButton;

stemobject_struct_definition(UIRadioGroup)

UIRadioGroup * UIRadioGroup_create();
bool UIRadioGroup_init(UIRadioGroup * self);
void UIRadioGroup_dispose(UIRadioGroup * self);

void UIRadioGroup_setCheckedButton(UIRadioGroup * self, UIRadioButton * radioButton);

#ifdef __cplusplus
}
#endif
#endif
