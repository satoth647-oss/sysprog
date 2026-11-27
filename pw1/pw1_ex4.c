#include <stdio.h>
#include <string.h>

#define MAX 1024

int from_c_string(const char *src, char *dst) {
    int j = 0;

    for (int i = 0; src[i] != '\0'; i++) {
        if (src[i] != '\\') {
            dst[j++] = src[i];
            continue;
        }

        i++;
        switch (src[i]) {
            case 'n':  dst[j++] = '\n'; break;
            case 'r':  dst[j++] = '\r'; break;
            case 't':  dst[j++] = '\t'; break;
            case 'b':  dst[j++] = '\b'; break;
            case '\\': dst[j++] = '\\'; break;
            case '\'': dst[j++] = '\''; break;
            case '"':  dst[j++] = '"';  break;
            default:
                if (src[i] >= '0' && src[i] <= '7') {
                    int val = 0;
                    int k = 0;
                    while (k < 3 && src[i] >= '0' && src[i] <= '7') {
                        val = val * 8 + (src[i] - '0');
                        i++;
                        k++;
                    }
                    i--;
                    dst[j++] = (char)val;
                } else if (src[i] == '\0') {
                    dst[j++] = '\\';
                    i--;
                } else {
                    dst[j++] = '\\';
                    dst[j++] = src[i];
                }
        }
    }
    return j;
}

int main(void) {
    char input[MAX];
    char output[MAX];

    printf("Type a string: ");
    if (fgets(input, MAX, stdin) == NULL)
        return 1;

    input[strcspn(input, "\n")] = '\0';

    int len = from_c_string(input, output);

    printf("Translated: ");
    fwrite(output, 1, len, stdout);
    printf("\n");

    return 0;
}