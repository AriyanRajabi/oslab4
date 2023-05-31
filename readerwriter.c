#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

#define NREADER 3
#define NWRITER 2

// Declare semaphores and readcount
struct semaphore mutex, wrt; // the mutex is a semaphore that is used to synchronize changes to readcount
// wrt is a semaphore that is used to prevent writers from writing while readers are reading
int readcount;

void reader(void)
{
    while (1)
    {
        // Critical section starts, reader wants to read, acquire mutex
        sem_acquire(&mutex);
        readcount++; // increment the number of readers

        // If this is the first reader, lock the write semaphore
        if (readcount == 1)
            sem_acquire(&wrt);

        // Reader done entering, release mutex
        sem_release(&mutex);

        // Start reading
        printf(1, "reader %d is reading\n", getpid());

        // Reader wants to leave
        sem_acquire(&mutex);
        readcount--; // decrement the number of readers

        // If this was the last reader, unlock the write semaphore
        if (readcount == 0)
            sem_release(&wrt);

        // Reader done leaving, release mutex
        sem_release(&mutex);
    }
}

void writer(void)
{
    while (1)
    {
        // Writer wants to write, lock the write semaphore
        sem_acquire(&wrt);

        // Start writing
        printf(1, "writer %d is writing\n", getpid());

        // Writer done writing, release the write semaphore
        sem_release(&wrt);
    }
}

int main(void)
{
    int i;

    // Initialize the semaphores and readcount
    sem_init(&mutex, 1);
    sem_init(&wrt, 1);
    readcount = 0;

    // Create reader processes
    for (i = 0; i < NREADER; i++)
        if (fork() == 0)
            reader();

    // Create writer processes
    for (i = 0; i < NWRITER; i++)
        if (fork() == 0)
            writer();

    // Wait for all processes to finish
    for (i = 0; i < NREADER + NWRITER; i++)
        wait();

    exit();
}
