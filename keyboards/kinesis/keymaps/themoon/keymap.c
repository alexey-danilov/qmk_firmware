#include "kinesis.h"

#define _______ KC_TRNS
#define XXXXXXX KC_NO

#define QWERTY 0
#define _CMD_ESC 1 // command layer
#define _CMD_SPACE 2 // command layer
#define _CTRL_ALT 3 // ctrl + alt layer

enum kinesis_keycodes {
  _L_CMD_ESC = LT(_CMD_ESC, KC_ESC),
  _L_CMD_SPACE = LT(_CMD_SPACE, KC_SPC),

  _L_PALM1 = LT(_CTRL_ALT, KC_F14),
  _L_PALM2 = LT(_CTRL_ALT, KC_F16),

  L_DEL = MT(MOD_LSFT | MOD_LALT, KC_BSPC),
  R_DEL = MT(MOD_RSFT | MOD_RALT, KC_BSPC),

  SHIFT_TAB = SFT_T(KC_TAB),

  ALT_SLASH = MT(MOD_LALT, KC_SLSH),
  ALT_BSLASH = MT(MOD_RALT, KC_BSLS),

  CTRL_F3 = MT(MOD_LCTL, KC_F3),
  CTRL_F18 = MT(MOD_RCTL, KC_F18),
};

enum holding_keycodes {
  HOLD_1 = SAFE_RANGE,
  HOLD_2,
  HOLD_3,
  HOLD_4,
  HOLD_5,
  HOLD_6,
  HOLD_7,
  HOLD_8,
  HOLD_9,
  HOLD_0,
  HOLD_ESC,
  HOLD_LBRC,
  HOLD_RBRC,

  HOLD_F1,
  HOLD_F2,
  HOLD_F3,
  HOLD_F4,
  HOLD_F5,
  HOLD_F6,
  HOLD_F7,
  HOLD_F8,
  HOLD_F9,
  HOLD_F10,
  HOLD_F11,
  HOLD_F12,

  // cmd layer
  CMD_F1,
  CMD_ENTER,
  CMD_W,
  CMD_E,
  CMD_R,
  CMD_T,
  CMD_S,
  CMD_D,
  CMD_F,
  CMD_G,
  CMD_X,
  CMD_C,
  CMD_V,
  CMD_B,

  CMD_SPACE,
  CMD_LEFT,
  CMD_RIGHT,
  CMD_UP,
  CMD_DOWN,
  CMD_K,

  CMD_UNDO, // command + z
  CMD_REDO, // command + shift + z

  CMD_TAB, // control + tab
  CMD_COMMA, // control + ,
  CMD_DOT, // control + .
  CMD_H, // control + H
  CMD_BSPC, // alt + backspace
  CMD_M, // control + M

  CTRL_ALT_P_UP,
  CTRL_ALT_P_DN,
  CTRL_ALT_HOME,
  CTRL_ALT_END,

  TRASH,
};

enum {
    EMAIL_MACRO = 0,
    SLEEP
};

/****************************************************************************************************
*
* Keymap: Default Layer in Qwerty
*
* ,-------------------------------------------------------------------------------------------------------------------.
* |ESC/HIDE|  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | PSCR | SLCK | PAUS |  FN0 |  Reset |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* | =+     |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  | -_     |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | `~     |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | CAPS   |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  |CAPS    |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |        |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  |        |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |EMAIL |  [{  |  ,.  |  ]}  |                                         | Left | Down | Right|MOUSE2|
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,-------------------------.
*                            | LShift+LAlt/BkSp |Trash |         | Sleep| LShift+LAlt/BkSp |
*                            `-----------|------|------|         |------+------+-----------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LCMD/|Shift/|------|         |------|Shift/|LCMD/ |
*                                 | ESC  |Enter |Ctrl/F3 |     |Ctrl/F18|Tab   |SPACE |
*                                 `--------------------'         `--------------------'
*                   LCtrl+LAlt/F14                                                     RCtrl+RAlt/F16
*/

