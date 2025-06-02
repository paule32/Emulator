// ---------------------------------------------------------------------------
// File:   Emulator.cpp
// Author: Jens Kallup - paule32
//
// This file is part of Emulator.
//
// (c) Copyright 2025 Jens Kallup - paule32
// all rights reserved.
// 
// only for non-profit usage !!!
// ---------------------------------------------------------------------------
#include "Emulator.h"

constexpr int MEM_SIZE  = 0x10000;
constexpr int MEM_START = 0x1000;

uint8_t memory[MEM_SIZE];

typedef struct {
	uint64_t rax, rbx, rcx, rdx, rsp, rdi;
	uint64_t rip;
} CPUState;

typedef struct {
    unsigned char e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_Phdr;

# define PT_LOAD    1
# define PF_R       4
# define PF_X       1

// Hilfsfunktion: Register-Pointer holen
uint64_t* get_reg_ptr(CPUState* cpu, uint8_t reg_id) {
    switch (reg_id) {
    case 0: return &cpu->rax;
    case 1: return &cpu->rbx;
    case 2: return &cpu->rcx;
    case 3: return &cpu->rdx;
    case 4: return &cpu->rsp;
    case 5: return &cpu->rdi;
    default: return NULL;
    }
}

void dump_regs(CPUState* cpu) {
    std::cout
        << "RAX="
        << std::showbase
        << std::showbase        // 0x Präfix
        << std::internal        // Padding hinter '0x'
        << std::setfill('0')    // Füllen mit Nullen
        << std::setw(18)        // (2 DWORD) + 2 Bytes: 0x
        << std::hex
        << cpu->rax
        << std::endl
        << "RBX="
        << std::showbase        // 0x Präfix
        << std::internal        // Padding hinter '0x'
        << std::setfill('0')    // Füllen mit Nullen
        << std::setw(18)        // (2 DWORD) + 2 Bytes: 0x
        << std::hex
        << cpu->rbx
        << std::endl
        << "RCX="
        << std::showbase        // 0x Präfix
        << std::internal        // Padding hinter '0x'
        << std::setfill('0')    // Füllen mit Nullen
        << std::setw(18)        // (2 DWORD) + 2 Bytes: 0x
        << std::hex
        << cpu->rcx
        << std::endl
        << "RDX="
        << std::showbase        // 0x Präfix
        << std::internal        // Padding hinter '0x'
        << std::setfill('0')    // Füllen mit Nullen
        << std::setw(18)        // (2 DWORD) + 2 Bytes: 0x
        << std::hex
        << cpu->rdx
        << std::endl
        << "RSP="
        << std::showbase        // 0x Präfix
        << std::internal        // Padding hinter '0x'
        << std::setfill('0')    // Füllen mit Nullen
        << std::setw(18)        // (2 DWORD) + 2 Bytes: 0x
        << std::hex
        << cpu->rsp
        << std::endl
        << "RDI="
        << std::showbase        // 0x Präfix
        << std::internal        // Padding hinter '0x'
        << std::setfill('0')    // Füllen mit Nullen
        << std::setw(18)        // (2 DWORD) + 2 Bytes: 0x
        << std::hex
        << cpu->rdi
        << std::endl
        << "RIP="
        << std::showbase        // 0x Präfix
        << std::internal        // Padding hinter '0x'
        << std::setfill('0')    // Füllen mit Nullen
        << std::setw(18)        // (2 DWORD) + 2 Bytes: 0x
        << std::hex
        << cpu->rip
        << std::endl
        << std::endl;
}

void print_header(const Elf64_Ehdr* hdr) {
    printf("ELF Header:\n");
    printf("  Magic:   %02x %02x %02x %02x\n",
        hdr->e_ident[0], hdr->e_ident[1], hdr->e_ident[2], hdr->e_ident[3]);

    printf("  Type:              0x%04x\n", hdr->e_type);
    printf("  Machine:           0x%04x\n", hdr->e_machine);
    printf("  Entry point:       0x%016llx\n", (unsigned long long)hdr->e_entry);
    printf("  Program header @   0x%llx (%u Einträge)\n",
        (unsigned long long)hdr->e_phoff, hdr->e_phnum);
    printf("  Section header @   0x%llx (%u Einträge)\n",
        (unsigned long long)hdr->e_shoff, hdr->e_shnum);
}

void parse_program_headers(FILE* f, const Elf64_Ehdr* hdr) {
    printf("\nProgram Headers:\n");
    fseek(f, hdr->e_phoff, SEEK_SET);

    for (int i = 0; i < hdr->e_phnum; ++i) {
        Elf64_Phdr ph;
        fread(&ph, sizeof(ph), 1, f);

        printf("  [%2d] Type: 0x%08x Offset: 0x%08llx VirtAddr: 0x%016llx MemSize: 0x%08llx\n",
            i, ph.p_type, (unsigned long long)ph.p_offset,
            (unsigned long long)ph.p_vaddr, (unsigned long long)ph.p_memsz);
    }
}

int write_elf(void)
{
    FILE* f = fopen("program.elf", "wb");
    if (!f) {
        perror("Datei öffnen");
        return EXIT_FAILURE;
    }

    unsigned char code[] = {
        0x10, 0x00, 0x78, 0x56, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00,
        0x30, 0x00,
        0x10, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x20, 0x00, 0x01,
        0x31, 0x02,
        0xFF
    };

    size_t code_size = sizeof(code);

    Elf64_Ehdr ehdr = { 0 };
    memcpy(ehdr.e_ident, "\x7f""ELF", 4);
    ehdr.e_ident[4] = 2; // 64-bit
    ehdr.e_ident[5] = 1; // little endian
    ehdr.e_ident[6] = 1; // version
    ehdr.e_type = 2; // ET_EXEC
    ehdr.e_machine = 0x3E; // x86_64
    ehdr.e_version = 1;
    ehdr.e_entry = MEM_START;
    ehdr.e_phoff = sizeof(Elf64_Ehdr);
    ehdr.e_ehsize = sizeof(Elf64_Ehdr);
    ehdr.e_phentsize = sizeof(Elf64_Phdr);
    ehdr.e_phnum = 1;

    Elf64_Phdr phdr = { 0 };
    phdr.p_type = PT_LOAD;
    phdr.p_flags = PF_R | PF_X;
    phdr.p_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);
    phdr.p_vaddr = MEM_START;
    phdr.p_paddr = MEM_START;
    phdr.p_filesz = code_size;
    phdr.p_memsz = code_size;
    phdr.p_align = 0x1000;

    fwrite(&ehdr, 1, sizeof(ehdr), f);
    fwrite(&phdr, 1, sizeof(phdr), f);
    fwrite(code, 1, code_size, f);

    fclose(f);
    printf("program.elf erzeugt!\n");

    return EXIT_SUCCESS;
}

