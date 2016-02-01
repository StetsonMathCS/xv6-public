typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

struct capture_stats {
    int tick_count;
    int proc_exit_count;
    int proc_tick_count;
};

enum scheduler_types { SCHED_ROUND_ROBIN, SCHED_FIFO };

