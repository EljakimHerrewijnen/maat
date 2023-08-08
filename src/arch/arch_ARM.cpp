#include "maat/arch.hpp"
#include "maat/exception.hpp"
#include "maat/cpu.hpp"

namespace maat
{
    
int Arch::bits() const
{
    return _bits;
}

int Arch::octets() const
{
    return _bits/8;
}

const std::string& Arch::reg_name(reg_t num) const 
{
    for (auto it = reg_map.begin(); it != reg_map.end(); ++it)
        if (it->second == num)
            return it->first;

    throw runtime_exception(
        Fmt() << "Arch::reg_name() got unknown reg num: "
        << num
        >> Fmt::to_str
    );
}

reg_t Arch::reg_num(const std::string& name) const 
{
    auto it = reg_map.find(name);
    if (it == reg_map.end())
        throw runtime_exception(Fmt ()
                << "Arch::reg_num() got unknown reg name: " << name
                >> Fmt::to_str
                );
    return it->second;
}

serial::uid_t Arch::class_uid() const
{
    switch (type)
    {
        case Arch::Type::ARM32: return serial::ClassId::ARCH_ARM;
        case Arch::Type::ARM64: return serial::ClassId::ARCH_ARM64;
        case Arch::Type::NONE: return serial::ClassId::ARCH_NONE;
        default:
            throw serialize_exception("Arch::class_uid(): unsupported arch type");
    }
}

void Arch::dump(serial::Serializer& s) const
{
    s << serial::empty(); // Nothing to serialize
}

void Arch::load(serial::Deserializer& d)
{
    d >> serial::empty(); // Nothing to serialize
}


namespace ARM32
{
    ArchARM32::ArchARM32(): Arch(Arch::Type::ARM32, 32, ARM32::NB_REGS)
    {
        available_modes = {CPUMode::A32};
        reg_map =
        {
            {"r0", R0},
            {"r1", R1},
            {"r2", R2},
            {"r3", R3},
        };
    }

    size_t ArchARM32::reg_size(reg_t reg_num) const 
    {
        switch (reg_num)
        {
            case R0:
                return 32;
            default:
                throw runtime_exception("ArchARM32::reg_size(): got unsupported reg num");
        }
    }

    reg_t ArchARM32::sp() const 
    {
        return ARM32::SP;
    }

    reg_t ArchARM32::pc() const 
    {
        return ARM32::PC;
    }
} // namespace ARM32


namespace ARM64
{
    ArchARM64::ArchARM64(): Arch(Arch::Type::ARM64, 64, ARM64::NB_REGS)
    {
        available_modes = {CPUMode::A64};
        reg_map =
        {
            {"rax", RAX},
            {"rbx", RBX},
            {"rcx", RCX},
            {"rdx", RDX},
            {"rdi", RDI},
            {"rsi", RSI},
            {"rbp", RBP},
            {"rsp", RSP},
            {"rip", RIP},
            {"r8", R8},
            {"r9", R9},
            {"r10", R10},
            {"r11", R11},
            {"r12", R12},
            {"r13", R13},
            {"r14", R14},
            {"r15", R15},
            {"cs", CS},
            {"ds", DS},
            {"es", ES},
            {"fs", FS},
            {"gs", GS},
            {"ss", SS},
            {"cf", CF},
            {"pf", PF},
            {"af", AF},
            {"zf", ZF},
            {"sf", SF},
            {"tf", TF},
            {"if", IF},
            {"df", DF},
            {"of", OF},
            {"iopl", IOPL},
            {"vm", VM},
            {"nt", NT},
            {"rf", RF},
            {"ac", AC},
            {"vip", VIP},
            {"vif", VIF},
            {"id", ID},
            {"tsc", TSC},
            {"mm0", MM0},
            {"mm1", MM1},
            {"mm2", MM2},
            {"mm3", MM3},
            {"mm4", MM4},
            {"mm5", MM5},
            {"mm6", MM6},
            {"mm7", MM7},
            {"zmm0", ZMM0},
            {"zmm1", ZMM1},
            {"zmm2", ZMM2},
            {"zmm3", ZMM3},
            {"zmm4", ZMM4},
            {"zmm5", ZMM5},
            {"zmm6", ZMM6},
            {"zmm7", ZMM7},
            {"zmm8", ZMM8},
            {"zmm9", ZMM9},
            {"zmm10", ZMM10},
            {"zmm11", ZMM11},
            {"zmm12", ZMM12},
            {"zmm13", ZMM13},
            {"zmm14", ZMM14},
            {"zmm15", ZMM15},
            {"xcr0", XCR0},
            {"fpucw", FPUCW},
            {"fpusw", FPUSW},
            {"fputw", FPUTW},
            {"fpuip", FPUIP},
            {"fpudp", FPUDP},
            {"fpuop", FPUOP},
            {"fpucs", FPUCS},
            {"fpuds", FPUDS},
            {"c0", C0},
            {"c1", C1},
            {"c2", C2},
            {"c3", C3},
            {"cr0", CR0},
            {"st0", ST0},
            {"st1", ST1},
            {"st2", ST2},
            {"st3", ST3},
            {"st4", ST4},
            {"st5", ST5},
            {"st6", ST6},
            {"st7", ST7},
            {"mxcsr", MXCSR},
            {"ssp", SSP},
            {"rflags", RFLAGS}
        };
    }

