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

#ifndef __SharedEvents_H__
#define __SharedEvents_H__
#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_KEY_DOWN "key_down"
#define EVENT_KEY_UP "key_up"
#define EVENT_KEY_MODIFIERS_CHANGED "key_modifiers_changed"
#define EVENT_MOUSE_DOWN "mouse_down"
#define EVENT_MOUSE_UP "mouse_up"
#define EVENT_MOUSE_MOVED "mouse_moved"
#define EVENT_MOUSE_DRAGGED "mouse_dragged"
#define EVENT_MOUSE_SCROLL_WHEEL "scroll_wheel"
#define EVENT_RESIZED "resized"
#define EVENT_BACKGROUNDED "backgrounded"
#define EVENT_FOREGROUNDED "foregrounded"
#define EVENT_FULLSCREENED "fullscreened"
#define EVENT_WINDOWED "windowed"
#define EVENT_DRAW "draw"
#define EVENT_CONFIRM_QUIT "confirm_quit"

#include "gamemath/Vector2f.h"

struct keyEvent {
	unsigned int charCode;
	unsigned int keyCode;
	unsigned int modifiers;
	bool isRepeat;
};

struct mouseEvent {
	unsigned int button; // Number for down/up, mask for dragged, undefined for moved
	Vector2f position;
};

struct mouseWheelEvent {
	int deltaX;
	int deltaY;
};

#ifdef __cplusplus
}
#endif
#endif
