#include "MemFunctions.h"

namespace memorylib
{
    bool MemFunctions::maskCompare(BYTE* dataPtr, ByteArray pattern, ByteMask mask)
    {
        for (size_t i = 0; i < pattern.size(); i++) {
            if (mask.at(i) && dataPtr[i] != pattern[i])
                return false;
        }
        return true;
    }

    ByteArray MemFunctions::decodeHex(std::string str)
    {
        ByteArray bytes;

        for (size_t i = 0; i < str.length(); i += 2) {
            std::string byteString = str.substr(i, 2);
            BYTE byte = (BYTE)strtol(byteString.c_str(), NULL, 16);
            bytes.push_back(byte);
        }

        return bytes;
    }

    ByteMask MemFunctions::decodeMask(std::string str)
    {
        ByteMask mask;

        for (size_t i = 0; i < str.length(); i += 2) {
            mask.push_back(str.compare(i, 2, "??") != 0);
        }

        return mask;
    }

    DWORD MemFunctions::findPattern(const char* moduleName, ByteArray lpPattern, ByteMask pszMask)
    {
        MODULEINFO mod = { 0 };
        if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(moduleName), &mod, sizeof(MODULEINFO)))
            return 0;

        const DWORD begin = (DWORD)mod.lpBaseOfDll;
        const DWORD end = begin + mod.SizeOfImage;

        for (DWORD curAddr = begin; curAddr < end; curAddr++) {
            if (MemFunctions::maskCompare(reinterpret_cast<BYTE*>(curAddr), lpPattern, pszMask))
                return curAddr;
        }
        return 0;
    }

    void MemFunctions::write_uint8(DWORD addr, uint8_t value)
    {
        if (addr != 0)
        {
            DWORD dwProtect;
            VirtualProtect((void*)addr, 4, PAGE_READWRITE, &dwProtect);
            *(uint8_t*)(addr) = value;
            VirtualProtect((void*)addr, 4, dwProtect, new DWORD);
        }
    }
};
