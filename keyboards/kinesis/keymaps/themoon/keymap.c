#include "kinesis.h"

#define __________ KC_TRNS
#define XXXXXXX KC_NO

// mac layers: ins + int4 (before first plug in)
#define _MAC 0
#define _PALM 1
#define _MAC_CMD_ESC 2
#define _MAC_CMD_SPACE 3
#define _MAC_DEL 4

// win layers: int + int5
#define _WIN 5

enum kinesis_keycodes {
  _M_CMD_ESC = LT(_MAC_CMD_ESC, KC_ESC),
  _M_CMD_SPACE = LT(_MAC_CMD_SPACE, KC_SPC),
  _M_DEL = LT(_MAC_DEL, KC_BSPC),
  _P = MO(_PALM),

  SHIFT_TAB = SFT_T(KC_TAB),

  ALT_SLASH = MT(MOD_LALT, KC_SLSH),
  ALT_BSLASH = MT(MOD_RALT, KC_BSLS),

  CTRL_DEL = MT(MOD_LCTL, KC_DEL),
  CTRL_F17 = MT(MOD_RCTL, KC_F17),
};

enum holding_keycodes {
  MOD_ESC = SAFE_RANGE,
  MOD_ENTER,
  MOD_W,
  MOD_E,
  MOD_R,
  MOD_T,
  MOD_S,
  MOD_D,
  MOD_F,
  MOD_G,
  MOD_X,
  MOD_C,
  MOD_V,
  MOD_B,
  MOD_LBRC,
  MOD_RBRC,

  MOD_SPACE,
  MOD_LEFT,
  MOD_RIGHT,
  MOD_UP,
  MOD_DOWN,
  MOD_K,
  MOD_U,
  MOD_I,
  MOD_O,
  MOD_N,
  MOD_Y,

  CTRL_COMMA, // control + ,
  CTRL_DOT, // control + .
  CTRL_H, // control + H
  CTRL_M, // control + M

  CTRL_TAB, // control + Tab

  ALT_BSPC, // alt + backspace
};

enum {
    MAIL = 0,
    SLEEP,
};

/****************************************************************************************************
*
* Keymap: Default Layer in Qwerty
*
* ,-------------------------------------------------------------------------------------------------------------------.
* | Power  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | PSCR | SLCK | PAUS |  FN0 |Program |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* | =+     |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  | -_     |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | `~     |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  | Mail   |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | CAPS   |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  | F20    |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | F19    |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  | F21    |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |Insert|  [{  |  ,<  |  ]}  |                                         | Left | Down | Right| M2   |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,-------------------------.
*                            | LShift+LAlt/BkSp |Trash |         | Sleep| LShift+LAlt/BkSp |
*                            `-----------|------|------|         |------+------+-----------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LCMD/|Shift/|------|         |------|Shift/|RCMD/ |
*                                 | ESC  |Enter |Ctrl/Del|     |Ctrl/F17|Tab   |SPACE |
*                                 `--------------------'         `--------------------'
*                            PALM                                                      PALM
*/

// base layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_MAC] = KEYMAP(
           // left side
           KC_POWER, KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8,
           KC_EQL, KC_1, KC_2, KC_3, KC_4, KC_5,
           KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_CAPSLOCK,KC_A, KC_S, KC_D, KC_F, KC_G,
           KC_F19 ,KC_Z, KC_X, KC_C, KC_V, KC_B,
                 KC_INS, KC_LBRC, KC_COMM, KC_RBRC,
                                           // left thumb keys
			                                    _M_DEL,LGUI(KC_BSPC),
                                                   ALT_SLASH,
                           _M_CMD_ESC, KC_SFTENT, CTRL_DEL,
                                     // left palm key
			                         _P,
    // right side
    KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,KC_PSCR ,KC_SLCK  ,KC_PAUS, KC_FN0, RESET,
	KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS,
	KC_Y, KC_U, KC_I, KC_O, KC_P, M(MAIL),
	KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_F20,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F21,
	KC_LEFT, KC_DOWN, KC_RGHT, KC_MS_BTN2,
           // right thumb keys
           M(SLEEP),_M_DEL,
           ALT_BSLASH,
           CTRL_F17, SHIFT_TAB, _M_CMD_SPACE,
                                    // right palm key
                                    _P
    ),

