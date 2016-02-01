#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    printf(1, "sched,rep,ticks,procexits,procticks\n");
    int sched_type;
    for(sched_type = SCHED_ROUND_ROBIN; sched_type <= SCHED_ROUND_ROBIN; sched_type++)
    {
        int pid;
        int rep;
        for(rep = 1; rep <= 30; rep++)
        {
            start_capture();
            // fork 1: child does math computations
            pid = fork();
            if(pid == 0)
            {
                double r;
                int j;
                for(j = 0; j < 100; j++)
                {
                    int i;
                    r = 1.0;
                    for(i = 0; i < 100; i++)
                    {
                        r = r + r / i;
                    }
                }
                exit();
            }

            // fork 2: child does math computations
            pid = fork();
            if(pid == 0)
            {
                double r;
                int j;
                for(j = 0; j < 100; j++)
                {
                    int i;
                    r = 1.0;
                    for(i = 0; i < 100; i++)
                    {
                        r = r + r * i;
                    }
                }
                exit();
            }

            // fork 3: child does some file opening/closing
            pid = fork();
            if(pid == 0)
            {
                int j;
                for(j = 0; j < 100; j++)
                {
                    int fd = open("benchmark", O_RDONLY);
                    close(fd);
                }
                exit();
            }

            wait(); // wait on a child
            wait(); // wait on other child
            wait(); // wait on other child

            struct capture_stats cs;
            stop_capture(&cs);
            char *sched_name;
            if(sched_type == SCHED_ROUND_ROBIN)
                sched_name = "rr";
            printf(1, "%s,%d,%d,%d,%d\n",
                    sched_name, rep, cs.tick_count,
                    cs.proc_exit_count, cs.proc_tick_count);
        }
    }
    exit();
}

