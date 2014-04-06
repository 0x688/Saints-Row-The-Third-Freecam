/*
	Saints Row The Third (SKIDROW VERSION) - Test Project by 0x688

	- It includes a freecam and some other things I did, unfortunately I lost the interest in doing more.
	- uwh, also there are unfinished structs somewhere around here I have a full structure but dunno where.
	- For making it more easy I used the keyhook on m0d_s0beit and detours.

*/

#include "main.h"
#include <stdio.h>
#include "detours.h"
class Vector3 {
	public:
		float x, y, z;
};

bool init = false;
void log(const char *Text, ...);
struct CCamera
{
	__int32 cUnk; //0x0000 
	float cUnk1; //0x0004 
	void* cUnk2; //0x0008 
	char _0x000C[208];
	float cFov; //0x00DC 

};//Size=0x00E4

struct CPlayer
{
	char _0x0000[7368];
	float cHealth; //0x1CC8 
	char _0x1CCC[48];
	Vector3 cVelocity; //0x1CFC 

};//Size=0x1D08

CCamera *cCamera = NULL;
CPlayer *cPlayer = NULL;
DWORD	g_GameAddr = NULL;

uint32_t getSR3Address ()
{
	uint32_t	sr3_exec;
	void	*temp = GetModuleHandle( "saintsrowthethird_dx11.exe" );
	__asm mov sr3_exec, eax
	return sr3_exec;
}

bool freecam_enabled = FALSE;
float freezepos = 0.0;

typedef int ( __cdecl *templatePreLoadSomeShit )( char*, BYTE );
int PreloadSomeShit ( char *name ) {
	DWORD funcExec = (getSR3Address() + 0x7C38B0);

	templatePreLoadSomeShit pAddr = (templatePreLoadSomeShit)(funcExec);
	return pAddr ( name, 1 );
}


