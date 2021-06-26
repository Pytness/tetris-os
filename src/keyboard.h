#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "util.h"

#define KEYBOARD_BUFFER_SIZE 256

typedef u16 kb_scancode;

enum KEY_SCAN_CODES
{
	KEY_NULL      = 0x00,
	KEY_ESC	      = 0x1B,
	KEY_BACKSPACE = '\b',
	KEY_TAB	      = '\t',
	KEY_ENTER     = '\n',
	KEY_RETURN    = '\r',

	KEY_INSERT    = 0x90,
	KEY_DELETE    = 0x91,
	KEY_HOME      = 0x92,
	KEY_END	      = 0x93,
	KEY_PAGE_UP   = 0x94,
	KEY_PAGE_DOWN = 0x95,

	KEY_LEFT  = 0x4B,
	KEY_UP	  = 0x48,
	KEY_RIGHT = 0x4D,
	KEY_DOWN  = 0x50,

	KEY_F1	= 0x80,
	KEY_F2	= 0x81,
	KEY_F3	= 0x82,
	KEY_F4	= 0x83,
	KEY_F5	= 0x84,
	KEY_F6	= 0x85,
	KEY_F7	= 0x86,
	KEY_F8	= 0x87,
	KEY_F9	= 0x88,
	KEY_F10 = 0x89,
	KEY_F11 = 0x8A,
	KEY_F12 = 0x8B,

	KEY_LCTRL = 0x1D,
	KEY_RCTRL = 0x1D,

	KEY_LALT = 0x38,
	KEY_RALT = 0x38,

	KEY_LSHIFT = 0x2A,
	KEY_RSHIFT = 0x36,

	KEY_CAPS_LOCK	= 0x3A,
	KEY_SCROLL_LOCK = 0x46,
	KEY_NUM_LOCK	= 0x45,

	KEY_MOD_ALT	    = 0x0200,
	KEY_MOD_CTRL	    = 0x0400,
	KEY_MOD_SHIFT	    = 0x0800,
	KEY_MOD_CAPS_LOCK   = 0x1000,
	KEY_MOD_NUM_LOCK    = 0x2000,
	KEY_MOD_SCROLL_LOCK = 0x4000,

	KEYBOARD_RELEASE = 0x80,
};

struct Keyboard {
	u16 mods;
	bool keys[128];
	bool chars[128];
};

extern u8 keyboard_layout_us[2][128];
extern struct Keyboard keyboard;

#define keyboard_key(_s)  (keyboard.keys[(_s)])
#define keyboard_char(_c) (keyboard.chars[(u8)(_c)])

void keyboard_init();

#endif