[_MAC_CMD_ESC] = KEYMAP(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,   __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    _M_CMD_ESC, __________,  __________,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         MOD_Y,  MOD_U,  MOD_I,  MOD_O,  __________,  __________,
         CTRL_H,  __________,  MOD_K,  __________,  __________,  __________,
         MOD_N,  CTRL_M,  MOD_UP,  CTRL_DOT ,  __________,  __________,
                   MOD_LEFT,  MOD_DOWN,  MOD_RIGHT, __________,
         KC_F15,  ALT_BSPC,
         __________,
         KC_F17,  CTRL_TAB,  MOD_SPACE,
                             __________
    ),

[_MAC_CMD_SPACE] = KEYMAP(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  MOD_W,  MOD_E,  MOD_R,  MOD_T,
         __________,  __________,  MOD_S,  MOD_D,  MOD_F,  MOD_G,
         __________,  __________,  MOD_X,  MOD_C,  MOD_V,  MOD_B,
                   __________,  MOD_LBRC,  CTRL_COMMA,  MOD_RBRC,
                             ALT_BSPC,  __________,
                                        KC_Z,
                    MOD_ESC, MOD_ENTER, LSFT(KC_Z),
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________,  _M_CMD_SPACE,
                                     __________
    ),

[_MAC_DEL] = KEYMAP(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             _M_DEL,  __________,
                                       __________,
                    __________, __________,  __________,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  _M_DEL,
         __________,
         __________,  __________,  __________,
                             __________
    ),

[_WIN] = KEYMAP(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  KC_0,  KC_1,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________,  __________,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                             __________
    ),

[_PALM] = KEYMAP(
__________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  KC__VOLDOWN,  KC__MUTE,  KC__VOLUP,
                             KC_F1,  KC_F2,
                                     KC_F3,
                      KC_F4, KC_F5,  KC_F6,
                                     _P,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_PGUP,  __________ ,  __________,  __________,
                   KC_HOME,  KC_PGDN, KC_END, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                             _P
    ),
};

const uint16_t PROGMEM fn_actions[] = {
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
 switch(id) {

    case MAIL: {
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

bool not_held(uint16_t hold_timer, uint8_t hold_duration) {
    return timer_elapsed(hold_timer) < hold_duration;
}

void key_code(uint16_t key) {
            register_code(key);
            unregister_code(key);
}

void with_1_mod(uint16_t key, uint16_t mod1) {
            register_code(mod1);
            key_code(key);
            unregister_code(mod1);
}

void with_2_mods(uint16_t key, uint16_t mod1, uint16_t mod2) {
            register_code(mod2);
            with_1_mod(key, mod1);
            unregister_code(mod2);
}

void with_3_mods(uint16_t key, uint16_t mod1, uint16_t mod2, uint16_t mod3) {
            register_code(mod3);
            with_2_mods(key, mod1, mod2);
            unregister_code(mod3);
}

bool ___if_held___add_mod(uint16_t code, uint16_t mod_to_add, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (not_held(hold_timer, hold_duration)){
          key_code(code);
      } else {
          with_1_mod(code, mod_to_add);
      }
  }
  return false;
}

bool ___if_held_replace_add_mod(uint16_t code, uint16_t replacement_code, uint16_t mod_if_held, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (not_held(hold_timer, hold_duration)){
          key_code(code);
      } else {
          with_1_mod(replacement_code, mod_if_held);
      }
  }
  return false;
}

bool ___if_held_replace(uint16_t code, uint16_t replacement_code, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (not_held(hold_timer, hold_duration)){
          key_code(code);
      } else {
          key_code(replacement_code);
      }
  }
  return false;
}

bool add_mod____if_held___add_mod(uint16_t code, uint16_t mod, uint16_t mod_if_held, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();

  } else {
      if (not_held(hold_timer, hold_duration)){
          with_1_mod(code, mod);
      } else {
          with_2_mods(code, mod, mod_if_held);
      }
  }
  return false;
}

bool replace_mod1_with_mod2____if_held___mod3_plus_mod4(uint16_t code, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();

  } else {
      unregister_code(mod1);

      if (not_held(hold_timer, hold_duration)){
          with_1_mod(code, mod2);
          register_code(mod1);

      } else {
          with_2_mods(code, mod3, mod4);
      }

      register_code(mod1);
  }
  return false;
}

