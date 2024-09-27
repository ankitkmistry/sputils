#include "utils.hpp"

uint64 doubleToRaw(double number) {
    union Converter {
        uint64 digits;
        double number;
    } converter{.number = number};
    return converter.digits;
}

uint64 signedToUnsigned(int64 number) {
    union Converter {
        uint64 number1;
        int64 number2;
    } converter{.number2 = number};
    return converter.number1;
}