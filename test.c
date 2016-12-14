#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>

static pthread_mutex_t cyassl_ctx_mutex = PTHREAD_MUTEX_INITIALIZER;

#define LOCK_CYASSL_CTX() do { \
	pthread_mutex_lock(&cyassl_ctx_mutex); \
} while (0)

#define UNLOCK_CYASSL_CTX() do { \
	pthread_mutex_unlock(&cyassl_ctx_mutex); \
} while (0)



void *threadTcp(void  *ip)
{
    FILE *fstream=NULL;     
    char buff[1024];  
	char buffstr[4096]; 
    int  errno = 0;   
    memset(buff,0,sizeof(buff));   
	memset(buffstr,0,sizeof(buffstr));   
	pthread_t id1,id2;

	FILE *fp; 
	char ch;
	
   
    if(NULL==(fstream=popen("ping -c 5 127.0.0.1","r")))     
    {    
        fprintf(stderr,"execute command failed: %s",strerror(errno));     
        return -1;     
    }    
	int m=0;
    while(NULL!=fgets(buff, sizeof(buff), fstream))    
    {    
       strcpy( buffstr + m,buff);
       m =m+ strlen(buff);
       // printf("%s",buff);  
		
    }   
//	printf("%s",buffstr);   
    LOCK_CYASSL_CTX();

	if((fp=fopen("myfile.txt","a+"))== NULL ) {
		printf("file cannot be opened/n"); 
		exit(1); 
	} 

	fwrite (buffstr,  strlen(buffstr) ,1, fp);
    
	fclose(fp); 

    UNLOCK_CYASSL_CTX();
     printf("ok\n");
	 pthread_exit((void*)0);

}

 
void *th_fun1(void*arg)
{
        int i=10/2;
        printf("%s",arg);
        while(i-->0)
                fputs("new thread1\n",stdout);
        pthread_exit ((void*)1);
}
 
void *th_fun2(void*arg)
{
        int i=10/2;
        printf("%s",arg);
        while(i-->0)
                fputs("new thread 2\n",stdout);
pthread_exit((void*)2);
}
 
int main()
{
        int ret;
		pthread_t dbthread[10];
        pthread_t tid1,tid2;
		int i;
		    for (i = 0; i<1 ; i++)
    {
       if((ret=pthread_create(&dbthread[i],NULL,threadTcp,"new thread 1  created!\n"))==-1)
        {
                perror("pthread_createerror ");
                exit(EXIT_FAILURE);
        }
	}

        void *fret1;
        void *fret2;
				    for (i = 0; i<1 ; i++)
    {
        pthread_join(dbthread[i],NULL);
				    	}

        printf("fret1=%d\n",(int)fret1);
    printf("fret2=%d\n",(int)fret2);
        return 0;
}

