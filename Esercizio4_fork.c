#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int primo(int n) {
    if (n < 2) {
        return 0;
    }

    for (int i = 2; i <= n / i; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Errore nella creazione del processo.\n");
        return 1;
    }

    if (pid == 0) {
        // Codice eseguito dal figlio
        int n;

        printf("[Figlio] Inserisci un numero intero: ");
        fflush(stdout);

        if (scanf("%d", &n) != 1) {
            printf("[Figlio] Input non valido.\n");
            exit(4);
        }

        if (primo(n)) {
            exit(1);
        } else if (n % 2 == 0) {
            exit(2);
        } else {
            exit(3);
        }

    } else {
        // Codice eseguito dal padre
        int status;
        pid_t child_pid = wait(&status);

        if (child_pid == -1) {
            perror("wait");
            return 1;
        }

        if (WIFEXITED(status)) {
            int codice = WEXITSTATUS(status);

            printf("[Padre] Il figlio %d e' terminato "
                   "con codice %d.\n", child_pid, codice);

            switch (codice) {
                case 1:
                    printf("[Padre] Il numero e' primo.\n");
                    break;

                case 2:
                    printf("[Padre] Il numero e' pari "
                           "ma non primo.\n");
                    break;

                case 3:
                    printf("[Padre] Il numero e' dispari "
                           "ma non primo.\n");
                    break;

                case 4:
                    printf("[Padre] Non e' stato inserito "
                           "un numero valido.\n");
                    break;

                default:
                    printf("[Padre] Codice sconosciuto.\n");
            }

        } else {
            printf("[Padre] Il figlio e' terminato "
                   "in modo anomalo.\n");
        }
    }

    return 0;
}