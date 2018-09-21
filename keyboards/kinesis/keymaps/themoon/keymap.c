#include "kinesis.h"

#define __________ KC_TRNS
#define XXXXXXX KC_NO

// mac layers
#define _MAC 0
#define _COMMAND_ESCAPE 1
#define _COMMAND_SPACE 2
#define _ALT_SHIFT_BS 3
#define _CTRL 4

// win layers
#define _WIN 5
#define _CONTROL_ESCAPE 6
#define _CONTROL_SPACE 7
#define _CTRL_SHIFT_BS 8

#define _PALM_L 9
#define _PALM_R 10
#define _ALT 11

enum kinesis_keycodes {
  // mac
  SET_LAYER_MAC,
  CMD_ESC = MO(_COMMAND_ESCAPE),
  CMD_SPACE = LT(_COMMAND_SPACE, KC_SPC),
  ALT_SHIFT_BS = MO(_ALT_SHIFT_BS),
  CTRL_CMD_BS = MO(_CTRL),
  CTRL_F16 = LT(_CTRL, KC_F16),

  // win
  SET_LAYER_WIN,
  CTRL_ESC = MO(_CONTROL_ESCAPE),
  CTRL_SPACE = LT(_CONTROL_SPACE, KC_SPC),
  CTRL_SHIFT_BS = MO(_CTRL_SHIFT_BS),
  LGUI_DEL = MT(MOD_LGUI, KC_DEL),

  // common
  ALT_SLASH = MO(_ALT),
  MEH_F13 = MO(_PALM_L),
  MEH_LAST_APP = MO(_PALM_R),
  ALT_BSLASH = MT(MOD_LALT, KC_BSLS),

  // media
  VOL_UP,
  VOL_DOWN,
  MUTE
};

enum holding_keycodes {
  MOD_ESC = SAFE_RANGE,
  MOD_W, MOD_E, MOD_R, MOD_T,
  MOD_S, MOD_D, MOD_F, MOD_G,
  MOD_X, MOD_C, MOD_V, MOD_B,
  MOD_LBRC, MOD_COMMA, MOD_RBRC,
  MOD_ENTER,

  MOD_SPACE,
  MOD_Y, MOD_U, MOD_I, MOD_O,
  MOD_H, MOD_J, MOD_K, MOD_L,
  MOD_N, MOD_M, MOD_UP, MOD_DOT,
  MOD_LEFT, MOD_DOWN, MOD_RIGHT,

  SHIFT_TAB,
  SHIFT_BSLS,

  LANG_CAPS,

  // mac-specific overrides
  CTRL_COMMA, CTRL_DOT, CTRL_H, CTRL_M, CMD_M,
  ALT_BSPC,

  // required for dynamic macros
  DYNAMIC_MACRO_RANGE
};

#include "dynamic_macro.h"

// macros
enum {
    MAIL = 0,
    CLOSE_APP,
    SLEEP,
    SHUTDOWN_WIN,
    DEL_WORD_WIN,
    DIR_LIST,
    DIR_UP,
    DELETE_FORCE,
    TERMINAL_CLEAR,
    DOCKER_LIST,
    DOCKER_LOGS,
    VIM_SAVE_QUIT,
    VIM_QUIT,
    CHANGE_LANG
};

// HELPER FUNCTIONS
// switch mac <-> win
static bool isMac = false;
static bool isWin = false;
void matrix_init_user(void) {
    switch (biton32(eeconfig_read_default_layer())) {
      case _MAC:
        isMac = true; isWin = false; break;
      case _WIN:
        isWin = true; isMac = false; break;
      default: break;
  }
}

uint16_t os_specific_key(uint16_t mac_key, uint16_t win_key) {
  if (isMac) { return mac_key; };
  if (isWin) { return win_key; };
  return KC_NO;
}

// sugar: for more clarity
void down(uint16_t key) {
  register_code(key);
}

void up(uint16_t key) {
  unregister_code(key);
}


void key_code(uint16_t key) {
            down(key);
            up(key);
}

void with_1_mod(uint16_t key, uint16_t mod1) {
            down(mod1);
            key_code(key);
            up(mod1);
}

void with_2_mods(uint16_t key, uint16_t mod1, uint16_t mod2) {
            down(mod2);
            with_1_mod(key, mod1);
            up(mod2);
}

void with_3_mods(uint16_t key, uint16_t mod1, uint16_t mod2, uint16_t mod3) {
            down(mod3);
            with_2_mods(key, mod1, mod2);
            up(mod3);
}

// defines whether keycode was held or not
bool not_held(uint16_t hold_timer, uint16_t hold_duration) {
    return timer_elapsed(hold_timer) < hold_duration;
}

// replaces single mod of keycode, adds additional mods if it was held for at least provided duration
bool replace_mods_hold_mods(uint16_t code, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();

  } else {
      up(mod1);

      if (not_held(hold_timer, hold_duration)){
          with_1_mod(code, mod2);

      } else {
          with_2_mods(code, mod3, mod4);
      }

      down(mod1);
  }
  return false;
}

