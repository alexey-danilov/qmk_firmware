#include "kinesis.h"

#define __________ KC_TRNS
#define XXXXXXX KC_NO

#define _MAC 0
#define _WIN 1

// mac layers
#define _COMMAND_ESCAPE 2
#define _COMMAND_SPACE 3
#define _ALT_SHIFT_BS 4
#define _CTRL_CMD_BS 5
#define _CTRL_NUBS 6
#define _ALT_SLASH_MAC 7
#define _ALT_BSLASH_MAC 8
#define _SHIFT_ENTER_MAC 9
#define _SHIFT_TAB_MAC 10
#define _PALM_L_MAC 11
#define _PALM_R_MAC 12

// win layers
#define _CONTROL_ESCAPE 13
#define _CONTROL_SPACE 14
#define _CTRL_SHIFT_BS 15
#define _CTRL_ALT_DEL 16
#define _ALT_SLASH_WIN 17
#define _ALT_BSLASH_WIN 18
#define _SHIFT_ENTER_WIN 19
#define _SHIFT_TAB_WIN 20
#define _PALM_L_WIN 21
#define _PALM_R_WIN 22

#define _KEYB_CONTROL 24

enum kinesis_keycodes {
  // mac
  CMD_ESC = MO(_COMMAND_ESCAPE),
  CMD_SPACE = LT(_COMMAND_SPACE, KC_SPC),
  ALT_SHIFT_BS = MO(_ALT_SHIFT_BS),
  ALT_SLASH_MAC = MO(_ALT_SLASH_MAC),
  ALT_BSLASH_MAC = MO(_ALT_BSLASH_MAC),
  CTRL_CMD_BS = MO(_CTRL_CMD_BS),
  CTRL_NUBS = MO(_CTRL_NUBS),
  SHIFT_ENTER_MAC = MO(_SHIFT_ENTER_MAC),
  SHIFT_TAB_MAC = MO(_SHIFT_TAB_MAC),
  HYPR_F14_MAC = MO(_PALM_L_MAC),
  PALM_F15_MAC = MO(_PALM_R_MAC),

  // win
  CTRL_ESC = MO(_CONTROL_ESCAPE),
  CTRL_SPACE = LT(_CONTROL_SPACE, KC_SPC),
  CTRL_SHIFT_BS = MO(_CTRL_SHIFT_BS),
  ALT_SLASH_WIN = MO(_ALT_SLASH_WIN),
  ALT_BSLASH_WIN = MO(_ALT_BSLASH_WIN),
  CTRL_ALT_DEL = MO(_CTRL_ALT_DEL),
  SHIFT_ENTER_WIN = MO(_SHIFT_ENTER_WIN),
  SHIFT_TAB_WIN = MO(_SHIFT_TAB_WIN),
  HYPR_F14_WIN = MO(_PALM_L_WIN),
  PALM_F15_WIN = MO(_PALM_R_WIN),

  // firmware-related stuff
  KEYB_CONTROL = MO(_KEYB_CONTROL)
};

enum holding_keycodes {
  MOD_ESC = SAFE_RANGE,
  MOD_ENTER,
  MOD_DELETE,
  MOD_CTRL,
  MOD_TAB,
  MOD_SPACE,
  MOD_INS,
  MOD_LEFT, MOD_RIGHT, MOD_UP, MOD_DOWN,
  MOD_SHIFT_SPACE,

  _0,
  _1,
  _2_PLEFT,
  _3_SLASH,
  _4_PRGHT,
  _5_EQL,
  _6_PLUS,
  _7_BANG,
  _8_DASH,
  _9_QUEST,

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

  SET_LAYER_MAC,
  SET_LAYER_WIN
};

