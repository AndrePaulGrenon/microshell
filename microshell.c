#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>

int ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

int EndofCommand(char **argv)
{
    int i;

    i = 0;
    while (argv[i])
    {
        if (!argv[i + 1] && strncmp(argv[i], ";", 2))
            return (i);
        if (!strncmp(argv[i], ";", ft_strlen(argv[i])))
            return (i - 1);
        if (!strncmp(argv[i], "|", ft_strlen(argv[i])))
            return (i - 1);
        i++;
    }
    return (i - 1);
}

bool    EndsWithPipe(char **argv)
{
    if (argv[EndofCommand(argv) + 1] && !strncmp(argv[EndofCommand(argv) + 1], "|", ft_strlen(argv[EndofCommand(argv) + 1])))
        return (true);
    return false;
}

void    ErrorStream(char *str, char *failedExec, int quit)
{
    write(2, str, ft_strlen(str));
    if (failedExec)
        write(2, failedExec, ft_strlen(failedExec));
    write(2, "\n", 1);
    if (quit)
        exit(1);
}

void    ExecCD(char **argv)
{
    if (!argv[1] || (argv[2] && strncmp(argv[2], ";", 2)))
        ErrorStream("error: cd: bad arguments", NULL, 0);
    if (chdir(argv[1]))
        ErrorStream("error: cd: cannot change directory to ", argv[1], 0);
}

int main(int argc, char **argv, char **env)
{
    int i;
    int fds[2];
    int pid;

    if (argc < 2)
        return (0);
    i = 1;
    while (argv[i])
    {
        if (!strncmp(argv[i], ";",ft_strlen(argv[i])) || !strncmp(argv[i], "|", ft_strlen(argv[i])))
        {
            i++;
            continue;
        }
        if (!strncmp(argv[i], "cd", 3))
        {
            ExecCD(&argv[i]);
            i = i + EndofCommand(&argv[i]) + 1;
            continue;
        }
        if (EndsWithPipe(&argv[i]))
        {
            if (pipe(fds) ==  -1)
                ErrorStream("error: fatal", NULL, 1);
        }
        pid = fork();
        if (pid == -1)
            ErrorStream("error: fatal", NULL, 1);
        if (pid == 0)
        {
            if (EndsWithPipe(&argv[i]))
            {
                if (dup2(fds[1], 1) == -1)
                     ErrorStream("error: fatal", NULL, 1);
                if (close(fds[0]) == -1)
                    ErrorStream("error: fatal", NULL, 1);
                if (close(fds[1]) == -1)
                    ErrorStream("error: fatal", NULL, 1);
            }
            argv[i + EndofCommand(&argv[i]) + 1] = NULL;
            if (execve(argv[i], &argv[i], env))
                ErrorStream("error: cannot execute ", argv[i], 0);
            exit(1);
        }
        if (EndsWithPipe(&argv[i]))
        {
            if (dup2(fds[0], STDIN_FILENO) == -1)
              ErrorStream("error: fatal", NULL, 1);
            if (close(fds[0]) == -1)
                ErrorStream("error: fatal", NULL, 1);
            if (close(fds[1]) == -1)
                ErrorStream("error: fatal", NULL, 1);
        }
        if (waitpid(pid, NULL, 0) == -1)
              ErrorStream("error: fatal", NULL, 1);
        i = i + EndofCommand(&argv[i]) + 1;
    }
    return (0);
}