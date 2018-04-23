#pragma once
#include "linked_list.h"
#include "disastrOS_pcb.h"
#include "disastrOS_semaphore.h"


struct SemDescriptorPtr;

typedef struct SemDescriptor{
  ListItem list;
  PCB* pcb;
  Semaphore* semaphore;
  int fd;
  struct SemDescriptorPtr* ptr; // pointer to the entry in the resource list
  struct SemDescriptorPtr* ptr_wait; // puntatore al descrittore del semaforo da usare nelle code di wait, se non lo posizonassimo qui dovremmo allocarlo e deallocarlo ad ogni wait e post
} SemDescriptor;

typedef struct SemDescriptorPtr{
  ListItem list;
  ListItem DescriptorPtr;
  SemDescriptor* descriptor;
} SemDescriptorPtr;

void SemDescriptor_init();
SemDescriptor* SemDescriptor_alloc(int fd, Semaphore* res, PCB* pcb);
int SemDescriptor_free(SemDescriptor* d);
SemDescriptor*  SemDescriptorList_byFd(ListHead* l, int fd);
void SemDescriptorList_print(ListHead* l);

SemDescriptorPtr* SemDescriptorPtr_alloc(SemDescriptor* descriptor);
int SemDescriptorPtr_free(SemDescriptorPtr* d);
void SemDescriptorPtrList_print(ListHead* l);

//funzione per cercare un semaforo tra quelli gia aperti (usata nella sem_open)
SemDescriptor* check_id(ListHead* l,int key);