// base layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWERTY] = KEYMAP(
           // left side
           HOLD_ESC, HOLD_F1  ,HOLD_F2  ,HOLD_F3  ,HOLD_F4  ,HOLD_F5  ,HOLD_F6  ,HOLD_F7  ,HOLD_F8,
           KC_EQL, HOLD_1, HOLD_2   ,HOLD_3   ,HOLD_4   ,HOLD_5   ,
           KC_GRV, KC_Q   ,KC_W   ,KC_E   ,KC_R   ,KC_T   ,
           KC_CAPS,KC_A   ,KC_S   ,KC_D   ,KC_F   ,KC_G   ,
           XXXXXXX,KC_Z   ,KC_X   ,KC_C   ,KC_V   ,KC_B   ,
                   M(EMAIL_MACRO),HOLD_LBRC,KC_COMM,HOLD_RBRC,
                                           // left thumb keys
			                                    L_DEL,TRASH,
                                                   ALT_SLASH,
                           _L_CMD_ESC, KC_SFTENT, CTRL_F3,
                                     // left palm key
			                         _L_PALM1,
    // right side
    HOLD_F9  ,HOLD_F10 ,HOLD_F11 ,HOLD_F12 ,KC_PSCR ,KC_SLCK  ,KC_PAUS, KC_FN0, RESET,
	HOLD_6   ,HOLD_7   ,HOLD_8   ,HOLD_9   ,HOLD_0   ,KC_MINS,
	KC_Y   ,KC_U   ,KC_I   ,KC_O   ,KC_P   ,XXXXXXX,
	KC_H   ,KC_J   ,KC_K   ,KC_L   ,KC_SCLN,KC_CAPS,
	KC_N   ,KC_M   ,KC_UP  ,KC_DOT ,KC_QUOT,XXXXXXX,
	    	KC_LEFT,KC_DOWN,KC_RGHT,KC_MS_BTN2,
           // right thumb keys
           M(SLEEP),R_DEL,
           ALT_BSLASH,
           CTRL_F18, SHIFT_TAB, _L_CMD_SPACE,
                                    // right palm key
                                    _L_PALM2
    ),

[_CMD_ESC] = KEYMAP(
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
                   _______,  _______,  _______,  _______,
                             _______,  _______,
                                       _______,
                    _L_CMD_ESC, _______,  _______,
                                     _______,
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         CMD_H,  _______,  CMD_K,  _______,  _______,  _______,
         _______,  CMD_M,  CMD_UP,  CMD_DOT ,  _______,  _______,
                   CMD_LEFT,  CMD_DOWN,  CMD_RIGHT, _______,
         _______,  CMD_BSPC,
         _______,
         _______,  CMD_TAB,  CMD_SPACE,
                             _______
    ),

[_CMD_SPACE] = KEYMAP(
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  CMD_W,  CMD_E,  CMD_R,  CMD_T,
         _______,  _______,  CMD_S,  CMD_D,  CMD_F,  CMD_G,
         _______,  _______,  CMD_X,  CMD_C,  CMD_V,  CMD_B,
                   _______,  _______,  CMD_COMMA,  _______,
                             CMD_BSPC,  _______,
                                       CMD_UNDO,
                    CMD_F1, CMD_ENTER,  CMD_REDO,
                                     _______,
          _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
          _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,
                   _______,  _______,  _______,  _______,
                             _______,  _______,
                                       _______,
                    _______, _______,  _L_CMD_SPACE,
                                     _______
    ),

// ctrl alt layer
[_CTRL_ALT] = KEYMAP(
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
          _______,  _______,  _______, _______,_______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,
                   _______,  _______,  _______,  _______,
                             _______,  _______,
                                       _______,
                    _______, _______,  _______,
                                     _L_PALM1,
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  CTRL_ALT_P_UP,  _______ ,  _______,  _______,
                   CTRL_ALT_HOME,  CTRL_ALT_P_DN, CTRL_ALT_END, _______,
         _______,  _______,
         _______,
         _______,  _______,  _______,
                             _L_PALM2
    ),
};

const uint16_t PROGMEM fn_actions[] = {
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
 switch(id) {

    case EMAIL_MACRO: {
        if (record->event.pressed) {
            SEND_STRING("oleksii.danilov@gmail.com");
            return false;
        }
    }

    case SLEEP: {
        if (record->event.pressed) {
            SEND_STRING(SS_DOWN(X_LCTRL) SS_DOWN(X_LSHIFT) SS_DOWN(X_POWER) SS_UP(X_POWER) SS_UP(X_LSHIFT) SS_UP(X_LCTRL));
            return false;
        }
    }
  }
    return MACRO_NONE;
};

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

bool add_mod_on_hold(uint16_t code, uint16_t mod_to_add, bool pressed) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (timer_elapsed(hold_timer) < 150){
          register_code(code);
          unregister_code(code);
      } else {
          register_code(mod_to_add);
          register_code(code);
          unregister_code(code);
          unregister_code(mod_to_add);
      }
  }
  return false;
}

bool add_shift_on_hold(uint16_t code, bool pressed) {
  return add_mod_on_hold(code, KC_LSFT, pressed);
}

