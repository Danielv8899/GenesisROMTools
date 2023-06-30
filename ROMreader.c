#include <stdio.h>

typedef struct {
    char name[16];
    char copyright_release[16];
    char game_title_domestic[48];
    char game_title_overseas[48];
    char serial_number[14];
    unsigned short checksum;
    char device_support[16];
    unsigned int rom_address_range[2];
    unsigned int ram_address_range[2];
    char extra_memory[12];
    char modem_support[12];
    char reserved[40];
    char region[3];
    char reserved2[13];
}ROMheader;

#define SWAP_ENDIANNES(value) (((value) >> 24) & 0xFF) | \
                              (((value) >> 8) & 0xFF00) | \
                              (((value) << 8) & 0xFF0000) | \
                              (((value) << 24) & 0xFF000000)

void printHeader(ROMheader header,int ADDR) {
    int rom_low = SWAP_ENDIANNES(header.rom_address_range[0]);
    rom_low = rom_low + ADDR;
    int rom_high = SWAP_ENDIANNES(header.rom_address_range[1]);
    rom_high = rom_high + ADDR;
    int ram_low = SWAP_ENDIANNES(header.ram_address_range[0]);
    ram_low = ram_low + ADDR;
    int ram_high = SWAP_ENDIANNES(header.ram_address_range[1]);
    ram_high = ram_high + ADDR;


    printf("Name: %.*s\n",16, header.name);
    printf("Copyright/Release: %.*s\n",16, header.copyright_release);
    printf("Game Title (Domestic): %.*s\n",48, header.game_title_domestic);
    printf("Game Title (Overseas): %.*s\n",48, header.game_title_overseas);
    printf("Serial Number: %.*s\n",14, header.serial_number);
    printf("Checksum: %x\n", header.checksum);
    printf("Device Support: %.*s\n",16, header.device_support);
    printf("ROM Address Range: %x - %x\n", rom_low, rom_high);
    printf("RAM Address Range: %x - %x\n", ram_low, ram_high);
    printf("Extra Memory: %.*s\n",12, header.extra_memory);
    printf("Modem Support: %.*s\n",12, header.modem_support);
    printf("Region: %.*s\n",3, header.region);
}

void ReadRom(FILE* fp, int ADDR){
    ROMheader header;
    int res = fread(&header, 1, sizeof(header), fp);
    if (res != sizeof(header)) {
        return;
    }

    printHeader(header,ADDR);
    int NEXT_ADDR = SWAP_ENDIANNES(header.rom_address_range[1]);
    NEXT_ADDR = NEXT_ADDR + ADDR;
    if (NEXT_ADDR & 1) {
        ADDR = NEXT_ADDR +1;
        NEXT_ADDR+= 0x101;
    }
    else {
        ADDR = NEXT_ADDR;
        NEXT_ADDR+= 0x100;
    }
    res = fseek(fp, NEXT_ADDR, SEEK_SET);
    if (res != 0) {
        printf("Error: Could not seek to ROM\n");
        return;
    }   
    ReadRom(fp,ADDR);
}

int main (int argc , char *argv[]) {
    
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    int res = fseek(fp, 0x100, SEEK_SET);
    if (res != 0) {
        printf("Error: Could not seek to header\n");
        return 1;
    }
    ReadRom(fp,0);
    return 0;
}