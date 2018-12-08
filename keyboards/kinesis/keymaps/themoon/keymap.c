#include "kinesis.h"

#define __________ KC_TRNS
#define _ KC_CLR
#define XXXXXXX KC_NO

#define _MAC 0
#define _WIN 1

// mac layers
#define _COMMAND_ESCAPE 2
#define _COMMAND_SPACE 3
#define _ALT_SHIFT_BS 4
#define _ALT_SLASH_MAC 5
#define _ALT_BSLASH_MAC 6
#define _SHIFT_ENTER_MAC 7
#define _SHIFT_TAB_MAC 8
#define _CTRL_DEL 9
#define _CTRL_NUBS 10
#define _PALM_L_MAC 11
#define _PALM_R_MAC 12

// win layers
#define _CONTROL_ESCAPE 13
#define _CONTROL_SPACE 14
#define _CTRL_SHIFT_BS 15
#define _ALT_SLASH_WIN 16
#define _ALT_BSLASH_WIN 17
#define _SHIFT_ENTER_WIN 18
#define _SHIFT_TAB_WIN 19
#define _CTRL_ALT_DEL 20
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
  SHIFT_ENTER_MAC = MO(_SHIFT_ENTER_MAC),
  SHIFT_TAB_MAC = MO(_SHIFT_TAB_MAC),
  CTRL_DEL = MO(_CTRL_DEL),
  CTRL_NUBS = MO(_CTRL_NUBS),
  HYPR_F14_MAC = MO(_PALM_L_MAC),
  PALM_F15_MAC = MO(_PALM_R_MAC),

  // win
  CTRL_ESC = MO(_CONTROL_ESCAPE),
  CTRL_SPACE = LT(_CONTROL_SPACE, KC_SPC),
  CTRL_SHIFT_BS = MO(_CTRL_SHIFT_BS),
  ALT_SLASH_WIN = MO(_ALT_SLASH_WIN),
  ALT_BSLASH_WIN = MO(_ALT_BSLASH_WIN),
  SHIFT_ENTER_WIN = MO(_SHIFT_ENTER_WIN),
  SHIFT_TAB_WIN = MO(_SHIFT_TAB_WIN),
  CTRL_ALT_DEL = MO(_CTRL_ALT_DEL),
  HYPR_F14_WIN = MO(_PALM_L_WIN),
  PALM_F15_WIN = MO(_PALM_R_WIN),

  // firmware-related stuff
  KEYB_CONTROL = MO(_KEYB_CONTROL)
};

enum holding_keycodes {
  MOD_ENTER = SAFE_RANGE,
  MOD_DELETE,
  MOD_TAB,
  MOD_SPACE,
  MOD_INS,
  MOD_UP_MAC, MOD_DOWN_MAC,
  MOD_UP_WIN, MOD_DOWN_WIN,
  MOD_LEFT_MAC, MOD_RIGHT_MAC,
  MOD_LEFT_WIN, MOD_RIGHT_WIN,
  MOD_SHIFT_SPACE,

  DEL_LEFT_MAC,
  DEL_RIGHT_MAC,
  DEL_LEFT_WIN,
  DEL_RIGHT_WIN,

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

  CMD_TAB,
  CMD_SHIFT_TAB,

  ALT_TAB,
  ALT_SHIFT_TAB,

  CMD_Q,
  ALT_F4,

  HOME_ALT_HOME,
  END_ALT_END,

  CTRL_HOME_ALT_HOME,
  CTRL_END_ALT_END,

  HIDE_FOCUS_MAC,
  HIDE_FOCUS_WIN,