// replaced command, if held adds shift to keycode
bool replace_cmd_hold_cmd_shift(uint16_t code, uint16_t replacement_mod, bool pressed, uint8_t hold_duration) {
  return replace_mods_hold_mods(code, KC_LGUI, replacement_mod, KC_LGUI, KC_LSFT, pressed, hold_duration);
}

// replaces keycode if it was held for at least provided duration
bool hold_replace(uint16_t code, uint16_t replacement_code, bool pressed, uint8_t hold_duration) {
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

// add mod to keycode if it was held for at least provided duration
bool hold_add_mods(uint16_t code, uint16_t mod1_to_add, uint16_t mod2_to_add, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (not_held(hold_timer, hold_duration)){
          key_code(code);
      } else {
          with_2_mods(code, mod1_to_add, mod2_to_add);
      }
  }
  return false;
}

// adds shift to keycode if it was held for at least provided duration
bool hold_shift(uint16_t code, bool pressed, uint8_t hold_duration) {
  return hold_add_mods(code, KC_LSFT, KC_NO, pressed, hold_duration);
}

// adds shift to keycode if it was held for at 140 ms
bool hold_140_add_shift(uint16_t code, bool pressed) {
  return hold_shift(code, pressed, 140);
}

// adds shift to keycode if it was held for at 180 ms
bool hold_180_add_shift(uint16_t code, bool pressed) {
  return hold_shift(code, pressed, 180);
}

// replaces keycode and adds mod to it if it was held for at least provided duration
bool hold_replace_add_mod(uint16_t code, uint16_t mod, uint16_t replacement_code, uint16_t replacement_mod1, uint16_t replacement_mod2, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (not_held(hold_timer, hold_duration)){
          with_1_mod(code, mod);
      } else {
          with_2_mods(replacement_code, replacement_mod1, replacement_mod2);
      }
  }
  return false;
}

// replaces keycode and adds mod to it if it was held for at least provided duration
bool hold_180_replace(uint16_t code, uint16_t replacement_code, uint16_t mod_if_held, bool pressed) {
  return hold_replace_add_mod(code, KC_NO, replacement_code, mod_if_held, KC_NO, pressed, 180);
}

// strips mods from keycode - without putting them back
bool without_mods(uint16_t code, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4) {
   up(mod1); up(mod2); up(mod3); up(mod4);
   key_code(code);
   return false;
}

// strips MEH from keycode - without putting it back
bool without_meh(uint16_t code) {
  return without_mods(code, KC_LCTL, KC_LALT, KC_LSFT, KC_NO);
}

// strips mods from keycode - and puts them back
bool without_mods_for_single_press(uint16_t code, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4, bool is_pressed) {
   if (is_pressed) {
     without_mods(code, mod1, mod2, mod3, mod4);
     down(mod1); down(mod2); down(mod3); down(mod4);
   }

   return false;
}

// handles repeat functionality (now only for palm keys)
static uint16_t palm_repeat_code;
static uint16_t palm_repeat_timer;
static uint8_t first_repeat_delay;
bool without_meh_repeat(uint16_t code, bool pressed) {
   if (pressed) {
       without_meh(code);
       palm_repeat_code = code;
       palm_repeat_timer = timer_read();
       first_repeat_delay = 250;
   } else {
       palm_repeat_code = 0;
   }

  return false;
}

// provides functionality similar to MT - except that mod is triggered immediately: this is useful when such mod is used with mouse;
// returns true if tap was triggered and false otherwise
bool mo_layer_tap(uint16_t tap_key, uint16_t tap_mod, uint16_t layer_mod1, uint16_t layer_mod2, uint16_t layer_mod3, uint16_t layer_mod4, uint16_t *layer_timer, bool *interrupted_flag, bool is_pressed, uint16_t hold_duration) {
  if (is_pressed) {
    *interrupted_flag = false;
    *layer_timer = timer_read();
  } else {
    if (not_held(*layer_timer, hold_duration)) {
      if (!*interrupted_flag) {
        // unregister mods associated with the layer, so that they don't intefere with the tap key
        up(layer_mod1); up(layer_mod2); up(layer_mod3); up(layer_mod4);

        // register tap key and its mod
        with_1_mod(tap_key, tap_mod);

        return true;
      }
    }
  }
  return false;
}

// provides functionality similar to "leader key", except that it works for non-dedicated key (currently, only escape)
bool after_leader(uint16_t key, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t *leader_timer, bool is_pressed, uint16_t esc_last_pressed_timeout) {
  if (*leader_timer && not_held(*leader_timer, esc_last_pressed_timeout)) {
    if (is_pressed) {
      *leader_timer = 0;
      with_3_mods(key, mod1, mod2, mod3);
      return false;
    }
  }
  return true;
}

enum {
  TAP_MACRO1 = 1,
  TAP_MACRO2 = 2
};


// dynamic macro1
// Whether the macro 1 is currently being recorded.
static bool is_macro1_recording = false;

// The current set of active layers (as a bitmask).
// There is a global 'layer_state' variable but it is set after the call
// to layer_state_set_user().
static uint32_t current_layer_state = 0;
uint32_t layer_state_set_user(uint32_t state);

