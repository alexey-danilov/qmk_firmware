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
  CMD_SPACE = LT(_COMMAND_SPACE, KC_SPC),
  ALT_SHIFT_BS = LT(_ALT_SHIFT_BS, KC_BSPC),
  CMD_ESC = MO(_COMMAND_ESCAPE),
  ALT_SLASH_MAC = MO(_ALT_SLASH_MAC),
  ALT_BSLASH_MAC = MO(_ALT_BSLASH_MAC),
  SHIFT_ENTER_MAC = MO(_SHIFT_ENTER_MAC),
  SHIFT_TAB_MAC = MO(_SHIFT_TAB_MAC),
  CTRL_DEL = MO(_CTRL_DEL),
  CTRL_F1 = MO(_CTRL_F1),
  PALM_L_MAC = MO(_PALM_L_MAC),
  PALM_R_MAC = MO(_PALM_R_MAC),

  // win
  CTRL_SPACE = LT(_CONTROL_SPACE, KC_SPC),
  CTRL_SHIFT_BS = LT(_CTRL_SHIFT_BS, KC_BSPC),
  CTRL_ESC = MO(_CONTROL_ESCAPE),
  ALT_SLASH_WIN = MO(_ALT_SLASH_WIN),
  ALT_BSLASH_WIN = MO(_ALT_BSLASH_WIN),
  SHIFT_ENTER_WIN = MO(_SHIFT_ENTER_WIN),
  SHIFT_TAB_WIN = MO(_SHIFT_TAB_WIN),
  CTRL_ALT_DEL = MO(_CTRL_ALT_DEL),
  PALM_L_WIN = MO(_PALM_L_WIN),
  PALM_R_WIN = MO(_PALM_R_WIN),

  // firmware-related stuff
  KEYB_CONTROL = MO(_KEYB_CONTROL)
};

enum holding_keycodes {
  _ENTER = SAFE_RANGE,
  _DEL,
  _TAB,
  _SPACE,
  _ESC,
  _BSLS,
  _INS,
  _BSPC,
  _NUBS,
  _APP,

  _1,
  _2_PLEFT,
  _3_SLASH,
  _4_PRGHT,
  _5_EQL,
  _6_PLUS,
  _7_BANG,
  _8_DASH,
  _9_QUEST,
  _0,

  _Z,
  _X,
  _C,
  _V,
  _F,

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

  HIDE_FOCUS_MAC,
  HIDE_FOCUS_WIN,

  SET_LAYER_MAC,
  SET_LAYER_WIN,

  // mac-specific overrides
  CTRL_DOT,
  CTRL_COMM,
  CTRL_H,
  CTRL_M,

  LANG_CAPS_MAC,
  LANG_CAPS_WIN,

  // LWin + key overrides
  W_F9, W_F10, W_F11, W_F12,
  W_6, W_7, W_8, W_9, W_0,
  W_Y, W_U, W_I, W_O, W_P,
  W_H, W_J, W_K, W_L, W_SCLN,
  W_N, W_M, W_DOT, W_QUOT,
  W_EQL, W_MINS
};

static uint16_t esc_timer = 0; // timer for leader key: esc
static uint16_t lead_timer = 0; // timer for leader key
static bool default_layer = true;

// HELPER FUNCTIONS
// switch mac <-> win
static bool isMac;
static bool isWin;
static bool caps_led;
static bool lang_switch_led;
static bool lead_led;
static bool init_complete;

static bool is_macro1_recording = false;
static bool is_macro2_recording = false;

static uint16_t m1[1024];
static uint16_t m2[1024];
static uint16_t pointer1 = 0;
static uint16_t pointer2 = 0;
static uint16_t down_ = 999;
static uint16_t up_ = 1000;

void keyboard_post_init_user(void) {
  debug_enable=true;
}

bool isMacroKey(uint16_t key) {
    if (!is_macro1_recording && !is_macro2_recording) {
      return false;
    }
    switch (key) {
        case KC_Q:    { dprint("\nKC_Q");    return true; }
        case KC_W:    { dprint("\nKC_W");    return true; }
        case KC_E:    { dprint("\nKC_E");    return true; }
        case KC_R:    { dprint("\nKC_R");    return true; }
        case KC_T:    { dprint("\nKC_T");    return true; }
        case KC_Y:    { dprint("\nKC_Y");    return true; }
        case KC_U:    { dprint("\nKC_U");    return true; }
        case KC_I:    { dprint("\nKC_I");    return true; }
        case KC_O:    { dprint("\nKC_O");    return true; }
        case KC_P:    { dprint("\nKC_P");    return true; }
        case KC_A:    { dprint("\nKC_A");    return true; }
        case KC_S:    { dprint("\nKC_S");    return true; }
        case KC_D:    { dprint("\nKC_D");    return true; }
        case KC_F:    { dprint("\nKC_F");    return true; }
        case KC_G:    { dprint("\nKC_G");    return true; }
        case KC_H:    { dprint("\nKC_H");    return true; }
        case KC_J:    { dprint("\nKC_J");    return true; }
        case KC_K:    { dprint("\nKC_K");    return true; }
        case KC_L:    { dprint("\nKC_L");    return true; }
        case KC_Z:    { dprint("\nKC_Z");    return true; }
        case KC_X:    { dprint("\nKC_X");    return true; }
        case KC_C:    { dprint("\nKC_C");    return true; }
        case KC_V:    { dprint("\nKC_V");    return true; }
        case KC_B:    { dprint("\nKC_B");    return true; }
        case KC_N:    { dprint("\nKC_N");    return true; }
        case KC_M:    { dprint("\nKC_M");    return true; }
        case KC_LBRC: { dprint("\nKC_LBRC"); return true; }
        case KC_RBRC: { dprint("\nKC_RBRC"); return true; }
        case KC_COMM: { dprint("\nKC_COMM"); return true; }
        case KC_SCLN: { dprint("\nKC_SCLN"); return true; }
        case KC_QUOT: { dprint("\nKC_QUOT"); return true; }
        case KC_GRV:  { dprint("\nKC_GRV");  return true; }
        case KC_DOT:  { dprint("\nKC_DOT");  return true; }
        case KC_MINS: { dprint("\nKC_MINS"); return true; }
        case KC_EQL:  { dprint("\nKC_EQL");  return true; }
        case KC_SLSH: { dprint("\nKC_SLSH"); return true; }
        case KC_BSLS: { dprint("\nKC_BSLS"); return true; }
        case KC_SPC:  { dprint("\nKC_SPC");  return true; }
        case KC_BSPC: { dprint("\nKC_BSPC"); return true; }
        case KC_ENTER:{ dprint("\nKC_ENTER");return true; }
        case KC_TAB:  { dprint("\nKC_TAB");  return true; }
        case KC_DEL:  { dprint("\nKC_DEL");  return true; }
        case KC_1:    { dprint("\nKC_1");    return true; }
        case KC_2:    { dprint("\nKC_2");    return true; }
        case KC_3:    { dprint("\nKC_3");    return true; }
        case KC_4:    { dprint("\nKC_4");    return true; }
        case KC_5:    { dprint("\nKC_5");    return true; }
        case KC_6:    { dprint("\nKC_6");    return true; }
        case KC_7:    { dprint("\nKC_7");    return true; }
        case KC_8:    { dprint("\nKC_8");    return true; }
        case KC_9:    { dprint("\nKC_9");    return true; }
        case KC_0:    { dprint("\nKC_0");    return true; }
        case KC_LGUI: { dprint("\nKC_LGUI"); return true; }
        case KC_RGUI: { dprint("\nKC_RGUI"); return true; }
        case KC_LALT: { dprint("\nKC_LALT"); return true; }
        case KC_RALT: { dprint("\nKC_RALT"); return true; }
        case KC_LSFT: { dprint("\nKC_LSFT"); return true; }
        case KC_RSFT: { dprint("\nKC_RSFT"); return true; }
        case KC_LCTL: { dprint("\nKC_LCTL"); return true; }
        case KC_RCTL: { dprint("\nKC_RCTL"); return true; }
        case KC_F1:   { dprint("\nKC_F1");   return true; }
        case KC_F2:   { dprint("\nKC_F2");   return true; }
        case KC_F3:   { dprint("\nKC_F3");   return true; }
        case KC_F4:   { dprint("\nKC_F4");   return true; }
        case KC_F5:   { dprint("\nKC_F5");   return true; }
        case KC_F6:   { dprint("\nKC_F6");   return true; }
        case KC_F7:   { dprint("\nKC_F7");   return true; }
        case KC_F8:   { dprint("\nKC_F8");   return true; }
        case KC_F9:   { dprint("\nKC_F9");   return true; }
        case KC_F10:  { dprint("\nKC_F10");  return true; }
        case KC_F11:  { dprint("\nKC_F11");  return true; }
        case KC_F12:  { dprint("\nKC_F12");  return true; }
        case KC_ESC:  { dprint("\nKC_ESC");  return true; }

        case KC_NO:   { dprint("\nKC_NO");                return false; }
        case 1000:    { dprint("\nKC_Event_UP");          return false; }
        case 999:     { dprint("\nKC_Event_DOWN");        return false; }
        default:      { dprintf("\nUnknown key %d", key); return false; }
        }
}