  // mac-specific overrides
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
    wait_ms(50);
    switch (biton32(eeconfig_read_default_layer())) {
      case _MAC: isMac = true; isWin = false; break;
      case _WIN: isWin = true; isMac = false; break;
      default: break;
  }
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
void with_4_mods(uint16_t key, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4) {
  uint8_t old_mods = get_mods();
  down(mod4); down(mod3); down(mod2); down(mod1);
  key_code(key);
  up(mod1); up(mod2); up(mod3); up(mod4);
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
bool replace_key_and_mods_if_held_replace_key_and_mods(
    uint16_t code,
    uint16_t mod1_to_be_replaced,
    uint16_t mod2_to_be_replaced,
    uint16_t mod3_to_be_replaced,
    uint16_t mod4_to_be_replaced,
    uint16_t replacement_mod1,
    uint16_t replacement_mod2,
    uint16_t replacement_mod3,
    uint16_t replacement_mod4,
    uint16_t held_code,
    uint16_t held_mod1,
    uint16_t held_mod2,
    uint16_t held_mod3,
    uint16_t held_mod4,
    bool pressed,
    uint16_t hold_duration,
    bool revertInitialMods
) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      remove_mods(mod1_to_be_replaced, mod2_to_be_replaced, mod3_to_be_replaced, mod4_to_be_replaced);

      if (is_not_held(hold_timer, hold_duration)){
          with_4_mods(code, replacement_mod1, replacement_mod2, replacement_mod3, replacement_mod4);
      } else {
          with_4_mods(held_code, held_mod1, held_mod2, held_mod3, held_mod4);
      }

      if (revertInitialMods) {
        insert_mods(mod4_to_be_replaced, mod3_to_be_replaced, mod2_to_be_replaced, mod1_to_be_replaced);
      }
  }
  return false;
}

// replaces command, if held - adds shift to keycode
bool replace_cmd_if_held_add_cmd_shift(uint16_t code, uint16_t replacement_mod, bool pressed, uint8_t hold_duration) {
  return replace_key_and_mods_if_held_replace_key_and_mods(code, KC_LGUI, KC_NO, KC_NO, KC_NO, replacement_mod, KC_NO, KC_NO, KC_NO, code, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, hold_duration, true);
}