// Method called at the end of the tap dance on the TAP_MACRO1 key. That key is
// used to start recording a macro (double tap or more), to stop recording (any
// number of tap), or to play the recorded macro (1 tap).
void macro1_tapdance_fn(qk_tap_dance_state_t *state, void *user_data) {
  uint16_t keycode;
  keyrecord_t record;
  if (is_macro1_recording) {
    keycode = DYN_REC_STOP;
    is_macro1_recording = false;
    layer_state_set_user(current_layer_state);
  } else if (state->count == 1) {
    keycode = DYN_MACRO_PLAY1;
  } else {
    keycode = DYN_REC_START1;
    is_macro1_recording = true;
    layer_state_set_user(current_layer_state);
  }

  record.event.pressed = true;
  process_record_dynamic_macro(keycode, &record);
  record.event.pressed = false;
  process_record_dynamic_macro(keycode, &record);
}

// dynamic macro 2
static bool is_macro2_recording = false;

void macro2_tapdance_fn(qk_tap_dance_state_t *state, void *user_data) {
  uint16_t keycode;
  keyrecord_t record;
  if (is_macro2_recording) {
    keycode = DYN_REC_STOP;
    is_macro2_recording = false;
    layer_state_set_user(current_layer_state);
  } else if (state->count == 1) {
    keycode = DYN_MACRO_PLAY2;
  } else {
    keycode = DYN_REC_START2;
    is_macro2_recording = true;
    layer_state_set_user(current_layer_state);
  }

  record.event.pressed = true;
  process_record_dynamic_macro(keycode, &record);
  record.event.pressed = false;
  process_record_dynamic_macro(keycode, &record);
}

// all tap macros
qk_tap_dance_action_t tap_dance_actions[] = {
  // This Tap dance plays the macro 1 on TAP and records it on double tap.
  [TAP_MACRO1] = ACTION_TAP_DANCE_FN(macro1_tapdance_fn),
  [TAP_MACRO2] = ACTION_TAP_DANCE_FN(macro2_tapdance_fn)
};

// NON-TAP MACROS
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
    bool is_pressed = record->event.pressed;
        switch(id) {

           case CLOSE_APP: {
                if (is_pressed) {
                   SEND_STRING(SS_UP(X_LSHIFT) SS_UP(X_LALT) SS_UP(X_LCTRL)); // remove meh

                   if (isMac) {
                     SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_Q) SS_UP(X_LGUI));
                   } else if (isWin) {
                     SEND_STRING(SS_DOWN(X_LALT)); _delay_ms(100); SEND_STRING(SS_TAP(X_F4) SS_UP(X_LALT));
                   }

                   SEND_STRING(SS_DOWN(X_LSHIFT) SS_DOWN(X_LALT) SS_DOWN(X_LCTRL)); // add meh
                   return false;
                }
           }

           case MAIL: {
                if (is_pressed) {
                    SEND_STRING("oleksii.danilov@gmail.com");
                    return false;
                }
            }

           case SLEEP: {
                if (is_pressed) {
                   if (isMac) {
                     SEND_STRING(SS_DOWN(X_LCTRL) SS_DOWN(X_LSHIFT) SS_DOWN(X_POWER) SS_UP(X_POWER) SS_UP(X_LSHIFT) SS_UP(X_LCTRL));
                   } else if (isWin) {
                     SEND_STRING(SS_LGUI("x")); _delay_ms(300); SEND_STRING("u"); _delay_ms(300); SEND_STRING("s");
                   }
                    return false;
                }
           }

           case SHUTDOWN_WIN: {
               if (is_pressed) {
                   SEND_STRING(SS_LGUI("x")); _delay_ms(300); SEND_STRING("u"); _delay_ms(300); SEND_STRING("u");
                   return false;
               }
           }

           case DEL_WORD_WIN: {
               if (is_pressed) {
                   SEND_STRING(SS_DOWN(X_LSHIFT) SS_TAP(X_LEFT) SS_UP(X_LSHIFT) SS_TAP(X_DELETE));
                   return false;
               }
           }

           case DIR_LIST: {
               if (is_pressed) {
                   SEND_STRING(SS_UP(X_LSHIFT) SS_UP(X_LALT) SS_UP(X_LCTRL)); // remove meh
                   SEND_STRING("ls -lah"); SEND_STRING(SS_TAP(X_ENTER));
                   return false;
               }
           }

           case DELETE_FORCE: {
               if (is_pressed) {
                   SEND_STRING(SS_UP(X_LSHIFT) SS_UP(X_LALT) SS_UP(X_LCTRL)); // remove meh
                   SEND_STRING("rm -rf ");
                   return false;
               }
           }

           case DIR_UP: {
               if (is_pressed) {
                   SEND_STRING(SS_UP(X_LSHIFT) SS_UP(X_LALT) SS_UP(X_LCTRL)); // remove meh
                   SEND_STRING("cd .."); SEND_STRING(SS_TAP(X_ENTER));
                   return false;
               }
           }

           case TERMINAL_CLEAR: {
               if (is_pressed) {
                   SEND_STRING(SS_UP(X_LSHIFT) SS_UP(X_LALT) SS_UP(X_LCTRL)); // remove meh
                   if (isMac) {
                     SEND_STRING("printf \'\\e]50;ClearScrollback\\a\'"); // works in iterm2
                   } else if (isWin) {
                     SEND_STRING("clear"); // works in conemu/msys64
                   }
                   SEND_STRING(SS_TAP(X_ENTER));
                   return false;
               }
           }

           case CHANGE_LANG: {
               if (is_pressed) {
                   if (isMac) {
                     SEND_STRING(SS_UP(X_LGUI) SS_DOWN(X_LALT) SS_TAP(X_SPACE) SS_UP(X_LALT));
                   } else if (isWin) {
                     SEND_STRING(SS_UP(X_LCTRL) SS_DOWN(X_LALT)); _delay_ms(100); SEND_STRING(SS_TAP(X_LSHIFT) SS_UP(X_LALT));
                   }
                   return false;
               }
           }

           case DOCKER_LOGS: {
               if (is_pressed) {
                   SEND_STRING(SS_UP(X_LSHIFT) SS_UP(X_LALT) SS_UP(X_LCTRL)); // remove meh
                   SEND_STRING("docker logs -f --tail=1000 $(docker ps -a -q | head -1)"); SEND_STRING(SS_TAP(X_ENTER));
                   return false;
               }
           }

           case DOCKER_LIST: {
               if (is_pressed) {
                   SEND_STRING(SS_UP(X_LSHIFT) SS_UP(X_LALT) SS_UP(X_LCTRL)); // remove meh
                   SEND_STRING("docker ps -a"); SEND_STRING(SS_TAP(X_ENTER));
                   return false;
               }
           }

           case VIM_SAVE_QUIT: {
               if (is_pressed) {
                   SEND_STRING(SS_UP(X_LSHIFT) SS_UP(X_LALT) SS_UP(X_LCTRL));  // remove meh
                   SEND_STRING(SS_TAP(X_ESCAPE)); _delay_ms(100);
                   SEND_STRING(":wq!"); SEND_STRING(SS_TAP(X_ENTER));
                   return false;
               }
           }

           case VIM_QUIT: {
               if (is_pressed) {
                   SEND_STRING(SS_UP(X_LSHIFT) SS_UP(X_LALT) SS_UP(X_LCTRL)); // remove meh
                   SEND_STRING(SS_TAP(X_ESCAPE)); _delay_ms(100);
                   SEND_STRING(":q!"); SEND_STRING(SS_TAP(X_ENTER));
                   return false;
               }
           }
         }
    return MACRO_NONE;
};

