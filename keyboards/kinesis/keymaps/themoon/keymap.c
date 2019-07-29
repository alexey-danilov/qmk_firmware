#include "kinesis.h"
#define __________ KC_TRNS
#define _ KC_CLR
#define XXXXXXX KC_NO

#define _MAC 0
#define _WIN 1

// mac layers
#define _COMMAND_SPACE 2
#define _COMMAND_ESCAPE 3
#define _ALT_SHIFT_BS 4
#define _ALT_SLASH_MAC 5
#define _ALT_BSLS_MAC 6
#define _SHIFT_ENTER_MAC 7
#define _SHIFT_TAB_MAC 8
#define _CTRL_DEL 9
#define _CTRL_F1 10
#define _PALM_1_MAC 11
#define _PALM_2_MAC 12
#define _PALM_3_MAC 13
#define _PALM_4_MAC 14

// win layers
#define _CONTROL_SPACE 15
#define _CONTROL_ESCAPE 16
#define _CTRL_SHIFT_BS 17
#define _ALT_SLASH_WIN 18
#define _ALT_BSLS_WIN 19
#define _SHIFT_ENTER_WIN 20
#define _SHIFT_TAB_WIN 21
#define _ALT_SHIFT_DEL 22
#define _RWIN 23
#define _PALM_1_WIN 24
#define _PALM_2_WIN 25
#define _PALM_3_WIN 26
#define _PALM_4_WIN 27

#define _KEYB_CONTROL 28

enum kinesis_keycodes {
  // mac
  CMD_SPACE = LT(_COMMAND_SPACE, KC_SPC),
  CMD_ESC = MO(_COMMAND_ESCAPE),
  ALT_SHIFT_BS = MO(_ALT_SHIFT_BS),
  ALT_SLASH_MAC = MO(_ALT_SLASH_MAC),
  ALT_BSLS_MAC = MO(_ALT_BSLS_MAC),
  SHIFT_ENTER_MAC = MO(_SHIFT_ENTER_MAC),
  SHIFT_TAB_MAC = MO(_SHIFT_TAB_MAC),
  CTRL_DEL = MO(_CTRL_DEL),
  CTRL_F1 = MO(_CTRL_F1),
  PALM_1_MAC = MO(_PALM_1_MAC),
  PALM_2_MAC = MO(_PALM_2_MAC),
  PALM_3_MAC = MO(_PALM_3_MAC),
  PALM_4_MAC = MO(_PALM_4_MAC),

  // win
  CTRL_SPACE = LT(_CONTROL_SPACE, KC_SPC),
  CTRL_ESC = MO(_CONTROL_ESCAPE),
  CTRL_SHIFT_BS = MO(_CTRL_SHIFT_BS),
  ALT_SLASH_WIN = MO(_ALT_SLASH_WIN),
  ALT_BSLS_WIN = MO(_ALT_BSLS_WIN),
  SHIFT_ENTER_WIN = MO(_SHIFT_ENTER_WIN),
  SHIFT_TAB_WIN = MO(_SHIFT_TAB_WIN),
  ALT_SHIFT_DEL = MO(_ALT_SHIFT_DEL),
  RWIN = MO(_RWIN),
  PALM_1_WIN = MO(_PALM_1_WIN),
  PALM_2_WIN = MO(_PALM_2_WIN),
  PALM_3_WIN = MO(_PALM_3_WIN),
  PALM_4_WIN = MO(_PALM_4_WIN),

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
  _SLSH,
  _BSPC,
  _EQL,
  _MINS,
  _INS,

  ALT_F9,
  ALT_F10,
  ALT_F11,
  ALT_F12,
  ALT_F13,
  ALT_F14,
  ALT_F15,
  ALT_F16,

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

  // mac-specific overrides
  CTRL_DOT,
  CTRL_H,
  CTRL_M,

  // LWin + key overrides
  W_INS, W_F1, W_F2, W_F3, W_F4, W_F5, W_F6, W_F7, W_F8, W_F9, W_F10, W_F11, W_F12, W_F13, W_F14, W_F15, W_F16,
  W_1, W_2, W_3, W_4, W_5, W_6, W_7, W_8, W_9, W_0,
  W_Q, W_W, W_E, W_R, W_T, W_Y, W_U, W_I, W_O, W_P,
  W_A, W_S, W_D, W_F, W_G, W_H, W_J, W_K, W_L, W_SCLN,
  W_Z, W_X, W_C, W_V, W_B, W_N, W_M, W_DOT, W_QUOT,
  W_LBRC, W_COMM, W_RBRC,
  W_GRV, W_APP, W_BSPC,
  W_EQL,
  W_MINS,

  SET_LAYER_MAC,
  SET_LAYER_WIN
};

static uint16_t esc_timer = 0; // timer for leader key: esc
static uint16_t lead_timer = 0; // timer for leader key
static uint16_t space_timer = 0;
static bool default_layer = true;
static bool palm_1_mac_layer = false;
static bool palm_2_mac_layer = false;
static bool palm_1_win_layer = false;
static bool palm_2_win_layer = false;

// HELPER FUNCTIONS
// switch mac <-> win
static bool isMac;
static bool isWin;
static bool caps_led;
static bool caps_on;
static bool lang_switch_led;
static bool lead_led;
static bool init_complete;

static bool macro1_recording = false;
static bool macro2_recording = false;

static uint16_t macro1Buffer[MACRO_BUFFER];
static uint16_t macro2Buffer[MACRO_BUFFER];
static uint16_t macroPointer1 = 0;
static uint16_t macroPointer2 = 0;
static uint16_t macro_key_down = 999;
static uint16_t macro_key_up = 1000;

// used to speed up macro playback (not to record unnecessary modifier down/up events)
static bool lgui_macro_pressed = false;
static bool rgui_macro_pressed = false;
static bool lctl_macro_pressed = false;
static bool rctl_macro_pressed = false;
static bool lalt_macro_pressed = false;
static bool ralt_macro_pressed = false;
static bool lshift_macro_pressed = false;
static bool rshift_macro_pressed = false;

//void keyboard_post_init_user(void) {
//  debug_enable=true; // also change CONSOLE_ENABLE = yes in rules.mk + use hid listener
//}

bool isMacroKey(uint16_t key) {
  if (!macro1_recording && !macro2_recording) {
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
      case KC_INS:  { dprint("\nKC_INS");  return true; }
      case KC_SLSH: { dprint("\nKC_SLSH"); return true; }
      case KC_BSLS: { dprint("\nKC_BSLS"); return true; }

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
      case KC_F13:  { dprint("\nKC_F13");   return true; }
      case KC_F14:  { dprint("\nKC_F14");   return true; }
      case KC_F15:  { dprint("\nKC_F15");   return true; }
      case KC_F16:  { dprint("\nKC_F16");   return true; }

      case KC_ESC:  { dprint("\nKC_ESC");  return true; }
      case KC_ENTER:{ dprint("\nKC_ENTER");return true; }
      case KC_TAB:  { dprint("\nKC_TAB");  return true; }
      case KC_DEL:  { dprint("\nKC_DEL");  return true; }

      case KC_SPC:  { dprint("\nKC_SPC");  return true; }
      case KC_BSPC: { dprint("\nKC_BSPC"); return true; }
      case KC_LEFT: { dprint("\nKC_LEFT"); return true; }
      case KC_RGHT: { dprint("\nKC_RGHT"); return true; }

      // modifiers are added to macro key in a special manner - see addMacroMod function
      case KC_LGUI: { dprint("\nKC_LGUI"); return false; }
      case KC_RGUI: { dprint("\nKC_RGUI"); return false; }
      case KC_LALT: { dprint("\nKC_LALT"); return false; }
      case KC_RALT: { dprint("\nKC_RALT"); return false; }
      case KC_LSFT: { dprint("\nKC_LSFT"); return false; }
      case KC_RSFT: { dprint("\nKC_RSFT"); return false; }
      case KC_LCTL: { dprint("\nKC_LCTL"); return false; }
      case KC_RCTL: { dprint("\nKC_RCTL"); return false; }

      case 999:     { dprint("\nKC_Event_DOWN");        return false; }
      case 1000:    { dprint("\nKC_Event_UP");          return false; }
      case KC_NO:   { dprint("\nKC_NO");                return false; }
      default:      { dprintf("\nUnknown key %d", key); return false; }
  }
}

void playMacro1(void) {
   dprint("\n... Printing macro buffer 1: ");
   for (uint16_t i = 0; i <= macroPointer1; ) {
     uint16_t event = macro1Buffer[i];
     if (event == KC_NO) { dprintf("\n >>> Reached end of macro 1 at %d", i); break; }
     i++;
     if (i >= macroPointer1) { dprintf("\n >>> Reached end of macro 1 at %d", i); break; }
     uint16_t key = macro1Buffer[i];
     i++;
     if (i >= macroPointer1) { dprintf("\n >>> Reached end of macro 1 at %d", i); break; }
     if (key == KC_NO) { dprintf("\n >>> Reached end of macro 1 at %d", i); break; }

     if (event == macro_key_down) { register_code(key); } else { unregister_code(key); }
   }
   clear_keyboard();
   layer_clear();
   dprint("\n... Done printing macro buffer 1\n");
}

void playMacro2(void) {
   dprint("\n... Printing macro buffer 2: ");
   for (uint16_t i = 0; i <= macroPointer2; ) {
     uint16_t event = macro2Buffer[i];
     if (event == KC_NO) { dprintf("\n >>> Reached end of macro 2 at %d", i); break; }
     i++;
     if (i >= macroPointer2) { dprintf("\n >>> Reached end of macro 2 at %d", i); break; }
     uint16_t key = macro2Buffer[i];
     i++;
     if (i >= macroPointer2) { dprintf("\n >>> Reached end of macro 2 at %d", i); break; }
     if (key == KC_NO) { dprintf("\n >>> Reached end of macro 2 at %d", i); break; }

     if (event == macro_key_down) { register_code(key); } else { unregister_code(key); }
   }
   clear_keyboard();
   layer_clear();
   dprint("\n... Done printing macro buffer 2\n");
}

void printMacro1(void) {
  for (uint16_t i = 0; i < macroPointer1; i++) {
    dprintf("\nmacro 1: %d == ", i); isMacroKey(macro1Buffer[i]);
  }
}

void printMacro2(void) {
  for (uint16_t i = 0; i < macroPointer2; i++) {
    dprintf("\nmacro 2: %d == ", i); isMacroKey(macro2Buffer[i]);
  }
}

void changeModInMacroBuffer(uint8_t mod, bool added) {
   if (macro1_recording) {
     if ((macroPointer1 + 2) < MACRO_BUFFER) {
           macro1Buffer[macroPointer1] = added ? macro_key_down : macro_key_up;
           macroPointer1++;
           macro1Buffer[macroPointer1] = mod;
           macroPointer1++;
     } else {
       dprint("\n!!! Macro buffer 1 full");
     }
   } else if (macro2_recording) {
     if ((macroPointer2 + 2) < MACRO_BUFFER) {
         dprint("\n >>> Adding macro 1 key");
           macro2Buffer[macroPointer2] = added ? macro_key_down : macro_key_up;
           macroPointer2++;
           macro2Buffer[macroPointer2] = mod;
           macroPointer2++;
     } else {
       dprint("\n!!! Macro buffer 2 full");
     }
   }
}

