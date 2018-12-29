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
#define _CTRL_F1 10
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
  CTRL_F1 = MO(_CTRL_F1),
  HYPR_F13_MAC = MO(_PALM_L_MAC),
  PALM_F14_MAC = MO(_PALM_R_MAC),

  // win
  CTRL_ESC = MO(_CONTROL_ESCAPE),
  CTRL_SPACE = LT(_CONTROL_SPACE, KC_SPC),
  CTRL_SHIFT_BS = MO(_CTRL_SHIFT_BS),
  ALT_SLASH_WIN = MO(_ALT_SLASH_WIN),
  ALT_BSLASH_WIN = MO(_ALT_BSLASH_WIN),
  SHIFT_ENTER_WIN = MO(_SHIFT_ENTER_WIN),
  SHIFT_TAB_WIN = MO(_SHIFT_TAB_WIN),
  CTRL_ALT_DEL = MO(_CTRL_ALT_DEL),
  HYPR_F13_WIN = MO(_PALM_L_WIN),
  PALM_F14_WIN = MO(_PALM_R_WIN),

  // firmware-related stuff
  KEYB_CONTROL = MO(_KEYB_CONTROL)
};

enum holding_keycodes {
  _ENTER = SAFE_RANGE,
  _DEL,
  _TAB,
  _SPACE,
  _SLASH,
  _BSLASH,
  _ESC,
  _INS,
  _NUBS,
  _PAUS,
  _BSPC,
  _CTRL_F2_F3,
  _APP,
  _LBRC,
  _RBRC,
  _COMM,
  _DOT,
  _SCLN,
  _QUOT,
  _GRV,
  _CMD_COMM,
  _CTRL_COMM,
  _GRV_,
  _INS_,
  _EQL_,
  _MINS_,

  _0_,
  _1_,
  _2_PLEFT,
  _3_SLASH,
  _4_PRGHT,
  _5_EQL,
  _6_PLUS,
  _7_BANG,
  _8_DASH,
  _9_QUEST,

  _Q,
  _W,
  _E,
  _R,
  _T,
  _Y,
  _U,
  _I,
  _O,
  _P,
  _A,
  _S,
  _D,
  _F,
  _G,
  _H,
  _J,
  _K,
  _L,
  _Z,
  _X,
  _C,
  _V,
  _B,
  _N,
  _M,

  HOME_,
  END_,

  CTRL_HOME,
  CTRL_END,

  SELECT_UP_MAC, SELECT_DOWN_MAC,
  SELECT_UP_WIN, SELECT_DOWN_WIN,

  SELECT_LEFT_MAC, SELECT_RIGHT_MAC,
  SELECT_LEFT_WIN, SELECT_RIGHT_WIN,

  LEAD_SPACE,

  DEL_LEFT_MAC,
  DEL_RIGHT_MAC,
  DEL_LEFT_WIN,
  DEL_RIGHT_WIN,

  FIND_NEXT,
  FIND_PREV,

  CMD_TAB,
  CMD_SHIFT_TAB,

  ALT_TAB,
  ALT_SHIFT_TAB,

  CMD_Q,
  ALT_F4,

  SLEEP_POWER,

  HIDE_FOCUS_MAC,
  HIDE_FOCUS_WIN,

  SET_LAYER_MAC,
  SET_LAYER_WIN,

  // mac-specific overrides
  CTRL_H,
  CTRL_M,
  CTRL_DOT,

  LANG_CAPS_WIN,
  LANG_CAPS_MAC,

  // LWin + key overrides
  W_F9, W_F10, W_F11, W_F12,
  W_6, W_7, W_8, W_9, W_0,
  W_Y, W_U, W_I, W_O, W_P,
  W_H, W_J, W_K, W_L, W_SCLN,
  W_N, W_M, W_DOT, W_QUOT,
  W_EQL, W_MINS
};

static uint16_t esc_timer; // timer for leader key: esc
static uint16_t lead_timer; // timer for leader key
static bool default_layer = true;

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

bool pressed_within(uint16_t hold_timer, uint16_t hold_duration) {
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
      up(mod1_to_be_replaced); up(mod2_to_be_replaced); up(mod3_to_be_replaced); up(mod4_to_be_replaced);

      if (pressed_within(hold_timer, hold_duration)){
          with_4_mods(code, replacement_mod1, replacement_mod2, replacement_mod3, replacement_mod4);
      } else {
          with_4_mods(held_code, held_mod1, held_mod2, held_mod3, held_mod4);
      }

      if (revertInitialMods) {
         down(mod4_to_be_replaced); down(mod3_to_be_replaced); down(mod2_to_be_replaced); down(mod1_to_be_replaced);
      }
  }
  return false;
}

// replaces command, if held - adds shift to keycode
bool replace_cmd_if_held_add_cmd_shift(uint16_t code, uint16_t replacement_mod, bool pressed, uint8_t hold_duration) {
  return replace_key_and_mods_if_held_replace_key_and_mods(code, KC_LGUI, KC_NO, KC_NO, KC_NO, replacement_mod, KC_NO, KC_NO, KC_NO, code, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, hold_duration, true);
}

bool replace_ctrl_alt_with_lgui(uint16_t code, bool pressed) {
  return replace_key_and_mods_if_held_replace_key_and_mods(code, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, code, KC_LGUI, KC_NO, KC_NO, KC_NO, pressed, 170, true);
}

// replaces keycode if it was held for at least provided duration
bool if_held_replace(uint16_t code, uint16_t held_code, bool pressed, uint8_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (pressed_within(hold_timer, hold_duration)){
          key_code(code);
      } else {
          key_code(held_code);
      }
  }
  return false;
}

// replaces keycode and adds mod to it if it was held for at least provided duration
bool replace_if_held_add_mods(uint16_t code, uint16_t mod, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint16_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (pressed_within(hold_timer, hold_duration)){
          with_1_mod(code, mod);
      } else {
          with_2_mods(held_code, held_mod1, held_mod2);
      }
  }
  return false;
}

// add mod to keycode if it was held for at least provided duration
bool if_held_add_mods(uint16_t code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint16_t hold_duration) {
  static uint16_t hold_timer;
  static uint8_t pressed_mods;
  if(pressed) {
      hold_timer= timer_read();
      pressed_mods = get_mods();
  } else {
      if (pressed_mods) { register_mods(pressed_mods); }
      if (pressed_within(hold_timer, hold_duration)){
          key_code(code);
      } else {
          with_2_mods(code, held_mod1, held_mod2);
      }
      pressed_mods = 0;
      if (default_layer) {
         up(KC_LGUI); up(KC_LCTL); up(KC_LALT); up(KC_LSFT);
      }
  }
  return false;
}

// adds shift to keycode if it was held for at 170 ms
bool if_held_170_add_shift(uint16_t code, bool pressed) {
  return if_held_add_mods(code, KC_LSFT, KC_NO, pressed, 170);
}