void InitPatches ( ) {
	DWORD sr3Addr = getSR3Address();
	uint32_t	tmpAddr;
	void *tmpFuncAddr = tGetProcAddress(L"saintsrowthethird_dx11.exe", "AK::Monitor::PostString");
	__asm mov tmpAddr, eax
	BYTE toWrite[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	memcpy( (void*)(DWORD)(tmpAddr + 0x638AA), toWrite, sizeof ( toWrite ) ); /* Disable Notierty Wipe on entering property*/
}

bool onKeyDown() {
	bool process = true;
	
	if ( KEY_PRESSED(VK_F4) ) {
		//int retn = ChangeCharTest("npc_bahamut");
		//log("ChangeCharTest RETN: %d\n", retn);
	}
	
#ifdef WIN32
	
	if(KEY_PRESSED(VK_NUMPAD8)) {
		freecam_enabled ^= 1;
		if ( freecam_enabled ) {
			uint32_t	sr3_exec;

			void	*temp = tGetProcAddress(L"saintsrowthethird_dx11.exe", "GDS_value_decode_values");
			__asm mov sr3_exec, eax
//			char buf[25];
			BYTE cPatch[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
			memcpy((void*)(DWORD)(sr3_exec+0xA8F3E), cPatch, sizeof(cPatch));

			BLOCK_KEY(VK_D);
			BLOCK_KEY(VK_A);
			BLOCK_KEY(VK_W);
			BLOCK_KEY(VK_S);

		} 
		else {
			uint32_t	sr3_exec;
			void	*temp = tGetProcAddress(L"saintsrowthethird_dx11.exe", "GDS_value_decode_values");
			__asm mov sr3_exec, eax
			BYTE cPatch[] = {0x0F, 0x29, 0x05, 0xB0, 0x5B, 0x7A, 0x04};
			memcpy((void*)(DWORD)(sr3_exec+0xA8F3E), cPatch, sizeof(cPatch));


			UNBLOCK_KEY(VK_D);
			UNBLOCK_KEY(VK_A);
			UNBLOCK_KEY(VK_W);
			UNBLOCK_KEY(VK_S);


		}
	}
	
	/* if ( KEY_DOWN(VK_X) ) {
		if ( cPlayer ) {
			if ( cPlayer->cVelocity.y < 0.1 ) cPlayer->cVelocity.y = 1;
			cPlayer->cVelocity.y += 3.5;
			log("Adding .y Velocity (%f)!\n", cPlayer->cVelocity.y);
		}
	} */
	if(KEY_PRESSED(VK_NUMPAD5)) {
		/*if ( freezepos != 0.0 ) 
			freezepos = 0.0;
		else
			freezepos = *(float*)(g_GameAddr+0x47961E4); */
		*(float*)(g_GameAddr+0x47961E8) = *(float*)(g_GameAddr+0x43A5BB8);
		*(float*)(g_GameAddr+0x47961E0) = *(float*)(g_GameAddr+0x43A5BB0);
		*(float*)(g_GameAddr+0x47961E4) = *(float*)(g_GameAddr+0x43A5BB4);
	}		
	if ( freecam_enabled ) {
		/*
			Actor Camera:
				X= saintsrowthethird_dx11.exe+43A5BB0
				Y= saintsrowthethird_dx11.exe+43A5BB4
				Z= saintsrowthethird_dx11.exe+43A5BB8
				
			Actor Position:
				X= saintsrowthethird_dx11.exe+47961E0
				Y= saintsrowthethird_dx11.exe+47961E4
				Z= saintsrowthethird_dx11.exe+47961E8

			Camera Rot:
				X: saintsrowthethird_dx11.exe+43A5DB4
				Y: saintsrowthethird_dx11.exe+43A5DD4
		*/
		if(KEY_DOWN(VK_UP))
		{
			*(float*)(g_GameAddr+0x43A5BB4) = *(float*)(g_GameAddr+0x43A5BB4) + 1.5f;
		}
		if(KEY_DOWN(VK_DOWN))
		{
			*(float*)(g_GameAddr+0x43A5BB4) = *(float*)(g_GameAddr+0x43A5BB4) - 1.5f;
		}

		if(KEY_DOWN(VK_W)) // D
		{
			*(float*)(g_GameAddr+0x43A5BB8) = *(float*)(g_GameAddr+0x43A5BB8) + 1.5f;
			process = 0;
		}
		if(KEY_DOWN(VK_S)) // A
		{
			*(float*)(g_GameAddr+0x43A5BB8) = *(float*)(g_GameAddr+0x43A5BB8) - 1.5f;
		}

		if(KEY_DOWN(VK_A)) // W
		{
			*(float*)(g_GameAddr+0x43A5BB0) = *(float*)(g_GameAddr+0x43A5BB0) + 1.5f;
		}
		if(KEY_DOWN(VK_D)) // S
		{
			*(float*)(g_GameAddr+0x43A5BB0) = *(float*)(g_GameAddr+0x43A5BB0) - 1.5f;
		}
			
		if(KEY_DOWN(VK_ADD)) {
			float fov = *(float*)(g_GameAddr+0xEBE120);
			*(float*)(g_GameAddr+0xEBE120)=fov+10.0f;
		}
		else if(KEY_DOWN(VK_SUBTRACT)) {
			float fov = *(float*)(g_GameAddr+0xEBE120);
			*(float*)(g_GameAddr+0xEBE120)=fov-10.0f;
		}
	}	
#endif
	return process;
}

void onRender() {
	if ( g_GameAddr == NULL ) {
		if ( getSR3Address() != 0x0) 
			g_GameAddr = getSR3Address();
	} 
	else {
		
		if ( cPlayer == NULL ) {
			DWORD pPointer = *(DWORD*)(g_GameAddr + 0x486D358);
			cPlayer = (struct CPlayer*)pPointer;
			if (cPlayer)	log("CPlayer: 0x%x | Pointer: 0x%x\n", cPlayer, pPointer);
		}
		else if ( cCamera == NULL ) {
			cCamera = (struct CCamera*)*(DWORD*)(g_GameAddr + 0x16C090);
			if (cCamera)	log("CCamera: %x | Pointer: %x\n", cCamera, (g_GameAddr + 0x16C090));
		}

		if ( freezepos != 0.0 ) {
			*(float*)(g_GameAddr+0x47961E4) = freezepos;
		}

		onKeyDown();

		keyhook_run();
	}
}

#ifdef _DEVELOPMENT_MODE
void AllocConsoleC(const char *szTitle) {
	AllocConsole();
	SetConsoleTitle(szTitle);
	int hOSFHandle;
	long hSTDHandle;
	FILE *fp;
	hSTDHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hOSFHandle = _open_osfhandle(hSTDHandle, _O_TEXT);
	fp = _fdopen(hOSFHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}

#endif

void log(const char *szMsg, ...) {
	char buf[1024];
	va_list arglist;
	va_start(arglist, szMsg);
	vsnprintf_s(buf, 1024, _TRUNCATE, szMsg, arglist);
	va_end(arglist);
	printf("%s", buf);
}

//p5 text, p6 + param list ?
typedef signed int sint;

typedef sint (*tMainLogFunc)( DWORD* p1, DWORD* p2, DWORD *p3, int p4, int p5, int p6 );
tMainLogFunc mainLogFunc;

sint NewLogFunc( DWORD* p1, DWORD* p2, DWORD *p3, int p4, int p5, int p6 ) {
	_asm pushad
	if ( p4 && p5 & p6 ) {
		log("-----------------------------------------\n");
		log((char*)p6, (char*)p4, (char*)p5);
	}
	_asm popad

	return mainLogFunc(p1, p2, p3, p4, p5, p6);
}

void main() {
#ifdef _DEVELOPMENT_MODE
	AllocConsoleC("");
#endif//
	
	// Old Log Function, could be interesting to look on.
	/*log("Detouring func %x!\n", (PBYTE)(getSR3Address()+0xA88B6A));
	mainLogFunc = (tMainLogFunc)DetourFunction((PBYTE)(getSR3Address()+0xA88B6A), (PBYTE)NewLogFunc);*/


	while ( true ) { /* Silly Method, had it on DXGI Hook Before P:*/
		onRender();
		Sleep ( 5 ) ;
	}
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	DisableThreadLibraryCalls( hinstDLL );
    switch(fdwReason) {
    case DLL_PROCESS_ATTACH: {
		wndMainHook( FindWindow(NULL, "Saints Row: The Third") );
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
        return true;
	}
    case DLL_PROCESS_DETACH:
        return true;
    }
    return false;
}  
