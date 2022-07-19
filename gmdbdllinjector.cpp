// gmdbdllinjector.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>

HANDLE hProcess;



bool Write(uint32_t vaddress, const void* bytes, size_t size)
{
    return WriteProcessMemory(hProcess, reinterpret_cast<void*>(vaddress), bytes, size, NULL);
}

uint32_t Allocate(size_t size, uint32_t vaddress = 0)
{
    return reinterpret_cast<uint32_t>(VirtualAllocEx(::hProcess, reinterpret_cast<void*>(vaddress), size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
}

bool NewThread(uint32_t vaddress, void* param)
{
    return CreateRemoteThread(hProcess, 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(vaddress), param, 0, 0);
}

bool Inject(const char* dll_path)
{
    uint32_t addr = Allocate(strlen(dll_path) + 1);
    if (addr && Write(addr, dll_path, strlen(dll_path)))
        return NewThread(reinterpret_cast<uint32_t>(LoadLibraryA), reinterpret_cast<void*>(addr));
    return false;
}


int main()
{

	SetConsoleTitle(L"GMDBespredelnik DLL Loader");
	
	std::string dll;
    std::cout << "Enter DLL path: ";
    std::cin >> dll;
	HWND hwnd = FindWindowA(NULL, "Geometry Dash");
	DWORD pid;

	GetWindowThreadProcessId(hwnd, &pid);
	
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

    Inject(dll.c_str());
}
