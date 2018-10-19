#include "kinesis.h"

#define __________ KC_TRNS
#define XXXXXXX KC_NO

// mac layers
#define _MAC 0
#define _COMMAND_ESCAPE 1
#define _COMMAND_SPACE 2
#define _ALT_SHIFT_BS 3
#define _CTRL_CMD_BS 4
#define _ALT_SLASH_MAC 5
#define _ALT_BSLASH_MAC 6
// win layers
#define _WIN 7
#define _CONTROL_ESCAPE 8
#define _CONTROL_SPACE 9
#define _CTRL_SHIFT_BS 10
#define _CTRL_ALT_DEL 11
#define _ALT_SLASH_WIN 12
#define _ALT_BSLASH_WIN 13

// common layers
#define _SHIFT_ENTER 14
#define _SHIFT_TAB 15
#define _PALM_L 16
#define _PALM_R 17

enum kinesis_keycodes {
  // mac
  SET_LAYER_MAC,
  CMD_ESC = MO(_COMMAND_ESCAPE),
  CMD_SPACE = LT(_COMMAND_SPACE, KC_SPC),
  ALT_SHIFT_BS = MO(_ALT_SHIFT_BS),
  ALT_SLASH_MAC = MO(_ALT_SLASH_MAC),
  ALT_BSLASH_MAC = MO(_ALT_BSLASH_MAC),
  CTRL_CMD_BS = MO(_CTRL_CMD_BS),
  CTRL_F16 = LT(_CTRL_CMD_BS, KC_F16),

  // win
  SET_LAYER_WIN,
  CTRL_ESC = MO(_CONTROL_ESCAPE),
  CTRL_SPACE = LT(_CONTROL_SPACE, KC_SPC),
  CTRL_SHIFT_BS = MO(_CTRL_SHIFT_BS),
  ALT_SLASH_WIN = MO(_ALT_SLASH_WIN),
  ALT_BSLASH_WIN = MO(_ALT_BSLASH_WIN),
  CTRL_ALT_DEL = MO(_CTRL_ALT_DEL),

  // common
  SHIFT_ENTER = MO(_SHIFT_ENTER),
  SHIFT_TAB = MO(_SHIFT_TAB),
  HYPR_F14 = MO(_PALM_L),
  HYPR_F15 = MO(_PALM_R),

  VOL_UP,
  VOL_DOWN,
};

enum holding_keycodes {
  MOD_W = SAFE_RANGE, MOD_E, MOD_R, MOD_T,
  MOD_S, MOD_D, MOD_F, MOD_G,
  MOD_X, MOD_C, MOD_V, MOD_B,
  MOD_LBRC, MOD_COMMA, MOD_RBRC,
  MOD_ESC,
  MOD_ENTER,
  MOD_Y, MOD_U, MOD_I, MOD_O, MOD_H,
  MOD_J, MOD_K, MOD_L,
  MOD_N, MOD_M, MOD_UP, MOD_DOT,
  MOD_LEFT, MOD_DOWN, MOD_RIGHT,
  MOD_DELETE,
  PIPE_MOD,
  SHIFT_TAB_CTRL,
  MOD_BSLS,
  MOD_TAB,
  MOD_SPACE,
  // mac-specific overrides
  CTRL_COMMA,

  CTRL_H, CTRL_M, CTRL_DOT,
  CTRL_TAB,
  // LWin + key overrides
  W_F9, W_F10, W_F11, W_F12,
  W_6, W_7, W_8, W_9, W_0,
  W_Y, W_U, W_I, W_O, W_P,
  W_H, W_J, W_K, W_L, W_SCLN,
  W_N, W_M, W_DOT, W_QUOT,

  // required for dynamic macros
  DYNAMIC_MACRO_RANGE
};
#include "dynamic_macro.h"

enum macros {
  MAIL = 0,
  CLOSE_APP,
  NEXT_APP,
  PREV_APP,
  POS_LEFT,
  POS_RIGHT,
  POS_MINIMIZE,
  POS_FULL,
  SLEEP,
  SHUTDOWN,
  DEL_WORD,
  DIR_UP,
  TERMINAL_CLEAR,
  DOCKER_LIST,
  DOCKER_LOGS,
  VIM_SAVE_QUIT,
  VIM_QUIT,
  MUTE,
  PLAY_UP,
  PLAY_DOWN,
  PLAY_NEXT,
  PLAY_PREV,
  PLAY_PAUSE
};

// HELPER FUNCTIONS
// switch mac <-> win
static bool isMac = false;
static bool isWin = false;
void matrix_init_user(void) {
    switch (biton32(eeconfig_read_default_layer())) {
      case _MAC: isMac = true; isWin = false; break;
      case _WIN: isWin = true; isMac = false; break;
      default: break;
  }
}

uint16_t os_specific_key(uint16_t mac_key, uint16_t win_key) {
  if (isMac) { return mac_key; };
  if (isWin) { return win_key; };
  return KC_NO;
}

void down(uint16_t key) { register_code(key); }
void up(uint16_t key) { unregister_code(key); }
void key_code(uint16_t key) { down(key); up(key); }

void with_1_mod(uint16_t key, uint16_t mod1) {
  uint8_t old_mods = get_mods();
  down(mod1); key_code(key); up(mod1);
  if (old_mods) { register_mods(old_mods); }
}
void with_2_mods(uint16_t key, uint16_t mod1, uint16_t mod2) {
  uint8_t old_mods = get_mods();
  down(mod2); down(mod1);
  key_code(key);
  up(mod1); up(mod2);
  if (old_mods) { register_mods(old_mods); }
}
void with_3_mods(uint16_t key, uint16_t mod1, uint16_t mod2, uint16_t mod3) {
  uint8_t old_mods = get_mods();
  down(mod3); down(mod2); down(mod1);
  key_code(key);
  up(mod1); up(mod2); up(mod3);
  if (old_mods) { register_mods(old_mods); }
}

void remove_mods(uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4) { up(mod1); up(mod2); up(mod3); up(mod4); }
void insert_mods(uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4) { down(mod1); down(mod2); down(mod3); down(mod4); }
void remove_meh(void) { return remove_mods(KC_LCTL, KC_LALT, KC_LSFT, KC_NO); }
void add_meh(void) { return insert_mods(KC_LCTL, KC_LALT, KC_LSFT, KC_NO); }
void remove_hypr(void) { up(KC_LGUI); remove_meh(); }
void add_hypr(void) { down(KC_LGUI); add_meh(); }

