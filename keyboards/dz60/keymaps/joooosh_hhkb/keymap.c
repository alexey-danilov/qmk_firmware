#include QMK_KEYBOARD_H


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


 /* BASE LAYER
 *
 * ,-----------------------------------------------------------------------------------------.
 * | Esc |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |  \| | Del |
 * |-----------------------------------------------------------------------------------------+
 * | Tab    |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \   |
 * |-----------------------------------------------------------------------------------------+
 * | LCTRL   |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter    |
 * |-----------------------------------------------------------------------------------------+
 * | LShift    |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |  RShift   | FN1 |
 * |-----------------------------------------------------------------------------------------+
 * |              LGUI |     LAlt  |            Space       |   RAlt    |    RCTRL           |
 * `-----------------------------------------------------------------------------------------'
 */
	LAYOUT_60_hhkb(
		KC_GESC,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,  KC_BSLS, KC_DEL,
		KC_TAB,           KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC, KC_RBRC, KC_BSPC,
		KC_LCTL,          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT, KC_ENT,
		KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT, MO(1),
		            KC_LGUI,             KC_LALT,                      KC_SPC,                    KC_RALT,           KC_RCTL),


/* FN1 LAYER
 *
 *  ,---------------------------------------------------------------------------------------------------------------------
 * |  KC_GRV | F1   | F2   | F3   | F4    | F5    | F6    | F7   | F8    | F9   | F10   | F11   | F12   | INSERT | RESET |
 * |---------------------------------------------------------------------------------------------------------------------+
 * |  RGB_TOG |   _  |   _  |   _   |    _  |    _  |    _  |    _  |  PSCR  |  SLCK  |  PAUS  |  UP   |   _   |   CLR   |
 * |---------------------------------------------------------------------------------------------------------------------+
 * |   CAPS     | VOLD | VOLU | MUTE |   _   |   _    |  PAST  |  PSLS  |  HOME  | PGUP  |  LEFT  |  RIGHT  |   RETURN   |
 * |---------------------------------------------------------------------------------------------------------------------+
 * |     _           |  _   |  _   |  _   |  _   |  _   |  PPLS  |  PMNS  |  END  |   PGDN  |  DOWN  |      _      |  _  |
 * |---------------------------------------------------------------------------------------------------------------------+
 * |                          _    |      _     |               _               |     APP      |     _                   |
 * `---------------------------------------------------------------------------------------------------------------------'
 */
	LAYOUT_60_hhkb(
		KC_GRV, KC_F1,   KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,   KC_F10,  KC_F11,   KC_F12,    KC_INS,   RESET,
		RGB_TOG,         _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PSCR, KC_SLCK, KC_PAUS,  KC_UP,     _______,   KC_CLR,
		KC_CAPS,         KC_VOLD, KC_VOLU,   KC_MUTE,  _______,  _______,  KC_PAST,  KC_PSLS,  KC_HOME, KC_PGUP, KC_LEFT,  KC_RIGHT,  KC_RETURN,
		_______,         _______,  _______,  _______,  _______,  _______,  KC_PPLS,  KC_PMNS,  KC_END,  KC_PGDN, KC_DOWN,  _______,   _______,
		        _______,           _______,                    _______,                             KC_APP,            _______),
};
