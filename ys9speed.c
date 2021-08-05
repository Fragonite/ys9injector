#define WIN32_LEAN_AND_MEAN

#include <stdint.h>
#include <windows.h>

void *find_subarray_32(volatile uint32_t *const array, size_t length, const uint32_t *const subarray, size_t subarrayLength)
{
	for(size_t i = 0; i <= length - subarrayLength; i++)
	{
		for(size_t j = 0; array[i + j] == subarray[j]; j++)
		{
			if(j == subarrayLength - 1)
			{
				return (void*) &array[i];
			}
		}
	}
	
	return NULL;
}

DWORD WINAPI ModuleMain(LPVOID lpParameter)
{
	HMODULE hModule;
	if(!(hModule = GetModuleHandleW(NULL)))//GET EXECUTABLE BASE ADDRESS.
	{
		return 1;
	}
	
	const uint32_t SPEED_SIGNATURE[] = {0x3F7D70A4, 0x3F7EB852, 0x3F7FBE77};//UNIQUE BYTE SIGNATURE FOUND WITH A DEBUGGER. NO OFFSET NEEDED.
	float *speed;
	if(!(speed = (float*) find_subarray_32((uint32_t*) ((uint8_t*) hModule + 0x600000), 0x200000, SPEED_SIGNATURE, sizeof(SPEED_SIGNATURE) / sizeof(SPEED_SIGNATURE[0]))))
	{
		return 1;
	}
	
	DWORD protect1, protect2;
	
	if(!(VirtualProtect(speed, sizeof(float), PAGE_EXECUTE_READWRITE, &protect1)))
	{
		return 1;
	}
	
	*speed = 1.09;//SET GAME SPEED. DEFAULT VALUE IS 0.99. LESS STUTTER COMPARED TO HOOKING QUERYPERFORMANCECOUNTER.
		
	if(!(VirtualProtect(speed, sizeof(float), protect1, &protect2)))
	{
		return 1;
	}
	
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			if(!(DisableThreadLibraryCalls(hinstDLL)))
			{
				;
			}
			
			HANDLE hObject;
			if(!(hObject = CreateThread(NULL, 0, ModuleMain, NULL, 0, NULL)))
			{
				return FALSE;
			}
			
			if(!(CloseHandle(hObject)))
			{
				;
			}
			
			break;
		default:
			break;
	}
	
	return TRUE;
}