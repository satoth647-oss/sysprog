#include <stdio.h>

#define MAX 1024

void to_c_string(const char *src, char *dst) {
    int j = 0;

    for (int i = 0; src[i] != '\0'; i++) {
        unsigned char c = src[i];

        switch (c) {
            case '\n': dst[j++] = '\\'; dst[j++] = 'n';  break;
            case '\r': dst[j++] = '\\'; dst[j++] = 'r';  break;
            case '\t': dst[j++] = '\\'; dst[j++] = 't';  break;
            case '\b': dst[j++] = '\\'; dst[j++] = 'b';  break;
            case '\\': dst[j++] = '\\'; dst[j++] = '\\'; break;
            case '\'': dst[j++] = '\\'; dst[j++] = '\'';  break;
            case '"':  dst[j++] = '\\'; dst[j++] = '"';  break;
            default:
                if (c >= 32 && c <= 126)
                    dst[j++] = c;
                else
                    j += sprintf(dst + j, "\\%03o", c);
        }
    }
    dst[j] = '\0';
}

int main(void) {
    char input[MAX];
    char output[4 * MAX + 1];

    printf("Type a string: ");
    if (fgets(input, MAX, stdin) == NULL)
        return 1;

    to_c_string(input, output);
    printf("C representation: %s\n", output);

    return 0;
}