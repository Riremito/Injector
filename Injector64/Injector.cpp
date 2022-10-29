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

bool Launcher(std::wstring wDroppedTarget) {
	Config conf(INI_FILE_NAME);
	std::wstring wTarget, wLoaderDLL, wMainDLL;
	bool check = true;

	if (wDroppedTarget.length()) {
		conf.Update(INJECTOR_NAME, L"TargetEXE", wDroppedTarget);

		std::wstring wDeafultLoaderDLL = LOADER_NAME;
		conf.Update(INJECTOR_NAME, L"LoaderDLL", wDeafultLoaderDLL);
	}

	check &= conf.Read(INJECTOR_NAME, L"TargetEXE", wTarget);
	check &= conf.Read(INJECTOR_NAME, L"LoaderDLL", wLoaderDLL);
	conf.Read(LOADER_MENU, L"MainDLL", wMainDLL); // write config

	if (!check) {
		return false;
	}

	std::wstring dir;
	if (!GetDir(dir)) {
		return false;
	}

	Injector injector(wTarget, dir + wLoaderDLL);
	return injector.Run();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	if (__argc < 2) {
		// create default ini file
		if (!Launcher(L"")) {
			MessageBoxW(NULL, L"Please check target exe file path", INJECTOR_NAME, MB_OK);
		}
	}
	else {
		// read config from ini file
		if (!Launcher(__wargv[1])) {
			MessageBoxW(NULL, L"Please drop target exe file", INJECTOR_NAME, MB_OK);
		}
	}

	return 0;
}