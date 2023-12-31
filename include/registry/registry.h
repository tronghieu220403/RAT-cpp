#ifndef RAT_REGISTRY_REGISTRY_H_
#define RAT_REGISTRY_REGISTRY_H_

#define UNICODE
#define _UNICODE

#ifdef _WIN32

#include <WS2tcpip.h>
#include <strsafe.h>

#include <map>
#include <string>

namespace rat
{
	class Registry 
    {
	private:
		HKEY root_ = nullptr;
        std::string sub_key_name_;
        static std::map < std::string, HKEY, std::less<> > hkey_map_;
	public:
		
		Registry();
		explicit Registry(HKEY root_key_value, const std::string_view& sub_key_name);
		explicit Registry(long long root_key_ll, const std::string_view& sub_key_name);
		explicit Registry(const std::string_view& root_hkey_name, const std::string_view& sub_key_name);
		
		void SetRootKey(long long root_key_ll);
		void SetRootKey(const std::string_view& root_hkey_name);
        void SetRootKey(HKEY root_hkey_value);

        void SetSubKey(const std::string_view& sub_key_name);

		bool DeleteSelf();

	};
}
#endif

#endif // !registry_h
