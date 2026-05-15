#pragma once

typedef enum APOLLO_KEY
{
    APOLLO_KEY_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    APOLLO_KEY_A = 4,
    APOLLO_KEY_B = 5,
    APOLLO_KEY_C = 6,
    APOLLO_KEY_D = 7,
    APOLLO_KEY_E = 8,
    APOLLO_KEY_F = 9,
    APOLLO_KEY_G = 10,
    APOLLO_KEY_H = 11,
    APOLLO_KEY_I = 12,
    APOLLO_KEY_J = 13,
    APOLLO_KEY_K = 14,
    APOLLO_KEY_L = 15,
    APOLLO_KEY_M = 16,
    APOLLO_KEY_N = 17,
    APOLLO_KEY_O = 18,
    APOLLO_KEY_P = 19,
    APOLLO_KEY_Q = 20,
    APOLLO_KEY_R = 21,
    APOLLO_KEY_S = 22,
    APOLLO_KEY_T = 23,
    APOLLO_KEY_U = 24,
    APOLLO_KEY_V = 25,
    APOLLO_KEY_W = 26,
    APOLLO_KEY_X = 27,
    APOLLO_KEY_Y = 28,
    APOLLO_KEY_Z = 29,

    APOLLO_KEY_1 = 30,
    APOLLO_KEY_2 = 31,
    APOLLO_KEY_3 = 32,
    APOLLO_KEY_4 = 33,
    APOLLO_KEY_5 = 34,
    APOLLO_KEY_6 = 35,
    APOLLO_KEY_7 = 36,
    APOLLO_KEY_8 = 37,
    APOLLO_KEY_9 = 38,
    APOLLO_KEY_0 = 39,

    APOLLO_KEY_RETURN = 40,
    APOLLO_KEY_ESCAPE = 41,
    APOLLO_KEY_BACKSPACE = 42,
    APOLLO_KEY_TAB = 43,
    APOLLO_KEY_SPACE = 44,

    APOLLO_KEY_MINUS = 45,
    APOLLO_KEY_EQUALS = 46,
    APOLLO_KEY_LEFTBRACKET = 47,
    APOLLO_KEY_RIGHTBRACKET = 48,
    APOLLO_KEY_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right End
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    APOLLO_KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate APOLLO_KEY_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    APOLLO_KEY_SEMICOLON = 51,
    APOLLO_KEY_APOSTROPHE = 52,
    APOLLO_KEY_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    APOLLO_KEY_COMMA = 54,
    APOLLO_KEY_PERIOD = 55,
    APOLLO_KEY_SLASH = 56,

    APOLLO_KEY_CAPSLOCK = 57,

    APOLLO_KEY_F1 = 58,
    APOLLO_KEY_F2 = 59,
    APOLLO_KEY_F3 = 60,
    APOLLO_KEY_F4 = 61,
    APOLLO_KEY_F5 = 62,
    APOLLO_KEY_F6 = 63,
    APOLLO_KEY_F7 = 64,
    APOLLO_KEY_F8 = 65,
    APOLLO_KEY_F9 = 66,
    APOLLO_KEY_F10 = 67,
    APOLLO_KEY_F11 = 68,
    APOLLO_KEY_F12 = 69,

    APOLLO_KEY_PRINTSCREEN = 70,
    APOLLO_KEY_SCROLLLOCK = 71,
    APOLLO_KEY_PAUSE = 72,
    APOLLO_KEY_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    APOLLO_KEY_HOME = 74,
    APOLLO_KEY_PAGEUP = 75,
    APOLLO_KEY_DELETE = 76,
    APOLLO_KEY_END = 77,
    APOLLO_KEY_PAGEDOWN = 78,
    APOLLO_KEY_RIGHT = 79,
    APOLLO_KEY_LEFT = 80,
    APOLLO_KEY_DOWN = 81,
    APOLLO_KEY_UP = 82,

    APOLLO_KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    APOLLO_KEY_KP_DIVIDE = 84,
    APOLLO_KEY_KP_MULTIPLY = 85,
    APOLLO_KEY_KP_MINUS = 86,
    APOLLO_KEY_KP_PLUS = 87,
    APOLLO_KEY_KP_ENTER = 88,
    APOLLO_KEY_KP_1 = 89,
    APOLLO_KEY_KP_2 = 90,
    APOLLO_KEY_KP_3 = 91,
    APOLLO_KEY_KP_4 = 92,
    APOLLO_KEY_KP_5 = 93,
    APOLLO_KEY_KP_6 = 94,
    APOLLO_KEY_KP_7 = 95,
    APOLLO_KEY_KP_8 = 96,
    APOLLO_KEY_KP_9 = 97,
    APOLLO_KEY_KP_0 = 98,
    APOLLO_KEY_KP_PERIOD = 99,

    APOLLO_KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Z.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    APOLLO_KEY_APPLICATION = 101, /**< windows contextual menu, compose */
    APOLLO_KEY_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    APOLLO_KEY_KP_EQUALS = 103,
    APOLLO_KEY_F13 = 104,
    APOLLO_KEY_F14 = 105,
    APOLLO_KEY_F15 = 106,
    APOLLO_KEY_F16 = 107,
    APOLLO_KEY_F17 = 108,
    APOLLO_KEY_F18 = 109,
    APOLLO_KEY_F19 = 110,
    APOLLO_KEY_F20 = 111,
    APOLLO_KEY_F21 = 112,
    APOLLO_KEY_F22 = 113,
    APOLLO_KEY_F23 = 114,
    APOLLO_KEY_F24 = 115,
    APOLLO_KEY_EXECUTE = 116,
    APOLLO_KEY_HELP = 117,    /**< AL Integrated Help Center */
    APOLLO_KEY_MENU = 118,    /**< Menu (show menu) */
    APOLLO_KEY_SELECT = 119,
    APOLLO_KEY_STOP = 120,    /**< AC Stop */
    APOLLO_KEY_AGAIN = 121,   /**< AC Redo/Repeat */
    APOLLO_KEY_UNDO = 122,    /**< AC Undo */
    APOLLO_KEY_CUT = 123,     /**< AC Cut */
    APOLLO_KEY_COPY = 124,    /**< AC Copy */
    APOLLO_KEY_PASTE = 125,   /**< AC Paste */
    APOLLO_KEY_FIND = 126,    /**< AC Find */
    APOLLO_KEY_MUTE = 127,
    APOLLO_KEY_VOLUMEUP = 128,
    APOLLO_KEY_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     APOLLO_KEY_LOCKINGCAPSLOCK = 130,  */
/*     APOLLO_KEY_LOCKINGNUMLOCK = 131, */
/*     APOLLO_KEY_LOCKINGSCROLLLOCK = 132, */
    APOLLO_KEY_KP_COMMA = 133,
    APOLLO_KEY_KP_EQUALSAS400 = 134,

    APOLLO_KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    APOLLO_KEY_INTERNATIONAL2 = 136,
    APOLLO_KEY_INTERNATIONAL3 = 137, /**< Yen */
    APOLLO_KEY_INTERNATIONAL4 = 138,
    APOLLO_KEY_INTERNATIONAL5 = 139,
    APOLLO_KEY_INTERNATIONAL6 = 140,
    APOLLO_KEY_INTERNATIONAL7 = 141,
    APOLLO_KEY_INTERNATIONAL8 = 142,
    APOLLO_KEY_INTERNATIONAL9 = 143,
    APOLLO_KEY_LANG1 = 144, /**< Hangul/English toggle */
    APOLLO_KEY_LANG2 = 145, /**< Hanja conversion */
    APOLLO_KEY_LANG3 = 146, /**< Katakana */
    APOLLO_KEY_LANG4 = 147, /**< Hiragana */
    APOLLO_KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
    APOLLO_KEY_LANG6 = 149, /**< reserved */
    APOLLO_KEY_LANG7 = 150, /**< reserved */
    APOLLO_KEY_LANG8 = 151, /**< reserved */
    APOLLO_KEY_LANG9 = 152, /**< reserved */

    APOLLO_KEY_ALTERASE = 153,    /**< Erase-Eaze */
    APOLLO_KEY_SYSREQ = 154,
    APOLLO_KEY_CANCEL = 155,      /**< AC Cancel */
    APOLLO_KEY_CLEAR = 156,
    APOLLO_KEY_PRIOR = 157,
    APOLLO_KEY_RETURN2 = 158,
    APOLLO_KEY_SEPARATOR = 159,
    APOLLO_KEY_OUT = 160,
    APOLLO_KEY_OPER = 161,
    APOLLO_KEY_CLEARAGAIN = 162,
    APOLLO_KEY_CRSEL = 163,
    APOLLO_KEY_EXSEL = 164,

    APOLLO_KEY_KP_00 = 176,
    APOLLO_KEY_KP_000 = 177,
    APOLLO_KEY_THOUSANDSSEPARATOR = 178,
    APOLLO_KEY_DECIMALSEPARATOR = 179,
    APOLLO_KEY_CURRENCYUNIT = 180,
    APOLLO_KEY_CURRENCYSUBUNIT = 181,
    APOLLO_KEY_KP_LEFTPAREN = 182,
    APOLLO_KEY_KP_RIGHTPAREN = 183,
    APOLLO_KEY_KP_LEFTBRACE = 184,
    APOLLO_KEY_KP_RIGHTBRACE = 185,
    APOLLO_KEY_KP_TAB = 186,
    APOLLO_KEY_KP_BACKSPACE = 187,
    APOLLO_KEY_KP_A = 188,
    APOLLO_KEY_KP_B = 189,
    APOLLO_KEY_KP_C = 190,
    APOLLO_KEY_KP_D = 191,
    APOLLO_KEY_KP_E = 192,
    APOLLO_KEY_KP_F = 193,
    APOLLO_KEY_KP_XOR = 194,
    APOLLO_KEY_KP_POWER = 195,
    APOLLO_KEY_KP_PERCENT = 196,
    APOLLO_KEY_KP_LESS = 197,
    APOLLO_KEY_KP_GREATER = 198,
    APOLLO_KEY_KP_AMPERSAND = 199,
    APOLLO_KEY_KP_DBLAMPERSAND = 200,
    APOLLO_KEY_KP_VERTICALBAR = 201,
    APOLLO_KEY_KP_DBLVERTICALBAR = 202,
    APOLLO_KEY_KP_COLON = 203,
    APOLLO_KEY_KP_HASH = 204,
    APOLLO_KEY_KP_SPACE = 205,
    APOLLO_KEY_KP_AT = 206,
    APOLLO_KEY_KP_EXCLAM = 207,
    APOLLO_KEY_KP_MEMSTORE = 208,
    APOLLO_KEY_KP_MEMRECALL = 209,
    APOLLO_KEY_KP_MEMCLEAR = 210,
    APOLLO_KEY_KP_MEMADD = 211,
    APOLLO_KEY_KP_MEMSUBTRACT = 212,
    APOLLO_KEY_KP_MEMMULTIPLY = 213,
    APOLLO_KEY_KP_MEMDIVIDE = 214,
    APOLLO_KEY_KP_PLUSMINUS = 215,
    APOLLO_KEY_KP_CLEAR = 216,
    APOLLO_KEY_KP_CLEARENTRY = 217,
    APOLLO_KEY_KP_BINARY = 218,
    APOLLO_KEY_KP_OCTAL = 219,
    APOLLO_KEY_KP_DECIMAL = 220,
    APOLLO_KEY_KP_HEXADECIMAL = 221,

    APOLLO_KEY_LCTRL = 224,
    APOLLO_KEY_LSHIFT = 225,
    APOLLO_KEY_LALT = 226, /**< alt, option */
    APOLLO_KEY_LGUI = 227, /**< windows, command (apple), meta */
    APOLLO_KEY_RCTRL = 228,
    APOLLO_KEY_RSHIFT = 229,
    APOLLO_KEY_RALT = 230, /**< alt gr, option */
    APOLLO_KEY_RGUI = 231, /**< windows, command (apple), meta */

    APOLLO_KEY_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special SDL_KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
    /* @{ */

    APOLLO_KEY_SLEEP = 258,                   /**< Sleep */
    APOLLO_KEY_WAKE = 259,                    /**< Wake */

    APOLLO_KEY_CHANNEL_INCREMENT = 260,       /**< Channel Increment */
    APOLLO_KEY_CHANNEL_DECREMENT = 261,       /**< Channel Decrement */

    APOLLO_KEY_MEDIA_PLAY = 262,          /**< Play */
    APOLLO_KEY_MEDIA_PAUSE = 263,         /**< Pause */
    APOLLO_KEY_MEDIA_RECORD = 264,        /**< Record */
    APOLLO_KEY_MEDIA_FAST_FORWARD = 265,  /**< Fast Forward */
    APOLLO_KEY_MEDIA_REWIND = 266,        /**< Rewind */
    APOLLO_KEY_MEDIA_NEXT_TRACK = 267,    /**< Next Track */
    APOLLO_KEY_MEDIA_PREVIOUS_TRACK = 268, /**< Previous Track */
    APOLLO_KEY_MEDIA_STOP = 269,          /**< Stop */
    APOLLO_KEY_MEDIA_EJECT = 270,         /**< Eject */
    APOLLO_KEY_MEDIA_PLAY_PAUSE = 271,    /**< Play / Pause */
    APOLLO_KEY_MEDIA_SELECT = 272,        /* Media Select */

    APOLLO_KEY_AC_NEW = 273,              /**< AC New */
    APOLLO_KEY_AC_OPEN = 274,             /**< AC Open */
    APOLLO_KEY_AC_CLOSE = 275,            /**< AC Close */
    APOLLO_KEY_AC_EXIT = 276,             /**< AC Exit */
    APOLLO_KEY_AC_SAVE = 277,             /**< AC Save */
    APOLLO_KEY_AC_PRINT = 278,            /**< AC Print */
    APOLLO_KEY_AC_PROPERTIES = 279,       /**< AC Properties */

    APOLLO_KEY_AC_SEARCH = 280,           /**< AC Search */
    APOLLO_KEY_AC_HOME = 281,             /**< AC Home */
    APOLLO_KEY_AC_BACK = 282,             /**< AC Back */
    APOLLO_KEY_AC_FORWARD = 283,          /**< AC Forward */
    APOLLO_KEY_AC_STOP = 284,             /**< AC Stop */
    APOLLO_KEY_AC_REFRESH = 285,          /**< AC Refresh */
    APOLLO_KEY_AC_BOOKMARKS = 286,        /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */


    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    APOLLO_KEY_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    APOLLO_KEY_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    APOLLO_KEY_CALL = 289, /**< Used for accepting phone calls. */
    APOLLO_KEY_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    APOLLO_KEY_RESERVED = 400,    /**< 400-500 reserved for dynamic keycodes */

    APOLLO_KEY_COUNT = 512 /**< not a key, just marks the number of scancodes for array bounds */

} APOLLO_KEY;
