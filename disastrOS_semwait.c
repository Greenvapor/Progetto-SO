#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){

    int fd = running->syscall_args[0];

    //prendo la lista dei semafori aperti dal processo
    ListHead sem_aperti = running->sem_descriptors;

    //cerco tra i semafori aperti quello sul quale devo fare la wait
    SemDescriptor* sem_desc = (SemDescriptor*) SemDescriptorList_byFd(&sem_aperti, fd);

    if( !sem_desc ){
        running->syscall_retvalue = DSOS_ESEMNOTOPENEDBYME;
        return;
    }

    //controllo se il semaforo è ancora aperto
    Semaphore* semaforo = sem_desc->semaphore;

    if ( !semaforo ){
        //entro qui solo se un altro processo ha chiuso il semaforo e nel running è rimasto il puntatore al descrittore
        running->syscall_retvalue = DSOS_ESEMNOTOPENED;
        return;
    }

    //prendo il puntatore per le code di wait che ho creato nella open dalla struttura
    SemDescriptorPtr * puntatore_wait = sem_desc->ptr_wait;

    //classica wait, se il count è <= 0 il processo verra messo in waiting list e sblocchera un processo nella ready queue
    if (semaforo -> count <= 0){
        List_insert(&(sem_desc->semaphore->waiting_descriptors), semaforo->waiting_descriptors.last, (ListItem*) puntatore_wait);
        running->status = Waiting;
        List_insert(&waiting_list, waiting_list.last, (ListItem*) running);
        running=(PCB*) List_detach(&ready_list, ready_list.first);

    }

    semaforo -> count-=1;

    running -> syscall_retvalue = 0;
    return;

}