/****************************************************************************************************
*
* Keymap: Default Mac Layer in Qwerty
*
* ,-------------------------------------------------------------------------------------------------------------------.
* | SLEEP  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | PSCR | SLCK | PAUS |Program| Power |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* | `~     |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  |        |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | Mail   |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |        |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |        |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  |        |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |  Ins |  [{  |  ,<  |  ]}  |                                         | Left | Down | Right|  F15 |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,---------------------------.
*                            | LShift+RAlt/BkSp |Macro1|         |Macro2| Lang/Caps/Close TD |
*                            `-----------|------|------|         |------+------+-------------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LCMD/|Shift/|------|         |------|Shift/|RCMD/ |
*                                 | ESC  |Enter |Ctrl/ |         |      |Tab   |SPACE |
*                                 |      |      |CMD_BkSp|     |Ctrl/F16|      |      |
*                                 `--------------------'         `--------------------'
*                             PALM/F13                                            PALM/F14
*/

// base mac layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_MAC] = LAYOUT(
           // left side
           M(SLEEP), KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8,
           KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5,
           M(MAIL), KC_Q, KC_W, KC_E, KC_R, KC_T,
           __________,KC_A, KC_S, KC_D, KC_F, KC_G,
           __________, KC_Z, KC_X, KC_C, KC_V, KC_B,
                 KC_INS, KC_LBRC, KC_COMM, KC_RBRC,
                                           // left thumb keys
			                                    ALT_SHIFT_BS,TD(TAP_MACRO1),
                                                   ALT_SLASH,
                           CMD_ESC, KC_SFTENT, CTRL_CMD_BS,
                                     // left palm key
			                         MEH_F13,
    // right side
    KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,KC_PSCR ,KC_SLCK  ,KC_PAUS, RESET, KC_POWER,
	KC_6, KC_7, KC_8, KC_9, KC_0, __________,
	KC_Y, KC_U, KC_I, KC_O, KC_P, __________,
	KC_H, KC_J, KC_K, KC_L, KC_SCLN, __________,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, __________,
	KC_LEFT, KC_DOWN, KC_RGHT, KC_F15,
           // right thumb keys
           TD(TAP_MACRO2), LANG_CAPS,
           ALT_BSLASH,
           CTRL_F16, SFT_T(KC_TAB), CMD_SPACE,
                                    // right palm key
                                    MEH_LAST_APP
    ),