void eraseM1(void) {
  dprint("\n!!! Erasing macro buffer 1");
  for (uint16_t i = 0; i < 1024; i++) {
    m1[i] = 0;
    pointer1 = 0;
  }
}

void eraseM2(void) {
  dprint("\n!!! Erasing macro buffer 2");
  for (uint16_t i = 0; i < 1024; i++) {
    m2[i] = 0;
    pointer2 = 0;
  }
}

void playM1(void) {
   dprint("\n... Printing macro buffer 1: ");
   for (uint16_t i = 0; i < 1024; ) {
     uint16_t event = m1[i];
     if (event == KC_NO) { dprintf("\n >>> Reached end of macro 1 at %d", i); break; }
     i++;
     if (i >= 1024) { dprintf("\n >>> Reached end of macro 1 at %d", i); break; }
     uint16_t key = m1[i];
     i++;
     if (i >= 1024) { dprintf("\n >>> Reached end of macro 1 at %d", i); break; }
     if (key == KC_NO) { dprintf("\n >>> Reached end of macro 1 at %d", i); break; }

     if (event == down_) { register_code(key); } else { unregister_code(key); }
   }
   clear_keyboard();
   layer_clear();
   dprint("\n... Done printing macro buffer 1\n");
}

void playM2(void) {
   dprint("\n... Printing macro buffer 2: ");
   for (uint16_t i = 0; i < 1024; ) {
     uint16_t event = m2[i];
     if (event == KC_NO) { dprintf("\n >>> Reached end of macro 2 at %d", i); break; }
     i++;
     if (i >= 1024) { dprintf("\n >>> Reached end of macro 2 at %d", i); break; }
     uint16_t key = m2[i];
     i++;
     if (i >= 1024) { dprintf("\n >>> Reached end of macro 2 at %d", i); break; }
     if (key == KC_NO) { dprintf("\n >>> Reached end of macro 2 at %d", i); break; }

     if (event == down_) { register_code(key); } else { unregister_code(key); }
   }
   clear_keyboard();
   layer_clear();
   dprint("\n... Done printing macro buffer 2\n");
}

void printM1(void) {
  for (uint16_t i = 0; i < 1024; i++) {
    dprintf("\nmacro 1: %d == ", i); isMacroKey(m1[i]);
  }
}

void printM2(void) {
  for (uint16_t i = 0; i < 1024; i++) {
    dprintf("\nmacro 2: %d == ", i); isMacroKey(m2[i]);
  }
}

void addToMacro(uint16_t key, bool down) {
  if (!is_macro1_recording && !is_macro2_recording) {
    dprint("\nMacro is not recording, ignoring key\n");
    return;
  }
  bool macroKey = isMacroKey(key);
  if (!macroKey) {
      dprint("\nNot a macro key, ignoring\n");
      return;
  }

   dprintf("\nM1 recording: %d, pointer %d; M2 recording: %d, pointer %d", is_macro1_recording, pointer1, is_macro2_recording, pointer2);
  if (is_macro1_recording) {
    if ((pointer1 + 2) < 1024) {
        dprint("\n >>> Adding macro 1 key");
        if (down) { m1[pointer1] = down_; dprint("\n<<< Added macro 1 down_"); }
        else { m1[pointer1] = up_; dprint("\n<<< Added macro 1 up_"); }
        pointer1++;
        m1[pointer1] = key;
        dprint("\n<<< Added macro 1 key");
        pointer1++;
    } else {
      dprint("\n!!! Macro buffer 1 full");
    }
  } else if (is_macro2_recording) {
    if ((pointer2 + 2) < 1024) {
        dprint("\n >>> Adding macro 2 key");
        if (down) { m2[pointer2] = down_; dprint("\n<<< Added macro 2 down_"); }
        else { m2[pointer2] = up_; dprint("\n<<< Added macro 2 up_"); }
        pointer2++;
        m2[pointer2] = key;
        dprint("\n<<< Added macro 2 key");
        pointer2++;
    } else {
      dprint("\n!!! Macro buffer 2 full");
    }
  }
//   dprint("\n??? Printing macro buffer");
//   printM1();
//   dprint("\n??? Printing macro buffer done \n");
}

void down(uint16_t key) { register_code(key); addToMacro(key, true); }
void up(uint16_t key) { unregister_code(key); addToMacro(key, false); }
void key_code(uint16_t key) { down(key); up(key); }

void with_1mod(uint16_t key, uint16_t mod1) {
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

void remove_mods(void) {
    uint8_t current_mods = get_mods();
    if (current_mods & (MOD_BIT(KC_LCTL))) { up(KC_LCTL); }
    if (current_mods & (MOD_BIT(KC_RCTL))) { up(KC_RCTL); }
    if (current_mods & (MOD_BIT(KC_LGUI))) { up(KC_LGUI); }
    if (current_mods & (MOD_BIT(KC_RGUI))) { up(KC_RGUI); }
    if (current_mods & (MOD_BIT(KC_LALT))) { up(KC_LALT); }
    if (current_mods & (MOD_BIT(KC_RALT))) { up(KC_RALT); }
    if (current_mods & (MOD_BIT(KC_LSFT))) { up(KC_LSFT); }
    if (current_mods & (MOD_BIT(KC_RSFT))) { up(KC_RSFT); }
}

bool pressed_within(uint16_t hold_timer, uint16_t hold_duration) {
 return timer_elapsed(hold_timer) < hold_duration;
}

void led_red_on(void) {
  PORTF &= ~(1<<3);
}

void led_red_off(void) {
  PORTF |= (1<<3);
}

void led_yellow_on(void) {
  PORTF &= ~(1<<2);
}

void led_yellow_off(void) {
  PORTF |= (1<<2);
}

void led_green_on(void) {
  PORTF &= ~(1<<1);
}

void led_green_off(void) {
  PORTF |= (1<<1);
}

void led_blue_on(void) {
  PORTF &= ~(1<<0);
}

void led_blue_off(void) {
  PORTF |= (1<<0);
}

void switch_lead_led_on(void) {
  if (!lead_led) {
    led_red_on();
    led_yellow_on();
    led_green_on();
    lead_led = true;
  }
}

void switch_lead_led_off(void) {
  if (lead_led) {
    if (!is_macro1_recording) { led_red_off(); }
    if (!is_macro2_recording) { led_green_off(); }
    led_yellow_off();
    lead_led = false;
  }
}

void all_leds_on(void) {
  led_red_on(); led_yellow_on(); led_green_on(); led_blue_on();
}

void all_leds_off(void) {
     if (!is_macro1_recording) { led_red_off(); }
     led_yellow_off();
     if (!is_macro2_recording) { led_green_off(); }
     if(!caps_led) { led_blue_off(); }
}

// lang/caps, with led indication
bool process_lang_caps(
    uint16_t lang_switch_code,
    uint16_t mod_to_be_replaced,
    uint16_t lang_switch_mod1,
    uint16_t lang_switch_mod2,
    bool pressed,
    uint16_t hold_duration
) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      up(mod_to_be_replaced);

      if (pressed_within(hold_timer, hold_duration)){
          lang_switch_led = true;
          if (isMac) {
             caps_led = false; // on mac changing language resets caps lock
          }
          with_2_mods(lang_switch_code, lang_switch_mod1, lang_switch_mod2);
      } else {
          if (caps_led) {
            up(KC_LCAP);
            caps_led = false;
          } else {
            down(KC_LCAP);
            caps_led = true;
          }
      }
  }
  return false;
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

