#ifndef SOURCE_LOADER_PARSER_HPP_
#define SOURCE_LOADER_PARSER_HPP_

#include "../spimp/exceptions.hpp"
#include "elpdef.hpp"

class ElpReader {
  private:
    uint32 index = 0;
    FILE *file;
    string path;

    MetaInfo readMetaInfo();

    ObjInfo readObjInfo();

    ClassInfo readClassInfo();

    FieldInfo readFieldInfo();

    TypeParamInfo readTypeParamInfo();

    MethodInfo readMethodInfo();

    MethodInfo::LineInfo readLineInfo();

    MethodInfo::ExceptionTableInfo readExceptionInfo();

    MethodInfo::LocalInfo readLocalInfo();

    MethodInfo::ArgInfo readArgInfo();

    MethodInfo::MatchInfo readMatchInfo();

    MethodInfo::MatchInfo::CaseInfo readCaseInfo();

    GlobalInfo readGlobalInfo();

    CpInfo readCpInfo();

    __Container readContainer();

    __UTF8 readUTF8();

    uint8 readByte() {
        index++;
        return fgetc(file);
    }

    uint16 readShort() {
        uint8 a = readByte();
        uint8 b = readByte();
        return a << 8 | b;
    }

    uint32 readInt() {
        uint16 a = readShort();
        uint16 b = readShort();
        return a << 16 | b;
    }

    uint64 readLong() {
        uint64 a = static_cast<uint64>(readInt());
        uint32 b = readInt();
        return a << 32 | b;
    }

    [[noreturn]] void corruptFileError() {
        throw exceptions::CorruptFileError(path);
    }

  public:
    explicit ElpReader(string path);

    /**
     * This function parses the file associated with this reader
     * and returns the bytecode data
     * @return The bytecode data in the form of ElpInfo
     */
    ElpInfo read();

    /**
     * Closes the file
     */
    void close() const;

    FILE *getFile() const { return file; }

    const string &getPath() const { return path; }
};

#endif /* SOURCE_LOADER_PARSER_HPP_ */