bool is_not_held(uint16_t hold_timer, uint16_t hold_duration) {
 return timer_elapsed(hold_timer) < hold_duration;
}

// replaces mods of keycode, adds additional mods if it was held for at least provided duration
bool replace_key_and_mods_if_held_replace_key_and_mods(uint16_t code, uint16_t mod1_to_be_replaced, uint16_t mod2_to_be_replaced, uint16_t replacement_mod1, uint16_t replacement_mod2, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      up(mod1_to_be_replaced);
      up(mod2_to_be_replaced);

      if (is_not_held(hold_timer, hold_duration)){
          with_2_mods(code, replacement_mod1, replacement_mod2);
      } else {
          with_2_mods(held_code, held_mod1, held_mod2);
      }

      down(mod1_to_be_replaced);
      down(mod2_to_be_replaced);
  }
  return false;
}

// replaces command, if held - adds shift to keycode
bool replace_cmd_if_held_add_cmd_shift(uint16_t code, uint16_t replacement_mod, bool pressed, uint8_t hold_duration) {
  return replace_key_and_mods_if_held_replace_key_and_mods(code, KC_LGUI, KC_NO, replacement_mod, KC_NO, code, KC_LGUI, KC_LSFT, pressed, hold_duration);
}

bool replace_ctrl_alt_with_lgui(uint16_t code, bool pressed) {
  return replace_key_and_mods_if_held_replace_key_and_mods(code, KC_LCTL, KC_LALT, KC_LGUI, KC_NO, code, KC_LGUI, KC_NO, pressed, 180);
}

// replaces keycode if it was held for at least provided duration
bool if_held_replace(uint16_t code, uint16_t held_code, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (is_not_held(hold_timer, hold_duration)){
          key_code(code);
      } else {
          key_code(held_code);
      }
  }
  return false;
}

// add mod to keycode if it was held for at least provided duration
bool if_held_add_mods(uint16_t code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (is_not_held(hold_timer, hold_duration)){
          key_code(code);
      } else {
          with_2_mods(code, held_mod1, held_mod2);
      }
  }
  return false;
}

// adds shift to keycode if it was held for at 140 ms
bool if_held_140_add_shift(uint16_t code, bool pressed) {
  return if_held_add_mods(code, KC_LSFT, KC_NO, pressed, 140);
}

// adds shift to keycode if it was held for at 180 ms
bool if_held_180_add_shift(uint16_t code, bool pressed) {
  return if_held_add_mods(code, KC_LSFT, KC_NO, pressed, 180);
}

// replaces keycode and adds mod to it if it was held for at least provided duration
bool replace_if_held_add_mods(uint16_t code, uint16_t mod, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (is_not_held(hold_timer, hold_duration)){
          with_1_mod(code, mod);
      } else {
          with_2_mods(held_code, held_mod1, held_mod2);
      }
  }
  return false;
}

// replaces keycode and adds mod to it if it was held for at least provided duration
bool if_held_180_replace(uint16_t code, uint16_t held_code, uint16_t held_mod, bool pressed) {
  return replace_if_held_add_mods(code, KC_NO, held_code, held_mod, KC_NO, pressed, 180);
}

void without_hypr(uint16_t code) {
  remove_hypr();
  key_code(code);
  isMac ? add_hypr() : add_meh();
}

// handles repeat functionality (only for palm keys)
static uint16_t palm_repeat_code;
static uint16_t palm_repeat_timer;
static uint8_t first_repeat_delay;
bool repeat_without_hypr(uint16_t code, bool pressed) {
   if (pressed) {
     without_hypr(code);
     palm_repeat_code = code;
     palm_repeat_timer = timer_read();
     first_repeat_delay = 250;
   } else {
       palm_repeat_code = 0;
   }

  return false;
}

// provides functionality similar to MT - except that layer with mod is triggered immediately: this is useful when such mod is used with mouse;
// returns true if tap was triggered and false otherwise
bool momentary_layer_tap(uint16_t tap_key, uint16_t tap_mod, uint16_t layer_mod1, uint16_t layer_mod2, uint16_t layer_mod3, uint16_t layer_mod4, uint16_t *layer_timer, bool *interrupted_flag, bool is_pressed, uint16_t hold_duration, bool bring_mods_back) {
  if (is_pressed) {
    *interrupted_flag = false;
    *layer_timer = timer_read();
  } else {
    if (is_not_held(*layer_timer, hold_duration)) {
      if (!*interrupted_flag) {
        up(layer_mod1); up(layer_mod2); up(layer_mod3); up(layer_mod4); // unregister mods associated with the layer, so that they don't intefere with the tap key
        with_1_mod(tap_key, tap_mod); // register tap key and its mod
        if (bring_mods_back) {
          down(layer_mod1); down(layer_mod2); down(layer_mod3); down(layer_mod4); // bring mods back
        }
        return true;
      }
    }
  }
  return false;
}

// provides functionality similar to "leader key", except that it works for non-dedicated key (currently, only escape)
bool following_custom_leader_key(uint16_t key, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t *leader_timer, bool is_pressed, uint16_t leader_last_pressed_timeout) {
  if (*leader_timer && is_not_held(*leader_timer, leader_last_pressed_timeout)) {
    if (is_pressed) {
      *leader_timer = 0;
      with_3_mods(key, mod1, mod2, mod3);
      return false;
    }
  }
  return true;
}

enum {
  TAP_MACRO1 = 0,
  TAP_MACRO2 = 1,
  K_TD = 2,
  COMMA_TD = 3
};

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5
};

typedef struct { bool is_press_action; int state; } tap;

int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) { if (state->interrupted || state->pressed==0) return SINGLE_TAP; else return SINGLE_HOLD; }
  else if (state->count == 2) {
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else return 6;
}

//**************** K TAP *********************//
static tap k_tap_state = { .is_press_action = true, .state = 0 };

void k_finished (qk_tap_dance_state_t *state, void *user_data) {
  k_tap_state.state = cur_dance(state);
  switch (k_tap_state.state) {
    case SINGLE_TAP: down(KC_K); break;
    case SINGLE_HOLD: down(KC_LSFT); key_code(KC_K); break;
    default:
      if (isMac) { down(KC_LCTL); down(KC_LALT); key_code(KC_SPC); up(KC_LCTL); break; }
      else if (isWin) { down(KC_LGUI); key_code(KC_SPC); break; }
    }
  }

