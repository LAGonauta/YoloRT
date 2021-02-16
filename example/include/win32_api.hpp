#pragma once

#include <string>
#include <array>
#include <optional>

std::optional<std::wstring> GetCurrentModulePath();
std::optional<std::wstring> GetShortcutPath();
bool SetAppModelID(const std::wstring& aumid);
bool SupportsModernNotifications();