#include "main.hpp"
using namespace std;

void* cleaning_thread(void* arg){
    int index = *(int*)arg;
    printf("Cleaning staff %d has arrived\n",index);
    while(1){
        sem_wait(&cleaning_sem);
        int room_id;
        while(1)
        {
            room_id = rand()%N;
            if(room[room_id].room_clean == 0)
            {
                room[room_id].room_clean = 1;
                break;
            }
            else continue;
        }
        int sleep_time = 2*room[room_id].occupancy_time;
        sleep(sleep_time);
        room[room_id].occupancy_time = 0;
        room[room_id].occupancy = 0;
        sem_post(&guest_sem);
        int ret = 0;
        for(int i=0; i<N; i++)
        {
            if(room[i].room_clean == 0)
            {
                ret = 1;
                break;
            }
        }
        if(ret == 0) sem_post(&room_sem);
    }
    pthread_exit(NULL);
}
// Compare this snippet from cleaning.cpp:
// #include "main.hpp"
// using namespace std;
// void* cleaning_thread(void* arg){}