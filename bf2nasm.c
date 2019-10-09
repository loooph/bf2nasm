#include <stdio.h>

int
main(void)
{
    size_t nested_count = 0;
    size_t linear_count[256] = { 0 };
    printf("%s%s%s%s%s%s%s",
            "section .bss\n",
            "tape:\tresb 65536\n\n",
            "section .text\n",
            "global _start\n",
            "_start:\n",
            "\tmov rbx, tape\n",
            "\tadd rbx, 32768\n");

    while (!feof(stdin)) {
        int next = fgetc(stdin);
        if (ferror(stdin)) {
            return 1;
        }
        char* cmd = "";
        switch (next) {
        case '>':
            cmd = "\tinc rbx\n";
            break;
        case '<':
            cmd = "\tdec rbx\n";
            break;
        case '+':
            cmd = "\tinc BYTE [rbx]\n";
            break;
        case '-':
            cmd = "\tdec BYTE [rbx]\n";
            break;
        case '.':
            cmd = "\tmov rax, 1\n\tmov rdi, 1\n\tmov rsi, rbx\n\tmov rdx, 1\n\tsyscall\n";
            break;
        case ',':
            cmd = "\tmov rax, 0\n\tmov rdi, 0\n\tmov rsi, rbx\n\tmov rdx, 1\n\tsyscall\n";
            break;
        case '[':
            printf("\tcmp BYTE [ebx], 0\n\tje end_%lu_%lu\nstart_%lu_%lu:\n", linear_count[nested_count],
                    nested_count, linear_count[nested_count], nested_count);
            ++nested_count;
            linear_count[nested_count] = 0;
            break;
        case ']':
            --nested_count;
            printf("\tcmp BYTE [ebx], 0\n\tjne start_%lu_%lu\nend_%lu_%lu:\n", linear_count[nested_count],
                    nested_count, linear_count[nested_count], nested_count);
            ++linear_count[nested_count];
            break;
        default:
            break;
        }
        if (*cmd != '\0') {
            printf("%s", cmd);
        }
    }
    if (nested_count) {
        fprintf(stderr, "Syntax error\n");
        return 2;
    }
    printf("\tmov rax, 60\n\tmov rdi, 0\n\tsyscall\n");
    return 0;
}
