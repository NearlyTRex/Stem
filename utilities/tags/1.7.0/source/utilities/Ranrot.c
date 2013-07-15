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

// Based on a RANROT-B implementation by Alex Eddy (http://homepage.mac.com/arekkusu/). Thanks Alex!

#include "utilities/Ranrot.h"
#include <limits.h>
#include <stdlib.h>

#define SUPERCLASS StemObject

static bool inited;
static Ranrot sharedInstance;

Ranrot * Ranrot_create() {
	stemobject_create_implementation(Ranrot, init)
}

void Ranrot_init(Ranrot * self) {
	call_super(init, self);
	self->protected_ivar(low) = 0;
	self->protected_ivar(high) = ~0;
	self->dispose = Ranrot_dispose;
	self->sdrand = Ranrot_sdrand;
	self->stirrand = Ranrot_stirrand;
	self->uirand = Ranrot_uirand;
	self->irand = Ranrot_irand;
	self->ufrand = Ranrot_ufrand;
	self->frand = Ranrot_frand;
}

void Ranrot_dispose(Ranrot * self) {
	call_super(dispose, self);
}

void Ranrot_sdrand(Ranrot * self, int seed) {
	self->protected_ivar(low) = seed;
	self->protected_ivar(high) = ~seed;
}

static void advance(Ranrot * self) {
	self->protected_ivar(high) = (self->protected_ivar(high) << 16) + (self->protected_ivar(high) >> 16);
	self->protected_ivar(high) += self->protected_ivar(low);
	self->protected_ivar(low) += self->protected_ivar(high);
}

void Ranrot_stirrand(Ranrot * self, int iterations) {
	while (iterations > 0) {
		advance(self);
		iterations--;
	}
}

unsigned int Ranrot_uirand(Ranrot * self) {
	advance(self);
	return self->protected_ivar(high) & INT_MAX;
}

int Ranrot_irand(Ranrot * self) {
	advance(self);
	return self->protected_ivar(high);
}

float Ranrot_ufrand(Ranrot * self, float range) {
	return range * (double) self->uirand(self) / INT_MAX;
}

float Ranrot_frand(Ranrot * self, float range) {
	return range * (double) self->irand(self) / INT_MAX;
}

void sdrand(int seed) {
	if (!inited) {
		Ranrot_init(&sharedInstance);
		inited = true;
	}
	sharedInstance.sdrand(&sharedInstance, seed);
}

void stirrand(int iterations) {
	if (!inited) {
		Ranrot_init(&sharedInstance);
		inited = true;
	}
	sharedInstance.stirrand(&sharedInstance, iterations);
}

unsigned int uirand() {
	if (!inited) {
		Ranrot_init(&sharedInstance);
		inited = true;
	}
	return sharedInstance.uirand(&sharedInstance);
}

int irand() {
	if (!inited) {
		Ranrot_init(&sharedInstance);
		inited = true;
	}
	return sharedInstance.irand(&sharedInstance);
}

float ufrand(float range) {
	if (!inited) {
		Ranrot_init(&sharedInstance);
		inited = true;
	}
	return sharedInstance.ufrand(&sharedInstance, range);
}

float frand(float range) {
	if (!inited) {
		Ranrot_init(&sharedInstance);
		inited = true;
	}
	return sharedInstance.frand(&sharedInstance, range);
}