// mac-specific layers
[_COMMAND_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,   __________, MOD_G,
         __________,  __________,  __________,  __________,  __________,  MOD_B,
                   __________,  __________,  __________,  MOD_RBRC,
                             __________,  __________,
                                       __________,
                    CMD_ESC, __________,  __________,
                                     KC_F13,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         MOD_Y,  MOD_U,  MOD_I,  MOD_O,  __________,  __________,
         CTRL_H,  MOD_J,  MOD_K,  MOD_L,  __________,  __________,
         MOD_N,  CTRL_M,  MOD_UP,  CTRL_DOT ,  __________,  __________,
                   MOD_LEFT,  MOD_DOWN,  MOD_RIGHT, __________,
         KC_F2,  M(CHANGE_LANG),
         SHIFT_BSLS,
         KC_F16,  SHIFT_TAB,  MOD_SPACE,
                             KC_F14
    ),

[_COMMAND_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  MOD_W,  MOD_E,  MOD_R,  MOD_T,
         __________,  __________,  MOD_S,  MOD_D,  MOD_F,  MOD_G,
         __________,  __________,  MOD_X,  MOD_C,  MOD_V,  MOD_B,
                   __________,  MOD_LBRC,  CTRL_COMMA,  MOD_RBRC,
                             ALT_BSPC,  KC_F1,
                                        KC_Z,
                    MOD_ESC, MOD_ENTER, LSFT(KC_Z),
                                     KC_F13,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________,  CMD_SPACE,
                                     KC_F14
    ),

[_ALT_SHIFT_BS] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             ALT_SHIFT_BS,  __________,
                                       __________,
                    __________, __________,  __________,
                                     KC_F13,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                             KC_F14
    ),

[_ALT] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       ALT_SLASH,
                    __________, __________,  __________,
                                     KC_F13,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  MOD_UP,  __________ ,  __________,  __________,
                   MOD_LEFT,  MOD_DOWN,  MOD_RIGHT, __________,
         __________,  __________,
         __________,
         __________,  KC_F7,  KC_F8,
                             KC_F14
    ),

[_CTRL] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________,  CTRL_CMD_BS,
                                     KC_F13,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  CMD_M, __________, __________,
         __________,  __________,
         __________,
         CTRL_F16,  __________,  __________,
                             KC_F14
    ),

// base win layer
[_WIN] = LAYOUT(
           // left side
           M(SLEEP), KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8,
           KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5,
           M(MAIL), KC_Q, KC_W, KC_E, KC_R, KC_T,
           __________,KC_A, KC_S, KC_D, KC_F, KC_G,
           __________, KC_Z, KC_X, KC_C, KC_V, KC_B,
                 KC_INS, KC_LBRC, KC_COMM, KC_RBRC,
                                           // left thumb keys
			                                    CTRL_SHIFT_BS,TD(TAP_MACRO1),
                                                   ALT_SLASH,
                           CTRL_ESC, KC_SFTENT, LGUI_DEL,
                                     // left palm key
			                         MEH_F13,
    // right side
    KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,KC_PSCR ,KC_SLCK  ,KC_PAUS, RESET, M(SHUTDOWN_WIN),
	KC_6, KC_7, KC_8, KC_9, KC_0, __________,
	KC_Y, KC_U, KC_I, KC_O, KC_P, __________,
	KC_H, KC_J, KC_K, KC_L, KC_SCLN, __________,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, __________,
	KC_LEFT, KC_DOWN, KC_RGHT, KC_APP,
           // right thumb keys
           TD(TAP_MACRO2), LANG_CAPS,
           ALT_BSLASH,
           KC_RGUI, SFT_T(KC_TAB), CTRL_SPACE,
                                    // right palm key
                                    MEH_LAST_APP
    ),

// win-specific layers
[_CONTROL_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,   __________, MOD_G,
         __________,  __________,  __________,  __________,  __________,  MOD_B,
                   __________,  __________,  HYPR(KC_COMM),  MOD_RBRC,
                             __________,  __________,
                                       __________,
                    CTRL_ESC, __________,  __________,
                                     KC_F13,

         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
           MOD_Y,  MOD_U,  MOD_I,  MOD_O,  __________,  __________,
           MOD_H,  MOD_J,  MOD_K,  MOD_L,  __________,  __________,
           MOD_N,  MOD_M,  MOD_UP,  MOD_DOT ,  __________,  __________,
                     MOD_LEFT,  MOD_DOWN,  MOD_RIGHT, __________,
         KC_F2,  M(CHANGE_LANG),
         SHIFT_BSLS,
         __________,  SHIFT_TAB,  MOD_SPACE,
                             KC_F14
    ),

[_CONTROL_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  MOD_W,  MOD_E,  MOD_R,  MOD_T,
         __________,  __________,  MOD_S,  MOD_D,  MOD_F,  MOD_G,
         __________,  __________,  MOD_X,  MOD_C,  MOD_V,  MOD_B,
                      __________,  MOD_LBRC, MOD_COMMA, MOD_RBRC,
                             M(DEL_WORD_WIN),  KC_F1,
                                        KC_Z,
                    MOD_ESC, MOD_ENTER, LSFT(KC_Z),
                                     KC_F13,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________,  CTRL_SPACE,
                                     KC_F14
    ),

[_CTRL_SHIFT_BS] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             CTRL_SHIFT_BS,  __________,
                                       __________,
                    __________, __________,  __________,
                                     KC_F13,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                             KC_F14
    ),

