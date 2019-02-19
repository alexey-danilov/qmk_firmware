#include "stapelberg.h"

void matrix_init_kb(void) {
	// put your keyboard start-up code here
	// runs once when the firmware starts up
    // * Set our LED pins as output
    DDRF |= (1<<0); // Keypad LED
    DDRF |= (1<<1); // ScrLock LED
    DDRF |= (1<<2); // NumLock LED
    DDRF |= (1<<3); // CapsLock LED

	matrix_init_user();
}

void matrix_scan_kb(void) {
	// put your looping keyboard code here
	// runs every cycle (a lot)

	matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
	// put your per-action keyboard code here
	// runs for every action, just before processing by the firmware

	return process_record_user(keycode, record);
}

void led_init_ports() {
  // * Set our LED pins as output
  DDRF |= (1<<0); // Keypad LED
  DDRF |= (1<<1); // ScrLock LED
  DDRF |= (1<<2); // NumLock LED
  DDRF |= (1<<3); // CapsLock LED
}

void led_set_kb(uint8_t usb_led) {
	led_set_user(usb_led);
}
