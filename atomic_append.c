#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file num-bytes [x]\n"
                 "        'x' means use lseek() instead of O_APPEND\n",
                 argv[0]);

    bool useLseek = argc > 3;
    int flags = useLseek ? 0 : O_APPEND;
    int numBytes = getInt(argv[2], 0, "num-bytes");

    int fd = open(argv[1], O_RDWR | O_CREAT | flags, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    for (int j = 0; j < numBytes; j++) {
        if (useLseek)
            if (lseek(fd, 0, SEEK_END) == -1)
                errExit("lseek");
        if (write(fd, "x", 1) != 1)
            fatal("write() failed");
    }

    printf("%ld done\n", (long) getpid());
    exit(EXIT_SUCCESS);
}
