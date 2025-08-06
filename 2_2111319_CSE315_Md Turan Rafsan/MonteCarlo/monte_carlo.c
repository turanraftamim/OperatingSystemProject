#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

long long total_points_under = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


struct ThreadArgs {
    double start_x;
    double end_x;
    double max_y;
    long long points;
};


double calculate_function(double x) {
    return 4.0 * sqrt(1.0 - x * x);
}


void* thread_work(void* arg) {
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    long long local_points_under = 0;


    for (long long i = 0; i < args->points; i++) {

        double x = args->start_x + (rand() / (double)RAND_MAX) * (args->end_x - args->start_x);
        double y = (rand() / (double)RAND_MAX) * args->max_y;

        if (y <= calculate_function(x)) {
            local_points_under++;
        }
    }

 
    pthread_mutex_lock(&mutex);
    total_points_under += local_points_under;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: %s <start_x> <end_x> <max_y> <total_points>\n", argv[0]);
        printf("Example: %s 0 1 4 1000000\n", argv[0]);
        return 1;
    }

    double start_x = atof(argv[1]);
    double end_x = atof(argv[2]);
    double max_y = atof(argv[3]);
    long long total_points = atoll(argv[4]);


    srand(time(NULL));


    const int NUM_THREADS = 8;
    pthread_t threads[NUM_THREADS];
    struct ThreadArgs thread_args[NUM_THREADS];
    long long points_per_thread = total_points / NUM_THREADS;

    printf("Starting %d threads...\n", NUM_THREADS);


    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].start_x = start_x;
        thread_args[i].end_x = end_x;
        thread_args[i].max_y = max_y;
        thread_args[i].points = points_per_thread;

        if (pthread_create(&threads[i], NULL, thread_work, &thread_args[i]) != 0) {
            printf("Error creating thread %d\n", i);
            return 1;
        }
    }


    printf("Waiting for threads to finish...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }


    double rectangle_area = (end_x - start_x) * max_y;
    double ratio = (double)total_points_under / (points_per_thread * NUM_THREADS);
    double estimated_area = rectangle_area * ratio;

    printf("Calculation complete!\n");
    printf("Total points under curve: %lld\n", total_points_under);
    printf("Estimated area: %.9f\n", estimated_area);

    pthread_mutex_destroy(&mutex);

    return 0;
} 