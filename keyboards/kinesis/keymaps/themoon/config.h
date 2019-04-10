#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#define TAPPING_TERM 150

#define AUTOSHIFT_QWERTY_KEYS_TERM 150 // usual a-z keys
#define AUTOSHIFT_MODIFIED_NUMBERS_TERM 150 // 2 -> (
#define AUTOSHIFT_SPECIAL_TERM 150 // ' -> "
#define AUTOSHIFT_NUMBERS_TERM 150 // mod + numbers: cmd + 2 -> cmd + shift + 2

#define MACRO_BUFFER 1024 // real size of buffer will be less: it holds 2 keys for a single press + modifiers; roughly / 2.5

#include "../../config.h"
#endif
