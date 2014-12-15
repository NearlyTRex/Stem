/*
  Copyright (c) 2014 Alex Diener
  
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

#include "screenmanager/Screen.h"

#define SUPERCLASS StemObject

bool Screen_init(Screen * self) {
	call_super(init, self);
	self->dispose = Screen_dispose;
	self->activate = Screen_activate;
	self->deactivate = Screen_deactivate;
	self->screenManager = NULL;
	return true;
}

void Screen_dispose(Screen * self) {
	call_super(dispose, self);
}

void Screen_activate(Screen * self) {
}

void Screen_deactivate(Screen * self) {
}
