#pragma once

//Grizzly Key Codes

#define GRIZZLY_MOUSE_BUTTON_1         0
#define GRIZZLY_MOUSE_BUTTON_2         1
#define GRIZZLY_MOUSE_BUTTON_3         2
#define GRIZZLY_MOUSE_BUTTON_4         3
#define GRIZZLY_MOUSE_BUTTON_5         4
#define GRIZZLY_MOUSE_BUTTON_6         5
#define GRIZZLY_MOUSE_BUTTON_7         6
#define GRIZZLY_MOUSE_BUTTON_8         7

#define GRIZZLY_MOUSE_BUTTON_LEFT      GRIZZLY_MOUSE_BUTTON_1
#define GRIZZLY_MOUSE_BUTTON_RIGHT     GRIZZLY_MOUSE_BUTTON_2
#define GRIZZLY_MOUSE_BUTTON_MIDDLE    GRIZZLY_MOUSE_BUTTON_3

#define GRIZZLY_UNKNOWN_KEY           754233
#define GRIZZLY_KEY_SPACE              0
#define GRIZZLY_KEY_APOSTROPHE         1  /* ' */
#define GRIZZLY_KEY_COMMA              2  /* , */
#define GRIZZLY_KEY_MINUS              3  /* - */
#define GRIZZLY_KEY_PERIOD             4  /* . */
#define GRIZZLY_KEY_SLASH              5  /* / */
#define GRIZZLY_KEY_0                  6
#define GRIZZLY_KEY_1                  7
#define GRIZZLY_KEY_2                  8
#define GRIZZLY_KEY_3                  9
#define GRIZZLY_KEY_4                  10
#define GRIZZLY_KEY_5                  11
#define GRIZZLY_KEY_6                  12
#define GRIZZLY_KEY_7                  13
#define GRIZZLY_KEY_8                  14
#define GRIZZLY_KEY_9                  15
#define GRIZZLY_KEY_SEMICOLON          16  /* ; */
#define GRIZZLY_KEY_EQUAL              17  /* = */
#define GRIZZLY_KEY_A                  18
#define GRIZZLY_KEY_B                  19
#define GRIZZLY_KEY_C                  20
#define GRIZZLY_KEY_D                  21
#define GRIZZLY_KEY_E                  22
#define GRIZZLY_KEY_F                  23
#define GRIZZLY_KEY_G                  24
#define GRIZZLY_KEY_H                  25
#define GRIZZLY_KEY_I                  26
#define GRIZZLY_KEY_J                  27
#define GRIZZLY_KEY_K                  28
#define GRIZZLY_KEY_L                  29
#define GRIZZLY_KEY_M                  30
#define GRIZZLY_KEY_N                  31
#define GRIZZLY_KEY_O                  32
#define GRIZZLY_KEY_P                  33
#define GRIZZLY_KEY_Q                  34
#define GRIZZLY_KEY_R                  35
#define GRIZZLY_KEY_S                  36
#define GRIZZLY_KEY_T                  37
#define GRIZZLY_KEY_U                  38
#define GRIZZLY_KEY_V                  39
#define GRIZZLY_KEY_W                  40
#define GRIZZLY_KEY_X                  41
#define GRIZZLY_KEY_Y                  42
#define GRIZZLY_KEY_Z                  43
#define GRIZZLY_KEY_LEFT_BRACKET       44  /* [ */
#define GRIZZLY_KEY_BACKSLASH          45  /* \ */
#define GRIZZLY_KEY_RIGHT_BRACKET      46  /* ] */
#define GRIZZLY_KEY_GRAVE_ACCENT       47  /* ` */

