#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    int piper2[2];

    piper2[0] = -1;
    piper2[1] = -1;

    pipe(piper2);

    close(piper2[0]);

    printf("piper2[0]: %d\n", piper2[0]);
    printf("piper2[1]: %d\n", piper2[1]);

    return 0;
}
