#include <pthread.h>
#include <stdio.h>

void *thread_function()
{
        int i;
		
		//Write code to allocate integer from the heap
		
        for (i = 0; i < 100; i++) {
                // Increment that integer
        }
        
		// Return a pointer to that integer
}

int main()
{
        pthread_t tid;
        void *thread_return_value;
        pthread_create(&tid,NULL,thread_function,NULL);
        pthread_join(tid, &thread_return_value);
		
        // Print the value of integer by extracting it from thread_return_value
		
        return 0;
}