int load_elf(const char* filename, CPUState* cpu) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        perror("Datei öffnen");
        return 0;
    }

    Elf64_Ehdr ehdr;
    if (fread(&ehdr, 1, sizeof(ehdr), f) != sizeof(ehdr)) {
        printf("Fehler beim Lesen des ELF-Headers\n");
        fclose(f);
        return 0;
    }

    // Prüfe ELF Magic
    if (memcmp(ehdr.e_ident, "\x7f""ELF", 4) != 0) {
        printf("Keine ELF-Datei\n");
        fclose(f);
        return 0;
    }

    if (ehdr.e_phoff == 0 || ehdr.e_phnum == 0) {
        printf("Keine Program Header\n");
        fclose(f);
        return 0;
    }

    // Für jeden Program Header
    for (int i = 0; i < ehdr.e_phnum; i++) {
        fseek(f, ehdr.e_phoff + i * sizeof(Elf64_Phdr), SEEK_SET);
        Elf64_Phdr phdr;
        if (fread(&phdr, 1, sizeof(phdr), f) != sizeof(phdr)) {
            printf("Fehler beim Lesen des Program Headers\n");
            fclose(f);
            return 0;
        }

        if (phdr.p_type == PT_LOAD) {
            if (phdr.p_vaddr + phdr.p_filesz > MEM_SIZE) {
                printf("Segment passt nicht in Speicher\n");
                fclose(f);
                return 0;
            }

            fseek(f, phdr.p_offset, SEEK_SET);
            if (fread(memory + phdr.p_vaddr, 1, phdr.p_filesz, f) != phdr.p_filesz) {
                printf("Fehler beim Lesen des Segment-Inhalts\n");
                fclose(f);
                return 0;
            }
            printf("Segment geladen: vaddr=0x%llx, size=0x%llx\n", phdr.p_vaddr, phdr.p_filesz);
        }
    }

    print_header(&ehdr);
    parse_program_headers(f, &ehdr);

    fclose(f);
    cpu->rip = ehdr.e_entry;  // Setze Startadresse aus ELF-Header
    return 1;
}

