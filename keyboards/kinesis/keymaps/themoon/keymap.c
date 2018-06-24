#include "kinesis.h"

#define _______ KC_TRNS
#define XXXXXXX KC_NO

#define QWERTY 0
#define _CMD 1 // command layer
#define _SHIFT_ALT 2 // shift + alt layer

enum kinesis_keycodes {
  CMD_ESC = LT(_CMD, KC_ESC),
  CMD_SPACE = LT(_CMD, KC_SPC),

  L_DEL = LT(_SHIFT_ALT, KC_BSPC),
  R_DEL = LT(_SHIFT_ALT, KC_BSPC),

  SHIFT_ENTER = MT(MOD_LSFT, KC_ENTER),
  SHIFT_TAB = MT(MOD_RSFT, KC_TAB),

  ALT_SLASH = MT(MOD_LALT, KC_SLSH),
  ALT_BSLASH = MT(MOD_RALT, KC_BSLS),

  PALM1 = MT(MOD_LCTL | MOD_LALT, KC_F16),
  PALM2 = MT(MOD_RCTL | MOD_RALT, KC_F17),

  TRASH = LGUI(KC_BSPC),
  SLEEP = LALT(LGUI(KC_EJCT)),
  CTRL_DEL = MT(MOD_LCTL, KC_DEL),
  CTRL_F18 = MT(MOD_RCTL, KC_F18)
};

enum holding_keycodes {
  // HR: replace with another key if held ("2" -> "(" )
  HR_2 = SAFE_RANGE,
  HR_3,
  HR_4,
  HR_5,
  HR_6,
  HR_7,
  HR_8,
  HR_9,
  HR_COMMA,
  HR_DOT,
  HR_LBRAC,
  HR_RBRAC,
  HR_F5,
  HR_ESC,

  // HS: add shift when held ("f1" -> "shift + f1")
  HS_F1,
  HS_F2,
  HS_F3,
  HS_F4,
  HS_F6,
  HS_F7,
  HS_F8,
  HS_F9,
  HS_F10,
  HS_F11,
  HS_F12,

  // !!!========== NOTE: C-prefix codes are to be used only on command layers! These will get command modifier by default (unless specifically noted they won't)
  // CHS: add shift when held
  CHS_LEFT,
  CHS_RIGHT,
  CHS_SPACE,
  CHS_ESC,
  CHS_ENTER,

  // CHRS: replace with another key (for OSX overrides). Add shift if held
  CHRS_LALT, // command + z
  CHRS_LCTL, // f3

  // CHRMS: replace command modifier from layer with another modifier (for OSX overrides). Add shift if held (NO command from layer in any case!)
  CHRMS_TAB, // control + tab
  CHRMS_COMMA, // control + ,
  CHRMS_DOT, // control + .
  CHRMS_H, // control + H
  CHRMS_M, // control + M
  CHRMS_BSPC, // alt + backspace

  SA_PAGE_UP,
  SA_PAGE_DOWN
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
           HR_ESC, HS_F1  ,HS_F2  ,HS_F3  ,HS_F4  ,HR_F5  ,HS_F6  ,HS_F7  ,HS_F8,
           KC_EQL, KC_1   ,HR_2   ,HR_3   ,HR_4   ,HR_5   ,
           KC_GRV, KC_Q   ,KC_W   ,KC_E   ,KC_R   ,KC_T   ,
           KC_CAPS,KC_A   ,KC_S   ,KC_D   ,KC_F   ,KC_G   ,
           XXXXXXX,KC_Z   ,KC_X   ,KC_C   ,KC_V   ,KC_B   ,
                   XXXXXXX,HR_LBRAC,HR_COMMA,HR_RBRAC,
                                           // left thumb keys
			                                    L_DEL,TRASH,
                                                   ALT_SLASH,
                           CMD_ESC, SHIFT_ENTER, CTRL_DEL,
                                     // left palm key
			                         PALM1,
    // right side
    HS_F9  ,HS_F10 ,HS_F11 ,HS_F12 ,KC_PSCR ,KC_SLCK  ,KC_PAUS, KC_FN0, RESET,
	HR_6   ,HR_7   ,HR_8   ,HR_9   ,KC_0   ,KC_MINS,
	KC_Y   ,KC_U   ,KC_I   ,KC_O   ,KC_P   ,XXXXXXX,
	KC_H   ,KC_J   ,KC_K   ,KC_L   ,KC_SCLN,XXXXXXX,
	KC_N   ,KC_M   ,KC_UP  ,HR_DOT ,KC_QUOT,XXXXXXX,
	    	KC_LEFT,KC_DOWN,KC_RGHT,XXXXXXX,
           // right thumb keys
           SLEEP,R_DEL,
           ALT_BSLASH,
           CTRL_F18, SHIFT_TAB, CMD_SPACE,
                                    // right palm key
                                    PALM2
    ),

