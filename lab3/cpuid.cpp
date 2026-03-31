#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <bitset>

/*bool isCPUIDAvailable() {
    bool canToggle = false;
    unsigned long long rflags;

    __asm__ volatile (
        "pushfq\n"
        "popq %0\n"
        : "=r" (rflags)
    );

    unsigned long long newRflags = rflags ^ 0x200000;

    __asm__ volatile (
        "pushq %0\n"
        "popfq\n"
        :
        : "r" (newRflags)
    );

    unsigned long long resultRflags;
    __asm__ volatile (
        "pushfq\n"
        "popq %0\n"
        : "=r" (resultRflags)
    );

    canToggle = ((resultRflags & 0x200000) != (rflags & 0x200000));

    __asm__ volatile (
        "pushq %0\n"
        "popfq\n"
        :
        : "r" (rflags)
    );

    return canToggle;
}*/

void cpuid(unsigned int eax, unsigned int ecx, unsigned int* regs) {
    //pochemu-to problemy s intrinsicami
    __asm__ volatile (
        "cpuid"
        : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
        : "a" (eax), "c" (ecx)
    );
}

void printCPUIDInfo() {
    /*if (!isCPUIDAvailable()) {
        std::cerr << "CPUID is not available on this system." << std::endl;
        return;
    }*/

    unsigned int regs[4];

    std::cout << "---------------=============== EAX = 0 ===============---------------" << std::endl;
    cpuid(0, 0, regs);
    std::string vendor;
    vendor.assign(reinterpret_cast<const char*>(&regs[1]), 4);
    vendor.append(reinterpret_cast<const char*>(&regs[3]), 4);
    vendor.append(reinterpret_cast<const char*>(&regs[2]), 4);
    std::cout << "Number of basic functions = " << regs[0] << std::endl;
    std::cout << "Vendor = " << vendor << std::endl;

    std::cout << "\n---------------=============== EAX = 1 ===============---------------" << std::endl;
    cpuid(1, 0, regs);
    std::cout << "EAX info = " << std::hex << std::setw(8) << std::setfill(' ') << regs[0] << "h" << std::endl;
    std::cout << "    Version info = " << std::hex << std::setw(6) << std::setfill(' ') << regs[0] << "h" << std::endl;
    std::cout << "    Stepping ID      = " << std::hex << ((regs[0] >> 0) & 0xF) << "h" << std::endl;
    std::cout << "    Model            = " << std::hex << ((regs[0] >> 4) & 0xF) << "h" << std::endl;
    std::cout << "    Family           = " << std::hex << ((regs[0] >> 8) & 0xF) << "h" << std::endl;
    std::cout << "    Processor type   = " << std::hex << ((regs[0] >> 12) & 0x3) << "h" << std::endl;
    std::cout << "    Extended model   = " << std::hex << ((regs[0] >> 16) & 0xF) << "h" << std::endl;
    std::cout << "    Extended family  = " << std::hex << ((regs[0] >> 20) & 0xFF) << "h" << std::endl;

    std::cout << "\nEBX info = " << std::hex << std::setw(8) << std::setfill(' ') << regs[1] << "h" << std::endl;
    std::cout << "    Brand index = " << std::hex << std::setw(2) << std::setfill(' ') << ((regs[1] >> 0) & 0xFF) << "h" << std::endl;
    std::cout << "    CLFLUSH line size = " << std::hex << std::setw(2) << std::setfill(' ') << ((regs[1] >> 8) & 0xFF) << "h = " << std::dec << (((regs[1] >> 8) & 0xFF) * 8) << " Bytes" << std::endl;
    std::cout << "    Maximal IDs number of logical processors per physical = " << std::hex << std::setw(2) << std::setfill(' ') << ((regs[1] >> 16) & 0xFF) << "h = " << std::dec << ((regs[1] >> 16) & 0xFF) << std::endl;
    std::cout << "    Local APIC ID = " << std::hex << std::setw(2) << std::setfill(' ') << ((regs[1] >> 24) & 0xFF) << "h" << std::endl;

    std::cout << "\nEDX info = " << std::hex << std::setw(8) << std::setfill(' ') << regs[3] << "h" << std::endl;
    const char* featuresEDX[] = {
        "FPU", "VME", "DE", "PSE", "TSC", "MSR", "PAE", "MCE",
        "CMPXCHG8B", "APIC", "SEP", "MTRR", "PGE", "MCA", "CMOV", "PAT",
        "PSE36", "PSN", "CLFLUSH", "DS", "ACPI", "MMX", "FXSR", "SSE",
        "SSE2", "SS", "HTT", "TM", "PBE"
    };
    for (int i = 0; i < 32; ++i) {
        if (i < sizeof(featuresEDX)/sizeof(featuresEDX[0])) {
            std::cout << "    " << std::left << std::setw(35) << featuresEDX[i] << " = " << ((regs[3] >> i) & 1) << std::endl;
        }
    }

    std::cout << "\nECX info = " << std::hex << std::setw(8) << std::setfill(' ') << regs[2] << "h" << std::endl;
    const char* featuresECX[] = {
        "SSE3", "PCLMULQDQ", "SSSE3", "FMA3", "SSE4.1", "SSE4.2", "POPCNT", "AES",
        "AVX", "RDRAND", "OSXSAVE", "XSAVE", "XRSTOR", "XSAVEOPT", "LWP", "FMA4",
        "TBM", "TOPEXT", "PCONFIG", "IBT", "AMX-BF16", "AMX-TILE", "AMX-INT8", "IBRS",
        "STIBP", "L1D_FLUSH", "ARCH_CAPABILITIES", "CORE_CAPABILITIES", "SSBD"
    };
    for (int i = 0; i < 32; ++i) {
        if (i < sizeof(featuresECX)/sizeof(featuresECX[0])) {
            std::cout << "    " << std::left << std::setw(35) << featuresECX[i] << " = " << ((regs[2] >> i) & 1) << std::endl;
        }
    }

    std::cout << "\n------------============ EAX = 80000002h ... 80000004h ============------------" << std::endl;
    std::string brandStr;
    for (unsigned int eax = 0x80000002; eax <= 0x80000004; ++eax) {
        cpuid(eax, 0, regs);
        brandStr.append(reinterpret_cast<const char*>(&regs[0]), 4);
        brandStr.append(reinterpret_cast<const char*>(&regs[1]), 4);
        brandStr.append(reinterpret_cast<const char*>(&regs[2]), 4);
        brandStr.append(reinterpret_cast<const char*>(&regs[3]), 4);
    }
    std::cout << "CPU brand string: " << brandStr << std::endl;
}

int main() {
    printCPUIDInfo();
    return 0;
}