bool sleep_power(bool pressed, uint16_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (pressed_within(hold_timer, hold_duration)){
          if (isMac) { down(KC_LCTL); down(KC_LSFT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LSFT); up(KC_LCTL); }
          if (isWin) { with_1_mod(KC_X, KC_LGUI); _delay_ms(300); key_code(KC_U); _delay_ms(300); down(KC_S); up(KC_S); }
      } else {
         if (isMac) { down(KC_LGUI); down(KC_LCTL); down(KC_LALT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LALT); up(KC_LCTL); up(KC_LGUI); up(KC_LGUI); }
          if (isWin) { with_1_mod(KC_X, KC_LGUI); _delay_ms(300); key_code(KC_U); _delay_ms(300); down(KC_U); up(KC_U); }
      }
  }
  return false;
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
    if (pressed_within(*layer_timer, hold_duration)) {
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
      if (pressed_within(hold_timer, hold_duration)){
          with_1_mod(code, mod_to_add);
      } else {
          with_2_mods(held_code, held_mod1, held_mod2); key_code(code);
      }
      down(mod_to_remove);
  }
  return false;
}

// provides functionality similar to "leader key", except that it works for escape
bool following_custom_leader_key(uint16_t code, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t *leader_timer, bool is_pressed, uint16_t leader_last_pressed_timeout) {
  if (*leader_timer && pressed_within(*leader_timer, leader_last_pressed_timeout)) {
    if (is_pressed) {
      *leader_timer = 0;
      with_3_mods(code, mod1, mod2, mod3);
      return false;
    }
  }
  return true;
}

// custom leader key: cmd/ctrl+space
bool lead(bool pressed) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (pressed_within(hold_timer, 170)){
          up(KC_LGUI); up(KC_LCTL);
          lead_timer = timer_read();
      } else {
          with_1_mod(KC_SPC, KC_LSFT);
          lead_timer = 0;
      }
  }
  return false;
}

bool is_lead_additional_mod_impl(uint16_t code, uint16_t os_mod, uint16_t additional_mod, bool is_pressed) {
  if (lead_timer) {
    if (pressed_within(lead_timer, 1000)) {
      if (is_pressed) {
        lead_timer = 0;
        with_3_mods(code, os_mod, KC_LSFT, additional_mod);
        return true;
      }
    }
  }
  return false;
}

bool is_lead(uint16_t code, bool is_pressed) {
  return is_lead_additional_mod_impl(code, isMac ? KC_LGUI : KC_LCTL, KC_LALT, is_pressed);
}

bool after_lead(uint16_t code, bool pressed) {
  if (is_lead_additional_mod_impl(code, isMac ? KC_LGUI : KC_LCTL, KC_LALT, pressed)) {
     return false;
  };
  return true;
}

bool after_lead_replace_if_held_add_mods(uint16_t code, uint16_t mod, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool *was_lead, bool pressed, uint16_t hold_duration) {
  static uint16_t hold_timer;
  static uint8_t pressed_mods;
  if(pressed) {
      pressed_mods = get_mods();
      hold_timer= timer_read();
  } else {
      if (*was_lead) {
        *was_lead = false;
        return false;
      }
      if (pressed_mods) { register_mods(pressed_mods); }
      if (pressed_within(hold_timer, hold_duration)){
          with_1_mod(code, mod);
      } else {
          with_2_mods(held_code, held_mod1, held_mod2);
      }
      *was_lead = false;
      pressed_mods = 0;
      if (default_layer) {
         up(KC_LGUI); up(KC_LCTL); up(KC_LALT); up(KC_LSFT);
      }
  }
  return false;
}

bool after_lead_replace_lead_code(uint16_t lead_code, uint16_t lead_additional_mod, uint16_t code, uint16_t mod, uint16_t held_code, uint16_t held_mod, bool pressed, uint16_t hold_duration) {
  static bool was_lead;
  if (is_lead_additional_mod_impl(lead_code, isMac ? KC_LGUI: KC_LCTL, lead_additional_mod, pressed)) {
     was_lead = true;
     return false;
  };
  return after_lead_replace_if_held_add_mods(code, mod, held_code, held_mod, KC_NO, &was_lead, pressed, 170);
}

bool after_lead_replace(uint16_t code, uint16_t held_code, uint16_t held_mod, bool pressed) {
  return after_lead_replace_lead_code(code, KC_LALT, code, KC_NO, held_code, held_mod, pressed, 170);
}


bool after_lead_f_key(uint16_t code, bool pressed) {
  static bool was_lead;
  if (is_lead(code, pressed)) {
     was_lead = true;
     return false;
  }
  return after_lead_replace_if_held_add_mods(code, KC_NO, code, KC_LSFT, KC_NO, &was_lead, pressed, 170);
}

/*
* ,-------------------------------------------------------------------------------------------------------------------.
* |  Cmd,  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 |  _  |  _  |  _  | Prog | Sleep/Off |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* |  `~    |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  |   =+   |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* |  Ins   |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  |   -_   |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |  _`~   |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |  K   |   L  |  ;:  |   _=+  |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |  _Ins  |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  |   _-_  |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          | Nubs |  [{  |  ,<  |  ]}  |                                         | Left | Down | Right| Menu |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,---------------------------.
*                            |    Select/Bspc   | DYN1 |         | DYN2 |       Bspc         |
*                            `-----------|------|------|         |------+------+-------------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LCMD/|Shift/|------|         |------|Shift/|RCMD/ |
*                                 | ESC  |Enter |Ctrl/ |         |Ctrl/ |Tab   |SPACE |
*                                 |      |      |Del   |         |§     |      |      |
*                                  --------------------           --------------------
*
*                             ----------                                           ----------
*                            | PALM/F13 |                                         | PALM/F14 |
*                             ----------                                           ----------
*/

// base mac layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_MAC] = LAYOUT(
           // left side
           _CMD_COMM, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8,
           _GRV, _1_, _2_PLEFT, _3_SLASH, _4_PRGHT, _5_EQL,
           _INS, KC_Q, KC_W, KC_E, KC_R, KC_T,
           _GRV_,KC_A, KC_S, KC_D, KC_F, KC_G,
           _INS_, KC_Z, KC_X, KC_C, KC_V, KC_B,
                 _NUBS, _LBRC, _COMM, _RBRC,
                                               // left thumb keys
			                            ALT_SHIFT_BS, KC_F15,
                                                   ALT_SLASH_MAC,
                           CMD_ESC, SHIFT_ENTER_MAC, CTRL_DEL,
                                     // left palm key
			                         HYPR_F13_MAC,
    // right side
  KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KEYB_CONTROL, SLEEP_POWER,
	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0_, KC_EQL,
	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS,
	KC_H, KC_J, KC_K, KC_L, _SCLN, _EQL_,
	KC_N, KC_M, KC_UP, _DOT, _QUOT, _MINS_,
	KC_LEFT, KC_DOWN, KC_RGHT, _CTRL_F2_F3,
           // right thumb keys
           KC_F16, _BSPC,
           ALT_BSLASH_MAC,
           CTRL_F1, SHIFT_TAB_MAC, CMD_SPACE,
                                    // right palm key
                                    PALM_F14_MAC
    ),

