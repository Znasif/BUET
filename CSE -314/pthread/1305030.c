#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<unistd.h>

#define STD_NO 30

//semaphore to control sleep and wake up

int Online[10],req_B,req_D;
sem_t empty_Online,full_Online,empty_reqB,full_reqB,empty_reqD,full_reqD;

int Duplicate[STD_NO],GivePassword[STD_NO],Password[STD_NO],Official[STD_NO];

pthread_mutex_t lock_on_Online;
pthread_mutex_t lock_on_Duplicate,lock_on_GivePassword,lock_on_Password;

void init_semaphore()
{
	srand(time(NULL));

	sem_init(&empty_Online,0,10);
	sem_init(&full_Online,0,0);

	sem_init(&empty_reqB,0,1);
	sem_init(&full_reqB,0,0);

	sem_init(&empty_reqD,0,1);
	sem_init(&full_reqD,0,0);

	pthread_mutex_init(&lock_on_Duplicate,0);
	pthread_mutex_init(&lock_on_GivePassword,0);
	pthread_mutex_init(&lock_on_Password,0);
	pthread_mutex_init(&lock_on_Online,0);

	for(int i=0;i<STD_NO;i++){ Official[i]=rand()%1234567+6384;}
	for(int i=0;i<STD_NO;i++){ Password[i]=-1;}
}

void * StudentFunc(void * arg)
{
	int sid=(int)arg;
	int pass;

	//printf("This is student thread : %d\n",sid);
	//sleep(1);

	sem_wait(&empty_Online);
	pthread_mutex_lock(&lock_on_Online);

	for(int i=0;i<10;i++){
		if(Online[i]==-1){
			Online[i]=sid;//this stores applications, replace this with queue if possible
			printf("Student %d requested for Online\n",sid);
			break;
		}
	}
	pthread_mutex_unlock(&lock_on_Online);
	sem_post(&full_Online);

	sem_wait(&empty_reqB);

	printf("Student %d requested to B\n",sid);
	req_B=sid;

	sem_post(&full_reqB);

	while(1){
		pthread_mutex_lock(&lock_on_Password);
		pass=Password[sid-1];
		pthread_mutex_unlock(&lock_on_Password);
		if(pass!=-1)break;

		sem_wait(&empty_reqD);
		printf("Student %d requested to B\n",sid);
		req_D=sid;
		sem_post(&full_reqD);

		//sleep(2);
	}
	printf("Student %d received password %d\n",sid,pass);
}

void * TeacherFunc(void * arg)
{
	int tis=(int)arg;
	char tid='A'+tis;
	int a;
	printf("This is teacher thread : %c\n",tid);
	while(1)
	{
		//sleep(1);

		sem_wait(&full_Online);
		pthread_mutex_lock(&lock_on_Online);
		pthread_mutex_lock(&lock_on_Duplicate);

		for(int i=0;i<10;i++){
			if(Online[i]!=-1){
				a=i;
				break;
			}
		}

		Duplicate[Online[a]-1]+=1;
		printf("Teacher %c received request for : %d\n",tid,Online[a]);
		/*for(int t=0;t<STD_NO;t++){
		    printf("%d %d ,",t+1,Duplicate[t]);
		}*/
		printf("\n");
		Online[a]=-1;

		pthread_mutex_unlock(&lock_on_Duplicate);
		pthread_mutex_unlock(&lock_on_Online);
		sem_post(&empty_Online);

        //sleep(1);
	}
}

void * BFunc(void * arg)
{
	int sid;
	//printf("This is teacher thread : B\n");
	while(1)
	{
		//sleep(1);

		sem_wait(&full_reqB);

		sid=req_B;
		printf("B recieved request : %d\n",sid);
		sem_post(&empty_reqB);

		pthread_mutex_lock(&lock_on_Duplicate);
		printf("B checking duplicate for : %d\n",sid);
		if(Duplicate[sid-1]==1){
		    pthread_mutex_lock(&lock_on_GivePassword);
		    GivePassword[sid-1]=1;
		    pthread_mutex_unlock(&lock_on_GivePassword);
		}else printf("Duplicates Found for : %d\n",sid);

		Duplicate[sid-1]=0;

		pthread_mutex_unlock(&lock_on_Duplicate);
	}
}

void * DFunc(void * arg)
{
	//printf("This is teacher thread : D\n");
	while(1)
	{
		sem_wait(&full_reqD);
		int sid=req_D;
		printf("D received request for : %d\n",sid);
		sem_post(&empty_reqD);

		pthread_mutex_lock(&lock_on_GivePassword);
		if(GivePassword[sid-1]){
			pthread_mutex_lock(&lock_on_Password);
			Password[sid-1]=Official[sid-1];
			printf("D sent password to : %d\n",sid);
			pthread_mutex_unlock(&lock_on_Password);
		}else printf("D could not send password to : %d\n",sid);
		GivePassword[sid-1]=0;
		pthread_mutex_unlock(&lock_on_GivePassword);
	}
}

int main(void)
{
	pthread_t Sthreads[35];
	pthread_t Tthreads[3];
	pthread_t Bthreads;
	pthread_t Dthreads;

	init_semaphore();
	for(int t=0;t<10;t++){
		Online[t]=-1;
	}

	for(int t=0;t<35;t++){
		//printf("Main: creating Student thread %d...\n",t);
		//pthread_create(&Sthreads[t],NULL,StudentFunc,(void *)(t%4+1));
		pthread_create(&Sthreads[t],NULL,StudentFunc,(void *)(t%STD_NO+1));
		/*rc=pthread_create(&threads[t],NULL,PrintHello,(void *)&data[t]);
		if(rc){
		    printf("Error! return code from pthread_create() is %d\n",rc);
		    exit(-1);
		}*/
	}
	int a[3]={0,2,4};
	for(int t=0;t<3;t++){
		//printf("Main: creating Teacher thread %c...\n",a[t]+'A');
		pthread_create(&Tthreads[t],NULL,TeacherFunc,(void *)a[t]);
		/*rc=pthread_create(&threads[t],NULL,PrintHello,(void *)&data[t]);
		if(rc){
		    printf("Error! return code from pthread_create() is %d\n",rc);
		    exit(-1);
		}*/
	}
	int p=0;
	pthread_create(&Bthreads,NULL,BFunc,(void *)p);
	pthread_create(&Dthreads,NULL,DFunc,(void *)p);

	while(1);
	return 0;
}