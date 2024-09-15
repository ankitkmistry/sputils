#ifndef VELOCITY_OPCODE_HPP
#define VELOCITY_OPCODE_HPP

#include "../spimp/common.hpp"

/**
 * Enum containing all opcodes of the bytecode language
 */
enum class Opcode {
    // Nop
    /// no operation
    NOP,

    // Stack op
    /// load constant
    CONST,
    /// load long constant
    CONSTL,
    /// pop
    POP,
    /// duplicate top of stack
    DUP,

    // Load-store op
    /// load global
    GLOAD,
    /// load global fast
    GFLOAD,

    /// store global
    GSTORE,
    /// store global fast
    GFSTORE,
    /// pop store global
    PGSTORE,
    /// pop store global fast
    PGFSTORE,

    /// load local
    LLOAD,
    /// load local
    LFLOAD,

    /// store local
    LSTORE,
    /// store local fast
    LFSTORE,
    /// pop store local
    PLSTORE,
    /// pop store local fast
    PLFSTORE,

    /// load arg
    ALOAD,

    /// store arg
    ASTORE,
    /// store arg fast
    PASTORE,

    /// load typearg
    TLOAD,

    /// store typearg
    TSTORE,
    /// store typearg fast
    PTSTORE,

    /// load member
    MLOAD,
    /// load member fast
    MFLOAD,

    /// store member
    MSTORE,
    /// store member fast
    MFSTORE,
    /// pop store member
    PMSTORE,
    /// pop store member fast
    PMFSTORE,

    /// load static
    SLOAD,
    /// load static fast
    SFLOAD,

    /// store static
    SSTORE,
    /// store static fast
    SFSTORE,
    /// pop store static
    PSSTORE,
    /// pop store static fast
    PSFSTORE,

    /// load lambda
    BLOAD,
    /// load lambda fast
    BFLOAD,

    // Array. op
    /// pack array
    ARRPACK,
    /// unpack array
    ARRUNPACK,
    /// build array
    ARRBUILD,
    /// build array fast
    ARRFBUILD,
    /// load array index
    ILOAD,
    /// load array index
    ISTORE,
    /// pop and load array index
    PISTORE,
    /// array length
    ARRLEN,

    // Call op
    /// invoke
    INVOKE,
    /// invoke virtual
    VINVOKE,
    /// invoke static
    SINVOKE,
    /// invoke local
    LINVOKE,
    /// invoke global
    GINVOKE,
    /// invoke arg
    AINVOKE,

    /// invoke virtual fast
    VFINVOKE,
    /// invoke static fast
    SFINVOKE,
    /// invoke local fast
    LFINVOKE,
    /// invoke global fast
    GFINVOKE,

    /// sub call
    CALLSUB,
    /// sub return
    RETSUB,

    // Jump op
    /// jump forward
    JFW,
    /// jump backward
    JBW,
    /// pop jump if true
    JT,
    /// pop jump if false
    JF,
    /// pop jump if less than
    JLT,
    /// pop jump if less than or equal
    JLE,
    /// pop jump if equal
    JEQ,
    /// pop jump if not equal
    JNE,
    /// pop jump if greater than or equal
    JGE,
    /// pop jump if greater than
    JGT,

    // Primitive op
    /// not
    NOT,
    /// invert
    INV,
    /// negate
    NEG,
    /// get type
    GETTYPE,
    /// safe cast
    SCAST,
    /// checked cast
    CCAST,
    /// power
    POW,
    /// multiply
    MUL,
    /// divide
    DIV,
    /// remainder
    REM,
    /// add
    ADD,
    /// subtract
    SUB,
    /// shift left
    SHL,
    /// shift right
    SHR,
    /// unsigned shift right
    USHR,
    /// bit and
    AND,
    /// bit or
    OR,
    /// bit xor
    XOR,
    /// less than
    LT,
    /// less than or equal
    LE,
    /// equal
    EQ,
    /// not equal
    NE,
    /// greater than or equal
    GE,
    /// greater than
    GT,
    /// is
    IS,
    /// is not
    NIS,
    /// is null
    ISNULL,
    /// is not null
    NISNULL,

    // thread safety
    /// enter monitor
    ENTERMONITOR,
    /// exit monitor
    EXITMONITOR,

    // Miscellaneous
    /// perform match
    MTPERF,
    /// perform match fast
    MTFPERF,
    /// load closure
    CLOSURELOAD,
    /// load reified object
    REIFIEDLOAD,
    /// load object
    OBJLOAD,
    /// throw
    THROW,
    /// return
    RET,
    /// return void
    VRET,
    /// return n items from the top of stack
    NRET,

    // Debug op
    PRINTLN,
    NUM_OPCODES
};

/**
 * Contains debug info for all opcodes
 */
class OpcodeInfo {
public:
    /**
     * @param opcode
     * @return string representation of the opcode
     */
    static string toString(Opcode opcode);

    /**
     * @param opcode
     * @return number of params needed for the opcode
     */
    static uint8 getParams(Opcode opcode);

    /**
     * @param opcode
     * @return whether the param of the opcode refers to the constant pool
     */
    static bool takeFromConstPool(Opcode opcode);

    /**
     * @param str
     * @return the opcode associated with str, Opcode::NOP otherwise
     */
    static Opcode fromString(string str);
};

#endif //VELOCITY_OPCODE_HPP