// mac-specific layers
[_COMMAND_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
               __________,  __________,  __________,  __________,
                             _,  __________,
                                       KC_LALT,
                    CMD_ESC, KC_LSFT, KC_LCTL,
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         _Y, _U ,_I, _O, _P, __________,
         CTRL_H,   _J,  _K,  _L,  __________,  __________,
         _N,  CTRL_M,  SELECT_UP_MAC,  CTRL_DOT,  __________,  __________,
                   SELECT_LEFT_MAC,  SELECT_DOWN_MAC,  SELECT_RIGHT_MAC, __________,
         __________,  DEL_LEFT_MAC,
         _BSLASH,
         DEL_RIGHT_MAC, LANG_CAPS_MAC, LEAD_SPACE,
                                     KC_F14
    ),

[_COMMAND_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
               __________,  __________,  KC_F4,  __________,
                             DEL_LEFT_MAC,  __________,
                                        _SLASH,
                    KC_F3, _ENTER, DEL_RIGHT_MAC,
                                     KC_F13,
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             _,  _,
                             KC_LALT,
                             KC_LCTL, KC_LSFT, CMD_SPACE,
                                     _
    ),

[_ALT_SHIFT_BS] = LAYOUT(
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             ALT_SHIFT_BS,  _,
                                            _,
                                      _, _, _,
                                     _,
         _,  _,  _,  _,  _,  _, _, _, _,
	       _, _, _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  __________,  _ ,  _,  _,
         __________, __________, __________, _,
         _,  _,
         _,
         _,  _,  _,
                           _
    ),

[_ALT_SLASH_MAC] = LAYOUT(
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             _,  _,
                                 ALT_SLASH_MAC,
                    KC_LGUI, KC_LSFT, KC_LCTL,
                                     _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         _Y, _U ,_I, _O, _P, __________,
         _H, _J, _K, _L,  __________,  __________,
         _N,  _M,  __________,  __________,  __________,  __________,
               __________,  __________,  __________,  __________,
         __________,  LCTL(KC_BSPC),
         _BSLASH,
         LCTL(KC_DEL),  _TAB,  KC_F1,
                           KC_F14
    ),

[_ALT_BSLASH_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
               __________,  __________,  __________,  __________,
                             LCTL(KC_BSPC),  __________,
                                       _SLASH,
                    _ESC, _ENTER,  LCTL(KC_DEL),
                                     KC_F13,
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _ ,  _,  _,
                   _,  _,  _, _,
         _,  _,
         ALT_BSLASH_MAC,
         KC_LCTL, KC_LSFT, KC_LGUI,
                           _
    ),

[_CTRL_F1] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
                  __________, __________,  __________, __________,
                             _BSPC,  __________,
                                       _SLASH,
               _ESC, _ENTER, _DEL,
                                        KC_F13,
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _ ,  _,  _,
                   _,  _,  _, _,
         _,  _,
         KC_LALT,
         CTRL_F1, KC_LSFT, KC_LCTL,
                           _
    ),

[_CTRL_DEL] = LAYOUT(
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             _,  _,
                                       KC_LALT,
                    KC_LGUI, KC_LSFT, CTRL_DEL,
                                     _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         _Y, _U ,_I, _O, _P, __________,
         _H, _J, _K, _L,  __________,  __________,
         _N, _M, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
         __________,  _BSPC,
         _BSLASH,
         _DEL,  KC_F2,  _SPACE,
                            KC_F14
    ),

[_SHIFT_ENTER_MAC] = LAYOUT(
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             _,  _,
                                       KC_LALT,
                    KC_LGUI, SHIFT_ENTER_MAC, KC_LCTL,
                                     _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	       KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         _Y, _U ,_I, _O, _P, __________,
         _H, _J, _K, _L,  __________,  __________,
         _N, _M, __________, __________, __________, __________,
                   __________,  __________,  __________, __________,
         MEH(KC_F16), _BSPC,
         _BSLASH,
         _DEL, _TAB, _SPACE,
         KC_F14
    ),

[_SHIFT_TAB_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
                   __________,  __________,  __________,  __________,
                             _BSPC,  MEH(KC_F15),
                                       _BSLASH,
                    _ESC, _ENTER,  _DEL,
                                     KC_F13,
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _ ,  _,  _,
                   _,  _,  _, _,
         _,  _,
         KC_LALT,
         KC_LCTL, SHIFT_TAB_MAC, KC_LGUI,
                           _
    ),

[_PALM_L_MAC] = LAYOUT(
__________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
                   __________, __________, __________, __________,
                             __________,  __________,
                                     __________,
                      __________, __________,  __________,
                                     HYPR_F13_MAC,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         _Y, _U ,_I, _O, _P, __________,
         _H,  CMD_SHIFT_TAB, CMD_Q, CMD_TAB,  __________,  __________,
         _N,  _M, KC_PGUP, KC_SPC,  __________,  __________,
           HOME_, KC_PGDN, END_, __________,
         __________, KC_LEFT,
         KC_BSLS,
         KC_RGHT, FIND_PREV, FIND_NEXT,
                                  HIDE_FOCUS_MAC
    ),

[_PALM_R_MAC] = LAYOUT(
         HYPR(KC_COMM), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), HYPR(KC_F4), HYPR(KC_F5), HYPR(KC_F6), HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_GRV), HYPR(KC_1), KC__VOLDOWN, KC__MUTE, KC__VOLUP, HYPR(KC_5),
         LCA(KC_INS), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         LCA(KC_F13), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         LCA(KC_F14), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_NUBS), KC_MRWD, KC_MPLY, KC_MFFD,
                            HYPR(KC_BSPC), HYPR(KC_F15),
                                           HYPR(KC_SLSH),
                   LGUI(KC_Z), LGUI(LSFT(KC_Z)), HYPR(KC_DEL),
                                         HYPR(KC_F13),
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _, _, _,
         _,  _,
         _,
         _,  _,  _,
                             PALM_F14_MAC
    ),