#define GRIZZLY_KEY_ESCAPE             48
#define GRIZZLY_KEY_ENTER              49
#define GRIZZLY_KEY_TAB                50
#define GRIZZLY_KEY_BACKSPACE          51
#define GRIZZLY_KEY_INSERT             52
#define GRIZZLY_KEY_DELETE             53
#define GRIZZLY_KEY_RIGHT              54
#define GRIZZLY_KEY_LEFT               55
#define GRIZZLY_KEY_DOWN               56
#define GRIZZLY_KEY_UP                 57
#define GRIZZLY_KEY_PAGE_UP            58
#define GRIZZLY_KEY_PAGE_DOWN          59
#define GRIZZLY_KEY_HOME               60
#define GRIZZLY_KEY_END                61
#define GRIZZLY_KEY_CAPS_LOCK          62
#define GRIZZLY_KEY_SCROLL_LOCK        63
#define GRIZZLY_KEY_NUM_LOCK           64
#define GRIZZLY_KEY_PRINT_SCREEN       65
#define GRIZZLY_KEY_PAUSE              66
#define GRIZZLY_KEY_F1                 67
#define GRIZZLY_KEY_F2                 68
#define GRIZZLY_KEY_F3                 69
#define GRIZZLY_KEY_F4                 70
#define GRIZZLY_KEY_F5                 71
#define GRIZZLY_KEY_F6                 72
#define GRIZZLY_KEY_F7                 73
#define GRIZZLY_KEY_F8                 74
#define GRIZZLY_KEY_F9                 75
#define GRIZZLY_KEY_F10                76
#define GRIZZLY_KEY_F11                77
#define GRIZZLY_KEY_F12                78
#define GRIZZLY_KEY_F13                79
#define GRIZZLY_KEY_F14                80
#define GRIZZLY_KEY_F15                81
#define GRIZZLY_KEY_F16                82
#define GRIZZLY_KEY_F17                83
#define GRIZZLY_KEY_F18                84
#define GRIZZLY_KEY_F19                85
#define GRIZZLY_KEY_F20                86
#define GRIZZLY_KEY_F21                87
#define GRIZZLY_KEY_F22                88
#define GRIZZLY_KEY_F23                89
#define GRIZZLY_KEY_F24                90
#define GRIZZLY_KEY_F25                91
#define GRIZZLY_KEY_KP_0               92
#define GRIZZLY_KEY_KP_1               93
#define GRIZZLY_KEY_KP_2               94
#define GRIZZLY_KEY_KP_3               95
#define GRIZZLY_KEY_KP_4               96
#define GRIZZLY_KEY_KP_5               97
#define GRIZZLY_KEY_KP_6               98
#define GRIZZLY_KEY_KP_7               99
#define GRIZZLY_KEY_KP_8               100
#define GRIZZLY_KEY_KP_9               101
#define GRIZZLY_KEY_KP_DECIMAL         102
#define GRIZZLY_KEY_KP_DIVIDE          103
#define GRIZZLY_KEY_KP_MULTIPLY        104
#define GRIZZLY_KEY_KP_SUBTRACT        105
#define GRIZZLY_KEY_KP_ADD             106
#define GRIZZLY_KEY_KP_ENTER           107
#define GRIZZLY_KEY_KP_EQUAL           108
#define GRIZZLY_KEY_LEFT_SHIFT         109
#define GRIZZLY_KEY_LEFT_CONTROL       110
#define GRIZZLY_KEY_LEFT_ALT           111
#define GRIZZLY_KEY_LEFT_SUPER         112
#define GRIZZLY_KEY_RIGHT_SHIFT        113
#define GRIZZLY_KEY_RIGHT_CONTROL      114
#define GRIZZLY_KEY_RIGHT_ALT          115
#define GRIZZLY_KEY_RIGHT_SUPER        116


namespace GrizzlyBear {

	class KeyCodeConv
	{
	public:

		virtual int to_grizzly_key(int keycode) = 0;
		virtual int to_platform_key(int keycode) = 0;

		virtual int to_grizzly_mouse(int button) = 0;
		virtual int to_platform_mouse(int button) = 0;
	};

}

