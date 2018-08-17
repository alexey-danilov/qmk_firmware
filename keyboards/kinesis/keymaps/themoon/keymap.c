#include "kinesis.h"

#define __________ KC_TRNS
#define XXXXXXX KC_NO

// mac layer: 5 + f5
#define _MAC 0
#define _COMMAND_ESCAPE 1
#define _COMMAND_SPACE 2
#define _ALT_SHIFT_BS 3
#define _CTRL 4

// win layer: 5 + f7
#define _WIN 5
#define _CONTROL_ESCAPE 6
#define _CONTROL_SPACE 7
#define _CTRL_SHIFT_BS 8

#define _PALM 9
#define _ALT 10

enum kinesis_keycodes {
  // mac
  CMD_ESC = MO(_COMMAND_ESCAPE),
  CMD_SPACE = LT(_COMMAND_SPACE, KC_SPC),
  ALT_SHIFT_BS = MO(_ALT_SHIFT_BS),
  ALT_SHIFT_DEL = MT(MOD_RALT | MOD_RSFT, KC_DEL),
  ALT_SLASH = MO(_ALT),
  CTRL_CMD_BS = MO(_CTRL),
  CTRL_F16 = LT(_CTRL, KC_F16),

  // win
  CTRL_ESC = MO(_CONTROL_ESCAPE),
  CTRL_SPACE = LT(_CONTROL_SPACE, KC_SPC),
  CTRL_SHIFT_DEL = MT(MOD_RCTL | MOD_RSFT, KC_DEL),
  CTRL_SHIFT_BS = MO(_CTRL_SHIFT_BS),
  LGUI_DEL = MT(MOD_LGUI, KC_DEL),
  ALT_SLASH_WIN = MT(MOD_LALT, KC_SLSH), // on windows single alt press results in activating menu

  // common
  MEH_F13 = MO(_PALM),
  MEH_F14 = LT(_PALM, KC_F14),
  ALT_BSLASH = MT(MOD_RALT, KC_BSLS),
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
  MOD_COMMA,
  MOD_RBRC,

  MOD_SPACE,
  MOD_Y,
  MOD_U,
  MOD_I,
  MOD_O,
  MOD_H,
  MOD_J,
  MOD_K,
  MOD_L,
  MOD_N,
  MOD_M,
  MOD_DOT,
  MOD_LEFT,
  MOD_RIGHT,
  MOD_UP,
  MOD_DOWN,

  // mac-specific overrides
  CTRL_COMMA,
  CTRL_DOT,
  CTRL_H,
  CTRL_M,
  SHIFT_TAB_MAC,
  SHIFT_BSLS_MAC,

  SHIFT_TAB_WIN,
  SHIFT_BSLS_WIN,

  ALT_BSPC, // alt + backspace

  DYNAMIC_MACRO_RANGE,
};

#include "dynamic_macro.h"

enum {
    MAIL = 0,
    SLEEP,
    DEL_WORD,
};

//**************** Definitions needed for quad function to work *********************//
enum {
  COMM_TD = 0,
  LB_TD = 1,
  RB_TD = 2,
  K_TD = 3,
  TAP_MACRO1 = 4,
  TAP_MACRO2 = 5
};
//Enums used to clearly convey the state of the tap dance
enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5 //send SINGLE_TAP twice - NOT DOUBLE_TAP
  // Add more enums here if you want for triple, quadruple, etc.
};

typedef struct {
  bool is_press_action;
  int state;
} tap;

int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    //If count = 1, and it has been interrupted - it doesn't matter if it is pressed or not: Send SINGLE_TAP
    if (state->interrupted || state->pressed==0) return SINGLE_TAP;
    else return SINGLE_HOLD;
  }
  //If count = 2, and it has been interrupted - assume that user is trying to type the letter associated
  //with single tap. In example below, that means to send `xx` instead of `Escape`.
  else if (state->count == 2) {
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else return 6; //magic number. At some point this method will expand to work for more presses
}

//**************** COMMA TAP *********************//
static tap comma_tap_state = {
  .is_press_action = true,
  .state = 0
};