// base win layer
[_WIN] = LAYOUT(
           // left side
           _CTRL_COMM, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8,
           _GRV, _1_, _2_PLEFT, _3_SLASH, _4_PRGHT, _5_EQL,
           _INS, KC_Q, KC_W, KC_E, KC_R, KC_T,
           _GRV_, KC_A, KC_S, KC_D, KC_F, KC_G,
           _INS_, KC_Z, KC_X, KC_C, KC_V, KC_B,
                _PAUS, _LBRC, _COMM, _RBRC,
                                                 // left thumb keys
			                             CTRL_SHIFT_BS, KC_F15,
                                                     ALT_SLASH_WIN,
                           CTRL_ESC, SHIFT_ENTER_WIN, CTRL_ALT_DEL,
                                     // left palm key
			                         HYPR_F13_WIN,
    // right side
    KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KEYB_CONTROL, SLEEP_POWER,
  	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0_, KC_EQL,
  	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS,
  	KC_H, KC_J, KC_K, KC_L, _SCLN, _EQL_,
  	KC_N, KC_M, KC_UP, _DOT, _QUOT, _MINS_,
  	KC_LEFT, KC_DOWN, KC_RGHT, _APP,
           // right thumb keys
           KC_F16, _BSPC,
           ALT_BSLASH_WIN,
           KC_RGUI, SHIFT_TAB_WIN, CTRL_SPACE,
                                    // right palm key
                                    PALM_F14_WIN
    ),

// win-specific layers
[_CONTROL_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
                  __________, __________,  __________, __________,
                                         _,  _,
                                       KC_LALT,
                    CTRL_ESC, KC_LSFT, KC_LGUI,
                                     __________,

         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         _Y, _U ,_I, _O, _P, __________,
         _H, _J, _K, _L,  __________,  __________,
         _N,  _M,  SELECT_UP_WIN,  __________,  __________,  __________,
         SELECT_LEFT_WIN,  SELECT_DOWN_WIN,  SELECT_RIGHT_WIN, __________,
         __________,  DEL_LEFT_WIN,
         _BSLASH,
         DEL_RIGHT_WIN, LANG_CAPS_WIN, LEAD_SPACE,
                                     KC_F14
    ),

[_CONTROL_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
                  __________, __________,  KC_F4, __________,
                              DEL_LEFT_WIN,  __________,
                                        _SLASH,
                    KC_F3, _ENTER, DEL_RIGHT_WIN,
                                     KC_F13,
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             _,  _,
                                       KC_LALT,
                    KC_LGUI, KC_LSFT, CTRL_SPACE,
                                     _
    ),

[_CTRL_SHIFT_BS] = LAYOUT(
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             CTRL_SHIFT_BS,  _,
                                             _,
                                       _, _, _,
                                     _,
         _,  _,  _,  _,  _,  _, _, _, _,
	       _, _, _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  __________,  _ ,  _,  _,
         __________, __________, __________, _,
         _,  _,
         _,
         _,  _,  _,
                           _
    ),

[_ALT_SLASH_WIN] = LAYOUT(
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             _,  _,
                                  ALT_SLASH_WIN,
                    KC_LCTL, KC_LSFT, KC_LGUI,
                                     _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         _Y, _U ,_I, _O, _P, __________,
         _H, _J, _K, _L,  __________,  __________,
         _N,  _M,  __________,  __________,  __________,  __________,
                  __________, __________,  __________, __________,
         __________,  _BSPC,
         _BSLASH,
         _DEL,  KC_F2,  KC_F1,
                         KC_F14
    ),

[_ALT_BSLASH_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
                  __________, __________,  __________, __________,
                                   _BSPC, __________,
                                       _SLASH,
                      _ESC, _ENTER, _DEL,
                                     KC_F13,
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _ ,  _,  _,
                   _,  _,  _, _,
         _,  _,
         ALT_BSLASH_WIN,
         KC_LGUI, KC_LSFT, KC_LCTL,
                           _
    ),

[_CTRL_ALT_DEL] = LAYOUT(
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             _,  _,
                                       KC_LALT,
                    KC_LCTL, KC_LSFT,  CTRL_ALT_DEL,
                                     __________,
         W_F9,  W_F10,  W_F11,  W_F12,  __________,  __________, __________, __________, __________,
         W_6,  W_7,  W_8,  W_9,  W_0,  W_EQL,
         W_Y,  W_U,  W_I,  W_O,  W_P,  W_MINS,
         W_H,  W_J,  W_K, W_L,  W_QUOT,  __________,
         W_N, W_M, KC_UP, W_DOT, W_SCLN,  __________,
         KC_LEFT,  KC_DOWN, KC_RGHT, __________,
         __________, KC_BSPC,
         _BSLASH,
         KC_DEL, KC_F2, KC_F1,
                      KC_F14
    ),

[_SHIFT_ENTER_WIN] = LAYOUT(
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _,  _,  _,
                             _,  _,
                                       KC_LALT,
                    KC_LCTL, SHIFT_ENTER_WIN, KC_LGUI,
                                     _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	       KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         _Y, _U ,_I, _O, _P, __________,
         _H, _J, _K, _L,  __________,  __________,
         _N,  _M,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________, __________,
         MEH(KC_F16), _BSPC,
         _BSLASH,
         _DEL, _TAB, KC_SPACE,
         KC_F14
    ),


[_SHIFT_TAB_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
                   __________,  __________,  __________,  __________,
                             _BSPC,  MEH(KC_F15),
                                         _BSLASH,
                           _ESC, _ENTER,  _DEL,
                                     KC_F13,
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _ ,  _,  _,
                   _,  _,  _, _,
         _,  _,
         KC_LALT,
         KC_LGUI, SHIFT_TAB_WIN, KC_LCTL,
                           _
    ),

[_PALM_L_WIN] = LAYOUT(
__________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, _Q, _W, _E, _R, _T,
         __________, _A, _S, _D, _F, _G,
         __________, _Z, _X, _C, _V, _B,
                   __________, __________, __________, __________,
                             __________,  __________,
                                     __________,
                      __________, __________,  __________,
                                     HYPR_F13_WIN,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         _Y, _U ,_I, _O, _P, __________,
         _H, ALT_SHIFT_TAB, ALT_F4, ALT_TAB,  __________,  __________,
         _N, _M, KC_PGUP, __________,  __________,  __________,
                      CTRL_HOME, KC_PGDN, CTRL_END, __________,
         __________, KC_LEFT,
         KC_BSLS,
         KC_RGHT, FIND_PREV, FIND_NEXT,
                               HIDE_FOCUS_WIN
    ),

