#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#define TAPPING_TERM 150

#define AUTOSHIFT_QWERTY_KEYS_TERM 9999 // usual a-z keys
#define AUTOSHIFT_MODIFIED_NUMBERS_TERM 150 // 2 -> (
#define AUTOSHIFT_SPECIAL_TERM 150 // ' -> "
#define AUTOSHIFT_NUMBERS_TERM 150 // mod + numbers: cmd + 2 -> cmd + shift + 2

#define MACRO_BUFFER 1024 // real size of buffer will be less: it holds 2 keys for a single press + modifiers; roughly / 2.5

#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY 50 // Delay between pressing a movement key and cursor movement

#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 16 // Time between cursor movements

#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 10 // Time until maximum cursor speed is reached. Setting MOUSEKEY_TIME_TO_MAX or MOUSEKEY_WHEEL_TIME_TO_MAX to 0 will disable acceleration for the cursor or scrolling respectively

#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED 2 // Maximum cursor speed at which acceleration stops

#undef MOUSEKEY_WHEEL_TIME_TO_MAX
#define MOUSEKEY_WHEEL_TIME_TO_MAX 0

#undef MOUSEKEY_WHEEL_MAX_SPEED
#define MOUSEKEY_WHEEL_MAX_SPEED 0 // Maximum number of scroll steps per scroll action

#include "../../config.h"
#endif

// make kinesis/stapelberg:themoon