void changeModInMacro(uint8_t current_mods, uint8_t mod) {
  if (current_mods & (MOD_BIT(mod))) {
    if (mod == KC_LGUI) { if (!lgui_macro_pressed) { changeModInMacroBuffer(mod, true); lgui_macro_pressed = true; return; }}
    if (mod == KC_RGUI) { if (!rgui_macro_pressed) { changeModInMacroBuffer(mod, true); rgui_macro_pressed = true; return; }}
    if (mod == KC_LCTL) { if (!lctl_macro_pressed) { changeModInMacroBuffer(mod, true); lctl_macro_pressed = true; return; }}
    if (mod == KC_RCTL) { if (!rctl_macro_pressed) { changeModInMacroBuffer(mod, true); rctl_macro_pressed = true; return; }}
    if (mod == KC_LALT) { if (!lalt_macro_pressed) { changeModInMacroBuffer(mod, true); lalt_macro_pressed = true; return; }}
    if (mod == KC_RALT) { if (!ralt_macro_pressed) { changeModInMacroBuffer(mod, true); ralt_macro_pressed = true; return; }}
    if (mod == KC_LSFT) { if (!lshift_macro_pressed) { changeModInMacroBuffer(mod, true); lshift_macro_pressed = true; return; }}
    if (mod == KC_RSFT) { if (!rshift_macro_pressed) { changeModInMacroBuffer(mod, true); rshift_macro_pressed = true; return; }}
  } else {
    if (mod == KC_LGUI) { if (lgui_macro_pressed) { changeModInMacroBuffer(mod, false); lgui_macro_pressed = false; return; }}
    if (mod == KC_RGUI) { if (lgui_macro_pressed) { changeModInMacroBuffer(mod, false); lgui_macro_pressed = false; return; }}
    if (mod == KC_LCTL) { if (lctl_macro_pressed) { changeModInMacroBuffer(mod, false); lctl_macro_pressed = false; return; }}
    if (mod == KC_RCTL) { if (rctl_macro_pressed) { changeModInMacroBuffer(mod, false); rctl_macro_pressed = false; return; }}
    if (mod == KC_RALT) { if (lalt_macro_pressed) { changeModInMacroBuffer(mod, false); lalt_macro_pressed = false; return; }}
    if (mod == KC_LSFT) { if (ralt_macro_pressed) { changeModInMacroBuffer(mod, false); ralt_macro_pressed = false; return; }}
    if (mod == KC_LSFT) { if (lshift_macro_pressed) { changeModInMacroBuffer(mod, false); lshift_macro_pressed = false; return; }}
    if (mod == KC_RSFT) { if (rshift_macro_pressed) { changeModInMacroBuffer(mod, false); rshift_macro_pressed = false; return; }}
  }
}

void recordMacroKey(uint16_t key, bool down) {
  if (!macro1_recording && !macro2_recording) {
    dprint("\nMacro is not recording, ignoring key\n");
    return;
  }
  bool macroKey = isMacroKey(key);
  if (!macroKey) {
      dprint("\nNot a macro key, ignoring\n");
      return;
  }

  dprintf("\nmacro1Buffer recording: %d, pointer %d; macro2Buffer recording: %d, pointer %d", macro1_recording, macroPointer1, macro2_recording, macroPointer2);

  if (down) {
    // when the key is pressed down, check if it needs to have a modifier; if it does, append modifier in macro buffer;
    // otherwise - remove modifier from buffer (on next key down);
    uint8_t current_mods = get_mods();
    changeModInMacro(current_mods, KC_LCTL);
    changeModInMacro(current_mods, KC_RCTL);
    changeModInMacro(current_mods, KC_LGUI);
    changeModInMacro(current_mods, KC_RGUI);
    changeModInMacro(current_mods, KC_LALT);
    changeModInMacro(current_mods, KC_RALT);
    changeModInMacro(current_mods, KC_LSFT);
    changeModInMacro(current_mods, KC_RSFT);
  }

  if (macro1_recording) {
    if ((macroPointer1 + 2) < MACRO_BUFFER) {
        dprint("\n >>> Adding macro 1 key");
        if (down) { macro1Buffer[macroPointer1] = macro_key_down; dprint("\n<<< Added macro 1 macro_key_down"); }
        else { macro1Buffer[macroPointer1] = macro_key_up; dprint("\n<<< Added macro 1 macro_key_up"); }
        macroPointer1++;
        macro1Buffer[macroPointer1] = key;
        dprint("\n<<< Added macro 1 key");
        macroPointer1++;
    } else {
      dprint("\n!!! Macro buffer 1 full");
      //   dprint("\n??? Printing macro buffer 1");
      //   printMacro1();
      //   dprint("\n??? Printing macro buffer 1 done \n");
    }
  } else if (macro2_recording) {
    if ((macroPointer2 + 2) < MACRO_BUFFER) {
        dprint("\n >>> Adding macro 2 key");
        if (down) { macro2Buffer[macroPointer2] = macro_key_down; dprint("\n<<< Added macro 2 macro_key_down"); }
        else { macro2Buffer[macroPointer2] = macro_key_up; dprint("\n<<< Added macro 2 macro_key_up"); }
        macroPointer2++;
        macro2Buffer[macroPointer2] = key;
        dprint("\n<<< Added macro 2 key");
        macroPointer2++;
    } else {
      dprint("\n!!! Macro buffer 2 full");
      //   dprint("\n??? Printing macro buffer 2");
      //   printMacro2();
      //   dprint("\n??? Printing macro buffer 2 done \n");
    }
  }
}

void down(uint16_t key) { register_code(key); recordMacroKey(key, true); }
void up(uint16_t key) { unregister_code(key); recordMacroKey(key, false); }
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

bool held_shorter(uint16_t hold_timer, uint16_t hold_duration) {
 return timer_elapsed(hold_timer) < hold_duration;
}

bool held_between(uint16_t hold_timer, uint16_t min_duration, uint16_t max_duration) {
 uint16_t cur_timer = timer_elapsed(hold_timer);
 return cur_timer >= min_duration && cur_timer <= max_duration;
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
    if (!macro1_recording) { led_red_off(); }
    if (!macro2_recording) { led_green_off(); }
    led_yellow_off();
    lead_led = false;
  }
}

void all_leds_on(void) {
  led_red_on(); led_yellow_on(); led_green_on(); led_blue_on();
}

void all_leds_off(void) {
     if (!macro1_recording) { led_red_off(); }
     led_yellow_off();
     if (!macro2_recording) { led_green_off(); }
     if(!caps_led) { led_blue_off(); }
}

bool capsOnHardCheck(void) {
  return host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK);
}

void capsOn(void) {
  down(KC_LCAP); caps_led = true; led_blue_on();
  caps_on = true;
}

void capsOff(void) {
  up(KC_LCAP); caps_led = false; led_blue_off();
    caps_on = false;
}

