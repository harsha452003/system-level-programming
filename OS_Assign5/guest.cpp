#include "main.hpp"
using namespace std;

int request_room(int index)
{
    int id = -1;
    for(int i=0; i<N; i++)
    {
        if(room[i].is_free == 0)
        {
            id = i;
            break;
        }
    }
    if(id == -1)
    {
        for(int i=0; i<N; i++)
        {
            if(room[i].current_guest > index) return i;
        }
        return -1;
    }
    else
    {
        return id;
    }
}

void* guest_thread(void* arg){
    arguments* a = (arguments*) arg;
    int index = a->index;
    int priority = a->priority;
    while(1)
    {
        int x = rand()%11 + 10;
        sleep(x);
        printf("Guest %d has arrived with priority %d\n",index,priority);
        int flag =0;
        for(int i=0; i<N; i++)
        {
            if(room[i].occupancy < 2)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            for(int i=0; i<N; i++)
            {
                sem_post(&cleaning_sem);
                sem_wait(&guest_sem);
                room[i].occupancy = 0;
            }
        }
        int value;
        if(sem_getvalue(&guest_sem,&value) == 0 && value < N) sem_wait(&room_sem);
        int room_no = request_room(index);
        int stay_time = rand()%21 + 10;
        if(room_no == -1) continue;
        else{
            room[room_no].is_free = 0;
            room[room_no].occupancy += 1;
            room[room_no].current_guest = index;
            room[room_no].occupancy_time += stay_time;
            room[room_no].room_clean = 0;
        }
        sleep(stay_time);
    }
    pthread_exit(NULL);
}