bool replace_on_hold_with_mod(uint16_t code, uint16_t replacement_code, uint16_t mod_if_held, bool pressed) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (timer_elapsed(hold_timer) < 150){
          register_code(code);
          unregister_code(code);
      } else {
          register_code(mod_if_held);
          register_code(replacement_code);
          unregister_code(replacement_code);
          unregister_code(mod_if_held);
      }
  }
  return false;
}

bool replace_modded_on_hold(uint16_t code, uint16_t mod, uint16_t replacement_code, bool pressed) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (timer_elapsed(hold_timer) < 150){
          register_code(mod);
          register_code(code);
          unregister_code(code);
          unregister_code(mod);
      } else {
          register_code(replacement_code);
          unregister_code(replacement_code);
      }
  }
  return false;
}

bool force_mod_plus_mod_if_held(uint16_t code, uint16_t mod, uint16_t mod_if_held, bool pressed) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();

  } else {
      uint8_t old_mods = get_mods();
      if (old_mods) {
        clear_mods();
      }

      if (timer_elapsed(hold_timer) < 150){
          register_code(mod);
          register_code(code);
          unregister_code(code);
          unregister_code(mod);
      } else {
          register_code(mod_if_held);
          register_code(mod);
          register_code(code);
          unregister_code(code);
          unregister_code(mod);
          unregister_code(mod_if_held);
      }

      if (old_mods) {
        set_mods(old_mods);
      }
  }
  return false;
}

bool force_mods_plus_mod_if_held(uint16_t code, uint16_t mod1, uint16_t mod2, uint16_t mod_if_held, bool pressed) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();

  } else {
      uint8_t old_mods = get_mods();
      if (old_mods) {
        clear_mods();
      }

      if (timer_elapsed(hold_timer) < 150){
          register_code(mod1);
          register_code(mod2);
          register_code(code);
          unregister_code(code);
          unregister_code(mod2);
          unregister_code(mod1);
      } else {
          register_code(mod_if_held);
          register_code(mod1);
          register_code(mod2);
          register_code(code);
          unregister_code(code);
          unregister_code(mod2);
          unregister_code(mod1);
          unregister_code(mod_if_held);
      }

      if (old_mods) {
        set_mods(old_mods);
      }
  }
  return false;
}

bool force_ctrl_plus_shift_if_held(uint16_t code, bool pressed) {
 return force_mod_plus_mod_if_held(code, KC_LCTL, KC_LSFT, pressed);
}

