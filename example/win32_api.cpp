#include <windows.h>
#include <shlobj.h>

#include "win32_api.hpp"

std::optional<std::wstring> GetCurrentModulePath()
{
    std::array<wchar_t, MAX_PATH> exePath;
    auto charWritten = GetModuleFileName(nullptr, exePath.data(), exePath.size());
    if (charWritten > 0)
    {
        return std::wstring(exePath.data());
    }
    return std::nullopt;
}

std::optional<std::wstring> GetShortcutPath()
{
    std::array<wchar_t, MAX_PATH> shortcutPath;
    auto charWritten = GetEnvironmentVariable(L"APPDATA", shortcutPath.data(), shortcutPath.size());
    if (charWritten > 0)
    {
        return std::wstring(shortcutPath.data());
    }
    return std::nullopt;
}

bool SetAppModelID(const std::wstring& aumid)
{
    return SUCCEEDED(SetCurrentProcessExplicitAppUserModelID(aumid.c_str()));
}

extern "C"
{
    // For some reason I had to declare it here
    NTSTATUS NTAPI RtlGetVersion(PRTL_OSVERSIONINFOW);
}

bool SupportsModernNotifications()
{
    RTL_OSVERSIONINFOW rovi = { 0 };
    rovi.dwOSVersionInfoSize = sizeof(rovi);
    if (S_OK == RtlGetVersion(&rovi))
    {
        return rovi.dwMajorVersion > 6;
    }
    return false;
}