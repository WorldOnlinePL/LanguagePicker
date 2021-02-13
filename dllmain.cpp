#include "pch.h"

#include <windows.h>
#include <algorithm>
#include <filesystem>
#include <string>

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

inline std::string trim(const std::string& s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {return std::isspace(c); }).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

void AttachProcess() {
    //Read self-ini file.
    char get_full_path[MAX_PATH];
    char current_lang_selected[3]; 

    GetPathUnderModuleA(HINST_THISCOMPONENT, get_full_path, "LangPicker.ini");
    GetPrivateProfileStringA("GameLauncher", "Language", "EN", current_lang_selected, sizeof(current_lang_selected), get_full_path);

    std::string language_picked(current_lang_selected);

    if (trim(language_picked) == "IT") {
        if (fs::exists("../LANGUAGES/Italian_Global.bin")) {
            fs::remove("../LANGUAGES/English_Global.bin");
            fs::copy_file("../LANGUAGES/Italian_Global.bin", "../LANGUAGES/English_Global.bin");
        } else {
            MessageBoxA(nullptr, "Failed to load Italian LanguagePack", "WorldOnlinePL", MB_OK);
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