void k_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (k_tap_state.state) {
    case SINGLE_TAP: up(KC_K); break;
    case SINGLE_HOLD: up(KC_LSFT); break;
    default:
      if (isMac) { up(KC_LALT); break; }
      else if (isWin) { up(KC_LGUI); break; }
    }
  k_tap_state.state = 0;
}

//**************** COMMA TAP *********************//
static tap comma_tap_state = { .is_press_action = true, .state = 0 };

void comma_finished (qk_tap_dance_state_t *state, void *user_data) {
  comma_tap_state.state = cur_dance(state);
  switch (comma_tap_state.state) {
    case SINGLE_TAP: down(KC_COMM); break;
    case SINGLE_HOLD: down(KC_LSFT); key_code(KC_COMM); break;
    default:
      if (isMac) { down(KC_LOCKING_CAPS); break; }
      else if (isWin) { down(KC_CAPS); break; }
    }
  }

void comma_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (comma_tap_state.state) {
    case SINGLE_TAP: up(KC_COMM); break;
    case SINGLE_HOLD: up(KC_LSFT); break;
    default:
      if (isMac) { up(KC_LOCKING_CAPS); break; }
      else if (isWin) { up(KC_CAPS); break; }
    }
  comma_tap_state.state = 0;
}

// dynamic macro1
static bool is_macro1_recording = false;
static uint32_t current_layer_state = 0;
uint32_t layer_state_set_user(uint32_t state);
void macro1_tapdance_fn(qk_tap_dance_state_t *state, void *user_data) {
  uint16_t keycode; keyrecord_t record;
  if (is_macro1_recording) { keycode = DYN_REC_STOP; is_macro1_recording = false; layer_state_set_user(current_layer_state); }
  else if (state->count == 1) { keycode = DYN_MACRO_PLAY1; }
  else { keycode = DYN_REC_START1; is_macro1_recording = true; layer_state_set_user(current_layer_state); }
  record.event.pressed = true; process_record_dynamic_macro(keycode, &record); record.event.pressed = false; process_record_dynamic_macro(keycode, &record);
}

// dynamic macro 2
static bool is_macro2_recording = false;
void macro2_tapdance_fn(qk_tap_dance_state_t *state, void *user_data) {
  uint16_t keycode; keyrecord_t record;
  if (is_macro2_recording) { keycode = DYN_REC_STOP; is_macro2_recording = false; layer_state_set_user(current_layer_state); }
  else if (state->count == 1) { keycode = DYN_MACRO_PLAY2; }
  else { keycode = DYN_REC_START2; is_macro2_recording = true; layer_state_set_user(current_layer_state); }
  record.event.pressed = true; process_record_dynamic_macro(keycode, &record); record.event.pressed = false; process_record_dynamic_macro(keycode, &record);
}

// all tap macros
qk_tap_dance_action_t tap_dance_actions[] = {
  // This Tap dance plays the macro 1 on TAP and records it on double tap.
  [TAP_MACRO1] = ACTION_TAP_DANCE_FN(macro1_tapdance_fn),
  [TAP_MACRO2] = ACTION_TAP_DANCE_FN(macro2_tapdance_fn),
  [K_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, k_finished, k_reset),
  [COMMA_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, comma_finished, comma_reset)
};

