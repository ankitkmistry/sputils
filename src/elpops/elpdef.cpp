#include "elpdef.hpp"
#include "../spimp/exceptions.hpp"
#include "../spimp/utils.hpp"

bool CpInfo::operator==(const CpInfo &rhs) const {
    if (tag != rhs.tag) return false;
    switch (tag) {
        case 0x03:
            return _char == rhs._char;
        case 0x04:
            return _int == rhs._int;
        case 0x05:
            return _float == rhs._float;
        case 0x06:
            return _string == rhs._string;
        case 0x07:
            return _array == rhs._array;
        default:
            throw exceptions::Unreachable();
    }
}

bool CpInfo::operator!=(const CpInfo &rhs) const {
    return !(rhs == *this);
}

CpInfo CpInfo::fromChar(uint32 c) {
    return CpInfo{.tag = 0x03, ._char = c};
}

CpInfo CpInfo::fromInt(int64 i) {
    return CpInfo{.tag = 0x04, ._int = signedToUnsigned(i)};
}

CpInfo CpInfo::fromFloat(double d) {
    return CpInfo{.tag = 0x05, ._float = doubleToRaw(d)};
}

CpInfo CpInfo::fromString(string s) {
    __UTF8 str;
    str.len = s.size();
    str.bytes = new ui1[str.len];
    for (int i = 0; i < str.len; ++i) {
        str.bytes[i] = s[i];
    }
    return CpInfo{.tag = 0x06, ._string = str};
}

CpInfo CpInfo::fromArray(std::vector<CpInfo> v) {
    __Container arr;
    arr.len = v.size();
    arr.items = new CpInfo[arr.len];
    for (int i = 0; i < arr.len; ++i) {
        arr.items = &v[i];
    }
    return CpInfo{.tag = 0x07, ._array = arr};
}

bool __UTF8::operator==(const __UTF8 &rhs) const {
    return len == rhs.len &&
           memcmp(bytes, rhs.bytes, len) == 0;
}

bool __UTF8::operator!=(const __UTF8 &rhs) const {
    return !(rhs == *this);
}

bool __Container::operator==(const __Container &rhs) const {
    if (len != rhs.len) return false;
    for (int i = 0; i < len; ++i)
        if (items[i] != rhs.items[i]) return false;
    return true;
}

bool __Container::operator!=(const __Container &rhs) const {
    return !(rhs == *this);
}
