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

#ifndef __ShellBatteryInfo_H__
#define __ShellBatteryInfo_H__
#ifdef __cplusplus
extern "C" {
#endif

enum ShellBatteryState {
	ShellBatteryState_unknown,          // Battery state could not be determined
	ShellBatteryState_full,             // Device is plugged in and battery is fully charged
	ShellBatteryState_charging,         // Device is plugged in and battery is charging
	ShellBatteryState_unplugged,        // Device is not plugged in and is running off battery power
	ShellBatteryState_batteryMissing,   // Device uses a battery, but none is connected
	ShellBatteryState_notBatteryPowered // Device does not use a battery
};

/** Returns a value from the ShellBatteryState enum specifying the state of the device's battery, if
    known. See enum values for specific descriptions. */
enum ShellBatteryState Shell_getBatteryState();

/** Returns the device's battery level, if known, in the range [0..1]. If Shell_getBatteryState()
    returns something other than ShellBatteryState_full, ShellBatteryState_charging, or
    ShellBatteryState_unplugged, the return value of this function is meaningless. In these cases,
    -1 will generally be returned. */
float Shell_getBatteryLevel();

#ifdef __cplusplus
}
#endif
#endif