// NON-TAP MACROS
void window_positioning(uint16_t mac_key, uint16_t win_key) {
  if (isMac) { key_code(mac_key); }
  else if (isWin) { remove_meh(); key_code(win_key); add_meh(); }
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
    bool is_pressed = record->event.pressed;
        switch(id) {
           case MUTE: { if (is_pressed) { remove_hypr(); key_code(os_specific_key(KC__MUTE, KC_MUTE)); add_hypr(); return false; }}

           case PLAY_UP: { if (is_pressed) { up(KC_LGUI), key_code(KC_1); return false; } }
           case PLAY_DOWN: { if (is_pressed) { up(KC_LGUI), key_code(KC_2); return false; } }
           case PLAY_PREV: { if (is_pressed) { up(KC_LGUI), key_code(KC_3); return false; } }
           case PLAY_PAUSE: { if (is_pressed) { up(KC_LGUI), key_code(KC_4); return false; } }
           case PLAY_NEXT: { if (is_pressed) { up(KC_LGUI), key_code(KC_5); return false; } }

           case POS_LEFT: { if (is_pressed) { window_positioning(KC_U, KC_LEFT); return false; } }
           case POS_RIGHT: { if (is_pressed) { window_positioning(KC_O, KC_RGHT); return false; } }
           case POS_FULL: { if (is_pressed) { window_positioning(KC_I, KC_UP); return false; } }
           case POS_MINIMIZE: {
             if (is_pressed) {
                remove_meh();
                if (isMac) { key_code(KC_M); }
                else if (isWin) { key_code(KC_DOWN); }
                add_meh();
                return false;
             }
           }

           case DEL_WORD: {
             if (is_pressed) {
              if (isMac) {
                up(KC_LGUI); with_1_mod(KC_BSPC, KC_LALT); down(KC_LGUI);
              } else if (isWin) {
                with_1_mod(KC_LEFT, KC_LSFT); key_code(KC_DEL);
              }
              return false;
           }}

           case DIR_UP: { if (is_pressed) { remove_hypr(); SEND_STRING("cd .. && ls"); key_code(KC_ENTER); add_hypr(); return false; } }

           case DOCKER_LOGS: { if (is_pressed) { remove_hypr(); SEND_STRING("docker logs -f --tail=1000 $(docker ps -a -q | head -1)"); key_code(KC_ENTER); add_hypr(); return false; } }
           case DOCKER_LIST: { if (is_pressed) { remove_hypr(); SEND_STRING("docker ps -a"); key_code(KC_ENTER); add_hypr(); return false; } }

           case VIM_SAVE_QUIT: { if (is_pressed) { remove_hypr(); key_code(KC_ESC); _delay_ms(100); SEND_STRING(":wq!"); key_code(KC_ENTER); add_hypr(); return false; } }
           case VIM_QUIT: { if (is_pressed) { remove_hypr(); key_code(KC_ESC); _delay_ms(100); SEND_STRING(":q!"); key_code(KC_ENTER); add_hypr(); return false; } }

           case MAIL: { if (is_pressed) { SEND_STRING("oleksii.danilov@gmail.com"); return false; } }

           case NEXT_APP: {
                if (is_pressed) {
                   if (isMac) { remove_meh(); key_code(KC_TAB); add_meh(); }
                   else if (isWin) { remove_mods(KC_LGUI, KC_LSFT, KC_LCTL, KC_NO); key_code(KC_TAB); insert_mods(KC_LGUI, KC_LSFT, KC_LCTL, KC_NO); }
                   return false;
                }
           }

           case PREV_APP: {
                if (is_pressed) {
                   if (isMac) { remove_mods(KC_LALT, KC_LCTL, KC_NO, KC_NO); key_code(KC_TAB); insert_mods(KC_LALT, KC_LCTL, KC_NO, KC_NO); }
                   else if (isWin) { remove_mods(KC_LGUI, KC_LCTL, KC_NO, KC_NO); key_code(KC_TAB); insert_mods(KC_LGUI, KC_LCTL, KC_NO, KC_NO); }
                   return false;
                }
           }

           case CLOSE_APP: {
                if (is_pressed) {
                   if (isMac) { remove_meh(); key_code(KC_Q); add_meh(); }
                   else if (isWin) { remove_mods(KC_LGUI, KC_LCTL, KC_LSFT, KC_NO); key_code(KC_F4); insert_mods(KC_LGUI, KC_LCTL, KC_LSFT, KC_NO); }
                   return false;
                }
           }

           case TERMINAL_CLEAR: {
               if (is_pressed) {
                   remove_hypr();
                   if (isMac) {
                     SEND_STRING("printf \'\\e]50;ClearScrollback\\a\'"); // works in iterm2
                   } else if (isWin) {
                     SEND_STRING("clear"); // works in conemu/msys64
                   }
                   key_code(KC_ENTER);
                   add_hypr();
                   return false;
               }
           }

           case SLEEP: {
                if (is_pressed) {
                   if (isMac) { down(KC_LCTL); down(KC_LSFT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LSFT); up(KC_LCTL); }
                    else if (isWin) { with_1_mod(KC_D, KC_LGUI); _delay_ms(200); down(KC_LALT); _delay_ms(200); key_code(KC_F4); up(KC_LALT); _delay_ms(200); key_code(KC_UP); key_code(KC_ENTER); }
                   return false;
                }
           }

           case SHUTDOWN: {
            if (is_pressed) {
              if (isMac) { up(KC_LSFT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); down(KC_LSFT); }
              else if (isWin) { remove_hypr(); with_1_mod(KC_D, KC_LGUI); _delay_ms(200); down(KC_LALT); _delay_ms(200); key_code(KC_F4); up(KC_LALT); _delay_ms(200); key_code(KC_ENTER); add_meh(); }
              return false;
              }
           }
         }

    return MACRO_NONE;
};

/*
* ,-------------------------------------------------------------------------------------------------------------------.
* | SLEEP  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | PSCR | SLCK | PAUS |      |Program |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* | F13    |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  |        |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | `~     |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | Ins    |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | Mail   |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  |        |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |  F17 |  [{  |  ,<  |  ]}  |                                         | Left | Down | Right|  F18 |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,---------------------------.
*                            | LShift+RAlt/BkSp |Macro1|         |Macro2| LShift+RAlt/Del    |
*                            `-----------|------|------|         |------+------+-------------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LCMD/|Shift/|------|         |------|Shift/|RCMD/ |
*                                 | ESC  |Enter |Ctrl/ |         |      |Tab   |SPACE |
*                                 |      |      |CMD_BkSp|     |Ctrl/F16|      |      |
*                                 `--------------------'         `--------------------'
*                             PALM/F14                                            PALM/F15
*/

// base mac layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_MAC] = LAYOUT(
           // left side
           M(SLEEP), KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8,
           KC_F13, KC_1, KC_2, KC_3, KC_4, KC_5,
           KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_INS,KC_A, KC_S, KC_D, KC_F, KC_G,
           M(MAIL), KC_Z, KC_X, KC_C, KC_V, KC_B,
                 KC_F17, KC_LBRC, TD(COMMA_TD), KC_RBRC,
                                           // left thumb keys
			                                    ALT_SHIFT_BS,TD(TAP_MACRO1),
                                                   ALT_SLASH_MAC,
                           CMD_ESC, SHIFT_ENTER, CTRL_CMD_BS,
                                     // left palm key
			                         HYPR_F14,
    // right side
    KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,KC_NO ,KC_NO  ,KC_NO, KC_NO, RESET,
	KC_6, KC_7, KC_8, KC_9, KC_0, HYPR(F19),
	KC_Y, KC_U, KC_I, KC_O, KC_P, HYPR(F20),
	KC_H, KC_J, TD(K_TD), KC_L, KC_SCLN, HYPR(F21),
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, HYPR(F22),
	KC_LEFT, KC_DOWN, KC_RGHT, KC_F18,
           // right thumb keys
           TD(TAP_MACRO2), KC_DEL,
           ALT_BSLASH_MAC,
           CTRL_F16, SHIFT_TAB, CMD_SPACE,
                                    // right palm key
                                    HYPR_F15
    ),

// mac-specific layers
[_COMMAND_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,   __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    CMD_ESC, __________,  __________,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         MOD_Y,  MOD_U,  MOD_I,  MOD_O,  __________,  __________,
         CTRL_H,  MOD_J,  MOD_K,  MOD_L,  __________,  __________,
         MOD_N,  CTRL_M,  MOD_UP,  CTRL_DOT ,  __________,  __________,
                   MOD_LEFT,  MOD_DOWN,  MOD_RIGHT, __________,
         __________,  __________,
         PIPE_MOD,
         KC_F16, SHIFT_TAB_CTRL, MOD_SPACE,
                                     KC_F15
    ),

