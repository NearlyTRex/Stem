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
  
  Alex Diener adiener@sacredsoftware.net
*/

// Adapted from GameShell (http://onesadcookie.com/svn/GameShell/Source/Common/GSKeyCodesInternal.h). Thanks Keith!

#ifndef __ShellKeyCodes_H__
#define __ShellKeyCodes_H__
#ifdef __cplusplus
extern "C" {
#endif

#define KEYBOARD_A                      0x04
#define KEYBOARD_B                      0x05
#define KEYBOARD_C                      0x06
#define KEYBOARD_D                      0x07
#define KEYBOARD_E                      0x08
#define KEYBOARD_F                      0x09
#define KEYBOARD_G                      0x0A
#define KEYBOARD_H                      0x0B
#define KEYBOARD_I                      0x0C
#define KEYBOARD_J                      0x0D
#define KEYBOARD_K                      0x0E
#define KEYBOARD_L                      0x0F
#define KEYBOARD_M                      0x10
#define KEYBOARD_N                      0x11
#define KEYBOARD_O                      0x12
#define KEYBOARD_P                      0x13
#define KEYBOARD_Q                      0x14
#define KEYBOARD_R                      0x15
#define KEYBOARD_S                      0x16
#define KEYBOARD_T                      0x17
#define KEYBOARD_U                      0x18
#define KEYBOARD_V                      0x19
#define KEYBOARD_W                      0x1A
#define KEYBOARD_X                      0x1B
#define KEYBOARD_Y                      0x1C
#define KEYBOARD_Z                      0x1D
#define KEYBOARD_1                      0x1E
#define KEYBOARD_2                      0x1F
#define KEYBOARD_3                      0x20
#define KEYBOARD_4                      0x21
#define KEYBOARD_5                      0x22
#define KEYBOARD_6                      0x23
#define KEYBOARD_7                      0x24
#define KEYBOARD_8                      0x25
#define KEYBOARD_9                      0x26
#define KEYBOARD_0                      0x27
#define KEYBOARD_RETURN_OR_ENTER        0x28
#define KEYBOARD_ESCAPE                 0x29
#define KEYBOARD_DELETE_OR_BACKSPACE    0x2A
#define KEYBOARD_TAB                    0x2B
#define KEYBOARD_SPACEBAR               0x2C
#define KEYBOARD_HYPHEN                 0x2D
#define KEYBOARD_EQUAL_SIGN             0x2E
#define KEYBOARD_OPEN_BRACKET           0x2F
#define KEYBOARD_CLOSE_BRACKET          0x30
#define KEYBOARD_BACKSLASH              0x31
#define KEYBOARD_NON_USPOUND            0x32
#define KEYBOARD_SEMICOLON              0x33
#define KEYBOARD_QUOTE                  0x34
#define KEYBOARD_GRAVE_ACCENT_AND_TILDE 0x35
#define KEYBOARD_COMMA                  0x36
#define KEYBOARD_PERIOD                 0x37
#define KEYBOARD_SLASH                  0x38
#define KEYBOARD_CAPS_LOCK              0x39
#define KEYBOARD_F1                     0x3A
#define KEYBOARD_F2                     0x3B
#define KEYBOARD_F3                     0x3C
#define KEYBOARD_F4                     0x3D
#define KEYBOARD_F5                     0x3E
#define KEYBOARD_F6                     0x3F
#define KEYBOARD_F7                     0x40
#define KEYBOARD_F8                     0x41
#define KEYBOARD_F9                     0x42
#define KEYBOARD_F10                    0x43
#define KEYBOARD_F11                    0x44
#define KEYBOARD_F12                    0x45
#define KEYBOARD_PRINT_SCREEN           0x46
#define KEYBOARD_SCROLL_LOCK            0x47
#define KEYBOARD_PAUSE                  0x48
#define KEYBOARD_INSERT                 0x49
#define KEYBOARD_HOME                   0x4A
#define KEYBOARD_PAGE_UP                0x4B
#define KEYBOARD_DELETE_FORWARD         0x4C
#define KEYBOARD_END                    0x4D
#define KEYBOARD_PAGE_DOWN              0x4E
#define KEYBOARD_RIGHT_ARROW            0x4F
#define KEYBOARD_LEFT_ARROW             0x50
#define KEYBOARD_DOWN_ARROW             0x51
#define KEYBOARD_UP_ARROW               0x52
#define KEYPAD_NUM_LOCK                 0x53
#define KEYPAD_SLASH                    0x54
#define KEYPAD_ASTERISK                 0x55
#define KEYPAD_HYPHEN                   0x56
#define KEYPAD_PLUS                     0x57
#define KEYPAD_ENTER                    0x58
#define KEYPAD_1                        0x59
#define KEYPAD_2                        0x5A
#define KEYPAD_3                        0x5B
#define KEYPAD_4                        0x5C
#define KEYPAD_5                        0x5D
#define KEYPAD_6                        0x5E
#define KEYPAD_7                        0x5F
#define KEYPAD_8                        0x60
#define KEYPAD_9                        0x61
#define KEYPAD_0                        0x62
#define KEYPAD_PERIOD                   0x63
#define KEYBOARD_NON_US_BACKSLASH       0x64
#define KEYBOARD_APPLICATION            0x65
#define KEYBOARD_POWER                  0x66
#define KEYPAD_EQUAL_SIGN               0x67
#define KEYBOARD_F13                    0x68
#define KEYBOARD_F14                    0x69
#define KEYBOARD_F15                    0x6A
#define KEYBOARD_F16                    0x6B
#define KEYBOARD_F17                    0x6C
#define KEYBOARD_F18                    0x6D
#define KEYBOARD_F19                    0x6E
#define KEYBOARD_F20                    0x6F
#define KEYBOARD_F21                    0x70
#define KEYBOARD_F22                    0x71
#define KEYBOARD_F23                    0x72
#define KEYBOARD_F24                    0x73
#define KEYBOARD_EXECUTE                0x74
#define KEYBOARD_HELP                   0x75
#define KEYBOARD_MENU                   0x76
#define KEYBOARD_SELECT                 0x77
#define KEYBOARD_STOP                   0x78
#define KEYBOARD_AGAIN                  0x79
#define KEYBOARD_UNDO                   0x7A
#define KEYBOARD_CUT                    0x7B
#define KEYBOARD_COPY                   0x7C
#define KEYBOARD_PASTE                  0x7D
#define KEYBOARD_FIND                   0x7E
#define KEYBOARD_MUTE                   0x7F
#define KEYBOARD_VOLUME_UP              0x80
#define KEYBOARD_VOLUME_DOWN            0x81
#define KEYBOARD_LOCKING_CAPS_LOCK      0x82
#define KEYBOARD_LOCKING_NUM_LOCK       0x83
#define KEYBOARD_LOCKING_SCROLL_LOCK    0x84
#define KEYPAD_COMMA                    0x85
#define KEYPAD_EQUAL_SIGN_AS400         0x86
#define KEYBOARD_INTERNATIONAL_1        0x87
#define KEYBOARD_INTERNATIONAL_2        0x88
#define KEYBOARD_INTERNATIONAL_3        0x89
#define KEYBOARD_INTERNATIONAL_4        0x8A
#define KEYBOARD_INTERNATIONAL_5        0x8B
#define KEYBOARD_INTERNATIONAL_6        0x8C
#define KEYBOARD_INTERNATIONAL_7        0x8D
#define KEYBOARD_INTERNATIONAL_8        0x8E
#define KEYBOARD_INTERNATIONAL_9        0x8F
#define KEYBOARD_LANG1                  0x90
#define KEYBOARD_LANG2                  0x91
#define KEYBOARD_LANG3                  0x92
#define KEYBOARD_LANG4                  0x93
#define KEYBOARD_LANG5                  0x94
#define KEYBOARD_LANG6                  0x95
#define KEYBOARD_LANG7                  0x96
#define KEYBOARD_LANG8                  0x97
#define KEYBOARD_LANG9                  0x98
#define KEYBOARD_ALTERNATE_ERASE        0x99
#define KEYBOARD_SYS_REQ_OR_ATTENTION   0x9A
#define KEYBOARD_CANCEL                 0x9B
#define KEYBOARD_CLEAR                  0x9C
#define KEYBOARD_PRIOR                  0x9D
#define KEYBOARD_RETURN                 0x9E
#define KEYBOARD_SEPARATOR              0x9F
#define KEYBOARD_OUT                    0xA0
#define KEYBOARD_OPER                   0xA1
#define KEYBOARD_CLEAR_OR_AGAIN         0xA2
#define KEYBOARD_CR_SEL_OR_PROPS        0xA3
#define KEYBOARD_EX_SEL                 0xA4
#define KEYBOARD_LEFT_CONTROL           0xE0
#define KEYBOARD_LEFT_SHIFT             0xE1
#define KEYBOARD_LEFT_ALT               0xE2
#define KEYBOARD_LEFT_GUI               0xE3
#define KEYBOARD_RIGHT_CONTROL          0xE4
#define KEYBOARD_RIGHT_SHIFT            0xE5
#define KEYBOARD_RIGHT_ALT              0xE6
#define KEYBOARD_RIGHT_GUI              0xE7

#define MODIFIER_SHIFT_BIT 0x01
#define MODIFIER_CONTROL_BIT 0x02
#define MODIFIER_ALT_BIT 0x04
#define MODIFIER_COMMAND_BIT 0x08
#define MODIFIER_CAPS_LOCK_BIT 0x10

#ifdef __cplusplus
}
#endif
#endif
