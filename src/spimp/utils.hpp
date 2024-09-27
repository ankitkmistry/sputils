#ifndef ELPOPS_UTILS_HPP
#define ELPOPS_UTILS_HPP

#include "common.hpp"

uint64 doubleToRaw(double number);

uint64 signedToUnsigned(int64 number);

template<typename T>
static vector<T> slice(vector<T> list, int64 start, int64 end) {
    if (start < 0) start += list.size();
    if (end < 0) end += list.size();
    if (start >= list.size() || end >= list.size()) throw std::runtime_error("slice(): index out of bounds");
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }
    vector<T> result = {};
    for (int i = start; i < end; ++i) { result.push_back(list[i]); }
    return result;
}

string join(const vector<string> list, string delimiter) {
    string text = "";
    for (int i = 0; i < list.size(); ++i) {
        text += list[i];
        if (i < list.size() - 1) text += delimiter;
    }
    return text;
}

#endif    // ELPOPS_UTILS_HPP
