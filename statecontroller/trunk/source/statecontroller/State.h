/*
  Copyright (c) 2013 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __STATE_H__
#define __STATE_H__

typedef struct State State;

#include "stemobject/StemObject.h"
#include "statecontroller/StateController.h"

#define State_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	StateController * stateController; \
	\
	void (* activate)(self_type * self); \
	void (* deactivate)(self_type * self); \
	void (* draw)(self_type * self);

stemobject_struct_definition(State)

void State_init(State * self);
void State_dispose(State * self);

void State_activate(State * self);
void State_deactivate(State * self);
void State_draw(State * self);

#endif
