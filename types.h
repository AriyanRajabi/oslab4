

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef uint pde_t;

struct semaphore
{
    int value;
    struct spinlock* lock;
    struct proc *queue[64]; // the processes that are waiting for the semaphore
    int count;                 // the number of processes in the queue
};
