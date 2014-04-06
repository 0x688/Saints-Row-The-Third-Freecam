#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define _DEVELOPMENT_MODE
//#define _INCLUDE_DX11_HOOK

#pragma region Inlcudes
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <Tlhelp32.h>
#include <vector>
#include <string>
#include <time.h>
#include <algorithm>
#include "detours.h"
#pragma endregion
#include "mem.h"
#include "wndproc_hook.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if((p)) { (p)->Release(); (p) = NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(a) { if((a)) { delete (a); (a) = NULL; } }
#endif

void onRender();
