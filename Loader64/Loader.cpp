#include"../Share/Simple/Simple.h"

#ifndef _WIN64
#define INJECTOR_NAME L"Injector"
#define LOADER_NAME L"Loader.dll"
#else
#define INJECTOR_NAME L"Injector64"
#define LOADER_NAME L"Loader64.dll"
#endif

#define INI_FILE_NAME INJECTOR_NAME".ini"
#define LOADER_MENU L"Loader"

bool GetDir2(std::wstring &wDir, HMODULE hDll) {
	WCHAR wcDir[MAX_PATH] = { 0 };

	if (!GetModuleFileNameW(hDll, wcDir, _countof(wcDir))) {
		return false;
	}

	std::wstring dir = wcDir;
	size_t pos = dir.rfind(L"\\");

	if (pos == std::wstring::npos) {
		return false;
	}

	dir = dir.substr(0, pos + 1);
	wDir = dir;
	return true;
}

HMODULE Loader(HMODULE hDll) {
	Config conf(INI_FILE_NAME, hDll);
	std::wstring wMainDll;

	bool check = true;
	check &= conf.Read(LOADER_MENU, L"MainDLL", wMainDll);

	if (!check) {
		return NULL;
	}

	if (!wMainDll.length()) {
		return NULL;
	}

	std::wstring wDir;
	if (!GetDir2(wDir, hDll)) {
		return NULL;
	}

	return LoadLibraryW((wDir + L"\\" + wMainDll).c_str());
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	static HMODULE hMainDLL = NULL;
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hinstDLL);
		hMainDLL = Loader(hinstDLL);
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		if (hMainDLL) {
			FreeLibrary(hMainDLL);
		}
		break;
	}
	default:
	{
		break;
	}
	}
	return TRUE;
}