void comma_finished (qk_tap_dance_state_t *state, void *user_data) {
  comma_tap_state.state = cur_dance(state);
  switch (comma_tap_state.state) {
    case SINGLE_TAP: register_code(KC_COMM); break;
    case SINGLE_HOLD: register_code(KC_LSFT); register_code(KC_COMM); unregister_code(KC_COMM); break;
    default: register_code(KC_LSFT); register_code(KC_F14); unregister_code(KC_F14); break;
  }
}

void comma_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (comma_tap_state.state) {
    case SINGLE_TAP: unregister_code(KC_COMM); break;
    case SINGLE_HOLD: unregister_code(KC_LSFT); break;
    default: unregister_code(KC_LSFT); break;
  }
  comma_tap_state.state = 0;
}

//**************** LBRAC TAP *********************//
static tap lb_tap_state = {
  .is_press_action = true,
  .state = 0
};

void lb_finished (qk_tap_dance_state_t *state, void *user_data) {
  lb_tap_state.state = cur_dance(state);
  switch (lb_tap_state.state) {
    case SINGLE_TAP: register_code(KC_LBRC); break;
    case SINGLE_HOLD: register_code(KC_LSFT); register_code(KC_LBRC); unregister_code(KC_LBRC); break;
    default: register_code(KC_HOME); break;
  }
}

void lb_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (lb_tap_state.state) {
    case SINGLE_TAP: unregister_code(KC_LBRC); break;
    case SINGLE_HOLD: unregister_code(KC_LSFT); break;
    default: unregister_code(KC_HOME); break;
  }
  lb_tap_state.state = 0;
}

//**************** RBRAC TAP *********************//
static tap rb_tap_state = {
  .is_press_action = true,
  .state = 0
};

void rb_finished (qk_tap_dance_state_t *state, void *user_data) {
  rb_tap_state.state = cur_dance(state);
  switch (rb_tap_state.state) {
    case SINGLE_TAP: register_code(KC_RBRC); break;
    case SINGLE_HOLD: register_code(KC_LSFT); register_code(KC_RBRC); unregister_code(KC_RBRC); break;
    default: register_code(KC_END); break;
  }
}

void rb_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (rb_tap_state.state) {
    case SINGLE_TAP: unregister_code(KC_RBRC); break;
    case SINGLE_HOLD: unregister_code(KC_LSFT); break;
    default: unregister_code(KC_END);
  }
  rb_tap_state.state = 0;
}

//**************** K TAP *********************//
static tap k_tap_state = {
  .is_press_action = true,
  .state = 0
};

void k_finished (qk_tap_dance_state_t *state, void *user_data) {
  k_tap_state.state = cur_dance(state);
  switch (k_tap_state.state) {
    case SINGLE_TAP: register_code(KC_K); break;
    case SINGLE_HOLD: register_code(KC_LSFT); register_code(KC_K); unregister_code(KC_K); break;
    default: register_code(KC_LCTL); register_code(KC_SPC); unregister_code(KC_SPC); break;
  }
}

void k_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (k_tap_state.state) {
    case SINGLE_TAP: unregister_code(KC_K); break;
    case SINGLE_HOLD: unregister_code(KC_LSFT); break;
    default: unregister_code(KC_LCTL); break;
  }
  k_tap_state.state = 0;
}

//**************** DYNAMIC MACRO1 TAP *********************//
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

//**************** DYNAMIC MACRO2 TAP *********************//
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

