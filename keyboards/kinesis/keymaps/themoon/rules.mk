EXTRAKEY_ENABLE = yes
AUTO_SHIFT_ENABLE = yes
AUTO_SHIFT_MODIFIERS = yes
TAP_DANCE_ENABLE = yes

NKRO_ENABLE = no
BOOTMAGIC_ENABLE = no
KEY_LOCK_ENABLE = no
MOUSEKEY_ENABLE = no
CONSOLE_ENABLE = yes
COMMAND_ENABLE = no
BACKLIGHT_ENABLE = no
MIDI_ENABLE = no
AUDIO_ENABLE = no
UNICODE_ENABLE = no
BLUETOOTH_ENABLE = no
RGBLIGHT_ENABLE = no
SLEEP_LED_ENABLE = no

ifndef QUANTUM_DIR
	include ../../../../Makefile
endif