    size_t ArchARM64::reg_size(reg_t reg_num) const 
    {
        switch (reg_num)
        {
            case RAX:
            case RBX:
            case RCX:
            case RDX:
            case RDI:
            case RSI:
            case RBP:
            case RSP:
            case SSP:
            case RIP:
            case R8:
            case R9:
            case R10:
            case R11:
            case R12:
            case R13:
            case R14:
            case R15:
            case CS:
            case DS:
            case ES:
            case FS:
            case GS:
            case SS:
                return 64;
            case CF:
            case PF:
            case AF:
            case ZF: 
            case SF:
            case TF:
            case IF:
            case DF:
            case OF:
            case IOPL:
            case VM:
            case NT:
            case RF:
            case AC:
            case VIP:
            case VIF:
            case ID: 
            case C0:
            case C1:
            case C2:
            case C3:
                return 8; // In sleigh/pcode, boolean flags are represented as bytes
            case CR0:
                return 64;
            case TSC:
            case MM0:
            case MM1:
            case MM2:
            case MM3:
            case MM4:
            case MM5:
            case MM6:
            case MM7:
                return 64;
            case ZMM0:
            case ZMM1:
            case ZMM2:
            case ZMM3:
            case ZMM4:
            case ZMM5:
            case ZMM6:
            case ZMM7:
            case ZMM8:
            case ZMM9:
            case ZMM10:
            case ZMM11:
            case ZMM12:
            case ZMM13:
            case ZMM14:
            case ZMM15:
                return 512;
            case XCR0:
                return 64;
            case FPUCW:
            case FPUSW:
            case FPUTW:
            case FPUCS:
            case FPUDS:
                return 16;
            case FPUDP:
            case FPUIP:
                return 64;
            case FPUOP:
                return 11;
            case MXCSR:
                return 32;
            case ST0:
            case ST1:
            case ST2:
            case ST3:
            case ST4:
            case ST5:
            case ST6:
            case ST7:
                return 80;
            case RFLAGS:
                return 64;
            default:
                throw runtime_exception("ArchARM64::reg_size(): got unsupported reg num");
        }
    }

    reg_t ArchARM64::sp() const 
    {
        return ARM64::RSP;
    }

    reg_t ArchARM64::pc() const 
    {
        return ARM64::RIP;
    }

    reg_t ArchARM64::tsc() const 
    {
        return ARM64::TSC;
    }
} // namespace ARM64

} // namespace maat

