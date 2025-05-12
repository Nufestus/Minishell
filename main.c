#include <stdio.h>
#include <unistd.h>

int main(int ac, char **av, char **env)
{
    execve("./ls", av, env);
    perror("aaaaaaaaa");
}