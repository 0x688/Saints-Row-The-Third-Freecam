/*
	0x: Why making a new one if theres one P:
	PROJECT:		mod_sa
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright we_sux

	mod_sa is available from http://code.google.com/p/m0d-s0beit-sa/

	mod_sa is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	mod_sa is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "main.h"

static WNDPROC		orig_wndproc;
static HWND			orig_wnd;
struct key_state	key_table[256];
int					keys_cleared;
int					key_being_pressed;
bool				vkey_blocked[0xFE] ;

static void process_key ( int down, int vkey, int repeat, int scancode, int extended, HWND wnd )
{
	if ( down && KEY_DOWN(vkey) )
		return; /* ignore key repeat, bad states */
	if ( !down && KEY_UP(vkey) )
		return; /* ignore bad states */

	if ( GetForegroundWindow() != wnd
		//|| 0x90 == *(uint8_t *)0x541DF5
		)
	{
		if ( !down && KEY_DOWN(vkey) )
			key_table[vkey].count++;
		return;
	}

	key_table[vkey].count++;
}

static LRESULT CALLBACK wnd_proc ( HWND wnd, UINT uMSG, WPARAM wParam, LPARAM lParam )
{
	switch ( uMSG ) {
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		process_key( (uMSG == WM_LBUTTONDOWN), VK_LBUTTON, 0, 0, 0, wnd );
		break;

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		process_key( (uMSG == WM_RBUTTONDOWN), VK_RBUTTON, 0, 0, 0, wnd );
		break;

	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		process_key( (uMSG == WM_MBUTTONDOWN), VK_MBUTTON, 0, 0, 0, wnd );
		break;

	/* :D */
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			unsigned long	p = (unsigned long)lParam;
			int				down = ( uMSG == WM_KEYDOWN || uMSG == WM_SYSKEYDOWN );
			int				vkey = (int)wParam;

			key_being_pressed = vkey;
			unsigned int	repeat = ( p >> 0 ) & 0x7FFF;
			unsigned int	scancode = ( p >> 16 ) & 0x00FF;
			unsigned int	extended = ( p >> 24 ) & 0x0001;
			process_key( down, vkey, repeat, scancode, extended, wnd );
			if ( vkey_blocked[vkey] )	return 0;
		}
		break;	

		case WM_KILLFOCUS:
			keys_cleared = 0;
			keyhook_clear_states();
			break;
	}

//wnd_proc_original:
	return CallWindowProc( orig_wndproc, wnd, uMSG, wParam, lParam );
}

int keyhook_key_down ( int v )
{
	if ( key_table[v].consume )
		return 0;
	else
		return ( key_table[v].count & 1 ) ^ key_table[v].flip;
}
int keyhook_key_up ( int v )
{
	return !KEY_DOWN( v );
}
void keyhook_clear_states ( void )
{
	if ( !keys_cleared )
	{
		keys_cleared = 1;
		for ( int i = 0; i < 256; i++ )
		{
			key_table[i].pstate = 0;
			key_table[i].count = 0;
			key_table[i].flip = 0;
		}
	}

}
void keyhook_run ( void )
{
	keys_cleared = 0;
	for ( int i = 0; i < 256; i++ )
	{
		key_table[i].consume = 0;

		if ( i == VK_PRIOR || i == VK_NEXT || i == VK_TAB )
			key_table[i].pstate = ( key_table[i].count & 1 );
		else
			key_table[i].pstate = KEY_DOWN( i );

		if ( key_table[i].count > 0 )
		{
			key_table[i].flip ^= 1;
			key_table[i].count--;
		}
	}
}

int keyhook_key_pressed ( int v )
{
	if ( key_table[v].consume )
		return 0;
	else if ( v == VK_PRIOR || v == VK_NEXT || v == VK_TAB )
		return KEY_DOWN( v ) && !( key_table[v].pstate ^ key_table[v].flip );
	else
		return KEY_DOWN( v ) && !key_table[v].pstate;
}


void wndMainHook ( HWND wnd )
{
	if ( orig_wndproc == NULL || wnd != orig_wnd )
	{
		orig_wndproc = ( WNDPROC ) ( UINT_PTR ) SetWindowLong( wnd, GWL_WNDPROC, (LONG) (UINT_PTR) wnd_proc );
		orig_wnd = wnd;
	}

	for ( int clear=0; clear != 0xFE; clear++) {
		vkey_blocked[clear] = false;
	}
}