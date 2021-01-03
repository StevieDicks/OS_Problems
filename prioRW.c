/* 
Stephen Dickson 2020
Intro to Operating Systems
*/
// Problem statement:
/* 
Write a multi-threaded C program that gives readers priority 
over writers concerning a shared (global) variable. 
Essentially, if any readers are waiting, 
then they have priority over writer threads -- 
writers can only write when there are no readers. 
This program should adhere to the following constraints:

    Multiple readers/writers must be supported (5 of each is fine)
    Readers must read the shared variable X number of times
    Writers must write the shared variable X number of times
    Readers must print:
        The value read
        The number of readers present when value is read
    Writers must print:
        The written value
        The number of readers present were when value is written (should be 0)
    Before a reader/writer attempts to access the shared variable 
    it should wait some random amount of time
        Note: This will help ensure that reads and writes do not occur all at once
    Use pthreads, mutexes, and condition variables to synchronize access 
    to the shared variable
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_READERS 5
#define NUM_WRITERS 5

int read_buff[NUM_READERS]; // shared reader buffer
int add_r = 0;  // place to add next element
int rem_r = 0;  // place to remove next element
int num_r = 0;  // number of readers total in buffer
int write_buff[NUM_WRITERS];
int add_w = 0;  // same as above but for writers
int rem_w = 0;
int num_w = 0;

int shared_variable = NULL;
pthread_mutex_t m_shared = PTHREAD_MUTEX_INITIALIZER; //mutex for shared variable
/* 
We need to enter m_shared first, then m_read or m_write second 
*/
pthread_mutex_t m_read = PTHREAD_MUTEX_INITIALIZER; //mutex for read buffer
pthread_mutex_t m_write = PTHREAD_MUTEX_INITIALIZER; //mutex for write buffer
pthread_cond_t c_read = PTHREAD_COND_INITIALIZER; //reader waits on this cond var
pthread_cond_t c_write = PTHREAD_COND_INITIALIZER; //writer waits on this cv

void *writer (void *param);
void *reader (void *param);

int main(int argc, char *argc[]) {

    pthread_t t_read[NUM_READERS], t_write[NUM_WRITERS]; //create threads for r/w-ers
    int i;
    int trNum[NUM_READERS];
    int twNum[NUM_WRITERS];

    /* create the writers; may be any number, in general */
    for (i = 0; i < NUM_WRITERS; i++) {
        if(pthread_create(&t_write[i], NULL, writer, NULL) != 0) {
            fprintf(stderr, "Unable to create writer thread num %d\n", i);
            exit(1);
        }
    }

    /* create the threads; may be any number, in general */
    for (i = 0; i < NUM_READERS; i++) {
        if(pthread_create(&t_read[i], NULL, reader, NULL) != 0) {
            fprintf(stderr, "Unable to create reader thread num %d\n", i);
            exit(1);
        }
    }

    /* wait for created thread to exit */
    for (i = 0; i < NUM_READERS; i++) {
        pthread_join(t_read[i], NULL);
    }

    /* wait for created thread to exit */
    for (i = 0; i < NUM_WRITERS; i++) {
        pthread_join(t_write[i], NULL);
    }
    printf("Parent quitting\n");

    return 0;
}

/* Read value; Note the consumer never terminates */
void *reader(void *param) {

	int i;

	while(1) {
        pthread_mutex_lock (&m_shared); // lock shared variable (no read lock necessary)
            if (num_r < 0) {
                exit(1);
            } /* underflow */

            while (num_r == 0) {  /* block if buffer empty */
                pthread_cond_wait (&c_read, &m_read);
            }

            /* if executing here, buffer not empty so remove element */
            i = buffer[rem_r];
            rem_r = (rem_r+1) % BUF_SIZE;
            num_r--;
        pthread_mutex_unlock (&m_shared);

		pthread_cond_signal (&c_read);
		printf ("Consume value %d\n", i);  fflush(stdout);

	}
	return 0;
}

/* Write value(s) */
void *writer(void *param) {

    int i;
    for (i=1; i<=NUM_WRITERS; i++) {

        pthread_mutex_lock (&m_shared);
        /* Insert into write buffer */
        pthread_mutex_lock (&m_write);
            if (num_w > NUM_WRITERS) {
                exit(1); // overflow
            }

            while (num_w == NUM_WRITERS) {
                pthread_cond_wait (&c_write, &m_write);
            }

            /* if executing here, buffer not full so add element */
            write_buff[add_w] = i;
            add_w = add_w+1 % NUM_WRITERS;
            num_w++;
        pthread_mutex_unlock (&m_write);

        pthread_cond_signal (&c_write);
        printf ("writer: inserted %d\n", i);
    }
    return 0;
}