enum macros {
  MAIL = 0,
  CLOSE_APP,
  NEXT_APP,
  PREV_APP,
  MINIMIZE_MAC,
  HIDE_OTHERS_MAC,
  SLEEP,
  SHUTDOWN,
  DEL_LEFT,
  DIR_UP,
  GIT_SQUASH,
  ITERM_CLEAR,
  CONEMU_CLEAR,
  DOCKER_LIST,
  DOCKER_LOGS,
  VIM_SAVE_QUIT,
  VIM_QUIT,
  FIND_NEXT,
  FIND_PREV
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

// adds shift to keycode if it was held for at 179 ms
bool if_held_179_add_shift(uint16_t code, bool pressed) {
  return if_held_add_mods(code, KC_LSFT, KC_NO, pressed, 179);
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

// handles repeat functionality (only for palm keys)
static uint16_t palm_repeat_code;
static uint16_t palm_repeat_timer;
static uint8_t first_repeat_delay;
bool repeat(uint16_t code, uint16_t remove_mod1, uint16_t remove_mod2, uint16_t remove_mod3, uint16_t remove_mod4, bool pressed) {
   if (pressed) {
     up(remove_mod1); up(remove_mod2); up(remove_mod3); up(remove_mod4);
     key_code(code);

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

// quad tap dance
enum {
  K_TD = 1,
  F13_TD = 2,
  F16_TD = 3,
  POWER_TD = 4
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
    case DOUBLE_SINGLE_TAP: key_code(KC_K), down(KC_K); break;
    case SINGLE_HOLD: down(KC_LSFT); key_code(KC_K); break;
    case DOUBLE_HOLD:
      if (isMac) { down(KC_LOCKING_CAPS); break; }
      if (isWin) { down(KC_CAPS); break; }
    default:
      if (isMac) { down(KC_LCTL); down(KC_LALT); key_code(KC_SPC); up(KC_LCTL); break; }
      if (isWin) { down(KC_LGUI); key_code(KC_SPC); break; }
    }
  }

void k_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (k_tap_state.state) {
    case SINGLE_TAP: up(KC_K); break;
    case DOUBLE_SINGLE_TAP: up(KC_K); break;
    case SINGLE_HOLD: up(KC_LSFT); break;
    case DOUBLE_HOLD:
      if (isMac) { up(KC_LOCKING_CAPS); break; }
      if (isWin) { up(KC_CAPS); break; }
    default:
      if (isMac) { up(KC_LALT); break; }
      if (isWin) { up(KC_LGUI); break; }
    }
  k_tap_state.state = 0;
}

//**************** F13 TAP *********************//
static tap f13_tap_state = { .is_press_action = true, .state = 0 };

void f13_finished (qk_tap_dance_state_t *state, void *user_data) {
  f13_tap_state.state = cur_dance(state);
  switch (f13_tap_state.state) {
    case SINGLE_TAP: down(KC_F13); break;
    case SINGLE_HOLD: down(KC_LSFT); key_code(KC_F13); break;
    case DOUBLE_TAP:
      if (isMac) { down(KC_LCTL); key_code(KC_F13); break; }
      if (isWin) { down(KC_LCTL); down(KC_LALT); key_code(KC_F13); up(KC_LALT); break; }
    default: // double hold
      if (isMac) { down(KC_LCTL); down(KC_LALT); key_code(KC_F13); up(KC_LALT); break; }
      if (isWin) { down(KC_LSFT); down(KC_LALT); key_code(KC_F13); up(KC_LALT); break; }
  }
}

void f13_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (f13_tap_state.state) {
    case SINGLE_TAP: up(KC_F13); break;
    case SINGLE_HOLD: up(KC_LSFT); break;
    case DOUBLE_TAP: up(KC_LCTL); break;
    default: // double hold
      if (isMac) {  up(KC_LCTL); break; }
      if (isWin) { up(KC_LSFT); break; }
    }
  f13_tap_state.state = 0;
}

//**************** F16 TAP *********************//
static tap f16_tap_state = { .is_press_action = true, .state = 0 };

void f16_finished (qk_tap_dance_state_t *state, void *user_data) {
  f16_tap_state.state = cur_dance(state);
  switch (f16_tap_state.state) {
    case SINGLE_TAP: down(KC_F16); break;
    case SINGLE_HOLD: down(KC_LSFT); key_code(KC_F16); break;
    case DOUBLE_TAP:
      if (isMac) { down(KC_LCTL); key_code(KC_F16); break; }
      if (isWin) { down(KC_LCTL); down(KC_LALT); key_code(KC_F16); up(KC_LALT); break; }
    default: // double hold
      if (isMac) { down(KC_LCTL); down(KC_LALT); key_code(KC_F16); up(KC_LALT); break; }
      if (isWin) { down(KC_LSFT); down(KC_LALT); key_code(KC_F16); up(KC_LALT); break; }    }
  }

void f16_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (f16_tap_state.state) {
    case SINGLE_TAP: up(KC_F16); break;
    case SINGLE_HOLD: up(KC_LSFT); break;
    case DOUBLE_TAP: up(KC_LCTL); break;
    default: // double hold
      if (isMac) {  up(KC_LCTL); break; }
      if (isWin) { up(KC_LSFT); break; }       }
  f16_tap_state.state = 0;
}

//**************** POWER TAP *********************//
static tap power_tap_state = { .is_press_action = true, .state = 0 };

void power_finished (qk_tap_dance_state_t *state, void *user_data) {
  power_tap_state.state = cur_dance(state);
  switch (power_tap_state.state) {
    case DOUBLE_TAP: // sleep
      if (isMac) { down(KC_LCTL); down(KC_LSFT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LSFT); break; }
      if (isWin) { with_1_mod(KC_D, KC_LGUI); _delay_ms(200); down(KC_LALT); _delay_ms(200); key_code(KC_F4); up(KC_LALT); _delay_ms(200); key_code(KC_UP); down(KC_ENTER); break; }
    case DOUBLE_HOLD: // shutdown
      if (isMac) { down(KC_LGUI); down(KC_LCTL); down(KC_LALT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LCTL); up(KC_LALT); break; }
      if (isWin) { with_1_mod(KC_D, KC_LGUI); _delay_ms(500); down(KC_LALT); _delay_ms(200); key_code(KC_F4); up(KC_LALT); _delay_ms(500); down(KC_ENTER); break; }
    }
  }

void power_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (power_tap_state.state) {
    case DOUBLE_TAP: // sleep
      if (isMac) { down(KC_LCTL); break; }
      if (isWin) { up(KC_ENTER); break; }
    case DOUBLE_HOLD: // shutdown
      if (isMac) { up(KC_LGUI); break; }
      if (isWin) { up(KC_ENTER); break; }
    }
  power_tap_state.state = 0;
}

