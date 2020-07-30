#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#define QUAD_TAP_TIMEOUT 400

#define DEF_DM1 "_"
#define DEF_DM2 "_"

#define TAPPING_TERM 150
#define TAPPING_FORCE_HOLD
#define AUTOSHIFT_QWERTY_KEYS_NO_MODIFIERS_TERM 9999 // usual a-z keys without any modifiers
#define AUTOSHIFT_QWERTY_KEYS_WITH_MODIFIERS_TERM 9999 // usual a-z keys with any modifier
#define AUTOSHIFT_MODIFIED_NUMBERS_TERM 150 // 2 -> (
#define AUTOSHIFT_SPECIAL_TERM 150 // ' -> "
#define AUTOSHIFT_NUMBERS_TERM 150 // mod + numbers: cmd + 2 -> cmd + shift + 2

#define MACRO_BUFFER 1024 // real size of buffer will be less: it holds 2 keys for a single press + modifiers; roughly / 2.5

#include "../../config.h"
#endif