void emulate(CPUState* cpu) {
    int running = 1;
    while (running) {
        if (cpu->rip >= MEM_SIZE) {
            printf("RIP außerhalb des Speicherbereichs!\n");
            break;
        }
        uint8_t opcode = memory[cpu->rip++];
        switch (opcode) {
        case 0x10: { // load immediate in reg
            if (cpu->rip + 8 > MEM_SIZE) {
                printf("Fehler: Kein Platz für Immediate\n");
                running = 0;
                break;
            }
            uint8_t reg_id = memory[cpu->rip++];
            uint64_t val = *(uint64_t*)(memory + cpu->rip);
            cpu->rip += 8;
            uint64_t* reg = get_reg_ptr(cpu, reg_id);
            if (reg) {
                *reg = val;
                printf("LOAD imm64 0x%llx in reg %d\n", val, reg_id);
            }
            else {
                printf("Ungültiges Register %d\n", reg_id);
                running = 0;
            }
            break;
        }
        case 0x20: { // reg1 += reg2
            if (cpu->rip + 2 > MEM_SIZE) {
                printf("Fehler: Kein Platz für Register IDs\n");
                running = 0;
                break;
            }
            uint8_t reg1 = memory[cpu->rip++];
            uint8_t reg2 = memory[cpu->rip++];
            uint64_t* r1 = get_reg_ptr(cpu, reg1);
            uint64_t* r2 = get_reg_ptr(cpu, reg2);
            if (r1 && r2) {
                *r1 += *r2;
                printf("ADD reg%d += reg%d -> 0x%llx\n", reg1, reg2, *r1);
            }
            else {
                printf("Ungültiges Register in ADD\n");
                running = 0;
            }
            break;
        }
        case 0x30: { // push reg
            if (cpu->rip + 1 > MEM_SIZE) {
                printf("Fehler: Kein Platz für Register ID\n");
                running = 0;
                break;
            }
            uint8_t reg_id = memory[cpu->rip++];
            uint64_t* reg = get_reg_ptr(cpu, reg_id);
            if (reg) {
                if (cpu->rsp < 8) {
                    printf("Stack Overflow!\n");
                    running = 0;
                    break;
                }
                cpu->rsp -= 8;
                *(uint64_t*)(memory + cpu->rsp) = *reg;
                printf("PUSH reg%d = 0x%llx\n", reg_id, *reg);
            }
            else {
                printf("Ungültiges Register in PUSH\n");
                running = 0;
            }
            break;
        }
        case 0x31: { // pop reg
            if (cpu->rip + 1 > MEM_SIZE) {
                printf("Fehler: Kein Platz für Register ID\n");
                running = 0;
                break;
            }
            uint8_t reg_id = memory[cpu->rip++];
            uint64_t* reg = get_reg_ptr(cpu, reg_id);
            if (reg) {
                if (cpu->rsp >= MEM_SIZE) {
                    printf("Stack Underflow!\n");
                    running = 0;
                    break;
                }
                *reg = *(uint64_t*)(memory + cpu->rsp);
                cpu->rsp += 8;
                printf("POP reg%d = 0x%llx\n", reg_id, *reg);
            }
            else {
                printf("Ungültiges Register in POP\n");
                running = 0;
            }
            break;
        }
        case 0xFF: {
            printf("Programmende erreicht\n");
            running = 0;
            break;
        }
        default:
            printf("Unbekannter Opcode 0x%02x bei RIP=0x%llx\n", opcode, cpu->rip - 1);
            running = 0;
            break;
        }
        dump_regs(cpu);
    }
}

using namespace std;

int main(int argc, char** argv)
{
    std::cout
        << "Emulator 1.0.0 (c) 2025 by Jens Kallup - paule32"
        << std::endl
        << "all rights reserved."
        << std::endl
        << std::endl;

    if (write_elf() == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (argc < 2) {
        std::string exePath = getExecutablePath();
        std::filesystem::path pathObj(exePath);
        std::cerr
            << "Usage: "
            << pathObj.filename().string()
            << " program.elf"
            << std::endl;

        return EXIT_FAILURE;
    }

    CPUState cpu = { 0 };
    cpu.rip = MEM_START;
    cpu.rsp = MEM_SIZE;  // Stack beginnt am Ende des Speichers

    if (!load_elf(argv[1], &cpu)) {
        std::cerr
            << "ELF Laden fehlgeschlagen"
            << std::endl;

        return EXIT_FAILURE;
    }

    std::cout
        << "Starte Emulator bei RIP="
        << std::showbase
        << std::setw(sizeof(uint64_t) + 2)    // (2 DWORD) + 2 Bytes: 0x
        << std::setfill('0')
        << ios::hex
        << cpu.rip
        << std::endl;

    emulate(&cpu);

    return EXIT_SUCCESS;
}
