#include "main.hpp"

using namespace std;
Room *room;
sem_t cleaning_sem, guest_sem, room_sem;
int X, Y, N;

// Creates the guest and cleaning staff threads. Initialize semaphores
// which will control access to rooms for guests and cleaning threads.
int main()
{
    srand(time(NULL));
    cout << "Enter the number of cleaning staff: ";
    cin >> X;
    cout << "Enter the number of guests: ";
    cin >> Y;
    cout << "Enter the number of rooms: ";
    cin >> N;
    // initialize the rooms
    room = new Room[N];

    for (int i = 0; i < N; i++)
    {
        room[i].room_num = i + 1;
        room[i].occupancy = 0;
        room[i].current_guest = 0;
        room[i].is_free = 1;
        room[i].room_clean = 1;
        room[i].occupancy_time = 0;
    }

    // initialize the semaphores
    sem_init(&cleaning_sem, 0, 0);
    sem_init(&guest_sem, 0, N);
    sem_init(&room_sem, 0, 0);

    // create and initialize and then join the threads
    vector<pthread_t> guest_t(Y);
    vector<pthread_t> cleaning_t(X);

    for (int i = 0; i < Y; i++)
    {
        arguments *a = new arguments;
        a->index = i + 1;
        a->priority = Y - i;
        pthread_create(&guest_t[i], NULL, guest_thread, a);
    }
    for (int i = 0; i < X; i++)
    {
        int *a = new int;
        *a = i + 1;
        pthread_create(&cleaning_t[i], NULL, cleaning_thread, a);
    }

    for (int i = 0; i < Y; i++)
    {
        pthread_join(guest_t[i], NULL);
    }
    for (int i = 0; i < X; i++)
    {
        pthread_join(cleaning_t[i], NULL);
    }

    // destroy the semaphores
    sem_destroy(&cleaning_sem);
    sem_destroy(&guest_sem);
    sem_destroy(&room_sem);

    return 0;
}
