#include "main.h"




uint32_t getSR3Address();

int strlenA(LPCTSTR str)
{
	int len = 0;
	while(str[len] != 0)
	len++;
	return len;
}
bool cmp_strA(LPCTSTR str, LPCTSTR cmp)
{
	int strlen = strlenA(str);
	int cmplen = strlenA(cmp);
	if(strlen != cmplen)
	return false;
	for(int i = 0; i < strlen; i++)
	if(str[i] != cmp[i])
	return false;
	return true;
}
void* tGetProcAddress(wchar_t * module, char* function)
{
	void* modBase = (void*)getSR3Address();
	IMAGE_NT_HEADERS* inh = (IMAGE_NT_HEADERS*)((DWORD64)modBase + ((IMAGE_DOS_HEADER*)modBase)->e_lfanew);
	IMAGE_DATA_DIRECTORY& idd = inh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if (idd.VirtualAddress == 0)
	return 0;
	IMAGE_EXPORT_DIRECTORY* ied = (IMAGE_EXPORT_DIRECTORY*)((DWORD64)modBase + idd.VirtualAddress);
	DWORD* rvaTable = (DWORD*)((DWORD64)modBase + ied->AddressOfFunctions);
	DWORD* nameTable = (DWORD*)((DWORD64)modBase + ied->AddressOfNames);
	WORD* ordTable = (WORD*)((DWORD64)modBase + ied->AddressOfNameOrdinals);
	for (DWORD i = 0; i < ied->NumberOfFunctions; i++)
	{
		if (cmp_strA((char*)((DWORD64)modBase + nameTable[i]), function))
		return (void*)((DWORD64)modBase + rvaTable[ordTable[i]]);
	}
	return 0;
}