// all tap macros
qk_tap_dance_action_t tap_dance_actions[] = {
  [K_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, k_finished, k_reset),
  [F13_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, f13_finished, f13_reset),
  [F16_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, f16_finished, f16_reset),
  [POWER_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, power_finished, power_reset)
};

// NON-TAP MACROS
void window_positioning(uint16_t mac_key, uint16_t win_key) {
  if (isMac) { key_code(mac_key); }
  if (isWin) { remove_meh(); key_code(win_key); add_meh(); }
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
    bool is_pressed = record->event.pressed;
        switch(id) {

           case FIND_NEXT: { // f3
             if (is_pressed) {
              remove_hypr();
              key_code(KC_F3);
              add_meh();
              if (isMac) { down(KC_LGUI); }
              return false;
           }}

           case FIND_PREV: { // shift f3
             if (is_pressed) {
              up(KC_LGUI); up(KC_LCTL); up(KC_LALT);
              key_code(KC_F3); // shift + f3
              down(KC_LALT); down(KC_LCTL);
              if (isMac) { down(KC_LGUI); }
              return false;
           }}

           case MINIMIZE_MAC: {
             if (is_pressed) {
                remove_meh(); key_code(KC_M); add_meh();
                return false;
             }
           }

           case HIDE_OTHERS_MAC: {
             if (is_pressed) {
                up(KC_LSFT); up(KC_LCTL); key_code(KC_H); down(KC_LCTL); down(KC_LSFT);
                return false;
             }
           }

           case DEL_LEFT: {
             if (is_pressed) {
              if (isMac) {
                up(KC_LGUI); with_1_mod(KC_BSPC, KC_LALT); down(KC_LGUI);
              } if (isWin) {
                with_1_mod(KC_LEFT, KC_LSFT); key_code(KC_DEL);
              }
              return false;
           }}

           case DIR_UP: { if (is_pressed) { SEND_STRING("cd .. && ls"); key_code(KC_ENTER); return false; } }

           case GIT_SQUASH: { if (is_pressed) { SEND_STRING("git reset --soft HEAD~XX && git commit"); return false; } }
           case DOCKER_LOGS: { if (is_pressed) { remove_hypr(); SEND_STRING("docker logs -f --tail=1000 $(docker ps -a -q | head -1)"); key_code(KC_ENTER); add_hypr(); return false; } }
           case DOCKER_LIST: { if (is_pressed) { remove_hypr(); SEND_STRING("docker ps -a"); key_code(KC_ENTER); add_hypr(); return false; } }

           case VIM_SAVE_QUIT: { if (is_pressed) { remove_hypr(); key_code(KC_ESC); _delay_ms(100); SEND_STRING(":wq!"); key_code(KC_ENTER); add_hypr(); return false; } }
           case VIM_QUIT: { if (is_pressed) { remove_hypr(); key_code(KC_ESC); _delay_ms(100); SEND_STRING(":q!"); key_code(KC_ENTER); add_hypr(); return false; } }

           case MAIL: { if (is_pressed) { up(KC_LALT); SEND_STRING("oleksii.danilov@gmail.com"); down(KC_LALT); return false; } }

           case NEXT_APP: {
                if (is_pressed) {
                   if (isMac) { remove_meh(); key_code(KC_TAB); add_meh(); }
                   if (isWin) { remove_mods(KC_LGUI, KC_LSFT, KC_LCTL, KC_NO); key_code(KC_TAB); insert_mods(KC_LGUI, KC_LSFT, KC_LCTL, KC_NO); }
                   return false;
                }
           }

           case PREV_APP: {
                if (is_pressed) {
                   if (isMac) { remove_mods(KC_LALT, KC_LCTL, KC_NO, KC_NO); key_code(KC_TAB); insert_mods(KC_LALT, KC_LCTL, KC_NO, KC_NO); }
                   if (isWin) { remove_mods(KC_LGUI, KC_LCTL, KC_NO, KC_NO); key_code(KC_TAB); insert_mods(KC_LGUI, KC_LCTL, KC_NO, KC_NO); }
                   return false;
                }
           }

           case CLOSE_APP: {
                if (is_pressed) {
                   if (isMac) { remove_meh(); key_code(KC_Q); add_meh(); }
                   if (isWin) { remove_mods(KC_LGUI, KC_LCTL, KC_LSFT, KC_NO); key_code(KC_F4); insert_mods(KC_LGUI, KC_LCTL, KC_LSFT, KC_NO); }
                   return false;
                }
           }

           case ITERM_CLEAR: {
               if (is_pressed) {
                   SEND_STRING("printf \'\\e]50;ClearScrollback\\a\'"); // works in iterm2
                   key_code(KC_ENTER);
                   return false;
               }
           }

           case CONEMU_CLEAR: {
               if (is_pressed) {
                   SEND_STRING("clear"); // works in conemu/msys64
                   key_code(KC_ENTER);
                   return false;
               }
           }
         }

    return MACRO_NONE;
};

