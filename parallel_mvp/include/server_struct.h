
#ifndef _SHM_SERVER_H_
#define _SHM_SERVER_H_ 1

struct server_struct
{
    int64_t dialog_counter;
    int64_t matrix_shmid;
    int64_t vector_shmid;
    int64_t results_shmid;
};

#endif