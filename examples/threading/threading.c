#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    // Wait for the specified time before attempting to obtain the mutex
    usleep(thread_func_args->wait_to_obtain_ms * 1000);

    // Attempt to obtain mutex
    int result = pthread_mutex_lock(thread_func_args->mutex);
    if (result != 0)
    {
        ERROR_LOG("Failed to obtain mutex, error code: %d", result);
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }

    // Mutex obtained, now wait for the specified time before releasing
    DEBUG_LOG("Mutex obtained, waiting for %d ms before releasing", thread_func_args->wait_to_release_ms);
    usleep(thread_func_args->wait_to_release_ms * 1000);

    // Release the mutex
    result = pthread_mutex_unlock(thread_func_args->mutex);
    if (result != 0)
    {
        ERROR_LOG("Failed to release mutex, error code: %d", result);
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }

    // Mark thread as Successful
    DEBUG_LOG("Mutex released successfully");
    thread_func_args->thread_complete_success = true;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    struct thread_data *thread_data = malloc(sizeof(struct thread_data));
    if (thread_data == NULL) 
    {
        ERROR_LOG("Failed to allocate memory for thread_data");
        return false;
    }

    // Initialize thread_data fields
    thread_data->mutex = mutex;
    thread_data->wait_to_obtain_ms = wait_to_obtain_ms;
    thread_data->wait_to_release_ms = wait_to_release_ms;
    thread_data->thread_complete_success = false;

    int result = pthread_create(thread, NULL, threadfunc, (void*)thread_data);
    if (result != 0)
    {
        ERROR_LOG("Failed to create thread, error code: %d", result);
        free(thread_data);
        return false;
    }

    DEBUG_LOG("Thread created successfully");
    return true;
}

