#include "keyboard.h"
#include "irq.h"
#include "system.h"
#include "timer.h"

typedef u8 kb_key_code;
typedef u8 char_code;

kb_key_code keyboard_layout_us[2][128] = {
	// clang-format off
	{
		KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		'-', '=', KEY_BACKSPACE, KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u',
		'i', 'o', 'p', '[', ']', KEY_ENTER, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
		'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
		',', '.', '/', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
		KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
		KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
		KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
	}, {
		KEY_NULL, KEY_ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
		'_', '+', KEY_BACKSPACE, KEY_TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U',
		'I', 'O', 'P',   '{', '}', KEY_ENTER, 0, 'A', 'S', 'D', 'F', 'G', 'H',
		'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
		'M', '<', '>', '?', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4,
		KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
		KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
		KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
	}
	// clang-format on
};

struct Keyboard keyboard;

// bad hack! for a better RNG
static bool seeded = false;

static bool is_key_press(kb_scancode scancode) {
	return (scancode & KEYBOARD_RELEASE) == 0;
}

static bool is_key_release(kb_scancode scancode) {
	return !is_key_press(scancode);
}

static kb_key_code get_key_code(kb_scancode scancode) {
	return scancode & 0x7F;
}

static char_code get_char_code(kb_scancode scancode) {

	const kb_key_code keycode	     = get_key_code(scancode);
	const kb_scancode has_shift_modifier = scancode & KEY_MOD_SHIFT;

	if (keycode < 128)
		return keyboard_layout_us[has_shift_modifier ? 1 : 0][keycode];

	return 0;
}

static kb_scancode get_modifier(kb_key_code keycode) {

	switch (keycode) {
	case KEY_LALT:
		return KEY_MOD_ALT;

	case KEY_LCTRL:
		return KEY_MOD_CTRL;

	case KEY_LSHIFT:
	case KEY_RSHIFT:
		return KEY_MOD_SHIFT;

	case KEY_CAPS_LOCK:
		return KEY_MOD_CAPS_LOCK;

	case KEY_NUM_LOCK:
		return KEY_MOD_NUM_LOCK;

	case KEY_SCROLL_LOCK:
		return KEY_MOD_SCROLL_LOCK;
	default:
		return KEY_NULL;
	}
}

static void keyboard_handler(struct Registers * regs) {
	const kb_scancode scancode = (kb_scancode) inportb(0x60);
	const kb_key_code keycode  = get_key_code(scancode);

	if (!seeded) {
		seed(((u32) scancode) * 17 + timer_get());
		seeded = true;
	}

	const bool key_is_pressed  = is_key_press(scancode);
	const kb_scancode modifier = get_modifier(keycode);

	keyboard.mods = BIT_SET(keyboard.mods, HIBIT(modifier), key_is_pressed);

	keyboard.keys[keycode]			= key_is_pressed;
	keyboard.chars[get_char_code(scancode)] = key_is_pressed;
}

void keyboard_init() {
	irq_install(1, keyboard_handler);
}
