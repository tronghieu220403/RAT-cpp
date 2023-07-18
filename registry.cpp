#pragma once

#include "registry.h"

#ifdef _WIN32

#include <strsafe.h>

namespace rat{

std::map<std::string, HKEY, std::less<> > Registry::hkey_map_ =
{
    {"HKEY_CURRENT_USER", HKEY_CURRENT_USER},
    {"HKEY_USERS", HKEY_USERS},
    {"HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE},
    {"HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT},
    {"HKEY_CURRENT_CONFIG", HKEY_CURRENT_CONFIG},
    {"HKEY_CURRENT_USER_LOCAL_SETTINGS", HKEY_CURRENT_USER_LOCAL_SETTINGS},
    {"HKEY_DYN_DATA", HKEY_DYN_DATA},
    {"HKEY_PERFORMANCE_DATA", HKEY_PERFORMANCE_DATA},
    {"HKEY_PERFORMANCE_NLSTEXT", HKEY_PERFORMANCE_NLSTEXT},
    {"HKEY_PERFORMANCE_TEXT", HKEY_PERFORMANCE_TEXT}
};

Registry::Registry() = default;;

Registry::Registry(long long root_hkey_ll, const std::string_view& sub_key_name): root_(*((HKEY*)root_hkey_ll)), sub_key_name_(sub_key_name){};

Registry::Registry(HKEY root_hkey_value, const std::string_view& sub_key_name): root_(root_hkey_value), sub_key_name_(sub_key_name){};

Registry::Registry(const std::string_view& root_hkey_name, const std::string_view& sub_key_name)
{
	auto it = hkey_map_.find(root_hkey_name);
	if (it == hkey_map_.end()) {
		return;
	}
	root_ = it->second;
    sub_key_name_ = sub_key_name;
};

void Registry::SetRootKey(long long root_hkey_ll)
{
    root_ = *((HKEY*)root_hkey_ll);
};

void Registry::SetRootKey(HKEY root_hkey_value)
{
    root_ = root_hkey_value;
};

void Registry::SetRootKey(const std::string_view& root_hkey_name)
{
	auto it = hkey_map_.find(root_hkey_name);
	if (it == hkey_map_.end()) {
		root_ = nullptr;
	}
    else
    {
        root_ = it->second;
    }
};

void Registry::SetSubKey(const std::string_view& sub_hkey_name)
{
    sub_key_name_ = sub_hkey_name;
};


bool Registry::DeleteSelf()
{
    TCHAR szDelKey[MAX_PATH*2];

    LPTSTR sub_key_name_lptstr = nullptr;
    if (int len = MultiByteToWideChar(CP_UTF8, 0, sub_key_name_.c_str(), -1, nullptr, 0); len > 0)
    {
        sub_key_name_lptstr = new TCHAR[len];
        MultiByteToWideChar(CP_UTF8, 0, sub_key_name_.c_str(), -1, sub_key_name_lptstr, len);
    }
    else {
        return false;
    }
    StringCchCopyW(szDelKey, MAX_PATH * 2, sub_key_name_lptstr);
    delete[] sub_key_name_lptstr;

    if (RegDeleteKey(root_, szDelKey) == ERROR_SUCCESS) 
    {
        return true;
    }

    return false;
}

}

#endif // _WIN32