bool replace_ctrl_alt_with_lgui(uint16_t code, bool pressed) {
  return replace_key_and_mods_if_held_replace_key_and_mods(code, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, code, KC_LGUI, KC_NO, KC_NO, KC_NO, pressed, 180, true);
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
bool if_held_add_mods(uint16_t code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint16_t hold_duration) {
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

// adds shift to keycode if it was held for at 175 ms
bool if_held_175_add_shift(uint16_t code, bool pressed) {
  return if_held_add_mods(code, KC_LSFT, KC_NO, pressed, 175);
}

// adds shift to keycode if it was held for at 180 ms
bool if_held_180_add_shift(uint16_t code, bool pressed) {
  return if_held_add_mods(code, KC_LSFT, KC_NO, pressed, 180);
}

// replaces keycode and adds mod to it if it was held for at least provided duration
bool replace_if_held_add_mods(uint16_t code, uint16_t mod, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint16_t hold_duration) {
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
static uint16_t repeat_code;
static uint16_t repeat_mod;
static uint16_t repeat_timer;
static uint16_t first_repeat_delay;
static uint16_t repeat_interval;
bool repeat(
    uint16_t code,
    uint16_t mod,
    uint16_t remove_mod1,
    uint16_t remove_mod2,
    uint16_t remove_mod3,
    uint16_t remove_mod4,
    bool pressed,
    uint16_t interval
) {
   if (pressed) {
     up(remove_mod1); up(remove_mod2); up(remove_mod3); up(remove_mod4);
     with_1_mod(code, mod);

     repeat_code = code;
     repeat_mod = mod;
     repeat_interval = interval;
     repeat_timer = timer_read();
     first_repeat_delay = 250;

   } else {
       repeat_code = 0;
       repeat_mod = 0;
       repeat_interval = 25; // default repeat interval
   }

  return false;
}

// provides functionality similar to MT - except that layer with mod is triggered immediately: this is useful when such mod is used with mouse;
// returns true if tap was triggered and false otherwise
bool momentary_layer_tap(
    uint16_t tap_key,
    uint16_t tap_mod,
    uint16_t layer_mod1,
    uint16_t layer_mod2,
    uint16_t layer_mod3,
    uint16_t layer_mod4,
    uint16_t *layer_timer,
    bool *interrupted_flag,
    bool is_pressed,
    uint16_t hold_duration,
    bool bring_mods_back) {

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

// if tapped, deletes a single word; if held - everything to line start/end
bool delete_word_line(uint16_t code, uint16_t mod_to_remove, uint16_t mod_to_add, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint16_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      up(mod_to_remove);
      if (is_not_held(hold_timer, hold_duration)){
          with_1_mod(code, mod_to_add);
      } else {
          with_2_mods(held_code, held_mod1, held_mod2); key_code(code);
      }
      down(mod_to_remove);
  }
  return false;
}

// quad tap dance
enum {
  K_TD = 1,
  POWER_TD = 2
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
      if (isMac) { down(KC_LALT); key_code(KC_SPC); break; }
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

//**************** POWER TAP *********************//
static tap power_tap_state = { .is_press_action = true, .state = 0 };

void power_finished (qk_tap_dance_state_t *state, void *user_data) {
  power_tap_state.state = cur_dance(state);
  switch (power_tap_state.state) {
    case DOUBLE_TAP: // sleep
      if (isMac) { down(KC_LCTL); down(KC_LSFT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LSFT); up(KC_LCTL); break; }
      if (isWin) { with_1_mod(KC_X, KC_LGUI); _delay_ms(100); key_code(KC_U); _delay_ms(100); down(KC_S); break; }
    case DOUBLE_HOLD: // shutdown
      if (isMac) { down(KC_LGUI); down(KC_LCTL); down(KC_LALT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LALT); up(KC_LCTL); up(KC_LGUI); break; }
      if (isWin) { with_1_mod(KC_X, KC_LGUI); _delay_ms(100); key_code(KC_U); _delay_ms(100); down(KC_U); break; }
    }
  }

void power_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (power_tap_state.state) {
    case DOUBLE_TAP: // sleep
      if (isMac) { up(KC_LCTL); break; }
      if (isWin) { up(KC_S); break; }
    case DOUBLE_HOLD: // shutdown
      if (isMac) { up(KC_LGUI); break; }
      if (isWin) { up(KC_U); break; }
    }
  power_tap_state.state = 0;
}

// all tap macros
qk_tap_dance_action_t tap_dance_actions[] = {
  [K_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, k_finished, k_reset),
  [POWER_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, power_finished, power_reset)
};

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

           case DIR_UP: { if (is_pressed) { SEND_STRING("cd .. && ls"); key_code(KC_ENTER); return false; } }

           case GIT_SQUASH: { if (is_pressed) { SEND_STRING("git reset --soft HEAD~XX && git commit"); return false; } }
           case DOCKER_LOGS: { if (is_pressed) { remove_hypr(); SEND_STRING("docker logs -f --tail=1000 $(docker ps -a -q | head -1)"); key_code(KC_ENTER); add_hypr(); return false; } }
           case DOCKER_LIST: { if (is_pressed) { remove_hypr(); SEND_STRING("docker ps -a"); key_code(KC_ENTER); add_hypr(); return false; } }

           case VIM_SAVE_QUIT: { if (is_pressed) { remove_hypr(); key_code(KC_ESC); _delay_ms(100); SEND_STRING(":wq!"); key_code(KC_ENTER); add_hypr(); return false; } }
           case VIM_QUIT: { if (is_pressed) { remove_hypr(); key_code(KC_ESC); _delay_ms(100); SEND_STRING(":q!"); key_code(KC_ENTER); add_hypr(); return false; } }

           case MAIL: {
             if (is_pressed) {
               if (isMac) { up(KC_LSFT); up(KC_LALT); SEND_STRING("oleksii.danilov@gmail.com"); down(KC_LALT); down(KC_LSFT); }
               if (isWin) { up(KC_LSFT); up(KC_LCTL); SEND_STRING("oleksii.danilov@gmail.com"); down(KC_LCTL); down(KC_LSFT); }
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
* |PowerTap|  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | ____ | ____ | ____ | Prog |Prog->FW|
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* |  F17   |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  |   F21  |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* |  F18   |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  |   F22  |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |  F19   |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  |   F23  |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |  F20   |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  |   F24  |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |  Ins |  [{  |  ,<  |  ]}  |                                         | Left | Down | Right|  `~  |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,---------------------------.
*                            |    Select/Bspc   | F13  |         | F16  |       Bspc         |
*                            `-----------|------|------|         |------+------+-------------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LCMD/|Shift/|------|         |------|Shift/|RCMD/ |
*                                 | ESC  |Enter |Ctrl/ |         |Ctrl/ |Tab   |SPACE |
*                                 |      |      |Del   |         |§     |      |      |
*                                  --------------------           --------------------
*
*                             ----------                                           ----------
*                            | PALM/F14 |                                         | PALM/F15 |
*                             ----------                                           ----------
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
           MOD_INS, KC_LBRC, KC_COMM, KC_RBRC,
                                               // left thumb keys
			                            ALT_SHIFT_BS, KC_F13,
                                                   ALT_SLASH_MAC,
                           CMD_ESC, SHIFT_ENTER_MAC, CTRL_DEL,
                                     // left palm key
			                         HYPR_F14_MAC,
    // right side
  KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KEYB_CONTROL, KC_NO,
	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0, KC_F21,
	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_F22,
	KC_H, KC_J, TD(K_TD), KC_L, KC_SCLN, KC_F23,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F24,
	KC_LEFT, KC_DOWN, KC_RGHT, KC_GRV,
           // right thumb keys
           KC_F16, KC_BSPC,
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
                             _,  __________,
                                       KC_LALT,
                    CMD_ESC, KC_LSFT, KC_LCTL,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         CTRL_H,  __________,  KC_K,  __________,  __________,  __________,
         __________,  CTRL_M,  MOD_UP_MAC,  CTRL_DOT ,  __________,  __________,
                   MOD_LEFT_MAC,  MOD_DOWN_MAC,  MOD_RIGHT_MAC, __________,
         __________,  DEL_LEFT_MAC,
         KC_BSLS,
         KC_NUBS, KC_F2, MOD_SPACE,
                                     KC_F15
    ),

[_COMMAND_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  KC_F3,  __________,
                             DEL_LEFT_MAC,  __________,
                                        KC_SLSH,
                    KC_F1, MOD_ENTER, DEL_RIGHT_MAC,
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  _,
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
         KC_F16,  __________,
         KC_BSLS,
         KC_NUBS,  KC_F11,  KC_F12,
                           KC_F15
    ),

[_ALT_SLASH_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             _,  __________,
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
         KC_NUBS,  KC_F1,  KC_F2,
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
                    KC_ESC, KC_ENTER,  MOD_DELETE,
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  _,
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
               KC_ESC, KC_ENTER, MOD_DELETE,
                                        KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  _,
         KC_LALT,
         CTRL_NUBS, KC_LSFT, KC_LCTL,
                           __________
    ),

[_CTRL_DEL] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             _,  __________,
                                       KC_LALT,
                    KC_LGUI, KC_LSFT, CTRL_DEL,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         KC_BSLS,
         LALT(KC_NUBS),  MOD_TAB,  MOD_SPACE,
                            KC_F15
    ),

[_SHIFT_ENTER_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             _,  __________,
                                       KC_LALT,
                    KC_LGUI, SHIFT_ENTER_MAC, KC_LCTL,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         HYPR(KC_F16), __________,
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
                   MEH(KC_INS),  __________,  __________,  __________,
                             KC_BSPC,  HYPR(KC_F13),
                                       MEH(KC_SLSH),
                    KC_ESC, KC_ENTER,  MEH(KC_DEL),
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  _,
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
         __________,  __________, __________, __________,  __________,  __________,
         __________,  CMD_SHIFT_TAB, CMD_Q, CMD_TAB,  __________,  __________,
         __________,  __________, KC_PGUP, KC_DEL,  __________,  __________,
                      HOME_ALT_HOME, KC_PGDN, END_ALT_END, __________,
         __________, __________,
         M(DOCKER_LIST),
         M(DOCKER_LOGS), M(FIND_PREV), M(FIND_NEXT),
                                                HIDE_FOCUS_MAC
    ),

[_PALM_R_MAC] = LAYOUT(
         KC_DEL, HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), HYPR(KC_F4), HYPR(KC_F5), HYPR(KC_F6), HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), _, KC__VOLDOWN, KC__MUTE, KC__VOLUP, M(VIM_SAVE_QUIT),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_INS), MEH(KC_F1), MEH(KC_F2), MEH(KC_F3),
                            M(VIM_QUIT), M(GIT_SQUASH),
                                           M(DIR_UP),
                   LGUI(KC_Z), LGUI(LSFT(KC_Z)), M(ITERM_CLEAR),
                                         HYPR(KC_F14),
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
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
           MOD_INS, KC_LBRC, KC_COMM, KC_RBRC,
                                                 // left thumb keys
			                             CTRL_SHIFT_BS, KC_F13,
                                                     ALT_SLASH_WIN,
                           CTRL_ESC, SHIFT_ENTER_WIN, CTRL_ALT_DEL,
                                     // left palm key
			                         HYPR_F14_WIN,
    // right side
    KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KEYB_CONTROL, KC_NO,
  	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0, KC_F21,
  	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_F22,
  	KC_H, KC_J, TD(K_TD), KC_L, KC_SCLN, KC_F23,
  	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F24,
  	KC_LEFT, KC_DOWN, KC_RGHT, KC_GRV,
           // right thumb keys
           KC_F16, KC_BSPC,
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
                   __________,  __________,  __________,  __________,
                             _,  __________,
                                       KC_LALT,
                    CTRL_ESC, KC_LSFT, KC_LGUI,
                                     __________,

         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
           __________,  __________,  __________,  __________,  __________,  __________,
           __________,  __________,  KC_K,  __________,  __________,  __________,
           __________,  __________,  MOD_UP_WIN,  __________ ,  __________,  __________,
                     MOD_LEFT_WIN,  MOD_DOWN_WIN,  MOD_RIGHT_WIN, __________,
         __________,  DEL_LEFT_WIN,
         KC_BSLS,
         KC_PAUS, MOD_TAB, MOD_SPACE,
                                     KC_F15
    ),