//**************** ALL TAP MACROS *********************//
qk_tap_dance_action_t tap_dance_actions[] = {
  // This Tap dance plays the macro 1 on TAP and records it on double tap.
  [TAP_MACRO1] = ACTION_TAP_DANCE_FN(macro1_tapdance_fn),
  [TAP_MACRO2] = ACTION_TAP_DANCE_FN(macro2_tapdance_fn),
  [LB_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lb_finished, lb_reset),
  [COMM_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, comma_finished, comma_reset),
  [RB_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rb_finished, rb_reset),
  [K_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, k_finished, k_reset)
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

    case DEL_WORD: {
        if (record->event.pressed) {
            SEND_STRING(SS_DOWN(X_LSHIFT) SS_TAP(X_LEFT) SS_UP(X_LSHIFT) SS_TAP(X_DELETE));
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
* | Sleep  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | PSCR | SLCK | PAUS |Program| Power |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* | =+     |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  | -_     |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | `~     |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  | F17    |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | CAPS   |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  | F18    |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | Mail   |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  | F19    |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |Insert|  [{  |  ,<  |  ]}  |                                         | Left | Down | Right| F15  |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,-------------------------.
*                            | LShift+RAlt/BkSp | Macro|         | Sleep| LShift+RAlt/Del  |
*                            `-----------|------|------|         |------+------+-----------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LCMD/|Shift/|------|         |------|Shift/|RCMD/ |
*                                 | ESC  |Enter |Ctrl/ |         |      |      |SPACE |
                                  |      |      |CMD_BkSp|     |Ctrl/F16|Tab   |      |
*                                 `--------------------'         `--------------------'
*                              PALM/F13                                            PALM/F14
*/

// base layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_MAC] = LAYOUT(
           // left side
           M(SLEEP), KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8,
           KC_EQL, KC_1, KC_2, KC_3, KC_4, KC_5,
           KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_CAPSLOCK,KC_A, KC_S, KC_D, KC_F, KC_G,
           M(MAIL) ,KC_Z, KC_X, KC_C, KC_V, KC_B,
                 KC_INS, TD(LB_TD), TD(COMM_TD), TD(RB_TD),
                                           // left thumb keys
			                                    ALT_SHIFT_BS,TD(TAP_MACRO1),
                                                   ALT_SLASH,
                           CMD_ESC, KC_SFTENT, CTRL_CMD_BS,
                                     // left palm key
			                         MEH_F13,
    // right side
    KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,KC_PSCR ,KC_SLCK  ,KC_PAUS, RESET, KC_POWER,
	KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS,
	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_F17,
	KC_H, KC_J, TD(K_TD), KC_L, KC_SCLN, KC_F18,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F19,
	KC_LEFT, KC_DOWN, KC_RGHT, KC_F15,
           // right thumb keys
           TD(TAP_MACRO2),ALT_SHIFT_DEL,
           ALT_BSLASH,
           CTRL_F16, SFT_T(KC_TAB), CMD_SPACE,
                                    // right palm key
                                    MEH_F14
    ),

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
         KC_F2,  __________,
         SHIFT_BSLS_MAC,
         KC_F16,  SHIFT_TAB_MAC,  MOD_SPACE,
                             __________
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
                                     __________,
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
         __________,
         __________,  __________,  __________,
                             __________
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
                                     __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________ ,  __________,  __________,
                   __________,  __________, __________, __________,
         __________,  __________,
         __________,
         CTRL_F16,  __________,  __________,
                             __________
    ),

[_WIN] = LAYOUT(
           // left side
           KC_SLEP, KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8,
           KC_EQL, KC_1, KC_2, KC_3, KC_4, KC_5,
           KC_GRV, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_CAPSLOCK,KC_A, KC_S, KC_D, KC_F, KC_G,
           M(MAIL) ,KC_Z, KC_X, KC_C, KC_V, KC_B,
                 KC_INS, TD(LB_TD), TD(COMM_TD), TD(RB_TD),
                                           // left thumb keys
			                                    CTRL_SHIFT_BS,TD(TAP_MACRO1),
                                                   ALT_SLASH_WIN,
                           CTRL_ESC, KC_SFTENT, LGUI_DEL,
                                     // left palm key
			                         MEH_F13,
    // right side
    KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,KC_PSCR ,KC_SLCK  ,KC_PAUS, RESET, KC_POWER,
	KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS,
	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_F17,
	KC_H, KC_J, TD(K_TD), KC_L, KC_SCLN, KC_F18,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F19,
	KC_LEFT, KC_DOWN, KC_RGHT, KC_APP,
           // right thumb keys
           TD(TAP_MACRO2),CTRL_SHIFT_DEL,
           ALT_BSLASH,
           KC_RGUI, SFT_T(KC_TAB), CTRL_SPACE,
                                    // right palm key
                                    MEH_F14
    ),

[_CONTROL_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,   __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
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
         KC_F2,  __________,
         SHIFT_BSLS_WIN,
         __________,  SHIFT_TAB_WIN,  MOD_SPACE,
                             __________
    ),

[_CONTROL_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  MOD_W,  MOD_E,  MOD_R,  MOD_T,
         __________,  __________,  MOD_S,  MOD_D,  MOD_F,  MOD_G,
         __________,  __________,  MOD_X,  MOD_C,  MOD_V,  MOD_B,
                      __________,  MOD_LBRC, MOD_COMMA, MOD_RBRC,
                             M(DEL_WORD),  KC_F1,
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
         __________,
         __________,  __________,  __________,
                             __________
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

[_PALM] = LAYOUT(
__________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
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
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  KC_PGUP,  __________ ,  __________,  __________,
                   KC__VOLDOWN,  KC_PGDN, KC__VOLUP, __________,
         __________,  __________,
         __________,
         __________,  __________,  KC__MUTE,
                             MEH_F14
    ),
};

void matrix_init_user(void) {
}

void key_code(uint16_t key) {
            register_code(key);
            unregister_code(key);
}

bool not_held(uint16_t hold_timer, uint16_t hold_duration) {
    return timer_elapsed(hold_timer) < hold_duration;
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

bool without_mods(uint16_t code, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t mod4) {
   unregister_code(mod1);
   unregister_code(mod2);
   unregister_code(mod3);
   unregister_code(mod4);

   key_code(code);

   register_code(mod4);
   register_code(mod3);
   register_code(mod2);
   register_code(mod1);

  return false;
}

bool no_meh(uint16_t code) {
  return without_mods(code, KC_LSFT, KC_LALT, KC_LCTL, KC_NO);
}

uint16_t palm_repeat_code;
uint16_t palm_repeat_timer;
uint8_t first_repeat_delay;
bool no_meh_repeat(uint16_t code, bool pressed) {
   if (pressed) {
       no_meh(code);
       palm_repeat_code = code;
       palm_repeat_timer = timer_read();
       first_repeat_delay = 180;
   } else {
       palm_repeat_code = 0;
   }

  return false;
}

bool ___if_held___add_shift(uint16_t code, bool pressed, uint8_t hold_duration) {
  return ___if_held___add_mod(code, KC_LSFT, pressed, hold_duration);
}

bool ___if_held_140___add_shift(uint16_t code, bool pressed) {
  return ___if_held___add_shift(code, pressed, 140);
}

bool ___if_held_180___add_shift(uint16_t code, bool pressed) {
  return ___if_held___add_shift(code, pressed, 180);
}

bool ___if_held_180___replace_add_mod(uint16_t code, uint16_t replacement_code, uint16_t mod_if_held, bool pressed) {
  return ___if_held_replace_add_mod(code, replacement_code, mod_if_held, pressed, 180);
}

void matrix_scan_user(void) {
   if (palm_repeat_code) {
      if (timer_elapsed(palm_repeat_timer) > (50 + first_repeat_delay)) {
         no_meh(palm_repeat_code);
         first_repeat_delay = 0;
      }
   }
}

// returns true if tap was triggered and false otherwise
bool mo_layer_tap(uint16_t tap_key, uint16_t tap_mod, uint16_t layer_mod1, uint16_t layer_mod2, uint16_t layer_mod3, uint16_t layer_mod4, uint16_t *layer_timer, bool *interrupted_flag, bool is_pressed, uint16_t hold_duration) {
  if (is_pressed) {
    *interrupted_flag = false;
    *layer_timer = timer_read();
  } else {
    if (not_held(*layer_timer, hold_duration)) {
      if (!*interrupted_flag) {
        // unregister mods associated with the layer, so that they don't intefere with the tap key
        unregister_code(layer_mod1);
        unregister_code(layer_mod2);
        unregister_code(layer_mod3);
        unregister_code(layer_mod4);

        // register tap key and its mod
        register_code(tap_mod);
        register_code(tap_key);
        unregister_code(tap_key);
        unregister_code(tap_mod);

        // register layer mods back
        register_code(layer_mod4);
        register_code(layer_mod3);
        register_code(layer_mod2);
        register_code(layer_mod1);

        return true;
      }
    }
  }
  return false;
}

bool after_leader(uint16_t key, uint16_t mod1, uint16_t mod2, uint16_t *leader_timer, bool is_pressed, uint16_t esc_last_pressed_timeout) {
  if (*leader_timer && not_held(*leader_timer, esc_last_pressed_timeout)) {
    if (is_pressed) {
      *leader_timer = 0;
      with_2_mods(key, mod1, mod2);
      return false;
    }
  }
  return true;
}

uint16_t esc_timer; // this will be a timer for leader key: esc
bool cmd_esc_interrupted = true;
bool meh_interrupted = true;
bool alt_interrupted = true;
bool ctrl_interrupted = true;
bool alt_shift_interrupted = true;
bool ctrl_shift_interrupted = true;
bool ctrl_esc_interrupted = true;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if ((keycode != TD(TAP_MACRO1)) && (keycode != TD(TAP_MACRO2))) {
      // That key is processed by the macro1_tapdance_fn. Not ignoring it here is
      // mostly a no-op except that it is recorded in the macros (and uses space).
      // We can't just return false when the key is a tap dance, because
      // process_record_user, is called before the tap dance processing (and
      // returning false would eat the tap dance).
      if (!process_record_dynamic_macro(keycode, record)) {
        return false;
      }
    }

    bool is_pressed = record->event.pressed;

    if (keycode != CMD_ESC) { cmd_esc_interrupted = true; }
    if (keycode != MEH_F13) { meh_interrupted = true; }
    if (keycode != ALT_SLASH) { alt_interrupted = true; }
    if (keycode != CTRL_CMD_BS) { ctrl_interrupted = true; }
    if (keycode != ALT_SHIFT_BS) { alt_shift_interrupted = true; }
    if (keycode != CTRL_ESC) { ctrl_esc_interrupted = true; }
    if (keycode != CTRL_SHIFT_BS) { ctrl_shift_interrupted = true; }

    switch (keycode) {
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
          static uint16_t meh_layer_timer;
          mo_layer_tap(KC_F13, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_NO, &meh_layer_timer, &meh_interrupted, is_pressed, 300);
          return true;
        }

        case ALT_SLASH: {
          static uint16_t alt_layer_timer;
          mo_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_layer_timer, &alt_interrupted, is_pressed, 180);
          return true;
        }

        // leaded keys
        case KC_RGHT: {
          return after_leader(KC_RGHT, KC_LGUI, KC_LCTL, &esc_timer, is_pressed, 180);
        }

        case KC_LEFT: {
          return after_leader(KC_LEFT, KC_LGUI, KC_LCTL, &esc_timer, is_pressed, 180);
        }

        // no holding delay
        case KC_PGUP: {return no_meh_repeat(KC_PGUP, is_pressed); }
        case KC_PGDN: {return no_meh_repeat(KC_PGDN, is_pressed); }

        case KC__VOLDOWN: {return no_meh_repeat(KC__VOLDOWN, is_pressed); }
        case KC__MUTE: {return no_meh_repeat(KC__MUTE, is_pressed); }
        case KC__VOLUP: {return no_meh_repeat(KC__VOLUP, is_pressed); }

        // 140 ms
        case MOD_SPACE: {return ___if_held_140___add_shift(KC_SPC, is_pressed); }
        case MOD_ESC: {return ___if_held_140___add_shift(KC_ESC, is_pressed); }
        case MOD_ENTER: {return ___if_held_140___add_shift(KC_ENTER, is_pressed); }

        // 180 ms
        case MOD_W: {return ___if_held_180___add_shift(KC_W, is_pressed); }
        case MOD_E: {return ___if_held_180___add_shift(KC_E, is_pressed); }
        case MOD_R: {return ___if_held_180___add_shift(KC_R, is_pressed); }
        case MOD_T: {return ___if_held_180___add_shift(KC_T, is_pressed); }
        case MOD_S: {return ___if_held_180___add_shift(KC_S, is_pressed); }
        case MOD_D: {return ___if_held_180___add_shift(KC_D, is_pressed); }
        case MOD_F: {return ___if_held_180___add_shift(KC_F, is_pressed); }
        case MOD_G: {return ___if_held_180___add_shift(KC_G, is_pressed); }
        case MOD_X: {return ___if_held_180___add_shift(KC_X, is_pressed); }
        case MOD_C: {return ___if_held_180___add_shift(KC_C, is_pressed); }
        case MOD_V: {return ___if_held_180___add_shift(KC_V, is_pressed); }
        case MOD_B: {return ___if_held_180___add_shift(KC_B, is_pressed); }
        case MOD_LBRC: {return ___if_held_180___add_shift(KC_LBRC, is_pressed); }
        case MOD_COMMA: {return ___if_held_180___add_shift(KC_COMM, is_pressed); }
        case MOD_RBRC: {return ___if_held_180___add_shift(KC_RBRC, is_pressed); }

        case MOD_Y: {return ___if_held_180___add_shift(KC_Y, is_pressed); }
        case MOD_U: {return ___if_held_180___add_shift(KC_U, is_pressed); }
        case MOD_I: {return ___if_held_180___add_shift(KC_I, is_pressed); }
        case MOD_O: {return ___if_held_180___add_shift(KC_O, is_pressed); }
        case MOD_H: {return ___if_held_180___add_shift(KC_H, is_pressed); }
        case MOD_J: {return ___if_held_180___add_shift(KC_J, is_pressed); }
        case MOD_K: {return ___if_held_180___add_shift(KC_K, is_pressed); }
        case MOD_L: {return ___if_held_180___add_shift(KC_L, is_pressed); }
        case MOD_N: {return ___if_held_180___add_shift(KC_N, is_pressed); }
        case MOD_M: {return ___if_held_180___add_shift(KC_M, is_pressed); }
        case MOD_DOT: {return ___if_held_180___add_shift(KC_DOT, is_pressed); }
        case MOD_LEFT: {return ___if_held_180___add_shift(KC_LEFT, is_pressed); }
        case MOD_RIGHT: {return ___if_held_180___add_shift(KC_RGHT, is_pressed); }
        case MOD_UP: {return ___if_held_180___add_shift(KC_UP, is_pressed); }
        case MOD_DOWN: {return ___if_held_180___add_shift(KC_DOWN, is_pressed); }

        case KC_2: {return ___if_held_180___replace_add_mod(KC_2, KC_9, KC_LSFT, is_pressed); }
        case KC_3: {return ___if_held_180___replace_add_mod(KC_3, KC_MINS, KC_LSFT, is_pressed); }
        case KC_4: {return ___if_held_180___replace_add_mod(KC_4, KC_0, KC_LSFT, is_pressed); }
        case KC_5: {return ___if_held_180___replace_add_mod(KC_5, KC_EQL, KC_NO, is_pressed); }
        case KC_6: {return ___if_held_180___replace_add_mod(KC_6, KC_EQL, KC_LSFT, is_pressed); }
        case KC_7: {return ___if_held_180___replace_add_mod(KC_7, KC_1, KC_LSFT, is_pressed); }
        case KC_8: {return ___if_held_180___replace_add_mod(KC_8, KC_MINS, KC_NO, is_pressed); }
        case KC_9: {return ___if_held_180___replace_add_mod(KC_9, KC_SLSH, KC_LSFT, is_pressed); }

        case KC_F1: {return ___if_held_180___add_shift(KC_F1, is_pressed); }
        case KC_F2: {return ___if_held_180___add_shift(KC_F2, is_pressed); }
        case KC_F3: {return ___if_held_180___add_shift(KC_F3, is_pressed); }
        case KC_F4: {return ___if_held_180___add_shift(KC_F4, is_pressed); }
        case KC_F5: {return ___if_held_180___add_shift(KC_F5, is_pressed); }
        case KC_F6: {return ___if_held_180___add_shift(KC_F6, is_pressed); }
        case KC_F7: {return ___if_held_180___add_shift(KC_F7, is_pressed); }
        case KC_F8: {return ___if_held_180___add_shift(KC_F8, is_pressed); }
        case KC_F9: {return ___if_held_180___add_shift(KC_F9, is_pressed); }
        case KC_F10: {return ___if_held_180___add_shift(KC_F10, is_pressed); }
        case KC_F11: {return ___if_held_180___add_shift(KC_F11, is_pressed); }
        case KC_F12: {return ___if_held_180___add_shift(KC_F12, is_pressed); }
        case KC_F13: {return ___if_held_180___add_shift(KC_F13, is_pressed); }
        case KC_F14: {return ___if_held_180___add_shift(KC_F14, is_pressed); }
        case KC_F15: {return ___if_held_180___add_shift(KC_F15, is_pressed); }
        case KC_F16: {return ___if_held_180___add_shift(KC_F16, is_pressed); }

        // mac-specific overrides
        case CTRL_COMMA: {return replace_cmd_with_mod____if_held___cmd_shift(KC_COMM, KC_LCTL, is_pressed, 180); }
        case CTRL_DOT: {return replace_cmd_with_mod____if_held___cmd_shift(KC_DOT, KC_LCTL, is_pressed, 180); }
        case CTRL_H: {return replace_cmd_with_mod____if_held___cmd_shift(KC_H, KC_LCTL, is_pressed, 180); }
        case CTRL_M: {return replace_cmd_with_mod____if_held___cmd_shift(KC_M, KC_LCTL, is_pressed, 180); }
        case SHIFT_TAB_MAC: {return replace_mod1_with_mod2____if_held___mod3_plus_mod4(KC_TAB, KC_LGUI, KC_LSFT, KC_LCTL, KC_LSFT, is_pressed, 180); }
        case SHIFT_BSLS_MAC: {return replace_mod1_with_mod2____if_held___mod3_plus_mod4(KC_BSLS, KC_LGUI, KC_LSFT, KC_LCTL, KC_LSFT, is_pressed, 180); }
        case ALT_BSPC: {return replace_cmd_with_mod____if_held___cmd_shift(KC_BSPC, KC_LALT, is_pressed, 180); }

        // win-specific overrides
        case SHIFT_TAB_WIN: {return replace_mod1_with_mod2____if_held___mod3_plus_mod4(KC_TAB, KC_LCTL, KC_LSFT, KC_LCTL, KC_LSFT, is_pressed, 180); }
        case SHIFT_BSLS_WIN: {return replace_mod1_with_mod2____if_held___mod3_plus_mod4(KC_BSLS, KC_LCTL, KC_LSFT, KC_LCTL, KC_LSFT, is_pressed, 180); }

        default: {
          return true;
        }
    }
}

void led_set_user(uint8_t usb_led) {
}

uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    // mac-specific
    case _COMMAND_ESCAPE:
      register_code(KC_LGUI);
      break;
    case _COMMAND_SPACE:
      register_code(KC_LGUI);
      break;
    case _ALT_SHIFT_BS:
      register_code(KC_LALT);
      register_code(KC_LSFT);
      break;
    case _CTRL:
      register_code(KC_LCTL);
      break;

    // win-specific
    case _CONTROL_ESCAPE:
      register_code(KC_LCTL);
      break;
    case _CONTROL_SPACE:
      register_code(KC_LCTL);
      break;
    case _CTRL_SHIFT_BS:
      register_code(KC_LCTL);
      register_code(KC_LSFT);
      break;

    // common
    case _ALT:
      register_code(KC_LALT);
      break;
    case _PALM:
      register_code(KC_LSFT);
      register_code(KC_LALT);
      register_code(KC_LCTL);
      break;

    default:
      unregister_code(KC_LGUI);
      unregister_code(KC_LCTL);
      unregister_code(KC_LALT);
      unregister_code(KC_LSFT);
      break;
    }
return state;
}