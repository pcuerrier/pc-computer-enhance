#include <stdio.h>

constexpr const char* reg8FieldEncoding[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
constexpr const char* reg16FieldEncoding[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Invalid usage! Usage: sim8086 <asm file>\n");
        return 1;
    }

    FILE* asmFile = fopen(argv[1], "r");
    if (!asmFile)
    {
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }

    printf("bits 16\n");
    while (!feof(asmFile))
    {
        unsigned char byte = fgetc(asmFile);
        unsigned char optcode = (byte >> 2);
        unsigned char dOption = ((byte >> 1) & 0b1);
        unsigned char wOption = (byte & 0b1);

        byte = fgetc(asmFile);
        //unsigned char mod = (byte >> 6);
        unsigned char reg = ((byte >> 3) & 0b111);
        unsigned char rem = (byte & 0b111);

        if (optcode == 0b100010) // mov
        {
            unsigned char* dst = &rem;
            unsigned char* src = &reg;
            if (dOption)
            {
                dst = &reg;
                src = &rem;
            }
            printf("mov %s, %s\n",
                wOption ? reg16FieldEncoding[*dst] : reg8FieldEncoding[*dst],
                wOption ? reg16FieldEncoding[*src] : reg8FieldEncoding[*src]
            );
        }
    }
    fclose(asmFile);
    return 0;
}