[_CONTROL_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                      __________,  __________, KC_F3, __________,
                             DEL_LEFT_WIN,  __________,
                                        KC_SLSH,
                    KC_F1, MOD_ENTER, DEL_RIGHT_WIN,
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             __________,  _,
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
         KC_F16,  __________,
         KC_BSLS,
         KC_PAUS,  KC_F11,  KC_F12,
                          KC_F15
    ),

[_ALT_SLASH_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             _,  __________,
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
         KC_PAUS,  KC_F1,  KC_F2,
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
                      KC_PAUS, KC_ENTER, MOD_DELETE,
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  _,
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
                             KC_CLR,  __________,
                                       KC_LALT,
                    KC_LCTL, KC_LSFT,  CTRL_ALT_DEL,
                                     __________,
         W_F9,  W_F10,  W_F11,  KC_F12,  __________,  __________, __________, __________, __________,
         W_6,  W_7,  W_8,  W_9,  W_0,  __________,
         W_Y,  W_U,  W_I,  W_O,  W_P,  __________,
         W_H,  W_J,  W_K, W_L,  W_QUOT,  __________,
         W_N, W_M, __________, W_DOT, W_SCLN,  __________,
              __________,  __________, __________, __________,
         KC_F16,  __________,
         KC_BSLS,
         KC_PAUS, KC_F1, KC_F2,
                      KC_F15
    ),

