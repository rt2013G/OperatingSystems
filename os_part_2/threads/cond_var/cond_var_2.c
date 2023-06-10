/*
 Dato un ponte con una capacità massima max che esprime il numero massimo di veicoli
 che possono transitare contemporaneamente su di esso. Considerata l'ipotesi
 che sul ponte possono transitare solo 2 tipi di veicoli: automobili e camion, e che
 il ponte è talmeste stretto che il transito di un camion in una particolare direzione
 impedisce l'accesso al ponte di altri veicoli in direzione opposte.

 Realizzare una politica di sincronizzazione delle entrate e delle uscite dal ponte
 che tenga conto delle specifiche date e che favorisca le automobili rispetto
 ai camion nell'accesso al ponte.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int max = 3, sx = 0, dx = 1;
struct timespec wait_time = {0, 250000000};

struct {
    pthread_mutex_t lock;
    int car[2];
    int truck[2];
    pthread_cond_t car_q[2];
    pthread_cond_t truck_q[2];
} ponte =   {PTHREAD_MUTEX_INITIALIZER, {0, 0}, {0, 0}, 
            {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER},
            {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER}};

void *car_fun(void *arg);
void *truck_fun(void *arg);
void broadcast_all();
int total_count();
int other_direction(int direction);

int main(int argc, char **argv)
{
    if(argc != 5) {
        fprintf(stderr, "Usage: %s <car sx> <car dx> <truck sx> <truck dx>\n", argv[0]);
        exit(0);
    }
    int n_car_sx = atoi(argv[1]);
    int n_car_dx = atoi(argv[2]);
    int n_truck_sx = atoi(argv[3]);
    int n_truck_dx = atoi(argv[4]);
    pthread_t car_sx[n_car_sx];
    pthread_t car_dx[n_car_dx];
    pthread_t truck_sx[n_truck_sx];
    pthread_t truck_dx[n_truck_dx];
    for(int i = 0; i < n_car_sx; i++) {
        pthread_create(&car_sx[i], NULL, car_fun, (void *)&sx);
    }
    for(int i = 0; i < n_car_dx; i++) {
        pthread_create(&car_dx[i], NULL, car_fun, (void *)&dx);
    }
    for(int i = 0; i < n_truck_sx; i++) {
        pthread_create(&truck_sx[i], NULL, truck_fun, (void *)&sx);
    }
    for(int i = 0; i < n_truck_dx; i++) {
        pthread_create(&truck_dx[i], NULL, truck_fun, (void *)&dx);
    }
    for(int i = 0; i < n_car_sx; i++) {
        pthread_join(car_sx[i], NULL);
    }
    for(int i = 0; i < n_car_dx; i++) {
        pthread_join(car_dx[i], NULL);
    }
    for(int i = 0; i < n_truck_sx; i++) {
        pthread_join(truck_sx[i], NULL);
    }
    for(int i = 0; i < n_truck_dx; i++) {
        pthread_join(truck_dx[i], NULL);
    }
    return 0;
}

void *car_fun(void *arg)
{
    int *direction = (int *) arg;
    pthread_mutex_lock(&ponte.lock);
    while(ponte.truck[other_direction(*direction)] > 0 || total_count() >= max) {
        pthread_cond_wait(&ponte.car_q[*direction], &ponte.lock);
    }
    ponte.car[*direction]++;
    pthread_mutex_unlock(&ponte.lock);

    printf("Car n. %u, crossing in direction %d. Total number of vehicles: %d\n", (unsigned) pthread_self(), *direction, total_count());
    nanosleep(&wait_time, NULL); //attraverso

    pthread_mutex_lock(&ponte.lock);
    ponte.car[*direction]--;
    pthread_mutex_unlock(&ponte.lock);
    broadcast_all();
    pthread_exit(NULL);
}

void *truck_fun(void *arg)
{
    int *direction = (int *) arg;
    pthread_mutex_lock(&ponte.lock);
    while(ponte.truck[other_direction(*direction)] > 0 || ponte.car[other_direction(*direction)] > 0 
                                                        || total_count() >= max) {
        pthread_cond_wait(&ponte.truck_q[*direction], &ponte.lock);
    }
    ponte.truck[*direction]++;
    pthread_mutex_unlock(&ponte.lock);

    printf("Truck n. %u, crossing in direction %d. Total number of vehicles: %d\n", (unsigned) pthread_self(), *direction, total_count());
    nanosleep(&wait_time, NULL); //attraverso

    pthread_mutex_lock(&ponte.lock);
    ponte.truck[*direction]--;
    pthread_mutex_unlock(&ponte.lock);
    broadcast_all();
    pthread_exit(NULL);
}

int total_count()
{
    return ponte.car[sx] + ponte.car[dx] + ponte.truck[sx] + ponte.truck[dx];
}

void broadcast_all()
{
    pthread_cond_broadcast(&ponte.car_q[sx]);
    pthread_cond_broadcast(&ponte.car_q[dx]);
    pthread_cond_broadcast(&ponte.truck_q[sx]);
    pthread_cond_broadcast(&ponte.truck_q[dx]);
}

int other_direction(int direction)
{
    if(direction == sx) return dx;
    else return sx;
}