bool replace_ctrl_alt_with_lgui(uint16_t code, bool pressed) {
  return replace_key_and_mods_if_held_replace_key_and_mods(code, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, code, KC_LGUI, KC_NO, KC_NO, KC_NO, pressed, 150, true);
}

// replaces keycode and adds mod to it if it was held for at least provided duration
bool replace_if_held_add_mods(uint16_t code, uint16_t mod, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint16_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (pressed_within(hold_timer, hold_duration)){
          with_1mod(code, mod);
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
         remove_mods();
      }
  }
  return false;
}

// adds shift to keycode if it was held for at least 150 ms
bool if_held_150_add_shift(uint16_t code, bool pressed) {
  return if_held_add_mods(code, KC_LSFT, KC_NO, pressed, 150);
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
     with_1mod(code, mod);

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
    bool pressed,
    uint16_t hold_duration,
    bool bring_mods_back) {

  if (pressed) {
    *interrupted_flag = false;
    *layer_timer = timer_read();
  } else {
    if (pressed_within(*layer_timer, hold_duration)) {
      if (!*interrupted_flag) {
        up(layer_mod1); up(layer_mod2); up(layer_mod3); up(layer_mod4); // unregister mods associated with the layer, so that they don't intefere with the tap key
        with_1mod(tap_key, tap_mod); // register tap key and its mod
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
          with_1mod(code, mod_to_add);
      } else {
          with_2_mods(held_code, held_mod1, held_mod2); key_code(code);
      }
      down(mod_to_remove);
  }
  return false;
}

// ESC AS A LEADER KEY
// provides functionality similar to "leader key", except that it works for escape
bool following_custom_leader_key(uint16_t code, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t *leader_timer, bool pressed, uint16_t leader_last_pressed_timeout) {
  if (*leader_timer && pressed_within(*leader_timer, leader_last_pressed_timeout)) {
    if (pressed) {
      *leader_timer = 0;
      with_3_mods(code, mod1, mod2, mod3);
      return false;
    }
  }
  return true;
}

// CMD/CTRL + SPACE AS A LEADER KEY
bool press_leader_key(bool pressed) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (pressed_within(hold_timer, 150)){
          up(KC_LGUI); up(KC_LCTL);
          lead_timer = timer_read();
          switch_lead_led_on();
      } else {
          with_1mod(KC_SPC, KC_LSFT);
          lead_timer = 0;
          switch_lead_led_off();
      }
  }
  return false;
}

bool lead_impl(uint16_t code, uint16_t os_mod, uint16_t additional_mod, bool pressed) {
  if (lead_timer) {
    if (pressed_within(lead_timer, 1000)) {
      if (pressed) {
        lead_timer = 0;
        with_3_mods(code, os_mod, KC_LSFT, additional_mod);
        return true;
      }
    }
  }
  return false;
}

bool lead_replace_if_held_add_mods(uint16_t code, uint16_t mod, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool *was_lead, bool pressed, uint16_t hold_duration) {
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
          with_1mod(code, mod);
      } else {
          with_2_mods(held_code, held_mod1, held_mod2);
      }
      *was_lead = false;
      pressed_mods = 0;
      if (default_layer) {
         remove_mods();
      }
  }
  return false;
}

bool is_after_lead(uint16_t code, bool pressed) {
  return lead_impl(code, isMac ? KC_LGUI : KC_LCTL, KC_LALT, pressed);
}

bool lead_custom_autoshifted(uint16_t code, uint16_t held_code, uint16_t held_mod, bool pressed) {
  static bool was_lead;
  if (lead_impl(code, isMac ? KC_LGUI: KC_LCTL, KC_LALT, pressed)) {
     was_lead = true;
     return false;
  };
  return lead_replace_if_held_add_mods(code, KC_NO, held_code, held_mod, KC_NO, &was_lead, pressed, 150);
}

bool lead_autoshifted_same_key(uint16_t code, bool pressed) {
  return lead_custom_autoshifted(code, code, KC_LSFT, pressed);
}

bool lead_f(uint16_t code, bool pressed) {
  static bool was_lead;
  if (is_after_lead(code, pressed)) {
     was_lead = true;
     return false;
  }
  return lead_replace_if_held_add_mods(code, KC_NO, code, KC_LSFT, KC_NO, &was_lead, pressed, 150);
}

enum {
  TAP_MACRO = 0,
  F5_TD = 1,
  REST_TD = 2
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

//**************** F5 TAP *********************//
static tap f5_tap_state = { .is_press_action = true, .state = 0 };

void f5_finished (qk_tap_dance_state_t *state, void *user_data) {
  f5_tap_state.state = cur_dance(state);
  if (!is_after_lead(KC_F5, true)) {
    switch (f5_tap_state.state) {
      case SINGLE_TAP: key_code(KC_F5); break;
      case SINGLE_HOLD: down(KC_LSFT); key_code(KC_F5); up(KC_LSFT); break;
      case DOUBLE_TAP:
        if (isMac) { down(KC_LGUI); key_code(KC_COMM); up(KC_LGUI); break; }
        else if (isWin) { key_code(KC_PAUS); break; }
      case DOUBLE_HOLD:
        if (isMac) { down(KC_LCTL); key_code(KC_F12); up(KC_LCTL); break; }
        else if (isWin) { key_code(KC_PSCR); break; }
      case DOUBLE_SINGLE_TAP: key_code(KC_F5); key_code(KC_F5); break;
    }
  }
}

void f5_reset (qk_tap_dance_state_t *state, void *user_data) {
  f5_tap_state.state = 0;
}

//**************** REST TAP *********************//
static tap rest_tap_state = { .is_press_action = true, .state = 0 };

void rest_finished (qk_tap_dance_state_t *state, void *user_data) {
  rest_tap_state.state = cur_dance(state);
    switch (rest_tap_state.state) {
      case SINGLE_TAP:
          // sleep
          if (isMac) {
            all_leds_on(); _delay_ms(125); all_leds_off(); _delay_ms(200); all_leds_on(); _delay_ms(125); all_leds_off();
            down(KC_LCTL); down(KC_LSFT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LSFT); up(KC_LCTL); break;
          }
          if (isWin) {
            with_1mod(KC_X, KC_LGUI); all_leds_on(); _delay_ms(125); all_leds_off(); key_code(KC_U); _delay_ms(200); all_leds_on(); _delay_ms(125); all_leds_off(); key_code(KC_S); break;
          }
      case DOUBLE_TAP:
         // shutdown
         all_leds_on();

         if (isMac) {
            _delay_ms(300);
            led_blue_off(); _delay_ms(300);
            led_green_off(); _delay_ms(300);
            led_yellow_off(); _delay_ms(300);
            led_red_off();
            down(KC_LGUI); down(KC_LCTL); down(KC_LALT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LALT); up(KC_LCTL); up(KC_LGUI); break;
         }
         if (isWin) {
            with_1mod(KC_X, KC_LGUI); _delay_ms(700); led_blue_off(); key_code(KC_U); _delay_ms(700); led_green_off(); _delay_ms(700); led_yellow_off(); _delay_ms(700); led_red_off(); _delay_ms(700); key_code(KC_U); _delay_ms(700); break;
         }
      default: break;
    }
}

void rest_reset (qk_tap_dance_state_t *state, void *user_data) {
  rest_tap_state.state = 0;
}

// dynamic macro
static tap dynamic_macro_state = { .is_press_action = true, .state = 0 };

void dynamic_macro_finished (qk_tap_dance_state_t *state, void *user_data) {
  dynamic_macro_state.state = cur_dance(state);
  if (!is_after_lead(isMac ? KC_NUBS : KC_APP, true)) {

    if (is_macro1_recording || is_macro2_recording) {
       dprint("\nStopping record of macros");
       is_macro1_recording = false;
       is_macro2_recording = false;
       if (!lead_led) { led_red_off(); led_green_off(); };

    } else {
      switch (dynamic_macro_state.state) {
          case SINGLE_TAP: playM1(); break;
          case DOUBLE_TAP: playM2(); break;

          case SINGLE_HOLD:
              eraseM1();
              is_macro1_recording = true;
              dprint("\nStarting record of macro 1");
              led_red_on();
          break;

          default:
              eraseM2();
              is_macro2_recording = true;
              dprint("\nStarting record of macro 2");
              led_green_on();
          break;
        }
    }
  }
}

void dynamic_macro_reset (qk_tap_dance_state_t *state, void *user_data) {
  dynamic_macro_state.state = 0;
}

// all tap macros
qk_tap_dance_action_t tap_dance_actions[] = {
  [TAP_MACRO] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, dynamic_macro_finished, dynamic_macro_reset, 350),
  [F5_TD] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, f5_finished, f5_reset, 350),
  [REST_TD] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, rest_finished, rest_reset, 350)
};