void toggleCaps(void) {
    if (capsOnHardCheck()) {
        capsOff();
    } else {
        capsOn();
    }
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

      if (held_shorter(hold_timer, hold_duration)){
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
  return replace_key_and_mods_if_held_replace_key_and_mods(code, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, code, KC_LGUI, KC_NO, KC_NO, KC_NO, pressed, AUTOSHIFT_QWERTY_KEYS_TERM, true);
}

// replaces keycode and adds mod to it if it was held for at least provided duration
bool replace_if_held_add_mods(uint16_t code, uint16_t mod, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint16_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      if (held_shorter(hold_timer, hold_duration)){
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
      if (held_shorter(hold_timer, hold_duration)){
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

// adds shift to keycode if it was held for at least AUTOSHIFT_QWERTY_KEYS_TERM ms
bool if_held_autoshift(uint16_t code, bool pressed) {
  return if_held_add_mods(code, KC_LSFT, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM);
}

// provides functionality similar to MT - except that layer with mod is triggered immediately: this is useful when such mod is used with mouse
uint8_t momentary_layer_tap_with_hold(
    uint16_t tap_key,
    uint16_t tap_mod,
    uint16_t layer_mod1,
    uint16_t layer_mod2,
    uint16_t layer_mod3,
    uint16_t layer_mod4,
    uint16_t *layer_timer,
    bool *interrupted_flag,
    bool pressed,
    uint16_t min_hold_duration,
    uint16_t max_hold_duration,
    bool bring_mods_back,
    uint16_t held_code,
    uint16_t held_mod1,
    uint16_t held_mod2) {

  if (pressed) {
    *interrupted_flag = false;
    *layer_timer = timer_read();

  } else {
    if (*interrupted_flag) {
      return false;
    }
    bool tapped = held_shorter(*layer_timer, min_hold_duration);
    bool supportsHold = (held_code != KC_NO) && held_between(*layer_timer, min_hold_duration, max_hold_duration);

    if (tapped || supportsHold) {
      uint8_t ret_code = 0;
      up(layer_mod1); up(layer_mod2); up(layer_mod3); up(layer_mod4); // unregister mods associated with the layer, so that they don't intefere with the tap key
      if (tapped) {
        with_1_mod(tap_key, tap_mod);
        ret_code = 1;
      } else if (supportsHold) {
        with_2_mods(held_code, held_mod1, held_mod2);
        ret_code = 2;
      }
      if (bring_mods_back) {
        down(layer_mod1); down(layer_mod2); down(layer_mod3); down(layer_mod4); // bring mods back
      }
      return ret_code;
    }
  }
  return 0; // false
}

uint8_t momentary_layer_tap(
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
  return momentary_layer_tap_with_hold(tap_key, tap_mod, layer_mod1, layer_mod2, layer_mod3, layer_mod4, layer_timer, interrupted_flag, pressed, hold_duration, hold_duration, bring_mods_back, KC_NO, KC_NO, KC_NO);
}

// if tapped, deletes a single word; if held - everything to line start/end
bool delete_word_line(uint16_t code, uint16_t mod_to_remove, uint16_t mod_to_add, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2, bool pressed, uint16_t hold_duration) {
  static uint16_t hold_timer;
  if(pressed) {
      hold_timer= timer_read();
  } else {
      up(mod_to_remove);
      if (held_shorter(hold_timer, hold_duration)){
          with_1_mod(code, mod_to_add);
      } else {
          with_2_mods(held_code, held_mod1, held_mod2); key_code(code);
      }
      down(mod_to_remove);
  }
  return false;
}

// ESC AS A LEADER KEY
// provides functionality similar to "leader key", except that it works for escape
bool not_following_esc(uint16_t code, uint16_t mod1, uint16_t mod2, uint16_t mod3, uint16_t *leader_timer, bool pressed, uint16_t leader_last_pressed_timeout) {
  if (*leader_timer && held_shorter(*leader_timer, leader_last_pressed_timeout)) {
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
  if (pressed) {

      hold_timer= timer_read();
  } else {
      if (held_shorter(hold_timer, AUTOSHIFT_SPECIAL_TERM)){
          up(KC_LGUI); up(KC_LCTL);
          lead_timer = timer_read();
          switch_lead_led_on();
      } else {
          with_1_mod(KC_SPC, KC_LSFT);
          lead_timer = 0;
          switch_lead_led_off();
      }
  }
  return false;
}

bool lead_impl(uint16_t code, uint16_t os_mod, uint16_t additional_mod, bool pressed) {
  if (lead_timer) {
    if (held_shorter(lead_timer, 1000)) {
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
      if (held_shorter(hold_timer, hold_duration)){
          with_1_mod(code, mod);
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

bool lead_custom_autoshifted_with_mods(uint16_t code, uint16_t mod, uint16_t lead_code, uint16_t held_code, uint16_t held_mod1, uint16_t held_mod2,bool pressed, uint16_t hold_duration) {
  static bool was_lead;
  if (lead_impl(lead_code, isMac ? KC_LGUI: KC_LCTL, KC_LALT, pressed)) {
     was_lead = true;
     return false;
  };
  return lead_replace_if_held_add_mods(code, mod, held_code, held_mod1, held_mod2, &was_lead, pressed, hold_duration);
}

bool lead_custom_autoshifted_with_mod(uint16_t code, uint16_t mod, uint16_t lead_code, uint16_t held_code, uint16_t held_mod, bool pressed, uint16_t hold_duration) {

  return lead_custom_autoshifted_with_mods(code, mod, lead_code, held_code, held_mod, KC_NO, pressed, hold_duration);
}

bool lead_custom_autoshifted(uint16_t code, uint16_t lead_code, uint16_t held_code, uint16_t held_mod, bool pressed, uint16_t hold_duration) {
  return lead_custom_autoshifted_with_mod(code, KC_NO, lead_code, held_code, held_mod, pressed, hold_duration);
}

bool lead_autoshifted_modified_numbers(uint16_t code, uint16_t held_code, uint16_t held_mod, bool pressed) {
  return lead_custom_autoshifted(code, code, held_code, held_mod, pressed, AUTOSHIFT_MODIFIED_NUMBERS_TERM);
}

bool lead_autoshifted_same_key(uint16_t code, bool pressed) {
  return lead_custom_autoshifted(code, code, code, KC_LSFT, pressed, AUTOSHIFT_QWERTY_KEYS_TERM);
}

bool lead_autoshifted_special(uint16_t code, bool pressed) {
  return lead_custom_autoshifted(code, code, code, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM);
}

bool lead_autoshifted_numbers(uint16_t code, bool pressed) {
  return lead_custom_autoshifted(code, code, code, KC_LSFT, pressed, AUTOSHIFT_NUMBERS_TERM);
}

bool lead_autoshifted_custom_term(uint16_t code, bool pressed, uint16_t hold_duration) {
  static bool was_lead;
  if (is_after_lead(code, pressed)) {
     was_lead = true;
     return false;
  }
  return lead_replace_if_held_add_mods(code, KC_NO, code, KC_LSFT, KC_NO, &was_lead, pressed, hold_duration);
}

enum {
  TAP_MACRO = 0,
  SET_TD = 1,
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

//**************** REST TAP *********************//
static tap rest_tap_state = { .is_press_action = true, .state = 0 };

void rest_finished (qk_tap_dance_state_t *state, void *user_data) {
  rest_tap_state.state = cur_dance(state);
    switch (rest_tap_state.state) {
      case SINGLE_TAP:
          // sleep
          if (isMac) {
            with_1_mod(KC_F3, KC_LCTL); break;
          }
          if (isWin) {
            key_code(KC_F18); break;
          }
      case SINGLE_HOLD:
          // sleep
          if (isMac) {
            with_1_mod(KC_F4, KC_LCTL); break;
          }
          if (isWin) {
            with_1_mod(KC_F18, KC_LSFT); break;
          }
      case DOUBLE_TAP:
          // sleep
          if (isMac) {
            all_leds_on(); _delay_ms(125); all_leds_off(); _delay_ms(200); all_leds_on(); _delay_ms(125); all_leds_off();
            down(KC_LCTL); down(KC_LSFT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LSFT); up(KC_LCTL); break;
          }
          if (isWin) {
            all_leds_on(); _delay_ms(125); all_leds_off(); _delay_ms(200); all_leds_on(); _delay_ms(125); all_leds_off();
            down(KC_SLEP); up(KC_SLEP); break;
          }
      case DOUBLE_HOLD:
         // shutdown
         all_leds_on(); _delay_ms(300); led_blue_off(); _delay_ms(300); led_green_off(); _delay_ms(300); led_yellow_off(); _delay_ms(300); led_red_off();

         if (isMac) {
            down(KC_LGUI); down(KC_LCTL); down(KC_LALT); SEND_STRING(SS_DOWN(X_POWER) SS_UP(X_POWER)); up(KC_LALT); up(KC_LCTL); up(KC_LGUI); break;
         }
         if (isWin) {
            down(KC_PWR); up(KC_PWR); break;
         }
      default: break;
    }
}

void rest_reset (qk_tap_dance_state_t *state, void *user_data) {
  rest_tap_state.state = 0;
}

//**************** SETTINGS TAP *********************//
static tap set_tap_state = { .is_press_action = true, .state = 0 };

void set_finished (qk_tap_dance_state_t *state, void *user_data) {
  set_tap_state.state = cur_dance(state);
  if (!is_after_lead(KC_F5, true)) {
      switch (set_tap_state.state) {
        case SINGLE_TAP:
            key_code(KC_F5); break;

        case SINGLE_HOLD:
            with_1_mod(KC_F5, KC_LSFT); break;

        case DOUBLE_TAP:
            // settings
            if (isMac) {
              with_1_mod(KC_COMM, KC_LGUI); break;
            }
            if (isWin) {
              key_code(KC_PAUS); break;
            }

        case DOUBLE_HOLD:
            // print screen
            if (isMac) {
              with_1_mod(KC_F2, KC_LCTL); break;
            }
            if (isWin) {
              key_code(KC_PSCR); break;
            }
        default: break;
      }
  }
}

void set_reset (qk_tap_dance_state_t *state, void *user_data) {
  set_tap_state.state = 0;
}

//**************** DYNAMIC MACRO TAP *********************//
static tap dynamic_macro_state = { .is_press_action = true, .state = 0 };

void dynamic_macro_finished (qk_tap_dance_state_t *state, void *user_data) {
  dynamic_macro_state.state = cur_dance(state);
  if (!is_after_lead(KC_F14, true)) {

    if (macro1_recording || macro2_recording) {
       dprint("\nStopping record of macros");
       macro1_recording = false;
       macro2_recording = false;

       lgui_macro_pressed = false;
       rgui_macro_pressed = false;
       lctl_macro_pressed = false;
       rctl_macro_pressed = false;
       lalt_macro_pressed = false;
       ralt_macro_pressed = false;
       lshift_macro_pressed = false;
       lshift_macro_pressed = false;
       rshift_macro_pressed = false;

       if (!lead_led) { led_red_off(); led_green_off(); };

    } else {
      switch (dynamic_macro_state.state) {
          case SINGLE_TAP: playMacro1(); break;
          case DOUBLE_TAP: playMacro2(); break;

          case SINGLE_HOLD:
              macroPointer1 = 0;
              macro1_recording = true;
              dprint("\nStarting recording of macro 1");
              led_red_on();
          break;

          default:
              macroPointer2 = 0;
              macro2_recording = true;
              dprint("\nStarting recording of macro 2");
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
  [TAP_MACRO] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, dynamic_macro_finished, dynamic_macro_reset, 400),
  [REST_TD] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, rest_finished, rest_reset, 400),
  [SET_TD] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, set_finished, set_reset, 400)
};

/*
* ,-------------------------------------------------------------------------------------------------------------------.
* | _Ins   |  F1  |  F2  |  F3  |  F4  |F5/SET|  F6 |  F8  |  F9  |  F10  |  F12 |  F13 | F14  | F15  | Prog | REST_TD|
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* | ALT_F9 |   1  |  2(  |  3_  |   4) |  5=  |                           |  6+  |  7!  |  8-  |  9?  |  0   | ALT_F13|
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | ALT_F10|   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |  I   |   O  |  P   | ALT_F14|
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | ALT_F11|   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |  K   |   L  |  ;:  | ALT_F15|
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | ALT_F12|   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  Up  |  .>  |  '"  | ALT_F16|
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |  `~  |  [{  |  ,<  |  ]}  |                                         | Left | Down | Right|  App |
*          `---------------------------'                                         `---------------------------'
*                            .-------------------------.         ,---------------------------.
*                            | Backspace |    Macro    |         | F16  |     Backspace      |
*                            `-----------|------|------|         |------+------+-------------`
*                                 |      |      | Alt//|         | Alt=+|      |      |
*                                 | LGui/|Shift/|------|         |------|Shift/|RGui/ |
*                                 | ESC  |Enter | Ctrl/|         | Ctrl |Tab   |SPACE |
*                                 |      |      | Del  |         | \|   |      |      |
*                                  --------------------           --------------------
*
*                    -------------        -------------           -------------         -------------
*                   |PALM_1/ALT_F5|      |PALM_2/ALT_F6|         |PALM_3/ALT_F7|       |PALM_4/ALT_F8|
*                    -------------        -------------           -------------         -------------
*/

// base mac layer
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_MAC] = LAYOUT(
           // left side
           _INS, KC_F1, KC_F2, KC_F3, KC_F4, TD(SET_TD), KC_F6, KC_F7, KC_F8,
           ALT_F9, _1, _2_PLEFT, _3_SLASH, _4_PRGHT, _5_EQL,
           ALT_F10, KC_Q, KC_W, KC_E, KC_R, KC_T,
           ALT_F11,KC_A, KC_S, KC_D, KC_F, KC_G,
           ALT_F12, KC_Z, KC_X, KC_C, KC_V, KC_B,
                 KC_GRV, KC_LBRC, KC_COMM, KC_RBRC,
                                               // left thumb keys
			                       ALT_SHIFT_BS, TD(TAP_MACRO),
                                                   ALT_SLASH_MAC,
                              CMD_ESC, SHIFT_ENTER_MAC, CTRL_DEL,
                                                // left palm keys
			                             PALM_1_MAC, PALM_2_MAC,
    // right side
  KC_F9, KC_F10, KC_F11, KC_F12, KC_F13, KC_F14, KC_F15, KEYB_CONTROL, TD(REST_TD),
	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0, ALT_F13,
	KC_Y, KC_U, KC_I, KC_O, KC_P, ALT_F14,
	KC_H, KC_J, KC_K, KC_L, KC_SCLN, ALT_F15,
	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, ALT_F16,
	KC_LEFT, KC_DOWN, KC_RGHT, KC_NUBS,
           // right thumb keys
           KC_F16, KC_BSPC,
           ALT_BSLS_MAC,
           CTRL_F1, SHIFT_TAB_MAC, CMD_SPACE,
           // right palm key
           PALM_3_MAC, PALM_4_MAC
    ),

// mac-specific layers
[_COMMAND_ESCAPE] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  KC_F4,  __________,
                                         _,  _,
                                       KC_LALT,
                     CMD_ESC, KC_LSFT, KC_LCTL,
                                          _, _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         CTRL_H, __________,  __________,  __________,  __________,  __________,
         __________, CTRL_M, SELECT_UP_MAC, CTRL_DOT, __________, __________,
                   SELECT_LEFT_MAC,  SELECT_DOWN_MAC,  SELECT_RIGHT_MAC, __________,
         __________, DEL_LEFT_MAC,
         _MINS,
         _EQL, KC_F2, LEAD_SPACE,
         KC_F7, KC_F8
    ),

[_COMMAND_SPACE] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  KC_F4,  __________,
                               DEL_LEFT_MAC, KC_F14,
                                              _SLSH,
                       KC_F3, _ENTER, DEL_RIGHT_MAC,
                                       KC_F5, KC_F6,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             _, _,
                             KC_LALT,
                             KC_LCTL, KC_LSFT, CMD_SPACE,
                             _, _
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
                                         _, _,
         _,  _,  _,  _,  _,  _, _, _, _,
	       _, _, _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  __________,  _ ,  _,  _,
         __________, __________, __________, _,
         _,  _,
         _,
         _,  _,  _,
         _, _
    ),

[_ALT_SLASH_MAC] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                              _,  _,
                      ALT_SLASH_MAC,
          KC_LGUI, KC_LSFT, KC_LCTL,
                                  __________, _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
         __________, LCTL(KC_BSPC),
         _MINS,
         _EQL, _TAB, KC_F1,
         KC_F7, KC_F8
    ),

[_ALT_BSLS_MAC] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                               LCTL(KC_BSPC), KC_F14,
                                               _SLSH,
                          _ESC, _ENTER, LCTL(KC_DEL),
                                        KC_F5, KC_F6,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
         _,  _,
         ALT_BSLS_MAC,
         KC_LCTL, KC_LSFT, KC_LGUI,
         _, _
    ),

[_CTRL_F1] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                  _BSPC, KC_F14,
                                         _SLSH,
                            _ESC, _ENTER, _DEL,
                                  KC_F5, KC_F6,
         _,  _,  _,  _,  _,  _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _ ,  _,  _,
                   _,  _,  _, _,
         _,  _,
         KC_LALT,
         CTRL_F1, KC_LSFT, KC_LCTL,
         _, _
    ),

[_CTRL_DEL] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                             _,  _,
                            KC_LALT,
         KC_LGUI, KC_LSFT, CTRL_DEL,
                                  __________, _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
         __________, _BSPC,
         _MINS,
         _EQL, _TAB, _SPACE,
         KC_F7, KC_F8
    ),

[_SHIFT_ENTER_MAC] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                   _,  _,
                                 KC_LALT,
       KC_LGUI, SHIFT_ENTER_MAC, KC_LCTL,
                                       __________, _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	       KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
                   __________,  __________,  __________, __________,
         __________, _BSPC,
         _BSLS,
         KC_F17, _TAB, _SPACE,
         KC_F7, KC_F8
    ),

[_SHIFT_TAB_MAC] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                             _BSPC, KC_F14,
                                    _SLSH,
                       _ESC, _ENTER, _DEL,
                             KC_F5, KC_F6,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
         _,  _,
         KC_LALT,
         KC_LCTL, SHIFT_TAB_MAC, KC_LGUI,
         _, _
    ),

[_PALM_1_MAC] = LAYOUT(
         HYPR(KC_INS), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), KC__VOLDOWN, KC__MUTE, KC__VOLUP, HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F9), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F10), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F11), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F12), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), KC_MRWD, KC_MPLY, KC_MFFD,
                                HYPR(KC_BSPC), A(KC_F13),
                                               A(KC_F14),
                       G(KC_Z), G(S(KC_Z)), HYPR(KC_DEL),
                                           PALM_1_MAC, _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  C(A(KC_7)), C(A(KC_8)), C(A(KC_9)),  __________,  __________,
         __________,  C(A(KC_U)), C(A(KC_I)), C(A(KC_O)),  __________,  __________,
         __________,  C(A(KC_J)), C(A(KC_K)), C(A(KC_L)),  __________,  __________,
         __________,  C(A(KC_M)), KC_PGUP, C(A(KC_DOT)),  __________, __________,
                      KC_HOME, KC_PGDN, KC_END, __________,
         C(A(KC_F16)), C(A(KC_BSPC)),
         C(A(KC_MINS)),
         C(A(KC_EQL)), FIND_PREV, FIND_NEXT,
         C(A(KC_F7)), C(A(KC_F8))
    ),

