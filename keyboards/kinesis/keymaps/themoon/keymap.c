#include "kinesis.h"

#define _______ KC_TRNS
#define XXXXXXX KC_NO

#define QWERTY 0
#define _CMD_ESC 1 // command layer
#define _CMD_SPACE 2 // command layer
#define _CTRL_ALT 3 // ctrl + alt layer

enum kinesis_keycodes {
  _L_LCMD = LT(_CMD_ESC, KC_ESC),
  _L_RCMD = LT(_CMD_SPACE, KC_SPC),

  _L_PALM1 = LT(_CTRL_ALT, KC_F16),
  _L_PALM2 = LT(_CTRL_ALT, KC_F17),

  L_DEL = MT(MOD_LSFT | MOD_LALT, KC_BSPC),
  R_DEL = MT(MOD_RSFT | MOD_RALT, KC_BSPC),

  SHIFT_ENTER = MT(MOD_LSFT, KC_ENTER),
  SHIFT_TAB = MT(MOD_RSFT, KC_TAB),

  ALT_SLASH = MT(MOD_LALT, KC_SLSH),
  ALT_BSLASH = MT(MOD_RALT, KC_BSLS),

  CTRL_DEL = MT(MOD_LCTL, KC_DEL),
  CTRL_F18 = MT(MOD_RCTL, KC_F18),

  TRASH = LGUI(KC_BSPC),
  SLEEP = LALT(LGUI(KC_EJCT))
};

enum holding_keycodes {
  HOLD_2 = SAFE_RANGE,
  HOLD_3,
  HOLD_4,
  HOLD_5,
  HOLD_6,
  HOLD_7,
  HOLD_8,
  HOLD_9,
  HOLD_COMMA,
  HOLD_DOT,
  HOLD_LBRAC,
  HOLD_RBRAC,
  HOLD_F5,
  HOLD_ESC,

  HOLD_F1,
  HOLD_F2,
  HOLD_F3,
  HOLD_F4,
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
  CMD_M,

  CMD_LALT, // command + z
  CMD_LCTL, // f3

  CMD_TAB, // control + tab
  CMD_COMMA, // control + ,
  CMD_DOT, // control + .
  CMD_H, // control + H
  CMD_BSPC, // alt + backspace

  CTRL_ALT_P_UP,
  CTRL_ALT_P_DN,
  CTRL_ALT_HOME,
  CTRL_ALT_END
};


/****************************************************************************************************
*
* Keymap: Default Layer in Qwerty
*
* ,-------------------------------------------------------------------------------------------------------------------.
* |ESC/hide|  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | PSCR | SLCK | PAUS |  FN0 |  Reset |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* | =+     |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  | -_     |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | `~     |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | Caps   |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |        |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  |        |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |      |  [{  |  ,.  |  ]}  |                                         | Left | Down | Right|      |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,-------------------------.
*                            | LShift+LAlt/BkSp |Trash |         | Sleep| LShift+LAlt/BkSp |
*                            `-----------|------|------|         |------+------+-----------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LCMD/|Shift/|------|         |------|Shift/|LCMD/ |
*                                 | ESC  |Enter |Ctrl/Del|     |Ctrl/F18|Tab   |SPACE |
*                                 `--------------------'         `--------------------'
*                   LCtrl+LAlt/F16                                                     RCtrl+RAlt/F17
*/

// base layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWERTY] = KEYMAP(
           // left side
           HOLD_ESC, HOLD_F1  ,HOLD_F2  ,HOLD_F3  ,HOLD_F4  ,HOLD_F5  ,HOLD_F6  ,HOLD_F7  ,HOLD_F8,
           KC_EQL, KC_1   ,HOLD_2   ,HOLD_3   ,HOLD_4   ,HOLD_5   ,
           KC_GRV, KC_Q   ,KC_W   ,KC_E   ,KC_R   ,KC_T   ,
           KC_CAPS,KC_A   ,KC_S   ,KC_D   ,KC_F   ,KC_G   ,
           XXXXXXX,KC_Z   ,KC_X   ,KC_C   ,KC_V   ,KC_B   ,
                   XXXXXXX,HOLD_LBRAC,HOLD_COMMA,HOLD_RBRAC,
                                           // left thumb keys
			                                    L_DEL,TRASH,
                                                   ALT_SLASH,
                           _L_LCMD, SHIFT_ENTER, CTRL_DEL,
                                     // left palm key
			                         _L_PALM1,
    // right side
    HOLD_F9  ,HOLD_F10 ,HOLD_F11 ,HOLD_F12 ,KC_PSCR ,KC_SLCK  ,KC_PAUS, KC_FN0, RESET,
	HOLD_6   ,HOLD_7   ,HOLD_8   ,HOLD_9   ,KC_0   ,KC_MINS,
	KC_Y   ,KC_U   ,KC_I   ,KC_O   ,KC_P   ,XXXXXXX,
	KC_H   ,KC_J   ,KC_K   ,KC_L   ,KC_SCLN,XXXXXXX,
	KC_N   ,KC_M   ,KC_UP  ,HOLD_DOT ,KC_QUOT,XXXXXXX,
	    	KC_LEFT,KC_DOWN,KC_RGHT,XXXXXXX,
           // right thumb keys
           SLEEP,R_DEL,
           ALT_BSLASH,
           CTRL_F18, SHIFT_TAB, _L_RCMD,
                                    // right palm key
                                    _L_PALM2
    ),