/*
* ,-------------------------------------------------------------------------------------------------------------------.
* |  `~    | F1  |  F2  |  F3  |  F4  | F5_TD |  F6 |  F8  |  F9  |  F10  |  F12 | Mute | Vol- | Vol+ | Prog |  Rest  |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* |  F17   |   1  |  2(  |  3_  |  4)  |  5=  |                           |  6+  |  7!  |  8-  |  9?  |  0)  |  F21   |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* |  F18   |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |  I   |   O  |  P   |  F22   |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |  F19   |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |  K   |   L  |  ;:  |  F23   |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |  F20   |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  |  F24   |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |  F13 |  [{  |  ,<  |  ]}  |                                        | Left | Down | Right |  F16 |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,---------------------------.
*                            | Select word/Bspc | Macro|         |Insert|     Backspace      |
*                            `-----------|------|------|         |------+------+-------------`
*                                 |      |      | Alt//|         | Alt/\|      |      |
*                                 | LGui/|Shift/|------|         |------|Shift/|RGui/ |
*                                 | ESC  |Enter | Ctrl/|         | Ctrl/|Tab   |SPACE |
*                                 |      |      | Del  |         | Apps |      |      |
*                                  --------------------           --------------------
*
*                      ----------                                                         ----------
*                     | PALM/F14 |                                                       | PALM/F15 |
*                      ----------                                                         ----------
*/

// base mac layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_MAC] = LAYOUT(
           // left side
           KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, TD(F5_TD), KC_F6, KC_F7, KC_F8,
           KC_F17, _1, _2_PLEFT, _3_SLASH, _4_PRGHT, _5_EQL,
           KC_F18, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_F19,KC_A, KC_S, KC_D, KC_F, KC_G,
           KC_F20, KC_Z, KC_X, KC_C, KC_V, KC_B,
                 KC_F13, KC_LBRC, KC_COMM, KC_RBRC,
                                               // left thumb keys
			                                      ALT_SHIFT_BS, TD(TAP_MACRO),
                                                   ALT_SLASH_MAC,
                              CMD_ESC, SHIFT_ENTER_MAC, CTRL_DEL,
                                                 // left palm key
			                                              PALM_L_MAC,
    // right side
  KC_F9, KC_F10, KC_F11, KC_F12, KC__MUTE, KC__VOLDOWN, KC__VOLUP, KEYB_CONTROL, TD(REST_TD),
	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0, KC_F21,
	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_F22,
	KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_F23,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F24,
	KC_LEFT, KC_DOWN, KC_RGHT, KC_F16,
           // right thumb keys
           _INS, KC_BSPC,
           ALT_BSLASH_MAC,
           CTRL_F1, SHIFT_TAB_MAC, CMD_SPACE,
           // right palm key
           PALM_R_MAC
    ),

// mac-specific layers
[_COMMAND_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, _F, __________,
         __________, _Z, _X, _C, _V, __________,
               __________,  __________,  __________,  __________,
                                         _,  _,
                                       KC_LALT,
                     CMD_ESC, KC_LSFT, KC_LCTL,
                                    __________,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         CTRL_H, __________,  __________,  __________,  __________,  __________,
         __________, CTRL_M, SELECT_UP_MAC, CTRL_DOT, __________, __________,
                   SELECT_LEFT_MAC,  SELECT_DOWN_MAC,  SELECT_RIGHT_MAC, __________,
         _INS, DEL_LEFT_MAC,
         KC_EQL,
         KC_MINS, LANG_CAPS_MAC, LEAD_SPACE,
         KC_F15
    ),

[_COMMAND_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  CTRL_COMM,  __________,
                                DEL_LEFT_MAC, _NUBS,
                                            KC_SLSH,
                       KC_F1, _ENTER, DEL_RIGHT_MAC,
                                             KC_F14,
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
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
         _INS, LCTL(KC_BSPC),
         KC_EQL,
         KC_MINS, _TAB, KC_F3,
         KC_F15
    ),

[_ALT_BSLASH_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                LCTL(KC_BSPC), _NUBS,
                                             KC_SLSH,
                          _ESC, _ENTER, LCTL(KC_DEL),
                                              KC_F14,
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
         __________,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                  _BSPC, _NUBS,
                                       KC_SLSH,
                            _ESC, _ENTER, _DEL,
                                        KC_F14,
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
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
         _INS, _BSPC,
         KC_EQL,
         KC_MINS, _TAB, _SPACE,
         KC_F15
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
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
                   __________,  __________,  __________, __________,
         _INS, _BSPC,
         _BSLS,
         KC_MINS, _TAB, _SPACE,
         KC_F15
    ),

[_SHIFT_TAB_MAC] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                             _BSPC, _NUBS,
                                  KC_SLSH,
                       _ESC, _ENTER, _DEL,
                                   KC_F14,
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
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                               __________,  __________,
                                            __________,
                   __________, __________,  __________,
                                          PALM_L_MAC,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         __________,  CMD_SHIFT_TAB, CMD_Q, CMD_TAB,  __________,  __________,
         __________,  __________, KC_PGUP, KC_SPC,  __________,  __________,
            HOME_, KC_PGDN, END_, __________,
         _INS, _BSPC,
         KC_EQL,
         KC_MINS, FIND_PREV, FIND_NEXT,
         HIDE_FOCUS_MAC
    ),

[_PALM_R_MAC] = LAYOUT(
         HYPR(KC_GRV), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), HYPR(KC_F4), HYPR(KC_F5), HYPR(KC_F6), HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_F13), HYPR(KC_LBRC), HYPR(KC_COMM), HYPR(KC_RBRC),
                                        KC_MRWD, KC_MFFD,
                                                 KC_MPLY,
              LGUI(KC_Z), LGUI(LSFT(KC_Z)), HYPR(KC_DEL),
                                            HYPR(KC_F14),
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   LGUI(KC_HOME),  _, LGUI(KC_END), _,
         _,  _,
         _,
         _,  _,  _,
         PALM_R_MAC
    ),

// base win layer
[_WIN] = LAYOUT(
           // left side
           KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, TD(F5_TD), KC_F6, KC_F7, KC_F8,
           KC_F17, _1, _2_PLEFT, _3_SLASH, _4_PRGHT, _5_EQL,
           KC_F18, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_F19, KC_A, KC_S, KC_D, KC_F, KC_G,
           KC_F20, KC_Z, KC_X, KC_C, KC_V, KC_B,
                KC_F13, KC_LBRC, KC_COMM, KC_RBRC,
                                                 // left thumb keys
			                                CTRL_SHIFT_BS, TD(TAP_MACRO),
                                                     ALT_SLASH_WIN,
                           CTRL_ESC, SHIFT_ENTER_WIN, CTRL_ALT_DEL,
                                                   // left palm key
			                                                PALM_L_WIN,
    // right side
    KC_F9, KC_F10, KC_F11, KC_F12, KC_MUTE, KC_VOLD, KC_VOLU, KEYB_CONTROL, TD(REST_TD),
  	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0, KC_F21,
  	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_F22,
  	KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_F23,
  	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F24,
  	KC_LEFT, KC_DOWN, KC_RGHT, KC_F16,
           // right thumb keys
           _INS, KC_BSPC,
           ALT_BSLASH_WIN,
           KC_RGUI, SHIFT_TAB_WIN, CTRL_SPACE,
           // right palm key
           PALM_R_WIN
    ),