[_PALM_R_WIN] = LAYOUT(
         HYPR(KC_COMM), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), HYPR(KC_F4), HYPR(KC_F5), HYPR(KC_F6), HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_GRV), HYPR(KC_1), KC_VOLD, KC_MUTE, KC_VOLU, HYPR(KC_5),
         HYPR(KC_INS), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F13), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F14), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_PAUS), KC_MPRV, KC_MPLY, KC_MNXT,
                            HYPR(KC_BSPC), HYPR(KC_F15),
                                           HYPR(KC_SLSH),
                   LCTL(KC_Z), LCTL(LSFT(KC_Z)), HYPR(KC_DEL),
                                         HYPR(KC_F13),
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   _,  _, _, _,
         _,  _,
         _,
         _,  _,  _,
                             PALM_F14_WIN
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
	     _, _, _, _, _, _,
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
    default_layer = false;
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
    case _CTRL_F1: down(KC_LCTL); break;
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
    default: up(KC_LGUI); up(KC_LCTL); up(KC_LALT); up(KC_LSFT); default_layer = true; break;
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
bool ctrl_f1_interrupted = true;
bool hypr_f13_mac_interrupted = true;
bool palm_f14_mac_interrupted = true;

bool ctrl_esc_interrupted = true;
bool ctrl_shift_interrupted = true;
bool alt_slash_win_interrupted = true;
bool alt_bslash_win_interrupted = true;
bool shift_enter_win_interrupted = true;
bool shift_tab_win_interrupted = true;
bool ctrl_alt_del_interrupted = true;
bool hypr_f13_win_interrupted = true;
bool palm_f14_win_interrupted = true;

