#ifndef __SHELL_BATTERY_INFO_H__
#define __SHELL_BATTERY_INFO_H__

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
    ShellBatteryState_unplugged, a negative value is returned. */
float Shell_getBatteryLevel();

#endif