// win-specific layers
[_CONTROL_ESCAPE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, _F, __________,
         __________, _Z, _X, _C, _V, __________,
               __________,  __________,  __________,  __________,
                                         _,  _,
                                       KC_LALT,
                    CTRL_ESC, KC_LSFT, KC_LGUI,
                                    __________,

         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________,  __________,  SELECT_UP_WIN,  __________,  __________,  __________,
         SELECT_LEFT_WIN,  SELECT_DOWN_WIN,  SELECT_RIGHT_WIN, __________,
         _INS, DEL_LEFT_WIN,
         KC_EQL,
         KC_MINS, LANG_CAPS_WIN, LEAD_SPACE,
         KC_F15
    ),

[_CONTROL_SPACE] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                               DEL_LEFT_WIN, _APP,
                                          KC_SLSH,
                     KC_F1, _ENTER, DEL_RIGHT_WIN,
                                           KC_F14,
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
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
                  __________, __________,  __________, __________,
         _INS, _BSPC,
         KC_EQL,
         KC_MINS, KC_F2, KC_F3,
         KC_F15
    ),

[_ALT_BSLASH_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                     _BSPC, _APP,
                                         KC_SLSH,
                              _ESC, _ENTER, _DEL,
                                          KC_F14,
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
                                     _,
         W_F9,  W_F10,  W_F11,  W_F12,  __________,  __________, __________, __________, __________,
         W_6,  W_7,  W_8,  W_9,  W_0,  __________,
         W_Y,  W_U,  W_I,  W_O,  W_P,  __________,
         W_H,  W_J,  W_K, W_L,  W_QUOT,  __________,
         W_N, W_M, KC_UP, W_DOT, W_SCLN,  __________,
         KC_LEFT,  KC_DOWN, KC_RGHT, __________,
         _INS, _BSPC,
         W_EQL,
         W_MINS, KC_F2, KC_F3,
         KC_F15
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
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
                   __________,  __________,  __________, __________,
         _INS, _BSPC,
         _BSLS,
         KC_MINS, _TAB, _SPACE,
         KC_F15
    ),


[_SHIFT_TAB_WIN] = LAYOUT(
         __________,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                     _BSPC, _APP,
                                         KC_SLSH,
                              _ESC, _ENTER, _DEL,
                                          KC_F14,
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
          __________, __________, __________, __________, __________, __________,
          __________, __________, __________, __________, __________, __________,
          __________, __________, __________, __________, __________, __________,
                __________,  __________,  __________,  __________,
                             __________,  __________,
                                     __________,
                      __________, __________,  __________,
                                     PALM_L_WIN,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         __________, ALT_SHIFT_TAB, ALT_F4, ALT_TAB,  __________,  __________,
         __________, __________, KC_PGUP, __________,  __________,  __________,
                      CTRL_HOME, KC_PGDN, CTRL_END, __________,
         _INS, _BSPC,
         KC_EQL,
         KC_MINS, FIND_PREV, FIND_NEXT,
         HIDE_FOCUS_WIN
    ),

[_PALM_R_WIN] = LAYOUT(
         HYPR(KC_GRV), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), HYPR(KC_F4), HYPR(KC_F5), HYPR(KC_F6), HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_F13), HYPR(KC_LBRC), HYPR(KC_COMM), HYPR(KC_RBRC),
                                        KC_MPRV, KC_MNXT,
                                                 KC_MPLY,
              LCTL(KC_Z), LCTL(LSFT(KC_Z)), HYPR(KC_DEL),
                                            HYPR(KC_F14),
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
                   KC_HOME,  _, KC_END, _,
         _,  _,
         _,
         _,  _,  _,
         PALM_R_WIN
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

void matrix_init_user(void) {
    all_leds_off();
    led_red_on(); _delay_ms(200);
    led_yellow_on(); _delay_ms(200);
    led_green_on(); _delay_ms(200);
    led_blue_on(); wait_ms(500);
    all_leds_off();

    switch (biton32(eeconfig_read_default_layer())) {
      case _MAC: isMac = true; isWin = false; break;
      case _WIN: isWin = true; isMac = false; break;
      default: break;
    }
    init_complete = true;
}

// support for repeat keycodes
void matrix_scan_user(void) {
   if (repeat_code) {
      if (timer_elapsed(repeat_timer) > (repeat_interval + first_repeat_delay)) {
         down(repeat_mod); key_code(repeat_code); up(repeat_mod);
         repeat_timer = timer_read();
         first_repeat_delay = 0;
      }
   }

   if (lead_timer > 0) {
     if (timer_elapsed(lead_timer) > 1000) {
       switch_lead_led_off();
     }
   } else {
     switch_lead_led_off();
   }

   if (lang_switch_led) {
     lang_switch_led = false;
     led_red_on(); _delay_ms(10);
     led_yellow_on(); _delay_ms(10); if (!is_macro1_recording) { led_red_off(); }
     led_green_on(); _delay_ms(10); led_yellow_off();
     led_blue_on(); _delay_ms(10);  if (!is_macro2_recording) { led_green_off(); }
     if (!caps_led) { _delay_ms(10); led_blue_off(); }
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

    case _KEYB_CONTROL: caps_led = false; all_leds_off(); break;

    // unregister everything (even if it was not pressed - no big deal; this works faster than getting pressed mods)
    default:
     remove_mods();
     default_layer = true; break;
    }
return state;
}

// support for "mo layer tap" functionality
bool cmd_esc_interrupted = true;
bool alt_slash_mac_interrupted = true;
bool alt_bslash_mac_interrupted = true;
bool shift_enter_mac_interrupted = true;
bool shift_tab_mac_interrupted = true;
bool ctrl_del_interrupted = true;
bool ctrl_f1_interrupted = true;
bool palm_l_mac_interrupted = true;
bool palm_r_mac_interrupted = true;

bool ctrl_esc_interrupted = true;
bool alt_slash_win_interrupted = true;
bool alt_bslash_win_interrupted = true;
bool shift_enter_win_interrupted = true;
bool shift_tab_win_interrupted = true;
bool ctrl_alt_del_interrupted = true;
bool palm_l_win_interrupted = true;
bool palm_r_win_interrupted = true;