[_SHIFT_ENTER_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             _,  __________,
                                       KC_LALT,
                    KC_LCTL, SHIFT_ENTER_WIN, KC_LGUI,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	     KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_K,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         HYPR(KC_F16), __________,
         KC_BSLS,
         KC_PAUS, KC_TAB, MOD_SHIFT_SPACE,
         KC_F15
    ),


[_SHIFT_TAB_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   MEH(KC_INS),  __________,  __________,  __________,
                             KC_BSPC,  HYPR(KC_F13),
                                       MEH(KC_SLSH),
                    KC_ESC, KC_ENTER,  MEH(KC_DEL),
                                     KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________,  __________, __________,
         __________,  _,
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
         __________,  __________, __________, __________,  __________,  __________,
         __________,  ALT_SHIFT_TAB, ALT_F4, ALT_TAB,  __________,  __________,
         __________,  __________, KC_PGUP, __________,  __________,  __________,
                      CTRL_HOME_ALT_HOME, KC_PGDN, CTRL_END_ALT_END, __________,
         __________, __________,
         M(DOCKER_LIST),
         M(DOCKER_LOGS), M(FIND_PREV), M(FIND_NEXT),
                               HIDE_FOCUS_WIN
    ),

[_PALM_R_WIN] = LAYOUT(
         KC_DEL, HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), HYPR(KC_F4), HYPR(KC_F5), HYPR(KC_F6), HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), _, KC_VOLD, KC_MUTE, KC_VOLU, M(VIM_SAVE_QUIT),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_INS), MEH(KC_F1), MEH(KC_F2), MEH(KC_F3),
                                            M(VIM_QUIT), M(GIT_SQUASH),
                                                        M(DIR_UP),
                   LCTL(KC_Z), LCTL(LSFT(KC_Z)), M(CONEMU_CLEAR),
                                         HYPR(KC_F14),
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         __________,
         __________,  __________,  __________,
                             PALM_F15_WIN
    ),