[_COMMAND_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  MOD_W,  MOD_E,  MOD_R,  MOD_T,
         __________,  __________,  MOD_S,  MOD_D,  MOD_F,  MOD_G,
         __________,  __________,  MOD_X,  MOD_C,  MOD_V,  MOD_B,
                   __________,  MOD_LBRC,  CTRL_COMMA,  MOD_RBRC,
                             M(DEL_WORD),  __________,
                                        KC_Z,
                    MOD_ESC, MOD_ENTER, LSFT(KC_Z),
                                     KC_NO,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________,  CMD_SPACE,
                                     __________
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
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_F16,  KC_F1,  KC_F2,
                           KC_NO
    ),

[_ALT_SLASH_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       ALT_SLASH_MAC,
                    __________, __________,  __________,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_F16,  KC_TAB,  KC_SPC,
                           KC_NO
    ),

[_ALT_BSLASH_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             KC_BSPC,  __________,
                                       KC_SLSH,
                    KC_ESC, KC_ENTER,  LGUI(KC_BSPC),
                                     KC_NO,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         ALT_BSLASH_MAC,
         __________,  __________,  __________,
                           __________
    ),

[_CTRL_CMD_BS] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________,  CTRL_CMD_BS,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_F16,  KC_TAB,  KC_SPC,
                            KC_NO
    ),

// base win layer
[_WIN] = LAYOUT(
           // left side
           M(SLEEP), KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8,
           KC_F13, KC_1, KC_2, KC_3, KC_4, KC_5,
           KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_INS, KC_A, KC_S, KC_D, KC_F, KC_G,
           M(MAIL), KC_Z, KC_X, KC_C, KC_V, KC_B,
                 KC_F17, KC_LBRC, TD(COMMA_TD), KC_RBRC,
                                           // left thumb keys
			                                    CTRL_SHIFT_BS,TD(TAP_MACRO1),
                                                   ALT_SLASH_WIN,
                           CTRL_ESC, SHIFT_ENTER, CTRL_ALT_DEL,
                                     // left palm key
			                         HYPR_F14,
    // right side
    KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,KC_PSCR ,KC_SLCK  ,KC_PAUS, KC_NO, RESET,
	KC_6, KC_7, KC_8, KC_9, KC_0, HYPR(F19),
	KC_Y, KC_U, KC_I, KC_O, KC_P, HYPR(F20),
	KC_H, KC_J, TD(K_TD), KC_L, KC_SCLN, HYPR(F21),
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, HYPR(F22),
	KC_LEFT, KC_DOWN, KC_RGHT, KC_APP,
           // right thumb keys
           TD(TAP_MACRO2), KC_DEL,
           ALT_BSLASH_WIN,
           KC_RGUI, SHIFT_TAB, CTRL_SPACE,
                                    // right palm key
                                    HYPR_F15
    ),

// win-specific layers
[_CONTROL_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,   __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  HYPR(KC_COMM),  __________,
                             __________,  __________,
                                       __________,
                    CTRL_ESC, __________,  __________,
                                     __________,

         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
           MOD_Y,  MOD_U,  MOD_I,  MOD_O,  __________,  __________,
           MOD_H,  MOD_J,  MOD_K,  MOD_L,  __________,  __________,
           MOD_N,  MOD_M,  MOD_UP,  MOD_DOT ,  __________,  __________,
                     MOD_LEFT,  MOD_DOWN,  MOD_RIGHT, __________,
         __________,  __________,
         PIPE_MOD,
         KC_F16, SHIFT_TAB_CTRL, MOD_SPACE,
                                     KC_F15
    ),

[_CONTROL_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  MOD_W,  MOD_E,  MOD_R,  MOD_T,
         __________,  __________,  MOD_S,  MOD_D,  MOD_F,  MOD_G,
         __________,  __________,  MOD_X,  MOD_C,  MOD_V,  MOD_B,
                      __________,  MOD_LBRC, MOD_COMMA, MOD_RBRC,
                             M(DEL_WORD),  __________,
                                        KC_Z,
                    MOD_ESC, MOD_ENTER, LSFT(KC_Z),
                                     KC_NO,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________,  CTRL_SPACE,
                                     __________
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
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_F16,  KC_F1,  KC_F2,
                          KC_NO
    ),

[_ALT_SLASH_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       ALT_SLASH_WIN,
                    __________, __________,  __________,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_F16,  KC_0,  KC_1,
                         KC_NO
    ),

[_ALT_BSLASH_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             KC_BSPC,  __________,
                                       KC_SLSH,
                    KC_F17, KC_ENTER,  KC_DEL,
                                     KC_NO,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         ALT_BSLASH_WIN,
         __________,  __________,  __________,
                           __________
    ),

[_CTRL_ALT_DEL] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________,  CTRL_ALT_DEL,
                                     __________,
         W_F9,  W_F10,  W_F11,  KC_F12,  __________,  __________, __________, __________, __________,
         W_6,  W_7,  W_8,  W_9,  W_0,  __________,
         W_Y,  W_U,  W_I,  W_O,  W_P,  __________,
         W_H,  W_J,  W_K, W_L,  W_QUOT,  __________,
         W_N, W_M, __________, W_DOT, W_SCLN,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_F16, KC_0, KC_1,
                      KC_NO
    ),

// common layers
[_SHIFT_ENTER] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, SHIFT_ENTER,  __________,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         MEH(KC_BSLS),
         KC_F16,  MEH(KC_F13), KC_SPC,
         KC_NO
    ),

// common layers
[_SHIFT_TAB] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             KC_BSPC,  __________,
                                       MEH(KC_SLSH),
                    KC_ESC, KC_ENTER,  __________,
                                     KC_NO,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         __________,
         __________,  SHIFT_TAB,  __________,
                           __________
    ),

[_PALM_R] = LAYOUT(
M(SHUTDOWN),  __________,  __________,  __________,  __________,  SET_LAYER_MAC, __________, SET_LAYER_WIN, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________, VOL_DOWN, M(MUTE), VOL_UP,
                                 M(PLAY_UP), __________,
                                     M(PLAY_PREV),
                    M(PLAY_PAUSE), M(PLAY_NEXT), M(PLAY_DOWN),
                                KC_NO,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                             HYPR_F15
    ),

