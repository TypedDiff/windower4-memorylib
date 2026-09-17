#include "MemFunctions.h"
#include <sstream>
#include <iomanip>

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

    std::string MemFunctions::bytesToHex(ByteArray bytes)
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (size_t i = 0; i < bytes.size(); ++i) {
            ss << std::setw(2) << static_cast<int>(bytes[i]);
        }
        return ss.str();
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

    ByteArray MemFunctions::read_bytes(DWORD addr, size_t count)
    {
        ByteArray result;

        if (addr != 0) {
            DWORD dwProtect;
            VirtualProtect((void*)addr, count, PAGE_READWRITE, &dwProtect);

            for (size_t i = 0; i < count; i++) {
                DWORD curAddr = addr + i;
                result.push_back(*(BYTE*)(curAddr));
            }

            VirtualProtect((void*)addr, count, dwProtect, new DWORD);
        }

        return result;
    }

    void MemFunctions::write_uint8(DWORD addr, uint8_t value)
    {
        if (addr != 0) {
            DWORD dwProtect;
            VirtualProtect((void*)addr, 1, PAGE_READWRITE, &dwProtect);
            *(uint8_t*)(addr) = value;
            VirtualProtect((void*)addr, 1, dwProtect, new DWORD);
        }
    }

    void MemFunctions::write_bytes(DWORD addr, ByteArray values)
    {
        DWORD dwProtect;
        VirtualProtect((void*)addr, values.size(), PAGE_READWRITE, &dwProtect);

        for (size_t i = 0; i < values.size(); i++) {
            DWORD curAddr = addr + i;
            *(BYTE*)(curAddr) = values[i];
        }

        VirtualProtect((void*)addr, values.size(), dwProtect, new DWORD);
    }
};