// command layer - activated when either of cmd keys is held. Mainly OSX-specific overrides
[_CMD] = KEYMAP(
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
          _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,
                   _______,  _______,  CHRMS_COMMA,  _______,
                             CHRMS_BSPC,  _______,
                                       CHRS_LALT,
                    CHS_ESC, CHS_ENTER,  CHRS_LCTL,
                                     _______,
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         CHRMS_H,  _______,  _______,  _______,  _______,  _______,
         _______,  CHRMS_M,  _______,  CHRMS_DOT ,  _______,  _______,
                   CHS_LEFT,  _______,  CHS_RIGHT, _______,
         _______,  CHRMS_BSPC,
         _______,
         _______,  CHRMS_TAB,  CHS_SPACE,
                             _______
    ),

// shift alt layer
[_SHIFT_ALT] = KEYMAP(
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
          _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,
                   _______,  _______,  _______,  _______,
                             _______,  _______,
                                       _______,
                    _______, _______,  _______,
                                     _______,
         _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  _______,  _______,  _______,  _______,
         _______,  _______,  SA_PAGE_UP,  _______,  _______,  _______,
         _______,  _______,  _______,  _______ ,  _______,  _______,
                   _______,  _______,  _______, _______,
         _______,  _______,
         _______,
         _______,  _______,  SA_PAGE_DOWN,
                             _______
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

bool hold_add_mod(uint16_t code, uint16_t mod_to_add, bool pressed, uint16_t this_timer) {
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
  return hold_add_mod(code, KC_LSFT, pressed, this_timer);
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

bool cancel_shift_alt(uint16_t code, bool pressed, uint16_t this_timer) {
  unregister_code(KC_LSFT);
  unregister_code(KC_LALT);
  register_code(code);
  unregister_code(code);
  register_code(KC_LSFT);
  register_code(KC_LALT);
  return false;
}

bool replace_cmd_with_mod_add_shift_on_hold(uint16_t code, uint16_t mod_to_replace_with, bool pressed, uint16_t this_timer) {
  if(pressed) {
      this_timer= timer_read();
  } else {
      unregister_code(KC_LGUI);
      if (timer_elapsed(this_timer) < 150){
          register_code(mod_to_replace_with);
          register_code(code);
          unregister_code(code);
          unregister_code(mod_to_replace_with);
      } else {
          register_code(KC_LSFT);
          register_code(mod_to_replace_with);
          register_code(code);
          unregister_code(code);
          unregister_code(mod_to_replace_with);
          unregister_code(KC_LSFT);
      }
  }
  return false;
}

bool replace_cmd_with_ctrl_add_shift_on_hold(uint16_t code, bool pressed, uint16_t this_timer) {
  return replace_cmd_with_mod_add_shift_on_hold(code, KC_LCTL, pressed, this_timer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HR_2: { static uint16_t hr_2_timer; return replace_on_hold(KC_2, KC_LEFT_PAREN, record->event.pressed, hr_2_timer); }
        case HR_3: { static uint16_t hr_3_timer; return replace_on_hold(KC_3, KC_UNDERSCORE, record->event.pressed, hr_3_timer); }
        case HR_4: { static uint16_t hr_4_timer; return replace_on_hold(KC_4, KC_RIGHT_PAREN, record->event.pressed, hr_4_timer); }
        case HR_5: { static uint16_t hr_5_timer; return replace_on_hold(KC_5, KC_PLUS, record->event.pressed, hr_5_timer); }
        case HR_6: { static uint16_t hr_6_timer; return replace_on_hold(KC_6, KC_EQL, record->event.pressed, hr_6_timer); }
        case HR_7: { static uint16_t hr_7_timer; return replace_on_hold(KC_7, KC_EXCLAIM, record->event.pressed, hr_7_timer); }
        case HR_8: { static uint16_t hr_8_timer; return replace_on_hold(KC_8, KC_MINS, record->event.pressed, hr_8_timer); }
        case HR_9: { static uint16_t hr_9_timer; return replace_on_hold(KC_9, KC_QUESTION, record->event.pressed, hr_9_timer); }
        case HR_LBRAC: { static uint16_t hs_lbrac_timer; return replace_on_hold(KC_LBRC, KC_LEFT_CURLY_BRACE, record->event.pressed, hs_lbrac_timer); }
        case HR_RBRAC: { static uint16_t hs_rbrac_timer; return replace_on_hold(KC_RBRC, KC_RIGHT_CURLY_BRACE, record->event.pressed, hs_rbrac_timer); }
        case HR_COMMA: { static uint16_t hs_comma_timer; return replace_on_hold(KC_COMM, KC_LEFT_ANGLE_BRACKET, record->event.pressed, hs_comma_timer); }
        case HR_DOT: { static uint16_t hs_dot_timer; return replace_on_hold(KC_DOT, KC_RIGHT_ANGLE_BRACKET, record->event.pressed, hs_dot_timer); }
        case HR_F5: { static uint16_t hr_f5_timer; return replace_on_hold(KC_F5, LGUI(KC_COMM), record->event.pressed, hr_f5_timer); }
        case HR_ESC: { static uint16_t hr_esc_timer; return replace_on_hold(KC_ESC, LCTL(KC_F12), record->event.pressed, hr_esc_timer); }

        case HS_F1: { static uint16_t hs_f1_timer; return add_shift_on_hold(KC_F1, record->event.pressed, hs_f1_timer); }
        case HS_F2: { static uint16_t hs_f2_timer; return add_shift_on_hold(KC_F2, record->event.pressed, hs_f2_timer); }
        case HS_F3: { static uint16_t hs_f3_timer; return add_shift_on_hold(KC_F3, record->event.pressed, hs_f3_timer); }
        case HS_F4: { static uint16_t hs_f4_timer; return add_shift_on_hold(KC_F4, record->event.pressed, hs_f4_timer); }
        case HS_F6: { static uint16_t hs_f6_timer; return add_shift_on_hold(KC_F6, record->event.pressed, hs_f6_timer); }
        case HS_F7: { static uint16_t hs_f7_timer; return add_shift_on_hold(KC_F7, record->event.pressed, hs_f7_timer); }
        case HS_F8: { static uint16_t hs_f8_timer; return add_shift_on_hold(KC_F8, record->event.pressed, hs_f8_timer); }
        case HS_F9: { static uint16_t hs_f9_timer; return add_shift_on_hold(KC_F9, record->event.pressed, hs_f9_timer); }
        case HS_F10: { static uint16_t hs_f10_timer; return add_shift_on_hold(KC_F10, record->event.pressed, hs_f10_timer); }
        case HS_F11: { static uint16_t hs_f11_timer; return add_shift_on_hold(KC_F11, record->event.pressed, hs_f11_timer); }
        case HS_F12: { static uint16_t hs_f12_timer; return add_shift_on_hold(KC_F12, record->event.pressed, hs_f12_timer); }

        case CHS_LEFT: { static uint16_t chs_left_timer; return add_shift_on_hold(KC_LEFT, record->event.pressed, chs_left_timer); }
        case CHS_RIGHT:{ static uint16_t chs_right_timer; return add_shift_on_hold(KC_RIGHT, record->event.pressed, chs_right_timer); }
        case CHS_SPACE: { static uint16_t chs_space_timer; return add_shift_on_hold(KC_SPC, record->event.pressed, chs_space_timer); }
        case CHS_ESC: { static uint16_t chs_esc_timer; return add_shift_on_hold(KC_ESC, record->event.pressed, chs_esc_timer); }
        case CHS_ENTER: { static uint16_t chs_enter_timer; return add_shift_on_hold(KC_ENTER, record->event.pressed, chs_enter_timer); }

        case CHRS_LALT: { static uint16_t chrs_lalt_timer; return add_shift_on_hold(KC_Z, record->event.pressed, chrs_lalt_timer); }
        case CHRS_LCTL: { static uint16_t chrs_lctl_timer; return add_shift_on_hold(KC_F3, record->event.pressed, chrs_lctl_timer); }

        case CHRMS_TAB: { static uint16_t chrms_tab_timer; return replace_cmd_with_ctrl_add_shift_on_hold(KC_TAB, record->event.pressed, chrms_tab_timer); }
        case CHRMS_COMMA: { static uint16_t chrms_comma_timer; return replace_cmd_with_ctrl_add_shift_on_hold(KC_COMM, record->event.pressed, chrms_comma_timer); }
        case CHRMS_DOT: { static uint16_t chrms_dot_timer; return replace_cmd_with_ctrl_add_shift_on_hold(KC_DOT, record->event.pressed, chrms_dot_timer); }
        case CHRMS_H: { static uint16_t chrms_h_timer; return replace_cmd_with_ctrl_add_shift_on_hold(KC_H, record->event.pressed, chrms_h_timer); }
        case CHRMS_M: { static uint16_t chrms_m_timer; return replace_cmd_with_ctrl_add_shift_on_hold(KC_M, record->event.pressed, chrms_m_timer); }
        case CHRMS_BSPC: { static uint16_t chrms_bspc_timer; return replace_cmd_with_mod_add_shift_on_hold(KC_BSPC, KC_LALT, record->event.pressed, chrms_bspc_timer); }

        case SA_PAGE_UP: { static uint16_t sa_page_up_timer; return cancel_shift_alt(KC_PGUP, record->event.pressed, sa_page_up_timer); }
        case SA_PAGE_DOWN: { static uint16_t sa_page_down_timer; return cancel_shift_alt(KC_PGDN, record->event.pressed, sa_page_down_timer); }

        default:
            return true;
    }
}

void led_set_user(uint8_t usb_led) {
}

uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    case _CMD:
      register_code(KC_LGUI);
      break;
    case _SHIFT_ALT:
      register_code(KC_LSFT);
      register_code(KC_LALT);
      break;
    default:
      unregister_code(KC_LGUI);
      unregister_code(KC_LSFT);
      unregister_code(KC_LALT);
      break;
    }
return state;
}
