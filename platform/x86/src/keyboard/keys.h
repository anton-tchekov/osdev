#ifndef __KEYS_H__
#define __KEYS_H__

typedef enum
{
	KEY_UNKNOWN          =   0,
	KEY_A                =   4,
	KEY_B                =   5,
	KEY_C                =   6,
	KEY_D                =   7,
	KEY_E                =   8,
	KEY_F                =   9,
	KEY_G                =  10,
	KEY_H                =  11,
	KEY_I                =  12,
	KEY_J                =  13,
	KEY_K                =  14,
	KEY_L                =  15,
	KEY_M                =  16,
	KEY_N                =  17,
	KEY_O                =  18,
	KEY_P                =  19,
	KEY_Q                =  20,
	KEY_R                =  21,
	KEY_S                =  22,
	KEY_T                =  23,
	KEY_U                =  24,
	KEY_V                =  25,
	KEY_W                =  26,
	KEY_X                =  27,
	KEY_Y                =  28,
	KEY_Z                =  29,
	KEY_1                =  30,
	KEY_2                =  31,
	KEY_3                =  32,
	KEY_4                =  33,
	KEY_5                =  34,
	KEY_6                =  35,
	KEY_7                =  36,
	KEY_8                =  37,
	KEY_9                =  38,
	KEY_0                =  39,
	KEY_RETURN           =  40,
	KEY_ESCAPE           =  41,
	KEY_BACKSPACE        =  42,
	KEY_TAB              =  43,
	KEY_SPACE            =  44,
	KEY_MINUS            =  45,
	KEY_EQUALS           =  46,
	KEY_L_BRACKET        =  47,
	KEY_R_BRACKET        =  48,
	KEY_BACKSLASH        =  49,
	KEY_SEMICOLON        =  51,
	KEY_APOSTROPHE       =  52,
	KEY_GRAVE            =  53,
	KEY_COMMA            =  54,
	KEY_PERIOD           =  55,
	KEY_SLASH            =  56,
	KEY_CAPSLOCK         =  57,
	KEY_F1               =  58,
	KEY_F2               =  59,
	KEY_F3               =  60,
	KEY_F4               =  61,
	KEY_F5               =  62,
	KEY_F6               =  63,
	KEY_F7               =  64,
	KEY_F8               =  65,
	KEY_F9               =  66,
	KEY_F10              =  67,
	KEY_F11              =  68,
	KEY_F12              =  69,
	KEY_PRINT_SCREEN     =  70,
	KEY_INSERT           =  73,
	KEY_HOME             =  74,
	KEY_PAGE_UP          =  75,
	KEY_DELETE           =  76,
	KEY_END              =  77,
	KEY_PAGE_DOWN        =  78,
	KEY_RIGHT            =  79,
	KEY_LEFT             =  80,
	KEY_DOWN             =  81,
	KEY_UP               =  82,
	KEY_KP_DIVIDE        =  84,
	KEY_KP_MULTIPLY      =  85,
	KEY_KP_MINUS         =  86,
	KEY_KP_PLUS          =  87,
	KEY_KP_ENTER         =  88,
	KEY_KP_1             =  89,
	KEY_KP_2             =  90,
	KEY_KP_3             =  91,
	KEY_KP_4             =  92,
	KEY_KP_5             =  93,
	KEY_KP_6             =  94,
	KEY_KP_7             =  95,
	KEY_KP_8             =  96,
	KEY_KP_9             =  97,
	KEY_KP_0             =  98,
	KEY_KP_PERIOD        =  99,
	KEY_NON_US_BACKSLASH = 100,
	KEY_KP_EQUALS        = 103,
	KEY_L_CTRL           = 224,
	KEY_L_SHIFT          = 225,
	KEY_L_ALT            = 226,
	KEY_L_GUI            = 227,
	KEY_R_CTRL           = 228,
	KEY_R_SHIFT          = 229,
	KEY_R_ALT            = 230,
	KEY_R_GUI            = 231,
	KEY_COUNT
} Key;

/* Keyboard Modifiers */
#define MOD_SHIFT          0x8000
#define MOD_CTRL           0x4000
#define MOD_OS             0x2000
#define MOD_ALT            0x1000
#define MOD_ALT_GR         0x800

#endif /* __KEYS_H__ */