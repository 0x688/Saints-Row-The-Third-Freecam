#ifndef __WNDPROCKEYHOOK_H__
#define __WNDPROCKEYHOOK_H__

struct key_state
{
	uint8_t count : 5;
	uint8_t flip : 1;
	uint8_t pstate : 1;		/* previous state (1 down, 0 up) */
	uint8_t consume : 1;	/* KEY_CONSUME(vkey) */
};
#define KEY_DOWN		keyhook_key_down
#define KEY_UP			keyhook_key_up
#define KEY_PRESSED		keyhook_key_pressed


int						keyhook_key_down ( int v );
void					keyhook_clear_states ( void );
int						keyhook_key_up ( int v );
int						keyhook_key_pressed ( int v );

#define BLOCK_KEY(vkey)		vkey_blocked[vkey]=TRUE;
#define UNBLOCK_KEY(vkey)	vkey_blocked[vkey]=FALSE;
extern bool				vkey_blocked[0xFE] ;
extern struct key_state key_table[256];
extern int				key_being_pressed;

void keyhook_run ( void );
void wndMainHook ( HWND wnd );

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52 
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A
#define VK_NUMPAD0    0x60
#define VK_NUMPAD1    0x61
#define VK_NUMPAD2    0x62
#define VK_NUMPAD3    0x63
#define VK_NUMPAD4    0x64
#define VK_NUMPAD5    0x65
#define VK_NUMPAD6    0x66
#define VK_NUMPAD7    0x67
#define VK_NUMPAD8    0x68
#define VK_NUMPAD9    0x69
#define SC_MINIMIZE 0xF020
#define SC_MAXIMIZE 0xF030
#define SC_CLOSE 0xF060

#endif