[_PALM_L] = LAYOUT(
__________,  __________,  __________,  __________,  __________,  SET_LAYER_MAC, __________, SET_LAYER_WIN, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________, VOL_DOWN, M(MUTE), VOL_UP,
                             __________,  __________,
                                     __________,
                      M(PLAY_PAUSE), __________,  __________,
                                     HYPR_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  M(VIM_SAVE_QUIT),  __________,  M(VIM_QUIT),  __________,  __________,
         __________,  M(POS_LEFT), M(POS_FULL),  M(POS_RIGHT),  __________,  __________,
         __________,  M(PREV_APP),  M(CLOSE_APP),  M(NEXT_APP),  __________,  __________,
         __________,  KC_0,  KC_PGUP,  KC_1,  __________,  __________,
                                KC_HOME,  KC_PGDN, KC_END, __________,
         __________,  M(TERMINAL_CLEAR),
         M(DOCKER_LIST),
         M(DOCKER_LOGS),  M(DIR_UP),  M(POS_MINIMIZE),
                                  KC_F15
    ),
};

// support for repeat keycodes (only for palm layer keys)
void matrix_scan_user(void) {
   if (palm_repeat_code) {
      if (timer_elapsed(palm_repeat_timer) > (10 + first_repeat_delay)) {
         without_hypr(palm_repeat_code);
         palm_repeat_timer = timer_read();
         first_repeat_delay = 0;
      }
   }
}

// support for "mo layer tap" functionality: activate mod as soon as layer is activated -> to allow key + mouse combination without delay
uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    // mac-specific
    case _COMMAND_ESCAPE: down(KC_LGUI); break;
    case _COMMAND_SPACE: down(KC_LGUI); break;
    case _ALT_SHIFT_BS: down(KC_LALT); down(KC_LSFT); break;
    case _ALT_SLASH_MAC: down(KC_LALT); break;
    case _CTRL_CMD_BS: down(KC_LCTL); break;

    // win-specific
    case _CONTROL_ESCAPE: down(KC_LCTL); break;
    case _CONTROL_SPACE: down(KC_LCTL); break;
    case _CTRL_SHIFT_BS: down(KC_LCTL); down(KC_LSFT); break;
    case _ALT_SLASH_WIN: down(KC_LALT); break;
    case _CTRL_ALT_DEL: down(KC_LCTL); down(KC_LALT); break;

    // common
    case _SHIFT_ENTER: down(KC_LSFT); break;
    case _SHIFT_TAB: down(KC_LSFT); break;
    case _ALT_BSLASH_MAC: down(KC_LALT); break;
    case _ALT_BSLASH_WIN: down(KC_LALT); break;
    case _PALM_L: down(KC_LGUI); down(KC_LSFT); down(KC_LALT); down(KC_LCTL); break;
    case _PALM_R: down(KC_LGUI); down(KC_LSFT); down(KC_LALT); down(KC_LCTL); break;

    // unregister everything (even if it was not pressed - no big deal; this works faster than getting pressed mods)
    default: up(KC_LGUI); up(KC_LCTL); up(KC_LALT); up(KC_LSFT); break;
    }
return state;
}

