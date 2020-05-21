#ifndef _QUEUE_LK_H_
#define _QUEUE_LK_H_
#include <omp.h>

struct queue_node_s {
   char* mesg;
   struct queue_node_s* next_p;
};

struct queue_s{
   omp_lock_t lock;
   int enqueued;
   int dequeued;
   struct queue_node_s* front_p;
   struct queue_node_s* tail_p;
};

struct queue_s* Allocate_queue();
void Free_queue(struct queue_s* q_p);
void Print_queue(struct queue_s* q_p);
void Enqueue(struct queue_s* q_p, char* mesg);
int Dequeue(struct queue_s* q_p, char* mesg_p);
char* Dequeue_from(struct queue_s* q_p);

#endif
