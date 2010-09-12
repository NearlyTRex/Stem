/*
  Copyright (c) 2010 Alex Diener
  
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

#include "preferences/Preferences_private.h"
#import <Foundation/Foundation.h>

void Preferences_loadPrivate(compat_type(Preferences *) selfPtr) {
	Preferences * self = selfPtr;
	NSUserDefaults * defaults;
	size_t valueIndex;
	NSObject * object;
	NSAutoreleasePool * pool;
	
	pool = [[NSAutoreleasePool alloc] init];
	// TODO: How to handle domain/identifier?
	defaults = [NSUserDefaults standardUserDefaults];
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		object = [defaults objectForKey: [NSString stringWithUTF8String: self->values[valueIndex].name]];
		
		if (object != nil) {
			switch (self->values[valueIndex].type) {
				case PREFERENCES_TYPE_INTEGER:
					if ([object isKindOfClass: [NSNumber class]]) {
						Preferences_setIntegerPrivate(self, self->values[valueIndex].name, [(NSNumber *) object intValue]);
					}
					break;
					
				case PREFERENCES_TYPE_FLOAT:
					if ([object isKindOfClass: [NSNumber class]]) {
						Preferences_setFloatPrivate(self, self->values[valueIndex].name, [(NSNumber *) object floatValue]);
					}
					break;
					
				case PREFERENCES_TYPE_BOOLEAN:
					if ([object isKindOfClass: [NSNumber class]]) {
						Preferences_setBooleanPrivate(self, self->values[valueIndex].name, [(NSNumber *) object boolValue]);
					}
					break;
					
				case PREFERENCES_TYPE_STRING:
					if ([object isKindOfClass: [NSString class]]) {
						Preferences_setStringPrivate(self, self->values[valueIndex].name, [(NSString *) object UTF8String]);
					}
					break;
					
				case PREFERENCES_TYPE_DATA:
					if ([object isKindOfClass: [NSData class]]) {
						Preferences_setDataPrivate(self, self->values[valueIndex].name, [(NSData *) object bytes], [(NSData *) object length]);
					}
					break;
			}
		}
	}
	[pool release];
}

void Preferences_savePrivate(compat_type(Preferences *) selfPtr) {
	Preferences * self = selfPtr;
	NSUserDefaults * defaults;
	size_t valueIndex;
	NSAutoreleasePool * pool;
	
	pool = [[NSAutoreleasePool alloc] init];
	defaults = [NSUserDefaults standardUserDefaults];
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		switch (self->values[valueIndex].type) {
			case PREFERENCES_TYPE_INTEGER:
				[defaults setInteger: self->values[valueIndex].value.integer forKey: [NSString stringWithUTF8String: self->values[valueIndex].name]];
				break;
				
			case PREFERENCES_TYPE_FLOAT:
				[defaults setFloat: self->values[valueIndex].value.number forKey: [NSString stringWithUTF8String: self->values[valueIndex].name]];
				break;
				
			case PREFERENCES_TYPE_BOOLEAN:
				[defaults setBool: self->values[valueIndex].value.boolean forKey: [NSString stringWithUTF8String: self->values[valueIndex].name]];
				break;
				
			case PREFERENCES_TYPE_STRING:
				[defaults setObject: [NSString stringWithUTF8String: self->values[valueIndex].value.string] forKey: [NSString stringWithUTF8String: self->values[valueIndex].name]];
				break;
				
			case PREFERENCES_TYPE_DATA:
				[defaults setObject: [NSData dataWithBytes: self->values[valueIndex].value.data.bytes length: self->values[valueIndex].value.data.length] forKey: [NSString stringWithUTF8String: self->values[valueIndex].name]];
				break;
		}
	}
	[defaults synchronize];
	[pool release];
}