/*
* ,-------------------------------------------------------------------------------------------------------------------.
* | Mail   |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | PSCR | SLCK | PAUS |      |Program |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* |        |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  |YEAR(19)|
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* |        |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  |YEAR(20)|
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |        |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  |YEAR(21)|
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |        |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  |YEAR(22)|
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |   `  |  [{  |  ,<  |  ]}  |                                         | Left | Down | Right | Ins |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,---------------------------.
*                            | LShift+RAlt/BkSp | F13  |         | Sleep| LShift+RAlt/Del    |
*                            `-----------|------|------|         |------+------+-------------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LCMD/|Shift/|------|         |------|Shift/|RCMD/ |
*                                 | ESC  |Enter |Ctrl/ |         |      |Tab   |SPACE |
*                                 |      |      |CMD_BkSp|     |Ctrl/F17|      |      |
*                                 `--------------------'         `--------------------'
*                             PALM/F14                                            PALM/F15
*/

// base mac layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_MAC] = LAYOUT(
           // left side
           TD(POWER_TD), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8,
           KC_F17, _1, _2_PLEFT, _3_SLASH, _4_PRGHT, _5_EQL,
           KC_F18, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_F19,KC_A, KC_S, KC_D, KC_F, KC_G,
           KC_F20, KC_Z, KC_X, KC_C, KC_V, KC_B,
           KC_GRV, KC_LBRC, KC_COMM, KC_RBRC,
                                               // left thumb keys
			                            ALT_SHIFT_BS, TD(F13_TD),
                                                   ALT_SLASH_MAC,
                           CMD_ESC, SHIFT_ENTER_MAC, CTRL_CMD_BS,
                                     // left palm key
			                         HYPR_F14_MAC,
    // right side
    KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_CLR, KC_CLR, KEYB_CONTROL, KC_CLR,
	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0, KC_F21,
	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_F22,
	KC_H, KC_J, TD(K_TD), KC_L, KC_SCLN, KC_F23,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F24,
	KC_LEFT, KC_DOWN, KC_RGHT, MOD_INS,
           // right thumb keys
           TD(F16_TD), KC_BSPC,
           ALT_BSLASH_MAC,
           CTRL_NUBS, SHIFT_TAB_MAC, CMD_SPACE,
                                    // right palm key
                                    PALM_F15_MAC
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
                                       MOD_LSFT | MOD_LALT,
                    CMD_ESC, KC_LSFT, KC_LCTL,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         CTRL_H,  __________,  KC_K,  __________,  __________,  __________,
         __________,  CTRL_M,  MOD_UP,  CTRL_DOT ,  __________,  __________,
                   MOD_LEFT,  MOD_DOWN,  MOD_RIGHT, __________,
         __________,  M(DEL_LEFT),
         KC_BSLS,
         KC_NUBS, MOD_CTRL, MOD_SPACE,
                                     KC_F15
    ),

[_COMMAND_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  CTRL_COMMA,  __________,
                             M(DEL_LEFT),  __________,
                                        KC_SLSH,
                    MOD_ESC, MOD_ENTER, MOD_DELETE,
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                             KC_LALT,
                             KC_LCTL, KC_LSFT, CMD_SPACE,
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
                                       KC_LALT,
                    KC_LGUI, KC_LSFT, KC_LCTL,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_NUBS,  KC_F1,  KC_F2,
                           KC_F15
    ),

[_ALT_SLASH_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             M(MAIL),  __________,
                                       ALT_SLASH_MAC,
                    KC_LGUI, KC_LSFT, KC_LCTL,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  LCTL(KC_BSPC),
         KC_BSLS,
         LCA(KC_NUBS),  KC_TAB,  KC_SPC,
                           KC_F15
    ),

[_ALT_BSLASH_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             LCTL(KC_BSPC),  __________,
                                       KC_SLSH,
                    KC_ESC, KC_ENTER,  LGUI(KC_BSPC),
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         ALT_BSLASH_MAC,
         KC_LCTL, KC_LSFT, KC_LGUI,
                           __________
    ),

[_CTRL_NUBS] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             KC_BSPC,  __________,
                                       KC_SLSH,
               KC_ESC, KC_ENTER, LGUI(KC_BSPC),
                                        KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         KC_LALT,
         CTRL_NUBS, KC_LSFT, KC_LCTL,

                           __________
    ),

[_CTRL_CMD_BS] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             MOD_LALT | MOD_LSFT,  __________,
                                       KC_LALT,
                    KC_LGUI, KC_LSFT, CTRL_CMD_BS,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         KC_BSLS,
         MEH(KC_NUBS),  KC_TAB,  KC_SPC,
                            KC_F15
    ),

[_SHIFT_ENTER_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       KC_LALT,
                    KC_LGUI, SHIFT_ENTER_MAC, KC_LCTL,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, MEH(KC_INS),
         MEH(KC_F16), KC_BSPC,
         KC_BSLS,
         KC_NUBS, KC_TAB, MOD_SHIFT_SPACE,
         KC_F15
    ),

[_SHIFT_TAB_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   MEH(KC_GRV),  __________,  __________,  __________,
                             KC_BSPC,  MEH(KC_F13),
                                       MEH(KC_SLSH),
                    KC_ESC, KC_ENTER,  __________,
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         KC_LALT,
         KC_LCTL, SHIFT_TAB_MAC, KC_LGUI,
                           __________
    ),

