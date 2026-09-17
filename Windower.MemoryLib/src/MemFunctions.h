#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#pragma comment(lib, "Psapi.lib")
#include <Psapi.h>

typedef std::vector<BYTE> ByteArray;
typedef std::vector<bool> ByteMask;

namespace memorylib
{
    class MemFunctions
    {
    public:
        static ByteArray decodeHex(std::string str);
        static ByteMask  decodeMask(std::string str);
        static std::string bytesToHex(ByteArray bytes);

        static DWORD findPattern(const char* moduleName, ByteArray pattern, ByteMask mask);

        static ByteArray read_bytes(DWORD addr, size_t count);

        static void write_uint8(DWORD addr, uint8_t value);
        static void write_bytes(DWORD addr, ByteArray values);

    private:
        static bool maskCompare(BYTE* dataPtr, ByteArray pattern, ByteMask mask);
    };

};
