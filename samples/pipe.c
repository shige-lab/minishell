#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int sts;

char *cmd1[] = {"cat", "pipe.c", NULL};
char *cmd2[] = {"head", NULL};
char *cmd3[] = {"grep", "char", NULL};
char **cmds[] = {cmd1, cmd2, cmd3};
int cmd_n = 3;

void recursive(int i) {
    pid_t pid;
    int fd[2];

    if (i == 0) {
        execvp(cmds[i][0], cmds[i]);
        return;
    }

    pipe(fd);
    pid = fork();
    
    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        recursive(i - 1);
    } 
    else {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execvp(cmds[i][0], cmds[i]);
    }
}

int main(int argc, char **argv, char **arge) {
    pid_t pid;
    pid = fork();

    if (pid == 0)
        recursive(cmd_n - 1);
    waitpid(pid, &sts, 0);
}