[_PALM_L_MAC] = LAYOUT(
__________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________, __________, __________, __________,
                             __________,  __________,
                                     __________,
                      __________, __________,  __________,
                                     HYPR_F14_MAC,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________, __________,  __________,  __________,  __________,
         __________,  M(PREV_APP),  M(CLOSE_APP),  M(NEXT_APP),  __________,  __________,
         __________,  M(MINIMIZE_MAC),  KC_PGUP,  M(HIDE_OTHERS_MAC),  __________,  __________,
                      KC_HOME,  KC_PGDN, KC_END, __________,
         __________, KC_DEL,
         M(DOCKER_LIST),
         M(DOCKER_LOGS), M(FIND_PREV), M(FIND_NEXT),
                                                KC_F15
    ),

[_PALM_R_MAC] = LAYOUT(
         KC_CLR, HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), HYPR(KC_F4), HYPR(KC_F5), HYPR(KC_F6), HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), HYPR(KC_1), KC__VOLDOWN, KC__MUTE, KC__VOLUP, M(VIM_SAVE_QUIT),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), MEH(KC_F1), MEH(KC_F2), MEH(KC_F3),
                            M(VIM_QUIT), M(GIT_SQUASH),
                                           M(DIR_UP),
                   LGUI(KC_Z), LGUI(LSFT(KC_Z)), M(ITERM_CLEAR),
                                         HYPR(KC_F14),
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                             PALM_F15_MAC
    ),

// base win layer
[_WIN] = LAYOUT(
           // left side
           TD(POWER_TD), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8,
           KC_F17, _1, _2_PLEFT, _3_SLASH, _4_PRGHT, _5_EQL,
           KC_F18, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_F19, KC_A, KC_S, KC_D, KC_F, KC_G,
           KC_F20, KC_Z, KC_X, KC_C, KC_V, KC_B,
           KC_GRV, KC_LBRC, KC_COMM, KC_RBRC,
                                                 // left thumb keys
			                             CTRL_SHIFT_BS, TD(F13_TD),
                                                     ALT_SLASH_WIN,
                           CTRL_ESC, SHIFT_ENTER_WIN, CTRL_ALT_DEL,
                                     // left palm key
			                         HYPR_F14_WIN,
    // right side
    KC_F9, KC_F10, KC_F11, KC_F12, KC_PSCR, KC_SLCK, KC_PAUS, KEYB_CONTROL, KC_CLR,
	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0, KC_F21,
	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_F22,
	KC_H, KC_J, TD(K_TD), KC_L, KC_SCLN, KC_F23,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F24,
	KC_LEFT, KC_DOWN, KC_RGHT, MOD_INS,
           // right thumb keys
           TD(F16_TD), KC_BSPC,
           ALT_BSLASH_WIN,
           KC_RGUI, SHIFT_TAB_WIN, CTRL_SPACE,
                                    // right palm key
                                    PALM_F15_WIN
    ),

// win-specific layers
[_CONTROL_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,   __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  LCA(KC_COMM),  __________,
                             __________,  __________,
                                       KC_LALT,
                    CTRL_ESC, KC_LSFT, KC_LGUI,
                                     __________,

         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
           __________,  __________,  __________,  __________,  __________,  __________,
           __________,  __________,  KC_K,  __________,  __________,  __________,
           __________,  __________,  MOD_UP,  __________ ,  __________,  __________,
                     MOD_LEFT,  MOD_DOWN,  MOD_RIGHT, __________,
         __________,  M(DEL_LEFT),
         KC_BSLS,
         KC_APP, MOD_CTRL, MOD_SPACE,
                                     KC_F15
    ),

[_CONTROL_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                      __________,  __________, __________, __________,
                             M(DEL_LEFT),  __________,
                                        KC_SLSH,
                    MOD_ESC, MOD_ENTER, MOD_DELETE,
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       KC_LALT,
                    KC_LGUI, KC_LSFT, CTRL_SPACE,
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
                                       KC_LALT,
                    KC_LCTL, KC_LSFT, KC_LGUI,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_APP,  KC_F1,  KC_F2,
                          KC_F15
    ),

[_ALT_SLASH_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             M(MAIL),  __________,
                                       ALT_SLASH_WIN,
                    KC_LCTL, KC_LSFT, KC_LGUI,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_APP,  KC_0,  KC_1,
                         KC_F15
    ),

[_ALT_BSLASH_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                               KC_BSPC, __________,
                                       KC_SLSH,
                      KC_F17, KC_ENTER, KC_DEL,
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         ALT_BSLASH_WIN,
         KC_LGUI, KC_LSFT, KC_LCTL,
                           __________
    ),

[_CTRL_ALT_DEL] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             MOD_LCTL | MOD_LSFT,  __________,
                                       KC_LALT,
                    KC_LCTL, KC_LSFT,  CTRL_ALT_DEL,
                                     __________,
         W_F9,  W_F10,  W_F11,  KC_F12,  __________,  __________, __________, __________, __________,
         W_6,  W_7,  W_8,  W_9,  W_0,  __________,
         W_Y,  W_U,  W_I,  W_O,  W_P,  __________,
         W_H,  W_J,  W_K, W_L,  W_QUOT,  __________,
         W_N, W_M, __________, W_DOT, W_SCLN,  __________,
              __________,  __________, __________, __________,
         __________,  __________,
         KC_BSLS,
         KC_APP, KC_0, KC_1,
                      KC_F15
    ),

