/**
 *
 * <strong>DEFINITIONS FOR THE ELP FILE FORMAT</strong>
 * <hr>
 * ELP stands for Executable or Linkable Program<br>
 * <br>
 * ELP is of two types :-
 * <oi>
 * <li>
 * .xp file - Executable Program<br>
 *      <em>Represents a program which as an entry point to start execution</em>
 * </li>
 * <li>
 * .sll file - Spade Linkable Library<br>
 *      <em>Represents a library which can be imported by other ELPs</em>
 * </li>
 * </ol>
 *
 */

#ifndef LOADER_ELPDEF_HPP_
#define LOADER_ELPDEF_HPP_

#include "../spimp/common.hpp"

typedef uint8 ui1;
typedef uint16 ui2;
typedef uint32 ui4;
typedef uint64 ui8;

typedef ui2 cpidx;

struct __UTF8 {
    ui2 len;
    ui1 *bytes;

    bool operator==(const __UTF8 &rhs) const;

    bool operator!=(const __UTF8 &rhs) const;
};

struct CpInfo;

struct __Container {
    ui2 len;
    CpInfo *items;

    bool operator==(const __Container &rhs) const;

    bool operator!=(const __Container &rhs) const;
};

struct CpInfo {
    ui1 tag;
    union {
        ui4 _char;
        ui8 _int;
        ui8 _float;
        __UTF8 _string;
        __Container _array;
    };

    static CpInfo fromChar(uint32 c);

    static CpInfo fromInt(int64 i);

    static CpInfo fromFloat(double d);

    static CpInfo fromString(string s);

    static CpInfo fromArray(std::vector<CpInfo> v);

    bool operator==(const CpInfo &rhs) const;

    bool operator!=(const CpInfo &rhs) const;

};

struct MetaInfo {
    ui2 len;
    struct __meta {
        __UTF8 key;
        __UTF8 value;
    } *table;
};

struct GlobalInfo {
    ui1 flags;
    cpidx thisGlobal;
    cpidx type;
    MetaInfo meta;
};

struct FieldInfo {
    ui2 flags;
    cpidx thisField;
    cpidx type;
    MetaInfo meta;
};

struct MethodInfo {
    ui2 accessFlags;
    ui1 type;
    cpidx thisMethod;

    cpidx typeParams;

    ui1 argsCount;
    struct ArgInfo {
        cpidx thisArg;
        cpidx type;
        MetaInfo meta;
    } *args;

    ui2 localsCount;
    ui2 closureStart;
    struct LocalInfo {
        cpidx thisLocal;
        cpidx type;
        MetaInfo meta;
    } *locals;

    ui4 maxStack;
    ui4 codeCount;
    ui1 *code;

    ui2 exceptionTableCount;
    struct ExceptionTableInfo {
        ui4 startPc;
        ui4 endPc;
        ui4 targetPc;
        cpidx exception;
        MetaInfo meta;
    } *exceptionTable;

    struct LineInfo {
        ui2 numberCount;
        struct NumberInfo {
            ui1 times;
            ui4 lineno;
        } *numbers;
    } lineInfo;

    ui2 lambdaCount;
    MethodInfo *lambdas;

    ui2 matchCount;
    struct MatchInfo {
        ui2 caseCount;
        struct CaseInfo {
            cpidx value;
            ui4 location;
        } *cases;
        ui4 defaultLocation;
        MetaInfo meta;
    } *matches;

    MetaInfo meta;
};

struct ObjInfo;

struct ClassInfo {
    ui1 type;
    ui2 accessFlags;
    cpidx thisClass;
    cpidx typeParams;
    cpidx supers;
    ui2 fieldsCount;
    FieldInfo *fields;
    ui2 methodsCount;
    MethodInfo *methods;
    ui2 objectsCount;
    ObjInfo *objects;
    MetaInfo meta;
};

struct ObjInfo {
    ui1 type;
    union {
        MethodInfo _method;
        ClassInfo _class;
    };
};

struct ElpInfo {
    ui4 magic;
    ui4 minorVersion;
    ui4 majorVersion;

    cpidx compiledFrom;
    ui1 type;
    cpidx thisModule;

    cpidx init;
    cpidx entry;
    cpidx imports;

    ui2 constantPoolCount;
    CpInfo *constantPool;
    ui2 globalsCount;
    GlobalInfo *globals;
    ui2 objectsCount;
    ObjInfo *objects;
    MetaInfo meta;
};

#endif /* LOADER_ELPDEF_HPP_ */
