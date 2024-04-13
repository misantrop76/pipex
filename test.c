#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s cmd1 cmd2 ... cmdN\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_cmds = argc - 1;
    int pipes[num_cmds - 1][2]; // Tableau de pipes

    // Création des pipes
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Boucle d'exécution des commandes
    for (int i = 0; i < num_cmds; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Processus enfant
            // Redirection de l'entrée standard si ce n'est pas la première commande
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][1]);
            }

            // Redirection de la sortie standard si ce n'est pas la dernière commande
            if (i < num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][0]);
            }

            // Fermeture des descripteurs de fichiers inutilisés
            for (int j = 0; j < num_cmds - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Exécution de la commande
            execlp(argv[i + 1], argv[i + 1], NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        } else { // Processus parent
            // Fermeture du descripteur de fichier d'entrée si ce n'est pas la première commande
            if (i > 0) {
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }

            // Attente de la fin du processus enfant
            wait(NULL);
        }
    }

    return 0;
}