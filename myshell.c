#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define DELIM " \t\r\n"

// 内置命令
int builtin_cmd(char **args)
{
    if (strcmp(args[0], "exit") == 0)
    {
        printf("退出 myshell\n");
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            chdir(getenv("HOME"));
        }
        else
        {
            if (chdir(args[1]) == -1)
            {
                perror("cd");
            }
        }
        return 1;
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        char buf[256];
        getcwd(buf, sizeof(buf));
        printf("%s\n", buf);
        return 1;
    }
    return 0;
}

// 分割输入字符串
void parse(char *input, char **args)
{
    char *token = strtok(input, DELIM);
    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, DELIM);
    }
    args[i] = NULL;
}

// 执行命令
void run_cmd(char **args)
{
    if (args[0] == NULL) return;
    if (builtin_cmd(args)) return;

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
    }
}

int main()
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    printf("===== 简易Shell模拟器启动 =====\n");
    while (1)
    {
        printf("myshell$ ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        parse(input, args);
        run_cmd(args);
    }
    return 0;
}
