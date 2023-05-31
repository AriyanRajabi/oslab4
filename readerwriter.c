#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

#define NREADER 3
#define NWRITER 2

#define MUTEX_INDEX 0
#define WRT_INDEX 1


// the mutex is a semaphore index that is used to synchronize changes to readcount
// wrt is a semaphore index that is used to prevent writers from writing while readers are reading
// Declare  readcount
int readcount;

void reader(void)
{
    while (1)
    {
        
        // Critical section starts, reader wants to read, acquire mutex
        sem_acquire(MUTEX_INDEX);
        readcount++; // increment the number of readers
        // If this is the first reader, lock the write semaphore
        if (readcount == 1){
            sem_acquire(WRT_INDEX);
        }

        // Reader done entering, release mutex
        sem_release(MUTEX_INDEX);

        // Start reading
        printf(1, "reader %d is reading\n", getpid());

        // Reader wants to leave
        sem_acquire(MUTEX_INDEX);
        readcount--; // decrement the number of readers


        // If this was the last reader, unlock the write semaphore
        if (readcount == 0){
            sem_release(WRT_INDEX);
        }
            
        // Reader done leaving, release mutex
        sem_release(MUTEX_INDEX);

    }
}

void writer(void)
{
    while (1)
    {
        // Writer wants to write, lock the write semaphore
        sem_acquire(WRT_INDEX);

        // Start writing
        printf(1, "writer %d is writing\n", getpid());

        // Writer done writing, release the write semaphore
        sem_release(WRT_INDEX);
    }
}

int main(void)
{
    int i;

    // Initialize the semaphores and readcount
    sem_init(MUTEX_INDEX, 1);
    sem_init(WRT_INDEX, 1);
    readcount = 0;

    // Create writer processes
    for (i = 0; i < NWRITER; i++)
        if (fork() == 0)
            writer();

    // Create reader processes
    for (i = 0; i < NREADER; i++){
        int pid = fork();
        if (pid == 0){
            reader();
        }
            
    }
        

    

    // Wait for all processes to finish
    for (i = 0; i < NREADER + NWRITER; i++)
        wait();

    exit();
}