// common layers
[_PALM_L] = LAYOUT(
__________,  __________,  __________,  __________,  __________,  SET_LAYER_MAC, __________, SET_LAYER_WIN, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________, __________, __________, __________,
                             __________,  __________,
                                     __________,
                      __________, __________,  __________,
                                     MEH_F13,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  M(VIM_SAVE_QUIT),  M(DOCKER_LIST),  M(VIM_QUIT),  __________,  __________,
         __________,  M(DIR_LIST),  M(DELETE_FORCE),  M(DOCKER_LOGS),  __________,  __________,
         __________,  M(TERMINAL_CLEAR),  KC_PGUP,  M(DIR_UP) ,  __________,  __________,
                   KC_HOME,  KC_PGDN, KC_END, __________,
         __________,  __________,
         __________,
         MUTE,  VOL_DOWN,  VOL_UP,
                             KC_F14
    ),

[_PALM_R] = LAYOUT(
__________,  __________,  __________,  __________,  __________,  SET_LAYER_MAC, __________, SET_LAYER_WIN, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________, __________, __________, __________,
                             __________,  __________,
                                     __________,
                      CLOSE_APP, __________,  __________,
                                     KC_F13,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                             MEH_LAST_APP
    ),
};

// support for repeat keycodes (now only for palm layer keys)
void matrix_scan_user(void) {
   if (palm_repeat_code) {
      if (timer_elapsed(palm_repeat_timer) > (10 + first_repeat_delay)) {
         without_meh(palm_repeat_code);
         palm_repeat_timer = timer_read();
         first_repeat_delay = 0;
      }
   }
}


// support for "mo layer tap" functionality: activate mod as soon as layer is activated
uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    // mac-specific
    case _COMMAND_ESCAPE: down(KC_LGUI); break;
    case _COMMAND_SPACE: down(KC_LGUI); break;
    case _ALT_SHIFT_BS: down(KC_LALT); down(KC_LSFT); break;
    case _ALT: down(KC_LALT); break;
    case _CTRL: down(KC_LCTL); break;

    // win-specific
    case _CONTROL_ESCAPE: down(KC_LCTL); break;
    case _CONTROL_SPACE: down(KC_LCTL); break;
    case _CTRL_SHIFT_BS: down(KC_LCTL); down(KC_LSFT); break;

    // common
    case _PALM_L: down(KC_LSFT); down(KC_LALT); down(KC_LCTL); break;
    case _PALM_R: down(KC_LSFT); down(KC_LALT); down(KC_LCTL); break;

    // unregister everything (even if it was not pressed - no big deal; this works faster than getting pressed mods)
    default: up(KC_LGUI); up(KC_LCTL); up(KC_LALT); up(KC_LSFT); break;
    }
return state;
}

// support for "mo layer tap" functionality
bool cmd_esc_interrupted = true;
bool meh_f13_interrupted = true;
bool meh_last_app_interrupted = true;
bool alt_interrupted = true;
bool ctrl_interrupted = true;
bool alt_shift_interrupted = true;
bool ctrl_shift_interrupted = true;
bool ctrl_esc_interrupted = true;
uint16_t esc_timer; // timer for leader key: esc