[_SHIFT_ENTER_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       KC_LALT,
                    KC_LCTL, SHIFT_ENTER_WIN, KC_LGUI,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, MEH(KC_INS),
         MEH(KC_F16), KC_BSPC,
         KC_BSLS,
         KC_APP, KC_TAB, MOD_SHIFT_SPACE,
         KC_F15
    ),


[_SHIFT_TAB_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   MEH(KC_GRV),  __________,  __________,  __________,
                             KC_BSPC,  MEH(KC_F13),
                                       MEH(KC_SLSH),
                    KC_ESC, KC_ENTER,  MOD_DELETE,
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         KC_LALT,
         KC_LGUI, SHIFT_TAB_WIN, KC_LCTL,
                           __________
    ),

[_PALM_L_WIN] = LAYOUT(
__________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________, __________, __________, __________,
                             __________,  __________,
                                     __________,
                      __________, __________,  __________,
                                     HYPR_F14_WIN,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________, __________,  __________,  __________,  __________,
         __________,  M(PREV_APP),  M(CLOSE_APP),  M(NEXT_APP),  __________,  __________,
         __________,  __________,  KC_PGUP,  __________,  __________,  __________,
                      KC_HOME,  KC_PGDN, KC_END, __________,
         __________, KC_DEL,
         M(DOCKER_LIST),
         M(DOCKER_LOGS), M(FIND_PREV), M(FIND_NEXT),
                               KC_F15
    ),

[_PALM_R_WIN] = LAYOUT(
         KC_DEL, HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), HYPR(KC_F4), HYPR(KC_F5), HYPR(KC_F6), HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), HYPR(KC_1), KC_VOLD, KC_MUTE, KC_VOLU, M(VIM_SAVE_QUIT),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), MEH(KC_F1), MEH(KC_F2), MEH(KC_F3),
                                            M(VIM_QUIT), M(GIT_SQUASH),
                                                        M(DIR_UP),
                   LCTL(KC_Z), LCTL(LSFT(KC_Z)), M(CONEMU_CLEAR),
                                         HYPR(KC_F14),
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                             PALM_F15_WIN
    ),

[_KEYB_CONTROL] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  SET_LAYER_MAC, __________, SET_LAYER_WIN, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  __________,
                                       __________,
                    __________, __________, __________,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, KEYB_CONTROL, RESET,
	     __________, __________, __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                         __________
    ),
};

