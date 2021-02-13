#include "pch.h"

#include <windows.h>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

LPSTR GetPathUnderModuleA(HMODULE hModule, LPSTR OutBuf, LPCSTR FileName) {
    GetModuleFileNameA(hModule, OutBuf, MAX_PATH);
    CHAR* dirend = strrchr(OutBuf, '\\') + 1;
    strcpy_s(dirend, MAX_PATH, FileName);
    return OutBuf;
}

void AttachProcess() {
    //Read self-ini file.
    char get_full_path[MAX_PATH];
    char current_lang_selected[MAX_PATH];

    GetPathUnderModuleA(HINST_THISCOMPONENT, get_full_path, "LangPicker.ini");
    GetPrivateProfileStringA("GameLauncher", "Language", "EN", current_lang_selected, 256, get_full_path);

    const char* default_languages[11] = { "EN", "DE", "ES", "FR", "PL", "RU", "PT", "TC", "SC", "TH", "TR" };
    if (std::find(std::begin(default_languages), std::end(default_languages), current_lang_selected) == std::end(default_languages)) {
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