bool replace_cmd_with_mod____if_held___cmd_shift(uint16_t code, uint16_t replacement_mod, bool pressed, uint8_t hold_duration) {
  return replace_mod1_with_mod2____if_held___mod3_plus_mod4(code, KC_LGUI, replacement_mod, KC_LGUI, KC_LSFT, pressed, hold_duration);
}

bool no_meh(uint16_t code, bool pressed) {
   unregister_code(KC_LSFT);
   unregister_code(KC_LALT);
   unregister_code(KC_LCTL);

   if (pressed) {
       key_code(code);
   }

   register_code(KC_LCTL);
   register_code(KC_LALT);
   register_code(KC_LSFT);

  return false;
}

bool ___if_held___add_shift(uint16_t code, bool pressed, uint8_t hold_duration) {
  return ___if_held___add_mod(code, KC_LSFT, pressed, hold_duration);
}

bool ___if_held_140___add_shift(uint16_t code, bool pressed) {
  return ___if_held___add_shift(code, pressed, 140);
}

bool ___if_held_200___add_shift(uint16_t code, bool pressed) {
  return ___if_held___add_shift(code, pressed, 200);
}


bool ___if_held_200___replace_add_mod(uint16_t code, uint16_t replacement_code, uint16_t mod_if_held, bool pressed) {
  return ___if_held_replace_add_mod(code, replacement_code, mod_if_held, pressed, 200);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // no holding delay
        case KC_PGUP: {return no_meh(KC_PGUP, record->event.pressed); }
        case KC_PGDN: {return no_meh(KC_PGDN, record->event.pressed); }
        case KC_HOME: {return no_meh(KC_HOME, record->event.pressed); }
        case KC_END: {return no_meh(KC_END, record->event.pressed); }
        case KC__VOLDOWN: {return no_meh(KC__VOLDOWN, record->event.pressed); }
        case KC__MUTE: {return no_meh(KC__MUTE, record->event.pressed); }
        case KC__VOLUP: {return no_meh(KC__VOLUP, record->event.pressed); }

        // 140 ms
        case MOD_LEFT: {return ___if_held_140___add_shift(KC_LEFT, record->event.pressed); }
        case MOD_RIGHT:{return ___if_held_140___add_shift(KC_RIGHT, record->event.pressed); }
        case MOD_UP:{return ___if_held_140___add_shift(KC_UP, record->event.pressed); }
        case MOD_DOWN:{return ___if_held_140___add_shift(KC_DOWN, record->event.pressed); }
        case MOD_SPACE: {return ___if_held_140___add_shift(KC_SPC, record->event.pressed); }
        case MOD_ESC: {return ___if_held_140___add_shift(KC_ESC, record->event.pressed); }
        case MOD_ENTER: {return ___if_held_140___add_shift(KC_ENTER, record->event.pressed); }

        // 200 ms
        case MOD_K: {return ___if_held_200___add_shift(KC_K, record->event.pressed); }
        case MOD_U: {return ___if_held_200___add_shift(KC_U, record->event.pressed); }
        case MOD_I: {return ___if_held_200___add_shift(KC_I, record->event.pressed); }
        case MOD_O: {return ___if_held_200___add_shift(KC_O, record->event.pressed); }
        case MOD_N: {return ___if_held_200___add_shift(KC_N, record->event.pressed); }
        case MOD_Y: {return ___if_held_200___add_shift(KC_Y, record->event.pressed); }
        case MOD_W: {return ___if_held_200___add_shift(KC_W, record->event.pressed); }
        case MOD_E: {return ___if_held_200___add_shift(KC_E, record->event.pressed); }
        case MOD_R: {return ___if_held_200___add_shift(KC_R, record->event.pressed); }
        case MOD_T: {return ___if_held_200___add_shift(KC_T, record->event.pressed); }
        case MOD_S: {return ___if_held_200___add_shift(KC_S, record->event.pressed); }
        case MOD_D: {return ___if_held_200___add_shift(KC_D, record->event.pressed); }
        case MOD_F: {return ___if_held_200___add_shift(KC_F, record->event.pressed); }
        case MOD_G: {return ___if_held_200___add_shift(KC_G, record->event.pressed); }
        case MOD_X: {return ___if_held_200___add_shift(KC_X, record->event.pressed); }
        case MOD_C: {return ___if_held_200___add_shift(KC_C, record->event.pressed); }
        case MOD_V: {return ___if_held_200___add_shift(KC_V, record->event.pressed); }
        case MOD_B: {return ___if_held_200___add_shift(KC_B, record->event.pressed); }
        case MOD_LBRC: {return ___if_held_200___add_shift(KC_LBRC, record->event.pressed); }
        case MOD_RBRC: {return ___if_held_200___add_shift(KC_RBRC, record->event.pressed); }

        case CTRL_H: {return replace_cmd_with_mod____if_held___cmd_shift(KC_H, KC_LCTL, record->event.pressed, 200); }
        case CTRL_COMMA: {return replace_cmd_with_mod____if_held___cmd_shift(KC_COMM, KC_LCTL, record->event.pressed, 200); }
        case CTRL_DOT: {return replace_cmd_with_mod____if_held___cmd_shift(KC_DOT, KC_LCTL, record->event.pressed, 200); }
        case CTRL_M: {return replace_cmd_with_mod____if_held___cmd_shift(KC_M, KC_LCTL, record->event.pressed, 200); }

        case CTRL_TAB: {return replace_mod1_with_mod2____if_held___mod3_plus_mod4(KC_TAB, KC_LGUI, KC_LCTL, KC_LCTL, KC_LSFT, record->event.pressed, 200); }

        case ALT_BSPC: {return replace_cmd_with_mod____if_held___cmd_shift(KC_BSPC, KC_LALT, record->event.pressed, 200); }

        case KC_2: {return ___if_held_200___replace_add_mod(KC_2, KC_9, KC_LSFT, record->event.pressed); }
        case KC_3: {return ___if_held_200___replace_add_mod(KC_3, KC_MINS, KC_LSFT, record->event.pressed); }
        case KC_4: {return ___if_held_200___replace_add_mod(KC_4, KC_0, KC_LSFT, record->event.pressed); }
        case KC_5: {return ___if_held_200___replace_add_mod(KC_5, KC_EQL, KC_NO, record->event.pressed); }
        case KC_6: {return ___if_held_200___replace_add_mod(KC_6, KC_EQL, KC_LSFT, record->event.pressed); }
        case KC_7: {return ___if_held_200___replace_add_mod(KC_7, KC_1, KC_LSFT, record->event.pressed); }
        case KC_8: {return ___if_held_200___replace_add_mod(KC_8, KC_MINS, KC_NO, record->event.pressed); }
        case KC_9: {return ___if_held_200___replace_add_mod(KC_9, KC_SLSH, KC_LSFT, record->event.pressed); }
        case KC_F5: {return ___if_held_200___replace_add_mod(KC_F5, KC_COMM, KC_LGUI, record->event.pressed); }

        case KC_F1: {return ___if_held_200___add_shift(KC_F1, record->event.pressed); }
        case KC_F2: {return ___if_held_200___add_shift(KC_F2, record->event.pressed); }
        case KC_F3: {return ___if_held_200___add_shift(KC_F3, record->event.pressed); }
        case KC_F4: {return ___if_held_200___add_shift(KC_F4, record->event.pressed); }
        case KC_F6: {return ___if_held_200___add_shift(KC_F6, record->event.pressed); }
        case KC_F7: {return ___if_held_200___add_shift(KC_F7, record->event.pressed); }
        case KC_F8: {return ___if_held_200___add_shift(KC_F8, record->event.pressed); }
        case KC_F9: {return ___if_held_200___add_shift(KC_F9, record->event.pressed); }
        case KC_F10: {return ___if_held_200___add_shift(KC_F10, record->event.pressed); }
        case KC_F11: {return ___if_held_200___add_shift(KC_F11, record->event.pressed); }
        case KC_F12: {return ___if_held_200___add_shift(KC_F12, record->event.pressed); }

        default:
            return true;
    }
}

void led_set_user(uint8_t usb_led) {
}

uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    case _MAC_CMD_ESC:
      register_code(KC_LGUI);
      break;
    case _MAC_CMD_SPACE:
      register_code(KC_LGUI);
      break;
    case _MAC_DEL:
      register_code(KC_LSFT);
      register_code(KC_LALT);
      break;
    case _PALM:
      register_code(KC_LSFT);
      register_code(KC_LALT);
      register_code(KC_LCTL);
      break;
    default:
      clear_mods();
      break;
    }
return state;
}