// adding logic to custom keycodes and overriding existing ones (taking hold duration into account);
// "mo layer tap" and "esc leader key" functionality
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool is_pressed = record->event.pressed;

    if (default_layer) {
       // remove stuck modifiers
       up(KC_LGUI); up(KC_LCTL); up(KC_LALT); up(KC_LSFT);
    }

    if (keycode != KC_LEFT && keycode != KC_RGHT) {
       esc_timer = 0;
    }

    // support for "mo layer tap" functionality
    if (keycode != CMD_ESC) { cmd_esc_interrupted = true; }
    if (keycode != ALT_SHIFT_BS) { alt_shift_interrupted = true; }
    if (keycode != ALT_SLASH_MAC) { alt_slash_mac_interrupted = true; }
    if (keycode != ALT_BSLASH_MAC) { alt_bslash_mac_interrupted = true; }
    if (keycode != SHIFT_ENTER_MAC) { shift_enter_mac_interrupted = true; }
    if (keycode != SHIFT_TAB_MAC) { shift_tab_mac_interrupted = true; }
    if (keycode != CTRL_DEL) { ctrl_del_interrupted = true; }
    if (keycode != CTRL_F1) { ctrl_f1_interrupted = true; }
    if (keycode != HYPR_F13_MAC) { hypr_f13_mac_interrupted = true; }
    if (keycode != PALM_F14_MAC) { palm_f14_mac_interrupted = true; }

    if (keycode != CTRL_ESC) { ctrl_esc_interrupted = true; }
    if (keycode != CTRL_SHIFT_BS) { ctrl_shift_interrupted = true; }
    if (keycode != ALT_SLASH_WIN) { alt_slash_win_interrupted = true; }
    if (keycode != ALT_BSLASH_WIN) { alt_bslash_win_interrupted = true; }
    if (keycode != SHIFT_ENTER_WIN) { shift_enter_win_interrupted = true; }
    if (keycode != SHIFT_TAB_WIN) { shift_tab_win_interrupted = true; }
    if (keycode != CTRL_ALT_DEL) { ctrl_alt_del_interrupted = true; }
    if (keycode != HYPR_F13_WIN) { hypr_f13_win_interrupted = true; }
    if (keycode != PALM_F14_WIN) { palm_f14_win_interrupted = true; }

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

        case LEAD_SPACE: {
          return lead(is_pressed);
        }

        case KC_Q: { return after_lead(KC_Q, is_pressed); }
        case KC_W: { return after_lead(KC_W, is_pressed); }
        case KC_E: { return after_lead(KC_E, is_pressed); }
        case KC_R: { return after_lead(KC_R, is_pressed); }
        case KC_T: { return after_lead(KC_T, is_pressed); }
        case KC_Y: { return after_lead(KC_Y, is_pressed); }
        case KC_U: { return after_lead(KC_U, is_pressed); }
        case KC_I: { return after_lead(KC_I, is_pressed); }
        case KC_O: { return after_lead(KC_O, is_pressed); }
        case KC_P: { return after_lead(KC_P, is_pressed); }
        case KC_A: { return after_lead(KC_A, is_pressed); }
        case KC_S: { return after_lead(KC_S, is_pressed); }
        case KC_D: { return after_lead(KC_D, is_pressed); }
        case KC_F: { return after_lead(KC_F, is_pressed); }
        case KC_G: { return after_lead(KC_G, is_pressed); }
        case KC_H: { return after_lead(isWin? KC_H: KC_F16, is_pressed); }
        case KC_J: { return after_lead(KC_J, is_pressed); }
        case KC_K: { return after_lead(KC_K, is_pressed); }
        case KC_L: { return after_lead(KC_L, is_pressed); }
        case KC_Z: { return after_lead(KC_Z, is_pressed); }
        case KC_X: { return after_lead(KC_X, is_pressed); }
        case KC_C: { return after_lead(KC_C, is_pressed); }
        case KC_V: { return after_lead(KC_V, is_pressed); }
        case KC_B: { return after_lead(KC_B, is_pressed); }
        case KC_N: { return after_lead(KC_N, is_pressed); }
        case KC_M: { return after_lead(KC_M, is_pressed); }

        case _Q: { return if_held_170_add_shift(KC_Q, is_pressed); }
        case _W: { return if_held_170_add_shift(KC_W, is_pressed); }
        case _E: { return if_held_170_add_shift(KC_E, is_pressed); }
        case _R: { return if_held_170_add_shift(KC_R, is_pressed); }
        case _T: { return if_held_170_add_shift(KC_T, is_pressed); }
        case _Y: { return if_held_170_add_shift(KC_Y, is_pressed); }
        case _U: { return if_held_170_add_shift(KC_U, is_pressed); }
        case _I: { return if_held_170_add_shift(KC_I, is_pressed); }
        case _O: { return if_held_170_add_shift(KC_O, is_pressed); }
        case _P: { return if_held_170_add_shift(KC_P, is_pressed); }
        case _A: { return if_held_170_add_shift(KC_A, is_pressed); }
        case _S: { return if_held_170_add_shift(KC_S, is_pressed); }
        case _D: { return if_held_170_add_shift(KC_D, is_pressed); }
        case _F: { return if_held_170_add_shift(KC_F, is_pressed); }
        case _G: { return if_held_170_add_shift(KC_G, is_pressed); }
        case _H: { return if_held_170_add_shift(KC_H, is_pressed); }
        case _J: { return if_held_170_add_shift(KC_J, is_pressed); }
        case _K: { return if_held_170_add_shift(KC_K, is_pressed); }
        case _L: { return if_held_170_add_shift(KC_L, is_pressed); }
        case _Z: { return if_held_add_mods(KC_Z, KC_LSFT, KC_NO, is_pressed, 250); }
        case _X: { return if_held_add_mods(KC_X, KC_LSFT, KC_NO, is_pressed, 250); }
        case _C: { return if_held_add_mods(KC_C, KC_LSFT, KC_NO, is_pressed, 250); }
        case _V: { return if_held_170_add_shift(KC_V, is_pressed); }
        case _B: { return if_held_170_add_shift(KC_B, is_pressed); }
        case _N: { return if_held_170_add_shift(KC_N, is_pressed); }
        case _M: { return if_held_170_add_shift(KC_M, is_pressed); }

        case _LBRC: { return after_lead_replace(KC_LBRC, KC_LBRC, KC_LSFT, is_pressed); }
        case _RBRC: { return after_lead_replace(KC_RBRC, KC_RBRC, KC_LSFT, is_pressed); }
        case _COMM: { return after_lead_replace(KC_COMM, KC_COMM, KC_LSFT, is_pressed); }
        case _SCLN: { return after_lead_replace(KC_SCLN, KC_SCLN, KC_LSFT, is_pressed); }
        case _QUOT: { return after_lead_replace(KC_QUOT, KC_QUOT, KC_LSFT, is_pressed); }
        case _GRV: { return after_lead_replace(KC_GRV, KC_GRV, KC_LSFT, is_pressed); }
        case _INS: { return after_lead_replace(KC_INS, KC_INS, KC_LSFT, is_pressed); }
        case _NUBS: { return after_lead_replace(KC_NUBS, KC_NUBS, KC_LSFT, is_pressed); }
        case _PAUS: { return after_lead_replace(KC_PAUS, KC_PAUS, KC_LSFT, is_pressed); }
        case _BSPC: { return after_lead_replace(KC_BSPC, KC_BSPC, KC_LSFT, is_pressed); }
        case _DOT: { return after_lead_replace(KC_DOT, KC_DOT, KC_LSFT, is_pressed); }

        case _CTRL_F2_F3: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F2, KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_F3, KC_LCTL, KC_NO, KC_NO, KC_NO, is_pressed, 170, true); }
        case _APP: { return if_held_170_add_shift(KC_APP, is_pressed); }
        case _CMD_COMM: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_COMM, KC_NO, KC_NO, KC_NO, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_COMM, KC_LGUI, KC_LSFT, KC_NO, KC_NO, is_pressed, 170, true); }
        case _CTRL_COMM: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_COMM, KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_COMM, KC_LCTL, KC_LSFT, KC_NO, KC_NO, is_pressed, 170, true); }
        case _GRV_: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_GRV, KC_NO, KC_NO, KC_NO, KC_NO, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_GRV, KC_LSFT, KC_LALT, KC_LCTL, KC_NO, is_pressed, 170, true); }
        case _INS_: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_INS, KC_NO, KC_NO, KC_NO, KC_NO, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_INS, KC_LSFT, KC_LALT, KC_LCTL, KC_NO, is_pressed, 170, true); }
        case _EQL_: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_EQL, KC_NO, KC_NO, KC_NO, KC_NO, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_EQL, KC_LSFT, KC_LALT, KC_LCTL, KC_NO, is_pressed, 170, true); }
        case _MINS_: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_MINS, KC_NO, KC_NO, KC_NO, KC_NO, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_MINS, KC_LSFT, KC_LALT, KC_LCTL, KC_NO, is_pressed, 170, true); }

        case _ENTER: { return if_held_170_add_shift(KC_ENTER, is_pressed); }
        case _TAB: { return if_held_170_add_shift(KC_TAB, is_pressed); }
        case _SPACE: { return if_held_170_add_shift(KC_SPC, is_pressed); }
        case _DEL: { return if_held_170_add_shift(KC_DEL, is_pressed); }
        case _SLASH: { return if_held_170_add_shift(KC_SLSH, is_pressed); }
        case _BSLASH: { return if_held_170_add_shift(KC_BSLS, is_pressed); }
        case _ESC: { return if_held_170_add_shift(KC_ESC, is_pressed); }

        // custom hold keys for numbers
        case _1_: { return after_lead_replace(KC_1, KC_1, KC_NO, is_pressed); } // disable key repeat
        case _0_: { return after_lead_replace(KC_0, KC_0, KC_NO, is_pressed); } // disable key repeat
        case _2_PLEFT: { return after_lead_replace(KC_2, KC_9, KC_LSFT, is_pressed); }
        case _3_SLASH: { return after_lead_replace(KC_3, KC_MINS, KC_LSFT, is_pressed); }
        case _4_PRGHT: { return after_lead_replace(KC_4, KC_0, KC_LSFT, is_pressed); }
        case _5_EQL: { return after_lead_replace(KC_5, KC_EQL, KC_NO, is_pressed); }
        case _6_PLUS: { return after_lead_replace(KC_6, KC_EQL, KC_LSFT, is_pressed); }
        case _7_BANG: { return after_lead_replace(KC_7, KC_1, KC_LSFT, is_pressed); }
        case _8_DASH: { return after_lead_replace(KC_8, KC_MINS, KC_NO, is_pressed); }
        case _9_QUEST: { return after_lead_replace(KC_9, KC_SLSH, KC_LSFT, is_pressed); }

        case KC_1: { return if_held_170_add_shift(KC_1, is_pressed); }
        case KC_2: { return if_held_170_add_shift(KC_2, is_pressed); }
        case KC_3: { return if_held_170_add_shift(KC_3, is_pressed); }
        case KC_4: { return if_held_170_add_shift(KC_4, is_pressed); }
        case KC_5: { return if_held_170_add_shift(KC_5, is_pressed); }
        case KC_6: { return if_held_170_add_shift(KC_6, is_pressed); }
        case KC_7: { return if_held_170_add_shift(KC_7, is_pressed); }
        case KC_8: { return if_held_170_add_shift(KC_8, is_pressed); }
        case KC_9: { return if_held_170_add_shift(KC_9, is_pressed); }
        case KC_0: { return if_held_170_add_shift(KC_0, is_pressed); }

        case KC_F1: { return after_lead_f_key(KC_F1, is_pressed); }
        case KC_F2: { return after_lead_f_key(KC_F2, is_pressed); }
        case KC_F3: { return after_lead_f_key(KC_F3, is_pressed); }
        case KC_F4: { return after_lead_f_key(KC_F4, is_pressed); }
        case KC_F5: { return after_lead_f_key(KC_F5, is_pressed); }
        case KC_F6: { return after_lead_f_key(KC_F6, is_pressed); }
        case KC_F7: { return after_lead_f_key(KC_F7, is_pressed); }
        case KC_F8: { return after_lead_f_key(KC_F8, is_pressed); }
        case KC_F9: { return after_lead_f_key(KC_F9, is_pressed); }
        case KC_F10: { return after_lead_f_key(KC_F10, is_pressed); }
        case KC_F11: { return after_lead_f_key(KC_F11, is_pressed); }
        case KC_F12: { return after_lead_f_key(KC_F12, is_pressed); }
        case KC_F13: { return after_lead_f_key(KC_F13, is_pressed); }
        case KC_F14: { return after_lead_f_key(KC_F14, is_pressed); }
        case KC_F15: { return after_lead_f_key(KC_F15, is_pressed); }
        case KC_F16: { return after_lead_f_key(KC_F16, is_pressed); }

        // ESCAPE AS LEADER KEY
        // cmd/ctrl home/end
        case KC_LEFT: {
          if (is_lead(KC_LEFT, is_pressed)) { return false; }
          return following_custom_leader_key(KC_HOME, isMac ? KC_LGUI : KC_NO, KC_NO, KC_NO, &esc_timer, is_pressed, 250);
        }
        case KC_RGHT: {
          if (is_lead(KC_RGHT, is_pressed)) { return false; }
          return following_custom_leader_key(KC_END, isMac ? KC_LGUI : KC_NO, KC_NO, KC_NO, &esc_timer, is_pressed, 250);
        }

        case KC_UP: { if (is_lead(KC_UP, is_pressed)) { return false; } return true; }
        case KC_DOWN: { if (is_lead(KC_DOWN, is_pressed)) { return false; } return true; }

        // mac layers
        case CMD_SPACE: {
          if (is_lead(KC_SPC, is_pressed)) { return false; }; return true;
        }

        case CMD_ESC: {
          if (is_lead(KC_F3, is_pressed)) { return false; }
          static uint16_t cmd_esc_layer_timer;
          if (momentary_layer_tap(KC_ESC, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, &cmd_esc_layer_timer, &cmd_esc_interrupted, is_pressed, 170, true)) {
            esc_timer = timer_read();
          }
          return true;
        }

        case ALT_SHIFT_BS: {
          if (is_lead(KC_BSPC, is_pressed)) { return false; }
          static uint16_t alt_shift_layer_timer;
          momentary_layer_tap(KC_BSPC, KC_NO, KC_LSFT, KC_LALT, KC_NO, KC_NO, &alt_shift_layer_timer, &alt_shift_interrupted, is_pressed, 170, true);
          return true;
        }

        case CTRL_DEL: {
          if (is_lead(KC_DEL, is_pressed)) { return false; }
          static uint16_t ctrl_del_layer_timer;
          momentary_layer_tap(KC_DEL, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_del_layer_timer, &ctrl_del_interrupted, is_pressed, 170, true);
          return true;
        }

        case CTRL_F1: {
          static uint16_t ctrl_f1_layer_timer;
          momentary_layer_tap(KC_F1, KC_LCTL, KC_LGUI, KC_NO, KC_NO, KC_NO, &ctrl_f1_layer_timer, &ctrl_f1_interrupted, is_pressed, 170, true);
          return true;
        }

        case ALT_SLASH_MAC: {
          if (is_lead(KC_SLSH, is_pressed)) { return false; }
          static uint16_t alt_slash_mac_layer_timer;
          momentary_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_slash_mac_layer_timer, &alt_slash_mac_interrupted, is_pressed, 170, true);
          return true;
        }

        case ALT_BSLASH_MAC: {
          if (is_lead(KC_BSLS, is_pressed)) { return false; }
          static uint16_t alt_bslash_mac_layer_timer;
          momentary_layer_tap(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bslash_mac_layer_timer, &alt_bslash_mac_interrupted, is_pressed, 170, true);
          return true;
        }

        case SHIFT_ENTER_MAC: {
          if (is_lead(KC_ENTER, is_pressed)) { return false; }
          static uint16_t shift_enter_mac_layer_timer;
          momentary_layer_tap(KC_ENTER, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_enter_mac_layer_timer, &shift_enter_mac_interrupted, is_pressed, 300, true);
          return true;
        }

        case SHIFT_TAB_MAC: {
          if (is_lead(KC_F2, is_pressed)) { return false; }
          static uint16_t shift_tab_mac_layer_timer;
          momentary_layer_tap(KC_TAB, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_tab_mac_layer_timer, &shift_tab_mac_interrupted, is_pressed, 300, true);
          return true;
        }

        case HYPR_F13_MAC: {
          if (is_lead(KC_F13, is_pressed)) { return false; }
          static uint16_t hypr_f13_mac_layer_timer;
          momentary_layer_tap(KC_F13, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_LGUI, &hypr_f13_mac_layer_timer, &hypr_f13_mac_interrupted, is_pressed, 300, true);
          return true;
        }

        case PALM_F14_MAC: {
          if (is_lead(KC_F14, is_pressed)) { return false; }
          static uint16_t palm_f14_mac_layer_timer;
          momentary_layer_tap(KC_F14, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &palm_f14_mac_layer_timer, &palm_f14_mac_interrupted, is_pressed, 300, false);
          return true;
        }

        // win layers
        case CTRL_SPACE: {
          if (is_lead(KC_SPC, is_pressed)) { return false; }; return true;
        }

        case CTRL_ESC: {
          if (is_lead(KC_F3, is_pressed)) { return false; }
          static uint16_t ctrl_esc_layer_timer;
          if (momentary_layer_tap(KC_ESC, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_esc_layer_timer, &ctrl_esc_interrupted, is_pressed, 170, true)) {
            esc_timer = timer_read();
          }
          return true;
        }

        case CTRL_SHIFT_BS: {
          if (is_lead(KC_BSPC, is_pressed)) { return false; }
          static uint16_t ctrl_shift_layer_timer;
          momentary_layer_tap(KC_BSPC, KC_NO, KC_LSFT, KC_LCTL, KC_NO, KC_NO, &ctrl_shift_layer_timer, &ctrl_shift_interrupted, is_pressed, 170, true);
          return true;
        }

        case CTRL_ALT_DEL: {
          if (is_lead(KC_DEL, is_pressed)) { return false; }
          static uint16_t ctrl_alt_del_layer_timer;
          momentary_layer_tap(KC_DEL, KC_NO, KC_LALT, KC_LCTL, KC_NO, KC_NO, &ctrl_alt_del_layer_timer, &ctrl_alt_del_interrupted, is_pressed, 170, false);
          return true;
        }

        case ALT_SLASH_WIN: {
          if (is_lead(KC_SLSH, is_pressed)) { return false; }
          static uint16_t alt_slash_win_layer_timer;
          momentary_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_slash_win_layer_timer, &alt_slash_win_interrupted, is_pressed, 170, false);
          return true;
        }

        case SHIFT_ENTER_WIN: {
          if (is_lead(KC_ENTER, is_pressed)) { return false; }
          static uint16_t shift_enter_win_layer_timer;
          momentary_layer_tap(KC_ENTER, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_enter_win_layer_timer, &shift_enter_win_interrupted, is_pressed, 300, true);
          return true;
        }

        case SHIFT_TAB_WIN: {
          if (is_lead(KC_F2, is_pressed)) { return false; }
          static uint16_t shift_tab_win_layer_timer;
          momentary_layer_tap(KC_TAB, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_tab_win_layer_timer, &shift_tab_win_interrupted, is_pressed, 300, true);
          return true;
        }

        case ALT_BSLASH_WIN: {
          if (is_lead(KC_BSLS, is_pressed)) { return false; }
          static uint16_t alt_bslash_win_layer_timer;
          momentary_layer_tap(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bslash_win_layer_timer, &alt_bslash_win_interrupted, is_pressed, 170, false);
          return true;
        }

        case HYPR_F13_WIN: {
          if (is_lead(KC_F13, is_pressed)) { return false; }
          static uint16_t hypr_f13_win_layer_timer;
          momentary_layer_tap(KC_F13, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_LGUI, &hypr_f13_win_layer_timer, &hypr_f13_win_interrupted, is_pressed, 300, false);
          return true;
        }

        case PALM_F14_WIN: {
          if (is_lead(KC_F14, is_pressed)) { return false; }
          static uint16_t palm_f14_win_layer_timer;
          momentary_layer_tap(KC_F14, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &palm_f14_win_layer_timer, &palm_f14_win_interrupted, is_pressed, 300, false);
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
        case W_EQL: { return replace_ctrl_alt_with_lgui(KC_EQL, is_pressed); }
        case W_MINS: { return replace_ctrl_alt_with_lgui(KC_MINS, is_pressed); }

        // Deleting words / lines
        case DEL_LEFT_MAC: { return delete_word_line(KC_BSPC, KC_LGUI, KC_LALT, KC_LEFT, KC_LGUI, KC_LSFT, is_pressed, 170); }
        case DEL_RIGHT_MAC: { return delete_word_line(KC_DEL, KC_LGUI, KC_LALT, KC_RGHT, KC_LGUI, KC_LSFT, is_pressed, 170); }

        case DEL_LEFT_WIN: { return delete_word_line(KC_BSPC, KC_LCTL, KC_LCTL, KC_HOME, KC_LSFT, KC_NO, is_pressed, 170); }
        case DEL_RIGHT_WIN: { return delete_word_line(KC_DEL, KC_LCTL, KC_LCTL, KC_END, KC_LSFT, KC_NO, is_pressed, 170); }

        // REPEATING KEYCODES
        case KC_PGUP: { return repeat(KC_PGUP, KC_NO, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, is_pressed, 25); }
        case KC_PGDN: { return repeat(KC_PGDN, KC_NO, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, is_pressed, 25); }

        // app switch
        case CMD_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LALT, KC_LCTL, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LGUI, KC_NO, KC_NO, KC_NO, is_pressed, 170, true); }
        case CMD_SHIFT_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LGUI, KC_LSFT, KC_NO, KC_NO, is_pressed, 170, true); }

        case ALT_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LGUI, KC_LCTL, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 170, true); }
        case ALT_SHIFT_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LGUI, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LALT, KC_LSFT, KC_NO, KC_NO, is_pressed, 170, true); }

        // home/end
        case HOME_: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_HOME, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_HOME, KC_NO, KC_NO, KC_NO, KC_NO, is_pressed, 250, true); }
        case END_: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_END, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_END, KC_NO, KC_NO, KC_NO, KC_NO, is_pressed, 250, true); }

        case CTRL_HOME: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_HOME, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LCTL, KC_NO, KC_NO, KC_NO, is_pressed, 250, false); }
        case CTRL_END: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_END, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LCTL, KC_NO, KC_NO, KC_NO, is_pressed, 250, false); }

        case LANG_CAPS_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_SPC, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_CAPS, KC_NO, KC_NO, KC_NO, KC_NO, is_pressed, 170, true); }
        case LANG_CAPS_MAC: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_SPC, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, KC_LCAP, KC_NO, KC_NO, KC_NO, KC_NO, is_pressed, 170, true); }

        case SELECT_UP_MAC: { return if_held_170_add_shift(KC_UP, is_pressed); }
        case SELECT_DOWN_MAC: { return if_held_170_add_shift(KC_DOWN, is_pressed); }

        case SELECT_UP_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_UP, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LCTL, KC_LSFT, KC_NO, KC_NO, is_pressed, 170, true); }
        case SELECT_DOWN_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOWN, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LCTL, KC_LSFT, KC_NO, KC_NO, is_pressed, 170, false); }

        case SELECT_LEFT_MAC: { return if_held_add_mods(KC_LEFT, KC_LSFT, KC_NO, is_pressed, 250); }
        case SELECT_RIGHT_MAC: { return if_held_add_mods(KC_RGHT, KC_LSFT, KC_NO, is_pressed, 250); }

        case SELECT_LEFT_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_LEFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LSFT, KC_NO, KC_NO, KC_NO, is_pressed, 250, true); }
        case SELECT_RIGHT_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_RGHT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LSFT, KC_NO, KC_NO, KC_NO, is_pressed, 250, true); }

        case FIND_NEXT: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F3, KC_LALT, KC_LSFT, KC_LCTL, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_NO, KC_F3, KC_NO, KC_NO, KC_NO, KC_NO, is_pressed, 170, true); }
        case FIND_PREV: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F3, KC_LALT, KC_LSFT, KC_LCTL, KC_LGUI, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_F3, KC_LSFT, KC_NO, KC_NO, KC_NO, is_pressed, 170, true); }

        // close app
        case CMD_Q: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_Q, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_Q, KC_LGUI, KC_NO, KC_NO, KC_NO, is_pressed, 170, true); }
        case ALT_F4: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F4, KC_LGUI, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_F4, KC_F4, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 170, true); }

        // minimize app / minimize others
        // cmd + H / cmd + opt + H
        case HIDE_FOCUS_MAC: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_H, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_H, KC_LALT, KC_NO, KC_NO, KC_NO, is_pressed, 300, true); }
        // hypr + down / hypr + up
        case HIDE_FOCUS_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOWN, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_NO, KC_NO, is_pressed, 300, true); }

        case SLEEP_POWER: { return sleep_power(is_pressed, 300); }

        // mac-only overrides
        case CTRL_DOT: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOT, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_DOT, KC_LGUI, KC_LSFT, KC_NO, KC_NO, is_pressed, 170, false); }
        case CTRL_H: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_H, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_H, KC_LGUI, KC_LSFT, KC_NO, KC_NO, is_pressed, 170, false); }
        case CTRL_M: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_M, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_M, KC_LGUI, KC_LSFT, KC_NO, KC_NO, is_pressed, 170, false); }

        default: {
          return true;
        }
    }
}
