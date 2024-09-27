#ifndef ELPOPS_EXCEPTIONS_HPP
#define ELPOPS_EXCEPTIONS_HPP

#include "common.hpp"
#include "format.hpp"

namespace exceptions
{
    class CorruptFileError : public std::runtime_error {
      private:
        string path;

      public:
        explicit CorruptFileError(const string &path)
            : std::runtime_error(format("'%s' is corrupted", path.c_str())), path(path) {}

        string getPath() const { return path; }
    };

    class Unreachable : public std::runtime_error {
      public:
        explicit Unreachable() : std::runtime_error("unreachable code reached") {}
    };

    class FileNotFoundError : public std::runtime_error {
        string path;

      public:
        explicit FileNotFoundError(const string &path)
            : std::runtime_error(format("file not found: '%s'", path.c_str())), path(path) {}

        const string &getPath() const { return path; }
    };

    class SignatureError : public std::runtime_error {
      public:
        SignatureError(string sign, string msg)
            : std::runtime_error(format("invalid signature: %s: '%s'", msg.c_str(), sign.c_str())) {}

        SignatureError(string sign)
            : std::runtime_error(format("invalid signature: '%s'", sign.c_str())) {}
    };
}    // namespace exceptions

#endif    // ELPOPS_EXCEPTIONS_HPP
