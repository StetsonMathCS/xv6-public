#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define DEBUG 0

void workload(int i)
{
    if(i == 0)
    {
#if DEBUG
        printf(1, "short io pid = %d\n", getpid());
#endif
        char buf[8192];
        int j;
        for(j = 0; j < 100; j++)
        {
            int fd = open("benchmark", O_RDONLY);
            read(fd, buf, 2000);
            close(fd);
        }
    }
    else if(i == 1)
    {
#if DEBUG
        printf(1, "add pid = %d\n", getpid());
#endif
        double r;
        int j;
        for(j = 0; j < 10000; j++)
        {
            int i;
            r = 0.0;
            for(i = 0; i < 100; i++)
            {
                r = r + r / i;
            }
        }
    }
    else if(i == 2)
    {
#if DEBUG
        printf(1, "mult pid = %d\n", getpid());
#endif
        double r;
        int j;
        for(j = 0; j < 20000; j++)
        {
            int i;
            r = 1.0;
            for(i = 0; i < 100; i++)
            {
                r = r + r * i;
            }
        }
    }
    else if(i == 3)
    {
#if DEBUG
        printf(1, "add and io pid = %d\n", getpid());
#endif
        double r;
        int j;
        char buf[8192];
        for(j = 0; j < 1000; j++)
        {
            int fd = open("benchmark", O_RDONLY);
            int i;
            r = 1.0;
            for(i = 0; i < 1000; i++)
            {
                r = r + r * i;
            }
            read(fd, buf, 2000);
            close(fd);
        }
    }
    else if(i == 4)
    {
#if DEBUG
        printf(1, "add and io pid = %d\n", getpid());
#endif
        double r;
        int j;
        char buf[8192];
        for(j = 0; j < 1000; j++)
        {
            int fd = open("benchmark", O_RDONLY);
            int i;
            r = 1.0;
            for(i = 0; i < 50; i++)
            {
                r = r + r * i;
            }
            read(fd, buf, 2000);
            close(fd);
        }
    }
    else if(i == 5)
    {
#if DEBUG
        printf(1, "sleeper pid = %d\n", getpid());
#endif
        int i, j;
        double r;
        for(i = 0; i < 100; i++) {
            r = 1.0;
            for(j = 0; j < 100; j++)
            {
                r = r + r * j;
            }
            sleep(10);
        }
    }
}

int main(int argc, char *argv[])
{
    printf(1, "sched,rep,ticks,procexits,procticks,procfirstrun\n");
    int sched_type;
    for(sched_type = SCHED_ROUND_ROBIN; sched_type <= SCHED_MLFQ; sched_type++)
    {
        switch_scheduler(sched_type);
        int pid;
        int rep;
        for(rep = 1; rep <= 30; rep++)
        {
            start_capture();

            int forki;
            for(forki = 0; forki < 10; forki++) {
                pid = fork();
                if(pid == 0)
                {
                    workload((rep + forki) % 6);
                    exit();
                }
                sleep(5);
            }

            while(wait() > 0);

            struct capture_stats cs;
            stop_capture(&cs);
            char *sched_name;
            if(sched_type == SCHED_ROUND_ROBIN)
                sched_name = "rr";
            else if(sched_type == SCHED_FIFO)
                sched_name = "fifo";
            else if(sched_type == SCHED_LIFO)
                sched_name = "lifo";
            else if(sched_type == SCHED_MLFQ)
                sched_name = "mlfq";
            printf(1, "%s,%d,%d,%d,%d,%d\n",
                    sched_name, rep, cs.tick_count,
                    cs.proc_exit_count, cs.proc_tick_count,
                    cs.proc_firstrun_count);
        }
    }
    exit();
}