[_KEYB_CONTROL] = LAYOUT(
         _,  _,  _,  _,  _,  SET_LAYER_MAC, _, SET_LAYER_WIN, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             _,  _,
                                       _,
                    _, _, _,
                                     _,
         _,  _,  _,  _,  _,  _, _, KEYB_CONTROL, RESET,
	     _, _, _, _, _, _,// скорее все all will be отлично so true
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _ ,  _,  _,
                   _,  _,  _, _,
         _,  _,
         _,
         _,  _,  _,
                         _
    ),
};

// support for repeat keycodes
void matrix_scan_user(void) {
   if (repeat_code) {
      if (timer_elapsed(repeat_timer) > (repeat_interval + first_repeat_delay)) {
         down(repeat_mod); key_code(repeat_code); up(repeat_mod);
         repeat_timer = timer_read();
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
    case _ALT_BSLASH_MAC: down(KC_LALT); break;
    case _SHIFT_ENTER_MAC: down(KC_LSFT); break;
    case _SHIFT_TAB_MAC: down(KC_LSFT); break;
    case _CTRL_DEL: down(KC_LCTL); break;
    case _CTRL_NUBS: down(KC_LCTL); break;
    case _PALM_L_MAC: down(KC_LGUI); down(KC_LSFT); down(KC_LALT); down(KC_LCTL); break;
    case _PALM_R_MAC: break;

    // win-specific
    case _CONTROL_ESCAPE: down(KC_LCTL); break;
    case _CONTROL_SPACE: down(KC_LCTL); break;
    case _CTRL_SHIFT_BS: down(KC_LCTL); down(KC_LSFT); break;
    case _ALT_SLASH_WIN: down(KC_LALT); break;
    case _ALT_BSLASH_WIN: down(KC_LALT); break;
    case _SHIFT_ENTER_WIN: down(KC_LSFT); break;
    case _SHIFT_TAB_WIN: down(KC_LSFT); break;
    case _CTRL_ALT_DEL: down(KC_LCTL); down(KC_LALT); break;
    case _PALM_L_WIN: down(KC_LGUI); down(KC_LSFT); down(KC_LALT); down(KC_LCTL); break;
    case _PALM_R_WIN: break;

    case _KEYB_CONTROL: break;

    // unregister everything (even if it was not pressed - no big deal; this works faster than getting pressed mods)
    default: up(KC_LGUI); up(KC_LCTL); up(KC_LALT); up(KC_LSFT); break;
    }
return state;
}

// support for "mo layer tap" functionality
bool cmd_esc_interrupted = true;
bool alt_shift_interrupted = true;
bool alt_slash_mac_interrupted = true;
bool alt_bslash_mac_interrupted = true;
bool shift_enter_mac_interrupted = true;
bool shift_tab_mac_interrupted = true;
bool ctrl_del_interrupted = true;
bool ctrl_nubs_interrupted = true;
bool hypr_f14_mac_interrupted = true;
bool palm_f15_mac_interrupted = true;

bool ctrl_esc_interrupted = true;
bool ctrl_shift_interrupted = true;
bool alt_slash_win_interrupted = true;
bool alt_bslash_win_interrupted = true;
bool shift_enter_win_interrupted = true;
bool shift_tab_win_interrupted = true;
bool ctrl_alt_del_interrupted = true;
bool hypr_f14_win_interrupted = true;
bool palm_f15_win_interrupted = true;
static uint16_t esc_timer; // timer for leader key: esc

// adding logic to custom keycodes and overriding existing ones (taking hold duration into account);
// "mo layer tap" and "esc leader key" functionality
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool is_pressed = record->event.pressed;

    // support for "mo layer tap" functionality
    if (keycode != CMD_ESC) { cmd_esc_interrupted = true; }
    if (keycode != ALT_SHIFT_BS) { alt_shift_interrupted = true; }
    if (keycode != ALT_SLASH_MAC) { alt_slash_mac_interrupted = true; }
    if (keycode != ALT_BSLASH_MAC) { alt_bslash_mac_interrupted = true; }
    if (keycode != SHIFT_ENTER_MAC) { shift_enter_mac_interrupted = true; }
    if (keycode != SHIFT_TAB_MAC) { shift_tab_mac_interrupted = true; }
    if (keycode != CTRL_DEL) { ctrl_del_interrupted = true; }
    if (keycode != CTRL_NUBS) { ctrl_nubs_interrupted = true; }
    if (keycode != HYPR_F14_MAC) { hypr_f14_mac_interrupted = true; }
    if (keycode != PALM_F15_MAC) { palm_f15_mac_interrupted = true; }

    if (keycode != CTRL_ESC) { ctrl_esc_interrupted = true; }
    if (keycode != CTRL_SHIFT_BS) { ctrl_shift_interrupted = true; }
    if (keycode != ALT_SLASH_WIN) { alt_slash_win_interrupted = true; }
    if (keycode != ALT_BSLASH_WIN) { alt_bslash_win_interrupted = true; }
    if (keycode != SHIFT_ENTER_WIN) { shift_enter_win_interrupted = true; }
    if (keycode != SHIFT_TAB_WIN) { shift_tab_win_interrupted = true; }
    if (keycode != CTRL_ALT_DEL) { ctrl_alt_del_interrupted = true; }
    if (keycode != HYPR_F14_WIN) { hypr_f14_win_interrupted = true; }
    if (keycode != PALM_F15_WIN) { palm_f15_win_interrupted = true; }

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

        case CTRL_DEL: {
          static uint16_t ctrl_del_layer_timer;
          momentary_layer_tap(KC_DEL, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_del_layer_timer, &ctrl_del_interrupted, is_pressed, 180, true);
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

        // Deleting words / lines
        case DEL_LEFT_MAC: { return delete_word_line(KC_BSPC, KC_LGUI, KC_LALT, KC_LEFT, KC_LGUI, KC_LSFT, is_pressed, 180); }
        case DEL_RIGHT_MAC: { return delete_word_line(KC_DEL, KC_LGUI, KC_LALT, KC_RGHT, KC_LGUI, KC_LSFT, is_pressed, 180); }

        case DEL_LEFT_WIN: { return delete_word_line(KC_BSPC, KC_LCTL, KC_LCTL, KC_HOME, KC_LSFT, KC_NO, is_pressed, 180); }
        case DEL_RIGHT_WIN: { return delete_word_line(KC_DEL, KC_LCTL, KC_LCTL, KC_END, KC_LSFT, KC_NO, is_pressed, 180); }

        // REPEATING KEYCODES
        case KC_PGUP: { return repeat(KC_PGUP, KC_NO, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, is_pressed, 25); }
        case KC_PGDN: { return repeat(KC_PGDN, KC_NO, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, is_pressed, 25); }

        // app switch
        case CMD_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LALT, KC_LCTL, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LGUI, KC_NO, KC_NO, KC_NO, is_pressed, 180, true); }
        case CMD_SHIFT_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LGUI, KC_LSFT, KC_NO, KC_NO, is_pressed, 180, true); }

        case ALT_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LGUI, KC_LCTL, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 180, true); }
        case ALT_SHIFT_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LGUI, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LALT, KC_LSFT, KC_NO, KC_NO, is_pressed, 180, true); }

        // home/end
        case HOME_ALT_HOME: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_HOME, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 250, false); }
        case END_ALT_END: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_END, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_END, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 250, false); }

        case CTRL_HOME_ALT_HOME: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_HOME, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 250, false); }
        case CTRL_END_ALT_END: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_END, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 250, false); }

        // ESCAPE AS LEADER KEY
        // cmd/ctrl home/end
        case KC_LEFT: { return following_custom_leader_key(KC_HOME, isMac ? KC_LGUI : KC_NO, KC_NO, KC_NO, &esc_timer, is_pressed, 250); }
        case KC_RGHT: { return following_custom_leader_key(KC_END, isMac ? KC_LGUI : KC_NO, KC_NO, KC_NO, &esc_timer, is_pressed, 250); }

        case MOD_ENTER: { return if_held_175_add_shift(KC_ENTER, is_pressed); }
        case MOD_TAB: { return if_held_175_add_shift(KC_TAB, is_pressed); }
        case MOD_SPACE: { return if_held_175_add_shift(KC_SPC, is_pressed); }
        case MOD_SHIFT_SPACE: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_SPC, KC_LCTL, KC_LALT, KC_NO, KC_NO, is_pressed, 175, true); }
        case MOD_INS: { return if_held_180_add_shift(KC_INS, is_pressed); }
        case MOD_DELETE: { return if_held_180_add_shift(KC_DEL, is_pressed); }

        case MOD_UP_MAC: { return if_held_180_add_shift(KC_UP, is_pressed); }
        case MOD_DOWN_MAC: { return if_held_180_add_shift(KC_DOWN, is_pressed); }

        case MOD_UP_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_UP, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LCTL, KC_LSFT, KC_NO, KC_NO, is_pressed, 180, false); }
        case MOD_DOWN_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOWN, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LCTL, KC_LSFT, KC_NO, KC_NO, is_pressed, 180, false); }

        case MOD_LEFT_MAC: { return if_held_add_mods(KC_LEFT, KC_LSFT, KC_NO, is_pressed, 250); }
        case MOD_RIGHT_MAC: { return if_held_add_mods(KC_RGHT, KC_LSFT, KC_NO, is_pressed, 250); }

        case MOD_LEFT_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_LEFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LSFT, KC_NO, KC_NO, KC_NO, is_pressed, 250, false); }
        case MOD_RIGHT_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_RGHT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LSFT, KC_NO, KC_NO, KC_NO, is_pressed, 250, false); }

        case _1: { return if_held_180_replace(KC_1, KC_1, KC_NO, is_pressed); } // disable key repeat
        case _0: { return if_held_180_replace(KC_0, KC_0, KC_NO, is_pressed); } // disable key repeat
        case _2_PLEFT: { return if_held_180_replace(KC_2, KC_9, KC_LSFT, is_pressed); }
        case _3_SLASH: { return if_held_180_replace(KC_3, KC_MINS, KC_LSFT, is_pressed); }
        case _4_PRGHT: { return if_held_180_replace(KC_4, KC_0, KC_LSFT, is_pressed); }
        case _5_EQL: { return if_held_180_replace(KC_5, KC_EQL, KC_NO, is_pressed); }
        case _6_PLUS: { return if_held_180_replace(KC_6, KC_EQL, KC_LSFT, is_pressed); }
        case _7_BANG: { return if_held_180_replace(KC_7, KC_1, KC_LSFT, is_pressed); }
        case _8_DASH: { return if_held_180_replace(KC_8, KC_MINS, KC_NO, is_pressed); }
        case _9_QUEST: { return if_held_180_replace(KC_9, KC_SLSH, KC_LSFT, is_pressed); }

        // close app
        case CMD_Q: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_Q, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_Q, KC_LGUI, KC_NO, KC_NO, KC_NO, is_pressed, 180, true); }
        case ALT_F4: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F4, KC_LGUI, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_F4, KC_F4, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 180, true); }

        // minimize app / minimize others
        // cmd + H / cmd + opt + H
        case HIDE_FOCUS_MAC: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_H, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_H, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 300, true); }
        // hypr + down / hypr + up
        case HIDE_FOCUS_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOWN, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_NO, KC_NO, is_pressed, 300, true); }

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
        case KC_F15: { return if_held_180_add_shift(KC_F15, is_pressed); }
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
        case CTRL_DOT: { return replace_cmd_if_held_add_cmd_shift(KC_DOT, KC_LCTL, is_pressed, 180); }
        case CTRL_H: { return replace_cmd_if_held_add_cmd_shift(KC_H, KC_LCTL, is_pressed, 180); }
        case CTRL_M: { return replace_cmd_if_held_add_cmd_shift(KC_M, KC_LCTL, is_pressed, 180); }

        default: {
          return true;
        }
    }
}