[_CMD_ESC] = KEYMAP(
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
          _______,  _______,  CMD_W,  CMD_E,  CMD_R,  CMD_T,
         _______,  _______,  CMD_S,  CMD_D,  CMD_F,  CMD_G,
        _______,  _______,  CMD_X,  CMD_C,  CMD_V,  CMD_B,
                   _______,  _______,  _______,  _______,
                             _______,  _______,
                                       _______,
                    _L_LCMD, _______,  _______,
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
          _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,
                   _______,  _______,  CMD_COMMA,  _______,
                             CMD_BSPC,  _______,
                                       CMD_LALT,
                    CMD_F1, CMD_ENTER,  CMD_LCTL,
                                     _______,
          _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
          _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,
                   _______,  _______,  _______,  _______,
                             _______,  _______,
                                       _______,
                    _______, _______,  _L_RCMD,
                                     _______
    ),

// ctrl alt layer
[_CTRL_ALT] = KEYMAP(
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
          _______,  _______,  _______,  _______,  _______,  _______,
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

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    return MACRO_NONE;
};


void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

bool add_mod_on_hold(uint16_t code, uint16_t mod_to_add, bool pressed, uint16_t this_timer) {
  if(pressed) {
      this_timer= timer_read();
  } else {
      if (timer_elapsed(this_timer) < 150){
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

bool add_shift_on_hold(uint16_t code, bool pressed, uint16_t this_timer) {
  return add_mod_on_hold(code, KC_LSFT, pressed, this_timer);
}

bool replace_on_hold(uint16_t code, uint16_t replacement_code, bool pressed, uint16_t this_timer) {
  if(pressed) {
      this_timer= timer_read();
  } else {
      if (timer_elapsed(this_timer) < 150){
          register_code(code);
          unregister_code(code);
      } else {
          register_code(replacement_code);
          unregister_code(replacement_code);
      }
  }
  return false;
}

bool force_mod_plus_mod_if_held(uint16_t code, uint16_t mod, uint16_t mod_if_held, bool pressed, uint16_t this_timer) {
  if(pressed) {
      this_timer= timer_read();

  } else {
      uint8_t old_mods = get_mods();
      if (old_mods) {
        clear_mods();
      }

      if (timer_elapsed(this_timer) < 150){
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

bool force_ctrl_plus_shift_if_held(uint16_t code, bool pressed, uint16_t this_timer) {
 return force_mod_plus_mod_if_held(code, KC_LCTL, KC_LSFT, pressed, this_timer);
}

bool without_mods(uint16_t code, bool pressed, uint16_t this_timer) {
  uint8_t old_mods = get_mods();
      if (old_mods) {
        clear_mods();
  }

  register_code(code);
  unregister_code(code);

  if (old_mods) {
        set_mods(old_mods);
  }

  return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HOLD_2: { static uint16_t hold_2_timer; return replace_on_hold(KC_2, KC_LEFT_PAREN, record->event.pressed, hold_2_timer); }
        case HOLD_3: { static uint16_t hold_3_timer; return replace_on_hold(KC_3, KC_UNDERSCORE, record->event.pressed, hold_3_timer); }
        case HOLD_4: { static uint16_t hold_4_timer; return replace_on_hold(KC_4, KC_RIGHT_PAREN, record->event.pressed, hold_4_timer); }
        case HOLD_5: { static uint16_t hold_5_timer; return replace_on_hold(KC_5, KC_PLUS, record->event.pressed, hold_5_timer); }
        case HOLD_6: { static uint16_t hold_6_timer; return replace_on_hold(KC_6, KC_EQL, record->event.pressed, hold_6_timer); }
        case HOLD_7: { static uint16_t hold_7_timer; return replace_on_hold(KC_7, KC_EXCLAIM, record->event.pressed, hold_7_timer); }
        case HOLD_8: { static uint16_t hold_8_timer; return replace_on_hold(KC_8, KC_MINS, record->event.pressed, hold_8_timer); }
        case HOLD_9: { static uint16_t hold_9_timer; return replace_on_hold(KC_9, KC_QUESTION, record->event.pressed, hold_9_timer); }
        case HOLD_LBRAC: { static uint16_t hold_lbrac_timer; return replace_on_hold(KC_LBRC, KC_LEFT_CURLY_BRACE, record->event.pressed, hold_lbrac_timer); }
        case HOLD_RBRAC: { static uint16_t hold_rbrac_timer; return replace_on_hold(KC_RBRC, KC_RIGHT_CURLY_BRACE, record->event.pressed, hold_rbrac_timer); }
        case HOLD_COMMA: { static uint16_t hold_comma_timer; return replace_on_hold(KC_COMM, KC_LEFT_ANGLE_BRACKET, record->event.pressed, hold_comma_timer); }
        case HOLD_DOT: { static uint16_t hold_dot_timer; return replace_on_hold(KC_DOT, KC_RIGHT_ANGLE_BRACKET, record->event.pressed, hold_dot_timer); }
        case HOLD_F5: { static uint16_t hold_f5_timer; return replace_on_hold(KC_F5, LGUI(KC_COMM), record->event.pressed, hold_f5_timer); }
        case HOLD_ESC: { static uint16_t hold_esc_timer; return replace_on_hold(KC_ESC, LCTL(KC_F12), record->event.pressed, hold_esc_timer); }

        case HOLD_F1: { static uint16_t hold_f1_timer; return add_shift_on_hold(KC_F1, record->event.pressed, hold_f1_timer); }
        case HOLD_F2: { static uint16_t hold_f2_timer; return add_shift_on_hold(KC_F2, record->event.pressed, hold_f2_timer); }
        case HOLD_F3: { static uint16_t hold_f3_timer; return add_shift_on_hold(KC_F3, record->event.pressed, hold_f3_timer); }
        case HOLD_F4: { static uint16_t hold_f4_timer; return add_shift_on_hold(KC_F4, record->event.pressed, hold_f4_timer); }
        case HOLD_F6: { static uint16_t hold_f6_timer; return add_shift_on_hold(KC_F6, record->event.pressed, hold_f6_timer); }
        case HOLD_F7: { static uint16_t hold_f7_timer; return add_shift_on_hold(KC_F7, record->event.pressed, hold_f7_timer); }
        case HOLD_F8: { static uint16_t hold_f8_timer; return add_shift_on_hold(KC_F8, record->event.pressed, hold_f8_timer); }
        case HOLD_F9: { static uint16_t hold_f9_timer; return add_shift_on_hold(KC_F9, record->event.pressed, hold_f9_timer); }
        case HOLD_F10: { static uint16_t hold_f10_timer; return add_shift_on_hold(KC_F10, record->event.pressed, hold_f10_timer); }
        case HOLD_F11: { static uint16_t hold_f11_timer; return add_shift_on_hold(KC_F11, record->event.pressed, hold_f11_timer); }
        case HOLD_F12: { static uint16_t hold_f12_timer; return add_shift_on_hold(KC_F12, record->event.pressed, hold_f12_timer); }

        case CMD_LEFT: { static uint16_t cmd_left_timer; return add_shift_on_hold(KC_LEFT, record->event.pressed, cmd_left_timer); }
        case CMD_RIGHT:{ static uint16_t cmd_right_timer; return add_shift_on_hold(KC_RIGHT, record->event.pressed, cmd_right_timer); }
        case CMD_UP:{ static uint16_t cmd_up_timer; return add_shift_on_hold(KC_RIGHT, record->event.pressed, cmd_up_timer); }
        case CMD_DOWN:{ static uint16_t cmd_down_timer; return add_shift_on_hold(KC_RIGHT, record->event.pressed, cmd_down_timer); }
        case CMD_K: { static uint16_t cmd_k_timer; return add_shift_on_hold(KC_K, record->event.pressed, cmd_k_timer); }
        case CMD_M: { static uint16_t cmd_m_timer; return add_shift_on_hold(KC_M, record->event.pressed, cmd_m_timer); }
        case CMD_SPACE: { static uint16_t cmd_space_timer; return add_shift_on_hold(KC_SPC, record->event.pressed, cmd_space_timer); }
        case CMD_F1: { static uint16_t cmd_f1_timer; return add_shift_on_hold(KC_F1, record->event.pressed, cmd_f1_timer); }
        case CMD_ENTER: { static uint16_t cmd_enter_timer; return add_shift_on_hold(KC_ENTER, record->event.pressed, cmd_enter_timer); }
        case CMD_W: { static uint16_t cmd_w_timer; return add_shift_on_hold(KC_W, record->event.pressed, cmd_w_timer); }
        case CMD_E: { static uint16_t cmd_e_timer; return add_shift_on_hold(KC_E, record->event.pressed, cmd_e_timer); }
        case CMD_R: { static uint16_t cmd_r_timer; return add_shift_on_hold(KC_R, record->event.pressed, cmd_r_timer); }
        case CMD_T: { static uint16_t cmd_t_timer; return add_shift_on_hold(KC_T, record->event.pressed, cmd_t_timer); }
        case CMD_S: { static uint16_t cmd_s_timer; return add_shift_on_hold(KC_S, record->event.pressed, cmd_s_timer); }
        case CMD_D: { static uint16_t cmd_d_timer; return add_shift_on_hold(KC_D, record->event.pressed, cmd_d_timer); }
        case CMD_F: { static uint16_t cmd_f_timer; return add_shift_on_hold(KC_F, record->event.pressed, cmd_f_timer); }
        case CMD_G: { static uint16_t cmd_g_timer; return add_shift_on_hold(KC_G, record->event.pressed, cmd_g_timer); }
        case CMD_X: { static uint16_t cmd_x_timer; return add_shift_on_hold(KC_X, record->event.pressed, cmd_x_timer); }
        case CMD_C: { static uint16_t cmd_c_timer; return add_shift_on_hold(KC_C, record->event.pressed, cmd_c_timer); }
        case CMD_V: { static uint16_t cmd_v_timer; return add_shift_on_hold(KC_V, record->event.pressed, cmd_v_timer); }
        case CMD_B: { static uint16_t cmd_b_timer; return add_shift_on_hold(KC_B, record->event.pressed, cmd_b_timer); }

        case CMD_LALT: { static uint16_t cmd_lalt_timer; return add_mod_on_hold(KC_Z, KC_LCTL, record->event.pressed, cmd_lalt_timer); }
        case CMD_LCTL: { static uint16_t cmd_lctl_timer; return add_mod_on_hold(LSFT(KC_Z), KC_LCTL, record->event.pressed, cmd_lctl_timer); }

        case CMD_TAB: { static uint16_t cmd_tab_timer; return force_ctrl_plus_shift_if_held(KC_TAB, record->event.pressed, cmd_tab_timer); }
        case CMD_COMMA: { static uint16_t cmd_comma_timer; return force_ctrl_plus_shift_if_held(KC_COMM, record->event.pressed, cmd_comma_timer); }
        case CMD_DOT: { static uint16_t cmd_dot_timer; return force_ctrl_plus_shift_if_held(KC_DOT, record->event.pressed, cmd_dot_timer); }
        case CMD_H: { static uint16_t cmd_h_timer; return force_ctrl_plus_shift_if_held(KC_H, record->event.pressed, cmd_h_timer); }
        case CMD_BSPC: { static uint16_t cmd_bspc_timer; return force_mod_plus_mod_if_held(KC_BSPC, KC_LALT, KC_LSFT, record->event.pressed, cmd_bspc_timer); }

        case CTRL_ALT_P_UP: { static uint16_t ctrl_alt_p_up_timer; return without_mods(KC_PGUP, record->event.pressed, ctrl_alt_p_up_timer); }
        case CTRL_ALT_P_DN: { static uint16_t ctrl_alt_p_dn_timer; return without_mods(KC_PGDN, record->event.pressed, ctrl_alt_p_dn_timer); }
        case CTRL_ALT_HOME: { static uint16_t ctrl_alt_home_timer; return without_mods(KC_PGDN, record->event.pressed, ctrl_alt_home_timer); }
        case CTRL_ALT_END: { static uint16_t ctrl_alt_end_timer; return without_mods(KC_PGDN, record->event.pressed, ctrl_alt_end_timer); }

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
