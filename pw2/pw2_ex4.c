#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void tail_file(const char *name, int n) {
    int fd = open(name, O_RDONLY);
    if (fd < 0) {
        perror(name);
        return;
    }

    // size of the file = position of the end
    off_t size = lseek(fd, 0, SEEK_END);
    if (size <= 0 || n == 0) {
        close(fd);
        return;
    }

    long m = 1024;          // how many bytes we read from the end
    char *buf = NULL;
    long start = 0;         // where the output begins inside buf

    while (1) {
        if (m > size)
            m = size;       // can't read more than the whole file

        free(buf);
        buf = malloc(m);
        if (buf == NULL) {
            perror("malloc");
            close(fd);
            return;
        }

        // read the last m bytes of the file
        if (pread(fd, buf, m, size - m) != m) {
            perror("read");
            free(buf);
            close(fd);
            return;
        }

        // count '\n' starting from the end
        long i = m - 1;
        int count = 0;
        int found = 0;

        if (buf[i] == '\n')  // the final \n only ends the last line, skip it
            i--;

        for (; i >= 0; i--) {
            if (buf[i] == '\n') {
                count++;
                if (count == n) {
                    start = i + 1;  // output starts right after this \n
                    found = 1;
                    break;
                }
            }
        }

        if (found)
            break;
        if (m == size) {    // we read the whole file, print everything
            start = 0;
            break;
        }
        m *= 2;             // not enough lines, read more and retry
    }

    fwrite(buf + start, 1, m - start, stdout);

    free(buf);
    close(fd);
}

int main(int argc, char *argv[]) {
    int n = 10;
    int opt;

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-n lines] file...\n", argv[0]);
                return 1;
        }
    }

    if (n < 0 || optind >= argc) {
        fprintf(stderr, "Usage: %s [-n lines] file...\n", argv[0]);
        return 1;
    }

    for (int k = optind; k < argc; k++) {
        if (argc - optind > 1)
            printf("==> %s <==\n", argv[k]);
        tail_file(argv[k], n);
    }

    return 0;
}