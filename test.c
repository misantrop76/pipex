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
            // Gestion des redirections des pipes
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO); // Redirection de l'entrée standard
                close(pipes[i - 1][1]); // Fermeture de l'extrémité d'écriture du pipe précédent
            }

            if (i < num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO); // Redirection de la sortie standard
                close(pipes[i][0]); // Fermeture de l'extrémité de lecture du pipe actuel
            }

            // Exécution de la commande
            execlp(argv[i + 1], argv[i + 1], NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }

    // Fermeture de tous les descripteurs de fichiers inutilisés par le processus parent
    for (int i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]); // Fermeture de l'extrémité de lecture du pipe
        close(pipes[i][1]); // Fermeture de l'extrémité d'écriture du pipe
    }

    // Attente de la fin de tous les processus enfants
    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }

    return 0;
}
