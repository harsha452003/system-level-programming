#ifndef MAIN_HPP
#define MAIN_HPP
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <queue>
#include <mutex>
#include <unistd.h>
#include <condition_variable>
#include <cmath>
#include <ctime>
#include <string>
#include <queue>
#include <set>


typedef struct{
    int room_num;
    int occupancy;
    int current_guest;
    int is_free;
    int room_clean;
    int occupancy_time;
} Room;

typedef struct{
    int index;
    int priority;
} arguments;


extern Room* room;
extern sem_t cleaning_sem,guest_sem,room_sem;
extern int X,Y,N;

void* guest_thread(void* arg);

void* cleaning_thread(void* arg);



#endif