bool without_ctrl_alt(uint16_t code, bool pressed) {
   unregister_code(KC_LCTL);
   unregister_code(KC_LALT);

   if (pressed) {
     register_code(code);
     unregister_code(code);
   }

   register_code(KC_LALT);
   register_code(KC_LCTL);

  return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HOLD_1: {return replace_on_hold_with_mod(KC_1, KC_PSCR, KC_NO, record->event.pressed); }
        case HOLD_2: {return replace_on_hold_with_mod(KC_2, KC_9, KC_LSFT, record->event.pressed); }
        case HOLD_3: {return replace_on_hold_with_mod(KC_3, KC_MINS, KC_LSFT, record->event.pressed); }
        case HOLD_4: {return replace_on_hold_with_mod(KC_4, KC_0, KC_LSFT, record->event.pressed); }
        case HOLD_5: {return replace_on_hold_with_mod(KC_5, KC_EQL, KC_LSFT, record->event.pressed); }
        case HOLD_6: {return replace_on_hold_with_mod(KC_6, KC_EQL, KC_NO, record->event.pressed); }
        case HOLD_7: {return replace_on_hold_with_mod(KC_7, KC_1, KC_LSFT, record->event.pressed); }
        case HOLD_8: {return replace_on_hold_with_mod(KC_8, KC_MINS, KC_NO, record->event.pressed); }
        case HOLD_9: {return replace_on_hold_with_mod(KC_9, KC_SLSH, KC_LSFT, record->event.pressed); }
        case HOLD_0: {return replace_on_hold_with_mod(KC_0, KC_BSLS, KC_LSFT, record->event.pressed); }

        case HOLD_ESC: {return replace_on_hold_with_mod(KC_ESC, KC_F12, KC_LCTL, record->event.pressed); }
        case HOLD_LBRC: {return replace_modded_on_hold(KC_LBRC, KC_LSFT, KC_LBRC, record->event.pressed); }
        case HOLD_RBRC: {return replace_modded_on_hold(KC_LBRC, KC_LSFT, KC_RBRC, record->event.pressed); }

        case HOLD_F1: {return add_shift_on_hold(KC_F1, record->event.pressed); }
        case HOLD_F2: {return add_shift_on_hold(KC_F2, record->event.pressed); }
        case HOLD_F3: {return add_shift_on_hold(KC_F3, record->event.pressed); }
        case HOLD_F4: {return add_shift_on_hold(KC_F4, record->event.pressed); }
        case HOLD_F5: {return add_shift_on_hold(KC_F5, record->event.pressed); }
        case HOLD_F6: {return add_shift_on_hold(KC_F6, record->event.pressed); }
        case HOLD_F7: {return add_shift_on_hold(KC_F7, record->event.pressed); }
        case HOLD_F8: {return add_shift_on_hold(KC_F8, record->event.pressed); }
        case HOLD_F9: {return add_shift_on_hold(KC_F9, record->event.pressed); }
        case HOLD_F10: {return add_shift_on_hold(KC_F10, record->event.pressed); }
        case HOLD_F11: {return add_shift_on_hold(KC_F11, record->event.pressed); }
        case HOLD_F12: {return add_shift_on_hold(KC_F12, record->event.pressed); }

        case CMD_LEFT: {return add_shift_on_hold(KC_LEFT, record->event.pressed); }
        case CMD_RIGHT:{return add_shift_on_hold(KC_RIGHT, record->event.pressed); }
        case CMD_UP:{return add_shift_on_hold(KC_UP, record->event.pressed); }
        case CMD_DOWN:{return add_shift_on_hold(KC_DOWN, record->event.pressed); }
        case CMD_K: {return add_shift_on_hold(KC_K, record->event.pressed); }
        case CMD_SPACE: {return add_shift_on_hold(KC_SPC, record->event.pressed); }
        case CMD_F1: {return add_shift_on_hold(KC_F1, record->event.pressed); }
        case CMD_ENTER: {return add_shift_on_hold(KC_ENTER, record->event.pressed); }
        case CMD_W: {return add_shift_on_hold(KC_W, record->event.pressed); }
        case CMD_E: {return add_shift_on_hold(KC_E, record->event.pressed); }
        case CMD_R: {return add_shift_on_hold(KC_R, record->event.pressed); }
        case CMD_T: {return add_shift_on_hold(KC_T, record->event.pressed); }
        case CMD_S: {return add_shift_on_hold(KC_S, record->event.pressed); }
        case CMD_D: {return add_shift_on_hold(KC_D, record->event.pressed); }
        case CMD_F: {return add_shift_on_hold(KC_F, record->event.pressed); }
        case CMD_G: {return add_shift_on_hold(KC_G, record->event.pressed); }
        case CMD_X: {return add_shift_on_hold(KC_X, record->event.pressed); }
        case CMD_C: {return add_shift_on_hold(KC_C, record->event.pressed); }
        case CMD_V: {return add_shift_on_hold(KC_V, record->event.pressed); }
        case CMD_B: {return add_shift_on_hold(KC_B, record->event.pressed); }

        case CMD_UNDO: {return add_mod_on_hold(KC_Z, KC_LCTL, record->event.pressed); }
        case CMD_REDO: {return force_mods_plus_mod_if_held(KC_Z, KC_LSFT, KC_LGUI, KC_LCTL, record->event.pressed); }

        case CMD_TAB: {return force_ctrl_plus_shift_if_held(KC_TAB, record->event.pressed); }
        case CMD_COMMA: {return force_ctrl_plus_shift_if_held(KC_COMM, record->event.pressed); }
        case CMD_DOT: {return force_ctrl_plus_shift_if_held(KC_DOT, record->event.pressed); }
        case CMD_H: {return force_ctrl_plus_shift_if_held(KC_H, record->event.pressed); }
        case CMD_M: {return force_ctrl_plus_shift_if_held(KC_M, record->event.pressed); }
        case CMD_BSPC: {return force_mod_plus_mod_if_held(KC_BSPC, KC_LALT, KC_LSFT, record->event.pressed); }

        case CTRL_ALT_P_UP: {return without_ctrl_alt(KC_PGUP, record->event.pressed); }
        case CTRL_ALT_P_DN: {return without_ctrl_alt(KC_PGDN, record->event.pressed); }
        case CTRL_ALT_HOME: {return without_ctrl_alt(KC_HOME, record->event.pressed); }
        case CTRL_ALT_END: {return without_ctrl_alt(KC_END, record->event.pressed); }

        case TRASH: {return replace_on_hold_with_mod(KC_DEL, KC_BSPC, KC_LGUI, record->event.pressed); }

        default:
            return true;
    }
}

void led_set_user(uint8_t usb_led) {
}

uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    case _CMD_ESC:
      register_code(KC_LGUI);
      break;
    case _CMD_SPACE:
      register_code(KC_LGUI);
      break;
    case _CTRL_ALT:
      register_code(KC_LCTL);
      register_code(KC_LALT);
      break;
    default:
      clear_mods();
      break;
    }
return state;
}