[_PALM_2_MAC] = LAYOUT(
         HYPR(KC_INS), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), KC__VOLDOWN, KC__MUTE, KC__VOLUP, HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F9), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F10), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F11), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F12), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), KC_MRWD, KC_MPLY, KC_MFFD,
                                HYPR(KC_BSPC), A(KC_F13),
                                               A(KC_F14),
                       G(KC_Z), G(S(KC_Z)), HYPR(KC_DEL),
                                           _, PALM_2_MAC,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  C(A(KC_7)), C(A(KC_8)), C(A(KC_9)),  __________,  KC_F13,
         __________,  C(A(KC_U)), C(A(KC_I)), C(A(KC_O)),  __________,  KC_F14,
         __________,  C(A(KC_J)), C(A(KC_K)), C(A(KC_L)),  __________,  KC_F15,
         __________,  KC_BTN3, KC_MS_U, KC_BTN2,  __________, KC_F16,
                      KC_MS_L, KC_MS_D, KC_MS_R, __________,
         KC_MS_WH_LEFT, KC_MS_WH_RIGHT,
         KC_WH_U,
         KC_WH_D, KC_BTN4, KC_BTN1,
         C(A(KC_F7)), C(A(KC_F8))
    ),

[_PALM_3_MAC] = LAYOUT(
         HYPR(KC_INS), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), KC__VOLDOWN, KC__MUTE, KC__VOLUP, HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F9), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F10), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F11), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F12), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), KC_MRWD, KC_MPLY, KC_MFFD,
                                HYPR(KC_BSPC), A(KC_F13),
                                               A(KC_F14),
                       G(KC_Z), G(S(KC_Z)), HYPR(KC_DEL),
                                HYPR(KC_F5), HYPR(KC_F6),
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________, __________, __________,
         _,  _,
         _,
         _,  _,  _,
         PALM_3_MAC, HYPR(KC_F8)
    ),

[_PALM_4_MAC] = LAYOUT(
         HYPR(KC_INS), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), KC__VOLDOWN, KC__MUTE, KC__VOLUP, HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F9), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F10), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F11), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F12), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), KC_MRWD, KC_MPLY, KC_MFFD,
                                HYPR(KC_BSPC), A(KC_F13),
                                               A(KC_F14),
                       G(KC_Z), G(S(KC_Z)), HYPR(KC_DEL),
                                HYPR(KC_F5), HYPR(KC_F6),
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________, __________, __________,
         _,  _,
         _,
         _,  _,  _,
         HYPR(KC_F7), PALM_4_MAC
    ),

// base win layer
[_WIN] = LAYOUT(
           // left side
           _INS, KC_F1, KC_F2, KC_F3, KC_F4, TD(SET_TD), KC_F6, KC_F7, KC_F8,
           KC_F17, _1, _2_PLEFT, _3_SLASH, _4_PRGHT, _5_EQL,
           KC_F18, KC_Q, KC_W, KC_E, KC_R, KC_T,
           KC_F19, KC_A, KC_S, KC_D, KC_F, KC_G,
           KC_F20, KC_Z, KC_X, KC_C, KC_V, KC_B,
                KC_GRV, KC_LBRC, KC_COMM, KC_RBRC,
                                                 // left thumb keys
			                        CTRL_SHIFT_BS, TD(TAP_MACRO),
                                                     ALT_SLASH_WIN,
                           CTRL_ESC, SHIFT_ENTER_WIN, ALT_SHIFT_DEL,
                                                  // left palm keys
			                              PALM_1_WIN, PALM_2_WIN,
    // right side
    KC_F9, KC_F10, KC_F11, KC_F12, KC_F13, KC_F14, KC_F15, KEYB_CONTROL, TD(REST_TD),
  	_6_PLUS, _7_BANG, _8_DASH, _9_QUEST, _0, KC_F21,
  	KC_Y, KC_U, KC_I, KC_O, KC_P, KC_F22,
  	KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_F23,
  	KC_N, KC_M, KC_UP, KC_DOT, KC_QUOT, KC_F24,
  	KC_LEFT, KC_DOWN, KC_RGHT, KC_APP,
           // right thumb keys
           KC_F16, KC_BSPC,
           ALT_BSLS_WIN,
           RWIN, SHIFT_TAB_WIN, CTRL_SPACE,
           // right palm key
           PALM_3_WIN, PALM_4_WIN
    ),

// win-specific layers
[_CONTROL_ESCAPE] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                         _,  _,
                                       KC_LALT,
                    CTRL_ESC, KC_LSFT, KC_LGUI,
                                          _, _,

         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________,  __________,  SELECT_UP_WIN,  __________,  __________,  __________,
         SELECT_LEFT_WIN,  SELECT_DOWN_WIN,  SELECT_RIGHT_WIN, __________,
         __________, DEL_LEFT_WIN,
         _MINS,
         _EQL, KC_F2, LEAD_SPACE,
         KC_F15, KC_F16
    ),

[_CONTROL_SPACE] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                               DEL_LEFT_WIN, KC_F14,
                                            _SLSH,
                     KC_F3, _ENTER, DEL_RIGHT_WIN,
                                     KC_F13, KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
                             _,  _,
                           KC_LALT,
      KC_LGUI, KC_LSFT, CTRL_SPACE,
                                  _, _
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
                                         _, _,
         _,  _,  _,  _,  _,  _, _, _, _,
	       _, _, _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  __________,  _ ,  _,  _,
         __________, __________, __________, _,
         _,  _,
         _,
         _,  _,  _,
         _, _
    ),

[_ALT_SLASH_WIN] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                  _,  _,
                          ALT_SLASH_WIN,
              KC_LCTL, KC_LSFT, KC_LGUI,
                                      __________, _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
                  __________, __________,  __________, __________,
         __________, _BSPC,
         _MINS,
         _EQL, KC_F2, KC_F1,
         KC_F15, KC_F16
    ),

[_ALT_BSLS_WIN] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                     _BSPC, KC_F14,
                                           _SLSH,
                              _ESC, _ENTER, _DEL,
                                    KC_F13, KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
         _,  _,
         ALT_BSLS_WIN,
         KC_LGUI, KC_LSFT, KC_LCTL,
         _, _
    ),

[_ALT_SHIFT_DEL] = LAYOUT(
         W_INS, W_F1,  W_F2,  W_F3,  W_F4, W_F5, W_F6, W_F7, W_F8,
         __________, W_1, W_2, W_3, W_4, W_5,
         __________, W_Q, W_W, W_E, W_R, W_T,
         __________, W_A, W_S, W_D, W_F, W_G,
         __________, W_Z, W_X, W_C, W_V, W_B,
               W_GRV,  W_LBRC,  W_COMM,  W_RBRC,
                            _,  _,
                               KC_LALT,
       KC_LCTL, KC_LSFT, ALT_SHIFT_DEL,
                                    _, _,
         W_F9,  W_F10,  W_F11,  W_F12,  W_F13, W_F14, W_F15, W_F16, __________,
         W_6,  W_7,  W_8,  W_9,  W_0,  __________,
         W_Y,  W_U,  W_I,  W_O,  W_P,  __________,
         W_H,  W_J,  W_K, W_L,  W_QUOT,  __________,
         W_N, W_M, KC_UP, W_DOT, W_SCLN,  __________,
         KC_LEFT,  KC_DOWN, KC_RGHT, W_APP,
         _, W_BSPC,
         W_MINS,
         W_EQL, KC_F2, KC_F1,
         KC_F15, KC_F16
    ),

