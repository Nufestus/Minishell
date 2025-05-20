#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

int g_var = 0;

void handler(int signum)
{
    write(1, "recieved\n",  10);
    ioctl(0, TIOCSTI, "\n");
    g_var = 130;
}

int main(void)
{
    signal(SIGINT, handler);
    while (1)
    {
        char *line = readline("test: ");
        if (g_var == 130)
        {
            // rl_on_new_line();
            // rl_redisplay();
            break ;
        }
    }
}