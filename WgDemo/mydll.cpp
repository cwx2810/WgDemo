#include <Windows.h>

BOOL WINAPI DLLMain(HINSTANCE hinstModule, DWORD dwReason, LPVOID lpvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstModule);
		MessageBox(NULL, "123", "321", MB_OK);
	}
	return true;
}