[_RWIN] = LAYOUT(
         LGUI(KC_INS), LGUI(KC_F1), LGUI(KC_F2), LGUI(KC_F3), LGUI(KC_F4), LGUI(KC_F5), LGUI(KC_F6), LGUI(KC_F7), LGUI(KC_F8),
         LGUI(KC_F17), LGUI(KC_1), LGUI(KC_2), LGUI(KC_3), LGUI(KC_4), LGUI(KC_5),
         LGUI(KC_F18), LGUI(KC_Q), LGUI(KC_W), LGUI(KC_E), LGUI(KC_R), LGUI(KC_T),
         LGUI(KC_F19), LGUI(KC_A), LGUI(KC_S), LGUI(KC_D), LGUI(KC_F), LGUI(KC_G),
         LGUI(KC_F20), LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), LGUI(KC_B),
                LGUI(KC_GRV), LGUI(KC_LBRC), LGUI(KC_COMM), LGUI(KC_RBRC),
                                     LGUI(KC_BSPC), LGUI(KC_F14),
                                             LGUI(KC_SLSH),
                            LGUI(KC_ESC), LGUI(KC_ENTER), LGUI(KC_DEL),
                                          LGUI(KC_F13), LGUI(KC_F14),
    LGUI(KC_F9), LGUI(KC_F10), LGUI(KC_F11), LGUI(KC_F12), LGUI(KC_F13), LGUI(KC_F14), LGUI(KC_F15), _, _,
	  LGUI(KC_6), LGUI(KC_7), LGUI(KC_8), LGUI(KC_9), LGUI(KC_0), LGUI(KC_F21),
  	LGUI(KC_Y), LGUI(KC_U), LGUI(KC_I), LGUI(KC_O), LGUI(KC_P), LGUI(KC_F22),
  	LGUI(KC_H), LGUI(KC_J), LGUI(KC_K), LGUI(KC_L), LGUI(KC_SCLN), LGUI(KC_F23),
  	LGUI(KC_N), LGUI(KC_M), LGUI(KC_UP), LGUI(KC_DOT), LGUI(KC_QUOT), LGUI(KC_F24),
  	LGUI(KC_LEFT), LGUI(KC_DOWN), LGUI(KC_RGHT), LGUI(KC_APP),
         LGUI(KC_F16), LGUI(KC_BSPC),
         LGUI(KC_MINS),
         RWIN, LGUI(KC_LSFT), LGUI(KC_LCTL),
         LGUI(KC_F15), LGUI(KC_F16)
    ),

[_SHIFT_ENTER_WIN] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                             _,  _,
                           KC_LALT,
 KC_LCTL, SHIFT_ENTER_WIN, KC_LGUI,
                                 __________, _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
	       KC_6, KC_7, KC_8, KC_9, KC_0, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
                   __________,  __________,  __________, __________,
         __________, _BSPC,
         _BSLS,
         KC_F17, _TAB, _SPACE,
         KC_F15, KC_F16
    ),


[_SHIFT_TAB_WIN] = LAYOUT(
         _INS,  __________,  __________,  __________,  __________,  KC_F5, __________, __________, __________,
         __________, KC_1, KC_2, KC_3, KC_4, KC_5,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
         __________, __________, __________, __________, __________, __________,
               __________,  __________,  __________,  __________,
                                     _BSPC, KC_F14,
                                           _SLSH,
                              _ESC, _ENTER, _DEL,
                                    KC_F13, KC_F14,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
         _,  _,
         KC_LALT,
         KC_LGUI, SHIFT_TAB_WIN, KC_LCTL,
         _, _
    ),

[_PALM_1_WIN] = LAYOUT(
         HYPR(KC_INS), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), KC_VOLD, KC_MUTE, KC_VOLU, HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), KC_MPRV, KC_MPLY, KC_MNXT,
                                HYPR(KC_BSPC), A(KC_F13),
                                               A(KC_F14),
                       C(KC_Z), C(S(KC_Z)), HYPR(KC_DEL),
                                           PALM_1_WIN, _,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  C(A(KC_7)), C(A(KC_8)), C(A(KC_9)),  __________,  __________,
         __________,  C(A(KC_U)), C(A(KC_I)), C(A(KC_O)),  __________,  __________,
         __________,  C(A(KC_J)), C(A(KC_K)), C(A(KC_L)),  __________,  __________,
         __________,  C(A(KC_M)), KC_PGUP, C(A(KC_DOT)),  __________, __________,
                      C(KC_HOME), KC_PGDN, C(KC_END), __________,
         C(A(KC_F16)), C(A(KC_BSPC)),
         C(A(KC_MINS)),
         C(A(KC_EQL)), FIND_PREV, FIND_NEXT,
         C(A(KC_F15)), C(A(KC_F16))
    ),

[_PALM_2_WIN] = LAYOUT(
         HYPR(KC_INS), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), KC_VOLD, KC_MUTE, KC_VOLU, HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), KC_MPRV, KC_MPLY, KC_MNXT,
                                HYPR(KC_BSPC), A(KC_F13),
                                               A(KC_F14),
                       C(KC_Z), C(S(KC_Z)), HYPR(KC_DEL),
                                           _, PALM_2_WIN,
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  C(A(KC_7)), C(A(KC_8)), C(A(KC_9)),  __________,  KC_F21,
         __________,  C(A(KC_U)), C(A(KC_I)), C(A(KC_O)),  __________,  KC_F22,
         __________,  C(A(KC_J)), C(A(KC_K)), C(A(KC_L)),  __________,  KC_F23,
         __________,  KC_BTN3, KC_MS_U, KC_BTN2,  __________, KC_F24,
                      KC_MS_L, KC_MS_D, KC_MS_R, __________,
         KC_MS_WH_LEFT, KC_MS_WH_RIGHT,
         KC_WH_U,
         KC_WH_D, KC_BTN4, KC_BTN1,
         C(A(KC_F15)), C(A(KC_F16))
    ),

[_PALM_3_WIN] = LAYOUT(
         HYPR(KC_INS), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), KC_VOLD, KC_MUTE, KC_VOLU, HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), KC_MPRV, KC_MPLY, KC_MNXT,
                                HYPR(KC_BSPC), A(KC_F13),
                                               A(KC_F14),
                       C(KC_Z), C(S(KC_Z)), HYPR(KC_DEL),
                              HYPR(KC_F13), HYPR(KC_F14),
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         KC_6,  KC_7,  KC_8,  KC_9,  KC_0,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
         _,  _,
         _,
         _,  _,  _,
         PALM_3_WIN, HYPR(KC_F16)
    ),

[_PALM_4_WIN] = LAYOUT(
         HYPR(KC_INS), HYPR(KC_F1), HYPR(KC_F2), HYPR(KC_F3), KC_VOLD, KC_MUTE, KC_VOLU, HYPR(KC_F7), HYPR(KC_F8),
         HYPR(KC_F17), HYPR(KC_1), HYPR(KC_2), HYPR(KC_3), HYPR(KC_4), HYPR(KC_5),
         HYPR(KC_F18), HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T),
         HYPR(KC_F19), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
         HYPR(KC_F20), HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B),
                 HYPR(KC_GRV), KC_MPRV, KC_MPLY, KC_MNXT,
                                HYPR(KC_BSPC), A(KC_F13),
                                               A(KC_F14),
                       C(KC_Z), C(S(KC_Z)), HYPR(KC_DEL),
                              HYPR(KC_F13), HYPR(KC_F14),
         __________,  __________,  __________,  __________,  __________,  __________, __________, __________, __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
         __________,  __________,  __________,  __________,  __________,  __________,
                   __________,  __________,  __________,  __________,
         _,  _,
         _,
         _,  _,  _,
         HYPR(KC_F15), PALM_4_WIN
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
                                 _, _,
         _,  _,  _,  _,  _,  _, _, KEYB_CONTROL, RESET,
	     _, _, _, _, _, _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _,  _,  _,
         _,  _,  _,  _ ,  _,  _,
                   _,  _,  _, _,
         _,  _,
         _,
         _,  _,  _,
         _, _
    )
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

void matrix_scan_user(void) {
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
     led_yellow_on(); _delay_ms(10); if (!macro1_recording) { led_red_off(); }
     led_green_on(); _delay_ms(10); led_yellow_off();
     led_blue_on(); _delay_ms(10);  if (!macro2_recording) { led_green_off(); }
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
    case _ALT_BSLS_MAC: down(KC_LALT); break;
    case _SHIFT_ENTER_MAC: down(KC_LSFT); break;
    case _SHIFT_TAB_MAC: down(KC_LSFT); break;
    case _CTRL_DEL: down(KC_LCTL); break;
    case _CTRL_F1: down(KC_LCTL); break;
    case _PALM_1_MAC: palm_1_mac_layer = true; down(KC_LALT); down(KC_LCTL); break;
    case _PALM_2_MAC: palm_2_mac_layer = true; down(KC_LALT); down(KC_LCTL); down(KC_LSFT); break;
    case _PALM_3_MAC: break;
    case _PALM_4_MAC: break;

    // win-specific
    case _CONTROL_ESCAPE: down(KC_LCTL); break;
    case _CONTROL_SPACE: down(KC_LCTL); break;
    case _CTRL_SHIFT_BS: down(KC_LCTL); down(KC_LSFT); break;
    case _ALT_SLASH_WIN: down(KC_LALT); break;
    case _ALT_BSLS_WIN: down(KC_LALT); break;
    case _SHIFT_ENTER_WIN: down(KC_LSFT); break;
    case _SHIFT_TAB_WIN: down(KC_LSFT); break;
    case _ALT_SHIFT_DEL: down(KC_LALT); down(KC_LSFT); break;
    case _RWIN: break;
    case _PALM_1_WIN: palm_1_win_layer = true; down(KC_LALT); down(KC_LCTL); break;
    case _PALM_2_WIN: palm_2_win_layer = true; down(KC_LALT); down(KC_LCTL); down(KC_LSFT); break;
    case _PALM_3_WIN: break;
    case _PALM_4_WIN: break;

    case _KEYB_CONTROL: capsOff(); all_leds_off(); break;

    // unregister everything (even if it was not pressed - no big deal; this works faster than getting pressed mods)
    default:
     remove_mods();
     default_layer = true;
     break;
    }
return state;
}

// support for "mo layer tap" functionality
bool cmd_esc_interrupted = true;
bool alt_shift_interrupted = true;
bool alt_slash_mac_interrupted = true;
bool alt_bsls_mac_interrupted = true;
bool shift_enter_mac_interrupted = true;
bool shift_tab_mac_interrupted = true;
bool ctrl_del_interrupted = true;
bool ctrl_f1_interrupted = true;
bool palm_1_mac_interrupted = true;
bool palm_2_mac_interrupted = true;
bool palm_3_mac_interrupted = true;
bool palm_4_mac_interrupted = true;