// adding logic to custom keycodes and overriding existing ones (taking hold duration into account);
// "mo layer tap" and "esc leader key" functionality
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool pressed = record->event.pressed;

    if (default_layer) {
       // remove stuck modifiers
       remove_mods();
    }

    if ((keycode == CTRL_SPACE || keycode == CMD_SPACE) && pressed) {
      addToMacro(KC_SPC, true); addToMacro(KC_SPC, false);
    }

    if ((keycode == ALT_SHIFT_BS || keycode == CTRL_SHIFT_BS || keycode == KC_BSPC) && pressed) {
      addToMacro(KC_BSPC, true); addToMacro(KC_BSPC, false);
    }

    if (keycode != KC_LEFT && keycode != KC_RGHT) {
      if (keycode != CMD_SPACE && keycode != CTRL_SPACE) {
             esc_timer = 0;
      }
    }

    // support for "mo layer tap" functionality
    if (keycode != CMD_ESC) { cmd_esc_interrupted = true; }
    if (keycode != ALT_SLASH_MAC) { alt_slash_mac_interrupted = true; }
    if (keycode != ALT_BSLASH_MAC) { alt_bslash_mac_interrupted = true; }
    if (keycode != SHIFT_ENTER_MAC) { shift_enter_mac_interrupted = true; }
    if (keycode != SHIFT_TAB_MAC) { shift_tab_mac_interrupted = true; }
    if (keycode != CTRL_DEL) { ctrl_del_interrupted = true; }
    if (keycode != CTRL_F1) { ctrl_f1_interrupted = true; }
    if (keycode != PALM_L_MAC) { palm_l_mac_interrupted = true; }
    if (keycode != PALM_R_MAC) { palm_r_mac_interrupted = true; }

    if (keycode != CTRL_ESC) { ctrl_esc_interrupted = true; }
    if (keycode != ALT_SLASH_WIN) { alt_slash_win_interrupted = true; }
    if (keycode != ALT_BSLASH_WIN) { alt_bslash_win_interrupted = true; }
    if (keycode != SHIFT_ENTER_WIN) { shift_enter_win_interrupted = true; }
    if (keycode != SHIFT_TAB_WIN) { shift_tab_win_interrupted = true; }
    if (keycode != CTRL_ALT_DEL) { ctrl_alt_del_interrupted = true; }
    if (keycode != PALM_L_WIN) { palm_l_win_interrupted = true; }
    if (keycode != PALM_R_WIN) { palm_r_win_interrupted = true; }

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
          return press_leader_key(pressed);
        }
        // >>>>>>> KEYS, RESPONDING TO LEAD_SPACE SEQUENCE
        // autoshifted keys - same key with a shift
        case KC_Q: { return lead_autoshifted_same_key(KC_Q, pressed); }
        case KC_W: { return lead_autoshifted_same_key(KC_W, pressed); }
        case KC_E: { return lead_autoshifted_same_key(KC_E, pressed); }
        case KC_R: { return lead_autoshifted_same_key(KC_R, pressed); }
        case KC_T: { return lead_autoshifted_same_key(KC_T, pressed); }
        case KC_Y: { return lead_autoshifted_same_key(KC_Y, pressed); }
        case KC_U: { return lead_autoshifted_same_key(KC_U, pressed); }
        case KC_I: { return lead_autoshifted_same_key(KC_I, pressed); }
        case KC_O: { return lead_autoshifted_same_key(KC_O, pressed); }
        case KC_P: { return lead_autoshifted_same_key(KC_P, pressed); }
        case KC_A: { return lead_autoshifted_same_key(KC_A, pressed); }
        case KC_S: { return lead_autoshifted_same_key(KC_S, pressed); }
        case KC_D: { return lead_autoshifted_same_key(KC_D, pressed); }
        case KC_F: { return lead_autoshifted_same_key(KC_F, pressed); }
        case KC_G: { return lead_autoshifted_same_key(KC_G, pressed); }
        case KC_H: { return lead_autoshifted_same_key(KC_H, pressed); }
        case KC_J: { return lead_autoshifted_same_key(KC_J, pressed); }
        case KC_K: { return lead_autoshifted_same_key(KC_K, pressed); }
        case KC_L: { return lead_autoshifted_same_key(KC_L, pressed); }
        case KC_Z: { return lead_autoshifted_same_key(KC_Z, pressed); }
        case KC_X: { return lead_autoshifted_same_key(KC_X, pressed); }
        case KC_C: { return lead_autoshifted_same_key(KC_C, pressed); }
        case KC_V: { return lead_autoshifted_same_key(KC_V, pressed); }
        case KC_B: { return lead_autoshifted_same_key(KC_B, pressed); }
        case KC_N: { return lead_autoshifted_same_key(KC_N, pressed); }
        case KC_M: { return lead_autoshifted_same_key(KC_M, pressed); }

        case KC_1: { return lead_autoshifted_same_key(KC_1, pressed); }
        case KC_2: { return lead_autoshifted_same_key(KC_2, pressed); }
        case KC_3: { return lead_autoshifted_same_key(KC_3, pressed); }
        case KC_4: { return lead_autoshifted_same_key(KC_4, pressed); }
        case KC_5: { return lead_autoshifted_same_key(KC_5, pressed); }
        case KC_6: { return lead_autoshifted_same_key(KC_6, pressed); }
        case KC_7: { return lead_autoshifted_same_key(KC_7, pressed); }
        case KC_8: { return lead_autoshifted_same_key(KC_8, pressed); }
        case KC_9: { return lead_autoshifted_same_key(KC_9, pressed); }
        case KC_0: { return lead_autoshifted_same_key(KC_0, pressed); }

        case KC_F1: { return lead_f(KC_F1, pressed); }
        case KC_F2: { return lead_f(KC_F2, pressed); }
        case KC_F3: { return lead_f(KC_F3, pressed); }
        case KC_F4: { return lead_f(KC_F4, pressed); }
        case KC_F5: { return lead_f(KC_F5, pressed); }
        case KC_F6: { return lead_f(KC_F6, pressed); }
        case KC_F7: { return lead_f(KC_F7, pressed); }
        case KC_F8: { return lead_f(KC_F8, pressed); }
        case KC_F9: { return lead_f(KC_F9, pressed); }
        case KC_F10: { return lead_f(KC_F10, pressed); }
        case KC_F11: { return lead_f(KC_F11, pressed); }
        case KC_F12: { return lead_f(KC_F12, pressed); }
        case KC_F13: { return lead_f(KC_F13, pressed); }
        case KC_F14: { return lead_f(KC_F14, pressed); }
        case KC_F15: { return lead_f(KC_F15, pressed); }
        case KC_F16: { return lead_f(KC_F16, pressed); }
        case KC_F17: { return lead_f(KC_F17, pressed); }
        case KC_F18: { return lead_f(KC_F18, pressed); }
        case KC_F19: { return lead_f(KC_F19, pressed); }
        case KC_F20: { return lead_f(KC_F20, pressed); }
        case KC_F21: { return lead_f(KC_F21, pressed); }
        case KC_F22: { return lead_f(KC_F22, pressed); }
        case KC_F23: { return lead_f(KC_F23, pressed); }
        case KC_F24: { return lead_f(KC_F24, pressed); }

        case KC_LBRC: { return lead_autoshifted_same_key(KC_LBRC, pressed); }
        case KC_RBRC: { return lead_autoshifted_same_key(KC_RBRC, pressed); }
        case KC_COMM: { return lead_autoshifted_same_key(KC_COMM, pressed); }
        case KC_SCLN: { return lead_autoshifted_same_key(KC_SCLN, pressed); }
        case KC_QUOT: { return lead_autoshifted_same_key(KC_QUOT, pressed); }
        case KC_GRV: { return lead_autoshifted_same_key(KC_GRV, pressed); }
        case KC_DOT: { return lead_autoshifted_same_key(KC_DOT, pressed); }
        case KC_MINS: { return lead_autoshifted_same_key(KC_MINS, pressed); }
        case KC_EQL: { return lead_autoshifted_same_key(KC_EQL, pressed); }
        case _INS: { return lead_autoshifted_same_key(KC_INS, pressed); }

        // custom autoshifted keys - when pressed, other key + shift is sent
        case _1: { return lead_custom_autoshifted(KC_1, KC_1, KC_NO, pressed); }
        case _2_PLEFT: { return lead_custom_autoshifted(KC_2, KC_9, KC_LSFT, pressed); }
        case _3_SLASH: { return lead_custom_autoshifted(KC_3, KC_MINS, KC_LSFT, pressed); }
        case _4_PRGHT: { return lead_custom_autoshifted(KC_4, KC_0, KC_LSFT, pressed); }
        case _5_EQL: { return lead_custom_autoshifted(KC_5, KC_EQL, KC_NO, pressed); }
        case _6_PLUS: { return lead_custom_autoshifted(KC_6, KC_EQL, KC_LSFT, pressed); }
        case _7_BANG: { return lead_custom_autoshifted(KC_7, KC_1, KC_LSFT, pressed); }
        case _8_DASH: { return lead_custom_autoshifted(KC_8, KC_MINS, KC_NO, pressed); }
        case _9_QUEST: { return lead_custom_autoshifted(KC_9, KC_SLSH, KC_LSFT, pressed); }
        case _0: { return lead_custom_autoshifted(KC_0, KC_0, KC_NO, pressed); }

        // non-shifted keys
        case _Z: { return lead_custom_autoshifted(KC_Z, KC_Z, KC_NO, pressed); }
        case _X: { return lead_custom_autoshifted(KC_X, KC_X, KC_NO, pressed); }
        case _C: { return lead_custom_autoshifted(KC_C, KC_C, KC_NO, pressed); }
        case _V: { return lead_custom_autoshifted(KC_V, KC_V, KC_NO, pressed); }
        case _F: { return lead_custom_autoshifted(KC_F, KC_F, KC_NO, pressed); }

        case KC_UP: { if (is_after_lead(KC_UP, pressed)) { return false; } return true; }
        case KC_DOWN: { if (is_after_lead(KC_DOWN, pressed)) { return false; } return true; }

        case KC_RGUI: { if (is_after_lead(KC_MINS, pressed)) { return false; } return true; }
        case KC_BSPC: { if (is_after_lead(KC_BSPC, pressed)) { return false; } return true; }

        // >>>>>>> escape as additional leader key
        case KC_LEFT: {
          if (is_after_lead(KC_LEFT, pressed)) { return false; }
          return following_custom_leader_key(KC_HOME, isMac ? KC_LGUI : KC_NO, KC_NO, KC_NO, &esc_timer, pressed, 250);
        }
        case KC_RGHT: {
          if (is_after_lead(KC_RGHT, pressed)) { return false; }
          return following_custom_leader_key(KC_END, isMac ? KC_LGUI : KC_NO, KC_NO, KC_NO, &esc_timer, pressed, 250);
        }
        // <<<<<<< escape as additional leader key

        // >>>>>>> mac layers
        case CMD_SPACE: {
          if (is_after_lead(KC_SPC, pressed)) { return false; }; return true;
        }

        case ALT_SHIFT_BS: {
          if (is_after_lead(KC_BSPC, pressed)) { return false; } return true;
        }

        case CMD_ESC: {
          if (is_after_lead(KC_F1, pressed)) { return false; }
          static uint16_t cmd_esc_layer_timer;
          if (momentary_layer_tap(KC_ESC, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, &cmd_esc_layer_timer, &cmd_esc_interrupted, pressed, 250, true)) {
            esc_timer = timer_read();
          }
          return true;
        }

        case CTRL_DEL: {
          if (is_after_lead(KC_DEL, pressed)) { return false; }
          static uint16_t ctrl_del_layer_timer;
          momentary_layer_tap(KC_DEL, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_del_layer_timer, &ctrl_del_interrupted, pressed, 250, true);
          return true;
        }

        case CTRL_F1: {
          if (is_after_lead(KC_MINS, pressed)) { return false; }
          static uint16_t ctrl_f1_layer_timer;
          momentary_layer_tap(KC_F1, KC_LCTL, KC_LGUI, KC_NO, KC_NO, KC_NO, &ctrl_f1_layer_timer, &ctrl_f1_interrupted, pressed, 250, true);
          return true;
        }

        case ALT_SLASH_MAC: {
          if (is_after_lead(KC_SLSH, pressed)) { return false; }
          static uint16_t alt_slash_mac_layer_timer;
          momentary_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_slash_mac_layer_timer, &alt_slash_mac_interrupted, pressed, 250, true);
          return true;
        }

        case ALT_BSLASH_MAC: {
          if (is_after_lead(KC_EQL, pressed)) { return false; }
          static uint16_t alt_bslash_mac_layer_timer;
          momentary_layer_tap(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bslash_mac_layer_timer, &alt_bslash_mac_interrupted, pressed, 250, true);
          return true;
        }

        case SHIFT_ENTER_MAC: {
          if (is_after_lead(KC_ENTER, pressed)) { return false; }
          static uint16_t shift_enter_mac_layer_timer;
          momentary_layer_tap(KC_ENTER, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_enter_mac_layer_timer, &shift_enter_mac_interrupted, pressed, 300, true);
          return true;
        }

        case SHIFT_TAB_MAC: {
          if (is_after_lead(KC_F2, pressed)) { return false; }
          static uint16_t shift_tab_mac_layer_timer;
          momentary_layer_tap(KC_TAB, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_tab_mac_layer_timer, &shift_tab_mac_interrupted, pressed, 300, true);
          return true;
        }

        case PALM_L_MAC: {
          if (is_after_lead(KC_F14, pressed)) { return false; }
          static uint16_t palm_l_mac_layer_timer;
          momentary_layer_tap(KC_F14, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_LGUI, &palm_l_mac_layer_timer, &palm_l_mac_interrupted, pressed, 300, true);
          return true;
        }

        case PALM_R_MAC: {
          if (is_after_lead(KC_F15, pressed)) { return false; }
          static uint16_t palm_r_mac_layer_timer;
          momentary_layer_tap(KC_F15, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &palm_r_mac_layer_timer, &palm_r_mac_interrupted, pressed, 300, false);
          return true;
        }

        // >>>>>>> win layers
        case CTRL_SPACE: {
          if (is_after_lead(KC_SPC, pressed)) { return false; }; return true;
        }

        case CTRL_SHIFT_BS: {
          if (is_after_lead(KC_BSPC, pressed)) { return false; } return true;
        }

        case CTRL_ESC: {
          if (is_after_lead(KC_F1, pressed)) { return false; }
          static uint16_t ctrl_esc_layer_timer;
          if (momentary_layer_tap(KC_ESC, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_esc_layer_timer, &ctrl_esc_interrupted, pressed, 250, true)) {
            esc_timer = timer_read();
          }
          return true;
        }

        case CTRL_ALT_DEL: {
          if (is_after_lead(KC_DEL, pressed)) { return false; }
          static uint16_t ctrl_alt_del_layer_timer;
          momentary_layer_tap(KC_DEL, KC_NO, KC_LALT, KC_LCTL, KC_NO, KC_NO, &ctrl_alt_del_layer_timer, &ctrl_alt_del_interrupted, pressed, 250, false);
          return true;
        }

        case ALT_SLASH_WIN: {
          if (is_after_lead(KC_SLSH, pressed)) { return false; }
          static uint16_t alt_slash_win_layer_timer;
          momentary_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_slash_win_layer_timer, &alt_slash_win_interrupted, pressed, 250, false);
          return true;
        }

        case SHIFT_ENTER_WIN: {
          if (is_after_lead(KC_ENTER, pressed)) { return false; }
          static uint16_t shift_enter_win_layer_timer;
          momentary_layer_tap(KC_ENTER, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_enter_win_layer_timer, &shift_enter_win_interrupted, pressed, 300, true);
          return true;
        }

        case SHIFT_TAB_WIN: {
          if (is_after_lead(KC_F2, pressed)) { return false; }
          static uint16_t shift_tab_win_layer_timer;
          momentary_layer_tap(KC_TAB, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_tab_win_layer_timer, &shift_tab_win_interrupted, pressed, 300, true);
          return true;
        }

        case ALT_BSLASH_WIN: {
          if (is_after_lead(KC_EQL, pressed)) { return false; }
          static uint16_t alt_bslash_win_layer_timer;
          momentary_layer_tap(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bslash_win_layer_timer, &alt_bslash_win_interrupted, pressed, 250, false);
          return true;
        }

        case PALM_L_WIN: {
          if (is_after_lead(KC_F14, pressed)) { return false; }
          static uint16_t palm_l_win_layer_timer;
          momentary_layer_tap(KC_F14, KC_NO, KC_LCTL, KC_LALT, KC_LSFT, KC_LGUI, &palm_l_win_layer_timer, &palm_l_win_interrupted, pressed, 300, false);
          return true;
        }

        case PALM_R_WIN: {
          if (is_after_lead(KC_F15, pressed)) { return false; }
          static uint16_t palm_r_win_layer_timer;
          momentary_layer_tap(KC_F15, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &palm_r_win_layer_timer, &palm_r_win_interrupted, pressed, 300, false);
          return true;
        }
        // <<<<<<< layers
        // <<<<<<< KEYS, RESPONDING TO LEAD_SPACE SEQUENCE

        // additional auto-shift keys
        case _ENTER: { return if_held_150_add_shift(KC_ENTER, pressed); }
        case _ESC: { return if_held_150_add_shift(KC_ESC, pressed); }
        case _TAB: { return if_held_150_add_shift(KC_TAB, pressed); }
        case _SPACE: { return if_held_150_add_shift(KC_SPC, pressed); }
        case _BSPC: { return if_held_150_add_shift(KC_BSPC, pressed); }
        case _NUBS: { return if_held_150_add_shift(KC_NUBS, pressed); }
        case _DEL: { return if_held_150_add_shift(KC_DEL, pressed); }
        case _APP: { return if_held_150_add_shift(KC_APP, pressed); }
        case _BSLS: { return if_held_150_add_shift(KC_BSLS, pressed); }

        // repeating keycodes
        case KC_PGUP: { return repeat(KC_PGUP, KC_NO, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, pressed, 25); }
        case KC_PGDN: { return repeat(KC_PGDN, KC_NO, KC_LGUI, KC_LSFT, KC_LALT, KC_LCTL, pressed, 25); }

        // app switch
        case CMD_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LALT, KC_LCTL, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LGUI, KC_NO, KC_NO, KC_NO, pressed, 150, true); }
        case CMD_SHIFT_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, 150, true); }
        case ALT_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LGUI, KC_LCTL, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LALT, KC_NO, KC_NO, KC_NO, pressed, 150, true); }
        case ALT_SHIFT_TAB: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_TAB, KC_LGUI, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TAB, KC_LALT, KC_LSFT, KC_NO, KC_NO, pressed, 150, true); }

        // lang switch
        case LANG_CAPS_MAC: { return process_lang_caps(KC_SPC, KC_LGUI, KC_LALT, KC_NO, pressed, 150); }
        case LANG_CAPS_WIN: { return process_lang_caps(KC_SPC, KC_LCTL, KC_LGUI, KC_NO, pressed, 150); }

        // home/end
        case HOME_: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_HOME, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_HOME, KC_NO, KC_NO, KC_NO, KC_NO, pressed, 250, true); }
        case END_: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_END, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_END, KC_NO, KC_NO, KC_NO, KC_NO, pressed, 250, true); }
        case CTRL_HOME: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_HOME, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LCTL, KC_NO, KC_NO, KC_NO, pressed, 250, false); }
        case CTRL_END: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_END, KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LCTL, KC_NO, KC_NO, KC_NO, pressed, 250, false); }

        // delete words / lines
        case DEL_LEFT_MAC: { return delete_word_line(KC_BSPC, KC_LGUI, KC_LALT, KC_LEFT, KC_LGUI, KC_LSFT, pressed, 150); }
        case DEL_RIGHT_MAC: { return delete_word_line(KC_DEL, KC_LGUI, KC_LALT, KC_RGHT, KC_LGUI, KC_LSFT, pressed, 150); }
        case DEL_LEFT_WIN: { return delete_word_line(KC_BSPC, KC_LCTL, KC_LCTL, KC_HOME, KC_LSFT, KC_NO, pressed, 150); }
        case DEL_RIGHT_WIN: { return delete_word_line(KC_DEL, KC_LCTL, KC_LCTL, KC_END, KC_LSFT, KC_NO, pressed, 150); }

        // select block of text
        case SELECT_UP_MAC: { return if_held_150_add_shift(KC_UP, pressed); }
        case SELECT_DOWN_MAC: { return if_held_150_add_shift(KC_DOWN, pressed); }
        case SELECT_UP_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_UP, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LCTL, KC_LSFT, KC_NO, KC_NO, pressed, 150, true); }
        case SELECT_DOWN_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOWN, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LCTL, KC_LSFT, KC_NO, KC_NO, pressed, 150, false); }
        case SELECT_LEFT_MAC: { return if_held_add_mods(KC_LEFT, KC_LSFT, KC_NO, pressed, 250); }
        case SELECT_RIGHT_MAC: { return if_held_add_mods(KC_RGHT, KC_LSFT, KC_NO, pressed, 250); }
        case SELECT_LEFT_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_LEFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LSFT, KC_NO, KC_NO, KC_NO, pressed, 250, true); }
        case SELECT_RIGHT_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_RGHT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LSFT, KC_NO, KC_NO, KC_NO, pressed, 250, true); }

        // f3 / shift + f3
        case FIND_NEXT: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F3, KC_LALT, KC_LSFT, KC_LCTL, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_NO, KC_F3, KC_NO, KC_NO, KC_NO, KC_NO, pressed, 150, false); }
        case FIND_PREV: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F3, KC_LALT, KC_LSFT, KC_LCTL, KC_LGUI, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_F3, KC_LSFT, KC_NO, KC_NO, KC_NO, pressed, 150, false); }

        // close app
        case CMD_Q: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_Q, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_Q, KC_LGUI, KC_NO, KC_NO, KC_NO, pressed, 150, true); }
        case ALT_F4: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F4, KC_LGUI, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_F4, KC_F4, KC_LALT, KC_NO, KC_NO, KC_NO, pressed, 150, true); }

        // minimize app / minimize others
        case HIDE_FOCUS_MAC: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_H, KC_LALT, KC_LSFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_H, KC_LALT, KC_NO, KC_NO, KC_NO, pressed, 300, true); }
        case HIDE_FOCUS_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOWN, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_NO, KC_NO, pressed, 300, true); }

        // mac overrides
        case CTRL_DOT: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOT, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_DOT, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, 150, true); }
        case CTRL_COMM: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_COMM, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_COMM, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, 150, true); }
        case CTRL_H: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_H, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_H, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, 150, true); }
        case CTRL_M: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_M, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_M, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, 150, true); }

        // win overrides
        case W_F9: { return replace_ctrl_alt_with_lgui(KC_F9, pressed); }
        case W_F10: { return replace_ctrl_alt_with_lgui(KC_F10, pressed); }
        case W_F11: { return replace_ctrl_alt_with_lgui(KC_F11, pressed); }
        case W_F12: { return replace_ctrl_alt_with_lgui(KC_F12, pressed); }
        case W_6: { return replace_ctrl_alt_with_lgui(KC_6, pressed); }
        case W_7: { return replace_ctrl_alt_with_lgui(KC_7, pressed); }
        case W_8: { return replace_ctrl_alt_with_lgui(KC_8, pressed); }
        case W_9: { return replace_ctrl_alt_with_lgui(KC_9, pressed); }
        case W_0: { return replace_ctrl_alt_with_lgui(KC_0, pressed); }
        case W_Y: { return replace_ctrl_alt_with_lgui(KC_Y, pressed); }
        case W_U: { return replace_ctrl_alt_with_lgui(KC_U, pressed); }
        case W_I: { return replace_ctrl_alt_with_lgui(KC_I, pressed); }
        case W_O: { return replace_ctrl_alt_with_lgui(KC_O, pressed); }
        case W_P: { return replace_ctrl_alt_with_lgui(KC_P, pressed); }
        case W_H: { return replace_ctrl_alt_with_lgui(KC_H, pressed); }
        case W_J: { return replace_ctrl_alt_with_lgui(KC_J, pressed); }
        case W_K: { return replace_ctrl_alt_with_lgui(KC_K, pressed); }
        case W_L: { return replace_ctrl_alt_with_lgui(KC_L, pressed); }
        case W_QUOT: { return replace_ctrl_alt_with_lgui(KC_QUOT, pressed); }
        case W_N: { return replace_ctrl_alt_with_lgui(KC_N, pressed); }
        case W_M: { return replace_ctrl_alt_with_lgui(KC_M, pressed); }
        case W_DOT: { return replace_ctrl_alt_with_lgui(KC_DOT, pressed); }
        case W_SCLN: { return replace_ctrl_alt_with_lgui(KC_SCLN, pressed); }
        case W_EQL: { return replace_ctrl_alt_with_lgui(KC_EQL, pressed); }
        case W_MINS: { return replace_ctrl_alt_with_lgui(KC_MINS, pressed); }

        default: {
          return true;
        }
    }
}

void led_set_user(uint8_t usb_led) {
  if (!init_complete) { return; }

  caps_led ? led_blue_on() : led_blue_off();
  lead_led ? switch_lead_led_on() : switch_lead_led_off();

  if (is_macro1_recording) {
    led_red_on();
  } else {
    if (!lead_led) {
      led_red_off();
    }
  }

  if (is_macro2_recording) {
    led_green_on();
  } else {
    if (!lead_led) {
      led_green_off();
    }
  }
}
