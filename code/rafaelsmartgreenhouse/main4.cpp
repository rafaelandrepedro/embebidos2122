#include <iostream>
#include <pthread.h>
#include <sched.h>


void* a(void* i){
	
	while(1){
		printf("%d\n",*(int*)i);
	}
}


int main()
{	
	int thread_policy;
	pthread_t thread_id[32];
	int arg[32];
	
	pthread_attr_t thread_attr;
	struct sched_param thread_param;
	
	pthread_attr_init (&thread_attr);
	pthread_attr_getschedpolicy (&thread_attr, &thread_policy);
	pthread_attr_getschedparam (&thread_attr, &thread_param);
	
	for(int i=31;i>-1;i--){
	arg[i]=i;
	thread_param.sched_priority = i;
	
	pthread_attr_setschedparam (&thread_attr, &thread_param);
	pthread_attr_setinheritsched (&thread_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy (&thread_attr, SCHED_RR);
	

	
	printf("priority - %d\n",thread_param.sched_priority);
	
	
	pthread_create(&thread_id[i], &thread_attr, a,(void*)&arg[i]);
	}
	

    while(1){
    }
    return 0;
}
