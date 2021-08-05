#define WIN32_LEAN_AND_MEAN

#include <wchar.h>
#include <windows.h>

#pragma comment(lib, "user32.lib")

int main(void)
{
    wprintf(L"Listening for Ys IX: Monstrum Nox.  ");
    
    HWND hWnd;
    for(int i = 1; !(hWnd = FindWindowW(L"Ys IX: Monstrum Nox", NULL)); i++)
    {
        Sleep(999);
        
        switch(i % 3)
        {
            case 0:
                wprintf(L"\b\b  ");
                break;
            case 1:
                wprintf(L"\b\b. ");
                break;
            case 2:
                wprintf(L"\b\b..");
                break;
        }
    }
    wprintf(L"\n");
    Sleep(999);
    
    DWORD dwProcessId;
    GetWindowThreadProcessId(hWnd, &dwProcessId);
    
    HANDLE hProcess;
    if(!(hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, dwProcessId)))
    {
        return 1;
    }
    
    LPVOID lpBaseAddress;
    wchar_t dllPath[1024];
    if(!(lpBaseAddress = VirtualAllocEx(hProcess, NULL, sizeof(dllPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)))
    {
        return 1;
    }
    
    if(!(GetCurrentDirectoryW(sizeof(dllPath), dllPath)))
    {
        return 1;
    }
    
    if(wcsncat_s(dllPath, sizeof(dllPath), L"\\ys9speed.dll", sizeof(L"\\ys9speed.dll")))
    {
        return 1;
    }
    
    if(!(WriteProcessMemory(hProcess, lpBaseAddress, dllPath, sizeof(dllPath), NULL)))
    {
        return 1;
    }
    
    HMODULE hModule;
    if(!(hModule = GetModuleHandleW(L"kernel32.dll")))
    {
        return 1;
    }
    
    LPTHREAD_START_ROUTINE lpStartAddress;
    if(!(lpStartAddress = (LPTHREAD_START_ROUTINE) GetProcAddress(hModule, "LoadLibraryW")))
    {
        return 1;
    }
    
    HANDLE hObject;
    if(!(hObject = CreateRemoteThread(hProcess, NULL, 0, lpStartAddress, lpBaseAddress, 0, NULL)))
    {
        return 1;
    }
    
    wprintf(L"Injection successful.");
    
    if(!(CloseHandle(hObject)))
    {
        ;
    }
    
    Sleep(999);
    
    return 0;
}