// support for "mo layer tap" functionality
bool cmd_esc_interrupted = true;
bool shift_enter_interrupted = true;
bool shift_tab_interrupted = true;
bool hypr_f14_interrupted = true;
bool hypr_f15_interrupted = true;
bool alt_slash_mac_interrupted = true;
bool alt_slash_win_interrupted = true;
bool alt_bslash_mac_interrupted = true;
bool alt_bslash_win_interrupted = true;
bool ctrl_cmd_bs_interrupted = true;
bool ctrl_alt_del_interrupted = true;
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
    if (keycode != SHIFT_ENTER) { shift_enter_interrupted = true; }
    if (keycode != SHIFT_TAB) { shift_tab_interrupted = true; }
    if (keycode != HYPR_F14) { hypr_f14_interrupted = true; }
    if (keycode != HYPR_F15) { hypr_f15_interrupted = true; }
    if (keycode != ALT_SLASH_MAC) { alt_slash_mac_interrupted = true; }
    if (keycode != ALT_SLASH_WIN) { alt_slash_win_interrupted = true; }
    if (keycode != ALT_BSLASH_MAC) { alt_bslash_mac_interrupted = true; }
    if (keycode != ALT_BSLASH_WIN) { alt_bslash_win_interrupted = true; }
    if (keycode != CTRL_CMD_BS) { ctrl_cmd_bs_interrupted = true; }
    if (keycode != ALT_SHIFT_BS) { alt_shift_interrupted = true; }
    if (keycode != CTRL_ESC) { ctrl_esc_interrupted = true; }
    if (keycode != CTRL_SHIFT_BS) { ctrl_shift_interrupted = true; }
    if (keycode != CTRL_ALT_DEL) { ctrl_alt_del_interrupted = true; }

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
         if (momentary_layer_tap(KC_ESC, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, &cmd_esc_layer_timer, &cmd_esc_interrupted, is_pressed, 180, true)) {
           esc_timer = timer_read();
         }
         return true;
        }

        case ALT_SHIFT_BS: {
          static uint16_t alt_shift_layer_timer;
          momentary_layer_tap(KC_BSPC, KC_NO, KC_LSFT, KC_LALT, KC_NO, KC_NO, &alt_shift_layer_timer, &alt_shift_interrupted, is_pressed, 180, true);
          return true;
        }

        case CTRL_CMD_BS: {
          static uint16_t ctrl_cmd_bs_layer_timer;
          momentary_layer_tap(KC_BSPC, KC_LGUI, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_cmd_bs_layer_timer, &ctrl_cmd_bs_interrupted, is_pressed, 180, true);
          return true;
        }

        case CTRL_ALT_DEL: {
          static uint16_t ctrl_alt_del_layer_timer;
          momentary_layer_tap(KC_DEL, KC_NO, KC_LALT, KC_LCTL, KC_NO, KC_NO, &ctrl_alt_del_layer_timer, &ctrl_alt_del_interrupted, is_pressed, 180, false);
          return true;
        }

        case ALT_SLASH_MAC: {
          static uint16_t alt_slash_mac_layer_timer;
          momentary_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_slash_mac_layer_timer, &alt_slash_mac_interrupted, is_pressed, 180, true);
          return true;
        }

        case ALT_SLASH_WIN: {
          static uint16_t alt_slash_win_layer_timer;
          momentary_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_slash_win_layer_timer, &alt_slash_win_interrupted, is_pressed, 180, false);
          return true;
        }

        case ALT_BSLASH_MAC: {
          static uint16_t alt_bslash_mac_layer_timer;
          momentary_layer_tap(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bslash_mac_layer_timer, &alt_bslash_mac_interrupted, is_pressed, 180, true);
          return true;
        }

        case ALT_BSLASH_WIN: {
          static uint16_t alt_bslash_win_layer_timer;
          momentary_layer_tap(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bslash_win_layer_timer, &alt_bslash_win_interrupted, is_pressed, 180, false);
          return true;
        }

        // win-specific layers
        case CTRL_ESC: {
         static uint16_t ctrl_esc_layer_timer;
         if (momentary_layer_tap(KC_ESC, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_esc_layer_timer, &ctrl_esc_interrupted, is_pressed, 180, true)) {
           esc_timer = timer_read();
         }
         return true;
        }

        case CTRL_SHIFT_BS: {
          static uint16_t ctrl_shift_layer_timer;
          momentary_layer_tap(KC_BSPC, KC_NO, KC_LSFT, KC_LCTL, KC_NO, KC_NO, &ctrl_shift_layer_timer, &ctrl_shift_interrupted, is_pressed, 180, true);
          return true;
        }

        // common layers
        case HYPR_F14: {
          static uint16_t hypr_f14_layer_timer;
          momentary_layer_tap(KC_F14, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_LGUI, &hypr_f14_layer_timer, &hypr_f14_interrupted, is_pressed, 300, isMac ? true : false);
          return true;
        }

        // common layers
        case SHIFT_ENTER: {
          static uint16_t shift_enter_layer_timer;
          momentary_layer_tap(KC_ENTER, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_enter_layer_timer, &shift_enter_interrupted, is_pressed, 300, true);
          return true;
        }

        case SHIFT_TAB: {
          static uint16_t shift_tab_layer_timer;
          momentary_layer_tap(KC_TAB, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_tab_layer_timer, &shift_tab_interrupted, is_pressed, 300, true);
          return true;
        }

        case HYPR_F15: {
          static uint16_t hypr_f15_layer_timer;
          momentary_layer_tap(KC_F15, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_LGUI, &hypr_f15_layer_timer, &hypr_f15_interrupted, is_pressed, 300, isMac ? true : false);
          return true;
        }

        // LWin + key overrides
        case W_F9: { return replace_ctrl_alt_with_lgui(KC_F9, is_pressed); }
        case W_F10: { return replace_ctrl_alt_with_lgui(KC_F10, is_pressed); }
        case W_F11: { return replace_ctrl_alt_with_lgui(KC_F11, is_pressed); }
        case W_F12: { return replace_ctrl_alt_with_lgui(KC_F12, is_pressed); }
        case W_6: { return replace_ctrl_alt_with_lgui(KC_6, is_pressed); }
        case W_7: { return replace_ctrl_alt_with_lgui(KC_7, is_pressed); }
        case W_8: { return replace_ctrl_alt_with_lgui(KC_8, is_pressed); }
        case W_9: { return replace_ctrl_alt_with_lgui(KC_9, is_pressed); }
        case W_0: { return replace_ctrl_alt_with_lgui(KC_0, is_pressed); }
        case W_Y: { return replace_ctrl_alt_with_lgui(KC_Y, is_pressed); }
        case W_U: { return replace_ctrl_alt_with_lgui(KC_U, is_pressed); }
        case W_I: { return replace_ctrl_alt_with_lgui(KC_I, is_pressed); }
        case W_O: { return replace_ctrl_alt_with_lgui(KC_O, is_pressed); }
        case W_P: { return replace_ctrl_alt_with_lgui(KC_P, is_pressed); }
        case W_H: { return replace_ctrl_alt_with_lgui(KC_H, is_pressed); }
        case W_J: { return replace_ctrl_alt_with_lgui(KC_J, is_pressed); }
        case W_K: { return replace_ctrl_alt_with_lgui(KC_K, is_pressed); }
        case W_L: { return replace_ctrl_alt_with_lgui(KC_L, is_pressed); }
        case W_QUOT: { return replace_ctrl_alt_with_lgui(KC_QUOT, is_pressed); }
        case W_N: { return replace_ctrl_alt_with_lgui(KC_N, is_pressed); }
        case W_M: { return replace_ctrl_alt_with_lgui(KC_M, is_pressed); }
        case W_DOT: { return replace_ctrl_alt_with_lgui(KC_DOT, is_pressed); }
        case W_SCLN: { return replace_ctrl_alt_with_lgui(KC_SCLN, is_pressed); }

        // REPEATING KEYCODES
        case KC_HOME: { return repeat_without_hypr(KC_HOME, is_pressed); }
        case KC_END: { return repeat_without_hypr(KC_END, is_pressed); }
        case KC_PGUP: { return repeat_without_hypr(KC_PGUP, is_pressed); }
        case KC_PGDN: { return repeat_without_hypr(KC_PGDN, is_pressed); }

        case VOL_UP: { return repeat_without_hypr(os_specific_key(KC__VOLUP, KC_VOLU), is_pressed); }
        case VOL_DOWN: { return repeat_without_hypr(os_specific_key(KC__VOLDOWN, KC_VOLD), is_pressed); }

        // ESCAPE AS LEADER KEY
        // ctrl home/end
        case KC_LEFT: { return following_custom_leader_key(KC_LEFT, KC_LSFT, os_specific_key(KC_LCTL, KC_LALT), KC_NO, &esc_timer, is_pressed, 250); }
        case KC_RGHT: { return following_custom_leader_key(KC_RGHT, KC_LSFT, os_specific_key(KC_LCTL, KC_LALT), KC_NO, &esc_timer, is_pressed, 250); }

        // CUSTOM KEYCODES
        // mac-only overrides
        case CTRL_COMMA: { return replace_cmd_if_held_add_cmd_shift(KC_COMM, KC_LCTL, is_pressed, 180); }
        case CTRL_DOT: { return replace_cmd_if_held_add_cmd_shift(KC_DOT, KC_LCTL, is_pressed, 180); }
        case CTRL_H: { return replace_cmd_if_held_add_cmd_shift(KC_H, KC_LCTL, is_pressed, 180); }
        case CTRL_M: { return replace_cmd_if_held_add_cmd_shift(KC_M, KC_LCTL, is_pressed, 180); }

        case MOD_ENTER: { return if_held_140_add_shift(KC_ENTER, is_pressed); }
        case MOD_TAB: { return if_held_140_add_shift(KC_TAB, is_pressed); }
        case MOD_SPACE: { return if_held_140_add_shift(KC_SPC, is_pressed); }
        case MOD_ESC: { return if_held_140_add_shift(os_specific_key(KC_ESC, KC_BSPC), is_pressed); }

        case MOD_W: { return if_held_180_add_shift(KC_W, is_pressed); }
        case MOD_E: { return if_held_180_add_shift(KC_E, is_pressed); }
        case MOD_R: { return if_held_180_add_shift(KC_R, is_pressed); }
        case MOD_T: { return if_held_180_add_shift(KC_T, is_pressed); }
        case MOD_S: { return if_held_180_add_shift(KC_S, is_pressed); }
        case MOD_D: { return if_held_180_add_shift(KC_D, is_pressed); }
        case MOD_F: { return if_held_180_add_shift(KC_F, is_pressed); }
        case MOD_G: { return if_held_180_add_shift(KC_G, is_pressed); }
        case MOD_X: { return if_held_180_add_shift(KC_X, is_pressed); }
        case MOD_C: { return if_held_180_add_shift(KC_C, is_pressed); }
        case MOD_V: { return if_held_180_add_shift(KC_V, is_pressed); }
        case MOD_B: { return if_held_180_add_shift(KC_B, is_pressed); }
        case MOD_COMMA: { return if_held_180_add_shift(KC_COMM, is_pressed); }
        case MOD_LBRC: { return if_held_180_add_shift(KC_LBRC, is_pressed); }
        case MOD_RBRC: { return if_held_180_add_shift(KC_RBRC, is_pressed); }

        case MOD_Y: { return if_held_180_add_shift(KC_Y, is_pressed); }
        case MOD_U: { return if_held_180_add_shift(KC_U, is_pressed); }
        case MOD_I: { return if_held_180_add_shift(KC_I, is_pressed); }
        case MOD_O: { return if_held_180_add_shift(KC_O, is_pressed); }
        case MOD_H: { return if_held_180_add_shift(KC_H, is_pressed); }
        case MOD_J: { return if_held_180_add_shift(KC_J, is_pressed); }
        case MOD_K: { return if_held_180_add_shift(KC_K, is_pressed); }
        case MOD_L: { return if_held_180_add_shift(KC_L, is_pressed); }
        case MOD_N: { return if_held_180_add_shift(KC_N, is_pressed); }
        case MOD_M: { return if_held_180_add_shift(KC_M, is_pressed); }
        case MOD_DOT: { return if_held_180_add_shift(KC_DOT, is_pressed); }

        case PIPE_MOD: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_BSLS, os_specific_key(KC_LGUI, KC_LCTL), KC_NO, KC_LSFT, KC_NO, KC_BSLS, KC_LSFT, os_specific_key(KC_LGUI, KC_LCTL), is_pressed, 180); }
        case SHIFT_TAB_CTRL: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, os_specific_key(KC_LGUI, KC_LCTL), KC_NO, KC_LSFT, KC_NO, KC_TAB, KC_LSFT, KC_LCTL, is_pressed, 180); }

        case MOD_LEFT: { return if_held_add_mods(KC_LEFT, os_specific_key(KC_LCTL, KC_LALT), KC_LSFT, is_pressed, 180); }
        case MOD_RIGHT: { return if_held_add_mods(KC_RGHT, os_specific_key(KC_LCTL, KC_LALT), KC_LSFT, is_pressed, 180); }
        case MOD_UP: { return if_held_add_mods(KC_UP, os_specific_key(KC_LCTL, KC_LALT), KC_LSFT, is_pressed, 180); }
        case MOD_DOWN: { return if_held_add_mods(KC_DOWN, os_specific_key(KC_LCTL, KC_LALT), KC_LSFT, is_pressed, 180); }

        case KC_0: { return if_held_180_replace(KC_0, KC_0, KC_NO, is_pressed); } // disable key repeat
        case KC_1: { return if_held_180_replace(KC_1, KC_1, KC_NO, is_pressed); }
        case KC_2: { return if_held_180_replace(KC_2, KC_9, KC_LSFT, is_pressed); }
        case KC_3: { return if_held_180_replace(KC_3, KC_MINS, KC_LSFT, is_pressed); }
        case KC_4: { return if_held_180_replace(KC_4, KC_0, KC_LSFT, is_pressed); }
        case KC_5: { return if_held_180_replace(KC_5, KC_EQL, KC_NO, is_pressed); }
        case KC_6: { return if_held_180_replace(KC_6, KC_EQL, KC_LSFT, is_pressed); }
        case KC_7: { return if_held_180_replace(KC_7, KC_1, KC_LSFT, is_pressed); }
        case KC_8: { return if_held_180_replace(KC_8, KC_MINS, KC_NO, is_pressed); }
        case KC_9: { return if_held_180_replace(KC_9, KC_SLSH, KC_LSFT, is_pressed); }

        case KC_F1: { return if_held_180_add_shift(KC_F1, is_pressed); }
        case KC_F2: { return if_held_180_add_shift(KC_F2, is_pressed); }
        case KC_F3: { return if_held_180_add_shift(KC_F3, is_pressed); }
        case KC_F4: { return if_held_180_add_shift(KC_F4, is_pressed); }
        case KC_F5: { return if_held_180_add_shift(KC_F5, is_pressed); }
        case KC_F6: { return if_held_180_add_shift(KC_F6, is_pressed); }
        case KC_F7: { return if_held_180_add_shift(KC_F7, is_pressed); }
        case KC_F8: { return if_held_180_add_shift(KC_F8, is_pressed); }
        case KC_F9: { return if_held_180_add_shift(KC_F9, is_pressed); }
        case KC_F10: { return if_held_180_add_shift(KC_F10, is_pressed); }
        case KC_F11: { return if_held_180_add_shift(KC_F11, is_pressed); }
        case KC_F12: { return if_held_180_add_shift(KC_F12, is_pressed); }
        case KC_F13: { return if_held_180_add_shift(KC_F13, is_pressed); }
        case KC_F14: { return if_held_180_add_shift(KC_F14, is_pressed); }
        case KC_F16: { return if_held_180_add_shift(KC_F16, is_pressed); }

        default: {
          return true;
        }
    }
}