// adding logic to custom keycodes and overriding existing ones (taking hold duration into account);
// "mo layer tap" and "esc leader key" functionality
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if ((keycode != TD(TAP_MACRO1)) && (keycode != TD(TAP_MACRO2))) {
      if (!process_record_dynamic_macro(keycode, record)) {
        return false;
      }
    }

    bool is_pressed = record->event.pressed;

    // support for "mo layer tap" functionality
    if (keycode != CMD_ESC) { cmd_esc_interrupted = true; }
    if (keycode != MEH_F13) { meh_f13_interrupted = true; }
    if (keycode != MEH_LAST_APP) { meh_last_app_interrupted = true; }
    if (keycode != ALT_SLASH) { alt_interrupted = true; }
    if (keycode != CTRL_CMD_BS) { ctrl_interrupted = true; }
    if (keycode != ALT_SHIFT_BS) { alt_shift_interrupted = true; }
    if (keycode != CTRL_ESC) { ctrl_esc_interrupted = true; }
    if (keycode != CTRL_SHIFT_BS) { ctrl_shift_interrupted = true; }

    // MO LAYER TAP FUNCTIONALITY
    switch (keycode) {
        case SET_LAYER_MAC: {
          eeconfig_update_default_layer(1UL << _MAC);
          default_layer_set(1UL << _MAC);
          isMac = true; isWin = false;
          return false;
        }

        case SET_LAYER_WIN: {
          eeconfig_update_default_layer(1UL << _WIN);
          default_layer_set(1UL << _WIN);
          isWin = true; isMac = false;
          return false;
        }

        // mac-specific layers
        case CMD_ESC: {
         static uint16_t cmd_esc_layer_timer;
         if (mo_layer_tap(KC_ESC, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, &cmd_esc_layer_timer, &cmd_esc_interrupted, is_pressed, 180)) {
           esc_timer = timer_read();
         }
         return true;
        }

        case ALT_SHIFT_BS: {
          static uint16_t alt_shift_layer_timer;
          mo_layer_tap(KC_BSPC, KC_NO, KC_LSFT, KC_LALT, KC_NO, KC_NO, &alt_shift_layer_timer, &alt_shift_interrupted, is_pressed, 180);
          return true;
        }

        case CTRL_CMD_BS: {
          static uint16_t ctrl_layer_timer;
          mo_layer_tap(KC_BSPC, KC_LGUI, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_layer_timer, &ctrl_interrupted, is_pressed, 180);
          return true;
        }

        case ALT_SLASH: {
          static uint16_t alt_layer_timer;
          mo_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_layer_timer, &alt_interrupted, is_pressed, 180);
          return true;
        }

        // win-specific layers
        case CTRL_ESC: {
         static uint16_t ctrl_esc_layer_timer;
         if (mo_layer_tap(KC_ESC, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_esc_layer_timer, &ctrl_esc_interrupted, is_pressed, 180)) {
           esc_timer = timer_read();
         }
         return true;
        }

        case CTRL_SHIFT_BS: {
          static uint16_t ctrl_shift_layer_timer;
          mo_layer_tap(KC_BSPC, KC_NO, KC_LSFT, KC_LCTL, KC_NO, KC_NO, &ctrl_shift_layer_timer, &ctrl_shift_interrupted, is_pressed, 180);
          return true;
        }

        // common layers
        case MEH_F13: {
          static uint16_t meh_f13_layer_timer;
          mo_layer_tap(KC_F13, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_NO, &meh_f13_layer_timer, &meh_f13_interrupted, is_pressed, 300);
          return true;
        }

        case MEH_LAST_APP: {
          static uint16_t meh_last_app_layer_timer;
          mo_layer_tap(KC_TAB, os_specific_key(KC_LGUI, KC_LALT), KC_LCTL, KC_LALT, KC_LSFT, KC_NO, &meh_last_app_layer_timer, &meh_last_app_interrupted, is_pressed, 300);
          return true;
        }

        // ESCAPE AS LEADER KEY
        case KC_RGHT: { return after_leader(KC_RGHT, KC_LALT, KC_LCTL, KC_LSFT, &esc_timer, is_pressed, 180); }
        case KC_LEFT: { return after_leader(KC_LEFT, KC_LALT, KC_LCTL, KC_LSFT, &esc_timer, is_pressed, 180); }

        // CUSTOM KEYCODES
        case KC_PGUP: { return without_meh_repeat(KC_PGUP, is_pressed); }
        case KC_PGDN: { return without_meh_repeat(KC_PGDN, is_pressed); }
        case KC_HOME: { return without_meh_repeat(KC_HOME, is_pressed); }
        case KC_END: { return without_meh_repeat(KC_END, is_pressed); }

        case LANG_CAPS: { return hold_replace_add_mod(os_specific_key(KC_SPC, KC_LSFT), KC_RALT, KC_LOCKING_CAPS, KC_NO, KC_NO, is_pressed, 140); }

        case SHIFT_TAB: { return replace_mods_hold_mods(KC_TAB, os_specific_key(KC_LGUI, KC_LCTL), KC_LSFT, KC_LCTL, KC_LSFT, is_pressed, 180); }
        case SHIFT_BSLS: { return replace_mods_hold_mods(KC_BSLS, os_specific_key(KC_LGUI, KC_LCTL), KC_LSFT, os_specific_key(KC_LGUI, KC_LCTL), KC_LSFT, is_pressed, 180); }

        case MUTE: { return without_mods_for_single_press(os_specific_key(KC__MUTE, KC_F22), KC_LCTL, KC_LALT, KC_LSFT, KC_NO, is_pressed); }

        case VOL_UP: { return without_meh_repeat(os_specific_key(KC__VOLUP, KC_F20), is_pressed); }
        case VOL_DOWN: { return without_meh_repeat(os_specific_key(KC__VOLDOWN, KC_F21), is_pressed); }

        // mac-only overrides
        case ALT_BSPC: { return replace_cmd_hold_cmd_shift(KC_BSPC, KC_LALT, is_pressed, 180); }
        case CTRL_COMMA: { return replace_cmd_hold_cmd_shift(KC_COMM, KC_LCTL, is_pressed, 180); }
        case CTRL_DOT: { return replace_cmd_hold_cmd_shift(KC_DOT, KC_LCTL, is_pressed, 180); }
        case CTRL_H: { return replace_cmd_hold_cmd_shift(KC_H, KC_LCTL, is_pressed, 180); }
        case CTRL_M: { return replace_cmd_hold_cmd_shift(KC_M, KC_LCTL, is_pressed, 180); }
        case CMD_M: { return replace_mods_hold_mods(KC_M, KC_LCTL, KC_LGUI, KC_LGUI, KC_LSFT, is_pressed, 180); }

        // MODIFYING KEYCODES BASED ON HOLD DURATION
        // 140 ms
        case MOD_SPACE: { return hold_140_add_shift(KC_SPC, is_pressed); }
        case MOD_ESC: { return hold_140_add_shift(os_specific_key(KC_ESC, KC_BSPC), is_pressed); }
        case MOD_ENTER: { return hold_140_add_shift(KC_ENTER, is_pressed); }

        // 180 ms
        case MOD_W: { return hold_180_add_shift(KC_W, is_pressed); }
        case MOD_E: { return hold_180_add_shift(KC_E, is_pressed); }
        case MOD_R: { return hold_180_add_shift(KC_R, is_pressed); }
        case MOD_T: { return hold_180_add_shift(KC_T, is_pressed); }
        case MOD_S: { return hold_180_add_shift(KC_S, is_pressed); }
        case MOD_D: { return hold_180_add_shift(KC_D, is_pressed); }
        case MOD_F: { return hold_180_add_shift(KC_F, is_pressed); }
        case MOD_G: { return hold_180_add_shift(KC_G, is_pressed); }
        case MOD_X: { return hold_180_add_shift(KC_X, is_pressed); }
        case MOD_C: { return hold_180_add_shift(KC_C, is_pressed); }
        case MOD_V: { return hold_180_add_shift(KC_V, is_pressed); }
        case MOD_B: { return hold_180_add_shift(KC_B, is_pressed); }
        case MOD_COMMA: { return hold_180_add_shift(KC_COMM, is_pressed); }
        case MOD_LBRC: { return hold_180_add_shift(KC_LBRC, is_pressed); }
        case MOD_RBRC: { return hold_180_add_shift(KC_RBRC, is_pressed); }

        case MOD_Y: { return hold_180_add_shift(KC_Y, is_pressed); }
        case MOD_U: { return hold_180_add_shift(KC_U, is_pressed); }
        case MOD_I: { return hold_180_add_shift(KC_I, is_pressed); }
        case MOD_O: { return hold_180_add_shift(KC_O, is_pressed); }
        case MOD_H: { return hold_180_add_shift(KC_H, is_pressed); }
        case MOD_J: { return hold_180_add_shift(KC_J, is_pressed); }
        case MOD_K: { return hold_180_add_shift(KC_K, is_pressed); }
        case MOD_L: { return hold_180_add_shift(KC_L, is_pressed); }
        case MOD_N: { return hold_180_add_shift(KC_N, is_pressed); }
        case MOD_M: { return hold_180_add_shift(KC_M, is_pressed); }
        case MOD_DOT: { return hold_180_add_shift(KC_DOT, is_pressed); }

        case MOD_LEFT: { return hold_add_mods(KC_LEFT, KC_LALT, KC_LCTL, is_pressed, 180); }
        case MOD_RIGHT: { return hold_add_mods(KC_RGHT, KC_LALT, KC_LCTL, is_pressed, 180); }
        case MOD_UP: { return hold_add_mods(KC_UP, KC_LALT, KC_LCTL, is_pressed, 180); }
        case MOD_DOWN: { return hold_add_mods(KC_DOWN, KC_LALT, KC_LCTL, is_pressed, 180); }

        case KC_1: { return hold_180_replace(KC_1, KC_5, KC_LSFT, is_pressed); }
        case KC_2: { return hold_180_replace(KC_2, KC_9, KC_LSFT, is_pressed); }
        case KC_3: { return hold_180_replace(KC_3, KC_MINS, KC_LSFT, is_pressed); }
        case KC_4: { return hold_180_replace(KC_4, KC_0, KC_LSFT, is_pressed); }
        case KC_5: { return hold_180_replace(KC_5, KC_EQL, KC_NO, is_pressed); }
        case KC_6: { return hold_180_replace(KC_6, KC_EQL, KC_LSFT, is_pressed); }
        case KC_7: { return hold_180_replace(KC_7, KC_1, KC_LSFT, is_pressed); }
        case KC_8: { return hold_180_replace(KC_8, KC_MINS, KC_NO, is_pressed); }
        case KC_9: { return hold_180_replace(KC_9, KC_SLSH, KC_LSFT, is_pressed); }
        case KC_0: { return hold_180_replace(KC_0, KC_6, KC_LSFT, is_pressed); }

        case KC_F1: { return hold_180_add_shift(KC_F1, is_pressed); }
        case KC_F2: { return hold_180_add_shift(KC_F2, is_pressed); }
        case KC_F3: { return hold_180_add_shift(KC_F3, is_pressed); }
        case KC_F4: { return hold_180_add_shift(KC_F4, is_pressed); }
        case KC_F5: { return hold_180_add_shift(KC_F5, is_pressed); }
        case KC_F6: { return hold_180_add_shift(KC_F6, is_pressed); }
        case KC_F7: { return hold_180_add_shift(KC_F7, is_pressed); }
        case KC_F8: { return hold_180_add_shift(KC_F8, is_pressed); }
        case KC_F9: { return hold_180_add_shift(KC_F9, is_pressed); }
        case KC_F10: { return hold_180_add_shift(KC_F10, is_pressed); }
        case KC_F11: { return hold_180_add_shift(KC_F11, is_pressed); }
        case KC_F12: { return hold_180_add_shift(KC_F12, is_pressed); }
        case KC_F13: { return hold_180_add_shift(KC_F13, is_pressed); }
        case KC_F14: { return hold_180_add_shift(KC_F14, is_pressed); }
        case KC_F15: { return hold_180_add_shift(KC_F15, is_pressed); }
        case KC_F16: { return hold_180_add_shift(KC_F16, is_pressed); }

        default: {
          return true;
        }
    }
}