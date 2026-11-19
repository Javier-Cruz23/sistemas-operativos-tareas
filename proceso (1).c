#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    setvbuf(stdout, NULL, _IOLBF, 0); /* salida por linea para que no se mezclen */
    pid_t pid = fork();
    int i;

    if (pid < 0) {
        perror("Error al crear el proceso");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        /* Proceso hijo: imprime del 10000 al 1 */
        for (i = 10000; i >= 1; i--) {
            printf("Hijo: %d\n", i);
        }
        fflush(stdout);
        exit(EXIT_SUCCESS);
    } else {
        /* Proceso padre: imprime del 1 al 10000 */
        for (i = 1; i <= 10000; i++) {
            printf("Padre: %d\n", i);
        }
        fflush(stdout);
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
