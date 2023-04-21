#include <stdio.h>

const char* regFieldEncoding[2][2][8] =
{
    {
        {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"},
        {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"}
    },
    {
        {"bx + si", "bx + di", "bp + si", "bp + di", "si", "di", "DIRECT", "bx"},
        {"xx", "xx", "xx", "xx", "xx", "xx", "xx", "xx"}
    }
};

const unsigned char MOV_REG_REG = 0b100010;
const unsigned char MOV_IMM_REG = 0b1011;

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

        if ((byte >> 4) == MOV_IMM_REG)
        {
            unsigned char wOption = ((byte >> 3) & 0b1);
            unsigned char reg = (byte & 0b111);
            unsigned char data1 = fgetc(asmFile);
            if (wOption) // 16-bits register
            {
                unsigned char data2 = fgetc(asmFile);
                printf("mov %s, %hd\n", regFieldEncoding[0][wOption][reg], (data1 | data2 << 8));
            }
            else // 8-bits register
            {
                printf("mov %s, %hhd\n", regFieldEncoding[0][wOption][reg], data1);
            }
        }
        else if ((byte >> 2) == MOV_REG_REG)
        {
            unsigned char dOption = ((byte >> 1) & 0b1);
            unsigned char wOption = (byte & 0b1);

            byte = fgetc(asmFile);
            unsigned char mod = (byte >> 6);
            unsigned char reg = ((byte >> 3) & 0b111);
            unsigned char r_m = (byte & 0b111);

            if (mod == 0b11)
            {
                unsigned char* dst = &r_m;
                unsigned char* src = &reg;
                if (dOption)
                {
                    dst = &reg;
                    src = &r_m;
                }
                printf("mov %s, %s\n", regFieldEncoding[0][wOption][*dst], regFieldEncoding[0][wOption][*src]);
            }
            else
            {
                if (mod == 0)
                {
                    if (dOption)
                    {
                        printf("mov %s, [%s]\n", regFieldEncoding[0][wOption][reg], regFieldEncoding[1][0][r_m]);
                    }
                    else
                    {
                        printf("mov [%s], %s\n", regFieldEncoding[1][0][r_m], regFieldEncoding[0][wOption][reg]);
                    }
                }
                else if (mod == 1)
                {
                    unsigned char data1 = fgetc(asmFile);
                    if (dOption)
                    {
                        printf("mov %s, [%s + %hhd]\n", regFieldEncoding[0][wOption][reg], regFieldEncoding[1][0][r_m], data1);
                    }
                    else
                    {
                        printf("mov [%s + %hhd], %s\n", regFieldEncoding[1][0][r_m], data1, regFieldEncoding[0][wOption][reg]);
                    }
                }
                else
                {
                    unsigned char data1 = fgetc(asmFile);
                    unsigned char data2 = fgetc(asmFile);
                    short data = data1 | data2 << 8;
                    if (dOption)
                    {
                        printf("mov %s, [%s + %hd]\n", regFieldEncoding[0][wOption][reg], regFieldEncoding[1][0][r_m], data);
                    }
                    else
                    {
                        printf("mov [%s + %hd], %s\n", regFieldEncoding[1][0][r_m], data, regFieldEncoding[0][wOption][reg]);
                    }
                }
            }
        }
    }
    fclose(asmFile);
    return 0;
}