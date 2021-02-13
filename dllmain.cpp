#include "pch.h"

#include <windows.h>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

#define GetRegistryDword GetRegistryDwordA
#define GetNFSWRegOptionDwordA(OptionName, Default) GetRegistryDwordA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Electronic Arts\\Need For Speed World\\", OptionName, Default)

DWORD GetRegistryDwordA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwDefault)
{
	DWORD dwResult = dwDefault;
	DWORD dwSize = sizeof(DWORD);
	DWORD regdword = REG_DWORD;

	LSTATUS lResult = 0;
	HKEY hTargetKey;
	if ((lResult = RegOpenKeyExA(hKey, lpSubKey, 0, KEY_READ, &hTargetKey)) == ERROR_SUCCESS) {
		RegQueryValueExA(hTargetKey, lpValueName, NULL, &regdword, (LPBYTE)&dwResult, &dwSize);
		RegCloseKey(hTargetKey);
	}

	return dwResult;
}


void AttachProcess() {
    //TODO - Find Settings.ini used by launcher
    char LauncherPos = GetNFSWRegOptionDwordA("LaunchInstallDir", 8123);

    const char* current_lang_selected = "IT"; //Hardcoded
    const char* default_languages[11] = { "EN", "DE", "ES", "FR", "PL", "RU", "PT", "TC", "SC", "TH", "TR" };
    if (std::find(std::begin(default_languages), std::end(default_languages), current_lang_selected) == std::end(default_languages)) {

        //TODO: Better way to find which file is which. Atm... Hardcoded
        if (current_lang_selected == "IT") {
            if (fs::exists("../LANGUAGES/Italian_Global.bin")) {
                fs::remove("../LANGUAGES/English_Global.bin");
                fs::copy_file("../LANGUAGES/Italian_Global.bin", "../LANGUAGES/English_Global.bin");
            }
        }
    }

    return;
}

void DetachProcess() {
    return;
}

BOOL APIENTRY DllMain(HMODULE, DWORD reasons, LPVOID) {
    switch (reasons) {
    case DLL_PROCESS_ATTACH:
        AttachProcess();
        break;
    case DLL_PROCESS_DETACH:
        DetachProcess();
        break;
    }

    return TRUE;
}