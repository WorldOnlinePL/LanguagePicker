#include "pch.h"

#include <windows.h>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void AttachProcess() {
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