// support for repeat keycodes
void matrix_scan_user(void) {
   if (palm_repeat_code) {
      if (timer_elapsed(palm_repeat_timer) > (25 + first_repeat_delay)) {
         key_code(palm_repeat_code);
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
    case _CTRL_NUBS: down(KC_LCTL); break;
    case _SHIFT_ENTER_MAC: down(KC_LSFT); break;
    case _SHIFT_TAB_MAC: down(KC_LSFT); break;
    case _PALM_L_MAC: down(KC_LGUI); down(KC_LSFT); down(KC_LALT); down(KC_LCTL); break;
    case _PALM_R_MAC: break;
    case _ALT_BSLASH_MAC: down(KC_LALT); break;

    // win-specific
    case _CONTROL_ESCAPE: down(KC_LCTL); break;
    case _CONTROL_SPACE: down(KC_LCTL); break;
    case _CTRL_SHIFT_BS: down(KC_LCTL); down(KC_LSFT); break;
    case _ALT_SLASH_WIN: down(KC_LALT); break;
    case _CTRL_ALT_DEL: down(KC_LCTL); down(KC_LALT); break;
    case _SHIFT_ENTER_WIN: down(KC_LSFT); break;
    case _SHIFT_TAB_WIN: down(KC_LSFT); break;
    case _PALM_L_WIN: down(KC_LGUI); down(KC_LSFT); down(KC_LALT); down(KC_LCTL); break;
    case _PALM_R_WIN: break;
    case _ALT_BSLASH_WIN: down(KC_LALT); break;

    case _KEYB_CONTROL: break;

    // unregister everything (even if it was not pressed - no big deal; this works faster than getting pressed mods)
    default: up(KC_LGUI); up(KC_LCTL); up(KC_LALT); up(KC_LSFT); break;
    }
return state;
}

// support for "mo layer tap" functionality
bool cmd_esc_interrupted = true;
bool shift_enter_mac_interrupted = true;
bool shift_enter_win_interrupted = true;
bool shift_tab_mac_interrupted = true;
bool shift_tab_win_interrupted = true;
bool hypr_f14_mac_interrupted = true;
bool hypr_f14_win_interrupted = true;
bool palm_f15_mac_interrupted = true;
bool palm_f15_win_interrupted = true;
bool alt_slash_mac_interrupted = true;
bool alt_slash_win_interrupted = true;
bool alt_bslash_mac_interrupted = true;
bool alt_bslash_win_interrupted = true;
bool ctrl_cmd_bs_interrupted = true;
bool ctrl_nubs_interrupted = true;
bool ctrl_alt_del_interrupted = true;
bool alt_shift_interrupted = true;
bool ctrl_shift_interrupted = true;
bool ctrl_esc_interrupted = true;
static uint16_t esc_timer; // timer for leader key: esc

// adding logic to custom keycodes and overriding existing ones (taking hold duration into account);
// "mo layer tap" and "esc leader key" functionality
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool is_pressed = record->event.pressed;

    // support for "mo layer tap" functionality
    if (keycode != CMD_ESC) { cmd_esc_interrupted = true; }
    if (keycode != SHIFT_ENTER_MAC) { shift_enter_mac_interrupted = true; }
    if (keycode != SHIFT_ENTER_WIN) { shift_enter_win_interrupted = true; }
    if (keycode != SHIFT_TAB_MAC) { shift_tab_mac_interrupted = true; }
    if (keycode != SHIFT_TAB_WIN) { shift_tab_win_interrupted = true; }
    if (keycode != HYPR_F14_MAC) { hypr_f14_mac_interrupted = true; }
    if (keycode != HYPR_F14_WIN) { hypr_f14_win_interrupted = true; }
    if (keycode != PALM_F15_MAC) { palm_f15_mac_interrupted = true; }
    if (keycode != PALM_F15_WIN) { palm_f15_win_interrupted = true; }
    if (keycode != ALT_SLASH_MAC) { alt_slash_mac_interrupted = true; }
    if (keycode != ALT_SLASH_WIN) { alt_slash_win_interrupted = true; }
    if (keycode != ALT_BSLASH_MAC) { alt_bslash_mac_interrupted = true; }
    if (keycode != ALT_BSLASH_WIN) { alt_bslash_win_interrupted = true; }
    if (keycode != CTRL_CMD_BS) { ctrl_cmd_bs_interrupted = true; }
    if (keycode != CTRL_NUBS) { ctrl_nubs_interrupted = true; }
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

        // mac layers
        case CMD_ESC: {
         static uint16_t cmd_esc_layer_timer;
         if( momentary_layer_tap(KC_ESC, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, &cmd_esc_layer_timer, &cmd_esc_interrupted, is_pressed, 180, true)) {
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

        case CTRL_NUBS: {
          static uint16_t ctrl_nubs_layer_timer;
          momentary_layer_tap(KC_NUBS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &ctrl_nubs_layer_timer, &ctrl_nubs_interrupted, is_pressed, 180, true);
          return true;
        }

        case ALT_SLASH_MAC: {
          static uint16_t alt_slash_mac_layer_timer;
          momentary_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_slash_mac_layer_timer, &alt_slash_mac_interrupted, is_pressed, 180, true);
          return true;
        }

        case ALT_BSLASH_MAC: {
          static uint16_t alt_bslash_mac_layer_timer;
          momentary_layer_tap(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bslash_mac_layer_timer, &alt_bslash_mac_interrupted, is_pressed, 180, true);
          return true;
        }

        case SHIFT_ENTER_MAC: {
          static uint16_t shift_enter_mac_layer_timer;
          momentary_layer_tap(KC_ENTER, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_enter_mac_layer_timer, &shift_enter_mac_interrupted, is_pressed, 300, true);
          return true;
        }

        case SHIFT_TAB_MAC: {
          static uint16_t shift_tab_mac_layer_timer;
          momentary_layer_tap(KC_TAB, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_tab_mac_layer_timer, &shift_tab_mac_interrupted, is_pressed, 300, true);
          return true;
        }

        case HYPR_F14_MAC: {
          static uint16_t hypr_f14_mac_layer_timer;
          momentary_layer_tap(KC_F14, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_LGUI, &hypr_f14_mac_layer_timer, &hypr_f14_mac_interrupted, is_pressed, 300, true);
          return true;
        }

        case PALM_F15_MAC: {
          static uint16_t palm_f15_mac_layer_timer;
          momentary_layer_tap(KC_F15, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &palm_f15_mac_layer_timer, &palm_f15_mac_interrupted, is_pressed, 300, false);
          return true;
        }

        // win layers
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

        case CTRL_ALT_DEL: {
          static uint16_t ctrl_alt_del_layer_timer;
          momentary_layer_tap(KC_DEL, KC_NO, KC_LALT, KC_LCTL, KC_NO, KC_NO, &ctrl_alt_del_layer_timer, &ctrl_alt_del_interrupted, is_pressed, 180, false);
          return true;
        }

        case ALT_SLASH_WIN: {
          static uint16_t alt_slash_win_layer_timer;
          momentary_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_slash_win_layer_timer, &alt_slash_win_interrupted, is_pressed, 180, false);
          return true;
        }

        case SHIFT_ENTER_WIN: {
          static uint16_t shift_enter_win_layer_timer;
          momentary_layer_tap(KC_ENTER, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_enter_win_layer_timer, &shift_enter_win_interrupted, is_pressed, 300, true);
          return true;
        }

        case SHIFT_TAB_WIN: {
          static uint16_t shift_tab_win_layer_timer;
          momentary_layer_tap(KC_TAB, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_tab_win_layer_timer, &shift_tab_win_interrupted, is_pressed, 300, true);
          return true;
        }

        case ALT_BSLASH_WIN: {
          static uint16_t alt_bslash_win_layer_timer;
          momentary_layer_tap(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bslash_win_layer_timer, &alt_bslash_win_interrupted, is_pressed, 180, false);
          return true;
        }

        case HYPR_F14_WIN: {
          static uint16_t hypr_f14_win_layer_timer;
          momentary_layer_tap(KC_F14, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_LGUI, &hypr_f14_win_layer_timer, &hypr_f14_win_interrupted, is_pressed, 300, false);
          return true;
        }

        case PALM_F15_WIN: {
          static uint16_t palm_f15_win_layer_timer;
          momentary_layer_tap(KC_F15, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &palm_f15_win_layer_timer, &palm_f15_win_interrupted, is_pressed, 300, false);
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
        case KC_HOME: { return repeat(KC_HOME, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, is_pressed); }
        case KC_END: { return repeat(KC_END, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, is_pressed); }
        case KC_PGUP: { return repeat(KC_PGUP, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, is_pressed); }
        case KC_PGDN: { return repeat(KC_PGDN, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, is_pressed); }

        // ESCAPE AS LEADER KEY
        // ctrl home/end
        case KC_LEFT: { return following_custom_leader_key(KC_F1, KC_LCTL, KC_LSFT, KC_NO, &esc_timer, is_pressed, 250); }
        case KC_RGHT: { return following_custom_leader_key(KC_F12, KC_LCTL, KC_LSFT, KC_NO, &esc_timer, is_pressed, 250); }

        case MOD_ENTER: { return if_held_179_add_shift(KC_ENTER, is_pressed); }
        case MOD_TAB: { return if_held_179_add_shift(KC_TAB, is_pressed); }
        case MOD_SPACE: { return if_held_179_add_shift(KC_SPC, is_pressed); }
        case MOD_ESC: { return if_held_179_add_shift(os_specific_key(KC_ESC, KC_BSPC), is_pressed); }
        case MOD_CTRL: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, os_specific_key(KC_LGUI, KC_NO), KC_NO, os_specific_key(KC_LCTL, KC_NO), KC_NO, KC_TAB, KC_LSFT, os_specific_key(KC_LCTL, KC_NO), is_pressed, 179); }
        case MOD_SHIFT_SPACE: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_F13, KC_LCTL, KC_LALT, is_pressed, 179); }
        case MOD_INS: { return if_held_180_add_shift(KC_INS, is_pressed); }
        case MOD_DELETE: { return if_held_180_add_shift(KC_DEL, is_pressed); }

        case MOD_LEFT: { return if_held_add_mods(KC_LEFT, os_specific_key(KC_LCTL, KC_LALT), KC_LSFT, is_pressed, 180); }
        case MOD_RIGHT: { return if_held_add_mods(KC_RGHT, os_specific_key(KC_LCTL, KC_LALT), KC_LSFT, is_pressed, 180); }
        case MOD_UP: { return if_held_add_mods(KC_UP, os_specific_key(KC_LCTL, KC_LALT), KC_LSFT, is_pressed, 180); }
        case MOD_DOWN: { return if_held_add_mods(KC_DOWN, os_specific_key(KC_LCTL, KC_LALT), KC_LSFT, is_pressed, 180); }

        case _0: { return if_held_180_replace(KC_0, KC_0, KC_NO, is_pressed); } // disable key repeat
        case _1: { return if_held_180_replace(KC_1, KC_1, KC_NO, is_pressed); } // disable key repeat
        case _2_PLEFT: { return if_held_180_replace(KC_2, KC_9, KC_LSFT, is_pressed); }
        case _3_SLASH: { return if_held_180_replace(KC_3, KC_MINS, KC_LSFT, is_pressed); }
        case _4_PRGHT: { return if_held_180_replace(KC_4, KC_0, KC_LSFT, is_pressed); }
        case _5_EQL: { return if_held_180_replace(KC_5, KC_EQL, KC_NO, is_pressed); }
        case _6_PLUS: { return if_held_180_replace(KC_6, KC_EQL, KC_LSFT, is_pressed); }
        case _7_BANG: { return if_held_180_replace(KC_7, KC_1, KC_LSFT, is_pressed); }
        case _8_DASH: { return if_held_180_replace(KC_8, KC_MINS, KC_NO, is_pressed); }
        case _9_QUEST: { return if_held_180_replace(KC_9, KC_SLSH, KC_LSFT, is_pressed); }

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
        case KC_F17: { return if_held_180_add_shift(KC_F17, is_pressed); }
        case KC_F18: { return if_held_180_add_shift(KC_F18, is_pressed); }
        case KC_F19: { return if_held_180_add_shift(KC_F19, is_pressed); }
        case KC_F20: { return if_held_180_add_shift(KC_F20, is_pressed); }
        case KC_F21: { return if_held_180_add_shift(KC_F21, is_pressed); }
        case KC_F22: { return if_held_180_add_shift(KC_F22, is_pressed); }
        case KC_F23: { return if_held_180_add_shift(KC_F23, is_pressed); }
        case KC_F24: { return if_held_180_add_shift(KC_F24, is_pressed); }

        // mac-only overrides
        case CTRL_COMMA: { return replace_cmd_if_held_add_cmd_shift(KC_COMM, KC_LCTL, is_pressed, 180); }
        case CTRL_DOT: { return replace_cmd_if_held_add_cmd_shift(KC_DOT, KC_LCTL, is_pressed, 180); }
        case CTRL_H: { return replace_cmd_if_held_add_cmd_shift(KC_H, KC_LCTL, is_pressed, 180); }
        case CTRL_M: { return replace_cmd_if_held_add_cmd_shift(KC_M, KC_LCTL, is_pressed, 180); }

        default: {
          return true;
        }
    }
}