bool ctrl_esc_interrupted = true;
bool ctrl_shift_interrupted = true;
bool alt_slash_win_interrupted = true;
bool alt_bsls_win_interrupted = true;
bool shift_enter_win_interrupted = true;
bool shift_tab_win_interrupted = true;
bool alt_shift_del_interrupted = true;
bool rwin_interrupted = true;
bool palm_1_win_interrupted = true;
bool palm_2_win_interrupted = true;
bool palm_3_win_interrupted = true;
bool palm_4_win_interrupted = true;

bool left_pressed = false;
bool right_pressed = false;
bool space_alone = false;

// adding logic to custom keycodes and overriding existing ones (taking hold duration into account);
// "mo layer tap" and "esc leader key" functionality
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool pressed = record->event.pressed;

    if (space_alone && pressed) {
       space_alone = false;
    }

    if (default_layer) {
       // remove stuck modifiers
       remove_mods();
    }

    // remove modifiers for palm layers, if any key is pressed
    // (if actual mouse keys were pressed, such modifiers should still be on;
    // if virtual mouse keys were pressed, modifiers should be off)
    if (palm_1_mac_layer && pressed) {
       if (keycode != PALM_1_MAC) {
         up(KC_LALT); up(KC_LCTL); up(KC_LSFT);
         palm_1_mac_layer = false;
       }
    }
    if (palm_1_win_layer && pressed) {
       if (keycode != PALM_1_WIN) {
         up(KC_LALT); up(KC_LCTL); up(KC_LSFT);
         palm_1_win_layer = false;
       }
    }
    if (palm_2_mac_layer && pressed) {
       if (keycode != PALM_2_MAC) {
         up(KC_LALT); up(KC_LSFT); up(KC_LCTL); up(KC_LGUI);
         palm_2_mac_layer = false;
       }
    }
    if (palm_2_win_layer && pressed) {
       if (keycode != PALM_2_WIN) {
         up(KC_LALT); up(KC_LSFT); up(KC_LCTL); up(KC_LGUI);
         palm_2_win_layer = false;
       }
    }

    if (keycode != KC_LEFT && keycode != KC_RGHT) {
      if (keycode != CMD_SPACE && keycode != CTRL_SPACE) {
             esc_timer = 0;
      }
    }

    // custom dynamic macros do no currently play nicely with standard LT functionality and repeating keycodes;
    // following code disables those if any of the macros is being currently recorded
    if ((macro1_recording || macro2_recording) && pressed) {
      if (keycode == CTRL_SPACE || keycode == CMD_SPACE) {
        key_code(KC_SPC);
        return false;
      }

      if (keycode == ALT_SHIFT_BS || keycode == CTRL_SHIFT_BS || keycode == KC_BSPC) {
        key_code(KC_BSPC);
        return false;
      }

      if (keycode == KC_LEFT || keycode == KC_RGHT) {
        key_code(keycode);
        return false;
      }
    }

    // support for "mo layer tap" functionality
    if (keycode != CMD_ESC) { cmd_esc_interrupted = true; }
    if (keycode != ALT_SHIFT_BS) { alt_shift_interrupted = true; }
    if (keycode != ALT_SLASH_MAC) { alt_slash_mac_interrupted = true; }
    if (keycode != ALT_BSLS_MAC) { alt_bsls_mac_interrupted = true; }
    if (keycode != SHIFT_ENTER_MAC) { shift_enter_mac_interrupted = true; }
    if (keycode != SHIFT_TAB_MAC) { shift_tab_mac_interrupted = true; }
    if (keycode != CTRL_DEL) { ctrl_del_interrupted = true; }
    if (keycode != CTRL_F1) { ctrl_f1_interrupted = true; }
    if (keycode != PALM_1_MAC) { palm_1_mac_interrupted = true; }
    if (keycode != PALM_2_MAC) { palm_2_mac_interrupted = true; }
    if (keycode != PALM_3_MAC) { palm_3_mac_interrupted = true; }
    if (keycode != PALM_4_MAC) { palm_4_mac_interrupted = true; }

    if (keycode != CTRL_ESC) { ctrl_esc_interrupted = true; }
    if (keycode != CTRL_SHIFT_BS) { ctrl_shift_interrupted = true; }
    if (keycode != ALT_SLASH_WIN) { alt_slash_win_interrupted = true; }
    if (keycode != ALT_BSLS_WIN) { alt_bsls_win_interrupted = true; }
    if (keycode != SHIFT_ENTER_WIN) { shift_enter_win_interrupted = true; }
    if (keycode != SHIFT_TAB_WIN) { shift_tab_win_interrupted = true; }
    if (keycode != ALT_SHIFT_DEL) { alt_shift_del_interrupted = true; }
    if (keycode != RWIN) { rwin_interrupted = true; }
    if (keycode != PALM_1_WIN) { palm_1_win_interrupted = true; }
    if (keycode != PALM_2_WIN) { palm_2_win_interrupted = true; }
    if (keycode != PALM_3_WIN) { palm_3_win_interrupted = true; }
    if (keycode != PALM_4_WIN) { palm_4_win_interrupted = true; }

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
        case KC_Q: { return lead_custom_autoshifted(KC_Q, isMac ? KC_F13 : KC_Q, KC_Q, KC_LSFT, pressed, AUTOSHIFT_QWERTY_KEYS_TERM); }
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
        case KC_H: { return lead_custom_autoshifted(KC_H, isMac ? KC_F16 : KC_H, KC_H, KC_LSFT, pressed, AUTOSHIFT_QWERTY_KEYS_TERM); }
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

        case KC_1: { return lead_autoshifted_numbers(KC_1, pressed); }
        case KC_2: { return lead_autoshifted_numbers(KC_2, pressed); }
        case KC_3: { return lead_autoshifted_numbers(KC_3, pressed); }
        case KC_4: { return lead_autoshifted_numbers(KC_4, pressed); }
        case KC_5: { return lead_autoshifted_numbers(KC_5, pressed); }
        case KC_6: { return lead_autoshifted_numbers(KC_6, pressed); }
        case KC_7: { return lead_autoshifted_numbers(KC_7, pressed); }
        case KC_8: { return lead_autoshifted_numbers(KC_8, pressed); }
        case KC_9: { return lead_autoshifted_numbers(KC_9, pressed); }
        case KC_0: { return lead_autoshifted_numbers(KC_0, pressed); }

        case KC_F1: { return lead_autoshifted_special(KC_F1, pressed); }
        case KC_F2: { return lead_autoshifted_special(KC_F2, pressed); }
        case KC_F3: { return lead_autoshifted_special(KC_F3, pressed); }
        case KC_F4: { return lead_autoshifted_special(KC_F4, pressed); }
        case KC_F5: { return lead_autoshifted_special(KC_F5, pressed); }
        case KC_F6: { return lead_autoshifted_special(KC_F6, pressed); }
        case KC_F7: { return lead_autoshifted_special(KC_F7, pressed); }
        case KC_F8: { return lead_autoshifted_special(KC_F8, pressed); }
        case KC_F9: { return lead_autoshifted_special(KC_F9, pressed); }
        case KC_F10: { return lead_autoshifted_special(KC_F10, pressed); }
        case KC_F11: { return lead_autoshifted_special(KC_F11, pressed); }
        case KC_F12: { return lead_autoshifted_special(KC_F12, pressed); }
        case KC_F13: { return lead_autoshifted_special(KC_F13, pressed); }
        case KC_F14: { return lead_autoshifted_special(KC_F14, pressed); }
        case KC_F15: { return lead_autoshifted_special(KC_F15, pressed); }
        case KC_F16: { return lead_autoshifted_special(KC_F16, pressed); }
        case KC_F17: { return lead_autoshifted_special(KC_F17, pressed); }
        case KC_F18: { return lead_autoshifted_special(KC_F18, pressed); }
        case KC_F19: { return lead_autoshifted_special(KC_F19, pressed); }
        case KC_F20: { return lead_autoshifted_special(KC_F20, pressed); }
        case KC_F21: { return lead_autoshifted_special(KC_F21, pressed); }
        case KC_F22: { return lead_autoshifted_special(KC_F22, pressed); }
        case KC_F23: { return lead_autoshifted_special(KC_F23, pressed); }
        case KC_F24: { return lead_autoshifted_special(KC_F24, pressed); }

        case KC_LBRC: { return lead_autoshifted_special(KC_LBRC, pressed); }
        case KC_RBRC: { return lead_autoshifted_special(KC_RBRC, pressed); }
        case KC_COMM: { return lead_autoshifted_special(KC_COMM, pressed); }
        case KC_SCLN: { return lead_autoshifted_special(KC_SCLN, pressed); }
        case KC_QUOT: { return lead_autoshifted_special(KC_QUOT, pressed); }
        case KC_GRV: { return lead_autoshifted_special(KC_GRV, pressed); }
        case KC_NUBS: { return lead_autoshifted_special(KC_NUBS, pressed); }
        case KC_APP: { return lead_autoshifted_special(KC_APP, pressed); }
        case KC_DOT: { return lead_autoshifted_special(KC_DOT, pressed); }

        // custom autoshifted keys - when pressed, other key + shift is sent
        case _1: { return lead_autoshifted_modified_numbers(KC_1, KC_1, KC_NO, pressed); }
        case _2_PLEFT: { return lead_autoshifted_modified_numbers(KC_2, KC_9, KC_LSFT, pressed); }
        case _3_SLASH: { return lead_autoshifted_modified_numbers(KC_3, KC_MINS, KC_LSFT, pressed); }
        case _4_PRGHT: { return lead_autoshifted_modified_numbers(KC_4, KC_0, KC_LSFT, pressed); }
        case _5_EQL: { return lead_autoshifted_modified_numbers(KC_5, KC_EQL, KC_NO, pressed); }
        case _6_PLUS: { return lead_autoshifted_modified_numbers(KC_6, KC_EQL, KC_LSFT, pressed); }
        case _7_BANG: { return lead_autoshifted_modified_numbers(KC_7, KC_1, KC_LSFT, pressed); }
        case _8_DASH: { return lead_autoshifted_modified_numbers(KC_8, KC_MINS, KC_NO, pressed); }
        case _9_QUEST: { return lead_autoshifted_modified_numbers(KC_9, KC_SLSH, KC_LSFT, pressed); }
        case _0: { return lead_autoshifted_modified_numbers(KC_0, KC_0, KC_NO, pressed); }

        case ALT_F9: { return lead_custom_autoshifted_with_mods(KC_F9, KC_LALT, KC_F9, KC_F9, KC_LALT, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case ALT_F10: { return lead_custom_autoshifted_with_mods(KC_F10, KC_LALT, KC_F10, KC_F10, KC_LALT, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case ALT_F11: { return lead_custom_autoshifted_with_mods(KC_F11, KC_LALT, KC_F11, KC_F11, KC_LALT, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case ALT_F12: { return lead_custom_autoshifted_with_mods(KC_F12, KC_LALT, KC_F12, KC_F12, KC_LALT, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case ALT_F13: { return lead_custom_autoshifted_with_mods(KC_F13, KC_LALT, KC_F13, KC_F13, KC_LALT, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case ALT_F14: { return lead_custom_autoshifted_with_mods(KC_F14, KC_LALT, KC_F14, KC_F14, KC_LALT, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case ALT_F15: { return lead_custom_autoshifted_with_mods(KC_F15, KC_LALT, KC_F15, KC_F15, KC_LALT, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case ALT_F16: { return lead_custom_autoshifted_with_mods(KC_F16, KC_LALT, KC_F16, KC_F16, KC_LALT, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }

        case KC_UP: { if (is_after_lead(KC_UP, pressed)) { return false; } return true; }
        case KC_DOWN: { if (is_after_lead(KC_DOWN, pressed)) { return false; } return true; }
        case KC_BSPC: { if (is_after_lead(KC_BSPC, pressed)) { return false; } return true; }

        // >>>>>>> escape as additional leader key
        case KC_LEFT: {
          if (pressed) { left_pressed = true; } else { left_pressed = false; }
          if (is_after_lead(KC_LEFT, pressed)) { return false; }
          return not_following_esc(KC_HOME, isMac ? KC_LGUI : KC_NO, KC_NO, KC_NO, &esc_timer, pressed, 300);
        }
        case KC_RGHT: {
          if (pressed) { right_pressed = true; } else { right_pressed = false; }
          if (is_after_lead(KC_RGHT, pressed)) { return false; }
          return not_following_esc(KC_END, isMac ? KC_LGUI : KC_NO, KC_NO, KC_NO, &esc_timer, pressed, 300);
        }

        // <<<<<<< escape as additional leader key

        // >>>>>>> mac layers
        case CMD_SPACE: {
          if (is_after_lead(KC_SPC, pressed)) { return false; };
          if (pressed) {
            space_alone = true;
            space_timer = timer_read();
          }
          else {
            uint16_t delta_millis = timer_elapsed(space_timer);
            if (space_alone && ((delta_millis > 1) && (delta_millis < 50))) {
              up(KC_LGUI); key_code(KC_SPC);
            } else if (space_alone && ((delta_millis >= 50) && (delta_millis < 850))) {
              up(KC_LGUI); with_1_mod(KC_SPC, KC_LALT); // change lang
              lang_switch_led = true;
              capsOff(); // on mac changing language resets caps lock
            }
            space_alone = false;
            space_timer = 0;
          }
          return true;
        }

        case CMD_ESC: {
          if (left_pressed || right_pressed) { return false; }
          if (is_after_lead(KC_F3, pressed)) { return false; }
          static uint16_t cmd_esc_layer_timer;
          if (momentary_layer_tap(KC_ESC, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, &cmd_esc_layer_timer, &cmd_esc_interrupted, pressed, 200, true)) {
            esc_timer = timer_read();
          }
          return true;
        }

        case ALT_SHIFT_BS: {
          if (is_after_lead(KC_BSPC, pressed)) { return false; }
          static uint16_t alt_shift_layer_timer;
          momentary_layer_tap(KC_BSPC, KC_NO, KC_LALT, KC_LSFT, KC_NO, KC_NO, &alt_shift_layer_timer, &alt_shift_interrupted, pressed, AUTOSHIFT_SPECIAL_TERM, true);
          return true;
        }

        case CTRL_DEL: {
          if (is_after_lead(KC_DEL, pressed)) { return false; }
          static uint16_t ctrl_del_layer_timer;
          momentary_layer_tap(KC_DEL, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_del_layer_timer, &ctrl_del_interrupted, pressed, 250, true);
          return true;
        }

        case CTRL_F1: {
          if (is_after_lead(KC_F17, pressed)) { return false; }
          static uint16_t ctrl_f1_layer_timer;
          momentary_layer_tap_with_hold(KC_F1, KC_LCTL, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_f1_layer_timer, &ctrl_f1_interrupted, pressed, AUTOSHIFT_SPECIAL_TERM, 350, false, KC_7, KC_LSFT, KC_NO);
          return true;
        }

        case ALT_SLASH_MAC: {
          if (is_after_lead(KC_SLSH, pressed)) { return false; }
          static uint16_t alt_slash_mac_layer_timer;
          momentary_layer_tap(KC_SLSH, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_slash_mac_layer_timer, &alt_slash_mac_interrupted, pressed, 250, true);
          return true;
        }

        case ALT_BSLS_MAC: {
          if (is_after_lead(KC_BSLS, pressed)) { return false; }
          static uint16_t alt_bsls_mac_layer_timer;
          momentary_layer_tap_with_hold(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bsls_mac_layer_timer, &alt_bsls_mac_interrupted, pressed, AUTOSHIFT_SPECIAL_TERM, 350, false, KC_BSLS, KC_LSFT, KC_NO);
          return true;
        }

        case SHIFT_ENTER_MAC: {
          if (is_after_lead(KC_ENTER, pressed)) { return false; }
          static uint16_t shift_enter_mac_layer_timer;
          momentary_layer_tap(KC_ENTER, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_enter_mac_layer_timer, &shift_enter_mac_interrupted, pressed, 300, true);
          return true;
        }

        case SHIFT_TAB_MAC: {
          if (caps_on) { capsOff(); return false; }
          if (is_after_lead(KC_F2, pressed)) { return false; }
          static uint16_t shift_tab_mac_layer_timer;
          if ((momentary_layer_tap_with_hold(KC_TAB, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_tab_mac_layer_timer, &shift_tab_mac_interrupted, pressed, 200, 1000, false, KC_CLR, KC_NO, KC_NO)) == 2) {
            // held key
            toggleCaps();
          }
          return true;
        }

        case PALM_1_MAC: {
          if (is_after_lead(KC_F5, pressed)) { return false; }
          static uint16_t palm_1_mac_layer_timer;
          momentary_layer_tap(KC_F5, KC_LALT, KC_LCTL, KC_NO, KC_NO, KC_NO, &palm_1_mac_layer_timer, &palm_1_mac_interrupted, pressed, 300, true);
          return true;
        }

        case PALM_2_MAC: {
          if (is_after_lead(KC_F6, pressed)) { return false; }
          static uint16_t palm_2_mac_layer_timer;
          momentary_layer_tap(KC_F6, KC_LALT, KC_LCTL, KC_LSFT, KC_NO, KC_NO, &palm_2_mac_layer_timer, &palm_2_mac_interrupted, pressed, 300, true);
          return true;
        }

        case PALM_3_MAC: {
          if (is_after_lead(KC_F7, pressed)) { return false; }
          static uint16_t palm_3_mac_layer_timer;
          momentary_layer_tap_with_hold(KC_F7, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, &palm_3_mac_layer_timer, &palm_3_mac_interrupted, pressed, 200, 1000, true, KC_F7, KC_LALT, KC_LSFT);
          return true;
        }

        case PALM_4_MAC: {
          if (is_after_lead(KC_F8, pressed)) { return false; }
          static uint16_t palm_4_mac_layer_timer;
          momentary_layer_tap_with_hold(KC_TAB, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_NO, &palm_4_mac_layer_timer, &palm_4_mac_interrupted, pressed, 200, 1000, true, KC_F8, KC_LALT, KC_LSFT);
          return true;
        }

        // >>>>>>> win layers
        case CTRL_SPACE: {
          if (is_after_lead(KC_SPC, pressed)) { return false; };
          if (pressed) {
            space_alone = true;
            space_timer = timer_read();
          }
          else {
            uint16_t delta_millis = timer_elapsed(space_timer);
            if (space_alone && ((delta_millis > 1) && (delta_millis < 50))) {
              up(KC_LCTL); key_code(KC_SPC);
            } else if (space_alone && ((delta_millis >= 50) && (delta_millis < 850))) {
              up(KC_LCTL); with_1_mod(KC_SPC, KC_LGUI); // change lang
              lang_switch_led = true;
            }
            space_alone = false;
            space_timer = 0;
          }
          return true;
        }

        case CTRL_ESC: {
          if (left_pressed || right_pressed) { return false; }
          if (is_after_lead(KC_F3, pressed)) { return false; }
          static uint16_t ctrl_esc_layer_timer;
          if (momentary_layer_tap(KC_ESC, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, &ctrl_esc_layer_timer, &ctrl_esc_interrupted, pressed, 200, true)) {
            esc_timer = timer_read();
          }
          return true;
        }

        case CTRL_SHIFT_BS: {
          if (is_after_lead(KC_BSPC, pressed)) { return false; }
          static uint16_t ctrl_shift_layer_timer;
          momentary_layer_tap(KC_BSPC, KC_NO, KC_LCTL, KC_LSFT, KC_NO, KC_NO, &ctrl_shift_layer_timer, &ctrl_shift_interrupted, pressed, AUTOSHIFT_SPECIAL_TERM, true);
          return true;
        }

        case ALT_SHIFT_DEL: {
          if (is_after_lead(KC_DEL, pressed)) { return false; }
          static uint16_t alt_shift_del_layer_timer;
          momentary_layer_tap(KC_DEL, KC_NO, KC_LALT, KC_LSFT, KC_NO, KC_NO, &alt_shift_del_layer_timer, &alt_shift_del_interrupted, pressed, 250, false);
          return true;
        }

        case RWIN: {
          if (is_after_lead(KC_F17, pressed)) { return false; }
          static uint16_t rwin_layer_timer;
          momentary_layer_tap_with_hold(KC_LGUI, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &rwin_layer_timer, &rwin_interrupted, pressed, AUTOSHIFT_SPECIAL_TERM, 350, false, KC_7, KC_LSFT, KC_NO);
          return true;
        }

        case ALT_BSLS_WIN: {
          if (is_after_lead(KC_BSLS, pressed)) { return false; }
          static uint16_t alt_bsls_win_layer_timer;
          momentary_layer_tap_with_hold(KC_BSLS, KC_NO, KC_LALT, KC_NO, KC_NO, KC_NO, &alt_bsls_win_layer_timer, &alt_bsls_win_interrupted, pressed, AUTOSHIFT_SPECIAL_TERM, 350, false, KC_BSLS, KC_LSFT, KC_NO);
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
          if (caps_on) { capsOff(); return false; }
          if (is_after_lead(KC_F2, pressed)) { return false; }
          static uint16_t shift_tab_win_layer_timer;
          if ((momentary_layer_tap_with_hold(KC_TAB, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, &shift_tab_win_layer_timer, &shift_tab_win_interrupted, pressed, 200, 1000, false, KC_CLR, KC_NO, KC_NO)) == 2) {
            // held key
            toggleCaps();
          }
          return true;
        }

       case PALM_1_WIN: {
          if (is_after_lead(KC_F13, pressed)) { return false; }
          static uint16_t palm_1_win_layer_timer;
          momentary_layer_tap(KC_F13, KC_NO, KC_LALT, KC_LCTL, KC_NO, KC_NO, &palm_1_win_layer_timer, &palm_1_win_interrupted, pressed, 300, false);
          return true;
        }

        case PALM_2_WIN: {
          if (is_after_lead(KC_F14, pressed)) { return false; }
          static uint16_t palm_2_win_layer_timer;
          momentary_layer_tap(KC_F14, KC_NO, KC_LALT, KC_LCTL, KC_LSFT, KC_NO, &palm_2_win_layer_timer, &palm_2_win_interrupted, pressed, 300, false);
          return true;
        }

        case PALM_3_WIN: {
          if (is_after_lead(KC_F15, pressed)) { return false; }
          static uint16_t palm_3_win_layer_timer;
          momentary_layer_tap_with_hold(KC_F15, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &palm_3_win_layer_timer, &palm_3_win_interrupted, pressed, 200, 1000, false, KC_F15, KC_LSFT, KC_NO);
          return true;
        }

        case PALM_4_WIN: {
          if (is_after_lead(KC_F16, pressed)) { return false; }
          static uint16_t palm_4_win_layer_timer;
          momentary_layer_tap_with_hold(KC_F16, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, &palm_4_win_layer_timer, &palm_4_win_interrupted, pressed, 200, 1000, false, KC_F16, KC_LSFT, KC_NO);
          return true;
        }

        // <<<<<<< layers
        // <<<<<<< KEYS, RESPONDING TO LEAD_SPACE SEQUENCE

        // additional auto-shifted keys
        case _ENTER: { return if_held_autoshift(KC_ENTER, pressed); }
        case _ESC: { return if_held_autoshift(KC_ESC, pressed); }
        case _TAB: { return if_held_autoshift(KC_TAB, pressed); }
        case _SPACE: { return if_held_autoshift(KC_SPC, pressed); }
        case _BSPC: { return if_held_autoshift(KC_BSPC, pressed); }
        case _DEL: { return if_held_autoshift(KC_DEL, pressed); }
        case _BSLS: { return if_held_autoshift(KC_BSLS, pressed); }
        case _SLSH: { return if_held_autoshift(KC_SLSH, pressed); }
        case _MINS: { return if_held_autoshift(KC_MINS, pressed); }
        case _INS: { return if_held_autoshift(KC_INS, pressed); }
        case _EQL: { return if_held_add_mods(KC_EQL, KC_LSFT, KC_NO, pressed, 200); }

        // delete words / lines
        case DEL_LEFT_MAC: { return delete_word_line(KC_BSPC, KC_LGUI, KC_LALT, KC_LEFT, KC_LGUI, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case DEL_RIGHT_MAC: { return delete_word_line(KC_DEL, KC_LGUI, KC_LALT, KC_RGHT, KC_LGUI, KC_LSFT, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case DEL_LEFT_WIN: { return delete_word_line(KC_BSPC, KC_LCTL, KC_LCTL, KC_HOME, KC_LSFT, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case DEL_RIGHT_WIN: { return delete_word_line(KC_DEL, KC_LCTL, KC_LCTL, KC_END, KC_LSFT, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM); }

        // select block of text
        case SELECT_UP_MAC: { return if_held_autoshift(KC_UP, pressed); }
        case SELECT_DOWN_MAC: { return if_held_autoshift(KC_DOWN, pressed); }
        case SELECT_UP_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_UP, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LCTL, KC_LSFT, KC_NO, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM, true); }
        case SELECT_DOWN_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOWN, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LCTL, KC_LSFT, KC_NO, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM, false); }
        case SELECT_LEFT_MAC: { return if_held_add_mods(KC_LEFT, KC_LSFT, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case SELECT_RIGHT_MAC: { return if_held_add_mods(KC_RGHT, KC_LSFT, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM); }
        case SELECT_LEFT_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_LEFT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_HOME, KC_LSFT, KC_NO, KC_NO, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM, true); }
        case SELECT_RIGHT_WIN: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_RGHT, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_END, KC_LSFT, KC_NO, KC_NO, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM, true); }

        // f3 / shift + f3
        case FIND_NEXT: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F3, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F3, KC_NO, KC_NO, KC_NO, KC_NO, pressed, AUTOSHIFT_QWERTY_KEYS_TERM, true); }
        case FIND_PREV: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_F3, KC_NO, KC_NO, KC_NO, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_F3, KC_LSFT, KC_NO, KC_NO, KC_NO, pressed, AUTOSHIFT_QWERTY_KEYS_TERM, true); }

        // mac overrides
        case CTRL_DOT: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_DOT, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_DOT, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, AUTOSHIFT_SPECIAL_TERM, true); }
        case CTRL_H: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_H, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_H, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, AUTOSHIFT_QWERTY_KEYS_TERM, true); }
        case CTRL_M: { return replace_key_and_mods_if_held_replace_key_and_mods(KC_M, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_M, KC_LGUI, KC_LSFT, KC_NO, KC_NO, pressed, AUTOSHIFT_QWERTY_KEYS_TERM, true); }

        // win overrides
        case W_F1: { return replace_ctrl_alt_with_lgui(KC_F1, pressed); }
        case W_F2: { return replace_ctrl_alt_with_lgui(KC_F2, pressed); }
        case W_F3: { return replace_ctrl_alt_with_lgui(KC_F3, pressed); }
        case W_F4: { return replace_ctrl_alt_with_lgui(KC_F4, pressed); }
        case W_F5: { return replace_ctrl_alt_with_lgui(KC_F5, pressed); }
        case W_F6: { return replace_ctrl_alt_with_lgui(KC_F6, pressed); }
        case W_F7: { return replace_ctrl_alt_with_lgui(KC_F7, pressed); }
        case W_F8: { return replace_ctrl_alt_with_lgui(KC_F8, pressed); }
        case W_F9: { return replace_ctrl_alt_with_lgui(KC_F9, pressed); }
        case W_F10: { return replace_ctrl_alt_with_lgui(KC_F10, pressed); }
        case W_F11: { return replace_ctrl_alt_with_lgui(KC_F11, pressed); }
        case W_F12: { return replace_ctrl_alt_with_lgui(KC_F12, pressed); }
        case W_F13: { return replace_ctrl_alt_with_lgui(KC_F13, pressed); }
        case W_F14: { return replace_ctrl_alt_with_lgui(KC_F14, pressed); }
        case W_F15: { return replace_ctrl_alt_with_lgui(KC_F15, pressed); }
        case W_F16: { return replace_ctrl_alt_with_lgui(KC_F16, pressed); }
        case W_1: { return replace_ctrl_alt_with_lgui(KC_1, pressed); }
        case W_2: { return replace_ctrl_alt_with_lgui(KC_2, pressed); }
        case W_3: { return replace_ctrl_alt_with_lgui(KC_3, pressed); }
        case W_4: { return replace_ctrl_alt_with_lgui(KC_4, pressed); }
        case W_5: { return replace_ctrl_alt_with_lgui(KC_5, pressed); }
        case W_6: { return replace_ctrl_alt_with_lgui(KC_6, pressed); }
        case W_7: { return replace_ctrl_alt_with_lgui(KC_7, pressed); }
        case W_8: { return replace_ctrl_alt_with_lgui(KC_8, pressed); }
        case W_9: { return replace_ctrl_alt_with_lgui(KC_9, pressed); }
        case W_0: { return replace_ctrl_alt_with_lgui(KC_0, pressed); }
        case W_Q: { return replace_ctrl_alt_with_lgui(KC_Q, pressed); }
        case W_W: { return replace_ctrl_alt_with_lgui(KC_W, pressed); }
        case W_E: { return replace_ctrl_alt_with_lgui(KC_E, pressed); }
        case W_R: { return replace_ctrl_alt_with_lgui(KC_R, pressed); }
        case W_T: { return replace_ctrl_alt_with_lgui(KC_T, pressed); }
        case W_Y: { return replace_ctrl_alt_with_lgui(KC_Y, pressed); }
        case W_U: { return replace_ctrl_alt_with_lgui(KC_U, pressed); }
        case W_I: { return replace_ctrl_alt_with_lgui(KC_I, pressed); }
        case W_O: { return replace_ctrl_alt_with_lgui(KC_O, pressed); }
        case W_P: { return replace_ctrl_alt_with_lgui(KC_P, pressed); }
        case W_A: { return replace_ctrl_alt_with_lgui(KC_A, pressed); }
        case W_S: { return replace_ctrl_alt_with_lgui(KC_S, pressed); }
        case W_D: { return replace_ctrl_alt_with_lgui(KC_D, pressed); }
        case W_F: { return replace_ctrl_alt_with_lgui(KC_F, pressed); }
        case W_G: { return replace_ctrl_alt_with_lgui(KC_G, pressed); }
        case W_H: { return replace_ctrl_alt_with_lgui(KC_H, pressed); }
        case W_J: { return replace_ctrl_alt_with_lgui(KC_J, pressed); }
        case W_K: { return replace_ctrl_alt_with_lgui(KC_K, pressed); }
        case W_L: { return replace_ctrl_alt_with_lgui(KC_L, pressed); }
        case W_SCLN: { return replace_ctrl_alt_with_lgui(KC_SCLN, pressed); }
        case W_Z: { return replace_ctrl_alt_with_lgui(KC_Z, pressed); }
        case W_X: { return replace_ctrl_alt_with_lgui(KC_X, pressed); }
        case W_C: { return replace_ctrl_alt_with_lgui(KC_C, pressed); }
        case W_V: { return replace_ctrl_alt_with_lgui(KC_V, pressed); }
        case W_B: { return replace_ctrl_alt_with_lgui(KC_B, pressed); }
        case W_N: { return replace_ctrl_alt_with_lgui(KC_N, pressed); }
        case W_M: { return replace_ctrl_alt_with_lgui(KC_M, pressed); }
        case W_DOT: { return replace_ctrl_alt_with_lgui(KC_DOT, pressed); }
        case W_QUOT: { return replace_ctrl_alt_with_lgui(KC_QUOT, pressed); }
        case W_LBRC: { return replace_ctrl_alt_with_lgui(KC_LBRC, pressed); }
        case W_COMM: { return replace_ctrl_alt_with_lgui(KC_COMM, pressed); }
        case W_RBRC: { return replace_ctrl_alt_with_lgui(KC_RBRC, pressed); }
        case W_BSPC: { return replace_ctrl_alt_with_lgui(KC_BSPC, pressed); }
        case W_GRV: { return replace_ctrl_alt_with_lgui(KC_GRV, pressed); }
        case W_APP: { return replace_ctrl_alt_with_lgui(KC_APP, pressed); }
        case W_INS: { return replace_ctrl_alt_with_lgui(KC_INS, pressed); }
        case W_MINS: { return replace_ctrl_alt_with_lgui(KC_MINS, pressed); }
        case W_EQL: { return replace_ctrl_alt_with_lgui(KC_EQL, pressed); }

        default: {
          return true;
        }
    }
}

void led_set_user(uint8_t usb_led) {
  if (!init_complete) { return; }

  caps_led ? led_blue_on() : led_blue_off();
  lead_led ? switch_lead_led_on() : switch_lead_led_off();

  if (macro1_recording) {
    led_red_on();
  } else {
    if (!lead_led) {
      led_red_off();
    }
  }

  if (macro2_recording) {
    led_green_on();
  } else {
    if (!lead_led) {
      led_green_off();
    }
  }
}
